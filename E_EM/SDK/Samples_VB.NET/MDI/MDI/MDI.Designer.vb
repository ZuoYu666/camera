<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class MDI
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
        Me.menuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.fileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.New_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Close_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Save_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Quit_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Camera_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.startToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.stopToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.propertyToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.WND_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Cascade_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Tile_Horizontal_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.Tile_Vertical_ToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.menuStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'menuStrip1
        '
        Me.menuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.fileToolStripMenuItem, Me.Camera_ToolStripMenuItem, Me.WND_ToolStripMenuItem})
        Me.menuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.menuStrip1.Name = "menuStrip1"
        Me.menuStrip1.Size = New System.Drawing.Size(1444, 25)
        Me.menuStrip1.TabIndex = 7
        Me.menuStrip1.Text = "menuStrip1"
        '
        'fileToolStripMenuItem
        '
        Me.fileToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.New_ToolStripMenuItem, Me.Close_ToolStripMenuItem, Me.Save_ToolStripMenuItem, Me.Quit_ToolStripMenuItem})
        Me.fileToolStripMenuItem.Name = "fileToolStripMenuItem"
        Me.fileToolStripMenuItem.Size = New System.Drawing.Size(44, 21)
        Me.fileToolStripMenuItem.Text = "文件"
        '
        'New_ToolStripMenuItem
        '
        Me.New_ToolStripMenuItem.Name = "New_ToolStripMenuItem"
        Me.New_ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
        Me.New_ToolStripMenuItem.Text = "新建"
        '
        'Close_ToolStripMenuItem
        '
        Me.Close_ToolStripMenuItem.Name = "Close_ToolStripMenuItem"
        Me.Close_ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
        Me.Close_ToolStripMenuItem.Text = "关闭"
        '
        'Save_ToolStripMenuItem
        '
        Me.Save_ToolStripMenuItem.Name = "Save_ToolStripMenuItem"
        Me.Save_ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
        Me.Save_ToolStripMenuItem.Text = "保存"
        '
        'Quit_ToolStripMenuItem
        '
        Me.Quit_ToolStripMenuItem.Name = "Quit_ToolStripMenuItem"
        Me.Quit_ToolStripMenuItem.Size = New System.Drawing.Size(152, 22)
        Me.Quit_ToolStripMenuItem.Text = "退出"
        '
        'Camera_ToolStripMenuItem
        '
        Me.Camera_ToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.startToolStripMenuItem, Me.stopToolStripMenuItem, Me.propertyToolStripMenuItem})
        Me.Camera_ToolStripMenuItem.Name = "Camera_ToolStripMenuItem"
        Me.Camera_ToolStripMenuItem.Size = New System.Drawing.Size(44, 21)
        Me.Camera_ToolStripMenuItem.Text = "相机"
        '
        'startToolStripMenuItem
        '
        Me.startToolStripMenuItem.Name = "startToolStripMenuItem"
        Me.startToolStripMenuItem.Size = New System.Drawing.Size(124, 22)
        Me.startToolStripMenuItem.Text = "开始采集"
        '
        'stopToolStripMenuItem
        '
        Me.stopToolStripMenuItem.Name = "stopToolStripMenuItem"
        Me.stopToolStripMenuItem.Size = New System.Drawing.Size(124, 22)
        Me.stopToolStripMenuItem.Text = "停止采集"
        '
        'propertyToolStripMenuItem
        '
        Me.propertyToolStripMenuItem.Name = "propertyToolStripMenuItem"
        Me.propertyToolStripMenuItem.Size = New System.Drawing.Size(124, 22)
        Me.propertyToolStripMenuItem.Text = "属性设置"
        '
        'WND_ToolStripMenuItem
        '
        Me.WND_ToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.Cascade_ToolStripMenuItem, Me.Tile_Horizontal_ToolStripMenuItem, Me.Tile_Vertical_ToolStripMenuItem})
        Me.WND_ToolStripMenuItem.Name = "WND_ToolStripMenuItem"
        Me.WND_ToolStripMenuItem.Size = New System.Drawing.Size(44, 21)
        Me.WND_ToolStripMenuItem.Text = "窗口"
        '
        'Cascade_ToolStripMenuItem
        '
        Me.Cascade_ToolStripMenuItem.Name = "Cascade_ToolStripMenuItem"
        Me.Cascade_ToolStripMenuItem.Size = New System.Drawing.Size(124, 22)
        Me.Cascade_ToolStripMenuItem.Text = "层叠"
        '
        'Tile_Horizontal_ToolStripMenuItem
        '
        Me.Tile_Horizontal_ToolStripMenuItem.Name = "Tile_Horizontal_ToolStripMenuItem"
        Me.Tile_Horizontal_ToolStripMenuItem.Size = New System.Drawing.Size(124, 22)
        Me.Tile_Horizontal_ToolStripMenuItem.Text = "水平平铺"
        '
        'Tile_Vertical_ToolStripMenuItem
        '
        Me.Tile_Vertical_ToolStripMenuItem.Name = "Tile_Vertical_ToolStripMenuItem"
        Me.Tile_Vertical_ToolStripMenuItem.Size = New System.Drawing.Size(124, 22)
        Me.Tile_Vertical_ToolStripMenuItem.Text = "垂直平铺"
        '
        'MDI
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.SystemColors.ActiveBorder
        Me.ClientSize = New System.Drawing.Size(1444, 841)
        Me.Controls.Add(Me.menuStrip1)
        Me.IsMdiContainer = True
        Me.MainMenuStrip = Me.menuStrip1
        Me.Name = "MDI"
        Me.Text = "MDI"
        Me.menuStrip1.ResumeLayout(False)
        Me.menuStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents menuStrip1 As System.Windows.Forms.MenuStrip
    Private WithEvents fileToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents New_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Close_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Save_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Quit_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Camera_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents startToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents stopToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents propertyToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents WND_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Cascade_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Tile_Horizontal_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents Tile_Vertical_ToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem

End Class
