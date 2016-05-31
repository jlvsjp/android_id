package com.lz.nfc.utils;

/**
 * Created by Administrator on 2016/5/6.
 */
public class CommandUtils {
    public static final byte[] success = new byte[]{(byte) 0x90, (byte) 0x00, (byte) 0x00};

    /**
     * 启动读卡
     */
    /**
     * 10Byte指令，前5Byte不变
     */
    public static final byte[] start_result_data = new byte[]{(byte) 0xE0, (byte) 0x35, (byte) 0x37, (byte) 0x48, (byte) 0x58};
    /**
     * 10Byte指令长度
     */
    public static final int start_result_data_len = start_result_data.length + 5;

    /**
     * 寻卡
     * 1、REQB命令
     * 单片机->CV628
     * 05 00 00
     * CV628->单片机
     * 50 00 00 00 00 D1 03 86 0C 00 80 80
     * public static final byte[] c_1 = new byte[]{(byte) 0x05, (byte) 0x00, (byte) 0x00};
     */

    /**
     * 选卡
     * 2、非标Attrib命令
     * 单片机->CV628
     * 1D 00 00 00 00 00 08 01 08
     * CV628->单片机
     * 08
     * public static final byte[] c_2 = new byte[]{(byte) 0x1D, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x08, (byte) 0x01, (byte) 0x08};
     */

    /**
     * 3、选择MF(主文件 60 02)
     * 单片机->CV628
     * 00 A4 00 00 02 60 02
     * CV628->单片机
     * 90 00
     */
    public static final byte[] c_3 = new byte[]{(byte) 0x00, (byte) 0xA4, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x60, (byte) 0x02};

