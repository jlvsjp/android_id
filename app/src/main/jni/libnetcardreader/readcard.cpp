#include "readcard.h"
#include "libnetcardreader.h"
#include "../libnfc/libnfc.h"
#include "verify_client.h"


#ifdef WIN32    // windows
#include <windows.h>

#else           // linux
#include <unistd.h>

#define Sleep(a)   usleep(a*1000)

#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 设备定义
const unsigned short nfc_productid[] = {0x7523, 0x5523};  // ch340 ch341

#define NFC_VENDOR_ID  0x1A86
#define NFC_PRODUCT_ID nfc_productid

#define NFC_READCARD_SPEED    115200
#define NFC_READEEPROM_SPEED  300

// 读序列号
#define NFC_READEEPROM_OFFSETADDR 0x08
#define NFC_SERIALNO_LEN          16

#define AUTH_PORT_OFFSET 2

#define CARD_INFO_LEN   256
#define CARD_PHOTO_LEN  1024

#define SDK_FUNCTION_LIMITED

const unsigned char START_NETNFC_RD[] = {0xe0, 0x35, 0x37, 0x48, 0x58};

unsigned char keys[]  = {0x75, 0x9A, 0x3D,0xB8,0x96,0xB8,0x6A,0xE7,0xB,0xC4,0xA8,0x53,0x24,0x8D,0xC,0x53};
unsigned char keys1[] = {0x75, 0x9A, 0x3D,0xB8,0x96,0xB8,0x6A,0xE7,0xB,0xC4,0xA8,0x53,0x24,0x8D,0xC,0x53};
unsigned char keys2[] = {0x75, 0x9A, 0x3D,0xB8,0x96,0xB8,0x6A,0xE7,0xB,0xC4,0xA8,0x53,0x24,0x8D,0xC,0x53};

char LIMITED_IFNO[] = {0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 
                       0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00, 0x58, 0x00};



#define START_NETNFC_RDLEN (sizeof(START_NETNFC_RD) + 5)

// 内部使用，不向用户公开

// OTG设备
struct SOTGDev
{
	unsigned short vendorId;  // 厂商ID 
	unsigned short productId; // 芯片ID
	char Reserve[28];         // 保留
};

// SDK配置
struct SSDKConfig
{
	unsigned char bySDKLimited; // SDK限制
	unsigned char byNoSNVerify; // 序列号不认证
	char Reserve[62];
};
 
CReadCard::CReadCard()
{
	m_iNFCType = UNKNOWN_NFC;
	m_wServerPort = 0;
	m_wAuthPort = 0;
	m_dwLastErrorCode = 0;
	m_nfcParam = NULL;

#ifdef SDK_FUNCTION_LIMITED
	m_dwFunLimited = 1;
#else
	m_dwFunLimited = 0;
#endif
	m_dwNoSNVerify = 0;

	memset(m_szServerIP, 0, sizeof(m_szServerIP));
	memset(&m_otgParam, 0, sizeof(m_otgParam));
	memset(&m_serParam, 0, sizeof(m_serParam));
	memset(&m_szSerialNumber, 0, sizeof(m_szSerialNumber));
	memset(&m_otgAndParam, 0, sizeof(m_otgAndParam));
}

CReadCard::~CReadCard()
{

}

