Attribute VB_Name = "Module1"
Enum MVSTATUS_CODES
    MVST_SUCCESS = 0                '      ///< 没有错误
    MVST_ERROR = -1001                  '  ///< 一般错误
    MVST_ERR_NOT_INITIALIZED = -1002    '  '< 没有初始化
    MVST_ERR_NOT_IMPLEMENTED = -1003    '  '< 没有实现
    MVST_ERR_RESOURCE_IN_USE = -1004    '  '< 资源被占用
    MVST_ACCESS_DENIED = -1005          '  ///< 无法访问
    MVST_INVALID_Long = -1006         '  ///< 错误句柄
    MVST_INVALID_ID = -1007             '  ///< 错误ID
    MVST_NO_DATA = -1008                '  ///< 没有数据
    MVST_INVALID_PARAMETER = -1009      '  ///< 错误参数
    MVST_FILE_IO = -1010                '  ///< IO错误
    MVST_TIMEOUT = -1011                '  ///< 超时
    MVST_ERR_ABORT = -1012              '  ///< 退出
    MVST_INVALID_BUFFER_SIZE = -1013    '  ///< 缓冲区尺寸错误
    MVST_ERR_NOT_AVAILABLE = -1014      '  ///< 无法访问
    MVST_INVALID_ADDRESS = -1015        '  ///< 地址错误
End Enum

Enum MV_PixelFormatEnums
    PixelFormat_Mono8 = 0 '  '<8Bit灰度
    PixelFormat_BayerBG8 = 1 '   '<8Bit Bayer图'颜色模式为BGGR
    PixelFormat_BayerRG8 = 2 '   '<8Bit Bayer图'颜色模式为RGGB
    PixelFormat_BayerGB8 = 3 '   '<8Bit Bayer图'颜色模式为GBRG
    PixelFormat_BayerGR8 = 4 '   '<8Bit Bayer图'颜色模式为GRBG
    PixelFormat_BayerGRW8 = 5
End Enum


Enum TriggerSourceEnums
    TriggerSource_Software = 0 '<触发模式下，由软触发(软件指令)来触发采集
    TriggerSource_Line1 = 2 '<触发模式下，有外触发信号来触发采集
End Enum

Enum TriggerModeEnums
    TriggerMode_Off = 0 '<触发模式关，即FreeRun模式，相机连续采集
    TriggerMode_On = 1 '<触发模式开，相机等待软触发或外触发信号再采集图像
End Enum

Enum TriggerActivationEnums
    TriggerActivation_RisingEdge = 0 '<上升沿触发
    TriggerActivation_FallingEdge = 1 '<下降沿触发
End Enum

Enum LineSourceEnums

    LineSource_Off = 0 '<关闭
    LineSource_ExposureActive = 5 '<和曝光同时
    LineSource_Timer1Active = 6 '<由定时器控制
    LineSource_UserOutput0 = 12 '<直接由软件控制
End Enum

Enum UserSetSelectorEnums
    UserSetSelector_Default = 0 '<出厂设置
    UserSetSelector_UserSet1 = 1 '<用户设置1
    UserSetSelector_UserSet2 = 2 '<用户设置2
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
'\brief 获取连接到计算机上的相机的数量
'\param [out]   pNumCams        相机数量
Public Declare Function MVGetNumOfCameras Lib "MVGigE.dll" (ByRef num As Long) As MVSTATUS_CODES
'\brief 得到第idx个相机的信息。
'\param [in]    idx idx从0开始，按照相机的IP地址排序，地址小的排在前面。
'\param [out]   pCamInfo  相机的信息 (IP,MAC,SN,型号...)

'\brief 打开第idx个相机
'\param [in]    idx     idx从0开始，按照相机的IP地址排序，地址小的排在前面。
Public Declare Function MVOpenCamByIndex Lib "MVGigE.dll" (ByVal idx As Byte, ByRef hCam As Long) As MVSTATUS_CODES
'\brief 打开指定UserDefinedName的相机
'\param [in]    name UserDefinedName。
'\brief 关闭相机。断开和相机的连接。
Public Declare Function MVCloseCam Lib "MVGigE.dll" (ByVal hCam As Long) As MVSTATUS_CODES

Public Declare Function MVSingleGrab Lib "MVGigE.dll" (ByVal hCam As Long, ByVal hImage As Long, ByVal nWaitMs As Long) As MVSTATUS_CODES

'\brief        读取图像宽度
'\param [out]  Long pWidth   图像宽度[像素]
Public Declare Function MVGetWidth Lib "MVGigE.dll" (ByVal hCam As Long, ByRef width As Long) As MVSTATUS_CODES

