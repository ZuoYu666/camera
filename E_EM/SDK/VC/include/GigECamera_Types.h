#ifndef GENICAM_TYPES_H
#define GENICAM_TYPES_H

#if ! defined(_MSC_VER)

  #include <stdint.h>

  #define interface struct

#else // if defined(_MSC_VER)

  //! \addtogroup Base_PublicUtilities
  //! \{

  //! makes int 8 portable across different platforms
  typedef char int8_t;
  //! makes char 8 portable across different platforms
  typedef char char8_t;
  //! makes unsigned char portable across different platforms
  typedef unsigned char uint8_t;
  //! makes char 8 portable across different platforms
  typedef unsigned char uchar8_t;

  //! makes short portable across different platforms
  typedef short int16_t;
  //! makes unsigned short portable across different platforms
  typedef unsigned short uint16_t;

  //! makes __int32 portable across different platforms
  typedef __int32 int32_t;
  //! makes unsigned int32 portable across different platforms
  typedef unsigned __int32 uint32_t;

  //! makes int64 portable across different platforms
  typedef __int64 int64_t;
  //! makes unsigned int64 portable across different platforms
  typedef unsigned __int64 uint64_t;

  //! use a universal platform dependend int
  typedef __int64 int_t;
  //! use a universal platform dependend unsigned int
  typedef unsigned __int64 uint_t;

  #ifndef _SIZE_T_DEFINED
    #ifdef  _WIN64
      //! use a universal platform dependend unsigned int
      typedef unsigned __int64 size_t;
    #else
      //! use a universal platform dependend unsigned int
      typedef unsigned int size_t;
    #endif
    #define _SIZE_T_DEFINED
  #endif

  #ifndef _INTPTR_T_DEFINED
    #ifdef  _WIN64
      //! use a universal platform dependend int
      typedef __int64 intptr_t;
    #else
      //! use a universal platform dependend int
      typedef int intptr_t;
    #endif
    #define _INTPTR_T_DEFINED
  #endif
#endif

#define INT64_MAX     0x7fffffffffffffffLL  /*maximum signed __int64 value */
#define INT64_MIN     0x8000000000000000LL  /*minimum signed __int64 value */
#define UINT64_MAX    0xffffffffffffffffULL  /*maximum unsigned __int64 value */

#define INT32_MAX     0x000000007fffffffLL  /*maximum signed __int32 value */
#define INT32_MIN     0xffffffff80000000LL  /*minimum signed __int32 value */
#define UINT32_MAX    0x00000000ffffffffULL  /*maximum unsigned __int32 value */

#define INT8_MAX     0x000000000000007fLL  /*maximum signed __int8 value */
#define INT8_MIN     0xffffffffffffff80LL  /*minimum signed __int8 value */
#define UINT8_MAX    0x00000000000000ffULL  /*maximum unsigned __int8 value */
  //! \}


/* 
*  \brief �ص������õ������ݵĽṹ
*/
typedef struct _IMAGE_INFO
{
	uint64_t	nTimeStamp;		///< ʱ������ɼ���ͼ���ʱ�̣�����Ϊ0.01us
	USHORT		nBlockId;		///< ֡�ţ��ӿ�ʼ�ɼ���ʼ����
	UCHAR		*pImageBuffer;	///< ͼ��ָ�룬��ָ��(0,0)���������ڴ�λ�õ�ָ�룬ͨ����ָ����Է�������ͼ��
	ULONG		nImageSizeAcq;	///< �ɼ�����ͼ���С[�ֽ�]
	UCHAR		nMissingPackets;///< ��������ж����İ�����
	uint64_t	nPixelType;		///< ͼ���ʽ
	uint32_t	nSizeX;			///< ͼ����
	uint32_t	nSizeY;         ///< ͼ��߶�
	uint32_t	nOffsetX;		///< 0
	uint32_t	nOffsetY;       ///< 0
} MV_IMAGE_INFO, * pMV_IMAGE_INFO ;

/* 
*  \brief Bayer��ɫģʽ
*/
typedef enum {	
	BayerRG,	//< ��ɫģʽRGGB
	BayerBG,	//< ��ɫģʽBGGR
	BayerGR,	//< ��ɫģʽGRBG
	BayerGB,	//< ��ɫģʽGBRG
	BayerGRW,	//< ��ɫģʽGRW
	BayerInvalid
} MV_BAYER_MODE;

/* 
*  \brief ͼ�����ظ�ʽ
*/
typedef enum {	
	PixelFormat_Mono8,	//!<8Bit�Ҷ�
	PixelFormat_BayerBG8,	//!<8Bit Bayerͼ,��ɫģʽΪBGGR
	PixelFormat_BayerRG8,	//!<8Bit Bayerͼ,��ɫģʽΪRGGB
	PixelFormat_BayerGB8,	//!<8Bit Bayerͼ,��ɫģʽΪGBRG
	PixelFormat_BayerGR8,	//!<8Bit Bayerͼ,��ɫģʽΪGRBG
	PixelFormat_BayerGRW8
} MV_PixelFormatEnums;

