#ifndef OTG_H
#define OTG_H

int CheckOTG(unsigned short vendorId, unsigned short productId);
int OpenOTG(unsigned short vendorId, unsigned short productId);
int OpenOTG_Android(int devdescriptor, char * devname);
int CloseOTG(int devfd);
int SettingOTG(int devfd, int speed, int databits, int stopbits, int parity, int flowcontrol);

int ReadOTGData(int devfd, char * buff, int len);
int WriteOTGData(int devfd, char * buff, int len);

#endif
