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
using MVImage=MVAPI.MVImage ;
using MVSTATUS = MVAPI.MVSTATUS_CODES;
using System.Diagnostics;

namespace Cross
{
    public partial class Cross : Form
    {
        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;
        IntPtr m_hCam = IntPtr.Zero;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
        IntPtr m_hImage = IntPtr.Zero;
        IntPtr m_hImageFlip = IntPtr.Zero;
        IntPtr m_hImageRotate = IntPtr.Zero;
        IntPtr m_hImageShow = IntPtr.Zero;
        int m_nZoom = 1;
        int m_nFlip = 0;
        int m_nRotate = 0;
        IAsyncResult ia = null;
        MVAPI.MV_SNAPPROC StreamCBDelegate = null;


        public Cross()
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
            IntPtr hImage = IntPtr.Zero;

            int w, h;
            IntPtr hdc = IntPtr.Zero;
            if (m_nRotate > 0)
            {
                w = m_nHeight/m_nZoom;
                h = m_nWidth / m_nZoom;
                m_hImageShow = m_hImageRotate;
            }
            else
            {
                h = m_nHeight / m_nZoom;
                w = m_nWidth / m_nZoom;
                if (m_nFlip > 0)
                    m_hImageShow = m_hImageFlip;
                else
                    m_hImageShow = m_hImage;
            }
            hImage = MVImage.MVImageCreate(w, h, 24);
            hdc = MVImage.MVImageGetDC(hImage);
            MVImage.MVImageDrawEx(m_hImageShow, hdc, 0, 0, w, h, 0, 0, MVImage.MVImageGetWidth(m_hImageShow), MVImage.MVImageGetHeight(m_hImageShow));
            Graphics g = Graphics.FromHdcInternal(hdc);

            g.DrawLine(new Pen(Color.Green), w / 2, 0, w / 2, h);//坐标
            g.DrawLine(new Pen(Color.Green), 0, h / 2, w, h/ 2);//坐标
            MVImage.MVImageDrawHwnd(hImage, this.Handle, 8, 40);
            MVImage.MVImageReleaseDC(hImage);
            MVImage.MVImageRelease(hImage);
            g.Dispose();
            return 0;
        }

        int ShowImage(ref MVAPI.IMAGE_INFO pInfo)
        {
            MVGigE.MVInfo2Image(m_hCam, ref pInfo, m_hImage);
            Flip();
            Rotate();
            return DrawImage();

        }

        int StreamCB(ref MVAPI.IMAGE_INFO pInfo, IntPtr UserVal)
        {
            return ShowImage(ref pInfo);
        }