    /**
     * 4、读二进制（DN码，60 02）
     * 单片机->CV628
     * 80 B0 00 00 20
     * CV628->单片机
     * 00 01 59 4e 01 0e 0b 05 00 00 00 00 00 05 09 94 90 e9 7f aa 27 0a c7 9d 06 02 c3 ba 44 2b 09 d5 90 00
     */
    public static final byte[] c_4 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x00, (byte) 0x20};

    /**
     * 5、内部认证
     * 单片机->CV628
     * CLA|INS|P1|P2|Lc|DATA（每次认证时DATA是不一样）
     * 00 88 00 52 0A F0 00 0B 78 B5 6D 20 0F 28 E4
     * CV628->单片机
     * 51 91 41 19 9B 17 33 61 90 00 (每次不一样)
     */

    /**
     * 6、取随机数
     * 单片机->CV628
     * 00 84 00 00 08
     * CV628->单片机
     * 76 22 0C 03 C2 D5 4C 0B 90 00 (每次不一样)
     */
    public static final byte[] c_6 = new byte[]{(byte) 0x00, (byte) 0x84, (byte) 0x00, (byte) 0x00, (byte) 0x08};

    /**
     * 7、外部认证
     * 单片机->CV628
     * 00 82 00 52 0A 0F 01 A6 19 44 4D 87 0B E6 C5
     * CV628->单片机
     * 90 00
     */

    /**
     * 8、选择MF(主文件 60 11)
     * 单片机->CV628
     * 00 A4 00 00 02 60 11
     * CV628->单片机
     * 90 00
     */
    public static final byte[] c_8 = new byte[]{(byte) 0x00, (byte) 0xA4, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x60, (byte) 0x11};

    /**
     * 9、读二进制（60 11)
     * 单片机->CV628
     * 80 B0 00 00 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_9 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x00, (byte) 0x70};

    /**
     * 10、读二进制（60 11)
     * 单片机->CV628
     * 80 B0 00 70 66
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_10 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x70, (byte) 0x66};

    /**
     * 11、选择MF(主文件 60 12)
     * 单片机->CV628
     * 00 A4 00 00 02 60 12
     * CV628->单片机
     * 90 00
     */
    public static final byte[] c_11 = new byte[]{(byte) 0x00, (byte) 0xA4, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x60, (byte) 0x12};

    /**
     * 12、读二进制（60 12)
     * 单片机->CV628
     * 80 B0 00 00 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_12 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x00, (byte) 0x70};

    /**
     * 13、读二进制（60 12)
     * 单片机->CV628
     * 80 B0 00 70 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_13 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x70, (byte) 0x70};

    /**
     * 14、读二进制（60 12)
     * 单片机->CV628
     * 80 B0 00 E0 20
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_14 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0xE0, (byte) 0x20};

    /**
     * 15、选择MF(主文件 60 13)
     * 单片机->CV628
     * 00 A4 00 00 02 60 13
     * CV628->单片机
     * 90 00
     */
    public static final byte[] c_15 = new byte[]{(byte) 0x00, (byte) 0xA4, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x60, (byte) 0x13};

    /**
     * 16、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 00 00 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_16 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x00, (byte) 0x70};

    /**
     * 17、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 00 70 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_17 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x70, (byte) 0x70};

    /**
     * 18、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 00 E0 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_18 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0xE0, (byte) 0x70};

    /**
     * 19、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 01 50 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_19 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x01, (byte) 0x50, (byte) 0x70};

    /**
     * 20、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 01 C0 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_20 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x01, (byte) 0xC0, (byte) 0x70};

    /**
     * 21、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 02 30 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_21 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x02, (byte) 0x30, (byte) 0x70};

    /**
     * 22、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 02 A0 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_22 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x02, (byte) 0xA0, (byte) 0x70};

    /**
     * 23、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 03 10 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_23 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x03, (byte) 0x10, (byte) 0x70};

    /**
     * 24、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 03 80 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_24 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x03, (byte) 0x80, (byte) 0x70};

    /**
     * 25、读二进制（60 13）
     * 单片机->CV628
     * 80 B0 03 F0 10
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_25 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x03, (byte) 0xF0, (byte) 0x10};

    /**
     * 26、选择MF(主文件 60 21)
     * 单片机->CV628
     * 00 A4 00 00 02 60 21
     * CV628->单片机
     * 90 00
     */
    public static final byte[] c_26 = new byte[]{(byte) 0x00, (byte) 0xA4, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x60, (byte) 0x21};

    /**
     * 27、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 00 00 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_27 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x00, (byte) 0x70};

    /**
     * 28、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 00 70 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_28 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0x70, (byte) 0x70};

    /**
     * 29、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 00 E0 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_29 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x00, (byte) 0xE0, (byte) 0x70};

    /**
     * 30、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 01 50 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_30 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x01, (byte) 0x50, (byte) 0x70};

    /**
     * 31、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 01 C0 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_31 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x01, (byte) 0xC0, (byte) 0x70};

    /**
     * 32、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 02 30 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_32 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x02, (byte) 0x30, (byte) 0x70};

    /**
     * 33、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 02 A0 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_33 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x02, (byte) 0xA0, (byte) 0x70};

    /**
     * 34、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 03 10 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_34 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x03, (byte) 0x10, (byte) 0x70};

    /**
     * 35、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 03 80 70
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_35 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x03, (byte) 0x80, (byte) 0x70};

    /**
     * 36、读二进制（60 21)
     * 单片机->CV628
     * 80 B0 03 F0 10
     * CV628->单片机
     * data 90 00
     */
    public static final byte[] c_36 = new byte[]{(byte) 0x80, (byte) 0xB0, (byte) 0x03, (byte) 0xF0, (byte) 0x10};

    /**
     * 客户端向服务端发送指令
     *
     * @param command 指令代码1B
     * @param bytes   数据（多个）
     * @param sendLen 客户端向服务器发送指令长度
     * @return 返回需要向服务器发送的指令
     */
    public static byte[] MblToSvr(byte command, byte[][] bytes, int sendLen) {
        byte[] b = new byte[sendLen];
        b[0] = command;//指令加入
        b[1] = (byte) bytes.length;//包数
        int data_first = 4;
        int count_len_temp = 0;
        for (byte[] by : bytes) {
            count_len_temp += by.length;
            byte[] len = DataTypeChangeHelper.shortToByteArray((short) by.length);//转换为2B
            count_len_temp += len.length;

            System.arraycopy(len, 0, b, data_first, len.length);//包长度
            data_first += len.length;

            System.arraycopy(by, 0, b, data_first, by.length);//包内容

            data_first += by.length;
        }
        //数据包长度
        byte[] countLen = DataTypeChangeHelper.shortToByteArray((short) count_len_temp);
        System.arraycopy(countLen, 0, b, 2, countLen.length);
        return b;
    }

    /**
     * 一个包多分
     * @param command 指令
     * @param bytes
     * @param countLen
     * @return
     */
    public static byte[] MblSplitToSvr(byte command, byte[] bytes, int countLen) {
        byte[] b = new byte[bytes.length+6];
        b[0] = command;//指令加入
        b[1] = (byte) 0x00;//包数代表一个包，但是包是分开的
        int data_first = 4;
        byte[] data_len = DataTypeChangeHelper.shortToByteArray((short) bytes.length);
        System.arraycopy(data_len, 0, b, data_first, data_len.length);//包长度
        data_first += data_len.length;
        System.arraycopy(bytes, 0, b, data_first, bytes.length);//包内容
        //数据包长度
        byte[] count = DataTypeChangeHelper.shortToByteArray((short) countLen);
        System.arraycopy(count, 0, b, 2, count.length);
        return b;
    }

    public static boolean endWith(byte[] value, byte[] suffix){
        if (value.length<suffix.length)
            return false;

        for (int offset=1; offset<=suffix.length;offset++) {
            if (value[value.length - offset] != suffix[suffix.length - offset]) {
                return false;
            }
        }
        return true;
    }

    public static boolean startWith(byte[] value, byte[] suffix){
        if (value.length<suffix.length)
            return false;

        for (int offset=0; offset<suffix.length;offset++) {
            if (value[offset] != suffix[offset]) {
                return false;
            }
        }
        return true;
    }

    public static byte[] subWithEnd(byte[] value, byte[] suffix){
        if(endWith(value,suffix)){
            byte[] result = new byte[value.length-suffix.length];
            System.arraycopy(value, 0, result, 0, value.length-suffix.length);
            return result;
        }
        return null;
    }

    /**
     * 从一个byte[]数组中截取一部分
     * @param src
     * @param begin
     * @param count
     * @return
     */
    public static byte[] subBytes(byte[] src, int begin, int count) {
        byte[] bs = new byte[count-begin];
        for (int i = begin; i< count; i++)
            bs[i-begin] = src[i];
        return bs;
    }

}
