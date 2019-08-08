// DlgRecConf.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRecConf.h"
#include "..\..\include\MVAVI.H"
#include "iniFile.h"
#include "IniValue.h"

#define noID CIniFile::noID
#define FORMAT_AVICOMPRESSOPTIONS1	"%d,%d,%d,%d,%d,%d,%d,%d"
#define FORMAT_AVICOMPRESSOPTIONS2	"%d,%d,%d"

int CALLBACK RecBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)   
{   
	if  (uMsg == BFFM_INITIALIZED )
	{   
		char *str = (char *)lpData;
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);   
		::SendMessage(hwnd, BFFM_SETEXPANDED, TRUE, lpData);
	}   
	return 0;  
}

void InitRecConf(CRecConf &recConf)
{
	recConf.m_bAutoFileName = FALSE;

	recConf.m_bIndex = FALSE;
	recConf.m_nIndex = 0;

	recConf.m_nTimeStyle = 2;

	recConf.m_strPath = "";
	recConf.m_strPrefixName = "";

	recConf.m_bAutoStopByTime = FALSE;
	recConf.m_bAutoStopByFrame = FALSE;
	recConf.m_bAutoStopByFileSize = FALSE;
	recConf.m_nAutoStopTime = 0;
	recConf.m_nAutoStopFrame = 0;
	recConf.m_nAutoStopFileSize = 0;
	recConf.m_nInterval = 1;

	memset(&recConf.m_CompressOpt,0,sizeof(AVICOMPRESSOPTIONS));
}

BOOL InitRecConfByFile( CRecConf &recConf, CString strIniFilePath )
{
	CIniFile IniFile;
	if (strIniFilePath.IsEmpty())
	{
		strIniFilePath = FILE_NAME_REC;
	}
	IniFile.SetPath(strIniFilePath.LockBuffer());
	IniFile.CaseSensitive();

	if (IniFile.ReadFile() == FALSE)
		return FALSE;

	if (IniFile.FindKey(KEY_RECORD_CONFIG) == noID)
		return FALSE;
	recConf.m_bAutoFileName			= IniFile.GetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOFILENAME, FALSE);
	recConf.m_bIndex				= IniFile.GetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_INDEX, FALSE);
	recConf.m_nIndex				= IniFile.GetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_INDEX, 1);
	if (recConf.m_nIndex < 0)
	{
		recConf.m_nIndex = 0;
	}
	recConf.m_nTimeStyle			= IniFile.GetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_TIME_STYLE, 2);
	if (recConf.m_nTimeStyle < 0 || recConf.m_nTimeStyle > 2)
	{
		recConf.m_nTimeStyle = 2;
	}
	recConf.m_strPrefixName			= IniFile.GetValue(KEY_RECORD_CONFIG,  VALUE_STRING_PREFIX_NAME, _T("Image")).c_str();
	recConf.m_strPath				= IniFile.GetValue(KEY_RECORD_CONFIG,  VALUE_STRING_PATH, _T("")).c_str();
	recConf.m_bAutoStopByTime		= IniFile.GetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOSTOPTIME, FALSE);
	recConf.m_bAutoStopByFrame		= IniFile.GetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOSTOPFRAME, FALSE);
	recConf.m_bAutoStopByFileSize	= IniFile.GetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOSTOPSIZE, FALSE);
	recConf.m_nAutoStopTime			= IniFile.GetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_AUTOSTOPTIME, 0);
	recConf.m_nAutoStopFrame		= IniFile.GetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_AUTOSTOPFRAME, 0);
	recConf.m_nAutoStopFileSize		= IniFile.GetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_AUTOSTOPSIZE, 0);
	recConf.m_nInterval				= IniFile.GetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_INTERVAL, 1);
	memset(&recConf.m_CompressOpt,0,sizeof(AVICOMPRESSOPTIONS));
	IniFile.GetValueV(KEY_RECORD_CONFIG, VALUE_VOID_AVICOMPRESSOPTIONS1,FORMAT_AVICOMPRESSOPTIONS1,
		&(recConf.m_CompressOpt.fccType),&(recConf.m_CompressOpt.fccHandler),&(recConf.m_CompressOpt.dwKeyFrameEvery),&(recConf.m_CompressOpt.dwQuality),
		&(recConf.m_CompressOpt.dwBytesPerSecond),&(recConf.m_CompressOpt.dwFlags),&(recConf.m_CompressOpt.lpFormat),&(recConf.m_CompressOpt.cbFormat));
	IniFile.GetValueV(KEY_RECORD_CONFIG, VALUE_VOID_AVICOMPRESSOPTIONS2,FORMAT_AVICOMPRESSOPTIONS2,
		&(recConf.m_CompressOpt.lpParms),&(recConf.m_CompressOpt.cbParms),&(recConf.m_CompressOpt.dwInterleaveEvery));

	return TRUE;
}

