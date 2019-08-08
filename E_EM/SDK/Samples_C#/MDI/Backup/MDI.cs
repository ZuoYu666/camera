using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MVGigE=MVAPI.MVGigE ;
using MVSTATUS = MVAPI.MVSTATUS_CODES;
using System.Diagnostics;

namespace MDI
{
    public partial class MDI : Form
    {
        IntPtr m_hCam=IntPtr.Zero;
        public MDI()
        {
            InitializeComponent();
        }

        private void MDI_Load(object sender, EventArgs e)
        {

            MVGigE.MVInitLib();
        }

        private void startToolStripMenuItem_Click(object sender, EventArgs e)
        {
           if (ActiveMdiChild != null)
           {
               ((Child)ActiveMdiChild).CaptureSnap();
           }
        }

        private void New_Window_Click(object sender, EventArgs e)
        {
            ToolStripItem tm = (ToolStripItem)(sender);
            string str = tm.Text;
            foreach (Form form in Application.OpenForms)
            {
                if (form.GetType() == typeof(Child) && form.Text == str)
                {
                    form.Activate();
                }
            }
        }

        public void Remove(string key)
        {
            this.WND_ToolStripMenuItem.DropDownItems.RemoveByKey(key);
        }
                        
        private void Start_Click(object sender, EventArgs e)
        {
            MessageBox.Show(ActiveMdiChild.Text);
        }

        private void stopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                ((Child)ActiveMdiChild).StopSnap();
            }
        }

        private bool IsThereForm( string CamInfo)
        {
            foreach (Form form in Application.OpenForms)
            {
                if (form.GetType() == typeof(Child) && form.Text == CamInfo)
                {
                    return true;
                }
            }
            return false;
        }

        private void propertyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                ((Child)ActiveMdiChild).Property();
            }
        }

        private void MDI_FormClosing(object sender, FormClosingEventArgs e)
        {
            MVGigE.MVTerminateLib();
        }

        private void New_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Camera_List lst = new Camera_List();
            if (lst.ShowDialog() == DialogResult.OK)
            {
                MVAPI.MVCamInfo Info;
                string CamInfo;

                int nCount = lst.CameraList.SelectedItems.Count;

                for (int i = 0; i < nCount; i++)
                {
                    if (lst.CameraList.SelectedItems[i].Text == "否")
                    {
                        continue ;
                    }
                    int idx = (int)(lst.CameraList.SelectedItems[i].Tag);
                    MVGigE.MVGetCameraInfo((byte)idx, out Info);
                    CamInfo = Info.mModelName + " " + Info.mSerialNumber;

                    ToolStripItem t = this.WND_ToolStripMenuItem.DropDownItems.Add(CamInfo, null, new EventHandler(New_Window_Click));

                    t.Name = CamInfo;

                    Child ChildForm = new Child();
                    ChildForm.m_nCam = (byte)idx;
                    ChildForm.MdiParent = this;
                    ChildForm.Name = CamInfo;
                    ChildForm.Text = CamInfo;
                    ChildForm.Show();
                }

            }
        }

        private void Save_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                ((Child)ActiveMdiChild).Save();
            }
            else
            {
                MessageBox.Show("请打开相机");
            }
        }

        private void Cascade_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi( MdiLayout.Cascade );
        }

        private void Tile_Vertical_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(MdiLayout.TileVertical);
        }

        private void Tile_Horizontal_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(MdiLayout.TileHorizontal);
        }

        private void menuStrip1_MenuActivate(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                startToolStripMenuItem.Enabled = !((Child)ActiveMdiChild).m_bRun;
                stopToolStripMenuItem.Enabled = ((Child)ActiveMdiChild).m_bRun;
                propertyToolStripMenuItem.Enabled = true;
            }
            else
            {
                startToolStripMenuItem.Enabled = false;
                stopToolStripMenuItem.Enabled = false;
                propertyToolStripMenuItem.Enabled = false;
            }

            if (ActiveMdiChild != null)
            {
                for (int i = 3; i < WND_ToolStripMenuItem.DropDownItems.Count; i++)
                {
                    if (ActiveMdiChild.Text == WND_ToolStripMenuItem.DropDownItems[i].Name)
                    {
                        ((ToolStripMenuItem)WND_ToolStripMenuItem.DropDownItems[i]).Checked = true;
                    }
                    else
                    {
                        ((ToolStripMenuItem)WND_ToolStripMenuItem.DropDownItems[i]).Checked = false;
                    }
                }
            }

            if (ActiveMdiChild == null)
            {
                if (fileToolStripMenuItem.DropDownItems[1].Text == "关闭")
                {
                    fileToolStripMenuItem.DropDownItems.RemoveAt(1);
                }
            }
            else
            {
                if (fileToolStripMenuItem.DropDownItems[1].Text != "关闭")
                {
                    fileToolStripMenuItem.DropDownItems.Insert(1, Close_ToolStripMenuItem);
                }
            }
        }

        private void Close_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
                this.ActiveMdiChild.Close();
        }

        private void Quit_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }

}
