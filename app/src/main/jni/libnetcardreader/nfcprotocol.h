#ifndef NFC_PROTOCOL_H
#define NFC_PROTOCOL_H



//NFC命令定义-----------------------|功能描述-------|参数p1--------|参数p2-----------|备注----------------
#define NFC_CMD_REQB      0x05      //选B卡        |    无        |     无           |
#define NFC_CMD_NOATTRIB  0x1D      //属性         |    无        |     无           |               

//APDU命令
#define NFC_CMD_SELFILE   0xA4      //选择文件     |   文件分区   |     无           |         
#define NFC_CMD_RDFILE    0xB0      //读文件       |   偏移地址   |     长度         |
#define NFC_CMD_INCERT    0x88      //内部认证     |   缓冲地址   |     长度         |
#define NFC_CMD_OUTCERT   0x82      //外部认证     |   缓冲地址   |     长度         |
#define NFC_CMD_RDRAND    0x84      //读随机数     |   随机数长度 |     无           |
#define NFC_CMD_RDUID     0x36      //读UID        |   UID长度    |     无           |

//组合命令
#define NFC_CMD_SELRDFILE 0xA4B0    //选择并读文件 |   文件分区   |     读取长度     |  该模式为连续读取，并去除90 00

//#include "serial.h"


class CNFCProtocol
{
public:
	CNFCProtocol();
	~CNFCProtocol();

	int CheckNFCDev(int nfcType, char * nfcDev);

	int OpenNFCDev(int nfcType, char * nfcDev);
	int CloseNFCDev();

	int InitNFC();
	int CmdRequest(int cmdno, int p1, int p2, char * recvbuff, int bufflen);

private:
//	CSerial m_ser;

	int m_nfcDev;


	void printf_frame(unsigned char *buff, unsigned char len);
	unsigned char  buildCommand(unsigned char* cmd, unsigned char cmd_len, unsigned char * des_buf);
	int CheckSum(unsigned char * buff, int len, unsigned char chksum);


};

#endif