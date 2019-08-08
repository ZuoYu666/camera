<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class TriggerCount
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
        Me.components = New System.ComponentModel.Container()
        Me.textBox_Receive = New System.Windows.Forms.TextBox()
        Me.textBox_Capture = New System.Windows.Forms.TextBox()
        Me.comboBox_Activation = New System.Windows.Forms.ComboBox()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Save = New System.Windows.Forms.Button()
        Me.StopSnap = New System.Windows.Forms.Button()
        Me.SyncSnap = New System.Windows.Forms.Button()
        Me.SoftwareSnap = New System.Windows.Forms.Button()
        Me.CaptureSnap = New System.Windows.Forms.Button()
        Me.SaveFileDialog1 = New System.Windows.Forms.SaveFileDialog()
        Me.timer_Update_Frame = New System.Windows.Forms.Timer(Me.components)
        Me.SuspendLayout()
        '
        'textBox_Receive
        '
        Me.textBox_Receive.Enabled = False
        Me.textBox_Receive.Location = New System.Drawing.Point(999, 13)
        Me.textBox_Receive.Name = "textBox_Receive"
        Me.textBox_Receive.Size = New System.Drawing.Size(75, 21)
        Me.textBox_Receive.TabIndex = 21
        '
        'textBox_Capture
        '
        Me.textBox_Capture.Enabled = False
        Me.textBox_Capture.Location = New System.Drawing.Point(802, 13)
        Me.textBox_Capture.Name = "textBox_Capture"
        Me.textBox_Capture.Size = New System.Drawing.Size(75, 21)
        Me.textBox_Capture.TabIndex = 20
        '
        'comboBox_Activation
        '
        Me.comboBox_Activation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.comboBox_Activation.ForeColor = System.Drawing.SystemColors.WindowText
        Me.comboBox_Activation.FormattingEnabled = True
        Me.comboBox_Activation.Location = New System.Drawing.Point(631, 13)
        Me.comboBox_Activation.Name = "comboBox_Activation"
        Me.comboBox_Activation.Size = New System.Drawing.Size(75, 20)
        Me.comboBox_Activation.TabIndex = 19
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(928, 17)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(65, 12)
        Me.Label3.TabIndex = 18
        Me.Label3.Text = "收到数量："
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(739, 17)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(65, 12)
        Me.Label2.TabIndex = 17
        Me.Label2.Text = "拍摄数量："
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(560, 17)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(65, 12)
        Me.Label1.TabIndex = 16
        Me.Label1.Text = "触发极性："
        '
        'Save
        '
        Me.Save.Location = New System.Drawing.Point(376, 12)
        Me.Save.Name = "Save"
        Me.Save.Size = New System.Drawing.Size(75, 23)
        Me.Save.TabIndex = 15
        Me.Save.Text = "保存"
        Me.Save.UseVisualStyleBackColor = True
        '
        'StopSnap
        '
        Me.StopSnap.Enabled = False
        Me.StopSnap.Location = New System.Drawing.Point(285, 12)
        Me.StopSnap.Name = "StopSnap"
        Me.StopSnap.Size = New System.Drawing.Size(75, 23)
        Me.StopSnap.TabIndex = 14
        Me.StopSnap.Text = "停止"
        Me.StopSnap.UseVisualStyleBackColor = True
        '
        'SyncSnap
        '
        Me.SyncSnap.Location = New System.Drawing.Point(194, 12)
        Me.SyncSnap.Name = "SyncSnap"
        Me.SyncSnap.Size = New System.Drawing.Size(75, 23)
        Me.SyncSnap.TabIndex = 13
        Me.SyncSnap.Text = "外触发"
        Me.SyncSnap.UseVisualStyleBackColor = True
        '
        'SoftwareSnap
        '
        Me.SoftwareSnap.Location = New System.Drawing.Point(103, 12)
        Me.SoftwareSnap.Name = "SoftwareSnap"
        Me.SoftwareSnap.Size = New System.Drawing.Size(75, 23)
        Me.SoftwareSnap.TabIndex = 12
        Me.SoftwareSnap.Text = "软触发"
        Me.SoftwareSnap.UseVisualStyleBackColor = True
        '
        'CaptureSnap
        '
        Me.CaptureSnap.Location = New System.Drawing.Point(12, 12)
        Me.CaptureSnap.Name = "CaptureSnap"
        Me.CaptureSnap.Size = New System.Drawing.Size(75, 23)
        Me.CaptureSnap.TabIndex = 11
        Me.CaptureSnap.Text = "采集"
        Me.CaptureSnap.UseVisualStyleBackColor = True
        '
        'SaveFileDialog1
        '
        Me.SaveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg"
        '
        'timer_Update_Frame
        '
        '
        'TriggerCount
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.textBox_Receive)
        Me.Controls.Add(Me.textBox_Capture)
        Me.Controls.Add(Me.comboBox_Activation)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.Save)
        Me.Controls.Add(Me.StopSnap)
        Me.Controls.Add(Me.SyncSnap)
        Me.Controls.Add(Me.SoftwareSnap)
        Me.Controls.Add(Me.CaptureSnap)
        Me.Name = "TriggerCount"
        Me.Text = "TriggerCount"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents textBox_Receive As System.Windows.Forms.TextBox
    Friend WithEvents textBox_Capture As System.Windows.Forms.TextBox
    Friend WithEvents comboBox_Activation As System.Windows.Forms.ComboBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Save As System.Windows.Forms.Button
    Friend WithEvents StopSnap As System.Windows.Forms.Button
    Friend WithEvents SyncSnap As System.Windows.Forms.Button
    Friend WithEvents SoftwareSnap As System.Windows.Forms.Button
    Friend WithEvents CaptureSnap As System.Windows.Forms.Button
    Friend WithEvents SaveFileDialog1 As System.Windows.Forms.SaveFileDialog
    Friend WithEvents timer_Update_Frame As System.Windows.Forms.Timer
End Class
