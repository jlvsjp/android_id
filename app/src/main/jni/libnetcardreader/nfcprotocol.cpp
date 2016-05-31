#include "nfcprotocol.h"
#include "../libnfc/libnfc.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <time.h>


#ifdef WIN32    // windows
#include <windows.h>

#define TRACE
#else           // linux
#include <unistd.h>

#define Sleep(a)   usleep(a*1000)

#if 0
#define TRACE printf 
#else
#define TRACE
#endif

#endif


#define RECV_TIMEOUT 2 // sec



// PN532相关定义

#define PN532_PREAMBLE 		0x00
#define PN532_STARTCODE1 	0x00
#define PN532_STARTCODE2 	0xFF
#define PN532_POSTAMBLE 	0x00

#define PN532_HOSTTOPN532 	0xD4

#define PN532_FIRMWAREVERSION 	0x02
#define PN532_GETGENERALSTATUS 	0x04
#define PN532_SAMCONFIGURATION  0x14
#define PN532_INLISTPASSIVETARGET 0x4A
#define PN532_INDATAEXCHANGE 	0x40
#define PN532_INJUMPFORDEP 	0x56
#define PN532_TGINITASTARGET 	0x8C
#define PN532_TGGETDATA 	0x86
#define PN532_TGSETDATA 	0x8E

#define PN532_MIFARE_READ 	0x30
#define PN532_MIFARE_WRITE 	0xA0

#define PN532_AUTH_WITH_KEYA 	0x60
#define PN532_AUTH_WITH_KEYB 	0x61

#define PN532_WAKEUP 		0x55

#define  PN532_SPI_STATREAD  	0x02
#define  PN532_SPI_DATAWRITE 	0x01
#define  PN532_SPI_DATAREAD  	0x03

#define  PN532_SPI_READY  	0x01

#define PN532_MIFARE_ISO14443A 	0x0


// Register addresses
#define PN53X_REG_Control_switch_rng 0x6106
#define PN53X_REG_CIU_Mode 0x6301
#define PN53X_REG_CIU_TxMode 0x6302
#define PN53X_REG_CIU_RxMode 0x6303
#define PN53X_REG_CIU_TxControl 0x6304
#define PN53X_REG_CIU_TxAuto 0x6305
#define PN53X_REG_CIU_TxSel 0x6306
#define PN53X_REG_CIU_RxSel 0x6307
#define PN53X_REG_CIU_RxThreshold 0x6308
#define PN53X_REG_CIU_Demod 0x6309
#define PN53X_REG_CIU_FelNFC1 0x630A
#define PN53X_REG_CIU_FelNFC2 0x630B
#define PN53X_REG_CIU_MifNFC 0x630C
#define PN53X_REG_CIU_ManualRCV 0x630D
#define PN53X_REG_CIU_TypeB 0x630E
// #define PN53X_REG_- 0x630F
// #define PN53X_REG_- 0x6310
#define PN53X_REG_CIU_CRCResultMSB 0x6311
#define PN53X_REG_CIU_CRCResultLSB 0x6312
#define PN53X_REG_CIU_GsNOFF 0x6313
#define PN53X_REG_CIU_ModWidth 0x6314
#define PN53X_REG_CIU_TxBitPhase 0x6315
#define PN53X_REG_CIU_RFCfg 0x6316
#define PN53X_REG_CIU_GsNOn 0x6317
#define PN53X_REG_CIU_CWGsP 0x6318
#define PN53X_REG_CIU_ModGsP 0x6319
#define PN53X_REG_CIU_TMode 0x631A
#define PN53X_REG_CIU_TPrescaler 0x631B
#define PN53X_REG_CIU_TReloadVal_hi 0x631C
#define PN53X_REG_CIU_TReloadVal_lo 0x631D
#define PN53X_REG_CIU_TCounterVal_hi 0x631E
#define PN53X_REG_CIU_TCounterVal_lo 0x631F
// #define PN53X_REG_- 0x6320
#define PN53X_REG_CIU_TestSel1 0x6321
#define PN53X_REG_CIU_TestSel2 0x6322
#define PN53X_REG_CIU_TestPinEn 0x6323
#define PN53X_REG_CIU_TestPinValue 0x6324
#define PN53X_REG_CIU_TestBus 0x6325
#define PN53X_REG_CIU_AutoTest 0x6326
#define PN53X_REG_CIU_Version 0x6327
#define PN53X_REG_CIU_AnalogTest 0x6328
#define PN53X_REG_CIU_TestDAC1 0x6329
#define PN53X_REG_CIU_TestDAC2 0x632A
#define PN53X_REG_CIU_TestADC 0x632B
// #define PN53X_REG_- 0x632C
// #define PN53X_REG_- 0x632D
// #define PN53X_REG_- 0x632E
#define PN53X_REG_CIU_RFlevelDet 0x632F
#define PN53X_REG_CIU_SIC_CLK_en 0x6330
#define PN53X_REG_CIU_Command 0x6331
#define PN53X_REG_CIU_CommIEn 0x6332
#define PN53X_REG_CIU_DivIEn 0x6333
#define PN53X_REG_CIU_CommIrq 0x6334
#define PN53X_REG_CIU_DivIrq 0x6335
#define PN53X_REG_CIU_Error 0x6336
#define PN53X_REG_CIU_Status1 0x6337
#define PN53X_REG_CIU_Status2 0x6338
#define PN53X_REG_CIU_FIFOData 0x6339
#define PN53X_REG_CIU_FIFOLevel 0x633A
#define PN53X_REG_CIU_WaterLevel 0x633B
#define PN53X_REG_CIU_Control 0x633C
#define PN53X_REG_CIU_BitFraming 0x633D
#define PN53X_REG_CIU_Coll 0x633E

