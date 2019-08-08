namespace Cross
{
    partial class Cross
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
            this.comboBox_Rotate = new System.Windows.Forms.ComboBox();
            this.comboBox_Flip = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBox_Zoom = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
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
            this.Save.Location = new System.Drawing.Point(196, 10);
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
            this.StopSnap.Location = new System.Drawing.Point(103, 10);
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
            // comboBox_Rotate
            // 
            this.comboBox_Rotate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Rotate.FormattingEnabled = true;
            this.comboBox_Rotate.Location = new System.Drawing.Point(701, 11);
            this.comboBox_Rotate.Name = "comboBox_Rotate";
            this.comboBox_Rotate.Size = new System.Drawing.Size(121, 20);
            this.comboBox_Rotate.TabIndex = 4;
            this.comboBox_Rotate.SelectedIndexChanged += new System.EventHandler(this.comboBox_Rotate_SelectedIndexChanged);
            // 
            // comboBox_Flip
            // 
            this.comboBox_Flip.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Flip.FormattingEnabled = true;
            this.comboBox_Flip.Location = new System.Drawing.Point(505, 11);
            this.comboBox_Flip.Name = "comboBox_Flip";
            this.comboBox_Flip.Size = new System.Drawing.Size(121, 20);
            this.comboBox_Flip.TabIndex = 4;
            this.comboBox_Flip.SelectedIndexChanged += new System.EventHandler(this.comboBox_Flip_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(450, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "翻转：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(647, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "旋转：";
            // 
            // comboBox_Zoom
            // 
            this.comboBox_Zoom.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Zoom.FormattingEnabled = true;
            this.comboBox_Zoom.Location = new System.Drawing.Point(357, 11);
            this.comboBox_Zoom.Name = "comboBox_Zoom";
            this.comboBox_Zoom.Size = new System.Drawing.Size(65, 20);
            this.comboBox_Zoom.TabIndex = 4;
            this.comboBox_Zoom.SelectedIndexChanged += new System.EventHandler(this.comboBox_Zoom_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(302, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "缩放：";
            // 
            // Cross
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBox_Zoom);
            this.Controls.Add(this.comboBox_Flip);
            this.Controls.Add(this.comboBox_Rotate);
            this.Controls.Add(this.StopSnap);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.CaptureSnap);
            this.Name = "Cross";
            this.Text = "Cross";
            this.Load += new System.EventHandler(this.Cross_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Cross_Paint);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Cross_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CaptureSnap;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.Button StopSnap;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ComboBox comboBox_Rotate;
        private System.Windows.Forms.ComboBox comboBox_Flip;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBox_Zoom;
        private System.Windows.Forms.Label label3;
    }
}