void CReadCard::SetNFCLocal(int iNFCType, char *DevName)
{
	m_iNFCType = iNFCType;

	m_nfcParam = NULL;
	switch(iNFCType)
	{
	case NFC_OTG_DEV:
		printf("nfc work in otg dev mode\n");
		memset(&m_otgParam, 0, sizeof(m_otgParam));
		if(!DevName)
		{
			int i = 0;
			m_otgParam.vendorId = NFC_VENDOR_ID;
			m_otgParam.speed = NFC_READCARD_SPEED;
			m_otgParam.databits = 8;
			m_otgParam.stopbits = 1;
			m_otgParam.parity = 'N';
			m_otgParam.flowcontrol = 0;
			for(i = 0; i < sizeof(NFC_PRODUCT_ID)/sizeof(unsigned short); i++)
			{
				m_otgParam.productId = NFC_PRODUCT_ID[i];
				if(m_nfcProtocol.CheckNFCDev(iNFCType, (char*)&m_otgParam))
				{
					break;
				}
			}
		}
		else
		{
			struct SOTGDev * otgDev = (struct SOTGDev *)DevName;
			m_otgParam.vendorId = otgDev->vendorId;
			m_otgParam.productId = otgDev->productId;
		}
		
		m_nfcParam = (char*)&m_otgParam;
		break;
	case NFC_OTG_ANDROID_DEV:
		printf("nfc work in android otg dev mode\n");
		memset(&m_otgAndParam, 0, sizeof(m_otgAndParam));
		if(DevName)
		{
			m_otgAndParam.devdescriptor = *(int*)DevName;
			m_otgAndParam.speed = NFC_READCARD_SPEED;
			m_otgAndParam.databits = 8;
			m_otgAndParam.stopbits = 1;
			m_otgAndParam.parity = 'N';
			m_otgAndParam.flowcontrol = 0;
		}
		
		m_nfcParam = (char*)&m_otgAndParam;
		break;
	case NFC_SER_DEV:
		printf("nfc work in serial dev mode\n");
		memset(&m_serParam, 0, sizeof(m_serParam));
		if(!DevName)
		{
#ifdef WIN32
		strcpy(m_serParam.devname, "COM1");
#else
		strcpy(m_serParam.devname, "ttyUSB0");
#endif
		}
		else
		{
			struct SSerDev * serDev = (struct SSerDev *)DevName;
			strcpy(m_serParam.devname, serDev->devname);
		}
		m_serParam.speed = NFC_READCARD_SPEED;
		m_serParam.databits = 8;
		m_serParam.stopbits = 1;
		m_serParam.parity = 'N';
		m_serParam.flowcontrol = 0;

		m_nfcParam = (char*)&m_serParam;
		break;
	case NFC_AUTO_DEV:
		{
			int i = 0;

			printf("nfc work in auto mode\n");
#ifdef WIN32
			memset(&m_serParam, 0, sizeof(m_serParam));
			m_serParam.speed = NFC_READCARD_SPEED;
			m_serParam.databits = 8;
			m_serParam.stopbits = 1;
			m_serParam.parity = 'N';
			m_serParam.flowcontrol = 0;
			
			for(i = 0; i < 256; i++)
			{
				memset(m_serParam.devname, 0, sizeof(m_serParam.devname));
				sprintf(m_serParam.devname, "COM%d", i + 1);
				if(m_nfcProtocol.CheckNFCDev(NFC_SER_DEV, (char*)&m_serParam))
				{
					m_iNFCType = NFC_SER_DEV;
					m_nfcParam = (char*)&m_serParam;
					break;
				}
			}
#else
			memset(&m_otgParam, 0, sizeof(m_otgParam));
			m_otgParam.speed = NFC_READCARD_SPEED;
			m_otgParam.databits = 8;
			m_otgParam.stopbits = 1;
			m_otgParam.parity = 'N';
			m_otgParam.flowcontrol = 0;	
			
			m_otgParam.vendorId = NFC_VENDOR_ID;
			for(i = 0; i < sizeof(NFC_PRODUCT_ID)/sizeof(unsigned short); i++)
			{
				m_otgParam.productId = NFC_PRODUCT_ID[i];
				if(m_nfcProtocol.CheckNFCDev(NFC_OTG_DEV, (char*)&m_otgParam))
				{
					break;
				}
			}
			m_iNFCType = NFC_OTG_DEV;
			m_nfcParam = (char*)&m_otgParam;
#endif
		}
		break;
	case UNKNOWN_NFC:
	default:
		m_iNFCType = UNKNOWN_NFC;
	}
}

void CReadCard::SetNFCServer(char * pServerIP, unsigned short wServerPort)
{
	memset(m_szServerIP, 0, sizeof(m_szServerIP));
	if(pServerIP)
	{
		strcpy(m_szServerIP, pServerIP);
	}
	if(wServerPort > 0)
	{
		m_wServerPort = wServerPort;
		m_wAuthPort = m_wServerPort + AUTH_PORT_OFFSET;		
	}
}