//唤醒模块
const char NFC_WAKEUP[] = {0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};

//NFC寄存器配置
const char NFC_REG_CONF[] = 
{
0x08, 
0x63, 0x01, 0xFF,
0x63, 0x02, 0x83,
0x63, 0x03, 0x83,
0x63, 0x05, 0x00,
0x63, 0x08, 0x4D,
0x63, 0x09, 0x4D,
0x63, 0x0D, 0x10,
0x63, 0x0E, 0x03,
0x63, 0x14, 0x68,
0x63, 0x17, 0xFF,
0x63, 0x18, 0x3F,
0x63, 0x19, 0x18,
0x63, 0x3C, 0x10,
};

const char NFC_ACK[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};      //ACK
const char NFC_FRM_HEAD[] = {0x00, 0x00, 0xFF};                   //帧头

#define NFC_START_LEN (sizeof(NFC_ACK) + sizeof(NFC_FRM_HEAD))
#define NFC_FRAME_LEN(a) (sizeof(NFC_FRM_HEAD) + 2 + a + 1 + 1)   //NFC帧长度： frame head | len | data | chksum | 00
#define NFC_ACK_FRAME_LEN(a) (sizeof(NFC_ACK) + NFC_FRAME_LEN(a)) //ACK + 帧头总长度


CNFCProtocol::CNFCProtocol()
{
	m_nfcDev = -1;
}

CNFCProtocol::~CNFCProtocol()
{

}

int CNFCProtocol::CheckNFCDev(int nfcType, char * nfcDev)
{
	if(CheckNFC(nfcType, nfcDev))
	{
		int dev = OpenNFC(nfcType, nfcDev);
		if(dev != -1)
		{
			unsigned char buff[64] = {0};
			int iRet = 0;
			int iLen = 0;
			//唤醒
			WriteNFCData(dev, (char*)NFC_WAKEUP, sizeof(NFC_WAKEUP));
			Sleep(10);
			iRet = ReadNFCData(dev, (char*)buff, sizeof(buff));
		    iLen = buff[NFC_START_LEN]; // 数据长度 00 00 ff | len lensum |
			
			CloseNFC(dev);
			if(iRet == NFC_ACK_FRAME_LEN(iLen))
			{
				unsigned char sum = buff[iRet - 2];
				
				if(!CheckSum(buff + sizeof(NFC_ACK), iRet - sizeof(NFC_ACK), sum))
				{
					return 0;   // 校验和错误
				}
				
				return 1;		// 是NFC设备		
			}
		}
	}

	return 0; 
}


int CNFCProtocol::OpenNFCDev(int nfcType, char * nfcDev)
{

	m_nfcDev = OpenNFC(nfcType, nfcDev);

	return m_nfcDev;
}

int CNFCProtocol::CloseNFCDev()
{
	return CloseNFC(m_nfcDev);

}

void CNFCProtocol::printf_frame(unsigned char *buff, unsigned char len)
{
	for(int i = 0; i < len; i++)
	{
		TRACE("%02x ", buff[i]);
	}
	
	TRACE("\n");
}

