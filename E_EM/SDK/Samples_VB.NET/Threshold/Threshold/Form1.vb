Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Drawing
Imports System.Drawing.Imaging
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports Threshold_VB.MVAPI.MVGigE
Imports Threshold_VB.MVAPI.MVImage
Imports Threshold_VB.MVAPI.MVSTATUS_CODES
Imports System.Diagnostics
Imports Threshold_VB.MVAPI
Imports System.Runtime.InteropServices

''' <summary>
''' 这个例子主要实现了相机连续采集、停止采集、保存（单帧图像）、二值化和反色（处理图像）.
''' </summary>
''' <remarks></remarks>
Public Class Threshold
    '异步编程，用于将图像画到画布上面显示
    Public Delegate Function InvokeDraw() As Integer  '定义委托
    Dim iDraw As InvokeDraw = Nothing   '声明委托
    Dim ia As IAsyncResult = Nothing    '获取异步操作是否完成的信息

    Dim m_hCam As IntPtr = IntPtr.Zero   '相机指针
    Dim m_PixelFormat As MV_PixelFormatEnums  '图像像素格式
    Dim m_nWidth, m_nHeight, m_nThres As Integer  '图像宽，高，二值化阈值
    Dim ttpSettings As ToolTip = Nothing   '控件属性提示信息
    Dim m_hImage As IntPtr = IntPtr.Zero   '原始图像指针
    Dim m_hImageDst As IntPtr = IntPtr.Zero '转换后的图像指针
    Dim StreamCBDelegate As MV_SNAPPROC = Nothing   '声明委托