int  CReadCard::NFCCardReader(struct SCardMsg * pCardMsg)
{
	int iRet = -1;
	char netbuff[1400] = {0};
	char buff[1400] = {0};
	int len = 0;
	int ret = 0;
	int offset =0;
	
	char dnbuff[64] = {0};
	int  dnlen = 0;
	int  filelen = 0;
	int  start_svrdata = 0;
	int  loop_recv = 1;

	SSvrCmd svrCmd;


	unsigned char cardInfo[512] = {0};
	int cardInfoLen = 0;
	unsigned char pic[1300] = {0};
	int piclen = 0;
	unsigned char keys[8] = {0};

	if(!pCardMsg)
	{
		m_dwLastErrorCode = NFC_NET_UNKNOWN_ERROR;
		return -1;
	}

	m_dwLastErrorCode = NFC_NET_NFCREADCARD_SUCCESS;

	//add by andy 2016-03-15 start
	// 读取序列号
	char serial[16] = {0};
	memset(serial,'\0',16);
	iRet = GetDevSerialNo(serial,16);
	if(iRet < 0)
	{
		m_dwLastErrorCode = NFC_NET_GETSERIAL_FAIL;
		return -1;
	}

	// 序列号认证
	if(!m_dwNoSNVerify)
	{
		iRet = verify_serial(m_szServerIP,m_wAuthPort,serial,strlen(serial));
		if(iRet < 0)
		{
			m_dwLastErrorCode = NFC_NET_AUTHOR_FAIL;
			return -1;
		}
	}
	//add by andy 2016-03-15 end

	// 打开本地设备
	iRet = m_nfcProtocol.OpenNFCDev(m_iNFCType, m_nfcParam);
	if(iRet == -1)
	{
		m_dwLastErrorCode = NFC_NET_NFCOPEN_ERROR;
		return -1;
	}
	// 初始化设备
	m_nfcProtocol.InitNFC();


	// 寻卡
	len = m_nfcProtocol.CmdRequest(NFC_CMD_REQB, 0, 0, buff, sizeof(buff));
	if(len <= 0)
	{
		m_dwLastErrorCode = NFC_NET_SEARCHCARD_FAIL;
		m_nfcProtocol.CloseNFCDev();
		return -1;
	}

	// 连接网络
	iRet = m_netProtocol.ConnectServer(m_szServerIP, m_wServerPort);
	if(iRet < 0)
	{
		m_dwLastErrorCode = NFC_NET_NETCONNECT_ERROR;
		m_nfcProtocol.CloseNFCDev();
		return -1;
	}

	// 接收网络读卡命令
	iRet = m_netProtocol.RecvData(netbuff, START_NETNFC_RDLEN);

	if(iRet < 0)
	{
		m_dwLastErrorCode = NFC_NET_NETRECV_ERROR;
		m_netProtocol.DisconnectServer();
		m_nfcProtocol.CloseNFCDev();
		return -1;
	}

	if((iRet != START_NETNFC_RDLEN) && memcmp(netbuff, START_NETNFC_RD, sizeof(START_NETNFC_RD)))
	{
		m_dwLastErrorCode = NFC_NET_STARTNETREADCARD_FAIL;
		m_netProtocol.DisconnectServer();
		m_nfcProtocol.CloseNFCDev();
		return -1;
	}

	// 发送设备序列号，并通过SM4加密
	{
		char serial_enc[64] = {0};
		int verify_len = sizeof(serial_enc);

		memset(netbuff, 0, sizeof(netbuff));
		sm4_serial_encode(serial, strlen(serial), serial_enc);
		offset = 4;
		offset = m_netProtocol.CopyNetData(offset, verify_len, serial_enc, netbuff, false);
		verify_len = m_netProtocol.BuildNetHead(CLT_CMD_VERIFY_SERIAL, 1, offset - 4, netbuff);
		iRet = m_netProtocol.SendData(netbuff, verify_len);

		if(iRet < 0)
		{
			m_dwLastErrorCode = NFC_NET_NETSEND_ERROR;
			m_netProtocol.DisconnectServer();
			m_nfcProtocol.CloseNFCDev();
			return -1;
		}
	}

	memset(netbuff, 0, sizeof(netbuff));

	offset = 4;
	offset = m_netProtocol.CopyNetData(offset, len, buff, netbuff);
	len = m_nfcProtocol.CmdRequest(NFC_CMD_NOATTRIB, 0, 0, buff, sizeof(buff));
	if(len <= 0)
	{
		m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
		m_netProtocol.DisconnectServer();
		m_nfcProtocol.CloseNFCDev();
		return -1;
	}

	len = m_nfcProtocol.CmdRequest(NFC_CMD_SELFILE, 0x6002, 0, buff, sizeof(buff));
	if(len <= 0)
	{
		m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
		m_netProtocol.DisconnectServer();
		m_nfcProtocol.CloseNFCDev();
		return -1;
		
	}
	len = m_nfcProtocol.CmdRequest(NFC_CMD_RDFILE, 0x0000, 0x20, buff, sizeof(buff));
	if(len <= 0)
	{
		m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
		m_netProtocol.DisconnectServer();
		m_nfcProtocol.CloseNFCDev();
		return -1;		
	}
	//add by andy 2016-01-18 start
	char DN[16] = {0};
	memcpy(DN,buff,16);
	//add by andy 2016-01-18 end


	offset = m_netProtocol.CopyNetData(offset, len, buff, netbuff);
	len = m_netProtocol.BuildNetHead(CLT_CMD_STARTRDCARD_ACK, 2, offset - 4, netbuff);

	memcpy(dnbuff, netbuff, len);
	dnlen = len;

	iRet = m_netProtocol.SendData(netbuff, len);

	if(iRet < 0)
	{
		m_dwLastErrorCode = NFC_NET_NETSEND_ERROR;
		m_netProtocol.DisconnectServer();
		m_nfcProtocol.CloseNFCDev();
		return -1;
	}

	// 读卡
	while(loop_recv)
	{
		memset(&svrCmd, 0, sizeof(svrCmd));
		iRet = m_netProtocol.RecvData(&svrCmd, sizeof(SSvrCmd));

		if(iRet < 0)
		{
			m_dwLastErrorCode = NFC_NET_NETRECV_ERROR;
			m_netProtocol.DisconnectServer();
			m_nfcProtocol.CloseNFCDev();
			return -1;
		}
		switch(svrCmd.type)
		{
		case SVR_CMDTYPE_SENDCDM:
			switch(svrCmd.cmd)
			{
			case SVR_CMD_INCERT:           // 内部认证
				memset(netbuff, 0, sizeof(netbuff));
				iRet = m_netProtocol.RecvData(netbuff, 17);
				if(iRet < 0)
				{
					m_dwLastErrorCode = NFC_NET_NETRECV_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}

				if((unsigned char)netbuff[2] != 0x00 || (unsigned char)netbuff[3] != NFC_CMD_INCERT)
				{
					m_dwLastErrorCode = NFC_NET_INVALIDNETCMD;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}

				len = m_nfcProtocol.CmdRequest(NFC_CMD_INCERT,(int)(netbuff + 2), iRet - 2, buff, sizeof(buff));
				if(len <= 0)
				{
					m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}
				
				offset = 4;
				memset(netbuff, 0, sizeof(netbuff));
				offset = m_netProtocol.CopyNetData(offset, len, buff, netbuff);
				
				len = m_nfcProtocol.CmdRequest(NFC_CMD_RDRAND, 8, 0, buff, sizeof(buff));
				if(len <= 0)
				{
					m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}

				offset = m_netProtocol.CopyNetData(offset, len, buff, netbuff);
				len = m_netProtocol.BuildNetHead(CLT_CMD_INAUTH_ACK, 2, offset - 4, netbuff);

				iRet = m_netProtocol.SendData(netbuff, len);
				if(iRet < 0)
				{
					m_dwLastErrorCode = NFC_NET_NETSEND_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}				
				break;
			case SVR_CMD_OUTCERT:           // 外部认证
				memset(netbuff, 0, sizeof(netbuff));
				iRet = m_netProtocol.RecvData(netbuff, 17);
				if(iRet < 0)
				{
					m_dwLastErrorCode = NFC_NET_NETRECV_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}
				
				if((unsigned char)netbuff[2] != 0x00 || (unsigned char)netbuff[3] != NFC_CMD_OUTCERT)
				{
					m_dwLastErrorCode = NFC_NET_INVALIDNETCMD;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}

				if(start_svrdata) // 等待接收NFC服务器解码后的数据
				{
					continue;
				}

				memset(buff, 0, sizeof(buff));
				len = m_nfcProtocol.CmdRequest(NFC_CMD_OUTCERT,(int)(netbuff + 2), iRet - 2, buff, sizeof(buff));
				
				if(len > 0 && (unsigned char)buff[0] == 0x90 && (unsigned char)buff[1] == 0x00) //外部认证 OK
				{
					// 读取身份证信息
					memset(buff, 0, sizeof(buff));

					// 读文件 6011
					filelen = m_nfcProtocol.CmdRequest(NFC_CMD_SELRDFILE, 0x6011, 0xD6, buff, sizeof(buff));
					if(filelen <= 0)
					{
						m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
						m_netProtocol.DisconnectServer();
						m_nfcProtocol.CloseNFCDev();
						return -1;
					}
					
					offset = 4;
					memset(netbuff, 0, sizeof(netbuff));
					offset = m_netProtocol.CopyNetData(offset, filelen, buff, netbuff, false);
					
					// 读文件 6012
					memset(buff, 0, sizeof(buff));
					filelen = m_nfcProtocol.CmdRequest(NFC_CMD_SELRDFILE, 0x6012, 0x100, buff, sizeof(buff));
					if(filelen <= 0)
					{
						m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
						m_netProtocol.DisconnectServer();
						m_nfcProtocol.CloseNFCDev();
						return -1;
					}
					offset = m_netProtocol.CopyNetData(offset, filelen, buff, netbuff, false);
					len = m_netProtocol.BuildNetHead(CLT_CMD_OUTAUTH_ACK, 2, offset - 4, netbuff);
					m_netProtocol.SendData(netbuff, len);
				}
				else
				{
					m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}

				break;
			case SVR_CMD_MSG_ACK:
				memset(buff, 0, sizeof(buff));
				filelen = m_nfcProtocol.CmdRequest(NFC_CMD_SELRDFILE, 0x6013, 0x400, buff, sizeof(buff));
				if(filelen <= 0)
				{
					m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}				
				// 上传照片第一包
				offset = 4;
				memset(netbuff, sizeof(netbuff), 0);
				offset = m_netProtocol.CopyNetData(offset, 514, buff, netbuff, false);
				len = m_netProtocol.BuildNetHead(CLT_CMD_SENDPHOTO1, 0, 1024, netbuff);
				m_netProtocol.SendData(netbuff, 520);
				break;
			case SVR_CMD_RDPHOTO_ACK1:
				// 上传照片第二包
				offset = 4;
				memset(netbuff, sizeof(netbuff), 0);
				offset = m_netProtocol.CopyNetData(offset, 510, &buff[514], netbuff, false);
				len = m_netProtocol.BuildNetHead(CLT_CMD_SENDPHOTO2, 0, 1024, netbuff);
				m_netProtocol.SendData(netbuff, 516);
				break;
			case SVR_CMD_RDPHOTO_ACK2:

				memset(buff, 0, sizeof(buff));
				filelen = m_nfcProtocol.CmdRequest(NFC_CMD_SELRDFILE, 0x6021, 0x400, buff, sizeof(buff));
				if(filelen < 0)
				{
					m_dwLastErrorCode = NFC_NET_NFCREADCARD_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}
				
				{
					char tmp[1024] = {0};
					if(memcmp(buff, tmp, 1024))  //有指纹
					{
						// 上传指纹第一包
						offset = 4;
						memset(netbuff, sizeof(netbuff), 0);
						offset = m_netProtocol.CopyNetData(offset, 514, buff, netbuff, false);
						len = m_netProtocol.BuildNetHead(CLT_CMD_SENDFINGERPRINT1, 0, 1024, netbuff);
						m_netProtocol.SendData(netbuff, 520);
					}
					else
					{
						dnbuff[0] = CLT_CMD_SECNED_ACK;
						m_netProtocol.SendData(dnbuff, dnlen);
						start_svrdata = 1; // 等待接收NFC服务器解码后的数据
					}
				}
				break;
			case SVR_CMD_RDFINGERPRINT_ACK1:
				offset = 4;
				memset(netbuff, sizeof(netbuff), 0);
				offset = m_netProtocol.CopyNetData(offset, 510, &buff[514], netbuff, false);
				len = m_netProtocol.BuildNetHead(CLT_CMD_SENDFINGERPRINT2, 0, 1024, netbuff);
				m_netProtocol.SendData(netbuff, 516);
				break;
			case SVR_CMD_RDFINGERPRINT_ACK2:
				dnbuff[0] = CLT_CMD_SECNED_ACK;
				m_netProtocol.SendData(dnbuff, dnlen);
				start_svrdata = 1; // 等待接收NFC服务器解码后的数据
				break;
			}
			break;
		case SVR_CMDTYPE_SENDDATA:
			{
				// 0x01 | 0x20 | 包数 | 包序 | 包长度 | 数据
				int packs = 0;
				int packno = 0;
				int packlen = 0;

				memset(netbuff, 0, sizeof(netbuff));
				iRet = m_netProtocol.RecvData(netbuff, 4);
				if(iRet < 0)
				{
					m_dwLastErrorCode = NFC_NET_NETRECV_ERROR;
					m_netProtocol.DisconnectServer();
					m_nfcProtocol.CloseNFCDev();
					return -1;
				}

				packs = netbuff[0];
				packno = netbuff[1];
				packlen = ((unsigned char)netbuff[2]) << 8;
				packlen |= (unsigned char)netbuff[3];

				if(packs > 0  && packno > 0 && packno <= packs)
				{
					memset(netbuff, 0, sizeof(netbuff));
					iRet = m_netProtocol.RecvData(netbuff, packlen);
					if(iRet < 0)
					{
						m_dwLastErrorCode = NFC_NET_NETRECV_ERROR;
						m_netProtocol.DisconnectServer();
						m_nfcProtocol.CloseNFCDev();
						return -1;
					}

					if(packno == 1) // 身份证信息
					{
						memcpy(keys, netbuff, 8);

						cardInfoLen = iRet - 8;	
						memcpy(cardInfo, &netbuff[8], cardInfoLen);
						
						netbuff[0] = CLT_CMD_DATA_ACK1;
					}
					else if(packno == 2)            
					{

						piclen += iRet;
						memcpy(pic, netbuff, iRet);

						netbuff[0] = CLT_CMD_DATA_ACK2;
					}
					else if(packno == 3)
					{
						piclen += iRet;
						memcpy(&pic[514], netbuff, iRet);

						netbuff[0] = CTL_CMD_DATA_ACK3;

						loop_recv = 0;
					}

					netbuff[1] = 0x00;
					m_netProtocol.SendData(netbuff, 2);
				}
			}
			break;
		default:
			m_dwLastErrorCode = NFC_NET_INVALIDNETCMD;
			m_netProtocol.DisconnectServer();
			m_nfcProtocol.CloseNFCDev();
			return -1;
		}
	}

	// 解码
	callTeaByteIn(cardInfo, (int)keys, cardInfoLen);
	callTeaByteIn(pic, (int)keys, piclen);

	// 获取信息
	int i = 0;
	unsigned char buff1[30] = {0};//Name
	unsigned char buff2[92] = {0};//Sex, Nationality,  szBirth, szAddress
	unsigned char buff3[66] = {0};//ID, Issuing authority
	unsigned char buff4[68] = {0};//Expiration date
	
	offset = 0;
	memcpy(buff1, cardInfo, sizeof(buff1));
	offset += sizeof(buff1);
	
	memcpy(buff2, &cardInfo[offset], sizeof(buff2));
	offset += sizeof(buff2);
	
	memcpy(buff3, &cardInfo[offset], sizeof(buff3));
	offset += sizeof(buff3);
	
	memcpy(buff4, &cardInfo[offset], sizeof(buff4));
	offset += sizeof(buff4);

	// 获取姓名，去除空格
	for(i = 0; i < 30; i++)
	{
		if(buff1[i] == 0x20 && buff1[i + 1] == 0x00)
		{
			memcpy(pCardMsg->szName, buff1, i);
			break;
		}
	}

	// 获取性别
	offset = 0;
	memcpy(pCardMsg->szSex, buff2, 2);
	offset += 2;

	// 获取民族
	memcpy(pCardMsg->szNationality, &buff2[offset], 4);
	offset += 4;

	// 获取出生
	memcpy(pCardMsg->szBirth, &buff2[offset], 16);
	offset += 16;

	// 获取住址
	if(m_dwFunLimited) // 限制
	{
		memcpy(pCardMsg->szAddress, LIMITED_IFNO, sizeof(LIMITED_IFNO));		
	}
	else               // 没限制
	{
		for(i = 0; i < 70; i++)
		{
			if(buff2[offset + i] == 0x20 && buff2[offset + i + 1] == 0x00)
			{
				memcpy(pCardMsg->szAddress, &buff2[offset], i);
				break;
			}
		}
	}

	// 获取身份证号
	offset = 0;
	if(m_dwFunLimited) // 限制
	{
		memcpy(pCardMsg->szCardNo, LIMITED_IFNO, sizeof(LIMITED_IFNO));
	}
	else			   // 没限制
	{
		memcpy(pCardMsg->szCardNo, buff3, 36);
	}
	offset += 36;

	// 获取签发机关
	for(i = 0; i < 30; i++)
	{
		if(buff3[offset + i] == 0x20 && buff3[offset + i + 1] == 0x00)
		{
			memcpy(pCardMsg->szAuthority, &buff3[offset], i);
			break;
		}
	}

	// 获取有效期限
	for(i = 0; i < 68; i++)
	{
		if(buff4[i] == 0x20 && buff4[i + 1] == 0x00)
		{
			memcpy(pCardMsg->szPeriod, buff4, i);
			break;
		}
	}

	// 获取照片
	if(piclen == CARD_PHOTO_LEN)
	{
		memcpy(pCardMsg->szPhoto, pic, piclen);
		if(m_dwFunLimited)
		{
			char tmp[256] = {0};
			memcpy(&pCardMsg->szPhoto[256], tmp, 256);
		}
	}

	//add by andy 获取DN码
	memcpy(pCardMsg->szDn,DN,16);

	// 断开网络
	m_netProtocol.DisconnectServer();

	// 关闭本地设备
	m_nfcProtocol.CloseNFCDev();

	return NFC_NET_NFCREADCARD_SUCCESS;
}

