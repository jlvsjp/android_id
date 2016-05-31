#ifndef LIBNFC_H
#define LIBNFC_H

#ifdef __cplusplus
extern "C"
{
#endif

#define OTG_NFC          1        // OTG类型NFC
#define SER_NFC          2        // 串口类型NFC
#define OTG_NFC_ANDROID  3        // OTG安卓NFC
#define UNKNOWN_NFC      0xff     // 未知类型NFC


//OTG参数
struct SOTGParam{
	unsigned short vendorId;  // 厂商ID 
	unsigned short productId; // 芯片ID
	int speed;                // 通信速度，波特率
	int databits;             // 数据位
	int stopbits;             // 停止位
	int parity;               // 奇偶位
	int flowcontrol;          // 流控制
};

struct SOTGParam_Android{
	int  devdescriptor;       // 设备描述
	char devname[64];         // 设备名称
	int speed;                // 通信速度，波特率
	int databits;             // 数据位
	int stopbits;             // 停止位
	int parity;               // 奇偶位
	int flowcontrol;          // 流控制
};

//串口参数
struct SSerParam{
	char devname[64];         // 串口设备，如ttyUSB0
	int speed;                // 通信速度，波特率
	int databits;             // 数据位
	int stopbits;             // 停止位
	int parity;               // 奇偶位
	int flowcontrol;          // 流控制
};


/* 测检NFC
 * 成功返回1，失败返回0	
 */
int CheckNFC(int nfctype, char *param);

/* 打开NFC
 * 成功返回设备句柄，失败返回-1	
 */
int OpenNFC(int nfctype, char *param);

/* 关闭NFC
 * 成功返回0，失败返回-1	
 */
int CloseNFC(int devfd);

/* 读NFC数据
 * 成功返回读取字节数，失败返回-1
 */
int ReadNFCData(int devfd, char *buff,int len);

/* 写NFC数据
 * 成功返回写入字节数，失败返回-1
 */
int WriteNFCData(int devfd, char *buff,int len);
	
#ifdef __cplusplus
}
#endif

#endif