#Region "委托方法"
    ''' <summary>
    ''' 画图模块
    ''' </summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Function DrawImage() As Integer
        '当使用BeginInvoke异步调用方法时，如果方法未执行完，EndInvoke方法就会一直阻塞，直到被调用的方法执行完毕。
        If (InvokeRequired) Then

            If (ia Is Nothing) Then

                iDraw = AddressOf DrawImage
                ia = Me.BeginInvoke(iDraw) '执行委托

            ElseIf (ia.IsCompleted) Then

                iDraw = AddressOf DrawImage
                EndInvoke(ia) '获得方法的返回值
                ia = Me.BeginInvoke(iDraw)  '使用线程异步地执行委托所指向的方法
            End If
            Return 0
        End If

        If (m_hImage <> IntPtr.Zero) Then
            '是否转换图像，二值化，反色
            If (checkBox_Invert.Checked Or checkBox_Binary.Checked) Then
                ProcessImage()
                MVAPI.MVImage.MVImageDrawHwnd(m_hImageDst, Me.Handle, 8, 40)
            Else
                MVAPI.MVImage.MVImageDrawHwnd(m_hImage, Me.Handle, 8, 40)
            End If
        End If
        Return 0
    End Function

    '接受委托结果，并处理
    Function StreamCB(ByRef pInfo As IMAGE_INFO, ByVal UserVal As IntPtr) As Integer
        MVGigE.MVInfo2Image(m_hCam, pInfo, m_hImage)      '将原始帧转化为m_hImage格式图像
        DrawImage()
        Return 0
    End Function

#End Region

#Region "图像处理"
    Sub ProcessImage()
        '获取采集图像信息
        Dim nPitch As Integer = MVImage.MVImageGetPitch(m_hImage)
        Dim ptrSrc As IntPtr = MVImage.MVImageGetBits(m_hImage)
        Dim ptrDst As IntPtr = MVImage.MVImageGetBits(m_hImageDst)
        Dim n As Integer = nPitch * m_nHeight
        Dim pSrc(), pDst() As Byte

        ReDim pSrc(n), pDst(n)

        If (m_PixelFormat = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8) Then
            'Marshal.copy()方法用来在托管对象（数组）和非托管对象（IntPtr）之间进行内容的复制
            Marshal.Copy(ptrSrc, pSrc, 0, n)
            Marshal.Copy(ptrDst, pDst, 0, n)

            For i As Integer = 0 To n Step 1
                If (checkBox_Binary.Checked) Then
                    If (pSrc(i) > m_nThres) Then
                        pDst(i) = 255
                    Else
                        pDst(i) = 0
                    End If
                    If (checkBox_Invert.Checked) Then
                        pDst(i) = CByte(255 - pDst(i))
                    End If
                Else
                    pDst(i) = CByte(255 - pSrc(i))
                End If
            Next

            Marshal.Copy(pSrc, 0, ptrSrc, nPitch * m_nHeight)
            Marshal.Copy(pDst, 0, ptrDst, nPitch * m_nHeight)

        Else
            Marshal.Copy(ptrSrc, pSrc, 0, nPitch * m_nHeight)
            Marshal.Copy(ptrDst, pDst, 0, nPitch * m_nHeight)

            For i As Integer = 0 To nPitch * m_nHeight Step 1
                pDst(i) = CByte((255 - (pSrc(i) + 1)) + 1)
            Next

            Marshal.Copy(pSrc, 0, ptrSrc, nPitch * m_nHeight)
            Marshal.Copy(pDst, 0, ptrDst, nPitch * m_nHeight)

        End If

    End Sub
#End Region



#Region "窗体事件"
    '窗体初始化
    Public Function Threshold()
        InitializeComponent()
        Return 0
    End Function

    ''' <summary>
    ''' 窗体关闭事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub Threshold_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            MVGigE.MVStopGrab(m_hCam)
            MVCloseCam(m_hCam)
            '退出函数库
            MVTerminateLib()
            If m_hImage <> IntPtr.Zero Then
                MVImageRelease(m_hImage)
            End If
            If m_hImageDst <> IntPtr.Zero Then
                MVImageRelease(m_hImageDst)
            End If
            '取消委托事件
            Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
            StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Else
            e.Cancel = True
        End If
   
    End Sub
    ''' <summary>
    ''' 窗体加载事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub Threshold_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        '初始化相机函数库
        MVInitLib()
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
            If (r = MVST_ACCESS_DENIED) Then
                MessageBox.Show("无法打开相机，可能正被别的软件控制")
                Return
            End If
        End If
        '获取图像宽
        r = MVGigE.MVGetWidth(m_hCam, m_nWidth)
        If (CamNum = 0) Then
            MessageBox.Show("取得图像宽度失败")
            Return
        End If
        '获取图像高
        r = MVGetHeight(m_hCam, m_nHeight)
        If (CamNum = 0) Then
            MessageBox.Show("取得图像高度失败")
            Return
        End If
        '获取图像像素格式()
        r = MVGigE.MVGetPixelFormat(m_hCam, m_PixelFormat)
        If (CamNum = 0) Then
            MessageBox.Show("取得图像颜色模式失败")
            Return
        End If
        '创建图像
        If (m_hImage = IntPtr.Zero) Then

            If (m_PixelFormat = MVAPI.MV_PixelFormatEnums.PixelFormat_Mono8) Then
                m_hImage = MVAPI.MVImage.MVImageCreate(m_nWidth, m_nHeight, 8)
                m_hImageDst = MVAPI.MVImage.MVImageCreate(m_nWidth, m_nHeight, 8)
            Else
                m_hImage = MVAPI.MVImage.MVImageCreate(m_nWidth, m_nHeight, 24)
                m_hImageDst = MVAPI.MVImage.MVImageCreate(m_nWidth, m_nHeight, 24)
                trackBar_Thres.Enabled = False
                textBox_Thres.Enabled = False
                checkBox_Binary.Enabled = False
            End If
        End If

        '初始化窗体控件，窗口提示信息
        ttpSettings = New ToolTip()
        ttpSettings.InitialDelay = 200
        ttpSettings.AutoPopDelay = 10 * 1000
        ttpSettings.ReshowDelay = 200
        ttpSettings.ShowAlways = True
        ttpSettings.IsBalloon = True
        InitThres()

    End Sub
    '按钮状态设置
    Private Sub Button_Enabale(ByVal bRun As Boolean)
        StopSnap.Enabled = bRun
        CaptureSnap.Enabled = Not bRun
    End Sub
    ''' <summary>
    ''' “连续采集”按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub ContinueSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CaptureSnap.Click
        '为StreamCBDelegate委托注册StreamCB方法
        StreamCBDelegate = New MVAPI.MV_SNAPPROC(AddressOf StreamCB)
        '开始采集
        MVGigE.MVStartGrab(m_hCam, StreamCBDelegate, Me.Handle)
        Button_Enabale(True)
    End Sub
    ''' <summary>
    ''' “停止采集”按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub StopSnap_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StopSnap.Click
        MVGigE.MVStopGrab(m_hCam)
        '取消委托事件
        Dim UnNeededDelegate As MVAPI.MV_SNAPPROC = AddressOf StreamCB
        StreamCBDelegate = System.Delegate.Remove(StreamCBDelegate, UnNeededDelegate)
        Button_Enabale(False)
    End Sub
    ''' <summary>
    ''' “保存图像”按钮点击事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub Save_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Save.Click
        If (saveFileDialog1.ShowDialog(Me) = DialogResult.OK) Then
            If (checkBox_Invert.Checked Or checkBox_Binary.Checked) Then
                MVAPI.MVImage.MVImageSave(m_hImageDst, saveFileDialog1.FileName)
            Else
                MVAPI.MVImage.MVImageSave(m_hImage, saveFileDialog1.FileName)
            End If
        End If
    End Sub
    ''' <summary>
    ''' 窗体重绘事件
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    ''' <remarks></remarks>
    Private Sub Threshold_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        If (CaptureSnap.Enabled) Then
            DrawImage()
        End If
    End Sub
    '初始化控件
    Private Function InitThres() As Boolean

        Me.trackBar_Thres.Minimum = 0
        Me.trackBar_Thres.Maximum = 255

        Me.trackBar_Thres.Value = 128
        m_nThres = 128
        Me.textBox_Thres.Text = Me.trackBar_Thres.Value.ToString()
        Dim tipOverwrite As String
        tipOverwrite = "下限：" + "0" + ",上限：" + "255"
        ttpSettings.SetToolTip(Me.textBox_Thres, tipOverwrite)
        ttpSettings.SetToolTip(Me.trackBar_Thres, tipOverwrite)
        Return True
    End Function

    '滑动条变化事件
    Private Sub trackBar_Thres_Scroll1(ByVal sender As Object, ByVal e As System.EventArgs) Handles trackBar_Thres.Scroll
        m_nThres = Me.trackBar_Thres.Value
        Me.textBox_Thres.Text = m_nThres.ToString()

        If (CaptureSnap.Enabled) Then
            DrawImage()
        End If
    End Sub

    '控件按下事件
    Private Sub textBox_Thres_KeyPress(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyPressEventArgs) Handles textBox_Thres.KeyPress
        m_nThres = Convert.ToInt32(Me.textBox_Thres.Text)
        If (CaptureSnap.Enabled) Then
            DrawImage()
        End If

        If e.KeyChar = ChrW(13) Then
            Me.trackBar_Thres.Value = m_nThres
        End If
    End Sub
    '二值化
    Private Sub checkBox_Binary_CheckedChanged1(ByVal sender As Object, ByVal e As System.EventArgs) Handles checkBox_Binary.CheckedChanged
        If (CaptureSnap.Enabled) Then
            DrawImage()
        End If
    End Sub
    '反色
    Private Sub checkBox_Invert_CheckedChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles checkBox_Invert.CheckedChanged
        If (CaptureSnap.Enabled) Then
            DrawImage()
        End If
    End Sub

#End Region
End Class