int  CReadCard::GetDevSerialNo(char *pSerialNoBuff, int iBuffLen)
{
	m_dwLastErrorCode = NFC_NET_NOERROR;

	if(!m_nfcParam)
	{
		m_dwLastErrorCode = NFC_NET_UNKNOWN_ERROR;
		return -1;
	}

	struct SOTGParam otgParam;
	struct SSerParam serParam;
	struct SOTGParam_Android otgAndParam;
	char * nfcParam = NULL;
	if(m_iNFCType == NFC_OTG_DEV)                  // otg
	{
		otgParam = *(struct SOTGParam *)m_nfcParam;
		otgParam.speed = NFC_READEEPROM_SPEED;
		nfcParam = (char*)&otgParam;
	}
	else if(m_iNFCType == NFC_OTG_ANDROID_DEV)     // otg android
	{
		otgAndParam = *(struct SOTGParam_Android *)m_nfcParam;
		otgAndParam.speed = NFC_READEEPROM_SPEED;
		nfcParam = (char*)&otgAndParam;
	}
	else if(m_iNFCType == NFC_SER_DEV)            // serial
	{
		
		serParam = *(struct SSerParam *)m_nfcParam;
		serParam.speed = NFC_READEEPROM_SPEED;
		nfcParam = (char*)&serParam;
	}
	else                                          // unknown
	{
	    m_dwLastErrorCode = NFC_NET_UNKNOWN_ERROR;
		return -1;
	}

	int dev = OpenNFC(m_iNFCType, nfcParam);

	//if(dev < 0)
	if(dev == -1)
	{
		m_dwLastErrorCode = NFC_NET_NFCOPEN_ERROR;
		return -1;
	}

    unsigned char cmd[4] = {0x40, 0xA1, 0x01, 0x00};
	int  i = 0;
	int  ret = 0;
	for(i = 0; i < NFC_SERIALNO_LEN; i++)
	{
		cmd[2] = NFC_READEEPROM_OFFSETADDR + i;
		ret = WriteNFCData(dev, (char*)cmd, sizeof(cmd));
		if(ret <= 0)
		{
			CloseNFC(dev);
			m_dwLastErrorCode = NFC_NET_NFCOPEN_ERROR;
			return -1;

		}
		Sleep(5);
		ret = ReadNFCData(dev, pSerialNoBuff + i, 1);
		if(ret <= 0)
		{
			CloseNFC(dev);
			m_dwLastErrorCode = NFC_NET_NFCOPEN_ERROR;
			return -1;
		}
	}

	CloseNFC(dev);

	TeaDecrypt((unsigned int *)pSerialNoBuff, NFC_SERIALNO_LEN / 4,  (int)keys2);

	return 	NFC_NET_NOERROR;
}

