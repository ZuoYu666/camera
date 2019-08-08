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
Imports Sequence_VB.MVAPI.MVGigE
Imports Sequence_VB.MVAPI.MVImage

Imports System.Diagnostics
Imports Sequence_VB.MVAPI
''' <summary>
''' 这个例子主要实现了相机的连续采集、停止、相机属性信息设置、队列控制（按照设定时间间隔和文件格式存图）功能.
''' </summary>
''' <remarks></remarks>
Public Class Sequence

    Dim m_hCam As IntPtr = IntPtr.Zero    '相机指针
    Dim m_hSeq As IntPtr = IntPtr.Zero       '相机队列指针

    Dim imgWidth As Integer   '图像宽
    Dim imgHeight As Integer  '图像高
    Dim m_hImage As IntPtr = IntPtr.Zero     '图像指针
    Dim hPro As IntPtr = IntPtr.Zero         '相机属性指针
    '相机采集帧数
    Dim m_nCapture As UInteger
    Dim m_nCapCntBase As UInteger
    Dim m_nLastID As UShort
    Dim m_nReceive As UInteger
    Dim pf As MVAPI.MV_PixelFormatEnums   '相机像素格式

    '采集图像数据委托
    Public StreamCBDelegate As MVAPI.MV_SNAPPROC = Nothing   '声明委托
    '异步编程，用于将图像画到画布上面进行显示
    Public Delegate Function InvokeDraw() As Integer  '定义委托 
    Public iDraw As InvokeDraw = Nothing   '声明委托
    Dim ia As IAsyncResult = Nothing      '获取异步操作是否完成的信息

#Region "窗体事件"
    '窗体关闭事件
    Private Sub Sequence_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
            MVCloseCam(m_hCam)
            m_hCam = IntPtr.Zero
            If m_hSeq <> IntPtr.Zero Then MVSequenceDlg.MVSequenceDlgDestroy(m_hSeq)
            If m_hImage <> IntPtr.Zero Then MVImage.MVImageRelease(m_hImage)
            MVCamProptySheet.MVCamProptySheetDestroy(hPro)
            hPro = IntPtr.Zero
            '取消委托事件
            Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
            StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Else
            e.Cancel = True
        End If

    End Sub

    '窗体加载事件
    Private Sub Sequence_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim CamNum As Long = 0
        Dim r As MVAPI.MVSTATUS_CODES
        Dim bTest As Boolean

        '初始化相机函数库
        MVAPI.MVGigE.MVInitLib()
        MVAPI.MVGigE.MVUpdateCameraList()   '查找连接计算机的相机

        '获取连接到计算机的相机的个数
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
        '初始化相机采集图像
        ImageCreat()
        '设置外闪光同步信号源
        MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive)
        If CamNum = 0 Then
            MsgBox("设置外闪光同步信号源失败！")
            Exit Sub
        End If
        '创建序列帧计时器对话框
        bTest = MVAPI.MVSequenceDlg.MVSequenceDlgCreateEx(m_hSeq, Me.Handle)
        '创建相机属性页
        MVCamProptySheet.MVCamProptySheetCreateEx(hPro, m_hCam, Me.Handle, "相机属性", MVCamProptySheet.PAGE_ALL)
    End Sub
    '初始化相机采集图像
    Private Sub ImageCreat()
        Dim w As Integer
        Dim h As Integer
        Dim r As MVAPI.MVSTATUS_CODES
        '获取图像高度
        r = MVAPI.MVGigE.MVGetWidth(m_hCam, w)
        If r <> MVSTATUS_CODES.MVST_SUCCESS Then
            MsgBox("取得图像宽度失败！")
            Exit Sub
        End If
        '获取图像宽度
        r = MVAPI.MVGigE.MVGetHeight(m_hCam, h)
        If r <> MVSTATUS_CODES.MVST_SUCCESS Then
            MsgBox("取得图像宽度失败！")
            Exit Sub
        End If
        '获取图像像素格式
        r = MVAPI.MVGigE.MVGetPixelFormat(m_hCam, pf)
        If r <> MVSTATUS_CODES.MVST_SUCCESS Then
            MsgBox("取得图像宽度失败！")
            Exit Sub
        End If

        If (imgWidth <> w Or imgHeight <> h) Then
            imgWidth = w
            imgHeight = h
            If m_hImage <> IntPtr.Zero Then
                MVAPI.MVImage.MVImageRelease(m_hImage)
                m_hImage = IntPtr.Zero
            End If

            If pf = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8 Then
                m_hImage = MVAPI.MVImage.MVImageCreate(imgWidth, imgHeight, 8)
            Else
                m_hImage = MVAPI.MVImage.MVImageCreate(imgWidth, imgHeight, 24)
            End If
            Refresh()
        End If
    End Sub
    '保存图像
    Public Sub SaveImage(ByVal strPath As String)
        MVAPI.MVImage.MVImageSave(m_hImage, strPath)
    End Sub
    '“连续采集”按钮点击事件
    Private Sub CaptureSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CaptureSnap.Click
        '“图像初始化”
        ImageCreat()
        '为委托注册方法
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
        '开始采集图像
        MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)

        Button_Enabale(True)
        '设置相机正在采集模式工作，禁止修改相机属性
        MVAPI.MVSequenceDlg.MVSequenceDlgCamRun(m_hSeq, True)
        MVAPI.MVCamProptySheet.MVCamProptySheetCameraRun(hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON)
    End Sub
    '按钮状态设置
    Private Sub Button_Enabale(ByVal bRun As Boolean)
        StopSnap.Enabled = bRun
        CaptureSnap.Enabled = Not bRun
    End Sub
    '“停止采集”按钮点击事件
    Private Sub StopSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopSnap.Click
        MVGigE.MVStopGrab(m_hCam)
        '取消委托事件
        Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
        StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Button_Enabale(False)
        '设置相机现在不是正工作在采集模式，允许修改相机属性
        MVAPI.MVSequenceDlg.MVSequenceDlgCamRun(m_hSeq, False)        '
        MVAPI.MVCamProptySheet.MVCamProptySheetCameraRun(hPro, MVCameraRunEnums.MVCameraRun_OFF)
    End Sub
    '窗体重绘事件
    Private Sub Sequence_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        If CaptureSnap.Enabled Then DrawImage()
    End Sub
    '“队列控制”按钮点击事件
    Private Sub Sequence_Control_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Sequence_Control.Click
        MVAPI.MVSequenceDlg.MVSequenceDlgShow(m_hSeq, MVAPI.MVShowWindowEnums.SW_SHOW)
    End Sub
    '“相机属性”按钮点击事件
    Private Sub BN_Property_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BN_Property.Click
        MVAPI.MVCamProptySheet.MVCamProptySheetShow(hPro, MVAPI.MVShowWindowEnums.SW_SHOW)
    End Sub

#End Region

#Region "委托方法"
    '画图模块
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

    '接受委托结果，并处理
    Function StreamCB(ByRef pInfo As IMAGE_INFO, ByVal UserVal As IntPtr) As Integer

        '将原始帧转化为m_hImage图像格式
        MVGigE.MVInfo2Image(m_hCam, pInfo, m_hImage)
        Dim strPath As StringBuilder = New StringBuilder(260)
        '获得即将进入队列的文件名，及当前图片是否需要保存
        If MVAPI.MVSequenceDlg.MVSequenceDlgGetFileName(m_hSeq, strPath, 260) = 0 Then
            SaveImage(strPath.ToString)
        End If
        DrawImage()
        Return 0
    End Function
#End Region
End Class
