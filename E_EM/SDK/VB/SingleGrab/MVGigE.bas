Attribute VB_Name = "Module1"
Enum MVSTATUS_CODES
    MVST_SUCCESS = 0                '      ///< û�д���
    MVST_ERROR = -1001                  '  ///< һ�����
    MVST_ERR_NOT_INITIALIZED = -1002    '  '< û�г�ʼ��
    MVST_ERR_NOT_IMPLEMENTED = -1003    '  '< û��ʵ��
    MVST_ERR_RESOURCE_IN_USE = -1004    '  '< ��Դ��ռ��
    MVST_ACCESS_DENIED = -1005          '  ///< �޷�����
    MVST_INVALID_Long = -1006         '  ///< ������
    MVST_INVALID_ID = -1007             '  ///< ����ID
    MVST_NO_DATA = -1008                '  ///< û������
    MVST_INVALID_PARAMETER = -1009      '  ///< �������
    MVST_FILE_IO = -1010                '  ///< IO����
    MVST_TIMEOUT = -1011                '  ///< ��ʱ
    MVST_ERR_ABORT = -1012              '  ///< �˳�
    MVST_INVALID_BUFFER_SIZE = -1013    '  ///< �������ߴ����
    MVST_ERR_NOT_AVAILABLE = -1014      '  ///< �޷�����
    MVST_INVALID_ADDRESS = -1015        '  ///< ��ַ����
End Enum

Enum MV_PixelFormatEnums
    PixelFormat_Mono8 = 0 '  '<8Bit�Ҷ�
    PixelFormat_BayerBG8 = 1 '   '<8Bit Bayerͼ'��ɫģʽΪBGGR
    PixelFormat_BayerRG8 = 2 '   '<8Bit Bayerͼ'��ɫģʽΪRGGB
    PixelFormat_BayerGB8 = 3 '   '<8Bit Bayerͼ'��ɫģʽΪGBRG
    PixelFormat_BayerGR8 = 4 '   '<8Bit Bayerͼ'��ɫģʽΪGRBG
    PixelFormat_BayerGRW8 = 5
End Enum


Enum TriggerSourceEnums
    TriggerSource_Software = 0 '<����ģʽ�£�������(���ָ��)�������ɼ�
    TriggerSource_Line1 = 2 '<����ģʽ�£����ⴥ���ź��������ɼ�
End Enum

Enum TriggerModeEnums
    TriggerMode_Off = 0 '<����ģʽ�أ���FreeRunģʽ����������ɼ�
    TriggerMode_On = 1 '<����ģʽ��������ȴ��������ⴥ���ź��ٲɼ�ͼ��
End Enum

Enum TriggerActivationEnums
    TriggerActivation_RisingEdge = 0 '<�����ش���
    TriggerActivation_FallingEdge = 1 '<�½��ش���
End Enum

Enum LineSourceEnums

    LineSource_Off = 0 '<�ر�
    LineSource_ExposureActive = 5 '<���ع�ͬʱ
    LineSource_Timer1Active = 6 '<�ɶ�ʱ������
    LineSource_UserOutput0 = 12 '<ֱ�����������
End Enum

Enum UserSetSelectorEnums
    UserSetSelector_Default = 0 '<��������
    UserSetSelector_UserSet1 = 1 '<�û�����1
    UserSetSelector_UserSet2 = 2 '<�û�����2
End Enum

Enum AutoFunctionProfileEnums
    AutoFunctionProfile_GainMinimum = 0 '<Keep gain at minimum
    AutoFunctionProfile_ExposureMinimum = 1 '<Exposure Time at minimum
End Enum

Enum GainAutoEnums
    GainAuto_Off = 0 '<Disables the Gain Auto function.
    GainAuto_Once = 1 '<Sets operation mode to 'once'.
    GainAuto_Continuous = 2 '<Sets operation mode to 'continuous'.
End Enum