int CReadCard::GetLastError()
{
	unsigned long dwErrorCode = m_dwLastErrorCode;
	m_dwLastErrorCode = NFC_NET_NOERROR;
	return dwErrorCode;
}

int  CReadCard::GetFunLimited()
{
	return m_dwFunLimited;
}

int CReadCard::SetSDKConfig(char *pConfigBuff, int iBuffLen)
{
	if(iBuffLen < 68)
	{
		return NFC_NET_UNKNOWN_ERROR;
	}
	keys1[0] = (unsigned char)pConfigBuff[0];
	keys1[4] = (unsigned char)pConfigBuff[1];
	keys1[8] = (unsigned char)pConfigBuff[2];
	keys1[12] = (unsigned char)pConfigBuff[3];

	SSDKConfig sdkConfig;

	memset(&sdkConfig, 0, sizeof(sdkConfig));
	memcpy(&sdkConfig, pConfigBuff + 4, sizeof(sdkConfig));
	TeaDecrypt((unsigned int*)&sdkConfig, sizeof(sdkConfig) / 4, (int)keys1);

	m_dwFunLimited = sdkConfig.bySDKLimited;
	m_dwNoSNVerify = sdkConfig.byNoSNVerify;

	return NFC_NET_NOERROR;
}

unsigned int CReadCard::TeaDecrypt(unsigned int *a1, unsigned int a2, int a3)
{
  unsigned int v3; // r6@1
  unsigned int result; // r0@1
  unsigned int v5; // r4@1
  unsigned int v6; // r12@1
  int v7; // r11@2
  int v8; // r3@3
  int v9; // r9@3
  unsigned int *v10; // r2@4
  int v11; // r5@5
  unsigned int *v12; // [sp+8h] [bp-30h]@1

  v12 = a1;
  v3 = *a1;
  result = 0x34 / a2 + 6;
  v5 = v3;
  v6 = -1640531527 * result;
  if ( -1640531527 * result )
  {
    v7 = a3;
    do
    {
      v8 = a2 - 1;
      v9 = (v6 >> 2) & 3;
      if ( (signed int)(a2 - 1) > 0 )
      {
	    v10 = &v12[a2 + 0x3FFFFFFF];
        do
        {
          v11 = ((*(unsigned long *)(4 * (v8-- & 3 ^ v9) + v7) ^ *(v10 - 1)) + (v6 ^ v5)) ^ ((16 * *(v10 - 1) ^ (v5 >> 3))
                                                                                    + (4 * v5 ^ (*(v10 - 1) >> 5)));
          v5 = *v10 - v11;
          *v10 = v5;
          --v10;
        }
        while ( v8 > 0 );
        v3 = *v12;
      }
      result = v5 >> 3;
      v5 = v3
         - (((16 * v12[a2 - 1] ^ (v5 >> 3)) + ((v12[a2 - 1] >> 5) ^ 4 * v5)) ^ ((*(unsigned long *)(4 * (v8 & 3 ^ v9) + v7) ^ v12[a2 - 1])
                                                                              + (v6 ^ v5)));
      v6 += 1640531527;
      v3 = v5;
      *v12 = v5;
    }
    while ( v6 );
  }
  return result;
}