unsigned char  CNFCProtocol::buildCommand(unsigned char* cmd, unsigned char cmd_len, unsigned char * des_buf)
{
	unsigned char checksum;
	unsigned char cmdlen_1;
	unsigned char i;
	unsigned char checksum_1;
	
	cmd_len++;
	
	
	checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
	
	des_buf[0] = PN532_PREAMBLE;    //0x00
	des_buf[1] = PN532_PREAMBLE;    //0x00
	des_buf[2] = PN532_STARTCODE2;  //0xff
	des_buf[3] = cmd_len;
	
	cmdlen_1=~cmd_len + 1;
	des_buf[4] = cmdlen_1;
	
	des_buf[5] = PN532_HOSTTOPN532;	//0xd4
	
	checksum += PN532_HOSTTOPN532;
	
	
	for (i=0; i<cmd_len-1; i++) 
	{
		
		des_buf[6 + i] = cmd[i];
		checksum += cmd[i];
	}
	
	checksum_1=~checksum;
	des_buf[6 + cmd_len - 1] = checksum_1;
	des_buf[6 + cmd_len] = PN532_POSTAMBLE;
	
	return (6 + cmd_len + 1);
}

int CNFCProtocol::InitNFC()
{
	unsigned char buff[64] = {0};
	unsigned char cmd[64] = {0};
	int len = 0;
	int ret = 0;

	//唤醒
	WriteNFCData(m_nfcDev, (char*)NFC_WAKEUP, sizeof(NFC_WAKEUP));

	Sleep(10);
	ret = ReadNFCData(m_nfcDev, (char*)buff, sizeof(buff));

	printf_frame(buff, ret);

	//配置参数
	cmd[0] = 0x12;
	cmd[1] = 0x14;
	memset(buff, 0, sizeof(buff));
	len = buildCommand(cmd, 2, buff);
	WriteNFCData(m_nfcDev, (char*)buff, len);
	Sleep(10);

	memset(buff, 0, sizeof(buff));
	ret = ReadNFCData(m_nfcDev, (char*)buff, sizeof(buff));
	printf_frame(buff, ret);

	//打开RF
	cmd[0] = 0x32;
	cmd[1] = 0x01;
	cmd[2] = 0x01;
	memset(buff, 0, sizeof(buff));
	len = buildCommand(cmd, 3, buff);
	WriteNFCData(m_nfcDev, (char*)buff, len);
	Sleep(10);
	
	memset(buff, 0, sizeof(buff));
	ret = ReadNFCData(m_nfcDev, (char*)buff, sizeof(buff));
	printf_frame(buff, ret);

	//RF参数
	cmd[0] = 0x32;
	cmd[1] = 0x05;
	cmd[2] = 0xff;
	cmd[3] = 0xff;
	cmd[4] = 0xff;
	memset(buff, 0, sizeof(buff));
	len = buildCommand(cmd, 5, buff);
	WriteNFCData(m_nfcDev, (char*)buff, len);
	Sleep(10);
	
	memset(buff, 0, sizeof(buff));
	ret = ReadNFCData(m_nfcDev, (char*)buff, sizeof(buff));
	printf_frame(buff, ret);

	//寄存器设置
	memset(buff, 0, sizeof(buff));
	len = buildCommand((unsigned char*)NFC_REG_CONF, sizeof(NFC_REG_CONF), buff);
	WriteNFCData(m_nfcDev, (char*)buff, len);
	Sleep(10);
	
	memset(buff, 0, sizeof(buff));
	ret = ReadNFCData(m_nfcDev, (char*)buff, sizeof(buff));
	printf_frame(buff, ret);

	return 0;
}

int CNFCProtocol::CheckSum(unsigned char * buff, int len, unsigned char chksum)
{
	unsigned char sum = 0;
	sum += buff[0];
	sum += buff[1];
	sum += buff[2];
	len -= 2;
	for(int i = 5; i < len; i++)
	{
		sum += buff[i];
	}

	sum = ~sum;

	if(chksum == sum)
	{
		return 1;
	}

	return 0;
}

