Module Module1
    Public ImgBuf() As Byte
    Public szBuf, szBuf1 As Long

    Public Declare Sub RtlMoveMemory Lib "kernel32" (ByVal hpvDest As Long, ByVal hpvSource As Long, ByVal cbCopy As Long)

End Module
