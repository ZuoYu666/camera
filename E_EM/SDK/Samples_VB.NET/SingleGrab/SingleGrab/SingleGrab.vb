Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Drawing
Imports System.Drawing.Imaging
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports SingleGrab_VB.MVAPI.MVGigE
Imports SingleGrab_VB.MVAPI.MVCamProptySheet
Imports SingleGrab_VB.MVAPI.MVSTATUS_CODES
Imports SingleGrab_VB.MVAPI.MVImage
Imports System.Diagnostics
Imports SingleGrab_VB.MVAPI

''' <summary>
''' 这个例子主要实现了相机的连续采集、单帧采集、停止、保存图像以及相机属性信息的设置.
''' </summary>
''' <remarks></remarks>
Public Class SingleGrab

    Dim m_hCam As IntPtr = IntPtr.Zero    '相机指针
    Dim m_hPro As IntPtr = IntPtr.Zero    '相机属性页句柄
    Dim m_PixelFormat As MV_PixelFormatEnums   '图像像素格式
    Dim m_nWidth, m_nHeight As Integer    '图像宽，高

    '异步编程，用于将图像画到画布上面显示
    Public Delegate Function InvokeDraw() As Integer  '定义委托
    Dim iDraw As InvokeDraw = Nothing   '声明委托
    Dim ia As IAsyncResult = Nothing    '获取异步操作是否完成的信息

    Dim m_hImage As IntPtr = IntPtr.Zero   '图像指针
    Dim StreamCBDelegate As MV_SNAPPROC = Nothing     '声明委托


#Region "委托方法"
    '画图模块
    Function DrawImage() As Integer
        '当使用BeginInvoke异步调用方法时，如果方法未执行完，EndInvoke方法就会一直阻塞，直到被调用的方法执行完毕。
        If (InvokeRequired) Then
            If (ia Is Nothing) Then
                iDraw = AddressOf DrawImage
                ia = Me.BeginInvoke(iDraw)    '执行委托
            ElseIf (ia.IsCompleted) Then
                iDraw = AddressOf DrawImage
                EndInvoke(ia)      '获得方法的返回值
                ia = Me.BeginInvoke(iDraw)    '使用线程异步地执行委托所指向的方法
            End If
            Return 0
        End If

        If (m_hImage <> IntPtr.Zero) Then
            '将图像画到窗体上
            MVAPI.MVImage.MVImageDrawHwnd(m_hImage, Me.Handle, 8, 40)
        End If
        Return 0
    End Function
    '接受委托结果，并处理
    Function StreamCB(ByRef pInfo As IMAGE_INFO, ByVal UserVal As IntPtr) As Integer
        MVInfo2Image(m_hCam, pInfo, m_hImage)
        DrawImage()
        Return 0
    End Function
#End Region


