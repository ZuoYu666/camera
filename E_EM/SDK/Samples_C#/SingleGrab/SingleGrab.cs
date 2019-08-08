using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MVGigE = MVAPI.MVGigE ;
using MVPro = MVAPI.MVCamProptySheet;
using MVSTATUS = MVAPI.MVSTATUS_CODES;
using MVImage = MVAPI.MVImage;
using System.Diagnostics;

namespace SingleGrab
{
    public partial class SingleGrab : Form
    {
        IntPtr m_hCam = IntPtr.Zero;
        IntPtr m_hPro = IntPtr.Zero;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
   
        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;
        IAsyncResult ia = null;
        IntPtr m_hImage = IntPtr.Zero;

        MVAPI.MV_SNAPPROC StreamCBDelegate = null;

        public SingleGrab()
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
            DrawImage();
            return 0;
        }

        private void SingleGrab_Load(object sender, EventArgs e)
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

            ImageCreat();
            
            MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive);
            if (CamNum == 0)
            {
                MessageBox.Show("设置外闪光同步信号源失败");
            }
            MVAPI.MVCamProptySheet.MVCamProptySheetCreateEx(out m_hPro, m_hCam, this.Handle, "相机属性", MVAPI.MVCamProptySheet.PAGE_ALL); 
        }

        private void CaptureSnap_Click(object sender, EventArgs e)
        {
            ImageCreat();
            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            StopSnap.Enabled = true;
            CaptureSnap.Enabled = false;
            BN_SingleGrab.Enabled = false;
            MVPro.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON);
        }

        private void BN_SingleGrab_Click(object sender, EventArgs e)
        {
            ImageCreat();
            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVPro.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON);
            StopSnap.Enabled = true;
            CaptureSnap.Enabled = false;
            BN_SingleGrab.Enabled = false;

            MVGigE.MVSingleGrab(m_hCam, m_hImage, 1000);
            DrawImage();
            StopSnap.Enabled = false;
            CaptureSnap.Enabled = true;
            BN_SingleGrab.Enabled = true;
            MVPro.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF);
        }

        private void StopSnap_Click(object sender, EventArgs e)
        {
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);

            StopSnap.Enabled = false;
            CaptureSnap.Enabled = true;
            BN_SingleGrab.Enabled = true;
            MVPro.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF);
        }

        private void Save_Click(object sender, EventArgs e)
        {
            int bpp = MVImage.MVImageGetBPP(m_hImage);
            IntPtr pImage = MVImage.MVImageCreate(m_nWidth, m_nHeight, bpp);
            MVGigE.CopyMemory(MVImage.MVImageGetBits(pImage), MVImage.MVImageGetBits(m_hImage), (uint)(m_nWidth * m_nHeight * bpp / 8));
            if (saveFileDialog1.ShowDialog(this) == DialogResult.OK)
            {
                MVImage.MVImageSave(pImage, saveFileDialog1.FileName);
            }
            MVImage.MVImageRelease(pImage);

        }

        private void SingleGrab_Paint(object sender, PaintEventArgs e)
        {
            if (CaptureSnap.Enabled)
            {
                DrawImage();
            }
        }

        private void SingleGrab_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVCloseCam(m_hCam);
            m_hCam = IntPtr.Zero;
            MVGigE.MVTerminateLib();
            MVAPI.MVImage.MVImageRelease(m_hImage);
            MVAPI.MVCamProptySheet.MVCamProptySheetDestroy(m_hPro);
        }

        private void Property_Click(object sender, EventArgs e)
        {
            MVPro.MVCamProptySheetShow(m_hPro, MVAPI.MVShowWindowEnums.SW_SHOW);
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