/* 
*  \brief ���󷵻�ֵ����
*/
typedef enum  
{
	MVST_SUCCESS                = 0,      ///< û�д���      
	MVST_ERROR                  = -1001,  ///< һ�����
	MVST_ERR_NOT_INITIALIZED    = -1002,  //!< û�г�ʼ��
	MVST_ERR_NOT_IMPLEMENTED    = -1003,  //!< û��ʵ��
	MVST_ERR_RESOURCE_IN_USE    = -1004,  //!< ��Դ��ռ��
	MVST_ACCESS_DENIED          = -1005,  ///< �޷�����
	MVST_INVALID_HANDLE         = -1006,  ///< ������
	MVST_INVALID_ID             = -1007,  ///< ����ID
	MVST_NO_DATA                = -1008,  ///< û������
	MVST_INVALID_PARAMETER      = -1009,  ///< �������
	MVST_FILE_IO                = -1010,  ///< IO����
	MVST_TIMEOUT                = -1011,  ///< ��ʱ
	MVST_ERR_ABORT              = -1012,  ///< �˳�
	MVST_INVALID_BUFFER_SIZE    = -1013,  ///< �������ߴ����
	MVST_ERR_NOT_AVAILABLE      = -1014,  ///< �޷�����
	MVST_INVALID_ADDRESS        = -1015,  ///< ��ַ����
}MVSTATUS_CODES;

typedef struct  
{
	unsigned char mIpAddr[4];	//!<�����IP��ַ
	unsigned char mEthernetAddr[6];	//!<�����MAC��ַ
	char mMfgName[32];	//!<�����������
	char mModelName[32]; //!<����ͺ�
	char mSerialNumber[16];	//!<������к�
	char mUserDefinedName[16];	//!<�û����õ��������
	unsigned char m_IfIp[4];	//!<�������������ӵ�����IP��ַ
	unsigned char m_IfMAC[6];	//!<�������������ӵ�����MAC��ַ
}MVCamInfo;

enum TriggerSourceEnums
{
	TriggerSource_Software=0,//!<����ģʽ�£�������(���ָ��)�������ɼ�
	TriggerSource_Line1=2 //!<����ģʽ�£����ⴥ���ź��������ɼ�
};

enum TriggerModeEnums
{
	TriggerMode_Off,  //!<����ģʽ�أ���FreeRunģʽ����������ɼ�
	TriggerMode_On	//!<����ģʽ��������ȴ��������ⴥ���ź��ٲɼ�ͼ��
};

enum TriggerActivationEnums
{
	TriggerActivation_RisingEdge,//!<�����ش���
	TriggerActivation_FallingEdge//!<�½��ش���
};

enum LineSourceEnums
{
	LineSource_Off=0,  //!<�ر�
	LineSource_ExposureActive=5,  //!<���ع�ͬʱ
	LineSource_Timer1Active=6,	//!<�ɶ�ʱ������
	LineSource_UserOutput0=12	//!<ֱ�����������
};

typedef struct  
{
	unsigned long m_nTotalBuf;	//!<�ӿ�ʼ�ɼ����ܼƳɹ��յ�������ͼ��֡��
	unsigned long m_nFailedBuf;	//!<�ӿ�ʼ�ɼ����ܼ��յ��Ĳ�����ͼ��֡��
	unsigned long m_nTotalPacket;	//!<�ӿ�ʼ�ɼ����ܼ��յ���ͼ�����ݰ���
	unsigned long m_nFailedPacket;	//!<�ӿ�ʼ�ɼ����ܼƶ�ʧ��ͼ�����ݰ���
	unsigned long m_nResendPacketReq;//!<�ӿ�ʼ�ɼ����ܼ��ط������ͼ�����ݰ���
	unsigned long m_nResendPacket;//!<�ӿ�ʼ�ɼ����ܼ��ط��ɹ���ͼ�����ݰ���
}MVStreamStatistic;

enum UserSetSelectorEnums
{
	UserSetSelector_Default,  //!<��������
	UserSetSelector_UserSet1,  //!<�û�����1
	UserSetSelector_UserSet2   //!<�û�����2
};

enum SensorTapsEnums
{
	SensorTaps_One,  //!<��ͨ��
	SensorTaps_Two,  //!<˫ͨ��
	SensorTaps_Three,  //!<��ͨ��
	SensorTaps_Four,  //!<��ͨ��
};

enum AutoFunctionProfileEnums
{
	AutoFunctionProfile_GainMinimum,  //!<Keep gain at minimum
	AutoFunctionProfile_ExposureMinimum   //!<Exposure Time at minimum
};

enum GainAutoEnums
{
	GainAuto_Off,  //!<Disables the Gain Auto function.
	GainAuto_Once,  //!<Sets operation mode to 'once'.
	GainAuto_Continuous   //!<Sets operation mode to 'continuous'.
};

//! Valid values for ExposureAuto
enum ExposureAutoEnums
{
	ExposureAuto_Off,  //!<Disables the Exposure Auto function.
	ExposureAuto_Once,  //!<Sets operation mode to 'once'.
	ExposureAuto_Continuous   //!<Sets operation mode to 'continuous'.
};

enum BalanceWhiteAutoEnums
{
	BalanceWhiteAuto_Off,  //!<Disables the Balance White Auto function.
	BalanceWhiteAuto_Once,  //!<Sets operation mode to 'once'.
	BalanceWhiteAuto_Continuous   //!<Sets operation mode to 'continuous'.
};


//////////////////////////////////////////////////////////////////////////
enum ImageFlipType
{
	FlipHorizontal = 0,  //!< ���ҷ�ת
	FlipVertical = 1, //!< ���·�ת
	FlipBoth = 2 //!< ��ת180��
} ;

enum ImageRotateType
{
	Rotate90DegCw = 0,       //!< ˳ʱ����ת90��
	Rotate90DegCcw = 1       //!< ��ʱ����ת90��
};

#endif 
