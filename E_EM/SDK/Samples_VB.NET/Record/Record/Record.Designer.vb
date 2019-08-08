<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Record
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
        Me.CaptureSnap = New System.Windows.Forms.Button()
        Me.StopSnap = New System.Windows.Forms.Button()
        Me.Record_Control = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'CaptureSnap
        '
        Me.CaptureSnap.Location = New System.Drawing.Point(27, 8)
        Me.CaptureSnap.Name = "CaptureSnap"
        Me.CaptureSnap.Size = New System.Drawing.Size(75, 23)
        Me.CaptureSnap.TabIndex = 0
        Me.CaptureSnap.Text = "采集"
        Me.CaptureSnap.UseVisualStyleBackColor = True
        '
        'StopSnap
        '
        Me.StopSnap.Location = New System.Drawing.Point(125, 8)
        Me.StopSnap.Name = "StopSnap"
        Me.StopSnap.Size = New System.Drawing.Size(75, 23)
        Me.StopSnap.TabIndex = 1
        Me.StopSnap.Text = "停止"
        Me.StopSnap.UseVisualStyleBackColor = True
        '
        'Record_Control
        '
        Me.Record_Control.Location = New System.Drawing.Point(223, 8)
        Me.Record_Control.Name = "Record_Control"
        Me.Record_Control.Size = New System.Drawing.Size(75, 23)
        Me.Record_Control.TabIndex = 2
        Me.Record_Control.Text = "录像控制"
        Me.Record_Control.UseVisualStyleBackColor = True
        '
        'Record
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.Record_Control)
        Me.Controls.Add(Me.StopSnap)
        Me.Controls.Add(Me.CaptureSnap)
        Me.Name = "Record"
        Me.Text = "Record"
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents CaptureSnap As System.Windows.Forms.Button
    Friend WithEvents StopSnap As System.Windows.Forms.Button
    Friend WithEvents Record_Control As System.Windows.Forms.Button

End Class