Enum ExposureAutoEnums
    ExposureAuto_Off = 0 '<Disables the Exposure Auto function.
    ExposureAuto_Once = 1 '<Sets operation mode to 'once'.
    ExposureAuto_Continuous = 2 '<Sets operation mode to 'continuous'.
End Enum

Enum BalanceWhiteAutoEnums
    BalanceWhiteAuto_Off = 0 '<Disables the Balance White Auto function.
    BalanceWhiteAuto_Once = 1 '<Sets operation mode to 'once'.
    BalanceWhiteAuto_Continuous = 2 '<Sets operation mode to 'continuous'.
End Enum


Public Declare Function MVUpdateCameraList Lib "MVGigE.dll" () As Long
'\brief ��ȡ���ӵ�������ϵ����������
'\param [out]   pNumCams        �������
Public Declare Function MVGetNumOfCameras Lib "MVGigE.dll" (ByRef num As Long) As MVSTATUS_CODES
'\brief �õ���idx���������Ϣ��
'\param [in]    idx idx��0��ʼ�����������IP��ַ���򣬵�ַС������ǰ�档
'\param [out]   pCamInfo  �������Ϣ (IP,MAC,SN,�ͺ�...)

'\brief �򿪵�idx�����
'\param [in]    idx     idx��0��ʼ�����������IP��ַ���򣬵�ַС������ǰ�档
Public Declare Function MVOpenCamByIndex Lib "MVGigE.dll" (ByVal idx As Byte, ByRef hCam As Long) As MVSTATUS_CODES
'\brief ��ָ��UserDefinedName�����
'\param [in]    name UserDefinedName��
'\brief �ر�������Ͽ�����������ӡ�
Public Declare Function MVCloseCam Lib "MVGigE.dll" (ByVal hCam As Long) As MVSTATUS_CODES

Public Declare Function MVSingleGrab Lib "MVGigE.dll" (ByVal hCam As Long, ByVal hImage As Long, ByVal nWaitMs As Long) As MVSTATUS_CODES

'\brief        ��ȡͼ����
'\param [out]  Long pWidth   ͼ����[����]
Public Declare Function MVGetWidth Lib "MVGigE.dll" (ByVal hCam As Long, ByRef width As Long) As MVSTATUS_CODES

'\brief        ��ȡͼ��߶�
'\param [out]  Long pHeight  ͼ��߶�[����]
Public Declare Function MVGetHeight Lib "MVGigE.dll" (ByVal hCam As Long, ByRef height As Long) As MVSTATUS_CODES

'\brief    ��ȡͼ������ظ�ʽ
'\param [out]  MV_PixelFormatEnums pPixelFormat
Public Declare Function MVGetPixelFormat Lib "MVGigE.dll" (ByVal hCam As Long, ByRef height As MV_PixelFormatEnums) As MVSTATUS_CODES


'\brief    ��ȡ��ǰ����ֵ
'\param [out]  Double pGain
Public Declare Function MVGetGain Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGain As Double) As MVSTATUS_CODES


'\brief    ��ȡ����������õķ�Χ
'\param [out]  Double pGainMin       ��Сֵ
'\param [out]  Double pGainMax       ���ֵ
Public Declare Function MVGetGainRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGainMin As Double, ByRef pGainMax As Double) As MVSTATUS_CODES


'\brief    ��������
'\param [in]   Double fGain    ����
Public Declare Function MVSetGain Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fGain As Double) As MVSTATUS_CODES

'\brief    ��ȡ��ǰ��ƽ��ϵ��
'\param [out]  Double pRed   ��ɫƽ��ϵ��
'\param [out]  Double pGreen ��ɫƽ��ϵ��
'\param [out]  Double pBlue  ��ɫƽ��ϵ��
Public Declare Function MVGetWhiteBalance Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pRed As Double, ByRef pGreen As Double, ByRef pBlue As Double) As MVSTATUS_CODES


'\brief        ��ȡ��ƽ�����õķ�Χ
'\param [out]  Double pMin   ϵ����Сֵ
'\param [out]  Double pMax   ϵ�����ֵ
Public Declare Function MVGetWhiteBalanceRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Double, ByRef pMax As Double) As MVSTATUS_CODES