int CReadCard::TeaEncrypt(unsigned int *a1, unsigned int a2, int a3)
{
	unsigned int *v3; // r5@1
	int v4; // r10@1
	unsigned int v5; // r4@1
	unsigned int v6; // r2@1
	unsigned int v7; // r9@1
	int v8; // r8@1
	int v9; // r12@2
	int v10; // r1@2
	int v11; // r0@3
	int v12; // r3@3
	unsigned int *v13; // r1@4
	int result; // r0@6
	signed int v15; // [sp+4h] [bp-34h]@1
	unsigned int *v16; // [sp+8h] [bp-30h]@1
	int v17; // [sp+Ch] [bp-2Ch]@1
	
	v16 = &a1[a2 - 1];
	v3 = a1;
	v4 = a3;
	v5 = *v16;
	v15 = 0x34 / a2 + 5;
	v6 = *v16;
	v7 = 0;
	v17 = (a2 - 1) & 3;
	v8 = a2 - 1;
	while ( 1 )
	{
		v7 -= 1640531527;
		v9 = (v7 >> 2) & 3;
		v10 = 0;
		if ( v8 )
		{
			v11 = 0;
			v12 = 0;
			do
			{
				v13 = &v3[v11];
				v5 = (((*(unsigned long *)(4 * (v11 & 3 ^ v9) + v4) ^ v5) + (v7 ^ v3[v11 + 1])) ^ ((16 * v5 ^ (v3[v11 + 1] >> 3))
					+ (4 * v3[v11 + 1] ^ (v5 >> 5))))
					+ *v13;
				++v12;
				*v13 = v5;
				v11 = v12;
			}
			while ( v12 != v8 );
			v10 = v17;
			v6 = *v16;
		}
		result = (*v3 >> 3) ^ 16 * v5;
		v5 = (((*(unsigned long *)(4 * (v10 ^ v9) + v4) ^ v5) + (v7 ^ *v3)) ^ (result + ((v5 >> 5) ^ 4 * *v3))) + v6;
		v6 = v5;
		*v16 = v5;
		if ( v15 <= 0 )
			break;
		--v15;
	}
	return result;
}