        private void Cross_Load(object sender, EventArgs e)
        {
            MVGigE.MVInitLib();
            MVGigE.MVUpdateCameraList();
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
            if (r != MVSTATUS.MVST_SUCCESS)
            {
                MessageBox.Show("取得图像宽度失败");
                return;
            }

            r = MVGigE.MVGetHeight(m_hCam, out h);
            if (r != MVSTATUS.MVST_SUCCESS)
            {
                MessageBox.Show("取得图像高度失败");
                return;
            }
            r = MVGigE.MVGetPixelFormat(m_hCam, out m_PixelFormat);
            if (r != MVSTATUS.MVST_SUCCESS)
            {
                MessageBox.Show("取得图像颜色模式失败");
                return;
            }
            m_nWidth = w;
            m_nHeight = h;

            if (m_PixelFormat == MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8)
            {
                m_hImage = MVImage.MVImageCreate(w, h, 8);
                m_hImageFlip = MVImage.MVImageCreate(w, h, 8);
                m_hImageRotate = MVImage.MVImageCreate(h, w, 8);
            }
            else
            {
                m_hImage = MVImage.MVImageCreate(w, h, 24);
                m_hImageFlip = MVImage.MVImageCreate(w, h, 24);
                m_hImageRotate = MVImage.MVImageCreate(h, w, 24);
            }

            r = MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive);
            if (r != MVSTATUS.MVST_SUCCESS)
            {
                MessageBox.Show("设置外闪光同步信号源失败");
            }
            InitZoom();
            InitFlip();
            InitRotate();
        }

        private void CaptureSnap_Click(object sender, EventArgs e)
        {
            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            StopSnap.Enabled = true;
            CaptureSnap.Enabled = false;
        }


        private void StopSnap_Click(object sender, EventArgs e)
        {
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);

            StopSnap.Enabled = false;
            CaptureSnap.Enabled = true;
        }

        private void Save_Click(object sender, EventArgs e)
        {

            if (saveFileDialog1.ShowDialog(this) == DialogResult.OK)
            {
                MVImage.MVImageSave(m_hImageShow, saveFileDialog1.FileName);
            }
        }

        private void Cross_Paint(object sender, PaintEventArgs e)
        {
            if (CaptureSnap.Enabled&&m_hImage !=IntPtr.Zero)
            {
                DrawImage();
            }
        }

        private void Cross_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVCloseCam(m_hCam);
            MVImage.MVImageRelease(m_hImage);
            MVImage.MVImageRelease(m_hImageFlip);
            MVImage.MVImageRelease(m_hImageRotate);

            m_hCam = IntPtr.Zero;
        }

        private void InitZoom()
        {
            comboBox_Zoom.Items.Add("25%");
            comboBox_Zoom.Items.Add("50%");
            comboBox_Zoom.Items.Add("100%");
            comboBox_Zoom.SelectedIndex = 2;
            m_nZoom = 1;
        }

        private void InitFlip()
        {
            comboBox_Flip.Items.Add("不翻转");
            comboBox_Flip.Items.Add("左右翻转");
            comboBox_Flip.Items.Add("上下翻转");
            comboBox_Flip.Items.Add("同时翻转");
            comboBox_Flip.SelectedIndex = 0;
        }

        private void InitRotate()
        {
            comboBox_Rotate.Items.Add("不旋转");
            comboBox_Rotate.Items.Add("顺时针90°");
            comboBox_Rotate.Items.Add("逆时针90°");
            comboBox_Rotate.SelectedIndex = 0;
        }

        private void comboBox_Flip_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_nFlip = comboBox_Flip.SelectedIndex;

            if (CaptureSnap.Enabled)
            {
                Flip();
                Rotate();
                this.Refresh();
            }            
        }

        private void comboBox_Rotate_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_nRotate = comboBox_Rotate.SelectedIndex;

            if (CaptureSnap.Enabled)
            {
                Rotate();
            }
            this.Refresh();
        }

        private void comboBox_Zoom_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (comboBox_Zoom.SelectedIndex)
            {
                case 0:
                    m_nZoom = 4;
                    break;
                case 1:
                    m_nZoom = 2;
                    break;
                case 2:
                    m_nZoom = 1;
                    break;
                default:
                    m_nZoom = 1;
                    break;
            }
            this.Refresh();
        }

        private void Flip()
        {
            int nFlip = m_nFlip;
            switch (nFlip)
            {
                case 0:
                    break;
                case 1:
                    MVGigE.MVImageFlip(m_hCam, m_hImage, m_hImageFlip, MVAPI.ImageFlipType.FlipHorizontal);
                    break;
                case 2:
                    MVGigE.MVImageFlip(m_hCam, m_hImage, m_hImageFlip, MVAPI.ImageFlipType.FlipVertical);
                    break;
                case 3:
                    MVGigE.MVImageFlip(m_hCam, m_hImage, m_hImageFlip, MVAPI.ImageFlipType.FlipBoth);
                    break;
                default:
                    break;
            }
        }

        private void Rotate()
        {
            int nRotate = m_nRotate;
            if (nRotate == 0)
            {
                return;
            }
            IntPtr hImage = m_hImage;
            if (m_nFlip != 0)
            {
                hImage = m_hImageFlip;
            }
            switch (nRotate)
            {
                case 1:
                    MVGigE.MVImageRotate(m_hCam, hImage, m_hImageRotate, MVAPI.ImageRotateType.Rotate90DegCw);
                    break;
                case 2:
                    MVGigE.MVImageRotate(m_hCam, hImage, m_hImageRotate, MVAPI.ImageRotateType.Rotate90DegCcw);
                    break;
                default:
                    break;
            }
        }
    }
}
