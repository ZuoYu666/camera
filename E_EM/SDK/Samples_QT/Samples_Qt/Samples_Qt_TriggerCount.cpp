#include "Samples_Qt_TriggerCount.h"

Samples_Qt_TriggerCount::Samples_Qt_TriggerCount(QWidget *parent)
	: QWidget(parent)
	, shotNumber(0)
	, saveImageFlag(false)
	, HCflag(true)
	, m_hProperty(nullptr)
	, displayScale(4)
{
	ui.setupUi(this);

	connect(ui.displayScale, &QComboBox::currentTextChanged, [=](const QString &) {switch (ui.displayScale->currentIndex()) { case 0:displayScale = 5; break; case 1:displayScale = 4; break; case 2:displayScale = 2; break; case 3:displayScale = 1; break; }; 
	if (currentModel == CONTINUITYSHOT || currentModel == OUTSHOT)
	{
		ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
	else
	{
		ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
	drawImageForWindow(); });

	init();
}

Samples_Qt_TriggerCount::~Samples_Qt_TriggerCount()
{
	MVCamProptySheetDestroy(m_hProperty);
	on_tz_clicked();
	MVTerminateLib();
}

void Samples_Qt_TriggerCount::init()
{
	bool MVInitLibState = false;
	switch (MVInitLib())													//初始化相机
	{
	case MVST_SUCCESS:MVInitLibState = true; break;							//没有错误
	case MVST_ERROR:WMSG("初始化相机:一般错误"); break;
	case MVST_ERR_NOT_INITIALIZED:WMSG("初始化相机:没有初始化"); break;
	case MVST_ERR_NOT_IMPLEMENTED:WMSG("初始化相机:没有实现"); break;
	case MVST_ERR_RESOURCE_IN_USE:WMSG("初始化相机:资源被占用"); break;
	case MVST_ACCESS_DENIED:WMSG("初始化相机:无法访问"); break;
	case MVST_INVALID_HANDLE:WMSG("初始化相机:错误句柄"); break;
	case MVST_INVALID_ID:WMSG("初始化相机:错误ID"); break;
	case MVST_NO_DATA:WMSG("初始化相机:没有数据"); break;
	case MVST_INVALID_PARAMETER:WMSG("初始化相机:错误参数"); break;
	case MVST_FILE_IO:WMSG("初始化相机:IO错误"); break;
	case MVST_TIMEOUT:WMSG("初始化相机:超时"); break;
	case MVST_ERR_ABORT:WMSG("初始化相机:退出"); break;
	case MVST_INVALID_BUFFER_SIZE:WMSG("初始化相机:缓冲区尺寸错误"); break;
	case MVST_ERR_NOT_AVAILABLE:WMSG("初始化相机:无法访问"); break;
	case MVST_INVALID_ADDRESS:WMSG("初始化相机:地址错误"); break;
	}

	if (MVInitLibState)
	{
		int linkCameraNumber = 0;
		MVGetNumOfCameras(&linkCameraNumber);								//获取连接到计算机上的相机的数量
		if (linkCameraNumber)
		{
			bool MVInitLibLink = false;
			switch (MVOpenCamByIndex(0, &hCamera))							//连接第1个相机
			{
			case MVST_SUCCESS:MVInitLibLink = true; break;					//没有错误
			case MVST_ERROR:WMSG("连接相机:一般错误"); break;
			case MVST_ERR_NOT_INITIALIZED:WMSG("连接相机:没有初始化"); break;
			case MVST_ERR_NOT_IMPLEMENTED:WMSG("连接相机:没有实现"); break;
			case MVST_ERR_RESOURCE_IN_USE:WMSG("连接相机:资源被占用"); break;
			case MVST_ACCESS_DENIED:WMSG("连接相机:无法访问,可能正被别的软件控制"); break;
			case MVST_INVALID_HANDLE:WMSG("连接相机:错误句柄"); break;
			case MVST_INVALID_ID:WMSG("连接相机:错误ID"); break;
			case MVST_NO_DATA:WMSG("连接相机:没有数据"); break;
			case MVST_INVALID_PARAMETER:WMSG("连接相机:错误参数"); break;
			case MVST_FILE_IO:WMSG("连接相机:IO错误"); break;
			case MVST_TIMEOUT:WMSG("连接相机:超时"); break;
			case MVST_ERR_ABORT:WMSG("连接相机:退出"); break;
			case MVST_INVALID_BUFFER_SIZE:WMSG("连接相机:缓冲区尺寸错误"); break;
			case MVST_ERR_NOT_AVAILABLE:WMSG("连接相机:无法访问"); break;
			case MVST_INVALID_ADDRESS:WMSG("连接相机:地址错误"); break;
			}
			MVInitLibLink ? CameraInitAfter() : changeUi_Button(CAMERAERR);
		}
		else
		{
			IMSG("没有找到相机,请确认连接和相机IP设置");
			changeUi_Button(CAMERAERR);
		}
	}
	else
		changeUi_Button(CAMERAERR);
}

void Samples_Qt_TriggerCount::CameraInitAfter()
{
	int hCameraW, hCameraH;
	MVGetWidth(hCamera, &hCameraW);
	MVGetHeight(hCamera, &hCameraH);
	MV_PixelFormatEnums mPixelFormat;
	MVGetPixelFormat(hCamera, &mPixelFormat);
	mImage.CreateByPixelFormat(hCameraW, hCameraH, mPixelFormat);

	MVSetTriggerMode(hCamera, TriggerMode_On);
	MVSetTriggerSource(hCamera, TriggerSource_Software);

	if (m_hProperty == nullptr)
	{
		MVCamInfo CamInfo;
		MVGetCameraInfo(0, &CamInfo);
		MVCamProptySheetInit(&m_hProperty, hCamera, 0, LPCTSTR("Propty"));
	}	
}

void Samples_Qt_TriggerCount::changeUi_Button(const BUTTONPRESSTYPE &type)
{
	if (type == CONTINUITYSHOT)
	{
		ui.lxcj->setEnabled(false);
		ui.rcf->setEnabled(false);
		ui.wcf->setEnabled(false);
		ui.tz->setEnabled(true);
		currentModel = CONTINUITYSHOT;

		shotNumber = 0;
		ui.pssl->setText(NUMBC(shotNumber));
	}
	else if (type == SINGLESHOT)
	{
		ui.lxcj->setEnabled(false);
		ui.rcf->setEnabled(true);
		ui.wcf->setEnabled(false);
		ui.tz->setEnabled(true);
		currentModel = SINGLESHOT;

		if (HCflag)
		{
			HCflag = false;

			shotNumber = 0;
			//ui.pssl->setText(NUMBC(shotNumber));
		}
	}
	else if (type == OUTSHOT)
	{
		ui.lxcj->setEnabled(false);
		ui.rcf->setEnabled(false);
		ui.wcf->setEnabled(false);
		ui.tz->setEnabled(true);
		currentModel = OUTSHOT;

		shotNumber = 0;
		ui.pssl->setText(NUMBC(shotNumber));
	}
	else if (type == STOPSHOT)
	{
		HCflag = true;

		ui.lxcj->setEnabled(true);
		ui.rcf->setEnabled(true);
		ui.wcf->setEnabled(true);
		ui.tz->setEnabled(true);
		currentModel = STOPSHOT;
	}
	else if (type == CAMERAERR)
	{
		ui.lxcj->setEnabled(false);
		ui.rcf->setEnabled(false);
		ui.wcf->setEnabled(false);
		ui.tz->setEnabled(false);
		currentModel = CAMERAERR;
		exit(-1);
	}

	if (currentModel == CONTINUITYSHOT || currentModel == OUTSHOT)
	{
		ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
	else
	{
		ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}

int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal)
{
	Samples_Qt_TriggerCount *pDlg = (Samples_Qt_TriggerCount *)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

int Samples_Qt_TriggerCount::OnStreamCB(MV_IMAGE_INFO *pInfo)
{
	ui.pssl->setText(NUMBC(++shotNumber));
	MVInfo2Image(hCamera, pInfo, &mImage);
	drawImageForWindow();
	return 0;
}

void Samples_Qt_TriggerCount::drawImageForWindow()
{
	wImage = mImage.GetWidth() / displayScale;
	hImage = mImage.GetHeight() / displayScale;
	tImage = qt_pixmapFromWinHBITMAP(mImage.GetHBitmap()).scaled(wImage, hImage, Qt::KeepAspectRatio).toImage();
	ui.showImage->setPixmap(QPixmap::fromImage(tImage));
}

void Samples_Qt_TriggerCount::on_lxcj_clicked()
{
	changeUi_Button(CONTINUITYSHOT);

	MVSetTriggerMode(hCamera, TriggerMode_Off);
	MVSetTriggerSource(hCamera, TriggerSource_Software);

	MVStartGrab(hCamera, StreamCB, (ULONG_PTR)this);
}

void Samples_Qt_TriggerCount::on_rcf_clicked()
{
	changeUi_Button(SINGLESHOT);

	MVSetTriggerSource(hCamera, TriggerSource_Software);
	MVSetTriggerMode(hCamera, TriggerMode_On);

	MVStartGrab(hCamera, StreamCB, (ULONG_PTR)this);
	MVTriggerSoftware(hCamera);
}

void Samples_Qt_TriggerCount::on_wcf_clicked()
{
	changeUi_Button(OUTSHOT);

	MVSetTriggerSource(hCamera, TriggerSource_Line1);
	//MVSetTriggerMode(hCamera, TriggerMode_On);	

	MVStartGrab(hCamera, StreamCB, (ULONG_PTR)this);
}

void Samples_Qt_TriggerCount::on_tz_clicked()
{
	changeUi_Button(STOPSHOT);

	MVStopGrab(hCamera);
}

void Samples_Qt_TriggerCount::on_Set_clicked()
{
	MVCamProptySheetShow(m_hProperty);
}

void Samples_Qt_TriggerCount::on_saveImage_clicked()
{
	saveImagePath = QFileDialog::getSaveFileName(this, SAVEPATH, "", tr("Image Files (*.bmp)"));

	if (!tImage.isNull() && !saveImagePath.isEmpty())
	{
		tImage.save(saveImagePath);
	}
}