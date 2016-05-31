#ifndef __LIB_NETCARDREADER_H
#define __LIB_NETCARDREADER_H

#ifdef WIN32
#define NFC_NET_API  __stdcall 


#else
#define NFC_NET_API   

#endif


struct SCardMsg
{
	char szCardNo[38];         //���֤��
	char szName[32];           //����
	char szSex[4];             //�Ա�
	char szNationality[6];     //����
	char szBirth[18];          //����
	char szAddress[92];        //סַ
	char szAuthority[92];      //ǩ������ 
	char szPeriod[40];         //��Ч����
	char szPhoto[1024];        //���֤��Ƭ, wlf��ʽ
	char szDn[16];
	char Reserve[48];
	//char Reserve[64];	       //����
};

struct SSerDev
{
	char devname[64];          // �����豸����ttyUSB0��COM1
	char Reserve[28];          // ����
};

struct SSDKInfo
{
	char szSDKVersion[32];      // SDK�汾
	char szSDKBuildDate[32];    // SDK��������
	unsigned char bySDKLimited; // SDK�������ƣ�0 δ���ƣ�����ֵΪ����
	char Reserve[32];           // ����
};

// NFC�豸����
#define NFC_OTG_DEV          1   // OTG
#define NFC_SER_DEV          2   // ����
#define NFC_OTG_ANDROID_DEV  3   // OTG ��׿
#define NFC_LOCAL_ANDROID_DEV   4   //�Դ�NFC ��׿
#define NFC_AUTO_DEV         99  // �Զ�


// ������
#define NFC_NET_NOERROR               0        // û�д���
#define NFC_NET_NFCREADCARD_SUCCESS   0        // NFC�����ɹ�
#define NFC_NET_NETCONNECT_ERROR      1        // �������Ӵ���
#define NFC_NET_NETRECV_ERROR         2        // ������մ���
#define NFC_NET_NETSEND_ERROR         3        // ���緢�ʹ���
#define NFC_NET_NFCOPEN_ERROR         4        // NFC�򿪴���
#define NFC_NET_NFCREADCARD_ERROR     5        // NFC��������
#define NFC_NET_AUTHENTICATION_FAIL   6        // ��֤ʧ��
#define NFC_NET_SEARCHCARD_FAIL       7        // NFCѰ��ʧ��
#define NFC_NET_STARTNETREADCARD_FAIL 8        // �����������ʧ��
#define NFC_NET_INVALIDNETCMD         9        // ��Ч����������
//add by andy 2016-03-15
#define NFC_NET_GETSERIAL_FAIL        10       // ��ȡ�豸���к�ʧ��
#define NFC_NET_AUTHOR_FAIL           11       // �豸��֤ʧ��

//add by andy 2016-05-27 for java call
#define NFC_NET_JAVA_TYPE_ERR         13        // NFC Type Error
#define NFC_NET_JAVA_API_ERR          14        // JAVA CallBack Error

#define NFC_NET_UNKNOWN_ERROR         99       // δ֪����


#ifdef __cplusplus
extern "C"
{
#endif

	// ����NFC����
	void NFC_NET_API NFC_NET_SetNFCLocal(int iNFCType, char *DevName);

	// ����NFC����
	void NFC_NET_API NFC_NET_SetNFCServer(char * pServerIP, unsigned short wServerPort);

	// NFC����
	int  NFC_NET_API NFC_NET_NFCCardReader(struct SCardMsg * pCardMsg);

	// ��ȡ�豸���к�
	int  NFC_NET_API NFC_NET_GetDevSerialNo(char *pSerialNoBuff, int iBuffLen);

	// ��ȡSDK��Ϣ
	int  NFC_NET_API NFC_NET_GetSDKInfo(struct SSDKInfo * pSDKInfo);

	// ��ȡ������Ϣ
	int  NFC_NET_API NFC_NET_GetLastError();

#ifdef __cplusplus
}
#endif

#endif
