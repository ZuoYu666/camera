namespace SingleGrab
{
    partial class SingleGrab
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
            this.CaptureSnap = new System.Windows.Forms.Button();
            this.Save = new System.Windows.Forms.Button();
            this.StopSnap = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.BN_SingleGrab = new System.Windows.Forms.Button();
            this.Property = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // CaptureSnap
            // 
            this.CaptureSnap.Location = new System.Drawing.Point(12, 10);
            this.CaptureSnap.Name = "CaptureSnap";
            this.CaptureSnap.Size = new System.Drawing.Size(75, 23);
            this.CaptureSnap.TabIndex = 0;
            this.CaptureSnap.Text = "采集";
            this.CaptureSnap.UseVisualStyleBackColor = true;
            this.CaptureSnap.Click += new System.EventHandler(this.CaptureSnap_Click);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(297, 10);
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(75, 23);
            this.Save.TabIndex = 2;
            this.Save.Text = "保存";
            this.Save.UseVisualStyleBackColor = true;
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // StopSnap
            // 
            this.StopSnap.Enabled = false;
            this.StopSnap.Location = new System.Drawing.Point(202, 10);
            this.StopSnap.Name = "StopSnap";
            this.StopSnap.Size = new System.Drawing.Size(75, 23);
            this.StopSnap.TabIndex = 3;
            this.StopSnap.Text = "停止";
            this.StopSnap.UseVisualStyleBackColor = true;
            this.StopSnap.Click += new System.EventHandler(this.StopSnap_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg";
            // 
            // BN_SingleGrab
            // 
            this.BN_SingleGrab.Location = new System.Drawing.Point(107, 10);
            this.BN_SingleGrab.Name = "BN_SingleGrab";
            this.BN_SingleGrab.Size = new System.Drawing.Size(75, 23);
            this.BN_SingleGrab.TabIndex = 0;
            this.BN_SingleGrab.Text = "采单帧";
            this.BN_SingleGrab.UseVisualStyleBackColor = true;
            this.BN_SingleGrab.Click += new System.EventHandler(this.BN_SingleGrab_Click);
            // 
            // Property
            // 
            this.Property.Location = new System.Drawing.Point(392, 10);
            this.Property.Name = "Property";
            this.Property.Size = new System.Drawing.Size(75, 23);
            this.Property.TabIndex = 2;
            this.Property.Text = "属性";
            this.Property.UseVisualStyleBackColor = true;
            this.Property.Click += new System.EventHandler(this.Property_Click);
            // 
            // SingleGrab
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.StopSnap);
            this.Controls.Add(this.Property);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.BN_SingleGrab);
            this.Controls.Add(this.CaptureSnap);
            this.Name = "SingleGrab";
            this.Text = "SingleGrab";
            this.Load += new System.EventHandler(this.SingleGrab_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.SingleGrab_Paint);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SingleGrab_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button CaptureSnap;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.Button StopSnap;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button BN_SingleGrab;
        private System.Windows.Forms.Button Property;
    }
}

