//#include <TIME.H>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "netprotocol.h"

#include "sm4.h"
#include "verify_client.h"


//ÃÜÔ¿
static unsigned char key[16] = {0x01,0x02,0x03,0x04,0x06,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16};


static int getRand(char *rand)
{
	
    time_t now_time;
	now_time = time(NULL);

    struct tm *r_tm;

	r_tm = gmtime(&now_time);

	sprintf(rand,"%02d%02d%02d",r_tm->tm_hour,r_tm->tm_min,r_tm->tm_sec);
	
	 return 6;
}

#define SM4_LEN 64
int verify_serial(char *ip,int port,char *serial,int serial_len)
{
	char rand[12] = {0};
	int rand_len = 0;
	rand_len = getRand(rand);


	unsigned char text[SM4_LEN] = {0};
	memcpy(text,rand,rand_len);
	memcpy(text+rand_len,serial,serial_len);

	unsigned char encode_data[SM4_LEN] = {0};
	int elen = SM4_LEN;
	sm4_context ctx;
	sm4_setkey_enc(&ctx,key);
	sm4_crypt_ecb(&ctx,1,elen,text,encode_data);
 
	CNetProtocol AuthServer;
	int sockfd = AuthServer.ConnectServer(ip,port);
	if(sockfd < 0)
	{
		return -1;
	} else {
	   AuthServer.SendData((void *)encode_data,elen); 
   	}
   	unsigned char rdata[SM4_LEN] = {0};
	int rlen = AuthServer.RecvData((void *)rdata, SM4_LEN);
	AuthServer.DisconnectServer();

    if(rlen != SM4_LEN)
        return -1;

   unsigned char res[SM4_LEN] = {0};

   sm4_setkey_dec(&ctx,key);
   sm4_crypt_ecb(&ctx,0,rlen,rdata,res);

    if(strstr((char *)res,"success"))
	{
        return 0;
	}
	else
	{
        return -1;
	}
	
    return 0;
}

int sm4_serial_encode(char *serial, int serial_len, char * enc_buff)
{
	char rand[12] = {0};
	int rand_len = 0;

	if(!serial || !enc_buff)
	{
		return -1;
	}

	rand_len = getRand(rand);

	if((serial_len + rand_len) > SM4_LEN)
	{
		return -1;
	}
		
	unsigned char text[SM4_LEN] = {0};
	memcpy(text, rand, rand_len);
	memcpy(text + rand_len, serial, serial_len);
	
	int elen = SM4_LEN;
	sm4_context ctx;
	sm4_setkey_enc(&ctx, key);
	sm4_crypt_ecb(&ctx, 1, elen, text, (unsigned char *)enc_buff);

	return 0;
}



