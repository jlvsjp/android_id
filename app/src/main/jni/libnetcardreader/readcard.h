#ifndef READCARD_H
#define READCARD_H

#include "../libnfc/libnfc.h"
#include "nfcprotocol.h"
#include "netprotocol.h"

class CReadCard
{
public:

	CReadCard();
	~CReadCard();

	void SetNFCLocal(int iNFCType, char *DevName);
	void SetNFCServer(char * pServerIP, unsigned short wServerPort);
	int  NFCCardReader(struct SCardMsg * pCardMsg);
	int  GetDevSerialNo(char *pSerialNoBuff, int iBuffLen);
	int  GetLastError();
	int  GetFunLimited();
	int  SetSDKConfig(char *pConfigBuff, int iBuffLen);

private:
	CNFCProtocol m_nfcProtocol;
	CNetProtocol m_netProtocol;

	int  m_iNFCType;
	char m_szServerIP[20];
	char m_szSerialNumber[64];
	unsigned short m_wServerPort;
	unsigned short m_wAuthPort;
	unsigned long  m_dwLastErrorCode;
	unsigned long  m_dwFunLimited;
	unsigned long  m_dwNoSNVerify;

	SOTGParam m_otgParam;
	SSerParam m_serParam;
	SOTGParam_Android m_otgAndParam;
	char * m_nfcParam;


	unsigned int TeaDecrypt(unsigned int *a1, unsigned int a2, int a3);
	int  TeaEncrypt(unsigned int *a1, unsigned int a2, int a3);
	char * callTeaByteIn(const void *a1, int a2, signed int a3);

};


#endif
