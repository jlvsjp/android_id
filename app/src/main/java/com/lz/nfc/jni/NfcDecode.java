package com.lz.nfc.jni;

public class NfcDecode {
	static{
		System.loadLibrary("NfcDecode");
	}

	//解码接口
	public native int decryptInfo(byte[] pack1,byte[] pack2,byte[] pack3,byte[] msg);
	
}
