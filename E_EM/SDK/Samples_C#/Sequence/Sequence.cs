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

namespace Sequence
{
    public partial class Sequence : Form
    {
        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;
        IAsyncResult ia = null;
        IntPtr m_hCam = IntPtr.Zero;
        IntPtr m_hSeq = IntPtr.Zero;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
        IntPtr m_hImage = IntPtr.Zero;
        IntPtr m_hPro = IntPtr.Zero;

        MVAPI.MV_SNAPPROC StreamCBDelegate = null;

        public Sequence()
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
                MVAPI.MVImage.MVImageDrawHwnd(m_hImage, this.Handle, 8, 40);
            }
            return 0;
        }

        int StreamCB(ref MVAPI.IMAGE_INFO pInfo, IntPtr UserVal)
        {
            MVGigE.MVInfo2Image(m_hCam, ref pInfo, m_hImage);
            StringBuilder strPath = new StringBuilder(260);
            if (MVAPI.MVSequenceDlg.MVSequenceDlgGetFileName(m_hSeq, strPath, 260) == 0)
            {
                SaveImage(strPath.ToString());
            }
            DrawImage();
            return 0;
        }

        private void Sequence_Load(object sender, EventArgs e)
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

            ImageCreat();

            MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive);
            if (CamNum == 0)
            {
                MessageBox.Show("设置外闪光同步信号源失败");
            }
            MVAPI.MVSequenceDlg.MVSequenceDlgCreateEx(out m_hSeq, this.Handle);
            MVAPI.MVCamProptySheet.MVCamProptySheetCreateEx(out m_hPro, m_hCam, this.Handle, "相机属性", MVAPI.MVCamProptySheet.PAGE_ALL);
        }

        private void CaptureSnap_Click(object sender, EventArgs e)
        {
            ImageCreat();

            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            StopSnap.Enabled = true;
            CaptureSnap.Enabled = false;
            MVAPI.MVSequenceDlg.MVSequenceDlgCamRun(m_hSeq, true);
            MVAPI.MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON);
        }

        private void StopSnap_Click(object sender, EventArgs e)
        {
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);
            StopSnap.Enabled = false;
            CaptureSnap.Enabled = true;
            MVAPI.MVSequenceDlg.MVSequenceDlgCamRun(m_hSeq, false);
            MVAPI.MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF);
        }

        private void Save_Click(object sender, EventArgs e)
        {

            if (saveFileDialog1.ShowDialog(this) == DialogResult.OK)
            {
                MVAPI.MVImage.MVImageSave(m_hImage, saveFileDialog1.FileName); 
            }
        }

        public void SaveImage(string strPath)
        {
            MVAPI.MVImage.MVImageSave(m_hImage,strPath);            
        }

        private void Sequence_Paint(object sender, PaintEventArgs e)
        {
            if (CaptureSnap.Enabled)
            {
              DrawImage();
            }
        }

        private void Sequence_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVCloseCam(m_hCam);
            m_hCam = IntPtr.Zero;
            MVAPI.MVSequenceDlg.MVSequenceDlgDestroy(m_hSeq);
            MVAPI.MVImage.MVImageRelease(m_hImage);
            m_hSeq = IntPtr.Zero;
            MVAPI.MVCamProptySheet.MVCamProptySheetDestroy(m_hPro);
            m_hPro = IntPtr.Zero;
        }

        private void Sequence_Control_Click(object sender, EventArgs e)
        {
            MVAPI.MVSequenceDlg.MVSequenceDlgShow(m_hSeq, MVAPI.MVShowWindowEnums.SW_SHOW);

        }

        private void BN_Property_Click(object sender, EventArgs e)
        {
            MVAPI.MVCamProptySheet.MVCamProptySheetShow(m_hPro, MVAPI.MVShowWindowEnums.SW_SHOW);
        }
        private void ImageCreat()
        {
            int w = 0, h = 0;

            MVSTATUS r = MVGigE.MVGetWidth(m_hCam, out w);
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
            if (m_nWidth != w || m_nHeight != h)
            {
                m_nWidth = w;
                m_nHeight = h;

                if (m_hImage != IntPtr.Zero)
                {
                    MVAPI.MVImage.MVImageRelease(m_hImage);
                    m_hImage = IntPtr.Zero;
                }

                if (m_PixelFormat == MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8)
                {
                    m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 8);
                }
                else
                {
                    m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 24);
                }
                Refresh();
            }
        }
     }
}
