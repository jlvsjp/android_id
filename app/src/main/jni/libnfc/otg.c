#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#ifdef WIN32
#else
#include <unistd.h>
#include <pthread.h>
#include <usbhost/usbhost.h>
#include <linux/usb/f_accessory.h>
#endif

#include "otg.h"

enum UartCmd{
	UCMD_VENDOR_WRITE_TYPE  = 0x40,  
	UCMD_VENDOR_READ_TYPE   = 0xC0,   
	UCMD_VENDOR_READ        = 0x95,        
	UCMD_VENDOR_WRITE       = 0x9A,       
	UCMD_VENDOR_SERIAL_INIT = 0xA1, 
	UCMD_VENDOR_MODEM_OUT   = 0xA4,   
	UCMD_VENDOR_VERSION     = 0x5F, 
	UCMD_VENDOR_EEPROM_READ = 0x54,
};

#define USB_RECIP_DEVICE 0x00    
#define DEFAULT_TIMEOUT  500

struct otg_dev
{
	struct usb_device * device;
	int rd_endpoint;
	int wr_endpoint;
};


struct otg_dev * g_sOTGDevice = NULL;
unsigned short g_vendorId = 0;
unsigned short g_productId = 0;

char g_otg_buff[64] = {0};
int  g_otg_bufflen = 0;
unsigned long g_otg_seep = 0;

unsigned char g_otg_android_mode = 0;

#ifdef WIN32
#else
pthread_t g_thOTG = 0;
pthread_mutex_t g_otgdev_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static int Uart_Control_In(struct usb_device * device, int request, int value, int index, char * buffer, int length)
{
	int retval = -1;
#ifdef WIN32
#else
	if(device)
	{
		retval = usb_device_control_transfer(device, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_DIR_IN,
				request, value, index, buffer, length, DEFAULT_TIMEOUT);
	}
#endif
	return retval;
}

static int Uart_Control_Out(struct usb_device * device, int request, int value, int index)
{
	int retval = -1;
#ifdef WIN32
#else
	if(device)
	{
		retval = usb_device_control_transfer(device, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_DIR_OUT,
			request, value, index, NULL, 0, DEFAULT_TIMEOUT);
	}
#endif
	
	return retval;
}

static int UartInit(struct usb_device * device)
{
	int ret;
	int size = 8;
	char buffer[2] = {0};
	if(!device)
	{
		return -1;
	}

	Uart_Control_Out(device, UCMD_VENDOR_SERIAL_INIT, 0x0000, 0x0000);
	ret = Uart_Control_In(device, UCMD_VENDOR_VERSION, 0x0000, 0x0000, buffer, 2);
	if(ret < 0)
		return -1;
	Uart_Control_Out(device, UCMD_VENDOR_WRITE, 0x1312, 0xD982);
	Uart_Control_Out(device, UCMD_VENDOR_WRITE, 0x0f2c, 0x0004);
	ret = Uart_Control_In(device,UCMD_VENDOR_READ, 0x2518, 0x0000, buffer, 2);
	if(ret < 0)
		return -1;
	Uart_Control_Out(device, UCMD_VENDOR_WRITE, 0x2727, 0x0000);
	Uart_Control_Out(device, UCMD_VENDOR_MODEM_OUT, 0x00ff, 0x0000);
	return 0;
}
	
