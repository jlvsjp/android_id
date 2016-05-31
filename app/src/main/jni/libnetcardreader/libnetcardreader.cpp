#include "libnetcardreader.h"
#include "readcard.h"

#include <string.h>

#define SDK_VERSION     "1.1.0"
#define SDK_BUILD_DATA  "20160512"


CReadCard g_clsReadCard;

// ����NFC����
void NFC_NET_API NFC_NET_SetNFCLocal(int iNFCType, char *DevName)
{
	g_clsReadCard.SetNFCLocal(iNFCType, DevName);
}

// ����NFC����
void NFC_NET_API NFC_NET_SetNFCServer(char * pServerIP, unsigned short wServerPort)
{
	g_clsReadCard.SetNFCServer(pServerIP, wServerPort);
}

// NFC����
int  NFC_NET_API NFC_NET_NFCCardReader(struct SCardMsg * pCardMsg)
{
	return g_clsReadCard.NFCCardReader(pCardMsg);
}

// ��ȡ�豸���к�
int  NFC_NET_API NFC_NET_GetDevSerialNo(char *pSerialNoBuff, int iBuffLen)
{
	return g_clsReadCard.GetDevSerialNo(pSerialNoBuff, iBuffLen);
}

// ��ȡSDK��Ϣ
int NFC_NET_API NFC_NET_GetSDKInfo(struct SSDKInfo * pSDKInfo)
{
	strcpy(pSDKInfo->szSDKVersion, SDK_VERSION);
	strcpy(pSDKInfo->szSDKBuildDate, SDK_BUILD_DATA);
	pSDKInfo->bySDKLimited = g_clsReadCard.GetFunLimited();
	return NFC_NET_NOERROR;
}

// ��ȡ������Ϣ
int  NFC_NET_API NFC_NET_GetLastError()
{
	return g_clsReadCard.GetLastError();
}

// ����SDK��Ϣ
int  NFC_NET_API NFC_NET_SetSDKConfig(char *pConfigBuff, int iBuffLen)
{
	return g_clsReadCard.SetSDKConfig(pConfigBuff, iBuffLen);
}





