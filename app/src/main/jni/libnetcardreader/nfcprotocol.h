#ifndef NFC_PROTOCOL_H
#define NFC_PROTOCOL_H



//NFC�����-----------------------|��������-------|����p1--------|����p2-----------|��ע----------------
#define NFC_CMD_REQB      0x05      //ѡB��        |    ��        |     ��           |
#define NFC_CMD_NOATTRIB  0x1D      //����         |    ��        |     ��           |               

//APDU����
#define NFC_CMD_SELFILE   0xA4      //ѡ���ļ�     |   �ļ�����   |     ��           |         
#define NFC_CMD_RDFILE    0xB0      //���ļ�       |   ƫ�Ƶ�ַ   |     ����         |
#define NFC_CMD_INCERT    0x88      //�ڲ���֤     |   �����ַ   |     ����         |
#define NFC_CMD_OUTCERT   0x82      //�ⲿ��֤     |   �����ַ   |     ����         |
#define NFC_CMD_RDRAND    0x84      //�������     |   ��������� |     ��           |
#define NFC_CMD_RDUID     0x36      //��UID        |   UID����    |     ��           |

//�������
#define NFC_CMD_SELRDFILE 0xA4B0    //ѡ�񲢶��ļ� |   �ļ�����   |     ��ȡ����     |  ��ģʽΪ������ȡ����ȥ��90 00

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