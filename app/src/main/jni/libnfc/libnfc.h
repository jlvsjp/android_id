#ifndef LIBNFC_H
#define LIBNFC_H

#ifdef __cplusplus
extern "C"
{
#endif

#define OTG_NFC          1        // OTG����NFC
#define SER_NFC          2        // ��������NFC
#define OTG_NFC_ANDROID  3        // OTG��׿NFC
#define UNKNOWN_NFC      0xff     // δ֪����NFC


//OTG����
struct SOTGParam{
	unsigned short vendorId;  // ����ID 
	unsigned short productId; // оƬID
	int speed;                // ͨ���ٶȣ�������
	int databits;             // ����λ
	int stopbits;             // ֹͣλ
	int parity;               // ��żλ
	int flowcontrol;          // ������
};

struct SOTGParam_Android{
	int  devdescriptor;       // �豸����
	char devname[64];         // �豸����
	int speed;                // ͨ���ٶȣ�������
	int databits;             // ����λ
	int stopbits;             // ֹͣλ
	int parity;               // ��żλ
	int flowcontrol;          // ������
};

//���ڲ���
struct SSerParam{
	char devname[64];         // �����豸����ttyUSB0
	int speed;                // ͨ���ٶȣ�������
	int databits;             // ����λ
	int stopbits;             // ֹͣλ
	int parity;               // ��żλ
	int flowcontrol;          // ������
};


/* ���NFC
 * �ɹ�����1��ʧ�ܷ���0	
 */
int CheckNFC(int nfctype, char *param);

/* ��NFC
 * �ɹ������豸�����ʧ�ܷ���-1	
 */
int OpenNFC(int nfctype, char *param);

/* �ر�NFC
 * �ɹ�����0��ʧ�ܷ���-1	
 */
int CloseNFC(int devfd);

/* ��NFC����
 * �ɹ����ض�ȡ�ֽ�����ʧ�ܷ���-1
 */
int ReadNFCData(int devfd, char *buff,int len);

/* дNFC����
 * �ɹ�����д���ֽ�����ʧ�ܷ���-1
 */
int WriteNFCData(int devfd, char *buff,int len);
	
#ifdef __cplusplus
}
#endif

#endif