'\brief    ���ð�ƽ��ϵ��
'\param [in]   Double fRed             ��ɫƽ��ϵ��
'\param [in]   Double fGreen   ��ɫƽ��ϵ��
'\param [in]   Double fBlue    ��ɫƽ��ϵ��
Public Declare Function MVSetWhiteBalance Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fRed As Double, ByVal fGreen As Double, ByVal fBlue As Double) As MVSTATUS_CODES


'\brief ��ȡ��ǰ�ع�ʱ��
'\param [in]    pExposuretime   ��λus
Public Declare Function MVGetExposureTime Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pExposuretime As Double) As MVSTATUS_CODES


'\brief  ��ȡ�ع�ʱ������÷�Χ
'\param [out]  Double pExpMin        ����ع�ʱ�� ��λΪus
'\param [out]  Double pExpMax        ��ع�ʱ�� ��λΪus
Public Declare Function MVGetExposureTimeRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pExpMin As Double, ByRef pExpMax As Double) As MVSTATUS_CODES


'\brief        �����ع�ʱ��
'\param [in]   Long nExp_us �ع�ʱ�� ��λΪus
Public Declare Function MVSetExposureTime Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nExp_us As Double) As MVSTATUS_CODES


'\brief        ��ȡ֡�ʿ����õķ�Χ
'\param [out]  DoublepFpsMin ���֡��
'\param [out]  DoublepFpsMax ���֡��
Public Declare Function MVGetFrameRateRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pFpsMin As Double, ByRef pFpsMax As Double) As MVSTATUS_CODES


'\brief        ��ȡ��ǰ֡��
'\param [out]  Double 'fFPS   ֡�� ֡/��
Public Declare Function MVGetFrameRate Lib "MVGigE.dll" (ByVal hCam As Long, ByRef fFPS As Double) As MVSTATUS_CODES


'\brief    ����֡��
'\param [in]   Double fps      ֡�� ֡/��
Public Declare Function MVSetFrameRate Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fps As Double) As MVSTATUS_CODES


'\brief    ��ȡ����ģʽ
'\param [out]  TriggerModeEnums pMode        ����ģʽ  TriggerMode_Off,TriggerMode_On
Public Declare Function MVGetTriggerMode Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMode As TriggerModeEnums) As MVSTATUS_CODES

 
'\brief ���ô���ģʽ
'\param [in]    mode    ����ģʽ
'TriggerMode_Off����������������ɼ�ģʽ��
'TriggerMode_On:��������ڴ���ģʽ����Ҫ���ⴥ���źŻ�����ָ�������
Public Declare Function MVSetTriggerMode Lib "MVGigE.dll" (ByVal hCam As Long, ByVal mode As TriggerModeEnums) As MVSTATUS_CODES


'\brief    ��ȡ����Դ
'\param [out]  TriggerSourceEnums pSource    ����Դ���������ⴥ��
Public Declare Function MVGetTriggerSource Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pSource As TriggerSourceEnums) As MVSTATUS_CODES

 
'\brief ���ô���Դ
'\param [in]    TriggerSourceEnums      source ����Դ
'TriggerSource_Software��ͨ��\c MVTriggerSoftware()����������
'TriggerSource_Line1: ͨ�����ӵĴ����ߴ���?
Public Declare Function MVSetTriggerSource Lib "MVGigE.dll" (ByVal hCam As Long, ByVal source As TriggerSourceEnums) As MVSTATUS_CODES


'\brief    ��ȡ��������
'\param [out]  TriggerActivationEnums pAct
'TriggerActivation_RisingEdge: �����ش���
'TriggerActivation_FallingEdge: �½��ش���

