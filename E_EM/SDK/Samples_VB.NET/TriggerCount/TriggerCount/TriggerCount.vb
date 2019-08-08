Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Drawing
Imports System.Drawing.Imaging
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports System.Threading
Imports TriggerCount_VB.MVAPI.MVGigE
Imports TriggerCount_VB.MVAPI.MVImage

Imports System.Diagnostics
Imports TriggerCount_VB.MVAPI

''' <summary>
''' 这个例子主要演示如何实现相机的连续采集、软触发、外触发、停止采集、保存图像、控制相机的触发极性、统计相机拍摄数量和收到数量（可以作为判断相机是否丢帧的依据）.
''' </summary>
''' <remarks></remarks>
Public Class TriggerCount

    Public m_hCam As IntPtr = IntPtr.Zero   '相机指针
    Public imgWidth As Integer    '图像宽
    Public imgHeight As Integer   '图像高
    Public m_hImage As IntPtr = IntPtr.Zero      '相机图像变量
    Dim m_nCapture As UInteger    '相机拍摄数量
    Dim m_nCapCntBase As UInteger
    Dim m_nLastID As UShort
    Dim m_nReceive As UInteger    '图像接受数量

    Public StreamCBDelegate As MVAPI.MV_SNAPPROC = Nothing     '声明委托
    '异步编程，用于将图像画到画布上面进行显示
    Public Delegate Function InvokeDraw() As Integer   '定义委托
    Public iDraw As InvokeDraw = Nothing   '声明委托
    Dim ia As IAsyncResult = Nothing    '获取异步操作是否完成的信息

    ''' <summary>
    ''' 窗体加载
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub TriggerCount_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim CamNum As Long = 0
        Dim r As MVAPI.MVSTATUS_CODES
        Dim pf As MVAPI.MV_PixelFormatEnums
    
        '初始化相机函数库
        MVAPI.MVGigE.MVInitLib()
        '查找连接到计算机上的相机
        MVAPI.MVGigE.MVUpdateCameraList()
    
        '获取连接到计算机上的相机个数
        r = MVAPI.MVGigE.MVGetNumOfCameras(CamNum)
        If CamNum = 0 Then
            MsgBox("没有找到相机，请确认相机连接和相机IP设置！")
            Exit Sub
        End If
        '打开相机
        r = MVAPI.MVGigE.MVOpenCamByIndex(0, m_hCam)
        If m_hCam = IntPtr.Zero Then
            If r = MVAPI.MVSTATUS_CODES.MVST_ACCESS_DENIED Then
                MsgBox("无法打开相机，可能正被别的软件控制！")
                Exit Sub
            End If
        End If
        '获取图像宽度
        r = MVAPI.MVGigE.MVGetWidth(m_hCam, imgWidth)
        If CamNum = 0 Then
            MsgBox("取得图像宽度失败！")
            Exit Sub
        End If
        '获取图像高度
        r = MVAPI.MVGigE.MVGetHeight(m_hCam, imgHeight)

        If CamNum = 0 Then
            MsgBox("取得图像高度失败！")
            Exit Sub
        End If
        '获取图像像素格式
        r = MVAPI.MVGigE.MVGetPixelFormat(m_hCam, pf)
        If CamNum = 0 Then
            MsgBox("取得图像像素格式！")
            Exit Sub
        End If


        '设置图像变量的像素格式
        If pf = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8 Then
            m_hImage = MVAPI.MVImage.MVImageCreate(imgWidth, imgHeight, 8)
        Else
            m_hImage = MVAPI.MVImage.MVImageCreate(imgWidth, imgHeight, 24)
        End If
        '设置外闪光同步信号源
        MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive)
        If CamNum = 0 Then
            MsgBox("设置外闪光同步信号源失败！")
            Exit Sub
        End If

        InitTtriggerActivation()
    End Sub
    ''' <summary>
    ''' 窗体关闭事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub TriggerCount_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
            MVSetTriggerSource(m_hCam, MVAPI.TriggerSourceEnums.TriggerSource_Software)
            MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_Off)
            MVSetStrobeInvert(m_hCam, False)
            '软件退出，关闭相机
            MVGigE.MVStopGrab(m_hCam)
            MVCloseCam(m_hCam)
            '退出函数库
            MVTerminateLib()
            '取消委托事件
            Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
            StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
            If m_hImage <> IntPtr.Zero Then
                MVImageRelease(m_hImage)
            End If
            m_hCam = IntPtr.Zero
            m_hImage = IntPtr.Zero
        Else
            e.Cancel = True
        End If
      
    End Sub
    ''' <summary>
    ''' 按钮状态设置
    ''' </summary>
    ''' <param name="bRun"></param>
    ''' <param name="bSoftware"></param>
    ''' <remarks></remarks>
    Private Sub TriggerCount_EnabaleButton(ByVal bRun As Boolean, ByVal bSoftware As Boolean)
        StopSnap.Enabled = bRun
        CaptureSnap.Enabled = Not bRun
        SoftwareSnap.Enabled = (Not bRun) Or bSoftware
        SyncSnap.Enabled = Not bRun
    End Sub
    ''' <summary>
    ''' 初始化下拉框控件
    ''' </summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Private Function InitTtriggerActivation() As Boolean
        Dim edge As MVAPI.TriggerActivationEnums
        Dim idx As Integer

        comboBox_Activation.Items.Add("下降沿")
        comboBox_Activation.Items.Add("上升沿")

        MVGetTriggerActivation(m_hCam, edge)
        idx = 0

        Select Case edge
            Case MVAPI.TriggerActivationEnums.TriggerActivation_FallingEdge
                idx = 0
            Case MVAPI.TriggerActivationEnums.TriggerActivation_RisingEdge
                idx = 1
            Case Else
                MsgBox("Get TriggerActivationEnums Failed!")
                Return False
        End Select
        comboBox_Activation.SelectedIndex = idx
        Return True
    End Function
    ''' <summary>
    ''' 采集按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub CaptureSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CaptureSnap.Click
        Dim fps As Double = 0
        Dim fpsMin As Double = 0
        Dim t As Integer = 0

        m_nReceive = 0
        m_nCapCntBase = 0
        m_nLastID = 0
        Dim mode As MVAPI.TriggerModeEnums
        timer_Update_Frame.Interval = 500
        timer_Update_Frame.Start()
        MVGetTriggerMode(m_hCam, mode)

        If mode <> MVAPI.TriggerModeEnums.TriggerMode_Off Then
            MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
            MVGetFrameRateRange(m_hCam, fpsMin, fps)
            t = CInt(1500 / fps)
            Thread.Sleep(t)
        End If

        '创建事件接受者，所做的事情
        '利用代理将对象及其方法注册进事件
        '启用delegate访问
        '指出方法委托去哪（call me），并开启委托
        '为StreamCBDelegate委托注册StreamCB方法
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB) '实例化
        '开始采集
        Dim r As MVSTATUS_CODES = MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)
        TriggerCount_EnabaleButton(True, False)
    End Sub

    '接受委托结果，并处理
    Function StreamCB(ByRef pInfo As IMAGE_INFO, ByVal UserVal As IntPtr) As Integer
        Dim id As UShort = pInfo.nBlockId

        If id < m_nLastID Then
            StopSnap.Text = m_nLastID.ToString
            If ((id < 100) And (m_nLastID > 60000)) Then
                m_nCapCntBase += 65535
            Else
                Save.Text = id.ToString()
            End If
        End If
        m_nCapture = m_nCapCntBase + id
        m_nReceive = m_nReceive + 1
        '将原始帧转化为m_hImage格式图像
        MVGigE.MVInfo2Image(m_hCam, pInfo, m_hImage)
        DrawImage()
        Return 0
    End Function
    ''' <summary>
    ''' 停止按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub StopSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopSnap.Click
        timer_Update_Frame.Stop()
        '停止连续采集
        MVAPI.MVGigE.MVStopGrab(m_hCam)
        '取消委托事件
        Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
        StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)

        TriggerCount_EnabaleButton(False, False)

        textBox_Capture.Text = m_nCapture.ToString()
        textBox_Receive.Text = m_nReceive.ToString()
    End Sub
    ''' <summary>
    ''' 画图模块
    ''' </summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Function DrawImage() As Integer

        If (InvokeRequired) Then
            If (ia Is Nothing) Then
                iDraw = AddressOf DrawImage
                ia = Me.BeginInvoke(iDraw)   '执行委托，参数类型与个数应与声明一致  
            ElseIf (ia.IsCompleted) Then
                iDraw = AddressOf DrawImage
                EndInvoke(ia)
                ia = Me.BeginInvoke(iDraw)
            End If
            Return 0
        End If

        If m_hImage <> IntPtr.Zero Then
            '将m_hImage图像画到me.Handle画布上面
            MVAPI.MVImage.MVImageDrawHwnd(m_hImage, Me.Handle, 8, 40)
        End If
        Return 0
    End Function
    ''' <summary>
    ''' 窗体初始化
    ''' </summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Public Function TriggerCount()
        InitializeComponent()
        Return 0
    End Function

    ''' <summary>
    ''' 软触发按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub SoftwareSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SoftwareSnap.Click
        Dim mode As MVAPI.TriggerModeEnums
        Dim fps As Double = 0
        Dim fpsMin As Double = 0
        Dim t As Integer = 0


        If (StopSnap.Enabled = False) Then

            m_nReceive = 0
            m_nCapCntBase = 0
            m_nLastID = 0
            timer_Update_Frame.Interval = 500
            timer_Update_Frame.Start()
            MVAPI.MVGigE.MVGetTriggerMode(m_hCam, mode)

            If (mode <> MVAPI.TriggerModeEnums.TriggerMode_On) Then
                MVAPI.MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_On)
                MVAPI.MVGigE.MVGetFrameRateRange(m_hCam, fpsMin, fps)
                t = CInt(1500 / fps)
                Thread.Sleep(t)
            End If
            MVAPI.MVGigE.MVSetTriggerSource(m_hCam, MVAPI.TriggerSourceEnums.TriggerSource_Software)

            StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
            Dim r As MVAPI.MVSTATUS_CODES = MVAPI.MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)
            TriggerCount_EnabaleButton(True, True)
        End If
        MVAPI.MVGigE.MVTriggerSoftware(m_hCam)
    End Sub
    ''' <summary>
    ''' 外触发按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub SyncSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SyncSnap.Click
        Dim mode As MVAPI.TriggerModeEnums
        Dim fps As Double = 0
        Dim fpsMin As Double = 0
        Dim t As Integer = 0

        m_nReceive = 0
        m_nCapCntBase = 0
        m_nLastID = 0
        timer_Update_Frame.Interval = 500
        timer_Update_Frame.Start()

        MVAPI.MVGigE.MVGetTriggerMode(m_hCam, mode)
        If (mode <> MVAPI.TriggerModeEnums.TriggerMode_On) Then
            MVAPI.MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_On)
            MVAPI.MVGigE.MVGetFrameRateRange(m_hCam, fpsMin, fps)
            t = CInt(1500 / fps)
            Thread.Sleep(t)
        End If
        MVAPI.MVGigE.MVSetTriggerSource(m_hCam, MVAPI.TriggerSourceEnums.TriggerSource_Line1)
        MVAPI.MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive)
        MVAPI.MVGigE.MVSetStrobeInvert(m_hCam, True)

        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)

        Dim r As MVAPI.MVSTATUS_CODES = MVAPI.MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)

        TriggerCount_EnabaleButton(True, False)

    End Sub
    ''' <summary>
    ''' 保存按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub Save_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Save.Click
        If (SaveFileDialog1.ShowDialog(Me) = DialogResult.OK) Then
            MVAPI.MVImage.MVImageSave(m_hImage, SaveFileDialog1.FileName)
        End If
    End Sub
    ''' <summary>
    ''' 窗体重绘事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub TriggerCount_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        If (StopSnap.Enabled = False) Then
            DrawImage()
        End If
    End Sub
    ''' <summary>
    ''' 下拉框内容更新事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub comboBox_Activation_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles comboBox_Activation.SelectedIndexChanged
        Dim idx As Integer = comboBox_Activation.SelectedIndex
        Select Case idx

            Case 0
                MVAPI.MVGigE.MVSetTriggerActivation(m_hCam, MVAPI.TriggerActivationEnums.TriggerActivation_FallingEdge)

            Case 1
                MVAPI.MVGigE.MVSetTriggerActivation(m_hCam, MVAPI.TriggerActivationEnums.TriggerActivation_RisingEdge)

            Case Else
                MessageBox.Show("Set TriggerActivationEnums Failed!")
        End Select
    End Sub
    ''' <summary>
    ''' 定时器事件，更新相机采集帧数
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub timer_Update_Frame_Tick(ByVal sender As Object, ByVal e As System.EventArgs) Handles timer_Update_Frame.Tick
        textBox_Capture.Text = m_nCapture.ToString()
        textBox_Receive.Text = m_nReceive.ToString()
    End Sub
End Class