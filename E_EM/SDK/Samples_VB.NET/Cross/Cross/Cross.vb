Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Drawing
Imports System.Drawing.Imaging
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports Cross_VB.MVAPI.MVGigE
Imports Cross_VB.MVAPI.MVImage
Imports System.Diagnostics
Imports Cross_VB.MVAPI
''' <summary>
''' 这个例子主要实现了相机的连续采集、停止、保存图像，以及图像的缩放、翻转、旋转操作,图像区域画有十字线。 
''' </summary>
''' <remarks></remarks>
Public Class Cross
    Dim m_nWidth, m_nHeight As Integer   '图像宽，高
    Dim m_hCam As IntPtr = IntPtr.Zero   '图像编号
    Dim m_hImage As IntPtr = IntPtr.Zero  '图像内存指针
    Dim m_hImageFlip As IntPtr = IntPtr.Zero   '图像翻转指针
    Dim m_hImageRotate As IntPtr = IntPtr.Zero  '图像旋转指针
    Dim m_hImageShow As IntPtr = IntPtr.Zero    '图像展示
    Dim m_PixelFormat As MV_PixelFormatEnums    '图像像素格式
    Dim m_nZoom As Integer = 1     '图像放大倍数
    Dim m_nFlip As Integer = 0   '图像翻转类别
    Dim m_nRotate As Integer = 0  '图像旋转类别


    '采集图像数据委托
    Public StreamCBDelegate As MVAPI.MV_SNAPPROC = Nothing     '声明委托
    '异步编程，用于将图像画到画布上面进行显示
    Public Delegate Function InvokeDraw() As Integer   '定义委托
    Dim iDraw As InvokeDraw = Nothing     '声明委托
    Dim ia As IAsyncResult = Nothing      '获取异步操作是否完成的信息