Public Declare Function MVGetTriggerActivation Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAct As TriggerActivationEnums) As MVSTATUS_CODES

 
'\brief ��ʹ�ô����ߴ���ʱ,�����������ش��������½��ش���
'\param [in]    act �����ػ��½���
'TriggerActivation_RisingEdge: �����ش���
'TriggerActivation_FallingEdge: �½��ش���
Public Declare Function MVSetTriggerActivation Lib "MVGigE.dll" (ByVal hCam As Long, ByVal act As TriggerActivationEnums) As MVSTATUS_CODES


'\brief        ��ȡ������ʱ
'\param [out]  Long pDelay_us   ������ʱ,��λus
Public Declare Function MVGetTriggerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDelay_us As Long) As MVSTATUS_CODES


'\brief    ��ȡ������ʱ��Χ
'\param [out]  Long pMin        ������ʱ��Сֵ,��λus
'\param [out]  Long pMax ������ʱ���ֵ,��λus
Public Declare Function MVGetTriggerDelayRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'\brief ��������ӵ������źź��ӳٶ���΢����ٿ�ʼ�ع⡣
'\param [in]    nDelay_us
Public Declare Function MVSetTriggerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDelay_us As Long) As MVSTATUS_CODES

 
'\brief �����������ָ��
Public Declare Function MVTriggerSoftware Lib "MVGigE.dll" (ByVal hCam As Long) As MVSTATUS_CODES


'\brief        ��ȡ����ͬ���ź�Դ
'\param [out]  LineSourceEnums pSource       ����ͬ���ź�Դ
Public Declare Function MVGetStrobeSource Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pSource As LineSourceEnums) As MVSTATUS_CODES

 
'\brief ����ͬ���ź�Դ
'\param [in]    source
'LineSource_Off: �ر�����ͬ��
'LineSource_ExposureActive: �ع��ͬʱ����
'LineSource_Timer1Active: �ɶ�ʱ������
'LineSource_UserOutput0: ���û�ͨ��ָ�����
Public Declare Function MVSetStrobeSource Lib "MVGigE.dll" (ByVal hCam As Long, ByVal source As LineSourceEnums) As MVSTATUS_CODES


'\brief        ��ȡ����ͬ���Ƿ�ת
'\param [out]  Long pInvert
Public Declare Function MVGetStrobeInvert Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pInvert As Long) As MVSTATUS_CODES

 
'\brief ����ͬ���Ƿ�ת��������ͬ����Чʱ����ߵ�ƽ���ǵ͵�ƽ��
'\param [in]    bInvert
Public Declare Function MVSetStrobeInvert Lib "MVGigE.dll" (ByVal hCam As Long, ByVal bInvert As Long) As MVSTATUS_CODES


'\brief        ��ȡ�û����õ�����ͬ��
'\param [out]  Long 'vSet
Public Declare Function MVGetUserOutputValue0 Lib "MVGigE.dll" (ByVal hCam As Long, ByRef vSet As Long) As MVSTATUS_CODES

 
'\brief ������ͬ��ԴѡΪUserOutputʱ
'��������ͨ��MVSetUserOutputValue0����������ͬ������ߵ�ƽ��͵�ƽ?
'\param [in]    bSet ���õ�ƽ
Public Declare Function MVSetUserOutputValue0 Lib "MVGigE.dll" (ByVal hCam As Long, ByVal bSet As Long) As MVSTATUS_CODES


'\brief ����������ʱʱ��
'\param [in]    Long nTimeOut  ������ʱʱ�� ��λms
Public Declare Function MVSetHeartbeatTimeout Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nTimeOut As Long) As MVSTATUS_CODES


'\brief        ��ȡ�������ݰ���С
'\param [out]  Long *pPacketSize ���ݰ���С
Public Declare Function MVGetPacketSize Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pPacketSize As Long) As MVSTATUS_CODES


'\brief    ��ȡ�������ݰ���С�ķ�Χ��
'\param [out]  Long pMin    �������ݰ���Сֵ
'\param [out]  Long pMax �������ݰ����ֵ
Public Declare Function MVGetPacketSizeRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'\brief �����������ݰ��Ĵ�С��
'\param [in]    nPacketSize �������ݰ���С(��λ:�ֽ�)���ô�С����С�������ܹ�֧�ֵ�������֡(Jumbo Frame)��
Public Declare Function MVSetPacketSize Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nPacketSize As Long) As MVSTATUS_CODES


