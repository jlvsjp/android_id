#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef WIN32
#include "pcomm.h"
#else
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#define FAIL (-1)

int OpenPort(char *pSerNum)
{
	char buff[20];
	int devfd = -1;

	if(!pSerNum)
	{
		printf("pSerNum is null!\n");
		return -1;
	}

	memset(buff,0,sizeof(buff));

#ifdef WIN32
	{
		char *p = strstr(pSerNum, "COM");
		if(p)
		{
			int port = 0;
			p += 3;
			port = atol(p);

			if(sio_open(port) != 0)
			{
				return -1;
			}
			devfd = port;
		}
	}

#else
	sprintf(buff,"/dev/%s", pSerNum);	
	devfd = open(buff, O_RDWR | O_NOCTTY | O_NDELAY);
	if(devfd == FAIL)
    {
		printf("opene serial %s fail\n",buff);
    }
	else
    {
		printf("open serial %s successful:%d\n",buff,devfd);
    }
#endif

	return devfd;	
}

int ClosePort(int devfd)
{
	int ret = FAIL;
	if(devfd!=FAIL)
	{
#ifdef WIN32
		ret = sio_close(devfd);
#else
		ret = close(devfd);
#endif
		devfd = FAIL;
	}
	return ret;
}

//设置串口属性
int SettingPort(int devfd, int speed, int databits, int stopbits, int parity, int flowcontrol)
{
#ifdef WIN32
	int mask = 0;

	switch (databits) 
	{  
	case 5:		
		mask |= BIT_5;
		break;
	case 6:		
		mask |= BIT_6;
		break;
	case 7:		
		mask |= BIT_7;
		break;
	case 8:		
		mask |= BIT_8;
		break;
	default:    
		mask |= BIT_8;
		break;
	}

	switch (parity)
	{
	case 'n':
	case 'N':
		mask |= P_NONE;
		break;
	case 'o':
	case 'O':
		mask |= P_ODD;
		break;  
	case 'e':  
	case 'E':   
		mask |= P_EVEN;
		break;
	case 's': 
	case 'S': 
		mask |= P_SPC;
		break;  
	default:   
		mask |= P_NONE;
		break;
	} 


	switch (stopbits)
	{   
	case 1:
		mask |= STOP_1;
		break;  
	case 2:    
		mask |= STOP_2;
		break;
	default:    
		mask |= STOP_1;
		break;
	}

	switch(speed)
	{
	case 300:
		speed=B300;
		break;
	case 600:
		speed=B600;
		break;
	case 1200:
		speed=B1200;
		break;
	case 2400:
		speed=B2400;
		break;
	case 4800:
		speed=B4800;
		break;
	case 9600:
		speed=B9600;
		break;
	case 19200:
		speed=B19200;
		break;
	case 38400:
		speed=B19200;
		break;
	case 57600:
		speed=B19200;
		break;
	case 115200:
		speed=B115200;
		break;
	case 230400:
		speed=B230400;
		break;
	case 460800:
		speed=B460800;
		break;		
	case 921600:
		speed=B921600;
		break;		
	default:
		speed=B9600;
		break;
	}

	sio_ioctl(devfd, speed, mask);

#else
	struct termios options;
	if(tcgetattr(devfd,&options) != 0)
	{ 
		return FAIL;  
	}	
	
	options.c_cflag |= (CLOCAL | CREAD);
	switch(speed)
	{
	case 300:
		speed=B300;
		break;
	case 600:
		speed=B600;
		break;
	case 1200:
		speed=B1200;
		break;
	case 2400:
		speed=B2400;
		break;
	case 4800:
		speed=B4800;
		break;
	case 9600:
		speed=B9600;
		break;
	case 19200:
		speed=B19200;
		break;
	case 38400:
		speed=B19200;
		break;
	case 57600:
		speed=B19200;
		break;
	case 115200:
		speed=B115200;
		break;
	case 230400:
		speed=B230400;
		break;
	case 460800:
		speed=B460800;
		break;		
	case 500000:
		speed=B500000;
		break;		
	case 576000:
		speed=B576000;
		break;		
	case 921600:
		speed=B921600;
		break;		
	case 1000000:
		speed=B1000000;
		break;		
	case 1152000:
		speed=B1152000;
		break;
	default:
		speed=B9600;
		break;
	}
	
	cfsetispeed(&options, speed);  
	cfsetospeed(&options, speed); 
	options.c_cflag &= ~CSIZE; 
	switch (databits) 
	{  
	case 5:		
		options.c_cflag |= CS5; 
		break;
	case 6:		
		options.c_cflag |= CS6; 
		break;
	case 7:		
		options.c_cflag |= CS7; 
		break;
	case 8:		
		options.c_cflag |= CS8; 
		break;
	default:    
		options.c_cflag |= CS8;
	}
		
	options.c_iflag &= ~ICRNL;
	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O':
		options.c_cflag |= PARENB;
		options.c_cflag |= PARODD;		
		options.c_iflag |= INPCK;
		break;  
	case 'e':  
	case 'E':   
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's': 
	case 'S': 
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_iflag |= INPCK;
		break;  
	default:   
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
	}  
	switch (stopbits)
	{   
	case 1:
		options.c_cflag &= ~CSTOPB;  
		break;  
	case 2:    
		options.c_cflag |= CSTOPB;  
		break;
	default:    
		options.c_cflag &= ~CSTOPB;  
	}
	tcflush(devfd,TCIOFLUSH);  
	options.c_lflag &= ~(ICANON | ECHO |ECHOE| ISIG);
	options.c_oflag &= ~OPOST; 
	options.c_iflag = 0;	
	options.c_cc[VINTR] = 0; //非中断模式

	options.c_cc[VMIN] = 0;  //非阻塞模式

	if(tcsetattr(devfd, TCSANOW, &options) != 0)
	{        
		printf("set fail\n");
		return FAIL;     
	}    
	tcflush(devfd,TCIOFLUSH);   

#endif
	return 0;
}

int WritePortData(int devfd, char *buff,int len)
{
	if(devfd==FAIL)
	{
		return FAIL;
	}
#ifdef WIN32
	return sio_write(devfd, buff, len);;
#else
	return write(devfd,buff,len);
#endif
}

int ReadPortData(int devfd, char *buff,int len)
{
	if(devfd==FAIL)
	{
		return FAIL;
	}
#ifdef WIN32
	return sio_read(devfd, buff, len);
#else
	return read(devfd, buff, len);
#endif
}

int CheckPort(char *pSerNum)
{
	char buff[20];
	int devfd = FAIL;
	
	if(!pSerNum)
	{
		return 0;
	}
	
	memset(buff,0,sizeof(buff));
	
#ifdef WIN32
	{
		char *p = strstr(pSerNum, "COM");
		if(!p)
		{
			p = strstr(pSerNum, "com");
		}
		if(p)
		{
			int port = 0;
			p += 3;
			port = atol(p);
			
			if(sio_open(port) == 0)
			{
				devfd = port;
				sio_close(port);
			}
		}
	}
	
#else
	sprintf(buff,"/dev/%s", pSerNum);	
	devfd = open(buff, O_RDWR | O_NOCTTY | O_NDELAY);
	if(devfd != FAIL)
    {
		close(devfd);
    }
#endif
	
	return (devfd != FAIL);	
}
