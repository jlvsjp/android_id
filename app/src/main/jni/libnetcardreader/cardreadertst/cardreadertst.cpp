#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <signal.h>

#include "libnetcardreader.h"

int g_run = 0;

void sig_term()
{
	printf("sig_term\n");
	g_run = 0;
}

void sig_int()
{
	printf("sig_int\n");
	g_run = 0;
}

struct SOTGDev
{
	unsigned short vendorId;  // ≥ß…ÃID 
	unsigned short productId; // –æ∆¨ID
	char Reserve[28];         //±£¡Ù
};


int main(int argc, char * argv[])
{
	int nfc_type = 0xff;
	int iRet = -1;
	struct SCardMsg cardMsg;
	struct SSDKInfo sdkInfo;
	char szSerialNo[16] = {0};

	
	//signal(SIGTERM, sig_term);
	//signal(SIGINT, sig_int);
	
	memset(&sdkInfo, 0, sizeof(sdkInfo));
	iRet = NFC_NET_GetSDKInfo(&sdkInfo);
	if(iRet == NFC_NET_NOERROR)
	{
		printf("sdk ver %s, build %s, %s\n", sdkInfo.szSDKVersion, sdkInfo.szSDKBuildDate, sdkInfo.bySDKLimited == 0 ? " " : "Limited"); 
	}

	NFC_NET_SetNFCLocal(NFC_OTG_DEV, NULL);
	if(argc == 3)
	{
		NFC_NET_SetNFCServer(argv[1], atol(argv[2]));
	}
	else
	{
		NFC_NET_SetNFCServer("192.168.3.254", 9018);
	}

	iRet = NFC_NET_GetDevSerialNo(szSerialNo, sizeof(szSerialNo));

	if(iRet == NFC_NET_NOERROR)
	{
		printf("serial no: %s\n", szSerialNo);
	}
	else
	{
		printf("read serial no fail\n");
	}
	
	memset(&cardMsg, 0, sizeof(cardMsg));
	iRet = NFC_NET_NFCCardReader(&cardMsg);
	
	if( iRet == NFC_NET_NFCREADCARD_SUCCESS)
	{
		int i = 0;
		printf("read card success\n");
		printf("name: ");
		for(i = 0; i < 6; i++)
		{
			printf("%02x ", cardMsg.szName[i]);
		}
		
		printf("\n");
		printf("birth: ");
		for(i = 0; i < 16; i++)
		{
			printf("%02x ", cardMsg.szBirth[i]);
		}
		printf("\n");
	}
	else
	{
		int iError = 0;
		iError = NFC_NET_GetLastError();
		printf("read card fail, errno %d\n", iError);  	
	}
	
	return 0;
}



