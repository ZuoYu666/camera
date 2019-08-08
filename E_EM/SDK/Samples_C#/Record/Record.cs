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

namespace Record
{
    public partial class Record : Form
    {
        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;
        IAsyncResult ia = null;
        IntPtr m_hCam = IntPtr.Zero;
        IntPtr m_hRecDlg = IntPtr.Zero;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
        IntPtr m_hImage = IntPtr.Zero;

        MVAPI.MV_SNAPPROC StreamCBDelegate = null;

        public Record()
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
            int r = MVAPI.MVRecordDlg.MVRecordDlgRecord(m_hRecDlg, m_hImage, pInfo.nBlockId);
            DrawImage();
            return 0;
        }

        private void Record_Load(object sender, EventArgs e)
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
                }
                else
                {
                    m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 24);
                }
            }

            MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive);
            if (CamNum == 0)
            {
                MessageBox.Show("设置外闪光同步信号源失败");
            }
            MVAPI.MVRecordDlg.MVRecordDlgCreateEx(out m_hRecDlg, m_hCam, this.Handle);
        }

        private void CaptureSnap_Click(object sender, EventArgs e)
        {
            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            MVAPI.MVRecordDlg.MVRecordDlgCamRun(m_hRecDlg, true);
            StopSnap.Enabled = true;
            CaptureSnap.Enabled = false;
        }

        private void StopSnap_Click(object sender, EventArgs e)
        {
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);

            MVAPI.MVRecordDlg.MVRecordDlgCamRun(m_hRecDlg, false);

            StopSnap.Enabled = false;
            CaptureSnap.Enabled = true;
        }

        private void Save_Click(object sender, EventArgs e)
        {

            if (saveFileDialog1.ShowDialog(this) == DialogResult.OK)
            {
                MVAPI.MVImage.MVImageSave(m_hImage,saveFileDialog1.FileName);
            }
        }

        private void Record_Paint(object sender, PaintEventArgs e)
        {
            if (CaptureSnap.Enabled&&m_hImage != IntPtr.Zero)
            {

            }
        }

        private void Record_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVCloseCam(m_hCam);
            MVGigE.MVTerminateLib();
            MVAPI.MVRecordDlg.MVRecordDlgDestroy(m_hRecDlg);
            m_hRecDlg = IntPtr.Zero;

            MVAPI.MVImage.MVImageRelease(m_hImage);
            m_hImage = IntPtr.Zero;

            m_hCam = IntPtr.Zero;
        }

        private void Record_Control_Click(object sender, EventArgs e)
        {
            MVAPI.MVRecordDlg.MVRecordDlgShow(m_hRecDlg, MVAPI.MVShowWindowEnums.SW_SHOW);
        }
    }
}

