VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   8820
   ClientLeft      =   60
   ClientTop       =   456
   ClientWidth     =   11436
   LinkTopic       =   "Form1"
   ScaleHeight     =   8820
   ScaleWidth      =   11436
   StartUpPosition =   3  'Windows Default
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   5760
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton SaveImage 
      Caption         =   "保存图像"
      Enabled         =   0   'False
      Height          =   375
      Left            =   2760
      TabIndex        =   3
      Top             =   120
      Width           =   1215
   End
   Begin VB.CommandButton SingleGrab 
      Caption         =   "采集1帧"
      Enabled         =   0   'False
      Height          =   375
      Left            =   1440
      TabIndex        =   2
      Top             =   120
      Width           =   1215
   End
   Begin VB.CommandButton OpenCam 
      Caption         =   "打开相机"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
   Begin VB.PictureBox Picture1 
      Height          =   8055
      Left            =   120
      ScaleHeight     =   8004
      ScaleWidth      =   11124
      TabIndex        =   0
      Top             =   600
      Width           =   11175
   End
   Begin VB.Label LabelW 
      Caption         =   "0"
      Height          =   375
      Left            =   4200
      TabIndex        =   4
      Top             =   120
      Width           =   495
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim hCam As Long
Dim hImage As Long


Private Sub Form_Terminate()
    If hCam <> 0 Then
        MVCloseCam hCam
    End If

End Sub

Private Sub OpenCam_Click()
    Dim num As Long
    Dim w As Long
    Dim h As Long
    Dim rt As MVSTATUS_CODES
    Dim pf As MV_PixelFormatEnums
    
    rt = MVGetNumOfCameras(num)
    
    
    If num <= 0 Then
        MsgBox "没有找到相机, 请检查设备连接 !"
    Else
        rt = MVOpenCamByIndex(0, hCam)
        
        rt = MVGetWidth(hCam, w)
        rt = MVGetHeight(hCam, h)
        
        rt = MVGetPixelFormat(hCam, pf)
        
        If pf = PixelFormat_Mono8 Then
            hImage = MVImageCreate(w, h, 8)
        Else
            hImage = MVImageCreate(w, h, 24)
        End If
        
        LabelW.Caption = w
        
        SingleGrab.Enabled = True
        
    End If
    
End Sub

Private Sub SaveImage_Click()
    CommonDialog1.Filter = "Bitmap (*.bmp)|*.bmp|Tiff (*.tif)|*.tif|Jpeg (*.jpg)|*.jpg|"
    CommonDialog1.DefaultExt = "bmp"
    CommonDialog1.DialogTitle = "Save File"
    CommonDialog1.InitDir = App.Path
    CommonDialog1.ShowSave

    MVImageSave hImage, CommonDialog1.FileName
End Sub

Private Sub SingleGrab_Click()
    Dim rt As MVSTATUS_CODES
    
    rt = MVSingleGrab(hCam, hImage, 500)
    If rt = MVST_SUCCESS Then
        MVImageDrawHwnd hImage, Picture1.hWnd, 0, 0
        SaveImage.Enabled = True
    End If
End Sub
