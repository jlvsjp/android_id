package com.lz.nfc.utils;

public class DataTypeChangeHelper {

    /**
     * 将16位的short转换成byte数组
     *
     * @param s short
     * @return byte[] 长度为2
     */
    public static byte[] shortToByteArray(short s) {
        byte[] targets = new byte[2];
        for (int i = 0; i < 2; i++) {
            int offset = (targets.length - 1 - i) * 8;
            targets[i] = (byte) ((s >>> offset) & 0xff);
        }
        return targets;
    }


    public static void main(String args[]) throws Exception {
        byte[] bytes = new byte[]{(byte) 0x01,(byte) 0xFE};
//        byte[] bytes = new byte[]{(byte) 0x01,(byte) 0x08};
//        byte[] bytes = new byte[]{(byte) 0x02, (byte) 0x02};
        int r = 0;
        r = bytes[0] << 8;
        r |= (bytes[1] & 0x000000ff);

        System.out.print(r);
    }


}
