#ifndef __LIB_NETCARDREADER_H
#define __LIB_NETCARDREADER_H

#ifdef WIN32
#define NFC_NET_API  __stdcall 


#else
#define NFC_NET_API   

#endif


struct SCardMsg
{
	char szCardNo[38];         //身份证号
	char szName[32];           //姓名
	char szSex[4];             //性别
	char szNationality[6];     //民族
	char szBirth[18];          //出生
	char szAddress[92];        //住址
	char szAuthority[92];      //签发机关 
	char szPeriod[40];         //有效期限
	char szPhoto[1024];        //身份证照片, wlf格式
	char szDn[16];
	char Reserve[48];
	//char Reserve[64];	       //保留
};

struct SSerDev
{
	char devname[64];          // 串口设备，如ttyUSB0，COM1
	char Reserve[28];          // 保留
};

struct SSDKInfo
{
	char szSDKVersion[32];      // SDK版本
	char szSDKBuildDate[32];    // SDK发布日期
	unsigned char bySDKLimited; // SDK功能限制，0 未限制，其他值为限制
	char Reserve[32];           // 保留
};

// NFC设备类型
#define NFC_OTG_DEV          1   // OTG
#define NFC_SER_DEV          2   // 串口
#define NFC_OTG_ANDROID_DEV  3   // OTG 安卓
#define NFC_LOCAL_ANDROID_DEV   4   //自带NFC 安卓
#define NFC_AUTO_DEV         99  // 自动


// 错误定义
#define NFC_NET_NOERROR               0        // 没有错误
#define NFC_NET_NFCREADCARD_SUCCESS   0        // NFC读卡成功
#define NFC_NET_NETCONNECT_ERROR      1        // 网络连接错误
#define NFC_NET_NETRECV_ERROR         2        // 网络接收错误
#define NFC_NET_NETSEND_ERROR         3        // 网络发送错误
#define NFC_NET_NFCOPEN_ERROR         4        // NFC打开错误
#define NFC_NET_NFCREADCARD_ERROR     5        // NFC读卡错误
#define NFC_NET_AUTHENTICATION_FAIL   6        // 认证失败
#define NFC_NET_SEARCHCARD_FAIL       7        // NFC寻卡失败
#define NFC_NET_STARTNETREADCARD_FAIL 8        // 启动网络读卡失败
#define NFC_NET_INVALIDNETCMD         9        // 无效的网络命令
//add by andy 2016-03-15
#define NFC_NET_GETSERIAL_FAIL        10       // 获取设备序列号失败
#define NFC_NET_AUTHOR_FAIL           11       // 设备认证失败

//add by andy 2016-05-27 for java call
#define NFC_NET_JAVA_TYPE_ERR         13        // NFC Type Error
#define NFC_NET_JAVA_API_ERR          14        // JAVA CallBack Error

#define NFC_NET_UNKNOWN_ERROR         99       // 未知错误


#ifdef __cplusplus
extern "C"
{
#endif

	// 设置NFC本地
	void NFC_NET_API NFC_NET_SetNFCLocal(int iNFCType, char *DevName);

	// 设置NFC服务
	void NFC_NET_API NFC_NET_SetNFCServer(char * pServerIP, unsigned short wServerPort);

	// NFC读卡
	int  NFC_NET_API NFC_NET_NFCCardReader(struct SCardMsg * pCardMsg);

	// 获取设备序列号
	int  NFC_NET_API NFC_NET_GetDevSerialNo(char *pSerialNoBuff, int iBuffLen);

	// 获取SDK信息
	int  NFC_NET_API NFC_NET_GetSDKInfo(struct SSDKInfo * pSDKInfo);

	// 获取错误信息
	int  NFC_NET_API NFC_NET_GetLastError();

#ifdef __cplusplus
}
#endif

#endif
