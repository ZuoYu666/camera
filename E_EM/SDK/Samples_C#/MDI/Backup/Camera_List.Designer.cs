namespace MDI
{
    partial class Camera_List
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Cancel = new System.Windows.Forms.Button();
            this.OK = new System.Windows.Forms.Button();
            this.BN_Refresh = new System.Windows.Forms.Button();
            this.CameraList = new System.Windows.Forms.ListView();
            this.SuspendLayout();
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(426, 365);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(75, 23);
            this.Cancel.TabIndex = 1;
            this.Cancel.Text = "取消";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // OK
            // 
            this.OK.Location = new System.Drawing.Point(345, 365);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(75, 23);
            this.OK.TabIndex = 1;
            this.OK.Text = "确定";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // BN_Refresh
            // 
            this.BN_Refresh.Location = new System.Drawing.Point(264, 365);
            this.BN_Refresh.Name = "BN_Refresh";
            this.BN_Refresh.Size = new System.Drawing.Size(75, 23);
            this.BN_Refresh.TabIndex = 1;
            this.BN_Refresh.Text = "刷新";
            this.BN_Refresh.UseVisualStyleBackColor = true;
            this.BN_Refresh.Click += new System.EventHandler(this.BN_Refresh_Click);
            // 
            // CameraList
            // 
            this.CameraList.FullRowSelect = true;
            this.CameraList.Location = new System.Drawing.Point(12, 12);
            this.CameraList.MultiSelect = false;
            this.CameraList.Name = "CameraList";
            this.CameraList.Size = new System.Drawing.Size(489, 347);
            this.CameraList.TabIndex = 2;
            this.CameraList.UseCompatibleStateImageBehavior = false;
            this.CameraList.View = System.Windows.Forms.View.Details;
            // 
            // Camera_List
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(513, 400);
            this.Controls.Add(this.CameraList);
            this.Controls.Add(this.BN_Refresh);
            this.Controls.Add(this.OK);
            this.Controls.Add(this.Cancel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Camera_List";
            this.Text = "Camera_List";
            this.Load += new System.EventHandler(this.Camera_List_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.Button BN_Refresh;
        public System.Windows.Forms.ListView CameraList;
    }
}