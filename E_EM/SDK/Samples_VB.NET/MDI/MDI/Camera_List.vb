Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Drawing
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports MDI_VB.MVAPI.MVGigE
Imports MDI_VB.MVAPI.MVSTATUS_CODES
Imports MDI_VB.MVAPI

Public Class Camera_List

#Region "窗体事件"
    '初始化窗体组件
    Public Function Camera_List()
        InitializeComponent()
        Return 0
    End Function
    '加载窗体，相机列表
    Private Sub Camera_List_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim ch(3) As ColumnHeader
        ch(0) = New ColumnHeader()
        ch(1) = New ColumnHeader()
        ch(2) = New ColumnHeader()
        ch(0).Text = "允许访问"
        ch(1).Text = "型号"
        ch(2).Text = "序列号"
        ch(0).Width = 65
        ch(1).Width = 120
        ch(2).Width = 120
        ch(0).TextAlign = HorizontalAlignment.Left
        ch(1).TextAlign = HorizontalAlignment.Left
        ch(2).TextAlign = HorizontalAlignment.Left
        Me.CameraList.Columns.AddRange({ch(0), ch(1), ch(2)})
        Dim lvi As ListViewItem = New ListViewItem()
        BN_Refresh_Click(BN_Refresh, Nothing)
    End Sub

    '点击“刷新”
    Private Sub BN_Refresh_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BN_Refresh.Click

        Dim isfirst As Boolean = True
        Dim bSelect As Boolean = False

        Dim nCameraCount As Integer = 0
        CameraList.Items.Clear()   '清空列表
        MVGigE.MVUpdateCameraList()  '寻找连接到计算机的相机
        '获取连接到计算机的相机的个数
        If (MVGigE.MVGetNumOfCameras(nCameraCount) = MVAPI.MVSTATUS_CODES.MVST_SUCCESS) Then

            For i As Integer = 0 To nCameraCount - 1 Step 1
                Dim info As MVCamInfo = Nothing
                MVGigE.MVGetCameraInfo(CByte(i), info)

                Dim hCam As IntPtr
                Dim str As String = "否"
                '打开相机
                If (MVGigE.MVOpenCamByIndex(CByte(i), hCam) = MVAPI.MVSTATUS_CODES.MVST_SUCCESS) Then
                    str = "是"
                    MVGigE.MVCloseCam(hCam)
                    If (isfirst = True) Then
                        bSelect = True
                        isfirst = False
                    End If
                End If
                Dim lvi As ListViewItem = New ListViewItem()
                lvi.Tag = i
                lvi.Text = str
                lvi.SubItems.Add(info.mModelName)
                lvi.SubItems.Add(info.mSerialNumber)
                Me.CameraList.Items.Add(lvi)
                If (bSelect = True) Then
                    Me.CameraList.Items(i).Selected = True
                End If
            Next
        Else
            MessageBox.Show("无法刷新列表")
        End If
    End Sub
    '点击“确定”
    Private Sub OK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OK.Click
        Dim nCount As Integer = Me.CameraList.SelectedItems.Count
        If (nCount <= 0) Then
            MessageBox.Show("没有选择相机或所有没有可访问的相机")
        Else
            If (Me.CameraList.SelectedItems(0).Text <> "否") Then
                Me.DialogResult = DialogResult.OK
            Else
                MessageBox.Show("选中的相机无法访问")
            End If
        End If
    End Sub
    '点击“取消”
    Private Sub Cancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Cancel.Click
        Me.DialogResult = DialogResult.Cancel
    End Sub
#End Region
End Class