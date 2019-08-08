VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   12045
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   15045
   LinkTopic       =   "Form1"
   ScaleHeight     =   12045
   ScaleWidth      =   15045
   StartUpPosition =   3  '窗口缺省
   Begin VB.CheckBox CheckFit 
      Caption         =   "充满"
      Height          =   375
      Left            =   5400
      TabIndex        =   6
      Top             =   120
      Width           =   975
   End
   Begin VB.CommandButton Freeze 
      Caption         =   "冻结"
      Enabled         =   0   'False
      Height          =   375
      Left            =   4080
      TabIndex        =   5
      Top             =   120
      Width           =   1215
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   12000
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
   Begin VB.CommandButton Grab 
      Caption         =   "实时采集"
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
      Height          =   11295
      Left            =   120
      ScaleHeight     =   11235
      ScaleWidth      =   14715
      TabIndex        =   0
      Top             =   600
      Width           =   14775
   End
   Begin VB.Label LabelW 
      Caption         =   "0"
      Height          =   255
      Left            =   6720
      TabIndex        =   4
      Top             =   240
      Width           =   1335
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim hCam As Long
Dim hImage As Long
Dim bLive As Long
Dim bFreeze As Long
Dim imgWidth As Long
Dim imgHeight As Long

Private Sub CheckFit_Click()
    Dim w As Long
    Dim h As Long
    
    Me.ScaleMode = vbPixels
    Picture1.ScaleMode = vbPixels
    
    w = Picture1.width
    h = Picture1.height

    If w > imgWidth Then
        w = imgWidth
    End If
    
    If h > imgHeight Then
        h = imgHeight
    End If
    
    If CheckFit.Value = 0 Then
        MVSetGrabWindow hCam, 0, 0, w, h, 0, 0, w, h
    Else
        MVSetGrabWindow hCam, 0, 0, Picture1.width, Picture1.height, 0, 0, imgWidth, imgHeight
    End If

End Sub

Private Sub Form_Unload(Cancel As Integer)
    If bLive <> 0 Then
        MVStopGrabWindow hCam
    End If
    
    If hCam <> 0 Then
        MVCloseCam hCam
        hCam = 0
    End If
End Sub

Private Sub Freeze_Click()
    Dim rt As MVSTATUS_CODES
    If bFreeze = 0 Then
        bFreeze = 1
        Freeze.Caption = "解除冻结"
    Else
        bFreeze = 0
        Freeze.Caption = "冻结"
    End If
    rt = MVFreezeGrabWindow(hCam, bFreeze)
End Sub

Private Sub Grab_Click()
    Dim rt As MVSTATUS_CODES
    
    If bLive = 0 Then
        rt = MVStartGrabWindow(hCam, Picture1.hWnd)
        If rt = MVST_SUCCESS Then
            Freeze.Enabled = True
            SaveImage.Enabled = True
            bLive = True
            Grab.Caption = "停止"
        End If
    Else
        rt = MVStopGrabWindow(hCam)
        Freeze.Enabled = False
        bLive = False
        Grab.Caption = "实时采集"
    End If
End Sub

Private Sub OpenCam_Click()
    Dim num As Long
    Dim w As Long
    Dim h As Long
    Dim rt As MVSTATUS_CODES
    Dim pf As MV_PixelFormatEnums
   
    SavedPointer = Screen.MousePointer
    Screen.MousePointer = 11
   
    If hCam <> 0 Then
        If bLive <> 0 Then
            MVStopGrabWindow hCam
        End If
        
        MVCloseCam hCam
        hCam = 0
        OpenCam.Caption = "打开相机"
    Else
        rt = MVGetNumOfCameras(num)
        
        If num <= 0 Then
            MsgBox "没有找到相机, 请检查设备连接 !"
        Else
            rt = MVOpenCamByIndex(0, hCam)
            
            rt = MVGetWidth(hCam, w)
            rt = MVGetHeight(hCam, h)
            
            imgWidth = w
            imgHeight = h
            
            rt = MVGetPixelFormat(hCam, pf)
            
            If pf = PixelFormat_Mono8 Then
                hImage = MVImageCreate(w, h, 8)
            Else
                hImage = MVImageCreate(w, h, 24)
            End If
            
            LabelW.Caption = w & "x" & h
            
            Grab.Enabled = True
            
            OpenCam.Caption = "断开相机"
            
        End If
    
    End If
    
    Screen.MousePointer = SavedPointer
    
End Sub

Private Sub SaveImage_Click()
    Dim FrameID
    MVGetSampleGrab hCam, hImage, FrameID
    
    CommonDialog1.Filter = "Bitmap (*.bmp)|*.bmp|Tiff (*.tif)|*.tif|Jpeg (*.jpg)|*.jpg|"
    CommonDialog1.DefaultExt = "bmp"
    CommonDialog1.DialogTitle = "Save File"
    CommonDialog1.InitDir = App.Path
    CommonDialog1.ShowSave
    
    MVImageSave hImage, CommonDialog1.FileName
End Sub

