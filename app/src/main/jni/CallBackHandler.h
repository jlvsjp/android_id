//
// Created by Administrator on 16-5-23.
//

#ifndef NFC_CALLBACKHANDLER_H
#define NFC_CALLBACKHANDLER_H
#include <jni.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <android/log.h>
#define TAG "andy_callback_test"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)

inline void LOGBUFF(char *tag,void *ibuff,int len)
{
    char *buff = (char *)ibuff;
    int i = 0;
    char pbuff[512] = {0};
    for(i = 0;i < len;i++)
    {
        sprintf(pbuff+2*i,"%02X",buff[i]);
    }

    LOGE("%s  Hex[%d]:[%s]\n",tag,len,pbuff);
}


//初始化接口
int JavaInitApi(JNIEnv *env,jobject obj);

//发送消息获取返回值 callBackCommnad
int JavaCmdRequest(int cmdno, int p1, int p2,char * recvbuff, int bufflen);

//检查获取applictionData
//int JavaCheckNFCDev(int nfcType, char * nfcDev);

int JavaGetApplicationData(unsigned char *recvbuff);
int JavaSendCmd(unsigned char *buff,int len,unsigned char *recvbuff);


/*********************************************************************************************/
//NFC命令定义-----------------------|功能描述-------|参数p1--------|参数p2-----------|备注----------------
#define NFC_CMD_REQB      0x05      //选B卡        |    无        |     无           |
#define NFC_CMD_NOATTRIB  0x1D      //属性         |    无        |     无           |               

//APDU命令
#define NFC_CMD_SELFILE   0xA4      //选择文件     |   文件分区   |     无           |         
#define NFC_CMD_RDFILE    0xB0      //读文件       |   偏移地址   |     长度         |
#define NFC_CMD_INCERT    0x88      //内部认证     |   缓冲地址   |     长度         |
#define NFC_CMD_OUTCERT   0x82      //外部认证     |   缓冲地址   |     长度         |
#define NFC_CMD_RDRAND    0x84      //读随机数     |   随机数长度 |     无           |
#define NFC_CMD_RDUID     0x36      //读UID        |   UID长度    |     无           |

//组合命令
#define NFC_CMD_SELRDFILE 0xA4B0    //选择并读文件 |   文件分区   |     读取长度     |  该模式为连续读取，并去除90 00


#endif //NFC_CALLBACKHANDLER_H
