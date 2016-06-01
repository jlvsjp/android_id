/* DO NOT EDIT THIS FILE - it is machine generated */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <jni.h>
#include <android/log.h>
#include "libnetcardreader/libnetcardreader.h"
#include "CallBackHandler.h"
#include "com_lz_nfc_jni_SelfCallbackNfc.h"
#include "javareadcard.h"

#define NFC_LOCAL_ANDROID_DEV   4
JavaReadCard g_reader;

JNIEXPORT jint JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_read
  (JNIEnv *env, jobject thzs, jstring ip, jint port)
{
    //test function
    return 0;
}


  /*
   * Class:     com_lz_nfc_jni_SelfCallbackNfc
   * Method:    SetNFCLocal
   * Signature: (II)V
   */
  JNIEXPORT void JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_SetNFCLocal
    (JNIEnv *env, jobject thzs, jint jtype, jint jfd)
    {
        //LOGE("Call SetNFCLocal");
	    int fd = jfd;
	    g_reader.SetNFCLocal(jtype,NULL);
    }

  /*
   * Class:     com_lz_nfc_jni_SelfCallbackNfc
   * Method:    SetNFCServer
   * Signature: (Ljava/lang/String;I)V
   */
  JNIEXPORT void JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_SetNFCServer
    (JNIEnv *env, jobject thzs, jstring jIP, jint jport)
    {
	    const char *IP = NULL;
	    IP = env->GetStringUTFChars(jIP,NULL);
        LOGE("Call SetNFCServer :%s : %d",(char *)IP,jport);
        g_reader.SetNFCServer((char *)IP, jport);

	    env->ReleaseStringUTFChars(jIP,IP);
    }

  /*
   * Class:     com_lz_nfc_jni_SelfCallbackNfc
   * Method:    SetNFCSerial
   * Signature: (Ljava/lang/String;I)V
   */
  JNIEXPORT void JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_SetNFCSerial
    (JNIEnv *env, jobject thzs, jstring jSerial, jint jlen)
    {
        	    const char *Serial = NULL;
        	    Serial = env->GetStringUTFChars(jSerial,NULL);
                g_reader.SetPhoneSerial((char *)Serial, jlen);

                LOGE("Call SetPhoneSerial.[%s]",(char *)Serial);

                if(strstr((char *)Serial,(char *)"10086"))
                {
                    g_reader.SetNoSNVerify();
                }
        	    env->ReleaseStringUTFChars(jSerial,Serial);
    }

  /*
   * Class:     com_lz_nfc_jni_SelfCallbackNfc
   * Method:    NFCCardReader
   * Signature: ([B)I
   */
  JNIEXPORT jint JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_NFCCardReader
    (JNIEnv *env , jobject thzs, jbyteArray jArrayData)
    {
        JavaInitApi(env,thzs);
        int iRet = 0;
    	struct SCardMsg cardMsg;

    	char data[2048] = {0};
    	memset(data,0,2048);
    	memset(&cardMsg, 0, sizeof(cardMsg));
        //g_reader.SetNoSNVerify();

        g_reader.SetFuncUnlimited();

    	iRet  = g_reader.NFCCardReader(&cardMsg);


    	if( iRet == NFC_NET_NFCREADCARD_SUCCESS)
      	{
            memcpy(data,&cardMsg,sizeof(cardMsg));
      		LOGE("read card success\n");
      		env->SetByteArrayRegion(jArrayData,0,(int)sizeof(cardMsg),(jbyte *)data);

      	}
      	else
      	{
            iRet = g_reader.GetLastError();
      		LOGE("read card fail [%d]\n",iRet);
      	}
    	return iRet;

    }




