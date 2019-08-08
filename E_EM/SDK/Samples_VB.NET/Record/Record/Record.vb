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
Imports Record_VB.MVAPI.MVGigE
Imports Record_VB.MVAPI.MVImage

Imports System.Diagnostics
Imports Record_VB.MVAPI
''' <summary>
''' 这个例子主要实现了相机的连续采集、停止、录像控制（按照设定视频格式、时间格式等）功能。 
''' </summary>
''' <remarks></remarks>
Public Class Record
    Public hCam As IntPtr = IntPtr.Zero     '相机指针
    Dim m_hRecDlg As IntPtr = IntPtr.Zero   '录像计时器对话框句柄
  
    Public imgWidth As Integer    '图像宽
    Public imgHeight As Integer   '图像高
    Public hImage As IntPtr = IntPtr.Zero    '图像指针
    '相机采集帧数
    Dim m_nCapture As UInteger
    Dim m_nCapCntBase As UInteger
    Dim m_nLastID As UShort
    Dim m_nReceive As UInteger

    '采集图像数据委托
    Public StreamCBDelegate As MVAPI.MV_SNAPPROC = Nothing   '声明委托
    '异步编程，用于将图像画到画布上面进行显示
    Public Delegate Function InvokeDraw() As Integer '定义委托
    Public iDraw As InvokeDraw = Nothing  '声明委托
    Dim ia As IAsyncResult = Nothing   '获取异步操作是否完成的信息

#Region "窗体事件"
    '窗体关闭事件
    Private Sub Record_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            '关闭相机触发
            MVSetTriggerMode(hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
            '关掉相机
            MVCloseCam(hCam)
            '释放相机资源
            MVTerminateLib()
            If m_hRecDlg <> IntPtr.Zero Then MVRecordDlg.MVRecordDlgDestroy(m_hRecDlg)
            m_hRecDlg = IntPtr.Zero
            If hImage <> IntPtr.Zero Then MVAPI.MVImage.MVImageDestroy(hImage)
            hImage = IntPtr.Zero
            hCam = IntPtr.Zero
            '取消委托事件
            Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
            StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Else
            e.Cancel = True
        End If
      

    End Sub

    '窗体加载事件
    Private Sub Record_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim CamNum As Long = 0
        Dim r As MVAPI.MVSTATUS_CODES
        Dim w As Long
        Dim h As Long
        Dim pf As MVAPI.MV_PixelFormatEnums
        '初始化相机函数库
        MVAPI.MVGigE.MVInitLib()
        '查找连接到计算机的相机
        MVAPI.MVGigE.MVUpdateCameraList()

        '获取连接到计算机的像素个数
        r = MVAPI.MVGigE.MVGetNumOfCameras(CamNum)
        If CamNum = 0 Then
            MsgBox("没有找到相机，请确认相机连接和相机IP设置！")
            Exit Sub
        End If
        '打开相机
        r = MVAPI.MVGigE.MVOpenCamByIndex(0, hCam)
        If hCam = IntPtr.Zero Then
            If r = MVAPI.MVSTATUS_CODES.MVST_ACCESS_DENIED Then
                MsgBox("无法打开相机，可能正被别的软件控制！")
                Exit Sub
            End If
        End If
        '获取图像宽度
        r = MVAPI.MVGigE.MVGetWidth(hCam, w)
        If CamNum = 0 Then
            MsgBox("取得图像宽度失败！")
            Exit Sub
        End If

        r = MVAPI.MVGigE.MVGetHeight(hCam, h)
        '获取图像高度
        If CamNum = 0 Then
            MsgBox("取得图像高度失败！")
            Exit Sub
        End If
        '获取图像像素格式
        r = MVAPI.MVGigE.MVGetPixelFormat(hCam, pf)
        If CamNum = 0 Then
            MsgBox("取得图像像素格式失败！")
            Exit Sub
        End If

        imgWidth = w
        imgHeight = h
        If pf = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8 Then
            hImage = MVAPI.MVImage.MVImageCreate(imgWidth, imgHeight, 8)
        Else
            hImage = MVAPI.MVImage.MVImageCreate(imgWidth, imgHeight, 24)
        End If
        '设置外闪光
        MVSetStrobeSource(hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive)
        If CamNum = 0 Then
            MsgBox("设置外闪光同步信号源失败！")
            Exit Sub
        End If
        '创建录像计时器对话框
        MVAPI.MVRecordDlg.MVRecordDlgCreateEx(m_hRecDlg, hCam, Me.Handle)
    End Sub
    '按钮状态设置
    Private Sub Button_Enabale(ByVal bRun As Boolean)
        StopSnap.Enabled = bRun
        CaptureSnap.Enabled = Not bRun
    End Sub
    '“采集”按钮点击事件
    Private Sub CaptureSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CaptureSnap.Click
        '为StreamCBDelegate委托注册StreamCB方法
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB) '实例化
        MVGigE.MVSetTriggerMode(hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)  '关闭
        MVGigE.MVStartGrab(hCam, StreamCBDelegate, Me.Handle)   '开始采集

        MVAPI.MVRecordDlg.MVRecordDlgCamRun(m_hRecDlg, True)    '设置相机现在是正工作在采集模式
       Button_Enabale(True)
    End Sub

   
    '“停止采集”按钮点击事件
    Private Sub StopSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopSnap.Click
        MVGigE.MVStopGrab(hCam)
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
        MVAPI.MVRecordDlg.MVRecordDlgCamRun(m_hRecDlg, False)
        Button_Enabale(False)
    End Sub
    '“录像控制”按钮点击事件
    Private Sub Record_Control_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Record_Control.Click
        MVAPI.MVRecordDlg.MVRecordDlgShow(m_hRecDlg, MVAPI.MVShowWindowEnums.SW_SHOW)
    End Sub

#End Region

#Region "委托方法"
    '接受委托结果，并处理
    Function StreamCB(ByRef pInfo As IMAGE_INFO, ByVal UserVal As IntPtr) As Integer

        '将原始帧转化为hImage图像格式
        MVGigE.MVInfo2Image(hCam, pInfo, hImage)
        Dim r As Integer = MVAPI.MVRecordDlg.MVRecordDlgRecord(m_hRecDlg, hImage, pInfo.nBlockId)
        DrawImage()
        Return 0
    End Function

    '画图模块
    Function DrawImage() As Integer
        '当使用BeginInvoke异步调用方法时，如果方法未执行完，EndInvoke方法就会一直阻塞，直到被调用的方法执行完毕。
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

        If hImage <> IntPtr.Zero Then
            '将hImage图像画到me.Handle画布上面
            MVAPI.MVImage.MVImageDrawHwnd(hImage, Me.Handle, 8, 40)
        End If
        Return 0
    End Function
#End Region
End Class
