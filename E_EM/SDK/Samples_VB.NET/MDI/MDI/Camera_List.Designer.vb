<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Camera_List
    Inherits System.Windows.Forms.Form

    'Form 重写 Dispose，以清理组件列表。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.CameraList = New System.Windows.Forms.ListView()
        Me.BN_Refresh = New System.Windows.Forms.Button()
        Me.OK = New System.Windows.Forms.Button()
        Me.Cancel = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'CameraList
        '
        Me.CameraList.FullRowSelect = True
        Me.CameraList.Location = New System.Drawing.Point(7, 7)
        Me.CameraList.MultiSelect = False
        Me.CameraList.Name = "CameraList"
        Me.CameraList.Size = New System.Drawing.Size(489, 347)
        Me.CameraList.TabIndex = 3
        Me.CameraList.UseCompatibleStateImageBehavior = False
        Me.CameraList.View = System.Windows.Forms.View.Details
        '
        'BN_Refresh
        '
        Me.BN_Refresh.Location = New System.Drawing.Point(262, 360)
        Me.BN_Refresh.Name = "BN_Refresh"
        Me.BN_Refresh.Size = New System.Drawing.Size(75, 23)
        Me.BN_Refresh.TabIndex = 6
        Me.BN_Refresh.Text = "刷新"
        Me.BN_Refresh.UseVisualStyleBackColor = True
        '
        'OK
        '
        Me.OK.Location = New System.Drawing.Point(343, 360)
        Me.OK.Name = "OK"
        Me.OK.Size = New System.Drawing.Size(75, 23)
        Me.OK.TabIndex = 5
        Me.OK.Text = "确定"
        Me.OK.UseVisualStyleBackColor = True
        '
        'Cancel
        '
        Me.Cancel.Location = New System.Drawing.Point(424, 360)
        Me.Cancel.Name = "Cancel"
        Me.Cancel.Size = New System.Drawing.Size(75, 23)
        Me.Cancel.TabIndex = 4
        Me.Cancel.Text = "取消"
        Me.Cancel.UseVisualStyleBackColor = True
        '
        'Camera_List
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(503, 390)
        Me.Controls.Add(Me.BN_Refresh)
        Me.Controls.Add(Me.OK)
        Me.Controls.Add(Me.Cancel)
        Me.Controls.Add(Me.CameraList)
        Me.Name = "Camera_List"
        Me.Text = "Camera_List"
        Me.ResumeLayout(False)

    End Sub
    Public WithEvents CameraList As System.Windows.Forms.ListView
    Private WithEvents BN_Refresh As System.Windows.Forms.Button
    Private WithEvents OK As System.Windows.Forms.Button
    Private WithEvents Cancel As System.Windows.Forms.Button
End Class
