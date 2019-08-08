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
using System.Threading;

namespace TriggerCount
{
    public partial class TriggerCount : Form
    {


        IntPtr m_hImage = IntPtr.Zero;
        IntPtr m_hCam=IntPtr.Zero;
        MVAPI.MV_PixelFormatEnums m_PixelFormat;
        int m_nWidth;
        int m_nHeight;
        UInt32 m_nCapture;
        UInt32 m_nCapCntBase;
        UInt16 m_nLastID;
        UInt32 m_nReceive;

        MVAPI.MV_SNAPPROC StreamCBDelegate = null;

        public delegate int InvokeDraw();
        InvokeDraw invokeDraw = null;

        IAsyncResult ia = null;

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
        public TriggerCount()
        {
            InitializeComponent();
        }

        int StreamCB(ref MVAPI.IMAGE_INFO pInfo, IntPtr UserVal)
        {
            UInt16 id = pInfo.nBlockId;
            if (id < m_nLastID)
            {
                StopSnap.Text = m_nLastID.ToString();

                if ((id < 100) && (m_nLastID > 60000))
                {
                    m_nCapCntBase += 65535;
                }
                else
                {
                    Save.Text = id.ToString();
                }
            }
            m_nLastID = id;
            m_nCapture = m_nCapCntBase + id;
            m_nReceive++;

            MVGigE.MVInfo2Image(m_hCam, ref pInfo, m_hImage);
            DrawImage();
            return 0;
        }

        private void TriggerCount_Load(object sender, EventArgs e)
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
            if (m_PixelFormat == MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8)
                m_hImage = MVAPI.MVImage.MVImageCreate(m_nWidth, m_nHeight, 8);
            else
                m_hImage = MVAPI.MVImage.MVImageCreate(m_nWidth, m_nHeight, 24);

            MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive);
            if (CamNum == 0)
            {
                MessageBox.Show("设置外闪光同步信号源失败");
            }
            InitTriggerActivation();
        }

        private void CaptureSnap_Click(object sender, EventArgs e)
        {
            m_nReceive = 0;
            m_nCapCntBase = 0;
            m_nLastID = 0;
            MVAPI.TriggerModeEnums mode;
            timer_Update_Frame.Interval = 500;
            timer_Update_Frame.Start();
            MVGigE.MVGetTriggerMode(m_hCam, out mode);
            if (mode != MVAPI.TriggerModeEnums.TriggerMode_Off)
            {
                double fps = 0;
                double fpsMin = 0;
                int t = 0;
                MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
                MVGigE.MVGetFrameRateRange(m_hCam, out fpsMin, out fps);
                t = (int)(1500 / fps);
                Thread.Sleep(t);
            }

            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVSTATUS r = MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            TriggerCount_EnabaleButton(true, false);
        }

        private void SoftwareSnap_Click(object sender, EventArgs e)
        {
            if (StopSnap.Enabled == false)
            {
                m_nReceive = 0;
                m_nCapCntBase = 0;
                m_nLastID = 0;
                MVAPI.TriggerModeEnums mode;
                timer_Update_Frame.Interval = 500;
                timer_Update_Frame.Start();
                MVGigE.MVGetTriggerMode(m_hCam, out mode);
                if (mode != MVAPI.TriggerModeEnums.TriggerMode_On)
                {
                    double fps = 0;
                    double fpsMin = 0;
                    int t = 0;

                    MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_On);
                    MVGigE.MVGetFrameRateRange(m_hCam, out fpsMin, out fps);
                    t = (int)(1500 / fps);
                    Thread.Sleep(t);
                }
                MVGigE.MVSetTriggerSource(m_hCam, MVAPI.TriggerSourceEnums.TriggerSource_Software);

                StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
                MVSTATUS r = MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

                TriggerCount_EnabaleButton(true, true);
            }
            MVGigE.MVTriggerSoftware(m_hCam);
        }
        private void SyncSnap_Click(object sender, EventArgs e)
        {
            m_nReceive = 0;
            m_nCapCntBase = 0;
            m_nLastID = 0;
            MVAPI.TriggerModeEnums mode;
            timer_Update_Frame.Interval = 500;
            timer_Update_Frame.Start();
            MVGigE.MVGetTriggerMode(m_hCam, out mode);
            if (mode != MVAPI.TriggerModeEnums.TriggerMode_On)
            {
                double fps = 0;
                double fpsMin = 0;
                int t = 0;

                MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_On);
                MVGigE.MVGetFrameRateRange(m_hCam, out fpsMin, out fps);
                t = (int)(1500 / fps);
                Thread.Sleep(t);
            }
            MVGigE.MVSetTriggerSource(m_hCam, MVAPI.TriggerSourceEnums.TriggerSource_Line1);
            MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive);
            MVGigE.MVSetStrobeInvert(m_hCam, true);

            StreamCBDelegate += new MVAPI.MV_SNAPPROC(StreamCB);
            MVSTATUS r = MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, this.Handle);

            TriggerCount_EnabaleButton(true, false);
        }

        private void StopSnap_Click(object sender, EventArgs e)
        {
            timer_Update_Frame.Stop();
            MVGigE.MVStopGrab(m_hCam);
            StreamCBDelegate -= new MVAPI.MV_SNAPPROC(StreamCB);
            TriggerCount_EnabaleButton(false, false);

            textBox_Capture.Text = m_nCapture.ToString();
            textBox_Receive.Text = m_nReceive.ToString();           
        }

        private void Save_Click(object sender, EventArgs e)
        {

            if (saveFileDialog1.ShowDialog(this) == DialogResult.OK)
            {
                MVAPI.MVImage.MVImageSave(m_hImage, saveFileDialog1.FileName);                
            }
        }

        private void TriggerCount_Paint(object sender, PaintEventArgs e)
        {
            if (!StopSnap.Enabled)
            {
                DrawImage();
            }
        }

        private void TriggerCount_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVGigE.MVStopGrab(m_hCam);
            MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off);
            MVGigE.MVSetTriggerSource(m_hCam, MVAPI.TriggerSourceEnums.TriggerSource_Software);
            MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_Off);
            MVGigE.MVSetStrobeInvert(m_hCam, false);
            MVGigE.MVCloseCam(m_hCam);
            MVGigE.MVTerminateLib();
            m_hCam = IntPtr.Zero;

            MVAPI.MVImage.MVImageDestroy(m_hImage);
            m_hImage = IntPtr.Zero;
        }

        private void TriggerCount_EnabaleButton( bool bRun, bool bSoftware )
        {

            StopSnap.Enabled = bRun;
            CaptureSnap.Enabled = !bRun;
            SoftwareSnap.Enabled = !bRun || bSoftware;
            SyncSnap.Enabled = !bRun;  
        }

        private bool InitTriggerActivation()
        {

            comboBox_Activation.Items.Add("下降沿");
            comboBox_Activation.Items.Add("上升沿");

            MVAPI.TriggerActivationEnums edge;
            int idx = 0;
            MVGigE.MVGetTriggerActivation(m_hCam, out edge);
            switch (edge)
            {
                case MVAPI.TriggerActivationEnums.TriggerActivation_FallingEdge:
                    idx = 0;
                    break;
                case MVAPI.TriggerActivationEnums.TriggerActivation_RisingEdge:
                    idx = 1;
                    break;
                default:
                    MessageBox.Show("Get TriggerActivationEnums Failed!");
                    return false;
            }
            comboBox_Activation.SelectedIndex = idx;
            return true;
        }

        private void comboBox_Activation_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = comboBox_Activation.SelectedIndex;
            switch (idx)
            {
                case 0:
                    MVGigE.MVSetTriggerActivation(m_hCam, MVAPI.TriggerActivationEnums.TriggerActivation_FallingEdge);
                    break;
                case 1:
                    MVGigE.MVSetTriggerActivation(m_hCam, MVAPI.TriggerActivationEnums.TriggerActivation_RisingEdge);
                    break;
                default:
                    MessageBox.Show("Set TriggerActivationEnums Failed!");
                    break;
            }
        }

        private void timer_Update_Frame_Tick(object sender, EventArgs e)
        {
            textBox_Capture.Text = m_nCapture.ToString();
            textBox_Receive.Text = m_nReceive.ToString();
        }
    }
}