'\brief        ��ȡ�������ݰ������
'\param [out]  Long *pDelay_us ���ݰ����ʱ�䣬��λus
Public Declare Function MVGetPacketDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDelay_us As Long) As MVSTATUS_CODES


'\brief    ��ȡ�������ݰ������Χ
'\param [out]  Long pMin    ���ݰ����ʱ����Сֵ����λus
'\param [out]  Long pMax ���ݰ����ʱ�����ֵ����λus
Public Declare Function MVGetPacketDelayRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'\brief �����������ݰ�֮���ʱ�����������������Ե�����Ƿ�ѣ��޷�������ٵ�������ݰ����ᵼ�¶�ʧ���ݰ���
'�Ӷ�ʹͼ������������ͨ���������ݰ�֮���ʱ�����Ա�֤ͼ���䡣�������Ӹ�ֵ������ͼ���ʱ���ӳ٣�
'���п���Ӱ��֡��?
'\param [in]    nDelay_us ʱ����(��λ:΢��)
Public Declare Function MVSetPacketDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDelay_us As Long) As MVSTATUS_CODES


'\brief    ��ȡ��ʱ����ʱ
'\param [out]  Long pDelay      ��ʱ����ʱ
Public Declare Function MVGetTimerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDelay As Long) As MVSTATUS_CODES


'\brief        ��ȡ��ʱ����ʱ�ķ�Χ
'\param [out]  Long pMin        ��ʱ����ʱ����Сֵ
'\param [out]  Long pMax ��ʱ����ʱ�����ֵ
Public Declare Function MVGetTimerDelayRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'����Timer1�ڽӵ������źź��ӳٶ���us��ʼ��ʱ
'\param [in]    Long nDelay �ӵ������źź��ӳٶ���us��ʼ��ʱ
Public Declare Function MVSetTimerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDelay As Long) As MVSTATUS_CODES


'\brief    ��ȡ��ʱ����ʱʱ��
'\param [out]  Long pDuration   ��ʱ����ʱʱ��
Public Declare Function MVGetTimerDuration Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDuration As Long) As MVSTATUS_CODES


'\brief    ��ȡ��ʱ����ʱʱ��ȡֵ��Χ
'\param [out]  Long pMin        ��ʱ����ʱʱ����Сֵ
'\param [out]  Long pMax        ��ʱ����ʱʱ�����ֵ
Public Declare Function MVGetTimerDurationRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'����Timer1�ڿ�ʼ��ʱ�󣬼�ʱ�೤ʱ�䡣
'\param [in]    Long nDuration ����Timer1�ڿ�ʼ��ʱ�󣬼�ʱ�೤ʱ��(us)���������/�͵�ƽ�������ȡ�
Public Declare Function MVSetTimerDuration Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDuration As Long) As MVSTATUS_CODES



'\brief ��ȡ��ǰ�Զ��ع�ģʽ
'\param [out]   ExposureAutoEnums pExposureAuto       ��ǰ�Զ��ع�ģʽ
Public Declare Function MVGetExposureAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pExposureAuto As ExposureAutoEnums) As MVSTATUS_CODES


'\brief �����Զ��ع�ģʽ
'\param [in]    ExposureAutoEnums ExposureAuto  �Զ��ع�ģʽ
Public Declare Function MVSetExposureAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByVal ExposureAuto As ExposureAutoEnums) As MVSTATUS_CODES

        
'\brief ��ȡ��ǰ�Զ�����ģʽ
'\param [out]   GainAutoEnums pGainAuto       ��ǰ�Զ�����ģʽ��
Public Declare Function MVGetGainAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGainAuto As GainAutoEnums) As MVSTATUS_CODES


