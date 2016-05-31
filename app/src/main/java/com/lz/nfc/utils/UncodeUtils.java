package com.lz.nfc.utils;

/**
 * Created by Administrator on 2016/1/8.
 */
public class UncodeUtils {

    private String Byte2Unicode(byte abyte[], int st, int bEnd) {
    // 不包含bEnd
        StringBuffer sb = new StringBuffer("");
        for (int j = st; j < bEnd; ) {
            int lw = abyte[j++];
            if (lw < 0) lw += 256;
            int hi = abyte[j++];
            if (hi < 0) hi += 256;
            char c = (char) (lw + (hi << 8));
            sb.append(c);
        }
        return sb.toString();
    }

    private String Byte2Unicode(byte abyte[], int len) {
        return Byte2Unicode(abyte, 0, len);
    }

    public String Byte2Unicode(byte abyte[]) {
        return Byte2Unicode(abyte, 0, abyte.length);
    }

    private byte[] Unicode2Byte(String s) {
        int len = s.length();
        byte abyte[] = new byte[len << 1];
        int j = 0;
        for (int i = 0; i < len; i++) {
            char c = s.charAt(i);
            abyte[j++] = (byte) (c & 0xff);
            abyte[j++] = (byte) (c >> 8);
        }
        return abyte;
    }
}
