#include "netprotocol.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//平台相关定
#ifdef WIN32
#include <io.h>
#include <Winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

#define MSG_NOSIGNAL 0

#define socklen_t int

#else  // linux
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <net/if.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> 


#define ioctlsocket ioctl
#define closesocket close

#define O_BINARY 0
#define INVALID_SOCKET (-1)

#endif 

#define NET_CON_TIMEOUT       5   //连接超时
#define NET_REC_TIMEOUT       5   //接收超时


CNetProtocol::CNetProtocol()
{
	m_iSock = -1;
}
CNetProtocol::~CNetProtocol()
{
}

int CNetProtocol::ConnectServer(char * pServerIP, unsigned short wServerPort)
{
	struct timeval tm;
	struct sockaddr_in addr;
	int sock;
	
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0);
	if(WSAStartup(wVersionRequested, &wsaData)!= 0)
	{
		return -1;
	}
#endif
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock != INVALID_SOCKET)
	{
		int  ret;
		unsigned long optval;
		
		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family   = AF_INET;
		addr.sin_addr.s_addr = inet_addr(pServerIP);
		addr.sin_port     = htons(wServerPort);
		
		optval = 1;
		ioctlsocket(sock, FIONBIO, &optval); 
		
		ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		
		if(ret < 0)
		{
			int error = -1;
			fd_set set;	
			socklen_t len;
			
#ifndef WIN32
			if (errno != EWOULDBLOCK && errno != EINPROGRESS)
			{
				closesocket(sock);
				return -1;
			}
#endif
			
			//设置连接超时
			tm.tv_sec  = NET_CON_TIMEOUT;
			tm.tv_usec = 0;
			FD_ZERO(&set);
			FD_SET(sock, &set);
			if(select(sock + 1, NULL,&set, NULL, &tm) > 0)
			{
				len = sizeof(int);
				getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
				if(error != 0)
				{
					closesocket(sock);
					return -1;
				}
			} 
			else
			{
				closesocket(sock);
				return -1;
			}
		}
		//设置SOCK为阻塞
		optval = 0;
		ioctlsocket(sock, FIONBIO, &optval); 

#ifdef WIN32
		tm.tv_sec = NET_REC_TIMEOUT * 1000;  
#else
		tm.tv_sec = NET_REC_TIMEOUT;  
#endif
        tm.tv_usec = 0; 
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tm, sizeof(tm));
		setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&tm, sizeof(tm));
	}

	m_iSock = sock;
	
	return sock;
}

int CNetProtocol::DisconnectServer()
{
	if(m_iSock != -1)
	{
		return closesocket(m_iSock);
	}

	return -1;
}



int CNetProtocol::RecvData(void * pdata, int len)
{
	int ret, count;
	
	if(!pdata)
	{
		return -1;
	}
	
	count = 0;
	if(len == 0)
	{
		return 0;
	}
	
	while(len)
	{
		if((ret = recv(m_iSock, (char*)pdata + count, len, 0)) <= 0)
		{
			return -1;
		}
		len  -= ret;
		count += ret;
	}
	return count;
}


int CNetProtocol::SendData(void * pdata, int len)
{
	
	int ret, count;
	
	if(!pdata)
	{
		return -1;
	}
	
	count = 0;
	if(len == 0)
	{
		return 0;
	}
	
	while(len > 0)
	{
		ret = send(m_iSock, (char*)pdata + count, len, MSG_NOSIGNAL);
		if(ret > 0)
		{
			len -= ret;
			count += ret;
		}
		else
		{
			return -1;
		}
	}
	
	return count;
}

int CNetProtocol::CopyNetData(int offset, int len, char *buff, char *desbuff, bool lenflat)
{
	char *tmp = desbuff;
	if(lenflat)
	{
		len += 1;
	}
	
	tmp[offset + 0] = 0xff & (len >> 8);
	tmp[offset + 1] = 0xff & (len >> 0);
	
	if(lenflat)
	{
		memcpy(tmp + offset + 2, buff, len - 1);
	}
	else
	{
		memcpy(tmp + offset + 2, buff, len);
	}
	
	offset += 2;
	offset += len;
	
	if(lenflat)
	{
		tmp[offset - 1] = 0x00;
	}
	return offset;
}


int CNetProtocol::BuildNetHead(int cmd, int packnum, int len, char *buff)
{
	buff[0] = cmd;
	buff[1] = packnum;
	buff[2] = 0xff & (len >> 8);
	buff[3] = 0xff & (len >> 0);
	
	len += 4;
	
	return len;
}