'\brief ���õ�ǰ�Զ�����ģʽ
'\param [in]    GainAutoEnums GainAuto  �Զ�����ģʽ
Public Declare Function MVSetGainAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByVal GainAuto As GainAutoEnums) As MVSTATUS_CODES


'\brief ��ȡ��ǰ�Զ���ƽ��ģʽ
'\param [out]   BalanceWhiteAutoEnums pBalanceWhiteAuto       ��ǰ�Զ���ƽ��ģʽ
Public Declare Function MVGetBalanceWhiteAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pBalanceWhiteAuto As BalanceWhiteAutoEnums) As MVSTATUS_CODES


'\brief �����Զ���ƽ��ģʽ
'\param [in]    BalanceWhiteAutoEnums BalanceWhiteAuto  �Զ���ƽ��ģʽ
Public Declare Function MVSetBalanceWhiteAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByVal BalanceWhiteAuto As BalanceWhiteAutoEnums) As MVSTATUS_CODES


'\brief ��ȡ�Զ���������ʱ�����������Χ����Сֵ
'\param [out]   Double pAutoGainLowerLimit    ���������Χ����Сֵ
Public Declare Function MVGetAutoGainLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoGainLowerLimit As Double) As MVSTATUS_CODES


'\brief �����Զ���������ʱ�����������Χ����Сֵ
'\param [in]    Double fAutoGainLowerLimit      ���������Χ����Сֵ
Public Declare Function MVSetAutoGainLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoGainLowerLimit As Double) As MVSTATUS_CODES


'\brief ��ȡ�Զ���������ʱ�����������Χ�����ֵ
'\param [out]   Double pAutoGainUpperLimit    ���������Χ�����ֵ
Public Declare Function MVGetAutoGainUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoGainUpperLimit As Double) As MVSTATUS_CODES


'\brief �����Զ���������ʱ�����������Χ�����ֵ
'\param [in]    Double fAutoGainUpperLimit      �ع�ʱ�������Χ����Сֵ
Public Declare Function MVSetAutoGainUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoGainUpperLimit As Double) As MVSTATUS_CODES


'\brief ��ȡ�Զ������ع�ʱ��ʱ���ع�ʱ�������Χ����Сֵ
'\param [out]   Double pAutoExposureTimeLowerLimit    �ع�ʱ�������Χ����Сֵ
Public Declare Function MVGetAutoExposureTimeLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoExposureTimeLowerLimit As Double) As MVSTATUS_CODES


'\brief �����Զ������ع�ʱ��ʱ���ع�ʱ�������Χ����Сֵ
'\param [in]    Double fAutoExposureTimeLowerLimit      �ع�ʱ�������Χ�����ֵ
Public Declare Function MVSetAutoExposureTimeLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoExposureTimeLowerLimit As Double) As MVSTATUS_CODES


'\brief ��ȡ�Զ������ع�ʱ��ʱ���ع�ʱ�������Χ�����ֵ
'\param [out]   Double pAutoExposureTimeUpperLimit    �ع�ʱ�������Χ�����ֵ
Public Declare Function MVGetAutoExposureTimeUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoExposureTimeUpperLimit As Double) As MVSTATUS_CODES


'\brief �����Զ������ع�ʱ��ʱ���ع�ʱ�������Χ�����ֵ
'\param [in]    Double fAutoExposureTimeUpperLimit
Public Declare Function MVSetAutoExposureTimeUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoExposureTimeUpperLimit As Double) As MVSTATUS_CODES


'\brief ��ȡ�Զ���������(�ع⡢����)ʱ��������������ͼ������
'\param [out]   Long pAutoTargetValue ������������ͼ������
Public Declare Function MVGetAutoTargetValue Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoTargetValue As Long) As MVSTATUS_CODES


'\brief �����Զ���������(�ع⡢����)ʱ��������������ͼ������
'\param [in]    Long nAutoTargetValue   ������������ͼ������
Public Declare Function MVSetAutoTargetValue Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nAutoTargetValue As Long) As MVSTATUS_CODES