int CNFCProtocol::CmdRequest(int cmdno, int p1, int p2, char * recvbuff, int bufflen)
{
	unsigned char buff[156] = {0};
	unsigned char cmd[64] = {0};
	int ret = 0;
	int len = 0;

	cmd[0] = 0x42;
	len += 1;
	switch(cmdno)
	{
	case NFC_CMD_REQB:
		cmd[1] = NFC_CMD_REQB;
		cmd[2] = 0x00;
		cmd[3] = 0x00;
		len += 3;
		break;
	case NFC_CMD_NOATTRIB:
		cmd[1] = NFC_CMD_NOATTRIB;
		cmd[2] = 0x00;
		cmd[3] = 0x00;
		cmd[4] = 0x00;
		cmd[5] = 0x00;
		cmd[6] = 0x00;
		cmd[7] = 0x08;
		cmd[8] = 0x01;
		cmd[9] = 0x08;
		len += 9;
		break;
	case NFC_CMD_SELFILE:
		cmd[1] = 0x00;
		cmd[2] = NFC_CMD_SELFILE;;
		cmd[3] = 0x00;
		cmd[4] = 0x00;
		cmd[5] = 0x02;
		cmd[6] = 0xff & (p1 >> 8);
		cmd[7] = 0xff & (p1 >> 0);
		len += 7;
		break;
	case NFC_CMD_RDFILE:
		cmd[1] = 0x80;
		cmd[2] = NFC_CMD_RDFILE;;
		cmd[3] = 0xff & (p1 >> 8);
		cmd[4] = 0xff & (p1 >> 0);
		cmd[5] = p2;
		len += 5;
		break;
	case NFC_CMD_SELRDFILE:
		{
			int len = p2;
			int off = 0; 
			if(CmdRequest(NFC_CMD_SELFILE, p1, 0, recvbuff, bufflen) < 0)
			{
				TRACE("select file %x error\n", p1);
				return -1;
			}
			memset(recvbuff, 0, bufflen);
			while(len)
			{
				int rdlen = len > 0x70 ? 0x70 : len;
				ret = CmdRequest(NFC_CMD_RDFILE, off, rdlen, recvbuff + off, bufflen - off);
				if(ret != rdlen + 2) // 含状态码 90 00
				{
					TRACE("read file fail\n");
					return -1;
				}
				ret -= 2;  // 去除90 00
				len -= ret;
				off += ret;
			}
			return off;
		}
		break;
	case NFC_CMD_INCERT: 
		memcpy(&cmd[1], (char*)p1, p2);
		len += p2;
		break;
	case NFC_CMD_OUTCERT: 
		memcpy(&cmd[1], (char*)p1, p2);
		len += p2;
		break;
	case NFC_CMD_RDRAND:
		cmd[1] = 0x00;
		cmd[2] = NFC_CMD_RDRAND;;
		cmd[3] = 0x00;
		cmd[4] = 0x00;
		cmd[5] = p1;
		len += 5;
		break;
	case NFC_CMD_RDUID: 
		cmd[1] = 0x00;
		cmd[2] = NFC_CMD_RDUID;
		cmd[3] = 0x00;
		cmd[4] = 0x00;
		cmd[5] = p1;
		len += 5;
		break;
	default:
			break;
	}
	len = buildCommand(cmd, len, buff);
	ret = WriteNFCData(m_nfcDev, (char*)buff, len);

	TRACE("\nsnd>[%dB]:", ret);
printf_frame(buff, ret);

//	Sleep(20);
	memset(buff, 0, sizeof(buff));

	len = 0;
	int off = 0;

	int start = time(NULL);
	while(1)
	{
		ret = ReadNFCData(m_nfcDev, (char*)buff + off, sizeof(buff));

		if(ret <= 0)
		{
			int end = time(NULL);
			if((end - start) >= RECV_TIMEOUT)
			{
				TRACE("read data timeout\n");
				return -1;
			}
		}

		off += ret;

		if(off > NFC_START_LEN)
		{
			len = buff[NFC_START_LEN]; // 数据长度 00 00 ff | len lensum |
		}

		if(len > 0 && off == NFC_ACK_FRAME_LEN(len))
		{
			unsigned char sum = buff[off - 2];

TRACE("\nrcv>[%dB]:", off);
			printf_frame(buff, off);

			if(!CheckSum(buff + sizeof(NFC_ACK), off - sizeof(NFC_ACK), sum))
			{
				TRACE("check sum error\n");
				return -1;
			}

			if(memcmp(buff, NFC_ACK, sizeof(NFC_ACK)) == 0)
			{
				off = sizeof(NFC_ACK);
				if(memcmp(buff + off, NFC_FRM_HEAD, sizeof(NFC_FRM_HEAD)) == 0)
				{
					off +=  sizeof(NFC_FRM_HEAD);
					len -= 3;
					off += 5;
					memcpy(recvbuff, buff + off, len);

					printf_frame((unsigned char*)recvbuff, len);
					
					return len;
				}
				else
				{
					TRACE("frame header error\n");
					return -1;
				}
			}
			else
			{
				TRACE("ack error\n");
				return -1;
			}
		}

		if(off >= sizeof(buff))
		{
			TRACE("buff is full\n");
			return -1;
		}
	}

	return -1;
}