#Region "窗体事件"
    '窗体关闭事件
    Private Sub SingleGrab_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            '关闭触发
            MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
            '关闭相机
            MVCloseCam(m_hCam)
            m_hCam = IntPtr.Zero
            '释放相机占用的资源
            MVTerminateLib()
            If m_hImage <> IntPtr.Zero Then MVImage.MVImageRelease(m_hImage)
            If m_hPro <> IntPtr.Zero Then MVCamProptySheet.MVCamProptySheetDestroy(m_hPro)
        Else
            e.Cancel = True
        End If
      
    End Sub
    '窗体加载事件
    Private Sub SingleGrab_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        '初始化相机函数库
        MVGigE.MVInitLib()
        '寻找连接到计算机的相机
        MVGigE.MVUpdateCameraList()
        Dim CamNum As Integer = 0
        '获取连接到计算机的相机个数
        Dim r As MVSTATUS_CODES = MVGigE.MVGetNumOfCameras(CamNum)
        If (CamNum = 0) Then
            MessageBox.Show("没有找到相机，请确认连接和相机IP设置")
            Return
        End If
        '打开相机
        r = MVGigE.MVOpenCamByIndex(0, m_hCam)
        If (m_hCam = IntPtr.Zero) Then
            If (r = MVST_ACCESS_DENIED) Then
                MessageBox.Show("无法打开相机，可能正被别的软件控制")
                Return
            End If
        End If
        '初始化相机采集的图像
        ImageCreat()
        '设置外闪光同步信号源
        MVGigE.MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive)
        If (CamNum = 0) Then
            MessageBox.Show("设置外闪光同步信号源失败")
        End If
        '创建相机属性页
        MVAPI.MVCamProptySheet.MVCamProptySheetCreateEx(m_hPro, m_hCam, Me.Handle, "相机属性", MVAPI.MVCamProptySheet.PAGE_ALL)
    End Sub

    '按钮状态设置
    Private Sub Button_Enabale(ByVal bRun As Boolean)
        StopSnap.Enabled = bRun
        CaptureSnap.Enabled = Not bRun
        BN_SingleGrab.Enabled = Not bRun
    End Sub

    '“连续采集”按钮点击事件
    Private Sub CaptureSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CaptureSnap.Click
        '初始化相机采集的图像
        ImageCreat()
        '为StreamCBDelegate委托注册StreamCB方法
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
        '关闭相机触发模式
        MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
        '开始采集
        MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)

        Button_Enabale(True)
        ''设置相机现在正工作在采集模式
        MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON)
    End Sub
    '“单帧采集”按钮事件
    Private Sub BN_SingleGrab_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BN_SingleGrab.Click
        '初始化相机采集的图像
        ImageCreat()
        '为StreamCBDelegate委托注册StreamCB方法
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
        '关掉相机触发
        MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
        '设置相机现在正工作在采集模式，采集状态下不允许改变的相机属性
        MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON)
        Button_Enabale(True)
        '开始单帧采集
        MVGigE.MVSingleGrab(m_hCam, m_hImage, 1000)
        DrawImage()
        Button_Enabale(False)
        '设置相机现在不工作在采集模式
        MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF)
    End Sub
    '“停止采集”按钮点击事件
    Private Sub StopSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopSnap.Click
        '停止相机采集
        MVGigE.MVStopGrab(m_hCam)
        '取消委托事件
        Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
        StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Button_Enabale(False)
        MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF)
    End Sub
    '"保存图像"按钮点击事件
    Private Sub Save_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Save.Click
        Dim bpp As Integer = MVImage.MVImageGetBPP(m_hImage)
        Dim pImage As IntPtr = MVImage.MVImageCreate(m_nWidth, m_nHeight, bpp)
        MVGigE.CopyMemory(MVImage.MVImageGetBits(pImage), MVImage.MVImageGetBits(m_hImage), CUInt(m_nWidth * m_nHeight * bpp / 8))
        If (saveFileDialog1.ShowDialog(Me) = DialogResult.OK) Then
            MVImage.MVImageSave(pImage, saveFileDialog1.FileName)
        End If
        MVImage.MVImageRelease(pImage)
    End Sub
    '窗体重绘事件
    Private Sub SingleGrab_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        If (CaptureSnap.Enabled) Then
            DrawImage()
        End If
    End Sub
    '“属性”按钮点击事件
    Private Sub Button_Property_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Property.Click
        MVCamProptySheet.MVCamProptySheetShow(m_hPro, MVAPI.MVShowWindowEnums.SW_SHOW)
    End Sub
    '初始化相机图像
    Private Sub ImageCreat()

        Dim w As Integer = 0
        Dim h As Integer = 0
        '获取图像宽
        Dim r As MVSTATUS_CODES = MVGigE.MVGetWidth(m_hCam, w)
        If (r <> MVST_SUCCESS) Then
            MessageBox.Show("取得图像宽度失败")
            Return
        End If
        '获取图像高
        r = MVGigE.MVGetHeight(m_hCam, h)
        If (r <> MVST_SUCCESS) Then

            MessageBox.Show("取得图像高度失败")
            Return
        End If
        '获取图像像素格式
        r = MVGigE.MVGetPixelFormat(m_hCam, m_PixelFormat)
        If (r <> MVST_SUCCESS) Then
            MessageBox.Show("取得图像颜色模式失败")
            Return
        End If

        If (m_nWidth <> w Or m_nHeight <> h) Then

            m_nWidth = w
            m_nHeight = h

            If (m_hImage <> IntPtr.Zero) Then
                MVAPI.MVImage.MVImageRelease(m_hImage)
                m_hImage = IntPtr.Zero
            End If

            If (m_PixelFormat = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8) Then
                m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 8)
            Else
                m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 24)
            End If
            Refresh()
        End If
    End Sub
#End Region
End Class