'\brief ���Զ�������Զ��ع�ʱ�䶼��ʱ����ȡ��һ��ֵ���ȵ���
'\param [out]   AutoFunctionProfileEnums pAutoFunctionProfile �������Ȼ��ع�ʱ������
Public Declare Function MVGetAutoFunctionProfile Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoFunctionProfile As AutoFunctionProfileEnums) As MVSTATUS_CODES


'\brief ���Զ�������Զ��ع�ʱ�䶼��ʱ��������һ��ֵ���ȵ���
'\param [in]    AutoFunctionProfileEnums AutoFunctionProfile    �������Ȼ��ع�ʱ������
Public Declare Function MVSetAutoFunctionProfile Lib "MVGigE.dll" (ByVal hCam As Long, ByVal AutoFunctionProfile As AutoFunctionProfileEnums) As MVSTATUS_CODES


'\brief �Զ�������Զ��ع�ʱ��ͼ��������Ŀ�����Ȳ�����ݲ
'\param [out]   Long pAutoThreshold   ͼ��������Ŀ�����Ȳ�����ݲ�
Public Declare Function MVGetAutoThreshold Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoThreshold As Long) As MVSTATUS_CODES


'\brief �Զ�������Զ��ع�ʱ��ͼ��������Ŀ�����Ȳ�����ݲ
'\param [in]    Long nAutoThreshold     ͼ��������Ŀ�����Ȳ�����ݲ�
Public Declare Function MVSetAutoThreshold Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nAutoThreshold As Long) As MVSTATUS_CODES


'\brief ��ȡ��ǰ٤��ֵ
'\param [out]   Double pGamma ��ǰ٤��ֵ
Public Declare Function MVGetGamma Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGamma As Double) As MVSTATUS_CODES


'\brief ��ȡ٤��ֵ�����õķ�Χ
'\param [out]   Double pGammaMin      ٤����Сֵ
'\param [out]   Double pGammaMax      ٤�����ֵ
Public Declare Function MVGetGammaRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGammaMin As Double, ByRef pGammaMax As Double) As MVSTATUS_CODES

'\brief ����٤��
'\param [out]   Double fGamma ٤��ֵ
Public Declare Function MVSetGamma Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fGamma As Double) As MVSTATUS_CODES


Public Declare Function MVImageCreate Lib "MVGigE.dll" (ByVal width As Long, ByVal height As Long, ByVal bpp As Long) As Long
Public Declare Function MVImageDraw Lib "MVGigE.dll" (ByVal hImage As Long, ByVal hdc As Long, ByVal xDest As Long, ByVal yDest As Long) As Long
Public Declare Function MVImageDrawHwnd Lib "MVGigE.dll" (ByVal hImage As Long, ByVal hWnd As Long, ByVal xDest As Long, ByVal yDest As Long) As Long
Public Declare Function MVImageRelease Lib "MVGigE.dll" (ByVal hImage As Long)
Public Declare Sub MVImageSave Lib "MVGigE.dll" (ByVal hImage As Long, ByVal fname As String)

Public Declare Function MVStartGrabWindow Lib "MVGigE.dll" (ByVal hCam As Long, ByVal hWnd As Long) As MVSTATUS_CODES
Public Declare Function MVStopGrabWindow Lib "MVGigE.dll" (ByVal hCam As Long) As MVSTATUS_CODES
Public Declare Function MVFreezeGrabWindow Lib "MVGigE.dll" (ByVal hCam As Long, ByVal bFreeze As Long) As MVSTATUS_CODES
Public Declare Function MVGetSampleGrab Lib "MVGigE.dll" (ByVal hCam As Long, ByVal hImage As Long, ByRef FrameID As Long) As MVSTATUS_CODES
Public Declare Function MVSetGrabWindow Lib "MVGigE.dll" (ByVal hCam As Long, ByVal xDest As Long, ByVal yDest As Long, ByVal wDest As Long, ByVal hDest As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal wSrc As Long, ByVal hSrc As Long) As MVSTATUS_CODES