char * CReadCard::callTeaByteIn(const void *a1, int a2, signed int a3)
{
	signed int v3;  // r6@1
	const void *v4; // r4@1
	int v5;         // r5@1
	char *result;   // r0@1
	char s;   // [sp+4h] [bp-824h]@1
	int  v8;  // [sp+404h] [bp-424h]@1
	char v9;  // [sp+804h] [bp-24h]@1
	char v10; // [sp+805h] [bp-23h]@1
	char v11; // [sp+806h] [bp-22h]@1
	char v12; // [sp+807h] [bp-21h]@1
	char v13; // [sp+808h] [bp-20h]@1
	char v14; // [sp+809h] [bp-1Fh]@1
	char v15; // [sp+80Ah] [bp-1Eh]@1
	char v16; // [sp+80Bh] [bp-1Dh]@1
	int  v17; // [sp+80Ch] [bp-1Ch]@1
	
	v3 = a3;  // len
	v4 = a1;  // buff
	v5 = a2;  // keys

	char * pv5 = (char *)v5;
	char pv9[8] = {0};

	pv9[0] = *(unsigned char*)(pv5 + 3);
	pv9[1] = *(unsigned char*)(pv5 + 2);
	pv9[2] = *(unsigned char*)(pv5 + 1);
	pv9[3] = *(unsigned char*)pv5;

	pv9[4] = *(unsigned char*)(pv5 + 7);
	pv9[5] = *(unsigned char*)(pv5 + 6);
	pv9[6] = *(unsigned char*)(pv5 + 5);
	pv9[7] = *(unsigned char*)(pv5 + 4);

	*(unsigned long*)&keys[8] = *(unsigned long*)&pv9[4];
	*(unsigned long*)keys = *(unsigned long*)pv9;

	TeaDecrypt((unsigned int *)v4, v3 / 4, (int)keys);
	result = (char*)v4;
	return result;
}
