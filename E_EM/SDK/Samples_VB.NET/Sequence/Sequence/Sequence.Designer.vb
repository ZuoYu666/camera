<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Sequence
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
        Me.BN_Property = New System.Windows.Forms.Button()
        Me.Sequence_Control = New System.Windows.Forms.Button()
        Me.StopSnap = New System.Windows.Forms.Button()
        Me.saveFileDialog1 = New System.Windows.Forms.SaveFileDialog()
        Me.SuspendLayout()
        '
        'CaptureSnap
        '
        Me.CaptureSnap.Location = New System.Drawing.Point(12, 12)
        Me.CaptureSnap.Name = "CaptureSnap"
        Me.CaptureSnap.Size = New System.Drawing.Size(75, 23)
        Me.CaptureSnap.TabIndex = 1
        Me.CaptureSnap.Text = "采集"
        Me.CaptureSnap.UseVisualStyleBackColor = True
        '
        'BN_Property
        '
        Me.BN_Property.Location = New System.Drawing.Point(177, 12)
        Me.BN_Property.Name = "BN_Property"
        Me.BN_Property.Size = New System.Drawing.Size(75, 23)
        Me.BN_Property.TabIndex = 7
        Me.BN_Property.Text = "相机属性"
        Me.BN_Property.UseVisualStyleBackColor = True
        '
        'Sequence_Control
        '
        Me.Sequence_Control.Location = New System.Drawing.Point(258, 12)
        Me.Sequence_Control.Name = "Sequence_Control"
        Me.Sequence_Control.Size = New System.Drawing.Size(75, 23)
        Me.Sequence_Control.TabIndex = 5
        Me.Sequence_Control.Text = "队列控制"
        Me.Sequence_Control.UseVisualStyleBackColor = True
        '
        'StopSnap
        '
        Me.StopSnap.Enabled = False
        Me.StopSnap.Location = New System.Drawing.Point(96, 12)
        Me.StopSnap.Name = "StopSnap"
        Me.StopSnap.Size = New System.Drawing.Size(75, 23)
        Me.StopSnap.TabIndex = 6
        Me.StopSnap.Text = "停止"
        Me.StopSnap.UseVisualStyleBackColor = True
        '
        'saveFileDialog1
        '
        Me.saveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg"
        '
        'Sequence
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.BN_Property)
        Me.Controls.Add(Me.Sequence_Control)
        Me.Controls.Add(Me.StopSnap)
        Me.Controls.Add(Me.CaptureSnap)
        Me.Name = "Sequence"
        Me.Text = "Sequence"
        Me.ResumeLayout(False)

    End Sub
    Private WithEvents CaptureSnap As System.Windows.Forms.Button
    Private WithEvents BN_Property As System.Windows.Forms.Button
    Private WithEvents Sequence_Control As System.Windows.Forms.Button
    Private WithEvents StopSnap As System.Windows.Forms.Button
    Private WithEvents saveFileDialog1 As System.Windows.Forms.SaveFileDialog

End Class
