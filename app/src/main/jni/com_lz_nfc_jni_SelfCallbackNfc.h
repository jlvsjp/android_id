/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_lz_nfc_jni_SelfCallbackNfc */

#ifndef _Included_com_lz_nfc_jni_SelfCallbackNfc
#define _Included_com_lz_nfc_jni_SelfCallbackNfc
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_lz_nfc_jni_SelfCallbackNfc
 * Method:    read
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_read
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_lz_nfc_jni_SelfCallbackNfc
 * Method:    SetNFCLocal
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_SetNFCLocal
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_lz_nfc_jni_SelfCallbackNfc
 * Method:    SetNFCServer
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_SetNFCServer
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_lz_nfc_jni_SelfCallbackNfc
 * Method:    SetNFCSerial
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_SetNFCSerial
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_lz_nfc_jni_SelfCallbackNfc
 * Method:    NFCCardReader
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_lz_nfc_jni_SelfCallbackNfc_NFCCardReader
  (JNIEnv *, jobject, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif