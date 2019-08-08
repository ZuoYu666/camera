namespace TriggerCount
{
    partial class TriggerCount
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
            this.components = new System.ComponentModel.Container();
            this.SyncSnap = new System.Windows.Forms.Button();
            this.Save = new System.Windows.Forms.Button();
            this.StopSnap = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.SoftwareSnap = new System.Windows.Forms.Button();
            this.CaptureSnap = new System.Windows.Forms.Button();
            this.comboBox_Activation = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_Capture = new System.Windows.Forms.TextBox();
            this.textBox_Receive = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.timer_Update_Frame = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // SyncSnap
            // 
            this.SyncSnap.Location = new System.Drawing.Point(186, 9);
            this.SyncSnap.Name = "SyncSnap";
            this.SyncSnap.Size = new System.Drawing.Size(75, 23);
            this.SyncSnap.TabIndex = 0;
            this.SyncSnap.Text = "外触发";
            this.SyncSnap.UseVisualStyleBackColor = true;
            this.SyncSnap.Click += new System.EventHandler(this.SyncSnap_Click);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(360, 9);
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
            this.StopSnap.Location = new System.Drawing.Point(273, 9);
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
            // SoftwareSnap
            // 
            this.SoftwareSnap.Location = new System.Drawing.Point(99, 9);
            this.SoftwareSnap.Name = "SoftwareSnap";
            this.SoftwareSnap.Size = new System.Drawing.Size(75, 23);
            this.SoftwareSnap.TabIndex = 0;
            this.SoftwareSnap.Text = "软触发";
            this.SoftwareSnap.UseVisualStyleBackColor = true;
            this.SoftwareSnap.Click += new System.EventHandler(this.SoftwareSnap_Click);
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
            // comboBox_Activation
            // 
            this.comboBox_Activation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Activation.FormattingEnabled = true;
            this.comboBox_Activation.Location = new System.Drawing.Point(579, 10);
            this.comboBox_Activation.Name = "comboBox_Activation";
            this.comboBox_Activation.Size = new System.Drawing.Size(83, 20);
            this.comboBox_Activation.TabIndex = 4;
            this.comboBox_Activation.SelectedIndexChanged += new System.EventHandler(this.comboBox_Activation_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(497, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "触发极性：";
            // 
            // textBox_Capture
            // 
            this.textBox_Capture.Enabled = false;
            this.textBox_Capture.Location = new System.Drawing.Point(772, 10);
            this.textBox_Capture.Name = "textBox_Capture";
            this.textBox_Capture.Size = new System.Drawing.Size(76, 21);
            this.textBox_Capture.TabIndex = 6;
            // 
            // textBox_Receive
            // 
            this.textBox_Receive.Enabled = false;
            this.textBox_Receive.Location = new System.Drawing.Point(966, 10);
            this.textBox_Receive.Name = "textBox_Receive";
            this.textBox_Receive.Size = new System.Drawing.Size(76, 21);
            this.textBox_Receive.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(697, 14);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 12);
            this.label2.TabIndex = 7;
            this.label2.Text = "拍摄数量:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(883, 14);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "收到数量:";
            // 
            // timer_Update_Frame
            // 
            this.timer_Update_Frame.Tick += new System.EventHandler(this.timer_Update_Frame_Tick);
            // 
            // TriggerCount
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1446, 841);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_Receive);
            this.Controls.Add(this.textBox_Capture);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBox_Activation);
            this.Controls.Add(this.StopSnap);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.CaptureSnap);
            this.Controls.Add(this.SoftwareSnap);
            this.Controls.Add(this.SyncSnap);
            this.Name = "TriggerCount";
            this.Text = "TriggerCount";
            this.Load += new System.EventHandler(this.TriggerCount_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.TriggerCount_Paint);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TriggerCount_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SyncSnap;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.Button StopSnap;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button SoftwareSnap;
        private System.Windows.Forms.Button CaptureSnap;
        private System.Windows.Forms.ComboBox comboBox_Activation;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_Capture;
        private System.Windows.Forms.TextBox textBox_Receive;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Timer timer_Update_Frame;
    }
}

