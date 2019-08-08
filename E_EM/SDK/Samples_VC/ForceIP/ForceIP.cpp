// forceip.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "atlbase.h"
#include "..\..\include\MVGigE.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int nDevCnt = 0;
	MVEnumerateAllDevices(&nDevCnt);

	printf("found %d devices\n",nDevCnt);

	for(int i=0;i<nDevCnt;i++)
	{
		MVCamInfo caminfo;
		MVGetDevInfo(0,&caminfo);
		printf("ModelName:%s\n",caminfo.mModelName);
	}

	if( nDevCnt != 0 )
	{
		MVCamInfo caminfo;
		HANDLE hCam;
		MVGetDevInfo(0,&caminfo);
		MVForceIp((const char *)caminfo.mEthernetAddr,
			"192.168.1.200","255.255.255.0","0.0.0.0");
			
		if (MVOpenCamByIndex(0, &hCam) == MVST_SUCCESS)
		{
			printf("open camera idx = 0\n");
			MVSetPersistentIpAddress(hCam, "192.168.1.200","255.255.255.0","0.0.0.0");
			unsigned char pGetIp[4], pGetSubNetmask[4], pGetGateWay[4];
			size_t nGetIpLen = 4, nGetSubNetmaskLen = 4, nGetGateWayLen = 4;
			int ret;

			 ret = MVGetPersistentIpAddress(hCam, (char*)pGetIp, &nGetIpLen, (char*)pGetSubNetmask,
				&nGetSubNetmaskLen, (char*)pGetGateWay, &nGetGateWayLen);

			if (ret != MVST_SUCCESS)
			{
				printf("GetPersistentIpAddress error : %d\n", ret);
				return -1;
			}

			printf("GetPersistentIpAddress : [ip]%d.%d.%d.%d  [sub-netmask]%d.%d.%d.%d [gateWay]%d.%d.%d.%d \n ", pGetIp[3], pGetIp[2], pGetIp[1], pGetIp[0], pGetSubNetmask[3], pGetSubNetmask[2], pGetSubNetmask[1], pGetSubNetmask[0], pGetGateWay[3], pGetGateWay[2],
				pGetGateWay[1], pGetGateWay[0]);
			MVCloseCam(0);
		}
	}

	system("pause");
	return 0;
}

