#ifndef NETPROTOCOL_H
#define NETPROTOCOL_H

#define SVR_CMDTYPE_SENDCDM         0x00
#define SVR_CMDTYPE_SENDDATA        0x01

#define SVR_CMD_INCERT         0x01
#define SVR_CMD_OUTCERT        0x02
#define SVR_CMD_MSG_ACK        0x03
#define SVR_CMD_RDPHOTO_ACK1   0x24
#define SVR_CMD_RDPHOTO_ACK2   0x25
#define SVR_CMD_RDFINGERPRINT_ACK1   0x28
#define SVR_CMD_RDFINGERPRINT_ACK2   0x29
#define SVR_CMD_SENDDATA             0x20

#define CLT_CMD_STARTRDCARD_ACK      0x11
#define CLT_CMD_INAUTH_ACK           0x12
#define CLT_CMD_OUTAUTH_ACK          0x13
#define CLT_CMD_SENDPHOTO1           0x24
#define CLT_CMD_SENDPHOTO2           0x25
#define CLT_CMD_SENDFINGERPRINT1     0x28
#define CLT_CMD_SENDFINGERPRINT2     0x29

#define CLT_CMD_VERIFY_SERIAL        0x59

#define CLT_CMD_DATA_ACK1     0x41
#define CLT_CMD_DATA_ACK2     0x42
#define CTL_CMD_DATA_ACK3     0x43

#define CLT_CMD_SECNED_ACK           0x21


struct SSvrCmd
{
	unsigned char type;
	unsigned char cmd;
};

class CNetProtocol
{
public:
	CNetProtocol();
	~CNetProtocol();

	int ConnectServer(char * pServerIP, unsigned short wServerPort);	
	int DisconnectServer();
	int RecvData(void * pdata, int len);	
	int SendData(void * pdata, int len);
	int CopyNetData(int offset, int len, char *buff, char *desbuff, bool lenflat = true);
	int BuildNetHead(int cmd, int packnum, int len, char *buff);

private:
	int m_iSock;

};

#endif