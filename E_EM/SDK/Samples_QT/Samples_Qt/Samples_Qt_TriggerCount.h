#pragma once

#include <QtWidgets/QWidget>
#include "ui_Samples_Qt_TriggerCount.h"

#include <QtWidgets/QMessageBox>
#include <QComboBox>
#include <QFileDialog>

#include <afxwin.h>

#include "MVImage.h"
#include "MVGigE.h"
#include "MVCamProptySheet.h"

#include "MVScrollArea.h"

#include <qtwinextras/qwinfunctions.h>
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat = 0);

#define		NUMBC(str)			QString::number(str)

#ifdef _DEBUG
#define		CODEC(str)			QString::fromLocal8Bit(str)
#define		WARNINGS			CODEC("错误")
#define		INFORMATION			CODEC("提示")
#define		SAVEPATH			CODEC("选择保存位置")
#define		IMSG(str)			QMessageBox::information(0,INFORMATION,CODEC(str),QMessageBox::Ok);
#define		WMSG(str)			QMessageBox::warning(0,WARNINGS,CODEC(str),QMessageBox::Ok);
#else
#pragma comment(lib, "Shell32.lib")
#pragma execution_character_set("utf-8")
#define		WARNINGS			("错误")
#define		INFORMATION			("提示")
#define		SAVEPATH			("选择保存位置")
#define		IMSG(str)			QMessageBox::information(this,INFORMATION,(str),QMessageBox::Ok);
#define		WMSG(str)			QMessageBox::warning(this,WARNINGS,(str),QMessageBox::Ok);
#endif

class Samples_Qt_TriggerCount : public QWidget
{
	Q_OBJECT

public:
	Samples_Qt_TriggerCount(QWidget *parent = Q_NULLPTR);
	~Samples_Qt_TriggerCount();

	enum BUTTONPRESSTYPE {CONTINUITYSHOT = 0, SINGLESHOT, OUTSHOT, STOPSHOT, CAMERAERR};

	int OnStreamCB(MV_IMAGE_INFO *);

protected:
	void init();
	void CameraInitAfter();
	void changeUi_Button(const BUTTONPRESSTYPE &);	
	void drawImageForWindow();

public slots:
	void on_lxcj_clicked();
	void on_rcf_clicked();
	void on_wcf_clicked();
	void on_tz_clicked();

	void on_Set_clicked();
	void on_saveImage_clicked();

private:
	Ui::Samples_Qt_TriggerCountClass ui;

	DWORD	m_nIdxCam;
	HANDLE hCamera, m_hProperty;
	MVImage mImage, pDstImage;

	unsigned int shotNumber;
	short hImage, wImage;

	QImage tImage;
	bool saveImageFlag;
	QString saveImagePath;
	unsigned short displayScale;

	BUTTONPRESSTYPE currentModel;

	//计数互斥
	bool HCflag;
};
