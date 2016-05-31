#ifndef SERIAL_H
#define SERIAL_H

int CheckPort(char *pSerNum);
int OpenPort(char *devname);
int ClosePort(int devfd);

int SettingPort(int devfd, int speed, int databits, int stopbits, int parity, int flowcontrol);
int ReadPortData(int devfd, char *buff,int len);
int WritePortData(int devfd, char *buff,int len);

#endif