// CDlgRecConf dialog

IMPLEMENT_DYNAMIC(CDlgRecConf, CDialog)

CDlgRecConf::CDlgRecConf(CWnd* pParent /*=NULL*/)
: CDialog(CDlgRecConf::IDD, pParent)
{
	InitRecConf(m_conf);
}

CDlgRecConf::~CDlgRecConf()
{
}

void CDlgRecConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_AUTO_FILENAME, m_conf.m_bAutoFileName);
	DDX_Check(pDX, IDC_CHECK_INDEX, m_conf.m_bIndex);

	DDX_Text(pDX, IDC_EDIT_FILENAME_PREFIX, m_conf.m_strPrefixName);
	DDX_Text(pDX, IDC_EDIT_PATH, m_conf.m_strPath);

	DDX_Text(pDX, IDC_EDIT_INDEX, m_conf.m_nIndex);
	DDV_MinMaxInt(pDX, m_conf.m_nIndex, 0, INT_MAX);

	DDX_CBIndex(pDX, IDC_COMBO_TIME_STYLE, m_conf.m_nTimeStyle);
	DDX_Check(pDX, IDC_CHK_REC_TIME_LIMIT, m_conf.m_bAutoStopByTime);
	DDX_Check(pDX, IDC_CHK_REC_FRAME_LIMIT, m_conf.m_bAutoStopByFrame);
	DDX_Check(pDX, IDC_CHK_REC_FSIZE_LIMIT, m_conf.m_bAutoStopByFileSize);
	DDX_Text(pDX, IDC_EDIT_REC_TIME_LIMIT, m_conf.m_nAutoStopTime);
	DDX_Text(pDX, IDC_EDIT_REC_FRAME_LIMIT, m_conf.m_nAutoStopFrame);
	DDX_Text(pDX, IDC_EDIT_REC_FSIZE_LIMIT, m_conf.m_nAutoStopFileSize);
	DDX_Text(pDX, IDC_EDIT_REC_INTERVAL, m_conf.m_nInterval);
	DDX_CBIndex(pDX, IDC_COMBO_TIME_STYLE, m_conf.m_nTimeStyle);
	DDX_Control(pDX, IDC_SPIN_INTERVAL, m_spinRecInterval);
}


BEGIN_MESSAGE_MAP(CDlgRecConf, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PATH, &CDlgRecConf::OnBnClickedButtonChangePath)
	ON_BN_CLICKED(IDOK, &CDlgRecConf::OnBnClickedOk)	
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_CODEC, &CDlgRecConf::OnBnClickedButtonChangeCodec)
	ON_BN_CLICKED(IDC_LOAD, &CDlgRecConf::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SAVE, &CDlgRecConf::OnBnClickedSave)
END_MESSAGE_MAP()


// CDlgRecConf message handlers

void CDlgRecConf::OnBnClickedButtonChangePath()
{
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));//指定存放文件的默认文件夹路径

	LPITEMIDLIST pidlRoot = NULL;

	bi.hwndOwner    = GetSafeHwnd();
	bi.lpszTitle    = _T("请选择目录");
	bi.ulFlags		= BIF_NEWDIALOGSTYLE;
	bi.lpfn         = NULL;
	bi.pidlRoot     = pidlRoot;
	bi.lParam       = (LPARAM)m_conf.m_strPath.GetBuffer();
	bi.lpfn         = RecBrowseCallbackProc;

	LPMALLOC pMalloc;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//以默认路径打开浏览文件夹对话框
	TCHAR * savepath = new TCHAR[MAX_PATH]; 
	CString savepath2="";
	if(pidl != NULL)
	{
		SHGetPathFromIDList(pidl,savepath);//把文件夹路径存放在savepath中 
		savepath2.Format(_T("%s"),savepath);
		savepath2=savepath2+"\\";//在路径后增加斜杠
		m_conf.m_strPath = savepath2;
		if(SUCCEEDED(SHGetMalloc(&pMalloc)))//pid释放
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		} 
	}
	UpdateData(FALSE);
	free(savepath);
}

