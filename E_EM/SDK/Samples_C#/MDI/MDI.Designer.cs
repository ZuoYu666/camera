namespace MDI
{
    partial class MDI
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Close_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.New_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Save_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Quit_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Camera_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.startToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.propertyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.WND_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Cascade_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Tile_Horizontal_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Tile_Vertical_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.New_ToolStripMenuItem,
            this.Close_ToolStripMenuItem,
            this.Save_ToolStripMenuItem,
            this.Quit_ToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.fileToolStripMenuItem.Text = "文件";
            // 
            // Close_ToolStripMenuItem
            // 
            this.Close_ToolStripMenuItem.Name = "Close_ToolStripMenuItem";
            this.Close_ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.Close_ToolStripMenuItem.Text = "关闭";
            this.Close_ToolStripMenuItem.Click += new System.EventHandler(this.Close_ToolStripMenuItem_Click);
            // 
            // New_ToolStripMenuItem
            // 
            this.New_ToolStripMenuItem.Name = "New_ToolStripMenuItem";
            this.New_ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.New_ToolStripMenuItem.Text = "新建";
            this.New_ToolStripMenuItem.Click += new System.EventHandler(this.New_ToolStripMenuItem_Click);
            // 
            // Save_ToolStripMenuItem
            // 
            this.Save_ToolStripMenuItem.Name = "Save_ToolStripMenuItem";
            this.Save_ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.Save_ToolStripMenuItem.Text = "保存";
            this.Save_ToolStripMenuItem.Click += new System.EventHandler(this.Save_ToolStripMenuItem_Click);
            // 
            // Quit_ToolStripMenuItem
            // 
            this.Quit_ToolStripMenuItem.Name = "Quit_ToolStripMenuItem";
            this.Quit_ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.Quit_ToolStripMenuItem.Text = "退出";
            this.Quit_ToolStripMenuItem.Click += new System.EventHandler(this.Quit_ToolStripMenuItem_Click);
            // 
            // Camera_ToolStripMenuItem
            // 
            this.Camera_ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.startToolStripMenuItem,
            this.stopToolStripMenuItem,
            this.propertyToolStripMenuItem});
            this.Camera_ToolStripMenuItem.Name = "Camera_ToolStripMenuItem";
            this.Camera_ToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.Camera_ToolStripMenuItem.Text = "相机";
            // 
            // startToolStripMenuItem
            // 
            this.startToolStripMenuItem.Name = "startToolStripMenuItem";
            this.startToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.startToolStripMenuItem.Text = "开始采集";
            this.startToolStripMenuItem.Click += new System.EventHandler(this.startToolStripMenuItem_Click);
            // 
            // stopToolStripMenuItem
            // 
            this.stopToolStripMenuItem.Name = "stopToolStripMenuItem";
            this.stopToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.stopToolStripMenuItem.Text = "停止采集";
            this.stopToolStripMenuItem.Click += new System.EventHandler(this.stopToolStripMenuItem_Click);
            // 
            // propertyToolStripMenuItem
            // 
            this.propertyToolStripMenuItem.Name = "propertyToolStripMenuItem";
            this.propertyToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.propertyToolStripMenuItem.Text = "属性设置";
            this.propertyToolStripMenuItem.Click += new System.EventHandler(this.propertyToolStripMenuItem_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.Camera_ToolStripMenuItem,
            this.WND_ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1446, 25);
            this.menuStrip1.TabIndex = 6;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.MenuActivate += new System.EventHandler(this.menuStrip1_MenuActivate);
            // 
            // WND_ToolStripMenuItem
            // 
            this.WND_ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Cascade_ToolStripMenuItem,
            this.Tile_Horizontal_ToolStripMenuItem,
            this.Tile_Vertical_ToolStripMenuItem});
            this.WND_ToolStripMenuItem.Name = "WND_ToolStripMenuItem";
            this.WND_ToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.WND_ToolStripMenuItem.Text = "窗口";
            // 
            // Cascade_ToolStripMenuItem
            // 
            this.Cascade_ToolStripMenuItem.Name = "Cascade_ToolStripMenuItem";
            this.Cascade_ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.Cascade_ToolStripMenuItem.Text = "层叠";
            this.Cascade_ToolStripMenuItem.Click += new System.EventHandler(this.Cascade_ToolStripMenuItem_Click);
            // 
            // Tile_Horizontal_ToolStripMenuItem
            // 
            this.Tile_Horizontal_ToolStripMenuItem.Name = "Tile_Horizontal_ToolStripMenuItem";
            this.Tile_Horizontal_ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.Tile_Horizontal_ToolStripMenuItem.Text = "水平平铺";
            this.Tile_Horizontal_ToolStripMenuItem.Click += new System.EventHandler(this.Tile_Horizontal_ToolStripMenuItem_Click);
            // 
            // Tile_Vertical_ToolStripMenuItem
            // 
            this.Tile_Vertical_ToolStripMenuItem.Name = "Tile_Vertical_ToolStripMenuItem";
            this.Tile_Vertical_ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.Tile_Vertical_ToolStripMenuItem.Text = "垂直平铺";
            this.Tile_Vertical_ToolStripMenuItem.Click += new System.EventHandler(this.Tile_Vertical_ToolStripMenuItem_Click);
            // 
            // MDI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.menuStrip1);
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MDI";
            this.Text = "MDI";
            this.Load += new System.EventHandler(this.MDI_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MDI_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Camera_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem startToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stopToolStripMenuItem;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem propertyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem New_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Save_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem WND_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Cascade_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Tile_Horizontal_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Tile_Vertical_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Close_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Quit_ToolStripMenuItem;
    }
}

