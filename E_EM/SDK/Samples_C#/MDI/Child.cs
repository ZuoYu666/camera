using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MVGigE = MVAPI.MVGigE;
using MVSTATUS = MVAPI.MVSTATUS_CODES;
using MVPro = MVAPI.MVCamProptySheet;
using MVImage = MVAPI.MVImage;
using System.Diagnostics;
using System.Threading;

namespace MDI
{
    public partial class Child : Form
    {
        IntPtr m_hImage = IntPtr.Zero;
        IntPtr m_hCam = IntPtr.Zero;
        IntPtr m_hPro = IntPtr.Zero;
        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;
        public byte m_nCam = 0;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
        MVAPI.MV_SNAPPROC StreamCBDelegate = null;
        IAsyncResult ia = null;
        public bool m_bRun = false;

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
            int x = this.AutoScrollPosition.X ;
            int y = this.AutoScrollPosition.Y;
            MVImage.MVImageDrawHwnd(m_hImage, this.Handle, x, y);            
            return 0;
        }

        int Str.
            ntPtr UserVal)
        {
            MVGigE.MVInfo2Image(m_hCam, ref pInfo, m_hImage);

            return DrawImage();
        }

        public Child()
        {
            InitializeComponent();
        }

        private void Child_Load(object sender, EventArgs e)
        {
            MVAPI.MVSTATUS_CODES r;
            r = MVGigE.MVOpenCamByIndex(m_nCam, out m_hCam);
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

            MVPro.MVCamProptySheetCreateEx(out m_hPro, m_hCam, this.Handle, this.Text, MVPro.PAGE_ALL);
        }

        private void Child_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVPro.MVCamProptySheetDestroy(m_hPro);
            MVGigE.MVCloseCam(m_hCam);
            MVImage.MVImageDestroy(m_hImage);

            ((MDI)(this.MdiParent)).Remove(Text);
        }

        public void CaptureSnap()
        {
            ImageCreat();
            m_bRun = true;
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVSetStrobeInvert(m_hCam, true);

            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVSTATUS r = MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);
            MVPro.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON);
        }

        public void StopSnap()
        {
            m_bRun = false;
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);
            MVPro.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF);
        }

        public void Property()
        {
            MVPro.MVCamProptySheetShow(m_hPro, MVAPI.MVShowWindowEnums.SW_SHOW);
        }

        public void Save()
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

        private void Child_Paint(object sender, PaintEventArgs e)
        {
            if (!m_bRun)
            {
                DrawImage();
            }
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
