<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class SingleGrab
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
        Me.StopSnap = New System.Windows.Forms.Button()
        Me.Save = New System.Windows.Forms.Button()
        Me.BN_SingleGrab = New System.Windows.Forms.Button()
        Me.CaptureSnap = New System.Windows.Forms.Button()
        Me.Button_Property = New System.Windows.Forms.Button()
        Me.saveFileDialog1 = New System.Windows.Forms.SaveFileDialog()
        Me.SuspendLayout()
        '
        'StopSnap
        '
        Me.StopSnap.Enabled = False
        Me.StopSnap.Location = New System.Drawing.Point(202, 12)
        Me.StopSnap.Name = "StopSnap"
        Me.StopSnap.Size = New System.Drawing.Size(75, 23)
        Me.StopSnap.TabIndex = 7
        Me.StopSnap.Text = "停止"
        Me.StopSnap.UseVisualStyleBackColor = True
        '
        'Save
        '
        Me.Save.Location = New System.Drawing.Point(297, 12)
        Me.Save.Name = "Save"
        Me.Save.Size = New System.Drawing.Size(75, 23)
        Me.Save.TabIndex = 6
        Me.Save.Text = "保存"
        Me.Save.UseVisualStyleBackColor = True
        '
        'BN_SingleGrab
        '
        Me.BN_SingleGrab.Location = New System.Drawing.Point(107, 12)
        Me.BN_SingleGrab.Name = "BN_SingleGrab"
        Me.BN_SingleGrab.Size = New System.Drawing.Size(75, 23)
        Me.BN_SingleGrab.TabIndex = 4
        Me.BN_SingleGrab.Text = "采单帧"
        Me.BN_SingleGrab.UseVisualStyleBackColor = True
        '
        'CaptureSnap
        '
        Me.CaptureSnap.Location = New System.Drawing.Point(12, 12)
        Me.CaptureSnap.Name = "CaptureSnap"
        Me.CaptureSnap.Size = New System.Drawing.Size(75, 23)
        Me.CaptureSnap.TabIndex = 5
        Me.CaptureSnap.Text = "采集"
        Me.CaptureSnap.UseVisualStyleBackColor = True
        '
        'Button_Property
        '
        Me.Button_Property.Location = New System.Drawing.Point(396, 12)
        Me.Button_Property.Name = "Button_Property"
        Me.Button_Property.Size = New System.Drawing.Size(75, 23)
        Me.Button_Property.TabIndex = 8
        Me.Button_Property.Text = "属性"
        Me.Button_Property.UseVisualStyleBackColor = True
        '
        'saveFileDialog1
        '
        Me.saveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg"
        '
        'SingleGrab
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.Button_Property)
        Me.Controls.Add(Me.StopSnap)
        Me.Controls.Add(Me.Save)
        Me.Controls.Add(Me.BN_SingleGrab)
        Me.Controls.Add(Me.CaptureSnap)
        Me.Name = "SingleGrab"
        Me.Text = "SingleGrab"
        Me.ResumeLayout(False)

    End Sub
    Private WithEvents StopSnap As System.Windows.Forms.Button
    Private WithEvents Save As System.Windows.Forms.Button
    Private WithEvents BN_SingleGrab As System.Windows.Forms.Button
    Private WithEvents CaptureSnap As System.Windows.Forms.Button
    Private WithEvents Button_Property As System.Windows.Forms.Button
    Private WithEvents saveFileDialog1 As System.Windows.Forms.SaveFileDialog

End Class