#Region "窗体事件"
    '初始化窗体组件
    Public Sub Cross()
        InitializeComponent()
    End Sub
    '加载窗体事件
    Private Sub Cross_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        '初始化相机函数库
        MVInitLib()
        '寻找连接到计算机的相机
        MVUpdateCameraList()
        Dim CamNum As Integer = 0
        '获取连接到计算机的相机个数
        Dim r As MVSTATUS_CODES = MVGigE.MVGetNumOfCameras(CamNum)
        If (CamNum = 0) Then
            MessageBox.Show("没有找到相机，请确认连接和相机IP设置")
            Return
        End If
        '打开相机
        r = MVOpenCamByIndex(0, m_hCam)
        If (m_hCam = IntPtr.Zero) Then
            If (r = MVSTATUS_CODES.MVST_ACCESS_DENIED) Then
                MessageBox.Show("无法打开相机，可能正被别的软件控制")
                Exit Sub
            End If
        End If
        '获取相机采集图像的宽和高
        Dim w, h As Integer
        r = MVGetWidth(m_hCam, w)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("取得图像宽度失败")
            Exit Sub
        End If

        r = MVGetHeight(m_hCam, h)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("取得图像高度失败")
            Exit Sub
        End If
        '获取相机采集图像的像素格式
        r = MVGetPixelFormat(m_hCam, m_PixelFormat)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("取得图像颜色模式失败")
            Exit Sub
        End If
        m_nWidth = w
        m_nHeight = h
        '创建图像变量
        If (m_PixelFormat = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8) Then
            m_hImage = MVImage.MVImageCreate(w, h, 8)
            m_hImageFlip = MVImage.MVImageCreate(w, h, 8)
            m_hImageRotate = MVImage.MVImageCreate(h, w, 8)
        Else
            m_hImage = MVImage.MVImageCreate(w, h, 24)
            m_hImageFlip = MVImage.MVImageCreate(w, h, 24)
            m_hImageRotate = MVImage.MVImageCreate(h, w, 24)
        End If
        '设置外闪光同步信号源
        r = MVSetStrobeSource(m_hCam, MVAPI.LineSourceEnums.LineSource_ExposureActive)
        If (r <> MVSTATUS_CODES.MVST_SUCCESS) Then
            MessageBox.Show("设置外闪光同步信号源失败")
        End If
        '初始化窗体控件
        InitZoom()   '缩放
        InitFlip()   '翻转
        InitRotate() '旋转
    End Sub
    '窗体关闭事件
    Private Sub Cross_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)   '关闭相机触发
            MVCloseCam(m_hCam)   '关闭相机
            '退出函数库
            MVTerminateLib()
            '释放图像变量占用的内存
            If m_hImage <> IntPtr.Zero Then
                MVImageRelease(m_hImage)
            End If
            If m_hImageFlip <> IntPtr.Zero Then
                MVImageRelease(m_hImageFlip)
            End If
            If m_hImageRotate <> IntPtr.Zero Then
                MVImageRelease(m_hImageRotate)
            End If
            '取消委托事件
            Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
            StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
            m_hCam = IntPtr.Zero
        Else
            e.Cancel = True
        End If
     
    End Sub

    '窗体重绘事件
    Private Sub Cross_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        If (CaptureSnap.Enabled And m_hImage <> IntPtr.Zero) Then DrawImage()
    End Sub
    '保存图像
    Private Sub Save_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Save.Click
        '设置文件筛选格式
        Me.SaveFileDialog1.Filter = "Bitmaps (*.bmp)|*.bmp|JPEG Image File (*.jpg)|*.jpg|JPEG Image File (*.jpeg)|*.jpeg|GIF Image (*.gif)|*.gif"

        If (SaveFileDialog1.ShowDialog(Me) = DialogResult.OK) Then
            MVImage.MVImageSave(m_hImageShow, SaveFileDialog1.FileName)   '按照指定路径保存图像文件
        End If
    End Sub
    '连续采集事件
    Private Sub CaptureSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CaptureSnap.Click
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)    '为委托注册方法
        MVSetTriggerMode(m_hCam, MVAPI.TriggerModeEnums.TriggerMode_Off)   '关闭相机触发
        MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)   '开始采集
        Button_Enabale(True)
    End Sub
    '停止采集事件
    Private Sub StopSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopSnap.Click
        '相机停止采集
        MVStopGrab(m_hCam)
        '取消委托事件
        Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
        StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Button_Enabale(False)
    End Sub
    '翻转下拉框选项变化事件
    Private Sub comboBox_Flip_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles comboBox_Flip.SelectedIndexChanged
        m_nFlip = comboBox_Flip.SelectedIndex
        If (CaptureSnap.Enabled) Then
            Flip()
            Rotate()
            Me.Refresh()
        End If
    End Sub
    '旋转下拉框选项变化事件
    Private Sub comboBox_Rotate_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles comboBox_Rotate.SelectedIndexChanged
        m_nRotate = comboBox_Rotate.SelectedIndex
        If (CaptureSnap.Enabled) Then
            Rotate()
        End If
        Me.Refresh()
    End Sub
    '缩放系数下拉框选项变化事件
    Private Sub comboBox_Zoom_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles comboBox_Zoom.SelectedIndexChanged
        Select Case (comboBox_Zoom.SelectedIndex)
            Case 0
                m_nZoom = 4

            Case 1
                m_nZoom = 2

            Case 2
                m_nZoom = 1

            Case Else
                m_nZoom = 1

        End Select
        Me.Refresh()
    End Sub

    ''' <summary>
    ''' 按钮状态设置
    ''' </summary>
    ''' <param name="bRun"></param>
    ''' <remarks></remarks>
    Private Sub Button_Enabale(ByVal bRun As Boolean)
        StopSnap.Enabled = bRun
        CaptureSnap.Enabled = Not bRun
    End Sub
#End Region

#Region "函数方法"
   

    '画图模块
    Function DrawImage() As Integer

        If (InvokeRequired) Then
            If (ia Is Nothing) Then
                iDraw = AddressOf DrawImage
                ia = Me.BeginInvoke(iDraw)   '执行委托，参数类型与个数应与声明一致  
            ElseIf (ia.IsCompleted) Then
                iDraw = AddressOf DrawImage
                EndInvoke(ia)  '获得方法的返回值（EndInvoke方法的返回值就是被调用方法的返回值），或是确定方法已经被成功调用
                ia = Me.BeginInvoke(iDraw)  '使用线程异步地执行委托所指向的方法
            End If
            Return 0
        End If
        '处理图像，旋转、缩放、翻转
        Dim hImage As IntPtr = IntPtr.Zero
        Dim w, h As Single
        Dim hdc As IntPtr = IntPtr.Zero
        If (m_nRotate > 0) Then
            w = m_nHeight / m_nZoom
            h = m_nWidth / m_nZoom
            m_hImageShow = m_hImageRotate
        Else
            h = m_nHeight / m_nZoom
            w = m_nWidth / m_nZoom
            If (m_nFlip > 0) Then
                m_hImageShow = m_hImageFlip
            Else
                m_hImageShow = m_hImage
            End If
        End If
        hImage = MVImage.MVImageCreate(w, h, 24)    '创建图像
        hdc = MVImage.MVImageGetDC(hImage)
        MVImage.MVImageDrawEx(m_hImageShow, hdc, 0, 0, w, h, 0, 0, MVImage.MVImageGetWidth(m_hImageShow), MVImage.MVImageGetHeight(m_hImageShow))

        Dim g As Graphics = Graphics.FromHdcInternal(hdc)  '创建Graphics对象
        Dim Ppen As Pen = New Pen(Color.Green)   '定义画线用的画笔
        g.DrawLine(Ppen, w / 2, 0, w / 2, h) '画线 
        g.DrawLine(Ppen, 0, h / 2, w, h / 2) '画线

        MVImageDrawHwnd(hImage, Me.Handle, 8, 40)  '将图像画到窗体上
        MVImageReleaseDC(hImage)   '释放图像占用的资源
        MVImageRelease(hImage)
        g.Dispose()
        Return 0
    End Function
    '展示图像
    Function ShowImage(ByRef pInfo As MVAPI.IMAGE_INFO) As Integer
        MVInfo2Image(m_hCam, pInfo, m_hImage)
        Flip()
        Rotate()
        Return DrawImage()
    End Function
    '委托执行的方法
    Function StreamCB(ByRef pInfo As MVAPI.IMAGE_INFO, ByVal UserVal As IntPtr) As Integer
        Return ShowImage(pInfo)
    End Function
    '初始化缩放下拉框选项
    Private Sub InitZoom()
        comboBox_Zoom.Items.Add("25%")
        comboBox_Zoom.Items.Add("50%")
        comboBox_Zoom.Items.Add("100%")
        comboBox_Zoom.SelectedIndex = 2
        m_nZoom = 1
    End Sub
    '初始化翻转下拉框选项
    Private Sub InitFlip()
        comboBox_Flip.Items.Add("不翻转")
        comboBox_Flip.Items.Add("左右翻转")
        comboBox_Flip.Items.Add("上下翻转")
        comboBox_Flip.Items.Add("同时翻转")
        comboBox_Flip.SelectedIndex = 0
    End Sub
    '初始化旋转下拉框选项
    Private Sub InitRotate()
        comboBox_Rotate.Items.Add("不旋转")
        comboBox_Rotate.Items.Add("顺时针90°")
        comboBox_Rotate.Items.Add("逆时针90°")
        comboBox_Rotate.SelectedIndex = 0
    End Sub

    '图像翻转方法
    Private Sub Flip()
        Dim nFlip As Integer = m_nFlip
        Select Case nFlip
            Case 1
                MVImageFlip(m_hCam, m_hImage, m_hImageFlip, MVAPI.ImageFlipType.FlipHorizontal)   '左右翻转
            Case 2
                MVImageFlip(m_hCam, m_hImage, m_hImageFlip, MVAPI.ImageFlipType.FlipVertical)     '上下翻转
            Case 3
                MVImageFlip(m_hCam, m_hImage, m_hImageFlip, MVAPI.ImageFlipType.FlipBoth)     '同时翻转
        End Select
    End Sub
    '图像旋转方法
    Private Sub Rotate()
        Dim nRotate As Integer = m_nRotate
        If (nRotate = 0) Then   '不旋转
            Exit Sub
        End If
        Dim hImage As IntPtr = m_hImage
        If (m_nFlip <> 0) Then
            hImage = m_hImageFlip
        End If

        Select Case nRotate
            Case 1
                MVImageRotate(m_hCam, hImage, m_hImageRotate, MVAPI.ImageRotateType.Rotate90DegCw)  '顺时针旋转90度
            Case 2
                MVImageRotate(m_hCam, hImage, m_hImageRotate, MVAPI.ImageRotateType.Rotate90DegCcw)  '逆时针旋转90度
        End Select
    End Sub
#End Region
End Class