static int usb_device_added(const char *devname, void* client_data) {
#ifdef WIN32
#else
    struct usb_descriptor_header* desc;
    struct usb_descriptor_iter iter;
    uint16_t vendorId, productId;
    int ret;

    printf("devname %s\n", devname);

    struct usb_device *device = usb_device_open(devname);
    if (!device) {
        fprintf(stderr, "usb_device_open failed\n");
        return 0;
    }

    vendorId = usb_device_get_vendor_id(device);
    productId = usb_device_get_product_id(device);
//chip 34X
//vendorId  = 0x1A86
//productId = 0x7523 (ch340) 0x5523 (ch341)
	printf("=======%x, %x\n", vendorId, productId);

    if (vendorId == g_vendorId && productId == g_productId) {
        struct usb_descriptor_header* desc;
        struct usb_descriptor_iter iter;
        struct usb_interface_descriptor *intf = NULL;
        struct usb_endpoint_descriptor *ep1 = NULL;
        struct usb_endpoint_descriptor *ep2 = NULL;

        printf("Found Android device in accessory mode (%x:%x)...\n",
               vendorId, productId);

        usb_descriptor_iter_init(device, &iter);
        while ((desc = usb_descriptor_iter_next(&iter)) != NULL && (!intf || !ep1 || !ep2)) {
            if (desc->bDescriptorType == USB_DT_INTERFACE) {
                intf = (struct usb_interface_descriptor *)desc;
            } else if (desc->bDescriptorType == USB_DT_ENDPOINT) {
                if (ep1)
                    ep2 = (struct usb_endpoint_descriptor *)desc;
                else
                    ep1 = (struct usb_endpoint_descriptor *)desc;
            }
        }

        if (!intf) {
            fprintf(stderr, "Interface not found\n");
        }
        if (!ep1 || !ep2) {
            fprintf(stderr, "Endpoints not found\n");
        }

        if (usb_device_claim_interface(device, intf->bInterfaceNumber)) {
            fprintf(stderr, "usb_device_claim_interface failed errno: %d\n", errno);
        }

		printf("get device ok\n");
		if(!g_sOTGDevice)
		{
			g_sOTGDevice = (struct otg_dev *)malloc(sizeof(struct otg_dev));
		}

		if(!g_sOTGDevice)
		{
			return  0;
		}
 
		g_sOTGDevice->device = device;
        if ((ep1->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) {
            g_sOTGDevice->rd_endpoint = ep1->bEndpointAddress;
            g_sOTGDevice->wr_endpoint = ep2->bEndpointAddress;
        } else {
            g_sOTGDevice->rd_endpoint = ep2->bEndpointAddress;
            g_sOTGDevice->wr_endpoint = ep1->bEndpointAddress;
        }

		return 1;  // find device
    } 

    usb_device_close(device);
#endif
    return 0;
}

static int usb_device_removed(const char *devname, void* client_data) {
    printf("usb_device_removed, %s\n", devname);
    return 0;
}

int CheckOTG(unsigned short vendorId, unsigned short productId)
{
	g_vendorId  = vendorId;
	g_productId = productId;

#ifdef WIN32
#else
	struct usb_host_context* context = usb_host_init();
	usb_host_run(context, usb_device_added, usb_device_removed, NULL, NULL);

	if(g_sOTGDevice)
	{
		int devfd = (int)g_sOTGDevice;
		g_sOTGDevice = NULL;
		CloseOTG(devfd);
		return 1;
	}
#endif

	return 0;
}

int OpenOTG(unsigned short vendorId, unsigned short productId)
{
	int count = 5;

	g_vendorId  = vendorId;
	g_productId = productId;
	g_otg_android_mode = 0;

#ifdef WIN32
#else

	struct usb_host_context* context = usb_host_init();
	usb_host_run(context, usb_device_added, usb_device_removed, NULL, NULL);

	if(g_sOTGDevice)
	{
		int devfd = (int)g_sOTGDevice;
		UartInit(g_sOTGDevice->device);
		g_sOTGDevice = NULL;
		return devfd;
	}
#endif

	return -1;
}

int OpenOTG_Android(int devdescriptor, char * devname)
{
	g_otg_android_mode = 1;
#ifdef WIN32
#else
	struct usb_device* device = NULL;
	if(devdescriptor < 0)
	{
		return -1;
	}
	
	printf("dev descriptor %d, dev name %s\n", devdescriptor, devname);
	device = usb_device_new(devname, devdescriptor);
    if (!device)
		return -1;
	
	{
        struct usb_descriptor_header* desc;
        struct usb_descriptor_iter iter;
        struct usb_interface_descriptor *intf = NULL;
        struct usb_endpoint_descriptor *ep1 = NULL;
        struct usb_endpoint_descriptor *ep2 = NULL;
		
		
        usb_descriptor_iter_init(device, &iter);
        while ((desc = usb_descriptor_iter_next(&iter)) != NULL && (!intf || !ep1 || !ep2)) {
            if (desc->bDescriptorType == USB_DT_INTERFACE) {
                intf = (struct usb_interface_descriptor *)desc;
            } else if (desc->bDescriptorType == USB_DT_ENDPOINT) {
                if (ep1)
                    ep2 = (struct usb_endpoint_descriptor *)desc;
                else
                    ep1 = (struct usb_endpoint_descriptor *)desc;
            }
        }
		
        if (!intf) {
            fprintf(stderr, "Interface not found\n");
        }
        if (!ep1 || !ep2) {
            fprintf(stderr, "Endpoints not found\n");
        }
		
        if (usb_device_claim_interface(device, intf->bInterfaceNumber)) {
            fprintf(stderr, "usb_device_claim_interface failed errno: %d\n", errno);
        }
		
		printf("get device ok\n");
		if(!g_sOTGDevice)
		{
			g_sOTGDevice = (struct otg_dev *)malloc(sizeof(struct otg_dev));
		}
		
		if(!g_sOTGDevice)
		{
			return  -1;
		}
		
		g_sOTGDevice->device = device;
        if ((ep1->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) {
            g_sOTGDevice->rd_endpoint = ep1->bEndpointAddress;
            g_sOTGDevice->wr_endpoint = ep2->bEndpointAddress;
        } else {
            g_sOTGDevice->rd_endpoint = ep2->bEndpointAddress;
            g_sOTGDevice->wr_endpoint = ep1->bEndpointAddress;
        }
		
		int devfd = (int)g_sOTGDevice;
		UartInit(g_sOTGDevice->device);
		g_sOTGDevice = NULL;
		return devfd;
    } 
#endif
	return -1;
}

int CloseOTG(int devfd)
{
#ifdef WIN32
#else

	pthread_mutex_lock(&g_otgdev_mutex);
	struct otg_dev * device = (struct otg_dev *)devfd;

	if(!g_otg_android_mode)
	{
		if(device && device->device)
		{
			printf("usb_device_close\n");
			usb_device_close(device->device);
		}
	}

	if(device)
	{
		free(device);
	}
	pthread_mutex_unlock(&g_otgdev_mutex);
#endif
	printf("CloseOTG\n");

	return 0;
}

int SettingOTG(int devfd, int speed, int databits, int stopbits, int parity, int flowcontrol)
{
	int value = 0;
	int index = 0;
	char valueHigh = 0, valueLow = 0, indexHigh = 0, indexLow = 0;
	switch(parity) {
	case 0:	/*NONE*/
		valueHigh = 0x00;
		break;
	case 1:	/*ODD*/
		valueHigh |= 0x08;
		break;
	case 2:	/*Even*/
		valueHigh |= 0x18;
		break;
	case 3:	/*Mark*/
		valueHigh |= 0x28;
		break;
	case 4:	/*Space*/
		valueHigh |= 0x38;
		break;
	default:	/*None*/
		valueHigh = 0x00;
		break;
	}
	
	if(stopbits == 2) {
		valueHigh |= 0x04;
	}
	
	switch(databits) {
	case 5:
		valueHigh |= 0x00;
		break;
	case 6:
		valueHigh |= 0x01;
		break;
	case 7:
		valueHigh |= 0x02;
		break;
	case 8:
		valueHigh |= 0x03;
		break;
	default:
		valueHigh |= 0x03;
		break;
	}
	
	valueHigh |= 0xc0;
	valueLow = 0x9c;
	
	value |= valueLow;
	value |= (int)(valueHigh << 8);
	
	switch(speed) {
	case 50:
		indexLow = 0;
		indexHigh = 0x16;
		break;
	case 75:
		indexLow = 0;
		indexHigh = 0x64;
		break;
	case 110:
		indexLow = 0;
		indexHigh = 0x96;
		break;
	case 135:
		indexLow = 0;
		indexHigh = 0xa9;
		break;
	case 150:
		indexLow = 0;
		indexHigh = 0xb2;
		break;    
	case 300:
		indexLow = 0;
		indexHigh = 0xd9;
		break;
	case 600:
		indexLow = 1;
		indexHigh = 0x64;
		break;
	case 1200:
		indexLow = 1;
		indexHigh = 0xb2;
		break;
	case 1800:
		indexLow = 1;
		indexHigh = 0xcc;
		break;
	case 2400:
		indexLow = 1;
		indexHigh = 0xd9;
		break;
	case 4800:
		indexLow = 2;
		indexHigh = 0x64;
		break;
	case 9600:
		indexLow = 2;
		indexHigh = 0xb2;
		break;
	case 19200:
		indexLow = 2;
		indexHigh = 0xd9;
		break;
	case 38400:
		indexLow = 3;
		indexHigh = 0x64;
		break;
	case 57600:
		indexLow = 3;
		indexHigh = 0x98;
		break;
	case 115200:
		indexLow = 3;
		indexHigh = 0xcc;
		break;
	case 230400:
		indexLow = 3;
		indexHigh = 0xe6;
		break;
	case 460800:
		indexLow = 3;
		indexHigh = 0xf3;
		break;				    
	case 500000:
		indexLow = 3;                                    
		indexHigh = 0xf4;
		break;
	case 921600:
		indexLow = 7;
		indexHigh = 0xf3;
		break;
	case 1000000:
		indexLow = 3;
		indexHigh = 0xfa;
		break;
	case 2000000:
		indexLow = 3;
		indexHigh = 0xfd;
		break;
	case 3000000:
		indexLow = 3;
		indexHigh = 0xfe;
		break;
	default:	// default baudRate "9600"
		indexLow = 2;
		indexHigh = 0xb2;
		break; 
	}
	
	index |= 0x88 |indexLow;
	index |= (int)(indexHigh << 8);

#ifdef WIN32
#else	
	struct otg_dev * device =  NULL;

	pthread_mutex_lock(&g_otgdev_mutex);
	device =  (struct otg_dev *)devfd;
	if(device && device->device)
	{
		Uart_Control_Out(device->device, UCMD_VENDOR_SERIAL_INIT, value, index);
	}
	pthread_mutex_unlock(&g_otgdev_mutex);

	if(flowcontrol == 1) {
		//	Uart_Tiocmset(UartModem.TIOCM_DTR | UartModem.TIOCM_RTS, 0x00);
	}
#endif	

	g_otg_seep = speed;
	return 0;
}

int ReadOTGData(int devfd, char * buff, int len)
{
	int ret = -1;

#ifdef WIN32
#else
	struct otg_dev * device = NULL;

	if(g_otg_seep == 300)
	{
		ret = g_otg_bufflen;
		memcpy(buff, g_otg_buff, g_otg_bufflen);
		memset(g_otg_buff, 0, sizeof(g_otg_buff));	
		g_otg_bufflen = 0;
		return ret;
	}

	pthread_mutex_lock(&g_otgdev_mutex);
	device = (struct otg_dev *)devfd;
	if(device)
	{

		ret = usb_device_bulk_transfer(device->device, device->rd_endpoint, buff, len, 1000);
	}	
	pthread_mutex_unlock(&g_otgdev_mutex);
#endif

	return ret;
}

int WriteOTGData(int devfd, char * buff, int len)
{
	int ret = -1;

#ifdef WIN32
#else
	struct otg_dev * device = NULL;

	pthread_mutex_lock(&g_otgdev_mutex);
	device = (struct otg_dev *)devfd;
	if(device)
	{
		if(g_otg_seep == 300)
		{
			if((unsigned char)buff[0] == 0x40)
			{
				char buffer[2] = {0};
				unsigned short wValue = ((unsigned char)buff[2] << 8) | (unsigned char)buff[3];
				
				if((unsigned char)buff[1] == 0xA1)
				{					
					ret = Uart_Control_In(device->device, UCMD_VENDOR_EEPROM_READ, wValue, 0xa001, buffer, 1);
					memcpy(g_otg_buff, buffer, 1);
					g_otg_bufflen = 1;
				}
			}
			pthread_mutex_unlock(&g_otgdev_mutex);
			return ret;
		}
        ret = usb_device_bulk_transfer(device->device, device->wr_endpoint, buff, len, 1000);
	}
	pthread_mutex_unlock(&g_otgdev_mutex);
#endif

	return ret;
}

