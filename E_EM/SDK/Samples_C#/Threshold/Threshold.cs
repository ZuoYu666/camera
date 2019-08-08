using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MVGigE=MVAPI.MVGigE ;
using MVSTATUS = MVAPI.MVSTATUS_CODES;
using System.Diagnostics;

namespace Threshold
{
    public partial class Threshold : Form
    {
        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;
        IAsyncResult ia = null;
        IntPtr m_hCam = IntPtr.Zero;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
        int m_nThres;
        ToolTip ttpSettings = null;
        IntPtr m_hImage = IntPtr.Zero;
        IntPtr m_hImageDst = IntPtr.Zero;

        MVAPI.MV_SNAPPROC StreamCBDelegate = null;

        public Threshold()
        {
            InitializeComponent();
        }

        int DrawImage()
        {
            if (InvokeRequired)
            {
                if (ia == null)
                {
                    invokeDraw = DrawImage;
                    ia = this.BeginInvoke(invokeDraw);
                }
                else if (ia.IsCompleted)
                {
                    invokeDraw = DrawImage;
                    EndInvoke(ia);
                    ia = this.BeginInvoke(invokeDraw);
                }
                return 0;
            }
            if (m_hImage != IntPtr.Zero)
            {
                int nPitch = MVAPI.MVImage.MVImageGetPitch(m_hImage);
                IntPtr ptrSrc = MVAPI.MVImage.MVImageGetBits(m_hImage);
                IntPtr ptrDst = MVAPI.MVImage.MVImageGetBits(m_hImageDst);

                if (checkBox_Invert.Checked || checkBox_Binary.Checked)
                {
                    if (m_PixelFormat == MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8)
                    {
                        unsafe
                        {
                            byte* pSrc = (byte*)((void*)ptrSrc);
                            byte* pDst = (byte*)((void*)ptrDst);

                            for (int i = 0; i < nPitch * m_nHeight; i++)
                            {
                                if (checkBox_Binary.Checked)
                                {
                                    if (*pSrc > m_nThres)
                                    {
                                        *pDst = 255;
                                    }
                                    else
                                    {
                                        *pDst = 0;
                                    }
                                    if (checkBox_Invert.Checked)
                                    {
                                        *pDst = (byte)(255 - *pDst);
                                    }
                                }
                                else
                                {
                                    *pDst = (byte)(255 - *pSrc);
                                }
                                pDst++;
                                pSrc++;
                            }
                        }                        
                    }
                    else
                    {
                        unsafe
                        {
                            byte* pSrc = (byte*)((void*)ptrSrc);
                            byte* pDst = (byte*)((void*)ptrDst);
                            for (int i = 0; i < nPitch * m_nHeight; i++)
                            {
                                *pDst++ = (byte)(255 - *pSrc++);
                            }
                        }
                    }
                    MVAPI.MVImage.MVImageDrawHwnd(m_hImageDst, this.Handle, 8, 40);
                }
                else
                {
                    MVAPI.MVImage.MVImageDrawHwnd(m_hImage, this.Handle, 8, 40);
                }
            }
            return 0;
        }

        int StreamCB(ref MVAPI.IMAGE_INFO pInfo, IntPtr UserVal)
        {
            MVGigE.MVInfo2Image(m_hCam, ref pInfo, m_hImage);
            DrawImage();
            return 0;
        }

