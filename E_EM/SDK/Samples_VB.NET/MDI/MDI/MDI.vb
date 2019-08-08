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
Imports System.Diagnostics
Imports MDI_VB.MVAPI
''' <summary>
''' 这个例子主要实现了多窗体下，多相机的连续采集、停止、图像保存。
''' </summary>
''' <remarks></remarks>
Public Class MDI
    Dim m_hCam As IntPtr = IntPtr.Zero    '相机指针

#Region "窗体事件"

    '窗体初始化
    Public Function MDI()
        InitializeComponent()
        Return 0
    End Function
    '窗体关闭事件
    Private Sub MDI_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        If MessageBox.Show("请确认是否退出？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) = DialogResult.OK Then
            MVGigE.MVTerminateLib()  '释放相机占用资源
        Else
            e.Cancel = True
        End If
    End Sub
    '窗体加载事件
    Private Sub MDI_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        MVInitLib()
    End Sub
    '“开始采集”点击事件
    Private Sub startToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles startToolStripMenuItem.Click
        If (ActiveMdiChild IsNot Nothing) Then
            CType(ActiveMdiChild, Child).CaptureSnap()
        End If
    End Sub
    '新建子窗口
    Private Sub New_Window_Click(ByVal sender As Object, ByVal e As EventArgs)

        Dim tm As ToolStripItem = CType(sender, ToolStripItem)
        Dim str As String = tm.Text

        For Each form As Form In Application.OpenForms
            If form.GetType = Child.GetType And (form.Text = str) Then
                form.Activate()
            End If
        Next
    End Sub
    '移除内容
    Public Sub Remove(ByVal key As String)
        Me.WND_ToolStripMenuItem.DropDownItems.RemoveByKey(key)
    End Sub
    '“新建”按钮点击事件
    Private Sub New_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles New_ToolStripMenuItem.Click
        Dim lst As Camera_List = New Camera_List()
        If (lst.ShowDialog() = DialogResult.OK) Then

            Dim Info As MVAPI.MVCamInfo = Nothing
            Dim CamInfo As String

            Dim nCount As Integer = lst.CameraList.SelectedItems.Count

            For i As Integer = 0 To nCount - 1 Step 1

                If (lst.CameraList.SelectedItems(i).Text = "否") Then
                    Continue For
                End If
                Dim idx As Integer = CInt(lst.CameraList.SelectedItems(i).Tag)
                MVGigE.MVGetCameraInfo(CByte(idx), Info)
                CamInfo = Info.mModelName + " " + Info.mSerialNumber

                Dim t As ToolStripItem = Me.WND_ToolStripMenuItem.DropDownItems.Add(CamInfo, Nothing, New EventHandler(AddressOf New_Window_Click))
                t.Name = CamInfo

                Dim ChildForm As Child = New Child()
                ChildForm.m_nCam = CByte(idx)
                ChildForm.MdiParent = Me
                ChildForm.Name = CamInfo
                ChildForm.Text = CamInfo
                ChildForm.Show()
            Next
        End If
    End Sub
    '“保存”按钮点击事件
    Private Sub Save_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Save_ToolStripMenuItem.Click

        If (ActiveMdiChild IsNot Nothing) Then
            CType(ActiveMdiChild, Child).Save()
        Else
            MessageBox.Show("请打开相机")
        End If
    End Sub

    '“停止”按钮点击事件
    Private Sub stopToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles stopToolStripMenuItem.Click
        If (ActiveMdiChild IsNot Nothing) Then
            CType(ActiveMdiChild, Child).StopSnap()
        End If
    End Sub
    '“属性设置”按钮点击事件
    Private Sub propertyToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles propertyToolStripMenuItem.Click
        If (ActiveMdiChild IsNot Nothing) Then
            CType(ActiveMdiChild, Child).Pperty()
        End If
    End Sub
    '窗口“层叠”
    Private Sub Cascade_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cascade_ToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.Cascade)
    End Sub
    '窗口“垂直平铺”
    Private Sub Tile_Horizontal_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Tile_Horizontal_ToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.TileHorizontal)
    End Sub
    '窗口“水平平铺”
    Private Sub Tile_Vertical_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Tile_Vertical_ToolStripMenuItem.Click
        Me.LayoutMdi(MdiLayout.TileVertical)
    End Sub
    '用户从键盘或鼠标访问菜单时发生的事件
    Private Sub menuStrip1_MenuActivate(ByVal sender As Object, ByVal e As System.EventArgs) Handles menuStrip1.MenuActivate

        If (ActiveMdiChild IsNot Nothing) Then
            startToolStripMenuItem.Enabled = Not CType(ActiveMdiChild, Child).m_bRun
            stopToolStripMenuItem.Enabled = CType(ActiveMdiChild, Child).m_bRun
            propertyToolStripMenuItem.Enabled = True
        Else
            startToolStripMenuItem.Enabled = False
            stopToolStripMenuItem.Enabled = False
            propertyToolStripMenuItem.Enabled = False
        End If
        If (ActiveMdiChild IsNot Nothing) Then
            For i As Integer = 3 To WND_ToolStripMenuItem.DropDownItems.Count - 1 Step 1
                Dim tsmi As ToolStripMenuItem = WND_ToolStripMenuItem.DropDownItems(i)
                If (ActiveMdiChild.Text = WND_ToolStripMenuItem.DropDownItems(i).Name) Then
                    tsmi.Checked = True
                Else
                    tsmi.Checked = False
                End If
            Next
        End If

        If (ActiveMdiChild Is Nothing) Then
            If (fileToolStripMenuItem.DropDownItems(1).Text = "关闭") Then
                fileToolStripMenuItem.DropDownItems.RemoveAt(1)
            Else
                If (fileToolStripMenuItem.DropDownItems(1).Text IsNot "关闭") Then
                    fileToolStripMenuItem.DropDownItems.Insert(1, Close_ToolStripMenuItem)
                End If
            End If
        End If
    End Sub
    '“关闭”事件
    Private Sub Close_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Close_ToolStripMenuItem.Click
        If (ActiveMdiChild IsNot Nothing) Then Me.ActiveMdiChild.Close()
    End Sub
    '“退出”事件
    Private Sub Quit_ToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Quit_ToolStripMenuItem.Click
        Me.Close()
    End Sub
    #End Region
End Class