void CDlgRecConf::GetData( CRecConf &conf )
{
	conf = m_conf;
}

void CDlgRecConf::SetData( CRecConf conf )
{
	m_conf = conf;
}

void CDlgRecConf::OnBnClickedOk()
{	
	UpdateData(TRUE);

	SaveRecConf( FILE_NAME_REC );
	OnOK();
}

struct CFccCode
{
	char fcc[5];
	char name[256];
};

char *FccToName(DWORD fccHandler)
{
	const struct CFccCode codes[] = {
		"3IV0","MPEG4-based codec 3ivx",
		"3IV1","MPEG4-based codec 3ivx",
		"3IV2","MPEG4-based codec 3ivx",
		"AASC","Autodesk Animator codec",
		"ABYR","Kensington codec",
		"AEMI","Array VideoONE MPEG1-I Capture",
		"AFLC","Autodesk Animator FLC (256 color)",
		"AFLI","Autodesk Animator FLI (256 color)",
		"AMPG","Array VideoONE MPEG",
		"ANIM","Intel RDX",
		"AP41","AngelPotion Definitive (hack MS MP43)",
		"ASV1","Asus Video V1",
		"ASV2","Asus Video V2",
		"ASVX","Asus Video 2.0",
		"AUR2","Aura 2 Codec YUV 422",
		"AURA","Aura 1 Codec YUV 411",
		"BINK","Bink Video (RAD Game Tools) (256 color)",
		"BT20","Conexant (ex Brooktree) ProSummer MediaStream",
		"BTCV","Conexant Composite Video",
		"BW10","Data Translation Broadway MPEG Capture/Compression",
		"CC12","Intel YUV12 Codec",
		"CDVC","Canopus DV Codec",
		"CFCC","Conkrete DPS Perception Motion JPEG",
		"CGDI","Camcorder Video (MS Office 97)",
		"CHAM","WinNow Caviara Champagne",
		"CJPG","Creative Video Blaster Webcam Go JPEG",
		"CLJR","Cirrus Logic YUV 4:1:1",
		"CMYK","Common Data Format in Printing",
		"CPLA","Weitek YUV 4:2:0 Planar",
		"CRAM","Microsoft Video 1",
		"CVID","Cinepak by Radius YUV 4:2:2",
		"CWLT","Microsoft Color WLT DIB",
		"CYUV","Creative Labs YUV 4:2:2",
		"CYUY","ATI Technologies YUV",
		"D261","H.261",
		"D263","H.263",
		"DIB","Full Frames (Uncompressed)",
		"DIB ","全帧(未压缩)",
		"DIV3","Low motion DivX MPEG-4",
		"DIV4","Fast motion DivX MPEG-4",
		"DIV5","DivX MPEG-4",
		"DIV6","DivX MPEG-4",
		"DIVX","OpenDivX (DivX 4.0 and later)",
		"DMB1","Matrox Rainbow Runner hardware compression (Motion JPEG)",
		"DMB2","Motion JPEG codec used by Paradigm",
		"DUCK","True Motion 1.0",
		"DVE2","DVE-2 Videoconferencing Codec",
		"DVHD","DV 1125 lines at 30.00 Hz or 1250 lines at 25.00 Hz",
		"DVS1","DV compressed in SD (SDL)",
		"DVSD","Sony Digital Video (DV) 525 lines at 29.97 Hz or 625 lines at 25.00 Hz",
		"DVX1","DVX1000SP Video Decoder",
		"DVX2","DVX2000S Video Decoder",
		"DVX3","DVX3000S Video Decoder",
		"DXT1","DirectX Compressed Texture",
		"DXT2","DirectX Compressed Texture",
		"DXT3","DirectX Compressed Texture",
		"DXT4","DirectX Compressed Texture",
		"DXT5","DirectX Compressed Texture",
		"DXTC","DirectX Texture Compression",
		"EKQ0","Else graphics card codec",
		"ELK0","Else graphics card codec",
		"ESCP","Eidos Technologies Escape codec",
		"ETV1","eTreppid Video Codec",
		"ETV2","eTreppid Video Codec",
		"ETVC","eTreppid Video Codec",
		"FLJP","Field Encoded Motion JPEG (Targa emulation)",
		"FRWA","Darim Vision Forward Motion JPEG with Alpha-channel",
		"FRWD","Darim Vision Forward Motion JPEG",
		"FVF1","Fractal Video Frame",
		"GLZW","Motion LZW by gabest@freemail.hu",
		"GPEG","Motion JPEG by gabest@freemail.hu (with floating point)",
		"GWLT","Microsoft Greyscale WLT DIB",
		"H260","H.260",
		"H261","H.261",
		"H262","H.262",
		"H263","Intel ITU H.263",
		"H264","H.264",
		"H265","H.265",
		"H266","H.266",
		"H267","H.267",
		"H268","H.268",
		"H269","H.263 for POTS-based videoconferencing",
		"HFYU","Huffman Lossless Codec YUV and RGB formats (with Alpha-channel)",
		"HMCR","Rendition Motion Compensation Format",
		"HMRR","Rendition Motion Compensation Format",
		"I263","Intel ITU H.263",
		"I420","Intel Indeo 4 H.263",
		"IAN","Indeo 4 (RDX) Codec",
		"IAVS","interleaved audio and video stream",
		"ICLB","CellB Videoconferencing Codec",
		"IGOR","Power DVD",
		"IJPG","Intergraph JPEG",
		"ILVC","Intel Layered Video",
		"ILVR","ITU H.263+ Codec",
		"IPDV","Giga AVI DV Codec",
		"IR21","Intel Indeo 2.1",
		"IRAW","Intel YUV Uncompressed",
		"IUYV","Interlaced version of UYVY (line order 0, 2, 4,....,1, 3, 5....)",
		"IV30","Intel Indeo Video 3",
		"IV31","Intel Indeo Video 3.1",
		"IV32","Intel Indeo Video 3.2",
		"IV33","Intel Indeo Video 3.3",
		"IV34","Intel Indeo Video 3.4",
		"IV35","Intel Indeo Video 3.5",
		"IV36","Intel Indeo Video 3.6",
		"IV37","Intel Indeo Video 3.7",
		"IV38","Intel Indeo Video 3.8",
		"IV39","Intel Indeo Video 3.9",
		"IV40","Intel Indeo Video 4.0",
		"IV41","Intel Indeo Video 4.1",
		"IV42","Intel Indeo Video 4.2",
		"IV43","Intel Indeo Video 4.3",
		"IV44","Intel Indeo Video 4.4",
		"IV45","Intel Indeo Video 4.5",
		"IV46","Intel Indeo Video 4.6",
		"IV47","Intel Indeo Video 4.7",
		"IV48","Intel Indeo Video 4.8",
		"IV49","Intel Indeo Video 4.9",
		"IV50","Intel Indeo Video 5.0 Wavelet",
		"IY41","Interlaced version of Y41P (line order 0, 2, 4,....,1, 3, 5....)",
		"IYU1","12 bit format used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec",
		"IYU2","24 bit format used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec",
		"JPEG","Still Image JPEG DIB",
		"JPGL","DIVIO JPEG Light for WebCams",
		"KMVC","Karl Morton Video Codec",
		"LEAD","LEAD Video Codec",
		"LJPG","LEAD Motion JPEG Codec",
		"M261","Microsoft H.261",
		"M263","Microsoft H.263",
		"MC12","ATI Motion Compensation Format",
		"MCAM","ATI Motion Compensation Format",
		"MJPG","IBM Motion JPEG including Huffman Tables",
		"MP42","Microsoft MPEG-4 V2",
		"MP43","Microsoft MPEG-4 V3",
		"MP4S","Microsoft MPEG-4 (hacked MS MPEG-4)",
		"MP4V","Microsoft MPEG-4 (hacked MS MPEG-4)",
		"MPEG","Chromatic MPEG 1 Video I Frame",
		"MPG4","Microsoft MPEG-4 V1",
		"MPGI","Sigma Design MPEG-1 I-frame",
		"MRCA","FAST Multimedia MR Codec",
		"MRLE","Microsoft Run Length Encoding",
		"MSVC","Microsoft Video 1",
		"MSVC","Microsoft Video 1",
		"MTX1","Matrox codec",
		"MTX2","Matrox codec",
		"MTX3","Matrox codec",
		"MTX4","Matrox codec",
		"MTX5","Matrox codec",
		"MTX6","Matrox codec",
		"MTX7","Matrox codec",
		"MTX8","Matrox codec",
		"MTX9","Matrox codec",
		"MWV1","Aware Motion Wavelets",
		"NTN1","Nogatech Video Compression 1",
		"NVS0","Nvidia texture compression format",
		"NVS1","Nvidia texture compression format",
		"NVS2","Nvidia texture compression format",
		"NVS3","Nvidia texture compression format",
		"NVS4","Nvidia texture compression format",
		"NVS5","Nvidia texture compression format",
		"NVT0","Nvidia texture compression format",
		"NVT1","Nvidia texture compression format",
		"NVT2","Nvidia texture compression format",
		"NVT3","Nvidia texture compression format",
		"NVT4","Nvidia texture compression format",
		"NVT5","Nvidia texture compression format",
		"PDVC","Panasonic DV codec",
		"PGVV","Radius Video Vision Telecast (adaptive JPEG)",
		"PHMO","Photomotion",
		"PIM1","Pegasus Imaging codec",
		"PIM2","Pegasus Imaging codec",
		"PIMJ","Pegasus Imaging PICvideo Lossless JPEG",
		"PVEZ","Horizons Technology PowerEZ codec",
		"PVMM","PacketVideo Corporation MPEG-4",
		"PVW2","Pegasus Imaging Wavelet 2000",
		"QPEG","Q-Team QPEG 1.1",
		"QPEQ","Q-Team QPEG 1.1",
		"RAW","Full Frames (Uncompressed)",
		"RGB","Full Frames (Uncompressed)",
		"RGBA","Raw RGB with alpha",
		"RGBT","Uncompressed RGB with transparency",
		"RLE","Raw RGB with arbitrary sample packing within a pixel",
		"RLE4","Run length encoded 4bpp RGB image",
		"RLE8","Run length encoded 8bpp RGB image",
		"RT21","Intel Real Time Video 2.1",
		"RV20","RealVideo G2",
		"RV30","RealVideo 8",
		"RVX","Intel RDX",
		"S422","VideoCap C210 YUV Codec",
		"SDCC","Sun Digital Camera Codec",
		"SFMC","Crystal Net SFM Codec",
		"SMSC","Radius proprietary codec",
		"SMSD","Radius proprietary codec",
		"SMSV","WorldConnect Wavelet Streaming Video",
		"SPIG","Radius Spigot",
		"SPLC","Splash Studios ACM Audio Codec",
		"SQZ2","Microsoft VXTreme Video Codec V2",
		"STVA","ST CMOS Imager Data (Bayer)",
		"STVB","ST CMOS Imager Data (Nudged Bayer)",
		"STVC","ST CMOS Imager Data (Bunched)",
		"STVX","ST CMOS Imager Data (Extended)",
		"STVY","ST CMOS Imager Data (Extended with Correction Data)",
		"SV10","Sorenson Media Video R1",
		"SVQ1","Sorenson Video (Apple Quicktime 3)",
		"TLMS","TeraLogic Motion Infraframe Codec A",
		"TLST","TeraLogic Motion Infraframe Codec B",
		"TM20","Duck TrueMotion 2.0",
		"TM2X","Duck TrueMotion 2X",
		"TMIC","TeraLogic Motion Intraframe Codec 2",
		"TMOT","TrueMotion Video Compression",
		"TR20","Duck TrueMotion RT 2.0",
		"TSCC","TechSmith Screen Capture Codec",
		"TV10","Tecomac Low-Bit Rate Codec",
		"TY0N","Trident Decompression Driver",
		"TY2C","Trident Decompression Driver",
		"TY2N","Trident Decompression Driver",
		"UCOD","ClearVideo (fractal compression-based codec)",
		"ULTI","IBM Corp. Ultimotion",
		"UYNV","A direct copy of UYVY registered by NVidia to work around problems in some old codecs which did not like hardware which offered more than 2 UYVY surfaces",
		"UYVP","YCbCr 4:2:2 extended precision 10-bits per component in U0Y0V0Y1 order",
		"UYVY","YUV 4:2:2 (Y sample at every pixel, U and V sampled at every second pixel horizontally on each line)",
		"V261","Lucent VX2000S",
		"V422","Vitec Multimedia YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel",
		"V655","Vitec Multimedia 16 bit YUV 4:2:2 (6:5:5) format",
		"VCR1","ATI VCR 1.0",
		"VCR2","ATI VCR 2.0 (MPEG YV12)",
		"VCR3","ATI VCR 3.0",
		"VCR4","ATI VCR 4.0",
		"VCR5","ATI VCR 5.0",
		"VCR6","ATI VCR 6.0",
		"VCR7","ATI VCR 7.0",
		"VCR8","ATI VCR 8.0",
		"VCR9","ATI VCR 9.0",
		"VDCT","Video Maker Pro DIB",
		"VDOM","VDOnet VDOWave",
		"VDOW","VDOLive (H.263)",
		"VDTZ","Darim Vision VideoTizer YUV",
		"VGPX","Alaris VGPixel Codec",
		"VIDS","Vitec Multimedia YUV 4:2:2 codec",
		"VIVO","Vivo H.263",
		"VIXL","miroVideo XL",
		"VLV1","VideoLogic codec",
		"VP30","On2 (ex Duck TrueMotion) VP3",
		"VP31","On2 (ex Duck TrueMotion) VP3",
		"VX1K","Lucent VX1000S Video Codec",
		"VX2K","Lucent VX2000S Video Codec",
		"VXSP","Lucent VX1000SP Video Codec",
		"WBVC","Winbond W9960 codec",
		"WHAM","Microsoft Video 1",
		"WINX","Winnov Software Compression",
		"WJPG","Winbond JPEG",
		"WNV1","WinNow Videum Hardware Compression",
		"X263","Xirlink H.263",
		"XLV0","NetXL Inc. XL Video Decoder",
		"XMPG","XING MPEG (I frame only)",
		"Y211","Packed YUV format with Y sampled at every second pixel across each line and U and V sampled at every fourth pixel",
		"Y411","YUV 4:1:1 Packed",
		"Y41B","YUV 4:1:1 Planar",
		"Y41P","Conexant (ex Brooktree) YUV 4:1:1 Raw",
		"Y41T","Format as for Y41P but the lsb of each Y component is used to signal pixel transparency",
		"Y422","Direct copy of UYVY as used by ADS Technologies Pyro WebCam firewire camera",
		"Y42B","YUV 4:2:2 Planar",
		"Y42T","Format as for UYVY but the lsb of each Y component is used to signal pixel transparency",
		"Y8","Simple grayscale video",
		"Y800","Simple grayscale video",
		"YC12","Intel YUV12 Codec",
		"YUNV","A direct copy of YUY2 registered by NVidia to work around problems in some old codecs which did not like hardware which offered more than 2 YUY2 surfaces",
		"YUV8","Winnov Caviar YUV8",
		"YUV9","Intel YUV9",
		"YUVP","YCbCr 4:2:2 extended precision 10-bits per component in Y0U0Y1V0 order",
		"YUY2","YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel",
		"YUYV","Canopus YUV format",
		"YV12","ATI YVU12 4:2:0 Planar",
		"YVU9","Brooktree YVU9 Raw (YVU9 Planar)",
		"YVYU","YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel",
		"ZLIB","Lossless codec (ZIP compression)",
		"ZPEG","Metheus Video Zipper "
	};
	
	int cnt = sizeof(codes)/sizeof(CFccCode);

	char strFcc[5] = "";
	memcpy(strFcc,&fccHandler,4);

	CString str(strFcc);
	str.MakeUpper();
	strcpy(strFcc,str.LockBuffer());

	for(int i=0;i<cnt;i++)
	{
		if( memcmp( codes[i].fcc,strFcc,4) == 0 )
		{
			return (char *)codes[i].name;
		}
	}

	return NULL;
}

