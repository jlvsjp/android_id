#include "libnetcardreader.h"
#include "readcard.h"

#include <string.h>

#define SDK_VERSION     "1.1.0"
#define SDK_BUILD_DATA  "20160512"


CReadCard g_clsReadCard;

// 设置NFC本地
void NFC_NET_API NFC_NET_SetNFCLocal(int iNFCType, char *DevName)
{
	g_clsReadCard.SetNFCLocal(iNFCType, DevName);
}

// 设置NFC服务
void NFC_NET_API NFC_NET_SetNFCServer(char * pServerIP, unsigned short wServerPort)
{
	g_clsReadCard.SetNFCServer(pServerIP, wServerPort);
}

// NFC读卡
int  NFC_NET_API NFC_NET_NFCCardReader(struct SCardMsg * pCardMsg)
{
	return g_clsReadCard.NFCCardReader(pCardMsg);
}

// 获取设备序列号
int  NFC_NET_API NFC_NET_GetDevSerialNo(char *pSerialNoBuff, int iBuffLen)
{
	return g_clsReadCard.GetDevSerialNo(pSerialNoBuff, iBuffLen);
}

// 获取SDK信息
int NFC_NET_API NFC_NET_GetSDKInfo(struct SSDKInfo * pSDKInfo)
{
	strcpy(pSDKInfo->szSDKVersion, SDK_VERSION);
	strcpy(pSDKInfo->szSDKBuildDate, SDK_BUILD_DATA);
	pSDKInfo->bySDKLimited = g_clsReadCard.GetFunLimited();
	return NFC_NET_NOERROR;
}

// 获取错误信息
int  NFC_NET_API NFC_NET_GetLastError()
{
	return g_clsReadCard.GetLastError();
}

// 设置SDK信息
int  NFC_NET_API NFC_NET_SetSDKConfig(char *pConfigBuff, int iBuffLen)
{
	return g_clsReadCard.SetSDKConfig(pConfigBuff, iBuffLen);
}





