#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#ifdef WIN32
#else
#include <unistd.h>
#include <sys/ioctl.h>

#endif

#include "libnfc.h"
#include "otg.h"
#include "serial.h"

int g_nfctype = UNKNOWN_NFC;

int CheckNFC(int nfctype, char *param)
{
	int devfd = -1;
	if(!param)
	{
		return 0;
	}
	switch(nfctype)
	{
	case OTG_NFC:
		{
			struct SOTGParam * otgDev = (struct SOTGParam *)param;
			return CheckOTG(otgDev->vendorId, otgDev->productId);
		}
	case SER_NFC:
		{
			struct SSerParam * serParam = (struct SSerParam *)param;
			return CheckPort(serParam->devname);
		}
	case UNKNOWN_NFC:
	default:
		break;
	}
	return 0;
}


int OpenNFC(int nfctype, char *param)
{
	int devfd = -1;
	if(!param)
	{
		return devfd;
	}
	g_nfctype = nfctype;
	switch(nfctype)
	{
	case OTG_NFC:
		{
			struct SOTGParam * otgParam = (struct SOTGParam *)param;
			devfd = OpenOTG(otgParam->vendorId, otgParam->productId);
			if(devfd != -1)
			{
				SettingOTG(devfd, otgParam->speed, otgParam->databits, otgParam->stopbits, otgParam->parity, otgParam->flowcontrol);
			}
		}
		break;
	case OTG_NFC_ANDROID:
		{			
			struct SOTGParam_Android * otgParam = (struct SOTGParam_Android *)param;
			devfd = OpenOTG_Android(otgParam->devdescriptor, otgParam->devname);
			if(devfd != -1)
			{
				SettingOTG(devfd, otgParam->speed, otgParam->databits, otgParam->stopbits, otgParam->parity, otgParam->flowcontrol);
			}
		}
		break;
	case SER_NFC:
		{
			struct SSerParam * serParam = (struct SSerParam *)param;
			devfd = OpenPort(serParam->devname);
			if(devfd != -1)
			{
				SettingPort(devfd, serParam->speed, serParam->databits, serParam->stopbits, serParam->parity, serParam->flowcontrol);
			}
		}
		break;
	case UNKNOWN_NFC:
	default:
		break;
	}
	return devfd;
}

int CloseNFC(int devfd)
{
	switch(g_nfctype)
	{
	case OTG_NFC:
	case OTG_NFC_ANDROID:
		return CloseOTG(devfd);
	case SER_NFC:
		return ClosePort(devfd);
	default:
		break;
	}
	return -1;
}

int ReadNFCData(int devfd, char *buff,int len)
{
	switch(g_nfctype)
	{
	case OTG_NFC:
	case OTG_NFC_ANDROID:
		return ReadOTGData(devfd, buff, len);
	case SER_NFC:
		return ReadPortData(devfd, buff, len);
	default:
		break;
	}
	return -1;
}

int WriteNFCData(int devfd, char *buff,int len)
{
	switch(g_nfctype)
	{
	case OTG_NFC:
	case OTG_NFC_ANDROID:
		return WriteOTGData(devfd, buff, len);
	case SER_NFC:
		return WritePortData(devfd, buff, len);
	default:
		break;
	}
	return -1;
}
