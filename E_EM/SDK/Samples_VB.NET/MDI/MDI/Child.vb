Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Drawing
Imports System.Drawing.Imaging
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports MDI_VB.MVAPI.MVGigE
Imports MDI_VB.MVAPI.MVSTATUS_CODES
Imports MDI_VB.MVAPI.MVCamProptySheet
Imports MDI_VB.MVAPI.MVImage
Imports MDI_VB.MVAPI
Imports System.Diagnostics
Imports System.Threading

Public Class Child
    Dim m_hImage As IntPtr = IntPtr.Zero   '图像指针
    Dim m_hCam As IntPtr = IntPtr.Zero     '相机指针
    Dim m_hPro As IntPtr = IntPtr.Zero     '相机属性页指针
    '异步编程，用于将图像画到画布上面显示    
    Public Delegate Function InvokeDraw() As Integer   '定义委托
    Dim iDraw As InvokeDraw = Nothing     '声明委托
    Dim ia As IAsyncResult = Nothing       '获取异步操作是否完成的信息

    Public m_nCam As Byte = 0       '相机编号
    Dim m_PixelFormat As MV_PixelFormatEnums    '像素格式
    Dim m_nWidth, m_nHeight As Integer     '图像宽，高
    Dim StreamCBDelegate As MV_SNAPPROC = Nothing   '声明委托

    Public m_bRun As Boolean = False     '标志相机是否运行

#Region "函数方法"
    '画图模块
    Function DrawImage() As Integer
        '当使用BeginInvoke异步调用方法时，如果方法未执行完，EndInvoke方法就会一直阻塞，直到被调用的方法执行完毕。
        If (InvokeRequired) Then
            If (ia Is Nothing) Then
                iDraw = AddressOf DrawImage
                ia = Me.BeginInvoke(iDraw)  '执行委托
            ElseIf (ia.IsCompleted) Then
                iDraw = AddressOf DrawImage
                EndInvoke(ia) '获得方法的返回值
                ia = Me.BeginInvoke(iDraw)  '使用线程异步地执行委托所指向的方法
            End If
            Return 0
        End If
        Dim x As Integer = Me.AutoScrollPosition.X
        Dim y As Integer = Me.AutoScrollPosition.Y
        MVImage.MVImageDrawHwnd(m_hImage, Me.Handle, x, y)
        Return 0
    End Function
    '为委托注册方法
    Function StreamCB(ByRef pInfo As MVAPI.IMAGE_INFO, ByVal UserVal As IntPtr) As Integer
        MVGigE.MVInfo2Image(m_hCam, pInfo, m_hImage)   '将原始帧转换为图像
        Return DrawImage()
    End Function
    '初始化图像变量
    Private Sub ImageCreat()

        Dim w As Integer = 0
        Dim h As Integer = 0

        Dim r As MVSTATUS_CODES = MVGigE.MVGetWidth(m_hCam, w)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("取得图像宽度失败")
            Exit Sub
        End If

        r = MVGigE.MVGetHeight(m_hCam, h)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("取得图像高度失败")
            Exit Sub
        End If

        r = MVGigE.MVGetPixelFormat(m_hCam, m_PixelFormat)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("取得图像颜色模式失败")
            Exit Sub
        End If

        If (m_nWidth <> w And m_nHeight <> h) Then

            m_nWidth = w
            m_nHeight = h

            If (m_hImage <> IntPtr.Zero) Then
                MVAPI.MVImage.MVImageRelease(m_hImage)
                m_hImage = IntPtr.Zero
            End If
            '创建图像变量
            If (m_PixelFormat = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8) Then
                m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 8)
            Else
                m_hImage = MVAPI.MVImage.MVImageCreate(w, h, 24)
            End If
            Refresh()
        End If
    End Sub

#End Region

#Region "窗体事件"
    '初始化窗口组件
    Public Sub Child()
        InitializeComponent()
    End Sub
    '窗体关闭事件
    Private Sub Child_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出当前相机窗口？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            '关闭相机，销毁相机所占资源
            MVCamProptySheetDestroy(m_hPro)
            MVCloseCam(m_hCam)
            If m_hImage <> IntPtr.Zero Then MVImageDestroy(m_hImage)
            MDI.Remove(Text)
            '取消委托事件
            Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
            StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Else
            e.Cancel = True
        End If

    End Sub
    '窗体加载事件
    Private Sub Child_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim r As MVSTATUS_CODES
        '打开相机
        r = MVOpenCamByIndex(m_nCam, m_hCam)
        If (m_hCam = IntPtr.Zero) Then
            If (r = MVSTATUS_CODES.MVST_ACCESS_DENIED) Then
                MessageBox.Show("无法打开相机，可能正被别的软件控制")
                Return
            End If
        End If
        '初始化图像
        ImageCreat()
        '设置外闪光同步信号源
        MVSetStrobeSource(m_hCam, LineSourceEnums.LineSource_ExposureActive)
        MVCamProptySheetCreateEx(m_hPro, m_hCam, Me.Handle, Me.Text, PAGE_ALL)
    End Sub
    '相机连续采集
    Public Sub CaptureSnap()
        ImageCreat()
        m_bRun = True
        '关闭触发模式
        MVGigE.MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)
        MVGigE.MVSetStrobeInvert(m_hCam, True)   '闪光同步是否反转，即闪光同步有效时输出高电平还是低电平

        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
        Dim r As MVSTATUS_CODES = MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)
        MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_ON)
    End Sub
    '停止相机采集
    Public Sub StopSnap()
        m_bRun = False
        MVGigE.MVStopGrab(m_hCam)
        '取消委托事件
        Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
        StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        MVCamProptySheet.MVCamProptySheetCameraRun(m_hPro, MVAPI.MVCameraRunEnums.MVCameraRun_OFF)
    End Sub
    '相机属性页
    Public Sub Pperty()
        MVCamProptySheet.MVCamProptySheetShow(m_hPro, MVAPI.MVShowWindowEnums.SW_SHOW)
    End Sub
    '保存图像到指定路径
    Public Sub Save()
        Dim bpp As Integer = MVImage.MVImageGetBPP(m_hImage)
        Dim pImage As IntPtr = MVImage.MVImageCreate(m_nWidth, m_nHeight, bpp)
        MVGigE.CopyMemory(MVImage.MVImageGetBits(pImage), MVImage.MVImageGetBits(m_hImage), CUInt(m_nWidth * m_nHeight * bpp / 8))
        If (saveFileDialog1.ShowDialog(Me) = DialogResult.OK) Then
            MVImage.MVImageSave(pImage, saveFileDialog1.FileName)
            MVImage.MVImageRelease(pImage)
        End If
    End Sub
    '窗体重绘事件
    Private Sub Child_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        If (Not m_bRun) Then
            DrawImage()
        End If
    End Sub
    #End Region

End Class