'\brief        读取图像高度
'\param [out]  Long pHeight  图像高度[像素]
Public Declare Function MVGetHeight Lib "MVGigE.dll" (ByVal hCam As Long, ByRef height As Long) As MVSTATUS_CODES

'\brief    读取图像的像素格式
'\param [out]  MV_PixelFormatEnums pPixelFormat
Public Declare Function MVGetPixelFormat Lib "MVGigE.dll" (ByVal hCam As Long, ByRef height As MV_PixelFormatEnums) As MVSTATUS_CODES


'\brief    读取当前增益值
'\param [out]  Double pGain
Public Declare Function MVGetGain Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGain As Double) As MVSTATUS_CODES


'\brief    读取增益可以设置的范围
'\param [out]  Double pGainMin       最小值
'\param [out]  Double pGainMax       最大值
Public Declare Function MVGetGainRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGainMin As Double, ByRef pGainMax As Double) As MVSTATUS_CODES


'\brief    设置增益
'\param [in]   Double fGain    增益
Public Declare Function MVSetGain Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fGain As Double) As MVSTATUS_CODES

'\brief    读取当前白平衡系数
'\param [out]  Double pRed   红色平衡系数
'\param [out]  Double pGreen 绿色平衡系数
'\param [out]  Double pBlue  蓝色平衡系数
Public Declare Function MVGetWhiteBalance Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pRed As Double, ByRef pGreen As Double, ByRef pBlue As Double) As MVSTATUS_CODES


'\brief        读取白平衡设置的范围
'\param [out]  Double pMin   系数最小值
'\param [out]  Double pMax   系数最大值
Public Declare Function MVGetWhiteBalanceRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Double, ByRef pMax As Double) As MVSTATUS_CODES


'\brief    设置白平衡系数
'\param [in]   Double fRed             红色平衡系数
'\param [in]   Double fGreen   绿色平衡系数
'\param [in]   Double fBlue    蓝色平衡系数
Public Declare Function MVSetWhiteBalance Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fRed As Double, ByVal fGreen As Double, ByVal fBlue As Double) As MVSTATUS_CODES


'\brief 读取当前曝光时间
'\param [in]    pExposuretime   单位us
Public Declare Function MVGetExposureTime Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pExposuretime As Double) As MVSTATUS_CODES


'\brief  读取曝光时间的设置范围
'\param [out]  Double pExpMin        最短曝光时间 单位为us
'\param [out]  Double pExpMax        最长曝光时间 单位为us
Public Declare Function MVGetExposureTimeRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pExpMin As Double, ByRef pExpMax As Double) As MVSTATUS_CODES


'\brief        设置曝光时间
'\param [in]   Long nExp_us 曝光时间 单位为us
Public Declare Function MVSetExposureTime Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nExp_us As Double) As MVSTATUS_CODES


'\brief        读取帧率可设置的范围
'\param [out]  DoublepFpsMin 最低帧率
'\param [out]  DoublepFpsMax 最高帧率
Public Declare Function MVGetFrameRateRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pFpsMin As Double, ByRef pFpsMax As Double) As MVSTATUS_CODES


'\brief        读取当前帧率
'\param [out]  Double 'fFPS   帧率 帧/秒
Public Declare Function MVGetFrameRate Lib "MVGigE.dll" (ByVal hCam As Long, ByRef fFPS As Double) As MVSTATUS_CODES


'\brief    设置帧率
'\param [in]   Double fps      帧率 帧/秒
Public Declare Function MVSetFrameRate Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fps As Double) As MVSTATUS_CODES


'\brief    读取触发模式
'\param [out]  TriggerModeEnums pMode        触发模式  TriggerMode_Off,TriggerMode_On
Public Declare Function MVGetTriggerMode Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMode As TriggerModeEnums) As MVSTATUS_CODES

 
'\brief 设置触发模式
'\param [in]    mode    触发模式
'TriggerMode_Off：相机工作在连续采集模式，
'TriggerMode_On:相机工作在触发模式，需要有外触发信号或软触发指令才拍摄
Public Declare Function MVSetTriggerMode Lib "MVGigE.dll" (ByVal hCam As Long, ByVal mode As TriggerModeEnums) As MVSTATUS_CODES


