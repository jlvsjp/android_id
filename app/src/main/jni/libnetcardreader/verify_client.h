#ifndef VERIFY_CLIENT_H
#define VERIFY_CLIENT_H


#define VERIFY_OK		0
#define VERIFY_FAIL		-1
#define VERIYF_TIMEOUT	-2
#define VERIFY_ERR		-3

#define MAXLINE 1024

int verify_serial(char *ip,int port,char *serial,int serial_len);
int sm4_serial_encode(char *serial, int serial_len, char * enc_buff);
#endif // VERIFY_CLIENT_H

