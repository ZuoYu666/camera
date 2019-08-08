namespace Sequence
{
    partial class Sequence
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
            this.Sequence_Control = new System.Windows.Forms.Button();
            this.BN_Property = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // CaptureSnap
            // 
            this.CaptureSnap.Location = new System.Drawing.Point(12, 9);
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
            this.StopSnap.Location = new System.Drawing.Point(98, 9);
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
            // Sequence_Control
            // 
            this.Sequence_Control.Location = new System.Drawing.Point(184, 9);
            this.Sequence_Control.Name = "Sequence_Control";
            this.Sequence_Control.Size = new System.Drawing.Size(75, 23);
            this.Sequence_Control.TabIndex = 0;
            this.Sequence_Control.Text = "队列控制";
            this.Sequence_Control.UseVisualStyleBackColor = true;
            this.Sequence_Control.Click += new System.EventHandler(this.Sequence_Control_Click);
            // 
            // BN_Property
            // 
            this.BN_Property.Location = new System.Drawing.Point(270, 9);
            this.BN_Property.Name = "BN_Property";
            this.BN_Property.Size = new System.Drawing.Size(75, 23);
            this.BN_Property.TabIndex = 4;
            this.BN_Property.Text = "相机属性";
            this.BN_Property.UseVisualStyleBackColor = true;
            this.BN_Property.Click += new System.EventHandler(this.BN_Property_Click);
            // 
            // Sequence
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.BN_Property);
            this.Controls.Add(this.Sequence_Control);
            this.Controls.Add(this.StopSnap);
            this.Controls.Add(this.CaptureSnap);
            this.Name = "Sequence";
            this.Text = "Sequence";
            this.Load += new System.EventHandler(this.Sequence_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Sequence_Paint);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Sequence_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button CaptureSnap;
        private System.Windows.Forms.Button StopSnap;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button Sequence_Control;
        private System.Windows.Forms.Button BN_Property;
    }
}