        private void Threshold_Load(object sender, EventArgs e)
        {
            MVGigE.MVInitLib();
            int CamNum = 0;
            MVSTATUS r = MVGigE.MVGetNumOfCameras(out CamNum);
            if (CamNum == 0)
            {
                MessageBox.Show("没有找到相机，请确认连接和相机IP设置");
                return;
            }

            r = MVGigE.MVOpenCamByIndex(0, out m_hCam);
            if (m_hCam == IntPtr.Zero)
            {
                if (r == MVSTATUS.MVST_ACCESS_DENIED)
                {
                    MessageBox.Show("无法打开相机，可能正被别的软件控制");
                    return;
                }
            }

            int w, h;
            r = MVGigE.MVGetWidth(m_hCam, out w);
            if (CamNum == 0)
            {
                MessageBox.Show("取得图像宽度失败");
                return;
            }

            r = MVGigE.MVGetHeight(m_hCam, out h);
            if (CamNum == 0)
            {
                MessageBox.Show("取得图像高度失败");
                return;
            }
            r = MVGigE.MVGetPixelFormat(m_hCam, out m_PixelFormat);
            if (CamNum == 0)
            {
                MessageBox.Show("取得图像颜色模式失败");
                return;
            }
            m_nWidth = w;
            m_nHeight = h;

            if (m_hImage == IntPtr.Zero)
            {
                if (m_PixelFormat == MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8)
                {
                    m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 8);
                    m_hImageDst = MVAPI.MVImage.MVImageCreate(w, h, 8);
                }
                else
                {
                    m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 24);
                    m_hImageDst = MVAPI.MVImage.MVImageCreate(w, h, 24);
                    trackBar_Thres.Enabled = false;
                    textBox_Thres.Enabled = false;
                    checkBox_Binary.Enabled = false;
                }
            }

            ttpSettings = new ToolTip();
            ttpSettings.InitialDelay = 200;
            ttpSettings.AutoPopDelay = 10 * 1000;
            ttpSettings.ReshowDelay = 200;
            ttpSettings.ShowAlways = true;
            ttpSettings.IsBalloon = true;
            InitThres();
        }

        private void ContinueSnap_Click(object sender, EventArgs e)
        {
            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            StopSnap.Enabled = true;
            ContinueSnap.Enabled = false;
        }

        private void StopSnap_Click(object sender, EventArgs e)
        {
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);

            StopSnap.Enabled = false;
            ContinueSnap.Enabled = true;            
        }

        private void Save_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog(this) == DialogResult.OK)
            {
                if (checkBox_Invert.Checked || checkBox_Binary.Checked)
                {
                    MVAPI.MVImage.MVImageSave(m_hImageDst, saveFileDialog1.FileName);                
                }
                else
                {
                    MVAPI.MVImage.MVImageSave(m_hImage, saveFileDialog1.FileName);                
                }                
            }
        }

        private void Threshold_Paint(object sender, PaintEventArgs e)
        {
            if (ContinueSnap.Enabled)
            {
                DrawImage();
            }
        }

        private bool InitThres() 
        {
            this.trackBar_Thres.Minimum = 0;
            this.trackBar_Thres.Maximum = 255;

            this.trackBar_Thres.Value = 128;
            m_nThres = 128;
            this.textBox_Thres.Text = this.trackBar_Thres.Value.ToString();
            string tipOverwrite;
            tipOverwrite = "下限：" + "0" + ",上限：" + "255" ;
            ttpSettings.SetToolTip(this.textBox_Thres, tipOverwrite);
            ttpSettings.SetToolTip(this.trackBar_Thres, tipOverwrite); 

            return true;
        }

        private void trackBar_Thres_Scroll(object sender, EventArgs e)
        {
            m_nThres = this.trackBar_Thres.Value;
            this.textBox_Thres.Text = m_nThres.ToString();

            if (ContinueSnap.Enabled)
            {
                DrawImage();
            }
        }

        private void textBox_Thres_KeyPress(object sender, KeyPressEventArgs e)
        {
            m_nThres = Convert.ToInt32(this.textBox_Thres.Text);

            if (ContinueSnap.Enabled)
            {
                DrawImage();
            }

            if (e.KeyChar == 13)
            {
                this.trackBar_Thres.Value = m_nThres;
            }
        }

        private void checkBox_Binary_CheckedChanged(object sender, EventArgs e)
        {
            if (ContinueSnap.Enabled)
            {
                DrawImage();
            }
        }

        private void checkBox_Invert_CheckedChanged(object sender, EventArgs e)
        {
            if (ContinueSnap.Enabled)
            {
                DrawImage();
            }
        }

    }
}
