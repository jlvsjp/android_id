//
// Created by Administrator on 16-5-23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <jni.h>
#include <android/log.h>
#include "CallBackHandler.h"

JNIEnv *g_env = NULL;			    	//  JAVA虚拟机环境变量
jobject g_obj = 0;				        //  全局obj
jclass g_cls = 0;				        //  类变量
jmethodID  g_method_first   =  0 ;		//  方法1 取applictionData
jmethodID  g_method_send =  0 ;		    //  发送数据方法

static  char g_str[128] = {0};
static void print_hex(char *type,unsigned char *data,int len)
{
    memset(g_str,'\0',128);
    int i = 0;
    for(i=0;i<len;i++)
    {
        if((i+1)%16  == 0)
        {
            printf("\n");
        }
        printf("%02X ",data[i]);
        sprintf(g_str+i*2,"%02X",data[i]);
    }
    LOGE("[%s:%d] --> :%s\n",type,len,g_str);
}





//将Java数组转换为C数组
static int getCArrayFormJava(JNIEnv *env,jbyteArray jarray,unsigned char *carray)
{
    if(jarray == NULL )
        return -1;
    jsize len = env->GetArrayLength(jarray);
    if(len <=0)
    	return -1;

    //LOGE("Get Byte len :%d\n",len);
    env->GetByteArrayRegion(jarray,0,len,(jbyte*)carray);
    //if(error)
    //print_hex(carray,len);
    return len;
}

int JavaInitApi(JNIEnv *env,jobject obj)
{
	g_env = env;
	g_obj = obj;
	g_cls = (env)->GetObjectClass(obj);
	g_method_first = (env)->GetMethodID(g_cls,"callBackFirst","()[B");

	g_method_send = (env)->GetMethodID(g_cls,"callBackCommand","([B)[B");

    LOGE("g_env= %p,g_obj=%p,g_cls=%p",g_env,&g_obj,&g_cls);
    LOGE("GetAppData=%p,SendCmd=%p",&g_method_first,&g_method_send);
}


int JavaGetApplicationData(unsigned char *recvbuff)
{
	int ret = 0;
	jobject firstBytesObj = (g_env)->CallObjectMethod(g_obj,g_method_first,NULL);
	//if(error)

	jbyteArray firstBytes = (jbyteArray)firstBytesObj;

    //jsize len = g_env->GetArrayLength(firstBytes);
	//if(error)
    //LOGE((char *)"Get Appliction len :%d\n",len);
	//unsigned char cData[128] = {0};

    ret = getCArrayFormJava(g_env,firstBytes,recvbuff);
    //print_hex((char *)"GetApplictionData",recvbuff,len);
	//if(error)

    return ret;
}

int JavaSendCmd(unsigned char *buff,int len,unsigned char *recvbuff)
{
    //print_hex((char *)"CMD Send",buff,len);
	int ret = 0;
	jbyteArray data = g_env->NewByteArray(len);
    g_env->SetByteArrayRegion(data, 0, len, (jbyte *)buff);
	jobject  recvObj = g_env->CallObjectMethod(g_obj,g_method_send,data);
    jbyteArray recvArray = (jbyteArray) recvObj;
    ret = getCArrayFormJava(g_env,recvArray,recvbuff);

    //print_hex((char *)"CMD Recv",recvbuff,ret);
	//	free bytes ?
    //g_env->ReleaseByteArrayElements(recvArray,(jbyte *)buff,0);
    //g_env->ReleaseByteArrayElements(data,(jbyte *)buff,0);
	return ret;
}


