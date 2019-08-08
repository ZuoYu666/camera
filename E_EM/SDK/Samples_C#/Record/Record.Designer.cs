namespace Record
{
    partial class Record
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
            this.StopSnap = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.Record_Control = new System.Windows.Forms.Button();
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
            // StopSnap
            // 
            this.StopSnap.Enabled = false;
            this.StopSnap.Location = new System.Drawing.Point(100, 10);
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
            // Record_Control
            // 
            this.Record_Control.Location = new System.Drawing.Point(188, 10);
            this.Record_Control.Name = "Record_Control";
            this.Record_Control.Size = new System.Drawing.Size(75, 23);
            this.Record_Control.TabIndex = 0;
            this.Record_Control.Text = "录像控制";
            this.Record_Control.UseVisualStyleBackColor = true;
            this.Record_Control.Click += new System.EventHandler(this.Record_Control_Click);
            // 
            // Record
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.Record_Control);
            this.Controls.Add(this.StopSnap);
            this.Controls.Add(this.CaptureSnap);
            this.Name = "Record";
            this.Text = "Record";
            this.Load += new System.EventHandler(this.Record_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Record_Paint);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Record_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button CaptureSnap;
        private System.Windows.Forms.Button StopSnap;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button Record_Control;
    }
}