'\brief    读取触发源
'\param [out]  TriggerSourceEnums pSource    触发源，软触发或外触发
Public Declare Function MVGetTriggerSource Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pSource As TriggerSourceEnums) As MVSTATUS_CODES

 
'\brief 设置触发源
'\param [in]    TriggerSourceEnums      source 触发源
'TriggerSource_Software：通过\c MVTriggerSoftware()函数触发。
'TriggerSource_Line1: 通过连接的触发线触发?
Public Declare Function MVSetTriggerSource Lib "MVGigE.dll" (ByVal hCam As Long, ByVal source As TriggerSourceEnums) As MVSTATUS_CODES


'\brief    读取触发极性
'\param [out]  TriggerActivationEnums pAct
'TriggerActivation_RisingEdge: 上升沿触发
'TriggerActivation_FallingEdge: 下降沿触发

Public Declare Function MVGetTriggerActivation Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAct As TriggerActivationEnums) As MVSTATUS_CODES

 
'\brief 当使用触发线触发时,设置是上升沿触发还是下降沿触发
'\param [in]    act 上升沿或下降沿
'TriggerActivation_RisingEdge: 上升沿触发
'TriggerActivation_FallingEdge: 下降沿触发
Public Declare Function MVSetTriggerActivation Lib "MVGigE.dll" (ByVal hCam As Long, ByVal act As TriggerActivationEnums) As MVSTATUS_CODES


'\brief        读取触发延时
'\param [out]  Long pDelay_us   触发延时,单位us
Public Declare Function MVGetTriggerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDelay_us As Long) As MVSTATUS_CODES


'\brief    读取触发延时范围
'\param [out]  Long pMin        触发延时最小值,单位us
'\param [out]  Long pMax 触发延时最大值,单位us
Public Declare Function MVGetTriggerDelayRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'\brief 设置相机接到触发信号后延迟多少微秒后再开始曝光。
'\param [in]    nDelay_us
Public Declare Function MVSetTriggerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDelay_us As Long) As MVSTATUS_CODES

 
'\brief 发出软件触发指令
Public Declare Function MVTriggerSoftware Lib "MVGigE.dll" (ByVal hCam As Long) As MVSTATUS_CODES


'\brief        读取闪光同步信号源
'\param [out]  LineSourceEnums pSource       闪光同步信号源
Public Declare Function MVGetStrobeSource Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pSource As LineSourceEnums) As MVSTATUS_CODES

 
'\brief 闪光同步信号源
'\param [in]    source
'LineSource_Off: 关闭闪光同步
'LineSource_ExposureActive: 曝光的同时闪光
'LineSource_Timer1Active: 由定时器控制
'LineSource_UserOutput0: 由用户通过指令控制
Public Declare Function MVSetStrobeSource Lib "MVGigE.dll" (ByVal hCam As Long, ByVal source As LineSourceEnums) As MVSTATUS_CODES


'\brief        读取闪光同步是否反转
'\param [out]  Long pInvert
Public Declare Function MVGetStrobeInvert Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pInvert As Long) As MVSTATUS_CODES

 
'\brief 闪光同步是否反转，即闪光同步有效时输出高电平还是低电平。
'\param [in]    bInvert
Public Declare Function MVSetStrobeInvert Lib "MVGigE.dll" (ByVal hCam As Long, ByVal bInvert As Long) As MVSTATUS_CODES


'\brief        读取用户设置的闪光同步
'\param [out]  Long 'vSet
Public Declare Function MVGetUserOutputValue0 Lib "MVGigE.dll" (ByVal hCam As Long, ByRef vSet As Long) As MVSTATUS_CODES

 
'\brief 当闪光同步源选为UserOutput时
'主机可以通过MVSetUserOutputValue0来控制闪光同步输出高电平或低电平?
'\param [in]    bSet 设置电平
Public Declare Function MVSetUserOutputValue0 Lib "MVGigE.dll" (ByVal hCam As Long, ByVal bSet As Long) As MVSTATUS_CODES


'\brief 设置心跳超时时间
'\param [in]    Long nTimeOut  心跳超时时间 单位ms
Public Declare Function MVSetHeartbeatTimeout Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nTimeOut As Long) As MVSTATUS_CODES


'\brief        读取网络数据包大小
'\param [out]  Long *pPacketSize 数据包大小
Public Declare Function MVGetPacketSize Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pPacketSize As Long) As MVSTATUS_CODES


'\brief    读取网络数据包大小的范围。
'\param [out]  Long pMin    网络数据包最小值
'\param [out]  Long pMax 网络数据包最大值
Public Declare Function MVGetPacketSizeRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'\brief 设置网络数据包的大小。
'\param [in]    nPacketSize 网络数据包大小(单位:字节)。该大小必须小于网卡能够支持的最大巨型帧(Jumbo Frame)。
Public Declare Function MVSetPacketSize Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nPacketSize As Long) As MVSTATUS_CODES


