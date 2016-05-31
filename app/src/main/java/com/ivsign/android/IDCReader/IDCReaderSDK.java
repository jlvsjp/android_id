package com.ivsign.android.IDCReader;

public class IDCReaderSDK {
	
	public static native int wltInit(String s);

	public static native int wltGetBMP(byte abyte0[], byte abyte1[]);

	static 
	{
		System.loadLibrary("wltdecode");
	}

}