void CDlgRecConf::OnBnClickedButtonChangeCodec()
{
	MVImage image;
	image.Create(640,480,24);
	MVAvi avi;
	avi.Create("__temptemp.avi",100);

	AVICOMPRESSOPTIONS opts;
	memset(&opts,0,sizeof(opts));
	avi.SetVideoCompression(&image,&opts,TRUE,m_hWnd);
	avi.Close();

	DeleteFile("__temptemp.avi");

	if( opts.fccHandler != 0 )
	{
		char *name = FccToName(opts.fccHandler);
		GetDlgItem(IDC_CODEC)->SetWindowText(name);

		memcpy( &(m_conf. m_CompressOpt), &opts, sizeof(opts));
	}
}

BOOL CDlgRecConf::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_spinRecInterval.SetRange(1,1000);
	m_spinRecInterval.SetPos(m_conf.m_nInterval);
	char *name = FccToName(m_conf.m_CompressOpt.fccHandler);
	if (name != NULL)
	{
		GetDlgItem(IDC_CODEC)->SetWindowText(name);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgRecConf::SaveRecConf( CString srtPath )
{
	CIniFile IniFile;
	bool r = true;
	if (srtPath.IsEmpty())
	{
		srtPath = KEY_RECORD_CONFIG;
	}
	IniFile.SetPath(srtPath.LockBuffer());
	IniFile.CaseSensitive();
	r |= IniFile.SetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOFILENAME, m_conf.m_bAutoFileName);
	r |= IniFile.SetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_INDEX, m_conf.m_bIndex);
	r |= IniFile.SetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_INDEX, m_conf.m_nIndex);
	r |= IniFile.SetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_TIME_STYLE, m_conf.m_nTimeStyle);
	r |= IniFile.SetValue(KEY_RECORD_CONFIG,  VALUE_STRING_PREFIX_NAME, m_conf.m_strPrefixName.LockBuffer());
	r |= IniFile.SetValue(KEY_RECORD_CONFIG,  VALUE_STRING_PATH, m_conf.m_strPath.LockBuffer());
	r |= IniFile.SetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOSTOPTIME, m_conf.m_bAutoStopByTime);
	r |= IniFile.SetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOSTOPFRAME, m_conf.m_bAutoStopByFrame);
	r |= IniFile.SetValueB(KEY_RECORD_CONFIG, VALUE_BOOLEAN_AUTOSTOPSIZE, m_conf.m_bAutoStopByFileSize);
	r |= IniFile.SetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_AUTOSTOPTIME, m_conf.m_nAutoStopTime);
	r |= IniFile.SetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_AUTOSTOPFRAME, m_conf.m_nAutoStopFrame);
	r |= IniFile.SetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_AUTOSTOPSIZE, m_conf.m_nAutoStopFileSize);
	r |= IniFile.SetValueI(KEY_RECORD_CONFIG, VALUE_INTEGER_INTERVAL, m_conf.m_nInterval);
	r |= IniFile.SetValueV(KEY_RECORD_CONFIG, VALUE_VOID_AVICOMPRESSOPTIONS1,FORMAT_AVICOMPRESSOPTIONS1,
		m_conf.m_CompressOpt.fccType, m_conf.m_CompressOpt.fccHandler, m_conf.m_CompressOpt.dwKeyFrameEvery, m_conf.m_CompressOpt.dwQuality,
		m_conf.m_CompressOpt.dwBytesPerSecond, m_conf.m_CompressOpt.dwFlags, m_conf.m_CompressOpt.lpFormat, m_conf.m_CompressOpt.cbFormat);
	r |= IniFile.SetValueV(KEY_RECORD_CONFIG, VALUE_VOID_AVICOMPRESSOPTIONS2,FORMAT_AVICOMPRESSOPTIONS2,
		m_conf.m_CompressOpt.lpParms, m_conf.m_CompressOpt.cbParms, m_conf.m_CompressOpt.dwInterleaveEvery);
	if (!r)
	{
		return FALSE;
	}

	if (IniFile.WriteFile() == false)
		return FALSE;
	return TRUE;
}

void CDlgRecConf::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("INI"), _T("未命名"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "INI (*.ini)|*.ini|");
	if (dlg.DoModal() == IDOK)
	{
		InitRecConfByFile(m_conf, dlg.GetPathName());
		UpdateData(FALSE);
	}
}

void CDlgRecConf::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog dlg(FALSE, _T("INI"), _T("未命名"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "INI (*.ini)|*.ini|");
	if (dlg.DoModal() == IDOK)
	{
		SaveRecConf(dlg.GetPathName());
	}
}