'\brief        读取网络数据包间隔。
'\param [out]  Long *pDelay_us 数据包间隔时间，单位us
Public Declare Function MVGetPacketDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDelay_us As Long) As MVSTATUS_CODES


'\brief    读取网络数据包间隔范围
'\param [out]  Long pMin    数据包间隔时间最小值，单位us
'\param [out]  Long pMax 数据包间隔时间最大值，单位us
Public Declare Function MVGetPacketDelayRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'\brief 设置网络数据包之间的时间间隔。如果网卡或电脑的性能欠佳，无法处理高速到达的数据包，会导致丢失数据包，
'从而使图像不完整。可以通过增加数据包之间的时间间隔以保证图像传输。但是增加该值将增加图像的时间延迟，
'并有可能影像到帧率?
'\param [in]    nDelay_us 时间间隔(单位:微秒)
Public Declare Function MVSetPacketDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDelay_us As Long) As MVSTATUS_CODES


'\brief    读取定时器延时
'\param [out]  Long pDelay      定时器延时
Public Declare Function MVGetTimerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDelay As Long) As MVSTATUS_CODES


'\brief        读取定时器延时的范围
'\param [out]  Long pMin        定时器延时的最小值
'\param [out]  Long pMax 定时器延时的最大值
Public Declare Function MVGetTimerDelayRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'设置Timer1在接到触发信号后延迟多少us开始计时
'\param [in]    Long nDelay 接到触发信号后延迟多少us开始计时
Public Declare Function MVSetTimerDelay Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDelay As Long) As MVSTATUS_CODES


'\brief    读取定时器计时时长
'\param [out]  Long pDuration   定时器计时时长
Public Declare Function MVGetTimerDuration Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pDuration As Long) As MVSTATUS_CODES


'\brief    读取定时器计时时长取值范围
'\param [out]  Long pMin        定时器计时时长最小值
'\param [out]  Long pMax        定时器计时时长最大值
Public Declare Function MVGetTimerDurationRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES

 
'设置Timer1在开始计时后，计时多长时间。
'\param [in]    Long nDuration 设置Timer1在开始计时后，计时多长时间(us)。即输出高/低电平的脉冲宽度。
Public Declare Function MVSetTimerDuration Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nDuration As Long) As MVSTATUS_CODES



'\brief 获取当前自动曝光模式
'\param [out]   ExposureAutoEnums pExposureAuto       当前自动曝光模式
Public Declare Function MVGetExposureAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pExposureAuto As ExposureAutoEnums) As MVSTATUS_CODES


'\brief 设置自动曝光模式
'\param [in]    ExposureAutoEnums ExposureAuto  自动曝光模式
Public Declare Function MVSetExposureAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByVal ExposureAuto As ExposureAutoEnums) As MVSTATUS_CODES

        
'\brief 获取当前自动增益模式
'\param [out]   GainAutoEnums pGainAuto       当前自动增益模式的
Public Declare Function MVGetGainAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGainAuto As GainAutoEnums) As MVSTATUS_CODES


'\brief 设置当前自动增益模式
'\param [in]    GainAutoEnums GainAuto  自动增益模式
Public Declare Function MVSetGainAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByVal GainAuto As GainAutoEnums) As MVSTATUS_CODES


'\brief 获取当前自动白平衡模式
'\param [out]   BalanceWhiteAutoEnums pBalanceWhiteAuto       当前自动白平衡模式
Public Declare Function MVGetBalanceWhiteAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pBalanceWhiteAuto As BalanceWhiteAutoEnums) As MVSTATUS_CODES


'\brief 设置自动白平衡模式
'\param [in]    BalanceWhiteAutoEnums BalanceWhiteAuto  自动白平衡模式
Public Declare Function MVSetBalanceWhiteAuto Lib "MVGigE.dll" (ByVal hCam As Long, ByVal BalanceWhiteAuto As BalanceWhiteAutoEnums) As MVSTATUS_CODES


'\brief 获取自动调整增益时，增益调整范围的最小值
'\param [out]   Double pAutoGainLowerLimit    增益调整范围的最小值
Public Declare Function MVGetAutoGainLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoGainLowerLimit As Double) As MVSTATUS_CODES


'\brief 设置自动调整增益时，增益调整范围的最小值
'\param [in]    Double fAutoGainLowerLimit      增益调整范围的最小值
Public Declare Function MVSetAutoGainLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoGainLowerLimit As Double) As MVSTATUS_CODES


