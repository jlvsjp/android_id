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

	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_int);
	g_run = 1;

	if(argc != 2)
	{
		printf("param fail\n");
		return 0;
	}

	struct SOTGParam_Android param;

	memset(&param, 0 , sizeof(param));

	int devfd = -1;
	devfd = open(argv[1], O_RDWR);
	if(devfd < 0)
	{
		printf("open %s fail\n", argv[1]);
		return 0;
	}
	else
	{
		printf("open %s ok\n", argv[1]);
	}
	param.devdescriptor = devfd;
	param.speed = 115200;
	param.databits = 8;
	param.stopbits = 1;
	param.parity = 'N';
	param.flowcontrol = 0;

	nfc_type = OTG_NFC_ANDROID;


	int fd = -1;

	{
		fd = OpenNFC(nfc_type, (char*)&param);
		if(fd != -1)
		{
			printf("nfc dev open ok %x\n", fd);
		}
		else
		{
			printf("nfc dev open fail, again\n");
			close(devfd);
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

	close(devfd);

	return 0;
}



