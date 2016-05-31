#ifndef JAVAREADCARD_H
#define JAVAREADCARD_H

#include "libnetcardreader/netprotocol.h"


class JavaReadCard
{
public:

        JavaReadCard();
        ~JavaReadCard();

        void SetNFCLocal(int iNFCType, char *DevName);
        void SetNFCServer(char * pServerIP, unsigned short wServerPort);
        int  NFCCardReader(struct SCardMsg * pCardMsg);

        //int  GetDevSerialNo(char *pSerialNoBuff, int iBuffLen);
        int  GetLastError();
        int  GetFunLimited();
        int SetFuncUnlimited()
        {
            m_dwFunLimited = 0;
            return m_dwFunLimited;
        }
        int SetNoSNVerify()
        {
            m_dwNoSNVerify = 1;
            return m_dwNoSNVerify;
        }

        //int  SetSDKConfig(char *pConfigBuff, int iBuffLen);

        //add by andy 2016-05-24
        //设置手机序列号
        int SetPhoneSerial(char *serial,int len);
private:
        //CNFCProtocol m_nfcProtocol;
        CNetProtocol m_netProtocol;

        int  m_iNFCType;
        char m_szServerIP[20];
        char m_szSerialNumber[64];
        unsigned short m_wServerPort;
        unsigned short m_wAuthPort;
        unsigned long  m_dwLastErrorCode;

        unsigned long  m_dwFunLimited;
        unsigned long  m_dwNoSNVerify;

        //SOTGParam m_otgParam;
        //SSerParam m_serParam;
        //SOTGParam_Android m_otgAndParam;
        //char * m_nfcParam;


        unsigned int TeaDecrypt(unsigned int *a1, unsigned int a2, int a3);
        int  TeaEncrypt(unsigned int *a1, unsigned int a2, int a3);
        char * callTeaByteIn(const void *a1, int a2, signed int a3);

        //add by andy 2-16-05-24
        char m_phoneSerial[64];

};

#endif // JAVAREADCARD_H
