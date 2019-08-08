Imports System
Imports System.Collections.Generic
Imports System.Linq
Imports System.Text
Imports System.Runtime.InteropServices
Namespace MVAPI

    'enum类
    'brief Bayer颜色模式
    Public Enum MV_BAYER_MODE
        BayerRG     '<RG
        BayerBG     '<BG
        BayerGR     '<GR
        BayerGB     '<GB
        BayerInvalid
    End Enum
    '相机像素格式
    Public Enum MV_PixelFormatEnums

        PixelFormat_Mono8 = &H1080001  '!<8Bit灰度
        PixelFormat_BayerBG8 = &H108000B     '!<8Bit Bayer图,颜色模式为BGGR
        PixelFormat_BayerRG8 = &H1080009     '!<8Bit Bayer图,颜色模式为RGGB
        PixelFormat_BayerGB8 = &H108000A     '!<8Bit Bayer图,颜色模式为GBRG
        PixelFormat_BayerGR8 = &H1080008     '!<8Bit Bayer图,颜色模式为GRBG
        PixelFormat_BayerGRW8 = &H108000C    '!<8Bit Bayer图,颜色模式为GRW8
        PixelFormat_Mono16 = &H1100007       '!<16Bit灰度
        PixelFormat_BayerGR16 = &H110002E    '!<16Bit Bayer图,颜色模式为GR
        PixelFormat_BayerRG16 = &H110002F    '!<16Bit Bayer图,颜色模式为RG
        PixelFormat_BayerGB16 = &H1100030    '!<16Bit Bayer图,颜色模式为GB
        PixelFormat_BayerBG16 = &H1100031    '!<16Bit Bayer图,颜色模式为BG  
    End Enum
    '相机触发模式
    Public Enum TriggerSourceEnums

        TriggerSource_Software = 0   '<触发模式下，由软触发(软件指令)来触发采集
        TriggerSource_Line1 = 2      '<触发模式下，有外触发信号来触发采集

    End Enum
    '相机触发模式状态
    Public Enum TriggerModeEnums

        TriggerMode_Off = 0     '<触发模式关，即FreeRun模式，相机连续采集
        TriggerMode_On = 1      '<触发模式开，相机等待软触发或外触发信号再采集图像

    End Enum

    Public Enum TriggerActivationEnums

        TriggerActivation_RisingEdge = 0    '<上升沿触发
        TriggerActivation_FallingEdge = 1   '<下降沿触发

    End Enum
    Public Enum LineSourceEnums

        LineSource_Off = 0                  '<关闭
        LineSource_ExposureActive = 5       '<和曝光同时
        LineSource_Timer1Active = 6         '<由定时器控制
        LineSource_UserOutput0 = 12         '<直接由软件控制

    End Enum
    Public Enum UserSetSelectorEnums

        UserSetSelector_Default = 0  '<出厂设置
        UserSetSelector_UserSet1 = 1 '<用户设置1
        UserSetSelector_UserSet2 = 2 '<用户设置2

    End Enum

    Public Enum AutoFunctionProfileEnums

        AutoFunctionProfile_GainMinimum = 0 '<Keep gain at minimum
        AutoFunctionProfile_ExposureMinimum = 1 '<Exposure Time at minimum

    End Enum

    Public Enum GainAutoEnums
        GainAuto_Off = 0 '<Disables the Gain Auto function.
        GainAuto_Once = 1 '<Sets operation mode to 'once'.
        GainAuto_Continuous = 2 '<Sets operation mode to 'continuous'.
    End Enum

    Public Enum ExposureAutoEnums
        ExposureAuto_Off = 0 '<Disables the Exposure Auto function.
        ExposureAuto_Once = 1 '<Sets operation mode to 'once'.
        ExposureAuto_Continuous = 2 '<Sets operation mode to 'continuous'.
    End Enum

    Public Enum BalanceWhiteAutoEnums
        BalanceWhiteAuto_Off = 0 '<Disables the Balance White Auto function.
        BalanceWhiteAuto_Once = 1 '<Sets operation mode to 'once'.
        BalanceWhiteAuto_Continuous = 2 '<Sets operation mode to 'continuous'.
    End Enum

    Public Enum SensorTapsEnums

        SensorTaps_One  '!<
        SensorTaps_Two  '!<
        SensorTaps_Three '!<
        SensorTaps_Four  '!<

    End Enum
    Public Enum ImageFlipType

        FlipHorizontal = 0  '!< Flip Horizontally (Mirror)
        FlipVertical = 1 '!< Flip Vertically
        FlipBoth = 2 '!< Flip Vertically

    End Enum
    Public Enum ImageRotateType

        Rotate90DegCw = 0       '!< 顺时针旋转90度
        Rotate90DegCcw = 1       '!< 逆时针旋转90度

        '\brief Error return code enumeration. This is returned by all \c Jai_Factory.dll functions
    End Enum
    Public Enum MVSTATUS_CODES

        MVST_SUCCESS = 0      '< OK      
        MVST_ERROR = -1001  '< Generic errorcode
        MVST_ERR_NOT_INITIALIZED = -1002    ''< 没有初始化
        MVST_ERR_NOT_IMPLEMENTED = -1003    ''< 没有实现
        MVST_ERR_RESOURCE_IN_USE = -1004    ''< 资源被占用
        MVST_ACCESS_DENIED = -1005  '< Access denied  无法访问
        MVST_INVALID_HANDLE = -1006  '< Invalid handle 错误句柄
        MVST_INVALID_ID = -1007  '< Invalid ID  错误ID
        MVST_NO_DATA = -1008  '< No data   没有数据
        MVST_INVALID_PARAMETER = -1009  '< Invalid parameter  错误参数
        MVST_FILE_IO = -1010  '< File IO error  IO错误
        MVST_TIMEOUT = -1011  '< Timeout  超时
        MVST_ERR_ABORT = -1012  ' GenTL v1.1  退出
        MVST_INVALID_BUFFER_SIZE = -1013  '< Invalid buffer size 缓冲区尺寸错误
        MVST_ERR_NOT_AVAILABLE = -1014  ' GenTL v1.2  无法访问
        MVST_INVALID_ADDRESS = -1015 ' GenTL v1.3   地址错误

    End Enum
    Public Enum MVCameraRunEnums

        MVCameraRun_ON
        MVCameraRun_OFF

    End Enum
    Public Enum MVShowWindowEnums
        SW_SHOW = 5
        SW_HIDE = 0
    End Enum
    '****************************************************
    '***************************************************
    <StructLayout(LayoutKind.Sequential)>
    Public Structure IMAGE_INFO
        Public nTimeStamp As UInt64    '< 时间戳，采集到图像的时刻，精度为0.01us ;
        Public nBlockId As UShort      '< 帧号，从开始采集开始计数
        Public pImageBuffer As IntPtr  '< 图像指针，即指向(0,0)像素所在内存位置的指针，通过该指针可以访问整个图像;
        Public nImageSizeAcq As ULong  '< 采集到的图像大小[字节];
        Public nMissingPackets As Byte '< 传输过程中丢掉的包数量
        Public nPixelType As UInt64    '< Pixel Format Type
        Public nSizeX As UInt32        '< Image width
        Public nSizeY As UInt32        '< Image height
        Public nOffsetX As UInt32      '< Image offset x
        Public nOffsetY As UInt32      '< Image offset y                            
    End Structure

    <StructLayout(LayoutKind.Sequential)>
    Public Structure MVCamInfo

        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)>
        Public mIpAddr() As Byte
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=6)>
        Public mEthernetAddr() As Byte
        <MarshalAs(UnmanagedType.ByValTStr, SizeConst:=32)>
        Public mMfgName() As String
        <MarshalAs(UnmanagedType.ByValTStr, SizeConst:=32)>
        Public mModelName As String
        <MarshalAs(UnmanagedType.ByValTStr, SizeConst:=16)>
        Public mSerialNumber As String
        <MarshalAs(UnmanagedType.ByValTStr, SizeConst:=16)>
        Public mUserDefinedName As String
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)>
        Public m_IfIp() As Byte
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=6)>
        Public m_IfMAC As Byte
    End Structure

    <StructLayout(LayoutKind.Sequential)>
    Public Structure MVStreamStatistic

        Public m_nTotalBuf As ULong
        Public m_nFailedBuf As ULong
        Public m_nTotalPacket As ULong
        Public m_nFailedPacket As ULong
        Public m_nResendPacketReq As ULong
        Public m_nResendPacket As ULong
    End Structure

    <StructLayout(LayoutKind.Sequential)>
    Public Structure RGBQUAD

        Public R As Byte
        Public G As Byte
        Public B As Byte
        Public res As Byte

    End Structure

    '回调函数声明,创建委托事件，申明代理
    '[System.Runtime.InteropServices.UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    'Delegate Sub myEvnetHandler(ByVal sender As Object, ByVal e As EventArgs)
    Public Delegate Function MV_SNAPPROC(ByRef pInfo As IMAGE_INFO, ByVal UserVal As Intptr) As Integer     '定义委托

    Class MVGigE
        Public Const INT64_MAX As Long = &H7FFFFFFFFFFFFFFF    'maximum signed __int64 value 
        Public Const INT64_MIN As ULong = &H800000000000000    'minimum signed __int64 value ???????
        Public Const UINT64_MAX As ULong = &HFFFFFFFFFFFFFFF   'maximum unsigned __int64 value ??????????

        Public Const INT32_MAX As UInteger = &H7FFFFFFF      '*maximum signed __int32 value 
        Public Const INT32_MIN As UInteger = &H8000000      'minimum signed __int32 value ???????
        Public Const UINT32_MAX As UInteger = &HFFFFFFF     'maximum unsigned __int32 value ?????

        Public Const UINT16_MAX As UShort = &HFFFF        'maximum unsigned __int32 value 

        Public Const INT8_MAX As Byte = &H7F             'maximum signed __int8 value 
        Public Const INT8_MIN As Byte = &H80          'minimum signed __int8 value 
        Public Const UINT8_MAX As Byte = &HFF      'maximum unsigned __int8 value 

        <DllImport("MVGigE.dll")>
        Public Shared Function MVInitLib() As MVSTATUS_CODES
        End Function
        <DllImport("MVGigE.dll")>
        Public Shared Function MVTerminateLib() As MVSTATUS_CODES
        End Function


        '<summary>
        ' 查找连接到计算机上的相机
        '</summary>
        '<returns></returns>
        <DllImport("MVGigE.dll")>
        Public Shared Function MVUpdateCameraList() As MVSTATUS_CODES
        End Function

        '<summary>
        '获取连接到计算机上的相机的数量
        '</summary>
        '<param name="pNumCams">param [out]	pNumCams	相机数量</param>
        '<returns>MVST_SUCCESS	: 成功</returns>
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetNumOfCameras(ByRef pNumCams As Long) As MVSTATUS_CODES
        End Function

        '<summary>
        '得到第idx个相机的信息
        '</summary>
        '<param name="idx">[in]	idx idx从0开始，按照相机的IP地址排序，地址小的排在前面。</param>
        '<param name="pCamInfo">pCamInfo [out]  相机的信息 (IP,MAC,SN,型号...)</param>
        '<returns>	MVST_SUCCESS	: 成功</returns>
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetCameraInfo(ByVal idx As Byte, ByRef pCamInfo As MVCamInfo) As MVSTATUS_CODES
        End Function


        '<summary>
        '打开第idx个相机
        '</summary>
        '<param name="idx">[in]	idx	idx从0开始，按照相机的IP地址排序，地址小的排在前面</param>
        '<param name="hCam">[out]	hCam 如果成功,返回的相机句柄</param>
        '<returns> MVST_INVALID_PARAMETER : idx取值不对 
        'MVST_ACCESS_DENIED: 相机无法访问，可能正被别的软件控制 
        ' MVST_ERROR: 其他错误 
        'MVST_SUCCESS	: 成功</returns>
        <DllImport("MVGigE.dll")>
        Public Shared Function MVOpenCamByIndex(ByVal idx As Byte, ByRef hCam As IntPtr) As MVSTATUS_CODES
        End Function

        'brief	打开指定UserDefinedName的相机
        'param [in]	name UserDefinedName。
        'param [out]	hCam 如果成功,返回的相机句柄
        ' retval 
        '		MVST_ACCESS_DENIED		: 相机无法访问，可能正被别的软件控制
        '		MVST_ERROR				: 其他错误
        '		MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVOpenCamByUserDefinedName(ByVal name As String, ByRef hCam As IntPtr) As MVSTATUS_CODES
        End Function

        'brief	打开指定IP的相机
        'param [in]	ip 相机的IP地址。
        'param [out] hCam 如果成功,返回的相机句柄。如果失败，为NULL。
        'retval 
        '			MVST_ACCESS_DENIED		: 相机无法访问，可能正被别的软件控制
        '			MVST_ERROR				: 其他错误
        '			MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVOpenCamByIP(ByVal ip As String, ByRef hCam As IntPtr) As MVSTATUS_CODES
        End Function

        ' brief	关闭相机。断开和相机的连接。
        ' param [in]	hCam 相机的句柄
        ' retval 
        <DllImport("MVGigE.dll")>
        Public Shared Function MVCloseCam(ByVal hCam As IntPtr) As MVSTATUS_CODES
        End Function

        '\brief        读取图像宽度
        '\param [out]  Long pWidth   图像宽度[像素]
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetWidth(ByVal hCam As IntPtr, ByRef width As Integer) As MVSTATUS_CODES
        End Function

        '\brief        读取图像高度
        '\param [out]  Long pHeight  图像高度[像素]
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetHeight(ByVal hCam As IntPtr, ByRef height As Integer) As MVSTATUS_CODES
        End Function

        '\brief    读取图像的像素格式
        '\param [out]  MV_PixelFormatEnums pPixelFormat
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetPixelFormat(ByVal hCam As IntPtr, ByRef pPixelFormat As MV_PixelFormatEnums) As MVSTATUS_CODES
        End Function

        'brief	读取传感器的通道数
        '\param [in]	HANDLE hCam		相机句柄
        'param [out] SensorTapsEnums* pSensorTaps
        'retval  	MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetSensorTaps(ByVal hCam As IntPtr, ByRef pSensorTaps As SensorTapsEnums) As MVSTATUS_CODES
        End Function

        '\brief    读取当前增益值
        '\param [out]  Double pGain
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGain(ByVal hCam As IntPtr, ByRef pGain As Double) As MVSTATUS_CODES
        End Function

        '\brief    读取增益可以设置的范围
        '\param [out]  Double pGainMin       最小值
        '\param [out]  Double pGainMax       最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGainRange(ByVal hCam As IntPtr, ByRef pGainMin As Double, ByRef pGainMax As Double) As MVSTATUS_CODES
        End Function

        '\brief    设置增益
        '\param [in]   Double fGain    增益
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetGain(ByVal hCam As IntPtr, ByVal fGain As Double) As MVSTATUS_CODES
        End Function

        'brief    当相机传感器为多通道时，设置某个通道的增益
        'param [in]	HANDLE hCam		相机句柄
        'param [in]	double fGain	增益
        'param [in]	int nTap		通道。双通道[0,1],四通道[0,1,2,3]
        'retval MVC_ST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetGainTaps(ByVal hCam As IntPtr, ByVal fGain As Double, ByVal nTap As Integer) As MVSTATUS_CODES
        End Function

        'brief    当相机传感器为多通道时，读取某个通道的增益
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	double* pGain
        'param [in]	int nTap		通道。双通道[0,1],四通道[0,1,2,3]
        'retval MVC_ST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGainTaps(ByVal hCam As Integer, ByRef pGain As Double, ByVal nTap As Integer) As MVSTATUS_CODES
        End Function

        'brief    当相机传感器为多通道时，读取某个通道的增益可设置的范围
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	double * pGainMin	增益最小值
        'param [out]	double * pGainMax	增益最大值
        'param [in]	int nTap		通道。双通道[0,1],四通道[0,1,2,3]
        'retval MVC_ST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGainRangeTaps(ByVal hCam As IntPtr, ByRef pGainMin As Double, ByRef pGainMax As Double, ByVal nTap As Integer) As MVSTATUS_CODES
        End Function

        '\brief    读取当前白平衡系数
        '\param [out]  Double pRed   红色平衡系数
        '\param [out]  Double pGreen 绿色平衡系数
        '\param [out]  Double pBlue  蓝色平衡系数
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetWhiteBalance(ByVal hCam As IntPtr, ByRef pRed As Double, ByRef pGreen As Double, ByRef pBlue As Double) As MVSTATUS_CODES
        End Function

        '\brief        读取白平衡设置的范围
        '\param [out]  Double pMin   系数最小值
        '\param [out]  Double pMax   系数最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetWhiteBalanceRange(ByVal hCam As IntPtr, ByRef pMin As Double, ByRef pMax As Double) As MVSTATUS_CODES
        End Function


        '\brief    设置白平衡系数
        '\param [in]   Double fRed             红色平衡系数
        '\param [in]   Double fGreen   绿色平衡系数
        '\param [in]   Double fBlue    蓝色平衡系数
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetWhiteBalance(ByVal hCam As IntPtr, ByRef fRed As Double, ByRef fGreen As Double, ByRef fBlue As Double) As MVSTATUS_CODES
        End Function

        'brief    读取是否通道自动平衡
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	int* pBalance	是否自动平衡
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGainBalance(ByVal hCam As IntPtr, ByVal pBalance As Integer) As MVSTATUS_CODES
        End Function

        '\brief 读取当前曝光时间
        '\param [in]    pExposuretime   单位us
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetExposureTime(ByVal hCam As IntPtr, ByRef pExposuretime As Double) As MVSTATUS_CODES
        End Function


        '\brief  读取曝光时间的设置范围
        '\param [out]  Double pExpMin        最短曝光时间 单位为us
        '\param [out]  Double pExpMax        最长曝光时间 单位为us
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetExposureTimeRange(ByVal hCam As IntPtr, ByRef pExpMin As Double, ByRef pExpMax As Double) As MVSTATUS_CODES
        End Function


        '\brief        设置曝光时间
        '\param [in]   Long nExp_us 曝光时间 单位为us
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetExposureTime(ByVal hCam As IntPtr, ByVal nExp_us As Double) As MVSTATUS_CODES
        End Function


        '\brief        读取帧率可设置的范围
        '\param [out]  DoublepFpsMin 最低帧率
        '\param [out]  DoublepFpsMax 最高帧率
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetFrameRateRange(ByVal hCam As IntPtr, ByRef pFpsMin As Double, ByRef pFpsMax As Double) As MVSTATUS_CODES
        End Function


        '\brief        读取当前帧率
        '\param [out]  Double 'fFPS   帧率 帧/秒
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetFrameRate(ByVal hCam As IntPtr, ByRef fFPS As Double) As MVSTATUS_CODES
        End Function


        '\brief    设置帧率
        '\param [in]   Double fps      帧率 帧/秒
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetFrameRate(ByVal hCam As IntPtr, ByVal fps As Double) As MVSTATUS_CODES
        End Function

        '\brief 开始采集图像
        '\param [in]	hCam	相机句柄
        '\param [in] StreamCB	回调函数指针
        '\param [in]	nUserVal	用户数据，传递到回调函数的形参
        '\retval  	MVST_SUCCESS			: 成功      
        <DllImport("MVGigE.dll")>
        Public Shared Function MVStartGrab(ByVal hCam As IntPtr, ByVal callbackFunc As MV_SNAPPROC, ByVal nUserVal As IntPtr) As MVSTATUS_CODES
        End Function

        '\brief 采集一帧图像
        '\param [in]    hCam 相机句柄
        '\param [out]   hImage 图像句柄
        '\param [in]    nWaitMs 等待图像采集的事件，单位为毫秒
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSingleGrab(ByVal hCam As IntPtr, ByVal hImage As IntPtr, ByVal nWaitMs As Long) As MVSTATUS_CODES
        End Function

        <DllImport("MVGigE.dll")>
        Public Shared Function MVStopGrab(ByVal hCam As IntPtr) As MVSTATUS_CODES
        End Function


        '********************************************
        '\brief    读取触发模式
        '\param [out]  TriggerModeEnums pMode        触发模式  TriggerMode_Off,TriggerMode_On
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTriggerMode(ByVal hCam As IntPtr, ByRef pMode As TriggerModeEnums) As MVSTATUS_CODES
        End Function


        '\brief 设置触发模式
        '\param [in]    mode    触发模式
        'TriggerMode_Off：相机工作在连续采集模式，
        'TriggerMode_On:相机工作在触发模式，需要有外触发信号或软触发指令才拍摄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetTriggerMode(ByVal hCam As IntPtr, ByVal mode As TriggerModeEnums) As MVSTATUS_CODES
        End Function


        '\brief    读取触发源
        '\param [out]  TriggerSourceEnums pSource    触发源，软触发或外触发
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTriggerSource(ByVal hCam As IntPtr, ByRef pSource As TriggerSourceEnums) As MVSTATUS_CODES
        End Function


        '\brief 设置触发源
        '\param [in]    TriggerSourceEnums      source 触发源
        'TriggerSource_Software：通过\c MVTriggerSoftware()函数触发。
        'TriggerSource_Line1: 通过连接的触发线触发?
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetTriggerSource(ByVal hCam As IntPtr, ByVal Source As TriggerSourceEnums) As MVSTATUS_CODES
        End Function

        '\brief    读取触发极性
        '\param [out]  TriggerActivationEnums pAct
        'TriggerActivation_RisingEdge: 上升沿触发
        'TriggerActivation_FallingEdge: 下降沿触发
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTriggerActivation(ByVal hCam As IntPtr, ByRef pAct As TriggerActivationEnums) As MVSTATUS_CODES
        End Function


        '\brief 当使用触发线触发时,设置是上升沿触发还是下降沿触发
        '\param [in]    act 上升沿或下降沿
        'TriggerActivation_RisingEdge: 上升沿触发
        'TriggerActivation_FallingEdge: 下降沿触发
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetTriggerActivation(ByVal hCam As IntPtr, ByVal act As TriggerActivationEnums) As MVSTATUS_CODES
        End Function


        '\brief        读取触发延时
        '\param [out]  Long pDelay_us   触发延时,单位us
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTriggerDelay(ByVal hCam As IntPtr, ByRef pDelay_us As UInteger) As MVSTATUS_CODES
        End Function


        'brief    读取触发延时范围
        'param [out]  Long pMin        触发延时最小值,单位us
        'param [out]  Long pMax 触发延时最大值,单位us
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTriggerDelayRange(ByVal hCam As IntPtr, ByRef pMin As UInteger, ByRef pMax As UInteger) As MVSTATUS_CODES
        End Function


        'brief 设置相机接到触发信号后延迟多少微秒后再开始曝光。
        'param [in]	HANDLE hCam		相机句柄
        'param [in]	nDelay_us
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetTriggerDelay(ByVal hCam As IntPtr, ByVal nDelay_us As UInteger) As MVSTATUS_CODES
        End Function


        '\brief 发出软件触发指令
        'param [in]	HANDLE hCam		相机句柄
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVTriggerSoftware(ByVal hCam As IntPtr) As MVSTATUS_CODES
        End Function


        '\brief        读取闪光同步信号源
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	LineSourceEnums * pSource	闪光同步信号源
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetStrobeSource(ByVal hCam As IntPtr, ByRef pSource As LineSourceEnums) As MVSTATUS_CODES
        End Function

        'brief	闪光同步信号源
        'param [in]	hCam
        'param [in]	source
        '     LineSource_Off：关闭闪光同步
        '	        LineSource_ExposureActive：曝光的同时闪光
        '	        LineSource_Timer1Active：由定时器控制
        '	        LineSource_UserOutput0：由用户通过指令控制
        'retval 
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetStrobeSource(ByVal hCam As IntPtr, ByVal bSouce As LineSourceEnums) As MVSTATUS_CODES
        End Function

        '\brief        读取闪光同步是否反转
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	BOOL * pInvert
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetStrobeInvert(ByVal hCam As IntPtr, ByRef pInvert As Boolean) As MVSTATUS_CODES
        End Function

        '\brief 闪光同步是否反转，即闪光同步有效时输出高电平还是低电平。
        '\param [in]    bInve '\brief        读取闪光同步是否反转
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	BOOL * bInvert
        'retval MVST_SUCCESS			: 成功rt
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetStrobeInvert(ByVal hCam As IntPtr, ByVal bInvert As Boolean) As MVSTATUS_CODES
        End Function

        'brief        读取用户设置的闪光同步
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	BOOL * pSet
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetUserOutputValue0(ByVal hCam As IntPtr, ByRef pSet As Boolean) As MVSTATUS_CODES
        End Function


        '\brief 当闪光同步源选为UserOutput时
        '主机可以通过MVSetUserOutputValue0来控制闪光同步输出高电平或低电平?
        'param [in]	HANDLE hCam		相机句柄
        'param [in]	bSet 设置电平
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetUserOutputValue0(ByVal hCam As IntPtr, ByVal bSet As Boolean) As MVSTATUS_CODES
        End Function


        '\brief 设置心跳超时时间
        '\param [in]    Long nTimeOut  心跳超时时间 单位ms
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetHeartbeatTimeout(ByVal hCam As IntPtr, ByVal nTimeOut As ULong) As MVSTATUS_CODES
        End Function


        'brief        读取网络数据包大小
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	int *pPacketSize 数据包大小
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetPacketSize(ByVal hCam As IntPtr, ByRef pPacketSize As UInteger) As MVSTATUS_CODES
        End Function


        '\brief    读取网络数据包大小的范围。
        'param [in]	HANDLE hCam			相机句柄
        'param [out]	unsigned int * pMin	网络数据包最小值	
        'param [out]	unsigned int * pMax 网络数据包最大值	
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetPacketSizeRange(ByVal hCam As IntPtr, ByRef pMin As UInteger, ByRef pMax As UInteger) As MVSTATUS_CODES
        End Function


        '\brief 设置网络数据包的大小。
        '\param [in]    nPacketSize 网络数据包大小(单位:字节)。该大小必须小于网卡能够支持的最大巨型帧(Jumbo Frame)。
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetPacketSize(ByVal hCam As IntPtr, ByVal nPacketSize As UInteger) As MVSTATUS_CODES
        End Function


        '\brief        读取网络数据包间隔。
        'param [in]	HANDLE hCam				相机句柄
        'param [out]	unsigned int *pDelay_us 数据包间隔时间，单位us
        'retval MVST_SUCCESS				: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetPacketDelay(ByVal hCam As IntPtr, ByRef pDelay_us As UInteger) As MVSTATUS_CODES
        End Function


        '\brief    读取网络数据包间隔范围
        'param [in]	HANDLE hCam			相机句柄
        'param [out]	unsigned int * pMin	数据包间隔时间最小值，单位us	
        'param [out]	unsigned int * pMax 数据包间隔时间最大值，单位us	
        'retval MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetPacketDelayRange(ByVal hCam As IntPtr, ByRef pMin As UInteger, ByRef pMax As UInteger) As MVSTATUS_CODES
        End Function


        '\brief 设置网络数据包之间的时间间隔。如果网卡或电脑的性能欠佳，无法处理高速到达的数据包，会导致丢失数据包，
        '从而使图像不完整。可以通过增加数据包之间的时间间隔以保证图像传输。但是增加该值将增加图像的时间延迟，
        '并有可能影像到帧率?
        '\param [in]    nDelay_us 时间间隔(单位:微秒)
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetPacketDelay(ByVal hCam As IntPtr, ByVal nDelay_us As UInteger) As MVSTATUS_CODES
        End Function

        '\brief    读取定时器延时
        '\param [out]  Long pDelay      定时器延时
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTimerDelay(ByVal hCam As IntPtr, ByRef pDelay As UInteger) As MVSTATUS_CODES
        End Function

        '\brief        读取定时器延时的范围
        '\param [out]  Long pMin        定时器延时的最小值
        '\param [out]  Long pMax 定时器延时的最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTimerDelayRange(ByVal hCam As IntPtr, ByRef pMin As Long, ByRef pMax As Long) As MVSTATUS_CODES
        End Function


        '设置Timer1在接到触发信号后延迟多少us开始计时
        '\param [in]    Long nDelay 接到触发信号后延迟多少us开始计时
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetTimerDelay(ByVal hCam As IntPtr, ByVal nDelay As UInteger) As MVSTATUS_CODES
        End Function


        '\brief    读取定时器计时时长
        '\param [out]  Long pDuration   定时器计时时长
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTimerDuration(ByVal hCam As IntPtr, ByRef pDuration As UInteger) As MVSTATUS_CODES
        End Function


        '\brief    读取定时器计时时长取值范围
        '\param [out]  Long pMin        定时器计时时长最小值
        '\param [out]  Long pMax        定时器计时时长最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetTimerDurationRange(ByVal hCam As IntPtr, ByRef pMin As UInteger, ByRef pMax As UInteger) As MVSTATUS_CODES
        End Function


        '设置Timer1在开始计时后，计时多长时间。
        '\param [in]    Long nDuration 设置Timer1在开始计时后，计时多长时间(us)。即输出高/低电平的脉冲宽度。
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetTimerDuration(ByVal hCam As IntPtr, ByVal nDuration As UInteger) As MVSTATUS_CODES
        End Function


        ' <summary>
        ' Bayer格式图像数据到RGB格式图像数据转换 ( 转成RGB24位)
        ' </summary>
        ' <param name="hCam">该相机句柄</param>
        ' <param name="psrc">原始图像数据指针</param>
        ' <param name="pdst">转换后图像存储内存指针</param>
        ' <param name="dststep">转换步长</param>
        ' <param name="width">图像宽度</param>
        ' <param name="height">图像高度</param>
        ' <param name="pixelformat">像素格式</param>
        ' <returns>成功返回Success,否则返回错误信息</returns>
        <DllImport("MVGigE.dll")>
        Public Shared Function MVBayerToBGR(ByVal hCam As IntPtr, ByVal psrc As IntPtr, ByVal pdst As IntPtr, ByVal dststep As UInteger, ByVal width As UInteger, ByVal height As UInteger, ByVal pixelformat As MV_PixelFormatEnums) As MVSTATUS_CODES
        End Function

        <DllImport("MVGigE.dll")>
        Public Shared Function MVInfo2Image(ByVal hCam As IntPtr, ByRef pInfo As IMAGE_INFO, ByVal pImage As IntPtr) As MVSTATUS_CODES
        End Function

        ' <summary>
        ' <summary>
        ' 图像缩放
        ' </summary>
        ' <param name="hCam">该相机句柄</param>
        ' <param name="pSrc">原始图像指针</param>
        ' <param name="srcWidth">原始图像宽度</param>
        ' <param name="srcHeight">原始图像高度</param>
        ' <param name="pDst">缩放后存放图像内存指针</param>
        ' <param name="fFactorX">水平方向缩放因子</param>
        ' <param name="fFactorY">垂直方向缩放因子</param>
        ' <returns>成功返回Success,否则返回错误信息</returns>
        <DllImport("MVGigE.dll")>
        Public Shared Function MVZoomImageBGR(ByVal hCam As IntPtr, ByVal pSrc As IntPtr, ByVal srcWidth As Integer, ByVal srcHeight As Integer, ByRef pDst As IntPtr, ByVal fFactorX As Double, ByVal fFactorY As Double) As MVSTATUS_CODES
        End Function

        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetStreamStatistic(ByVal hCam As IntPtr, ByVal pStatistic As MVStreamStatistic) As MVSTATUS_CODES
        End Function

        '  \brief	读取并应用某组用户预设的参数
        ' \param [in]	HANDLE hCam		相机句柄
        ' \param [in]	UserSetSelectorEnums userset
        ' \retval  	MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVLoadUserSet(ByVal hCam As IntPtr, ByVal userset As UserSetSelectorEnums) As MVSTATUS_CODES
        End Function

        'brief	将当前相机的参数保存到用户设置中
        'param [in]	HANDLE hCam		相机句柄
        '\param [in]	UserSetSelectorEnums userset
        'retval  	MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSaveUserSet(ByVal hCam As IntPtr, ByVal userset As UserSetSelectorEnums) As MVSTATUS_CODES
        End Function

        'brief	设置相机上电开机时默认读取并应用哪一组用户设置
        'param [in]	HANDLE hCam		相机句柄
        'param [in]	UserSetSelectorEnums userset
        'retval  	MVST_SUCCESS			: 成功
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetDefaultUserSet(ByVal hCam As IntPtr, ByVal userset As UserSetSelectorEnums) As MVSTATUS_CODES
        End Function

        'brief	读取相机上电开机时默认读取并应用哪一组用户设置
        'param [in]	HANDLE hCam		相机句柄
        'param [out]	UserSetSelectorEnums* pUserset	用户设置
        'retval  	MVGIGE_API MVSTATUS_CODES __stdcall
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetDefaultUserSet(ByVal hCam As IntPtr, ByRef puserset As UserSetSelectorEnums) As MVSTATUS_CODES
        End Function


        '\brief 获取当前自动曝光模式
        '\param [out]   ExposureAutoEnums pExposureAuto       当前自动曝光模式
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetExposureAuto(ByVal hCam As IntPtr, ByRef pExposureAuto As ExposureAutoEnums) As MVSTATUS_CODES
        End Function


        '\brief 设置自动曝光模式
        '\param [in]    ExposureAutoEnums ExposureAuto  自动曝光模式
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetExposureAuto(ByVal hCam As IntPtr, ByVal ExposureAuto As ExposureAutoEnums) As MVSTATUS_CODES
        End Function


        '\brief 获取当前自动增益模式
        '\param [out]   GainAutoEnums pGainAuto       当前自动增益模式的
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGainAuto(ByVal hCam As IntPtr, ByRef pGainAuto As GainAutoEnums) As MVSTATUS_CODES
        End Function


        '\brief 设置当前自动增益模式
        '\param [in]    GainAutoEnums GainAuto  自动增益模式
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetGainAuto(ByVal hCam As IntPtr, ByVal GainAuto As GainAutoEnums) As MVSTATUS_CODES
        End Function


        '\brief 获取当前自动白平衡模式
        '\param [out]   BalanceWhiteAutoEnums pBalanceWhiteAuto       当前自动白平衡模式

        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetBalanceWhiteAuto(ByVal hCam As IntPtr, ByRef pBalanceWhiteAuto As BalanceWhiteAutoEnums) As MVSTATUS_CODES
        End Function


        '\brief 设置自动白平衡模式
        '\param [in]    BalanceWhiteAutoEnums BalanceWhiteAuto  自动白平衡模式
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetBalanceWhiteAuto(ByVal hCam As IntPtr, ByVal BalanceWhiteAuto As BalanceWhiteAutoEnums) As MVSTATUS_CODES
        End Function


        '\brief 获取自动调整增益时，增益调整范围的最小值
        '\param [out]   Double pAutoGainLowerLimit    增益调整范围的最小值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoGainLowerLimit(ByVal hCam As IntPtr, ByRef pAutoGainLowerLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 设置自动调整增益时，增益调整范围的最小值
        '\param [in]    Double fAutoGainLowerLimit      增益调整范围的最小值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoGainLowerLimit(ByVal hCam As IntPtr, ByVal fAutoGainLowerLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 获取自动调整增益时，增益调整范围的最大值
        '\param [out]   Double pAutoGainUpperLimit    增益调整范围的最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoGainUpperLimit(ByVal hCam As IntPtr, ByRef pAutoGainUpperLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 设置自动调整增益时，增益调整范围的最大值
        '\param [in]    Double fAutoGainUpperLimit      曝光时间调整范围的最小值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoGainUpperLimit(ByVal hCam As IntPtr, ByVal fAutoGainUpperLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 获取自动调整曝光时间时，曝光时间调整范围的最小值
        '\param [out]   Double pAutoExposureTimeLowerLimit    曝光时间调整范围的最小值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoExposureTimeLowerLimit(ByVal hCam As IntPtr, ByRef pAutoExposureTimeLowerLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 设置自动调整曝光时间时，曝光时间调整范围的最小值
        '\param [in]    Double fAutoExposureTimeLowerLimit      曝光时间调整范围的最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoExposureTimeLowerLimit(ByVal hCam As IntPtr, ByVal fAutoExposureTimeLowerLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 获取自动调整曝光时间时，曝光时间调整范围的最大值
        '\param [out]   Double pAutoExposureTimeUpperLimit    曝光时间调整范围的最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoExposureTimeUpperLimit(ByVal hCam As IntPtr, ByRef pAutoExposureTimeUpperLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 设置自动调整曝光时间时，曝光时间调整范围的最大值
        '\param [in]    Double fAutoExposureTimeUpperLimit
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoExposureTimeUpperLimit(ByVal hCam As IntPtr, ByVal fAutoExposureTimeUpperLimit As Double) As MVSTATUS_CODES
        End Function


        '\brief 获取自动调整亮度(曝光、增益)时，期望调整到的图像亮度
        '\param [out]   Long pAutoTargetValue 期望调整到的图像亮度
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoTargetValue(ByVal hCam As IntPtr, ByRef pAutoTargetValue As Long) As MVSTATUS_CODES
        End Function


        '\brief 设置自动调整亮度(曝光、增益)时，期望调整到的图像亮度
        '\param [in]    Long nAutoTargetValue   期望调整到的图像亮度
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoTargetValue(ByVal hCam As IntPtr, ByVal nAutoTargetValue As Long) As MVSTATUS_CODES
        End Function


        '\brief 当自动增益和自动曝光时间都打开时，获取哪一个值优先调整
        '\param [out]   AutoFunctionProfileEnums pAutoFunctionProfile 增益优先或曝光时间优先
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoFunctionProfile(ByVal hCam As IntPtr, ByRef pAutoFunctionProfile As AutoFunctionProfileEnums) As MVSTATUS_CODES
        End Function


        '\brief 当自动增益和自动曝光时间都打开时，设置哪一个值优先调整
        '\param [in]    AutoFunctionProfileEnums AutoFunctionProfile    增益优先或曝光时间优先
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoFunctionProfile(ByVal hCam As IntPtr, ByVal AutoFunctionProfile As AutoFunctionProfileEnums) As MVSTATUS_CODES
        End Function


        '\brief 自动增益或自动曝光时，图像亮度与目标亮度差异的容差。
        '\param [out]   Long pAutoThreshold   图像亮度与目标亮度差异的容差
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetAutoThreshold(ByVal hCam As IntPtr, ByRef pAutoThreshold As Long) As MVSTATUS_CODES
        End Function


        '\brief 自动增益或自动曝光时，图像亮度与目标亮度差异的容差。
        '\param [in]    Long nAutoThreshold     图像亮度与目标亮度差异的容差
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetAutoThreshold(ByVal hCam As IntPtr, ByVal nAutoThreshold As Long) As MVSTATUS_CODES
        End Function


        '\brief 获取当前伽马值
        '\param [out]   Double pGamma 当前伽马值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGamma(ByVal hCam As IntPtr, ByRef pGamma As Double) As MVSTATUS_CODES
        End Function


        '\brief 获取伽马值可设置的范围
        '\param [out]   Double pGammaMin      伽马最小值
        '\param [out]   Double pGammaMax      伽马最大值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetGammaRange(ByVal hCam As IntPtr, ByRef pGammaMin As Double, ByRef pGammaMax As Double) As MVSTATUS_CODES
        End Function

        '\brief 设置伽马
        '\param [out]   Double fGamma 伽马值
        <DllImport("MVGigE.dll")>
        Public Shared Function MVSetGamma(ByVal hCam As IntPtr, ByVal fGamma As Double) As MVSTATUS_CODES
        End Function

        '  \brief 获取设备的型号
        '  \param [in]  hCam    相机句柄
        '  \param [out] pBuf 用于保存型号的缓冲区，大于等于32字节
        '  \param [in,out]      szBuf 缓冲区大小
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetDeviceModelName(ByVal hCam As IntPtr, ByRef fBuf As Byte, ByVal nSize As Long) As MVSTATUS_CODES
        End Function

        '  \brief 获取设备的ID号，即序列号
        '  \param [in]  hCam    相机句柄
        '  \param [out] pBuf    用于保存序列号的缓冲区，大于等于16字节
        '  \param [in,out]      szBuf   缓冲区大小
        <DllImport("MVGigE.dll")>
        Public Shared Function MVGetDeviceDeviceID(ByVal hCam As IntPtr, ByRef fBuf As Byte, ByVal nSize As Long) As MVSTATUS_CODES
        End Function

        '  \brief 图像翻转
        '  \param [in] hCam    相机句柄
        '  \param [in]   pSrcImage      源图像指针
        '  \param [out] pDstImage       结果图像指针。如果为NULL，则翻转的结果还在源图像内。
        '  \param [in]  flipType        翻转类型。FlipHorizontal:左右翻转,FlipVertical:上下翻转,FlipBoth:旋转180度
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageFlip(ByVal hCam As IntPtr, ByVal pSrcImage As IntPtr, ByVal pDstImage As IntPtr, ByVal flipType As ImageFlipType) As MVSTATUS_CODES
        End Function


        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageRotate(ByVal hCam As IntPtr, ByVal pSrcImage As IntPtr, ByVal pDstImage As IntPtr, ByVal roateType As ImageFlipType) As MVSTATUS_CODES
        End Function

        <DllImport("kernel32.dll")>
        Public Shared Sub CopyMemory(ByVal dest As IntPtr, ByVal src As IntPtr, ByVal count As UInteger)
        End Sub
    End Class

    Class MVImage
        '  \brief 创建图像
        '  \param [in]  nWidth  图像宽度
        '  \param [in]  nHeight 图像高度
        '  \param [in]  nBPP    每像素Bit数。取值范围为8(8bit灰度),16(16bit灰度),24(8bit彩色),48(16bit彩色)
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageCreate(ByVal width As Integer, ByVal height As Integer, ByVal bpp As Integer) As IntPtr
        End Function

        '  \brief 判断图像是否为空。在调用Create或CreateByPixelFormat之前，图像为空
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageIsNull(ByVal hImage As IntPtr) As Boolean
        End Function

        '  \brief 获取图像宽度，单位为像素
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageGetWidth(ByVal hImage As IntPtr) As Integer
        End Function

        '  \brief 获取图像高度，单位为像素
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageGetHeight(ByVal hImage As IntPtr) As Integer
        End Function

        '  \brief 获取一行图像的字节数
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageGetPitch(ByVal hImage As IntPtr) As Integer
        End Function

        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageGetBPP(ByVal hImage As IntPtr) As Integer
        End Function

        '  \brief 获取图像缓冲区的指针
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageGetBits(ByVal hImage As IntPtr) As IntPtr
        End Function

        '  \brief 在目标DC上的指定位置1:1绘制本图像
        '  \param [in]  hImage  相机句柄
        '  \param [in]  hDestDC 目标DC
        '  \param [in]  xDest   图像绘制的起始水平坐标
        '  \param [in]  yDest   图像绘制的起始垂直坐标
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageDraw(ByVal hImage As IntPtr, ByRef hDestDC As IntPtr, ByVal xDest As Integer, ByVal yDest As Integer) As Long
        End Function

        '  \brief 在目标DC上的指定区域绘制本图像的全部或局部
        '  \param [in]  hImage  相机句柄
        '  \param [in]  hDestDC 目标DC的句柄
        '  \param [in]  xDest   x坐标，目标矩形的左上角。
        '  \param [in]  yDest   y坐标，目标矩形的左上角。
        '  \param [in]  nDestWidth      宽度，目标矩形。
        '  \param [in]  nDestHeight     高度，目标矩形。
        '  \param [in]  xSrc    x坐标，源矩形的左上角。
        '  \param [in]  ySrc    y坐标，源矩形的左上角。
        '  \param [in]  nSrcWidth       宽度，源矩形。
        '  \param [in]  nSrcHeight      高度，源矩形。
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageDrawEx(ByVal hImage As IntPtr, ByVal hDestDC As IntPtr, ByVal xDest As Integer, ByVal yDest As Integer, ByVal nDestWidth As Integer, ByVal nDestHeight As Integer, ByVal xSrc As Integer, ByVal ySrc As Integer, ByVal nSrcWidth As Integer, ByVal nSrcHeight As Integer) As Long
        End Function

        '  \brief 在目标窗体上的指定位置1:1绘制本图像
        '  \param [in]  hImage  相机句柄
        '  \param [in]  hWnd    目标窗体
        '  \param [in]  xDest   图像绘制的起始水平坐标
        '  \param [in]  yDest   图像绘制的起始垂直坐标
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageDrawHwnd(ByVal hImage As IntPtr, ByVal HWND As IntPtr, ByVal xDest As Integer, ByVal yDest As Integer) As Long
        End Function

        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageDrawHwndEx(ByVal hImage As IntPtr, ByVal HWND As IntPtr, ByVal xDest As Integer, ByVal yDest As Integer) As Long
        End Function

        '  \brief 获取图像的DC，可用于在图像上绘制图形。
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageGetDC(ByVal hImage As IntPtr) As IntPtr
        End Function

        '  \brief 释放使用GetDC获取的DC
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Function MVImageReleaseDC(ByVal hImage As IntPtr) As IntPtr
        End Function

        '  \brief 释放图像资源
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Sub MVImageRelease(ByVal hImage As IntPtr)
        End Sub

        '  \brief 释放图像资源
        '  \param [in]  hImage  相机句柄
        <DllImport("MVGigE.dll")>
        Public Shared Sub MVImageDestroy(ByVal hImage As IntPtr)
        End Sub

        '  \brief 保存图像
        '  \param [in]  hImage  相机句柄
        '  \param [in]  pszFileName     文件名。文件类型可以为.bmp,.jpg,.tif,.raw(仅支持位深为16位的图像)
        <DllImport("MVGigE.dll")>
        Public Shared Sub MVImageSave(ByVal hImage As IntPtr, ByVal fname As String)
        End Sub
    End Class

    Class MVCamProptySheet

        Public Const PAGE_NONE As Integer = &H0
        Public Const PAGE_ALL As Integer = &HFFFF
        Public Const PAGE_ACQUISITION As Integer = &H1
        Public Const PAGE_WHITE_BALANCE As Integer = &H2
        Public Const PAGE_TRANS_LAYER As Integer = &H4
        Public Const PAGE_TRIGGER As Integer = &H8
        Public Const PAGE_CAMERA_INFO As Integer = &H10
        Public Const PAGE_IMAGE_FORMAT As Integer = &H20
        Public Const PAGE_AUTOGE_CONTROL As Integer = &H40
        Public Const PAGE_USERSET As Integer = &H80

        Public Const SW_SHOW As Integer = 5
        Public Const SW_HIDE As Integer = 0


        '\brief 创建相机属性页
        '\param [out] phPropty  返回相机属性页句柄
        '\param [in] hCam    相机句柄
        '\param [in] pParentWnd  父窗口句柄,类型CWnd'
        '\param [in] lpszText   相机属性页标题栏文字
        '\param [in] nStyle  相机属性页风格
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetInit(ByRef phPropty As IntPtr, ByVal hCam As IntPtr, ByVal pParentWnd As IntPtr, ByVal lpszText As String, ByVal nPageDisplay As UShort) As MVSTATUS_CODES
        End Function


        '\brief 创建相机属性页
        '\param [out] phPropty  返回相机属性页句柄
        '\param [in] hCam    相机句柄
        '\param [in] pParentWnd  父窗口句柄,类型CWnd'
        '\param [in] lpszText   相机属性页标题栏文字
        '\param [in] nStyle  相机属性页风格
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetCreate(ByRef phPropty As IntPtr, ByVal hCam As IntPtr, ByVal pParentWnd As IntPtr, ByVal lpszText As String, ByVal nPageDisplay As UShort) As MVSTATUS_CODES
        End Function

        '\brief 创建相机属性页
        '\param [out] phPropty  返回相机属性页句柄
        '\param [in] hCam    相机句柄
        '\param [in] pParentWnd  父窗口句柄,类型HWND
        '\param [in] lpszText   相机属性页标题栏文字
        '\param [in] nStyle  相机属性页风格
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetInitEx(ByRef phPropty As IntPtr, ByVal hCam As IntPtr, ByVal pParentWnd As IntPtr, ByVal lpszText As String, ByVal nPageDisplay As UShort) As MVSTATUS_CODES
        End Function

        '\brief 创建相机属性页
        '\param [out] phPropty  返回相机属性页句柄
        '\param [in] hCam    相机句柄
        '\param [in] pParentWnd  父窗口句柄,类型HWND
        '\param [in] lpszText   相机属性页标题栏文字
        '\param [in] nStyle  相机属性页风格
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetCreateEx(ByRef phPropty As IntPtr, ByVal hCam As IntPtr, ByVal pParentWnd As IntPtr, ByVal lpszText As String, ByVal nPageDisplay As UShort) As MVSTATUS_CODES
        End Function


        '\brief 销毁相机属性页
        '\param [in] hPropty    相机属性页句柄
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetDestroy(ByVal hPropty As IntPtr) As MVSTATUS_CODES
        End Function


        '\brief 设置相机属性页标题栏文字
        '\param [in] hPropty    相机属性页句柄
        '\param [in] lpszText   相机属性页标题栏文字
        '\param [in] nStyle   指定属性表标题的样式。 必须指定该样式在0或作为 PSH_PROPTITLE。 如果该样式设置为 PSH_PROPTITLE，单词“属性”中为声明指定的文本之后。 例如，调用 SetTitle(“简单”， PSH_PROPTITLE)会导致“简单属性的属性表声明”。
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetSetTitle(ByVal hPropty As IntPtr, ByVal lpszText As String) As MVSTATUS_CODES
        End Function


        '\brief 设置相机属性页对应的相机。
        '\param [in] hPropty    相机属性页句柄
        '\param [in] hCam    相机句柄
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetSetCamera(ByVal hPropty As IntPtr, ByVal hCam As IntPtr) As MVSTATUS_CODES
        End Function


        '\brief 获取相机属性页当前对应的相机。
        '\param [in] hPropty    相机属性页句柄
        '\param [in] phCam   相机句柄
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetGetCamera(ByVal hPropty As IntPtr, ByRef hCam As IntPtr) As MVSTATUS_CODES
        End Function


        '\brief 设置相机现在是否正工作在采集模式。
        '\param [in] hPropty    相机属性页句柄
        '\param [in] Run 如果相机正工作在采集模式下，设置为TRUE,否则设置为FALSE
        '\note 如果正工作在采集模式，属性页中将禁用一些采集状态下不允许改变的相机属性，如图像大小等。
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetCameraRun(ByVal hPropty As IntPtr, ByVal Run As MVCameraRunEnums) As MVSTATUS_CODES
        End Function


        '\brief  以非模式框方式显示或关闭相机属性页
        '\param [in] hPropty    相机属性页句柄
        '\param [in] nCmdShow    SW_SHOW:显示， SW_HIDE:关闭
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetShow(ByVal hPropty As IntPtr, ByVal nCmdShow As MVCameraRunEnums) As MVSTATUS_CODES
        End Function


        '\brief  以模式框方式显示相机属性页
        '\param [in] hPropty    相机属性页句柄
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetDoModal(ByVal hPropty As IntPtr) As MVSTATUS_CODES
        End Function

        '\brief 设置属性页的父窗口
        '\param [out] hPropty   相机属性页句柄
        '\param [in] pParentWnd  父窗口句柄,类型HWND
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetSetParent(ByVal hPropty As IntPtr, ByVal hParent As Long) As MVSTATUS_CODES
        End Function

        '\brief 插入属性页
        '\param [out] hPropty   相机属性页句柄
        '\param [in] nPageInsert  属性页标识, 可以以或的形式插入多页
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetInsertPage(ByVal hProptySheet As IntPtr, ByVal nPageInsert As UShort) As MVSTATUS_CODES
        End Function

        '\brief 删除属性页
        '\param [out] hPropty   相机属性页句柄
        '\param [in] nPageInsert  属性页标识, 可以以或的形式删除多页
        '\retval
        <DllImport("MVCamProptySheet.dll")>
        Public Shared Function MVCamProptySheetDeletePage(ByVal hProptySheet As IntPtr, ByVal nPageDelete As UShort) As MVSTATUS_CODES
        End Function
    End Class

    Class MVSequenceDlg
        '  \brief 创建序列帧计时器对话框
        '  \param [out] phSeqDlg   返回序列帧计时器对话框句柄
        '  \param [in] pParentWnd  父窗口句柄
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVSequenceDlgCreate(ByRef pSeqDlg As Long, ByVal pParentWnd As IntPtr) As Long
        End Function

        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVSequenceDlgCreateEx(ByRef pSeqDlg As IntPtr, ByVal pParentWnd As IntPtr) As Long
        End Function

        '  \brief 销毁序列帧计时器对话框
        '  \param [in] hSeqDlg 序列帧计时器对话框句柄
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVSequenceDlgDestroy(ByVal hSeqDlg As IntPtr) As Long
        End Function

        '  \brief  以非模式框方式显示或关闭序列帧计时器对话框
        '  \param [in] hSeqDlg 序列帧计时器对话框句柄
        '  \param [in] nCmdShow    SW_SHOW:显示， SW_HIDE:关闭
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVSequenceDlgShow(ByVal hSeqDlg As IntPtr, ByVal nCmdShow As MVShowWindowEnums) As Long
        End Function

        '  \brief 设置相机现在是否正工作在采集模式。
        '  \param [in] hSeqDlg 序列帧计时器对话框句柄
        '  \param [in] bRun    如果相机正工作在采集模式下，设置为TRUE,否则设置为FALSE
        '  \note 如果正工作在采集模式，属性页中将禁用一些采集状态下不允许改变的相机属性，如图像大小等。
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Sub MVSequenceDlgCamRun(ByVal hSeqDlg As IntPtr, ByVal bCamRun As Boolean)
        End Sub

        '  \brief  获得即将进入队列的文件名，及当前图片是否需要保存
        '  \param [in] hSeqDlg 序列帧计时器对话框句柄
        '  \param [out] fname  返回文件名
        '  \param [in] szBuf   文件名长度
        '  \retval 0:保存   -1:不保存  -2:获取文件名出错
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVSequenceDlgGetFileName(ByVal hSeqDlg As IntPtr, ByRef fname As StringBuilder, ByVal szBuf As Integer) As Long
        End Function

        '  \brief  设置父窗口句柄
        '  \param [in] hSeqDlg 序列帧计时器对话框句柄
        '  \param [in] hParent 父窗口句柄
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVSequenceDlgSetParent(ByVal hSeqDlg As IntPtr, ByVal hParent As IntPtr) As Long
        End Function

    End Class
    Class MVRecorDlg


        '  \brief 创建录像计时器对话框
        '  \param [out] phRecDlg   返回录像计时器对话框句柄
        '  \param [in] hCam    相机句柄
        '  \param [in] pParentWnd  父窗口句柄
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVRecordDlgCreate(ByRef pRecDlg As IntPtr, ByVal hCam As IntPtr, ByVal pParentWnd As IntPtr) As Boolean
        End Function

        '  \brief 创建录像计时器对话框
        '  \param [out] phRecDlg   返回录像计时器对话框句柄
        '  \param [in] hCam    相机句柄
        '  \param [in] pParentWnd  父窗口句柄
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVRecordDlgCreateEx(ByRef pRecDlg As IntPtr, ByVal hCam As IntPtr, ByVal pParentWnd As IntPtr) As Boolean
        End Function

        '  \brief 销毁录像计时器对话框
        '  \param [in] hRecDlg 录像计时器对话框句柄
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVRecordDlgDestroy(ByVal hRecDlg As IntPtr) As Boolean
        End Function

        '  \brief  以非模式框方式显示或关闭录像计时器对话框
        '  \param [in] hRecDlg 录像计时器对话框句柄
        '  \param [in] nCmdShow    SW_SHOW:显示， SW_HIDE:关闭
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVRecordDlgShow(ByVal hRecDlg As IntPtr, ByVal nCmdShow As MVShowWindowEnums) As Boolean
        End Function

        '  \brief 设置相机现在是否正工作在采集模式。
        '  \param [in] hRecDlg 录像计时器对话框句柄
        '  \param [in] bRun    如果相机正工作在采集模式下，设置为TRUE,否则设置为FALSE
        '  \note 如果正工作在采集模式，属性页中将禁用一些采集状态下不允许改变的相机属性，如图像大小等。
        '  \retval
        <DllImport("MVTickDlg.dll")>
        Public Shared Sub MVRecordDlgCamRun(ByVal hRecDlg As IntPtr, ByVal bCamRun As Boolean)
        End Sub

        '  \brief  向录像中插入帧
        '  \param [in] hRecDlg 录像计时器对话框句柄
        '  \param [in] pImageBuffer 图像指针
        '  \param [in] nWidth 图像宽度
        '  \param [in] nHeight 图像高度
        '  \param [in] nBpp 单个像素点位数
        '  \param [in] nBlockId 帧号，从开始采集开始计数，计时器根据帧号确定该帧是否插入录像
        '  \retval 0:插入成功  -1:不需插入 -2:插入失败 -3:文件指针为空 1:插入成，并且计时停止
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVRecordDlgRecord(ByVal hRecDlg As IntPtr, ByVal hImage As IntPtr, ByVal nBlockId As UShort) As Integer
        End Function

        '  \brief  设置父窗口句柄
        '  \param [in] hRecDlg 序列帧计时器对话框句柄
        '  \param [in] hParent 父窗口句柄
        <DllImport("MVTickDlg.dll")>
        Public Shared Function MVRecordDlgSetParent(ByVal hRecDlg As IntPtr, ByVal hParent As Long) As Long
        End Function
    End Class
End Namespace

