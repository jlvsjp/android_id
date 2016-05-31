package com.lz.nfc.utils;

/**
 * Created by Administrator on 2016/5/11.
 */
public class SSvrCMD {

    /**
     * 服务端向客户端发送指令
     */
    public static final byte SVR_CMDTYPE_SENDCDM = (byte) 0x00;
    /**
     * 服务端向客户端发送数据
     */
     public static final byte SVR_CMDTYPE_SENDDATA = (byte) 0x01;
    /**
     * 内部认证
     */
     public static final byte SVR_CMD_INCERT = (byte) 0x01;
    /**
     * 外部认证
     */
     public static final byte SVR_CMD_OUTCERT = (byte) 0x02;
    /**
     * 读照片信息
     */
     public static final byte SVR_CMD_MSG_ACK = (byte) 0x03;
    /**
     * 读照片信息确认1
     */
     public static final byte SVR_CMD_RDPHOTO_ACK1 = (byte) 0x24;
    /**
     * 读照片信息确认2
     */
     public static final byte SVR_CMD_RDPHOTO_ACK2 = (byte) 0x25;
    /**
     * 读指纹信息确认1
     */
     public static final byte SVR_CMD_RDFINGERPRINT_ACK1 = (byte) 0x28;
    /**
     * 读指纹信息确认2
     */
     public static final byte SVR_CMD_RDFINGERPRINT_ACK2 = (byte) 0x29;

     public static final byte SVR_CMD_SENDDATA = (byte) 0x20;




    /**
     * 回应启动读卡指令
     */
     public static final byte CLT_CMD_STARTRDCARD_ACK = (byte) 0x11;
    /**
     * 回应内部认证
     */
     public static final byte CLT_CMD_INAUTH_ACK = (byte) 0x12;
    /**
     * 回应外部认证
     */
     public static final byte CLT_CMD_OUTAUTH_ACK = (byte) 0x13;
    /**
     * 上传第一包照片信息
     */
     public static final byte CLT_CMD_SENDPHOTO1 = (byte) 0x24;
    /**
     * 上传第二包照片信息
     */
     public static final byte CLT_CMD_SENDPHOTO2 = (byte) 0x25;
    /**
     * 上传第一包指纹信息
     */
     public static final byte CLT_CMD_SENDFINGERPRINT1 = (byte) 0x28;
    /**
     * 上传第二包指纹信息
     */
     public static final byte CLT_CMD_SENDFINGERPRINT2 = (byte) 0x29;

    /**
     * 接收文字信息回应
     */
     public static final byte CLT_CMD_DATA_ACK1 = (byte) 0x41;
    /**
     * 接收图片第一包信息回应
     */
     public static final byte CLT_CMD_DATA_ACK2 = (byte) 0x42;
    /**
     * 接收图片第二包信息回应
     */
     public static final byte CTL_CMD_DATA_ACK3 = (byte) 0x43;

    /**
     * 第二次认证
     */
     public static final byte CLT_CMD_SECNED_ACK = (byte) 0x21;
}