'\brief 获取自动调整增益时，增益调整范围的最大值
'\param [out]   Double pAutoGainUpperLimit    增益调整范围的最大值
Public Declare Function MVGetAutoGainUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoGainUpperLimit As Double) As MVSTATUS_CODES


'\brief 设置自动调整增益时，增益调整范围的最大值
'\param [in]    Double fAutoGainUpperLimit      曝光时间调整范围的最小值
Public Declare Function MVSetAutoGainUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoGainUpperLimit As Double) As MVSTATUS_CODES


'\brief 获取自动调整曝光时间时，曝光时间调整范围的最小值
'\param [out]   Double pAutoExposureTimeLowerLimit    曝光时间调整范围的最小值
Public Declare Function MVGetAutoExposureTimeLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoExposureTimeLowerLimit As Double) As MVSTATUS_CODES


'\brief 设置自动调整曝光时间时，曝光时间调整范围的最小值
'\param [in]    Double fAutoExposureTimeLowerLimit      曝光时间调整范围的最大值
Public Declare Function MVSetAutoExposureTimeLowerLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoExposureTimeLowerLimit As Double) As MVSTATUS_CODES


'\brief 获取自动调整曝光时间时，曝光时间调整范围的最大值
'\param [out]   Double pAutoExposureTimeUpperLimit    曝光时间调整范围的最大值
Public Declare Function MVGetAutoExposureTimeUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoExposureTimeUpperLimit As Double) As MVSTATUS_CODES


'\brief 设置自动调整曝光时间时，曝光时间调整范围的最大值
'\param [in]    Double fAutoExposureTimeUpperLimit
Public Declare Function MVSetAutoExposureTimeUpperLimit Lib "MVGigE.dll" (ByVal hCam As Long, ByVal fAutoExposureTimeUpperLimit As Double) As MVSTATUS_CODES


'\brief 获取自动调整亮度(曝光、增益)时，期望调整到的图像亮度
'\param [out]   Long pAutoTargetValue 期望调整到的图像亮度
Public Declare Function MVGetAutoTargetValue Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoTargetValue As Long) As MVSTATUS_CODES


'\brief 设置自动调整亮度(曝光、增益)时，期望调整到的图像亮度
'\param [in]    Long nAutoTargetValue   期望调整到的图像亮度
Public Declare Function MVSetAutoTargetValue Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nAutoTargetValue As Long) As MVSTATUS_CODES


'\brief 当自动增益和自动曝光时间都打开时，获取哪一个值优先调整
'\param [out]   AutoFunctionProfileEnums pAutoFunctionProfile 增益优先或曝光时间优先
Public Declare Function MVGetAutoFunctionProfile Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoFunctionProfile As AutoFunctionProfileEnums) As MVSTATUS_CODES


'\brief 当自动增益和自动曝光时间都打开时，设置哪一个值优先调整
'\param [in]    AutoFunctionProfileEnums AutoFunctionProfile    增益优先或曝光时间优先
Public Declare Function MVSetAutoFunctionProfile Lib "MVGigE.dll" (ByVal hCam As Long, ByVal AutoFunctionProfile As AutoFunctionProfileEnums) As MVSTATUS_CODES


'\brief 自动增益或自动曝光时，图像亮度与目标亮度差异的容差。
'\param [out]   Long pAutoThreshold   图像亮度与目标亮度差异的容差
Public Declare Function MVGetAutoThreshold Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pAutoThreshold As Long) As MVSTATUS_CODES


'\brief 自动增益或自动曝光时，图像亮度与目标亮度差异的容差。
'\param [in]    Long nAutoThreshold     图像亮度与目标亮度差异的容差
Public Declare Function MVSetAutoThreshold Lib "MVGigE.dll" (ByVal hCam As Long, ByVal nAutoThreshold As Long) As MVSTATUS_CODES


'\brief 获取当前伽马值
'\param [out]   Double pGamma 当前伽马值
Public Declare Function MVGetGamma Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGamma As Double) As MVSTATUS_CODES


'\brief 获取伽马值可设置的范围
'\param [out]   Double pGammaMin      伽马最小值
'\param [out]   Double pGammaMax      伽马最大值
Public Declare Function MVGetGammaRange Lib "MVGigE.dll" (ByVal hCam As Long, ByRef pGammaMin As Double, ByRef pGammaMax As Double) As MVSTATUS_CODES

'\brief 设置伽马
'\param [out]   Double fGamma 伽马值
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
