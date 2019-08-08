namespace Threshold
{
    partial class Threshold
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
            this.ContinueSnap = new System.Windows.Forms.Button();
            this.Save = new System.Windows.Forms.Button();
            this.StopSnap = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.trackBar_Thres = new System.Windows.Forms.TrackBar();
            this.textBox_Thres = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.checkBox_Binary = new System.Windows.Forms.CheckBox();
            this.checkBox_Invert = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Thres)).BeginInit();
            this.SuspendLayout();
            // 
            // ContinueSnap
            // 
            this.ContinueSnap.Location = new System.Drawing.Point(12, 10);
            this.ContinueSnap.Name = "ContinueSnap";
            this.ContinueSnap.Size = new System.Drawing.Size(75, 23);
            this.ContinueSnap.TabIndex = 0;
            this.ContinueSnap.Text = "连续采集";
            this.ContinueSnap.UseVisualStyleBackColor = true;
            this.ContinueSnap.Click += new System.EventHandler(this.ContinueSnap_Click);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(199, 10);
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
            this.StopSnap.Location = new System.Drawing.Point(106, 10);
            this.StopSnap.Name = "StopSnap";
            this.StopSnap.Size = new System.Drawing.Size(75, 23);
            this.StopSnap.TabIndex = 3;
            this.StopSnap.Text = "停止采集";
            this.StopSnap.UseVisualStyleBackColor = true;
            this.StopSnap.Click += new System.EventHandler(this.StopSnap_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.Filter = "Bitmap|*.bmp|JPEG|*.jpg";
            // 
            // trackBar_Thres
            // 
            this.trackBar_Thres.AutoSize = false;
            this.trackBar_Thres.Location = new System.Drawing.Point(369, 10);
            this.trackBar_Thres.Name = "trackBar_Thres";
            this.trackBar_Thres.Size = new System.Drawing.Size(215, 23);
            this.trackBar_Thres.TabIndex = 4;
            this.trackBar_Thres.Scroll += new System.EventHandler(this.trackBar_Thres_Scroll);
            // 
            // textBox_Thres
            // 
            this.textBox_Thres.Location = new System.Drawing.Point(590, 10);
            this.textBox_Thres.Name = "textBox_Thres";
            this.textBox_Thres.Size = new System.Drawing.Size(36, 21);
            this.textBox_Thres.TabIndex = 5;
            this.textBox_Thres.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_Thres_KeyPress);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(326, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "阈值：";
            // 
            // checkBox_Binary
            // 
            this.checkBox_Binary.AutoSize = true;
            this.checkBox_Binary.Location = new System.Drawing.Point(657, 12);
            this.checkBox_Binary.Name = "checkBox_Binary";
            this.checkBox_Binary.Size = new System.Drawing.Size(60, 16);
            this.checkBox_Binary.TabIndex = 7;
            this.checkBox_Binary.Text = "二值化";
            this.checkBox_Binary.UseVisualStyleBackColor = true;
            this.checkBox_Binary.CheckedChanged += new System.EventHandler(this.checkBox_Binary_CheckedChanged);
            // 
            // checkBox_Invert
            // 
            this.checkBox_Invert.AutoSize = true;
            this.checkBox_Invert.Location = new System.Drawing.Point(737, 12);
            this.checkBox_Invert.Name = "checkBox_Invert";
            this.checkBox_Invert.Size = new System.Drawing.Size(48, 16);
            this.checkBox_Invert.TabIndex = 7;
            this.checkBox_Invert.Text = "反色";
            this.checkBox_Invert.UseVisualStyleBackColor = true;
            this.checkBox_Invert.CheckedChanged += new System.EventHandler(this.checkBox_Invert_CheckedChanged);
            // 
            // Threshold
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.checkBox_Invert);
            this.Controls.Add(this.checkBox_Binary);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_Thres);
            this.Controls.Add(this.trackBar_Thres);
            this.Controls.Add(this.StopSnap);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.ContinueSnap);
            this.Name = "Threshold";
            this.Text = "Threshold";
            this.Load += new System.EventHandler(this.Threshold_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Threshold_Paint);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Thres)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ContinueSnap;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.Button StopSnap;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.TrackBar trackBar_Thres;
        private System.Windows.Forms.TextBox textBox_Thres;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox checkBox_Binary;
        private System.Windows.Forms.CheckBox checkBox_Invert;
    }
}

