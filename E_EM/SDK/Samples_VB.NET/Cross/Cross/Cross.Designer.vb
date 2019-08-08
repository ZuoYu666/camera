<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Cross
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
        Me.label2 = New System.Windows.Forms.Label()
        Me.label3 = New System.Windows.Forms.Label()
        Me.label1 = New System.Windows.Forms.Label()
        Me.comboBox_Zoom = New System.Windows.Forms.ComboBox()
        Me.comboBox_Flip = New System.Windows.Forms.ComboBox()
        Me.comboBox_Rotate = New System.Windows.Forms.ComboBox()
        Me.StopSnap = New System.Windows.Forms.Button()
        Me.Save = New System.Windows.Forms.Button()
        Me.CaptureSnap = New System.Windows.Forms.Button()
        Me.SaveFileDialog1 = New System.Windows.Forms.SaveFileDialog()
        Me.SuspendLayout()
        '
        'label2
        '
        Me.label2.AutoSize = True
        Me.label2.Location = New System.Drawing.Point(640, 12)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(41, 12)
        Me.label2.TabIndex = 12
        Me.label2.Text = "旋转："
        '
        'label3
        '
        Me.label3.AutoSize = True
        Me.label3.Location = New System.Drawing.Point(295, 12)
        Me.label3.Name = "label3"
        Me.label3.Size = New System.Drawing.Size(41, 12)
        Me.label3.TabIndex = 13
        Me.label3.Text = "缩放："
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(443, 12)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(41, 12)
        Me.label1.TabIndex = 14
        Me.label1.Text = "翻转："
        '
        'comboBox_Zoom
        '
        Me.comboBox_Zoom.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.comboBox_Zoom.FormattingEnabled = True
        Me.comboBox_Zoom.Location = New System.Drawing.Point(350, 8)
        Me.comboBox_Zoom.Name = "comboBox_Zoom"
        Me.comboBox_Zoom.Size = New System.Drawing.Size(65, 20)
        Me.comboBox_Zoom.TabIndex = 11
        '
        'comboBox_Flip
        '
        Me.comboBox_Flip.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.comboBox_Flip.FormattingEnabled = True
        Me.comboBox_Flip.Location = New System.Drawing.Point(498, 8)
        Me.comboBox_Flip.Name = "comboBox_Flip"
        Me.comboBox_Flip.Size = New System.Drawing.Size(121, 20)
        Me.comboBox_Flip.TabIndex = 10
        '
        'comboBox_Rotate
        '
        Me.comboBox_Rotate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.comboBox_Rotate.FormattingEnabled = True
        Me.comboBox_Rotate.Location = New System.Drawing.Point(694, 8)
        Me.comboBox_Rotate.Name = "comboBox_Rotate"
        Me.comboBox_Rotate.Size = New System.Drawing.Size(121, 20)
        Me.comboBox_Rotate.TabIndex = 9
        '
        'StopSnap
        '
        Me.StopSnap.Enabled = False
        Me.StopSnap.Location = New System.Drawing.Point(96, 7)
        Me.StopSnap.Name = "StopSnap"
        Me.StopSnap.Size = New System.Drawing.Size(75, 23)
        Me.StopSnap.TabIndex = 8
        Me.StopSnap.Text = "停止"
        Me.StopSnap.UseVisualStyleBackColor = True
        '
        'Save
        '
        Me.Save.Location = New System.Drawing.Point(189, 7)
        Me.Save.Name = "Save"
        Me.Save.Size = New System.Drawing.Size(75, 23)
        Me.Save.TabIndex = 7
        Me.Save.Text = "保存"
        Me.Save.UseVisualStyleBackColor = True
        '
        'CaptureSnap
        '
        Me.CaptureSnap.Location = New System.Drawing.Point(5, 7)
        Me.CaptureSnap.Name = "CaptureSnap"
        Me.CaptureSnap.Size = New System.Drawing.Size(75, 23)
        Me.CaptureSnap.TabIndex = 6
        Me.CaptureSnap.Text = "采集"
        Me.CaptureSnap.UseVisualStyleBackColor = True
        '
        'Cross
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.label2)
        Me.Controls.Add(Me.label3)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.comboBox_Zoom)
        Me.Controls.Add(Me.comboBox_Flip)
        Me.Controls.Add(Me.comboBox_Rotate)
        Me.Controls.Add(Me.StopSnap)
        Me.Controls.Add(Me.Save)
        Me.Controls.Add(Me.CaptureSnap)
        Me.Name = "Cross"
        Me.Text = "Cross"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents label2 As System.Windows.Forms.Label
    Private WithEvents label3 As System.Windows.Forms.Label
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents comboBox_Zoom As System.Windows.Forms.ComboBox
    Private WithEvents comboBox_Flip As System.Windows.Forms.ComboBox
    Private WithEvents comboBox_Rotate As System.Windows.Forms.ComboBox
    Private WithEvents StopSnap As System.Windows.Forms.Button
    Private WithEvents Save As System.Windows.Forms.Button
    Private WithEvents CaptureSnap As System.Windows.Forms.Button
    Friend WithEvents SaveFileDialog1 As System.Windows.Forms.SaveFileDialog

End Class
