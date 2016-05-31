package com.lz.nfc.jni;
public class SelfCallbackNfcOOOO {
    static{
        System.loadLibrary("SelfCallbackNfcOOOO");
    }

//    public native int read(String ip,int port);

    public static int ANDROID_LOCAL_DEV =   4;


    //public native int GetDevSerialNo(byte[] var1, int len);

    //public native int GetLastError();

    //public native void SetNFCLocal(int type, String DevName);
//    public native void SetNFCLocal(int type, int devfd);


    public native void SetNFCServer(String IP, int Port);

    public native void SetNFCSerial(String Serial,int len);

    public native int NFCCardReader(byte[] msg);

}