int JavaCmdRequest(int cmdno, int p1, int p2,char * recvbuff, int bufflen)
{
    unsigned char buff[156] = {0};
    unsigned char cmd[64] = {0};
    int ret = 0;
    int len = 0;

    cmd[0] = 0x42;
    len += 1;
    switch(cmdno)
    {
    case NFC_CMD_REQB:
        cmd[1] = NFC_CMD_REQB;
        cmd[2] = 0x00;
        cmd[3] = 0x00;
        len += 3;
        break;
    case NFC_CMD_NOATTRIB:
        cmd[1] = NFC_CMD_NOATTRIB;
        cmd[2] = 0x00;
        cmd[3] = 0x00;
        cmd[4] = 0x00;
        cmd[5] = 0x00;
        cmd[6] = 0x00;
        cmd[7] = 0x08;
        cmd[8] = 0x01;
        cmd[9] = 0x08;
        len += 9;
        break;
    case NFC_CMD_SELFILE:
        cmd[1] = 0x00;
        cmd[2] = NFC_CMD_SELFILE;;
        cmd[3] = 0x00;
        cmd[4] = 0x00;
        cmd[5] = 0x02;
        cmd[6] = 0xff & (p1 >> 8);
        cmd[7] = 0xff & (p1 >> 0);
        len += 7;
        break;
    case NFC_CMD_RDFILE:
        cmd[1] = 0x80;
        cmd[2] = NFC_CMD_RDFILE;;
        cmd[3] = 0xff & (p1 >> 8);
        cmd[4] = 0xff & (p1 >> 0);
        cmd[5] = p2;
        len += 5;
        break;
    case NFC_CMD_SELRDFILE:
    {
        int len = p2;
        int off = 0;
        if(JavaCmdRequest(NFC_CMD_SELFILE, p1, 0, recvbuff, bufflen) < 0)
        {
            //TRACE("select file %x error\n", p1);
            return -1;
        }
        memset(recvbuff, 0, bufflen);
        //LOGE("==== goto read %d\n",len);
        while(len)
        {
            int rdlen = len > 0x70 ? 0x70 : len;
            ret = JavaCmdRequest(NFC_CMD_RDFILE, off, rdlen, recvbuff + off, bufflen - off);
            //LOGE("==== read %d, ret = %d\n",rdlen,ret);
            if(ret != rdlen + 2) // ��״̬�� 90 00
            {
                //TRACE("read file fail\n");
                return -1;
            }
            ret -= 2;  // ȥ��90 00
            len -= ret;
            off += ret;
        }
        return off;
    }
        break;
    case NFC_CMD_INCERT:
        memcpy(&cmd[1], (char*)p1, p2);
        len += p2;
        break;
    case NFC_CMD_OUTCERT:
        memcpy(&cmd[1], (char*)p1, p2);
        len += p2;
        break;
    case NFC_CMD_RDRAND:
        cmd[1] = 0x00;
        cmd[2] = NFC_CMD_RDRAND;;
        cmd[3] = 0x00;
        cmd[4] = 0x00;
        cmd[5] = p1;
        len += 5;
        break;
    case NFC_CMD_RDUID:
        cmd[1] = 0x00;
        cmd[2] = NFC_CMD_RDUID;
        cmd[3] = 0x00;
        cmd[4] = 0x00;
        cmd[5] = p1;
        len += 5;
        break;
    default:
        break;
    }
    //LOGE("################# type = %d ################\n",m_nfcType);
    if(1)
    {
        //LOGE("-----> Call JavaSendCmd \n");
        //if an android local dev
        //unsigned char recv_tmp[156] = {0};
        //LOGE("Send CMD [%02x%02x] \n",cmd[1],cmd[2]);
        //LOGBUFF((char *)"发送命令",cmd+1,len-1);
        //LOGE("函数地址:%p\n",JavaSendCmd);
        ret = JavaSendCmd(cmd+1,len-1,buff);
        //LOGE("-----> Call JavaSendCmd ret %d\n",ret);
        //ret = JavaSendCmd(cmd+1,len-1,recvbuff);
        if(ret > 0)
        {
            if(ret >= 3 && buff[ret-1]==0x00 && buff[ret-2]== 0x00 && buff[ret-3]==0x90)
            {
                ret -=1;
            }

            memcpy(recvbuff,buff,ret);
            return ret;
        }
        //android 的返回是900000,多一个00

        //recvbuff

    }

    return -1;
}




/*********************************************************/
//	CallBackHandler
/*********************************************************/



