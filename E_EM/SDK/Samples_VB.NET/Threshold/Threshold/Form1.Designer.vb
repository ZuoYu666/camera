<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Threshold
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
        Me.checkBox_Invert = New System.Windows.Forms.CheckBox()
        Me.checkBox_Binary = New System.Windows.Forms.CheckBox()
        Me.label1 = New System.Windows.Forms.Label()
        Me.textBox_Thres = New System.Windows.Forms.TextBox()
        Me.trackBar_Thres = New System.Windows.Forms.TrackBar()
        Me.StopSnap = New System.Windows.Forms.Button()
        Me.Save = New System.Windows.Forms.Button()
        Me.CaptureSnap = New System.Windows.Forms.Button()
        Me.saveFileDialog1 = New System.Windows.Forms.SaveFileDialog()
        CType(Me.trackBar_Thres, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'checkBox_Invert
        '
        Me.checkBox_Invert.AutoSize = True
        Me.checkBox_Invert.Location = New System.Drawing.Point(737, 14)
        Me.checkBox_Invert.Name = "checkBox_Invert"
        Me.checkBox_Invert.Size = New System.Drawing.Size(48, 16)
        Me.checkBox_Invert.TabIndex = 14
        Me.checkBox_Invert.Text = "反色"
        Me.checkBox_Invert.UseVisualStyleBackColor = True
        '
        'checkBox_Binary
        '
        Me.checkBox_Binary.AutoSize = True
        Me.checkBox_Binary.Location = New System.Drawing.Point(657, 14)
        Me.checkBox_Binary.Name = "checkBox_Binary"
        Me.checkBox_Binary.Size = New System.Drawing.Size(60, 16)
        Me.checkBox_Binary.TabIndex = 15
        Me.checkBox_Binary.Text = "二值化"
        Me.checkBox_Binary.UseVisualStyleBackColor = True
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(326, 14)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(41, 12)
        Me.label1.TabIndex = 13
        Me.label1.Text = "阈值："
        '
        'textBox_Thres
        '
        Me.textBox_Thres.Location = New System.Drawing.Point(590, 12)
        Me.textBox_Thres.Name = "textBox_Thres"
        Me.textBox_Thres.Size = New System.Drawing.Size(36, 21)
        Me.textBox_Thres.TabIndex = 12
        '
        'trackBar_Thres
        '
        Me.trackBar_Thres.AutoSize = False
        Me.trackBar_Thres.Location = New System.Drawing.Point(369, 12)
        Me.trackBar_Thres.Name = "trackBar_Thres"
        Me.trackBar_Thres.Size = New System.Drawing.Size(215, 23)
        Me.trackBar_Thres.TabIndex = 11
        '
        'StopSnap
        '
        Me.StopSnap.Enabled = False
        Me.StopSnap.Location = New System.Drawing.Point(106, 12)
        Me.StopSnap.Name = "StopSnap"
        Me.StopSnap.Size = New System.Drawing.Size(75, 23)
        Me.StopSnap.TabIndex = 10
        Me.StopSnap.Text = "停止采集"
        Me.StopSnap.UseVisualStyleBackColor = True
        '
        'Save
        '
        Me.Save.Location = New System.Drawing.Point(199, 12)
        Me.Save.Name = "Save"
        Me.Save.Size = New System.Drawing.Size(75, 23)
        Me.Save.TabIndex = 9
        Me.Save.Text = "保存"
        Me.Save.UseVisualStyleBackColor = True
        '
        'CaptureSnap
        '
        Me.CaptureSnap.Location = New System.Drawing.Point(12, 12)
        Me.CaptureSnap.Name = "CaptureSnap"
        Me.CaptureSnap.Size = New System.Drawing.Size(75, 23)
        Me.CaptureSnap.TabIndex = 8
        Me.CaptureSnap.Text = "连续采集"
        Me.CaptureSnap.UseVisualStyleBackColor = True
        '
        'saveFileDialog1
        '
        Me.saveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg"
        '
        'Threshold
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.checkBox_Invert)
        Me.Controls.Add(Me.checkBox_Binary)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.textBox_Thres)
        Me.Controls.Add(Me.trackBar_Thres)
        Me.Controls.Add(Me.StopSnap)
        Me.Controls.Add(Me.Save)
        Me.Controls.Add(Me.CaptureSnap)
        Me.Name = "Threshold"
        Me.Text = "Threshold"
        CType(Me.trackBar_Thres, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents checkBox_Invert As System.Windows.Forms.CheckBox
    Private WithEvents checkBox_Binary As System.Windows.Forms.CheckBox
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents textBox_Thres As System.Windows.Forms.TextBox
    Private WithEvents trackBar_Thres As System.Windows.Forms.TrackBar
    Private WithEvents StopSnap As System.Windows.Forms.Button
    Private WithEvents Save As System.Windows.Forms.Button
    Private WithEvents CaptureSnap As System.Windows.Forms.Button
    Private WithEvents saveFileDialog1 As System.Windows.Forms.SaveFileDialog

End Class
