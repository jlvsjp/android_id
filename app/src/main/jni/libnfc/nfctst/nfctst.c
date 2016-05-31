#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <signal.h>


#include "libnfc.h"

#define VENDOR_ID  0x1A86
//#define PRODUCT_ID 0x7523
#define PRODUCT_ID 0x5523

int g_run = 0;

void sig_term()
{
	printf("sig_term\n");
	g_run = 0;
}

void sig_int()
{
	printf("sig_int\n");
	g_run = 0;
}


int main(int argc, char * argv[])
{
	int nfc_type = UNKNOWN_NFC;
	int nfc_speed = 115200;

	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_int);
	g_run = 1;

	if(argc == 2)
	{
		nfc_speed = atol(argv[1]);
	}

#if 1
	struct SOTGParam param;

	memset(&param, 0 , sizeof(param));
	param.vendorId = VENDOR_ID;
	param.productId = PRODUCT_ID;
	param.speed = nfc_speed;
	param.databits = 8;
	param.stopbits = 1;
	param.parity = 'N';
	param.flowcontrol = 0;

	nfc_type = OTG_NFC;
#else
	struct SSerParam param;

	memset(&param, 0 , sizeof(param));
	strcpy(param.devname, "ttyUSB0");
	param.speed = nfc_speed;
	param.databits = 8;
	param.stopbits = 1;
	param.parity = 'N';
	param.flowcontrol = 0;

	nfc_type = SER_NFC;
#endif

	int fd = -1;


	if(CheckNFC(nfc_type, (char*)&param))
	{
		printf("nfc check ok\n");
	}
	else
	{
		printf("nfc check fail\n");
	}

	if(nfc_speed == 300)
	{
		unsigned char rd_cmd[4] = {0x40, 0xA1, 0x01, 0x00};
		unsigned char szSerialNo[16] = {0};
		int ret = 0;
		int i = 0;
		fd = OpenNFC(nfc_type, (char*)&param);
		for(i = 0; i < 12; i++)
		{
			rd_cmd[2] = 0x08 + i;
			ret = WriteNFCData(fd, (char*)rd_cmd, sizeof(rd_cmd));
			ret = ReadNFCData(fd, (char*)&szSerialNo[i], 1);
		}
		printf("serial no: ");
		for(i = 0; i < 12; i++)
		{
			printf("%02x ", szSerialNo[i]);
		}
		printf("\n");

		CloseNFC(fd);
		return 0;
	}

//	while(g_run)
	{
		fd = OpenNFC(nfc_type, (char*)&param);
		if(fd != -1)
		{
			printf("nfc dev open ok %x\n", fd);
		}
		else
		{
			printf("nfc dev open fail, again\n");
			return 0;
		}


		int num = 0;
		int err = 0;
		int i = 0;
		for(i = 0; i < 10; i++)
		{
			unsigned char buff[100] = {0};
			const unsigned char NFC_WAKEUP[] = {0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};

			int ret = -1;
			int  i = 0;
			num++;
			sprintf(buff, "test nfc:%d", num);

			printf("%s\n", buff);
			ret = WriteNFCData(fd, (char*)NFC_WAKEUP, sizeof(NFC_WAKEUP));
			if(ret < 0)
			{
				err++;
				if(err > 10)
				{
					printf("send fail, open dev again\n");
					CloseNFC(fd);
					break;
				}
			}

			printf("send[%d], ", ret);
			for( i = 0; i < ret; i++)
			{
				printf("%02x ", NFC_WAKEUP[i]);
			}
			printf("\n");
			usleep(500000);
			memset(buff, 0, sizeof(buff));
			ret = ReadNFCData(fd, (char*)buff, sizeof(buff));
			printf("recv[%d], ", ret);
			if(ret > 0)
			{
				for(i = 0; i < ret; i++)
				{
					printf("%02x ", buff[i]);
				}
			}
			printf("\n");
	
			fflush(stdout);


		}
	}

	if(fd != -1)
	{
		CloseNFC(fd);
	}

	return 0;
}



