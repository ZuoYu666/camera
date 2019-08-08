using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MVGigE = MVAPI.MVGigE;
using MVSTATUS = MVAPI.MVSTATUS_CODES;

namespace MDI
{
    public partial class Camera_List : Form
    {
        public Camera_List()
        {
            InitializeComponent();
        }

        private void Camera_List_Load(object sender, EventArgs e)
        {
            ColumnHeader[] ch = new ColumnHeader[3];
            ch[0] = new ColumnHeader();
            ch[1] = new ColumnHeader();
            ch[2] = new ColumnHeader();
            ch[0].Text = "允许访问";
            ch[1].Text = "型号";
            ch[2].Text = "序列号";
            ch[0].Width = 65;
            ch[1].Width = 120;
            ch[2].Width = 120;
            ch[0].TextAlign = HorizontalAlignment.Left;
            ch[1].TextAlign = HorizontalAlignment.Left;
            ch[2].TextAlign = HorizontalAlignment.Left;
            this.CameraList.Columns.AddRange(ch);
            ListViewItem lvi = new ListViewItem();
            BN_Refresh_Click(BN_Refresh, null);
        }

        private void BN_Refresh_Click(object sender, EventArgs e)
        {
            bool isfirst = true;
            bool bSelect = false;
	        int nCameraCount = 0;
            CameraList.Items.Clear();
	        MVGigE.MVUpdateCameraList();

	        if (MVGigE.MVGetNumOfCameras(out nCameraCount) == MVAPI.MVSTATUS_CODES.MVST_SUCCESS)
	        {
		        for(int i = 0; i < nCameraCount; i++)
		        {
			        MVAPI.MVCamInfo info;
                    MVGigE.MVGetCameraInfo((byte)i, out info);

			        IntPtr hCam;
			        string str = "否";
                    if (MVGigE.MVOpenCamByIndex((byte)i, out hCam) == MVAPI.MVSTATUS_CODES.MVST_SUCCESS)
			        {
                        str = "是";
				        MVGigE.MVCloseCam(hCam);
                        if (isfirst == true)
                        {
                            bSelect = true;
                            isfirst = false;
                        }
			        }

                    ListViewItem lvi = new ListViewItem();
                    lvi.Tag = i;
                    lvi.Text = str;
                    lvi.SubItems.Add(info.mModelName);
                    lvi.SubItems.Add(info.mSerialNumber);
                    this.CameraList.Items.Add(lvi);
                    if (bSelect == true)
                    {
                        this.CameraList.Items[i].Selected = true;
                    }
		        }
	        }
	        else
	        {
                MessageBox.Show("无法刷新列表");
	        }
        }

        private void OK_Click(object sender, EventArgs e)
        {
            int nCount = this.CameraList.SelectedItems.Count;
            if (nCount <= 0)
            {
                MessageBox.Show("没有选择相机或所有没有可访问的相机");
            }
            else
            {
                if (this.CameraList.SelectedItems[0].Text != "否")
                {
                    this.DialogResult = DialogResult.OK;
                }
                else
                {
                    MessageBox.Show("选中的相机无法访问");
                }
            }
            
        }
        private void Cancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }
    }
}
