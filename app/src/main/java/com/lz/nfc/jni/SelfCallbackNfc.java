package com.lz.nfc.jni;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.NfcB;
import android.os.Handler;
import android.os.Message;

import com.ivsign.android.IDCReader.IDCReaderSDK;
import com.lz.domain.IdentityCard;
import com.lz.nfc.utils.ByteHex;
import com.lz.nfc.utils.MessageUtils;
import com.lz.nfc.utils.StreamUtils;
import com.lz.nfc.utils.UncodeUtils;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Administrator on 2016/5/12.
 */
public class SelfCallbackNfc {
    //    private TcpDataService tcpDataService = new TcpDataService();
//    private NfcDecode nfcDecode = new NfcDecode();
//    private String TAG = "SelfCallbackNfcOOOO";
    private UncodeUtils uncodeUtils = new UncodeUtils();
    private Handler mHandler;
    private Context context;
    private String ip;
    private int port;
    private NfcAdapter mAdapter = null;
    private PendingIntent pi = null;
    private IntentFilter tagDetected = null;
    private String[][] mTechLists;
    private NfcB isodep = null;

    static{
        System.loadLibrary("SelfCallbackNfc");
    }
    //发送指令的回调函数
    public byte[] callBackCommand(byte[] command){
        try {
            String cs =ByteHex.bytesToHexString(command);
            // Log.i()
            return isodep.transceive(command);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return new byte[0];
    }
    //获取applicationData
    public byte[] callBackFirst(){
        byte[] applicationData = isodep.getApplicationData();
        byte[] protocolInfo = isodep.getProtocolInfo();
        byte[] bytes_data = new byte[applicationData.length+protocolInfo.length];
        System.arraycopy(applicationData, 0, bytes_data, 0, applicationData.length);
        System.arraycopy(protocolInfo, 0, bytes_data, applicationData.length, protocolInfo.length);
        return bytes_data;
    }

    /**
     * char szCardNo[38];         //身份证号
     * char szName[32];           //姓名
     * char szSex[4];             //性别
     * char szNationality[6];     //民族
     * char szBirth[18];          //出生
     * char szAddress[92];        //住址
     * char szAuthority[92];      //签发机关
     * char szPeriod[40];         //有效期限
     * char szPhoto[1024];        //身份证照片, wlf格式
     * char szDn[16];		       //DN码
     * char Reserve[48];          //保留
     */
    //数据解码
    public void callBackFinish(byte[] bytes){
        IdentityCard identityCard = new IdentityCard();
        // 身份证：38
        byte[] idcard_bytes = new byte[38];
        for (int i = 0; i <= 37; i++) {
            idcard_bytes[i - 0] = bytes[i];
        }
        String idCard = uncodeUtils.Byte2Unicode(idcard_bytes);
        identityCard.setCardNo(idCard);
        // 姓名:32
        byte[] name_bytes = new byte[32];
        for (int i = 38; i <= 69; i++) {
            name_bytes[i - 38] = bytes[i];
        }
        String name = uncodeUtils.Byte2Unicode(name_bytes);
        identityCard.setName(name);
        // 性别:4
        byte[] xb_bytes = new byte[4];
        for (int i = 70; i <= 73; i++) {
            xb_bytes[i - 70] = bytes[i];
        }
        String xb = uncodeUtils.Byte2Unicode(xb_bytes);
        identityCard.setSex(MessageUtils.getSex(xb));
        // 民族:6
        byte[] mz_bytes = new byte[6];
        for (int i = 74; i <= 79; i++) {
            mz_bytes[i - 74] = bytes[i];
        }
        String mz = uncodeUtils.Byte2Unicode(mz_bytes);
        identityCard.setEthnicity(MessageUtils.getMinZu(mz));
        // 出生:18
        byte[] birth_bytes = new byte[18];
        for (int i = 80; i <= 97; i++) {
            birth_bytes[i - 80] = bytes[i];
        }
        String birth = uncodeUtils.Byte2Unicode(birth_bytes);
        identityCard.setBirth(birth);
        // 住址:92
        byte[] zz_bytes = new byte[92];
        for (int i = 98; i <= 189; i++) {
            zz_bytes[i - 98] = bytes[i];
        }
        String zz = uncodeUtils.Byte2Unicode(zz_bytes);
        identityCard.setAddress(zz);
        // 签发机关:92
        byte[] jf_bytes = new byte[92];
        for (int i = 190; i <= 281; i++) {
            jf_bytes[i - 190] = bytes[i];
        }
        String jftext = uncodeUtils.Byte2Unicode(jf_bytes);
        identityCard.setAuthority(jftext);
        // 有效期限:40
        byte[] start_bytes = new byte[40];
        for (int i = 282; i <= 321; i++) {
            start_bytes[i - 282] = bytes[i];
        }
        String validity = uncodeUtils.Byte2Unicode(start_bytes);
        identityCard.setPeriod(validity);

        // 身份证照片, wlf格式:1024
        byte[] photo_bytes = new byte[1024];
        for (int i = 322; i <= 1345; i++) {
            photo_bytes[i - 322] = bytes[i];
        }

        InputStream lic = getClass().getResourceAsStream("/res/raw/license.lic");
        InputStream base = getClass().getResourceAsStream("/res/raw/base.dat");

        String p = context.getFilesDir().getAbsolutePath();

        File license = new File(p + "/license.lic");
        if (!license.exists()) {
            StreamUtils.copyInputStream(lic, p + "/license.lic");
        }
        File baseDat = new File(p + "/base.dat");
        if (!baseDat.exists()) {
            StreamUtils.copyInputStream(base, p + "/base.dat");
        }

        int result = IDCReaderSDK.wltInit(p);

        if (result == 0) {
            byte[] var = new byte[]{(byte) 5, (byte) 0, (byte) 1, (byte) 0, (byte) 91, (byte) 3, (byte) 51, (byte) 1, (byte) 90, (byte) -77, (byte) 30, (byte) 0};
            int wltGetBMP = IDCReaderSDK.wltGetBMP(photo_bytes, var);
            if (wltGetBMP == 1) {
                String p1 = context.getFilesDir().getAbsolutePath() + "/zp.bmp";
                byte[] b = StreamUtils.input2byte(p1);
                Message message = new Message();
                message.what = SelfCallbackNfc.READ_PHOTO_SUCCESS;
                message.obj = b;
                mHandler.sendMessage(message);
            } else {
                Message message = new Message();
                message.what = SelfCallbackNfc.READ_PHOTO_ERROR;
                message.obj = "读取照片返回错误码" + wltGetBMP;
                mHandler.sendMessage(message);
            }
        }else {
            Message message = new Message();
            message.what = SelfCallbackNfc.READ_PHOTO_ERROR;
            message.obj = "读取照片返回错误码" + result;
            mHandler.sendMessage(message);
        }

        byte[] dn_bytes = new byte[16];
        for (int i = 1346; i <= 1361; i++) {
            dn_bytes[i - 1346] = bytes[i];
        }
        String dn = ByteHex.bytesToHexString(dn_bytes).toUpperCase();
        identityCard.setDN(dn);

        Message message = new Message();
        message.what = READ_IDCARD_SUCCESS;
        message.obj = identityCard;
        mHandler.sendMessage(message);
    }


    /*******************************/

    public static int ANDROID_LOCAL_DEV =   4;

    //获取设备序列号
    //public native int GetDevSerialNo(byte[] var1, int len);
    //获取错误号
    //public native int GetLastError();
    //本地设备设置
    //public native void SetNFCLocal(int type, String DevName);
    public native void SetNFCLocal(int type, int devfd);

    //设置NFC服务器
    public native void SetNFCServer(String IP, int Port);
    // 设置手机序列号
    public native void SetNFCSerial(String Serial,int len);
    //NFC读卡
    public native int NFCCardReader(byte[] msg);


    /*********************************/


    //JNI调用，参数是Ip,端口
    public native int read(String ip,int port);





    public int callBackAdd(int a)
    {
        return a+1;
    }

    public static final int NFC_NET_CONNECT_ERROR = 10001; // 网络连接错误
    public static final int NFC_NET_STARTREADCARD_FAIL = 10002; // 启动网络读卡失败
    public static final int NFC_READCARD_FAIL = 10003; // 读卡失败
    public static final int READ_IDCARD_SUCCESS = 10004; //读取身份信息成功
    public static final int READ_PHOTO_SUCCESS = 10005; //读取照片成功
    public static final int PLASE_INIT_SERVER = 10006; //请先初始化服务器
    public static final int READ_PHOTO_ERROR = 10007; //读取照片出错
    public static final int DEC_DATA_ERROR = 10008; //解码数据

    public SelfCallbackNfc(Handler mHandler, Context context) {
        this.mHandler = mHandler;
        this.context = context;
    }

    public void setTheServer(String ip,int port){
        this.ip = ip;
        this.port = port;
    }

    public void startNFC_Listener() {
        this.mAdapter.enableForegroundDispatch((Activity) this.context, this.pi, new IntentFilter[]{this.tagDetected}, this.mTechLists);
    }

    public void stopNFC_Listener() {
        this.mAdapter.disableForegroundDispatch((Activity) this.context);
    }

    public void init_NFC() {
        this.pi = PendingIntent.getActivity(this.context, 0, (new Intent(this.context, this.context.getClass())).addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING), 0);
        this.tagDetected = new IntentFilter("android.nfc.action.TAG_DISCOVERED");
        this.tagDetected.addCategory("android.intent.category.DEFAULT");
        this.mTechLists = new String[][]{{NfcB.class.getName()}};
    }

    public Boolean EnableSystemNFCMessage() {
        this.mAdapter = NfcAdapter.getDefaultAdapter(this.context);
        if (this.mAdapter == null) {
            return Boolean.valueOf(false);
        } else {
            this.init_NFC();
            if (this.mAdapter == null) {
                return Boolean.valueOf(false);
            } else {
                this.startNFC_Listener();
                return Boolean.valueOf(true);
            }
        }
    }

    public Boolean isNFC(Intent intent) {
        String var2 = intent.getAction();
        if ("android.nfc.action.TECH_DISCOVERED".equals(var2)) {
            return Boolean.valueOf(true);
        } else {
            return Boolean.valueOf(false);
        }
    }

    public void readCardWithIntent(Intent intent) {
        Tag tag = intent.getParcelableExtra("android.nfc.extra.TAG");
        isodep = NfcB.get(tag);
        try {
            isodep.connect();
            if (isodep.isConnected()) {
                readIdCard(isodep);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void DisableSystemNFCMessage() {
        if (this.mAdapter != null) {
            this.stopNFC_Listener();
        }

    }

    public void readIdCard(final NfcB isodep) {
        this.isodep = isodep;

        int ret = 0;
        //int ret = read(this.ip,this.port);
        //ANDROID_LOCAL_DEV
        SetNFCLocal(ANDROID_LOCAL_DEV, 2);
        SetNFCServer(this.ip, this.port);
        //SetNFCServer("117.149.14.7",8018);
        // 设置手机序列号
       SetNFCSerial("10086",5);
        //NFC读卡
        byte[] nfc_data = new byte[2048];

        ret =  NFCCardReader(nfc_data);
        if(ret == 0 )
            callBackFinish(nfc_data);
        ret = ret +1 -1;
 /*
        try {
            byte[] bytes_8 = isodep.transceive(CommandUtils.c_8);
            String hex_8 = CodingUtils.byte2hex(bytes_8);
            //Log.i(TAG, "选择6011MF:" + hex_8);

            if (!CommandUtils.endWith(bytes_8, CommandUtils.success)) {
                //Log.e(TAG, "******************选择6011MF错误******************");
                Message message = new Message();
                message.what = NFC_READCARD_FAIL;
                message.obj = "选择6011MF错误";
                mHandler.sendMessage(message);
                return;

            }

            byte[] bytes_9 = isodep.transceive(CommandUtils.c_9);
            if (!CommandUtils.endWith(bytes_9, CommandUtils.success)) {
                //Log.e(TAG, "******************读取6011数据_1错误******************");
                Message message = new Message();
                message.what = NFC_READCARD_FAIL;
                message.obj = "读取6011数据_1错误";
                mHandler.sendMessage(message);
                return;

            }
            bytes_9 = CommandUtils.subWithEnd(bytes_9, CommandUtils.success);
            String hex_9 = CodingUtils.byte2hex(bytes_9);
            //Log.i(TAG, "读取6011数据_1:" + hex_9);


            byte[] bytes_10 = isodep.transceive(CommandUtils.c_10);
            if (!CommandUtils.endWith(bytes_10, CommandUtils.success)) {
                //Log.e(TAG, "******************读取6011数据_2错误******************");
                Message message = new Message();
                message.what = NFC_READCARD_FAIL;
                message.obj = "读取6011数据_2错误";
                mHandler.sendMessage(message);
                return;

            }
            bytes_10 = CommandUtils.subWithEnd(bytes_10, CommandUtils.success);
            String hex_10 = CodingUtils.byte2hex(bytes_10);
            //Log.i(TAG, "读取6011数据_2:" + hex_10);


            byte[] bytes6011 = new byte[bytes_9.length + bytes_10.length];
            System.arraycopy(bytes_9, 0, bytes6011, 0, bytes_9.length);
            System.arraycopy(bytes_10, 0, bytes6011, bytes_9.length, bytes_10.length);


    } catch (Exception e) {
        e.printStackTrace();
        Message message = new Message();
        message.what = NFC_READCARD_FAIL;
        message.obj = "NFC读卡失败，出现异常";
        mHandler.sendMessage(message);
        return;
    } finally {
        //tcpDataService.disconnect();
        try {
            isodep.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
*/




        /*new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    *//**
         * MBL->SVR 发起TCP连接
         *//*

                    int i = tcpDataService.connect(ip,port);
                    if (i != 1) {
                        Log.e(TAG, "******************连接网络异常******************");
                        Message message = new Message();
                        message.what = NFC_NET_CONNECT_ERROR;
                        message.obj = "连接网络异常";
                        mHandler.sendMessage(message);
                        return;
                    }

                    *//**
         * SVR->MBL(10B)
         *//*
                    byte[] bytes = new byte[CommandUtils.start_result_data_len];
                    int r = tcpDataService.recData(bytes, bytes.length);
                    if (r == -1) {
                        Log.e(TAG, "******************网络接收错误******************");
                        Message message = new Message();
                        message.what = NFC_NET_CONNECT_ERROR;
                        message.obj = "网络接收错误";
                        mHandler.sendMessage(message);
                        return;
                    }
                    String hex_bytes = CodingUtils.byte2hex(bytes);
                    //Log.i(TAG, "启动读卡指令:" + hex_bytes);

                    if (!CommandUtils.startWith(bytes, CommandUtils.start_result_data)) {
                        Log.e(TAG, "******************启动读卡指令错误******************");
                        Message message = new Message();
                        message.what = NFC_NET_STARTREADCARD_FAIL;
                        message.obj = "启动读卡指令错误";
                        mHandler.sendMessage(message);
                        return;
                    }
                    //选择MF
                    byte[] bytes_3 = isodep.transceive(CommandUtils.c_3);
                    String hex_3 = CodingUtils.byte2hex(bytes_3);
                    //Log.i(TAG, "选择MF_3:" + hex_3);
                    if (!CommandUtils.endWith(bytes_3, CommandUtils.success)) {
                        Log.e(TAG, "******************选择MF_3错误******************");
                        Message message = new Message();
                        message.what = NFC_READCARD_FAIL;
                        message.obj = "选择MF_3错误";
                        mHandler.sendMessage(message);
                        return;

                    }
                    //读取DN码
                    byte[] bytes_4 = isodep.transceive(CommandUtils.c_4);
                    String hex_4 = CodingUtils.byte2hex(bytes_4);
                    //Log.i(TAG, "读取DN码_4:" + hex_4);
                    if (!CommandUtils.endWith(bytes_4, CommandUtils.success)) {
                        Log.e(TAG, "******************读取DN码_4错误******************");
                        Message message = new Message();
                        message.what = NFC_READCARD_FAIL;
                        message.obj = "读取DN码_4错误";
                        mHandler.sendMessage(message);
                        return;

                    }


                    *//**
         * MBL->SVR(81B)
         *//*
                    byte[] bytes_head = new byte[]{
                            (byte) 0x50, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00
                    };

                    byte[] applicationData = isodep.getApplicationData();
                    byte[] protocolInfo = isodep.getProtocolInfo();

                    byte[] bytes_data = new byte[13];
                    System.arraycopy(bytes_head, 0, bytes_data, 0, bytes_head.length);
                    System.arraycopy(applicationData, 0, bytes_data, bytes_head.length, applicationData.length);
                    System.arraycopy(protocolInfo, 0, bytes_data, bytes_head.length + applicationData.length, protocolInfo.length);
                    bytes_data[12] = (byte) 0x00;

                    String hex_bytes_data = CodingUtils.byte2hex(bytes_data);
                    //Log.i(TAG, "REQB/ATQB数据:" + hex_bytes_data);

                    byte[] bytes_3100 = new byte[]{
                            (byte) 0x31, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00
                    };

                    byte[][] params_bytes = new byte[][]{
                            bytes_data, bytes_4
                    };
                    byte[] b = CommandUtils.MblToSvr((byte) 0x11, params_bytes, 56);

                    byte[] data_81 = new byte[81];

                    System.arraycopy(b, 0, data_81, 0, b.length);
                    System.arraycopy(bytes_3100, 0, data_81, b.length, bytes_3100.length);

                    String hex_b = CodingUtils.byte2hex(data_81);
                    //Log.i(TAG, "发送DN码到服务器:" + hex_b);

                    tcpDataService.sendData(data_81, data_81.length);

                    boolean loop_recv = true;
                    byte[] bytes_zp = null;
                    byte[] bytes_zw = null;
                    boolean start_svrdata = false;
                    byte[] receive_data = null;
                    byte[] receive_data_520 = null;
                    byte[] receive_data_516 = null;

                    while (loop_recv) {
                        byte[] SSvrCmd = new byte[2];
                        r = tcpDataService.recData(SSvrCmd, SSvrCmd.length);
                        if (r == -1) {
                            Log.e(TAG, "******************网络接收错误******************");
                            Message message = new Message();
                            message.what = NFC_NET_CONNECT_ERROR;
                            message.obj = "网络接收错误";
                            mHandler.sendMessage(message);
                            return;
                        }
//                        Log.e(TAG, "******************接收指令type:"+SSvrCmd[0]+",cmd:"+SSvrCmd[1]+"*****************");
                        switch (SSvrCmd[0]) {
                            case SSvrCMD.SVR_CMDTYPE_SENDCDM:
                                switch (SSvrCmd[1]) {
                                    case SSvrCMD.SVR_CMD_INCERT:
                                        byte[] incert = new byte[17];
                                        r = tcpDataService.recData(incert, incert.length);
                                        if (r == -1) {
                                            Log.e(TAG, "******************网络接收错误******************");
                                            Message message = new Message();
                                            message.what = NFC_NET_CONNECT_ERROR;
                                            message.obj = "网络接收错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }
                                        String hex_bytes2 = CodingUtils.byte2hex(incert);
                                        //Log.i(TAG, "接收服务器内部认证指令:" + hex_bytes2);
                                        if (incert[2] != (byte) 0x00 || incert[3] != (byte)0x88) {
                                            Log.e(TAG, "******************接收内部认证指令错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "接收内部认证指令错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }
                                        byte[] n_rz = new byte[15];
                                        System.arraycopy(incert, 2, n_rz, 0, n_rz.length);
                                        String n_rz2 = CodingUtils.byte2hex(n_rz);
                                        //Log.i(TAG, "获取本地内部认证指令数据:" + n_rz2);

                                        byte[] bytes_5 = isodep.transceive(n_rz);
                                        String hex_5 = CodingUtils.byte2hex(bytes_5);
                                        //Log.i(TAG, "获取本地内部认证返回:" + hex_5);

                                        if (!CommandUtils.endWith(bytes_5, CommandUtils.success)) {
                                            Log.e(TAG, "******************执行内部认证指令错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "执行内部认证指令错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }

                                        byte[] bytes_6 = isodep.transceive(CommandUtils.c_6);
                                        String hex_6 = CodingUtils.byte2hex(bytes_6);
                                        //Log.i(TAG, "获取本地随机数:" + hex_6);

                                        if (!CommandUtils.endWith(bytes_6, CommandUtils.success)) {
                                            Log.e(TAG, "******************获取随机数错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "获取随机数错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }
                                        byte[][] params_rz_bytes = new byte[][]{
                                                bytes_5, bytes_6
                                        };
                                        byte[] b_n_rz = CommandUtils.MblToSvr((byte) 0x12, params_rz_bytes, 30);
                                        String b_n_rz_hex_6 = CodingUtils.byte2hex(b_n_rz);
                                        //Log.i(TAG, "向服务器提交内部认证数据:" + b_n_rz_hex_6);

                                        tcpDataService.sendData(b_n_rz, b_n_rz.length);
                                        break;
                                    case SSvrCMD.SVR_CMD_OUTCERT:
                                        byte[] n_rz_receive = new byte[17];
                                        r = tcpDataService.recData(n_rz_receive, n_rz_receive.length);
                                        if (r == -1) {
                                            Log.e(TAG, "******************网络接收错误******************");
                                            Message message = new Message();
                                            message.what = NFC_NET_CONNECT_ERROR;
                                            message.obj = "网络接收错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }
                                        String n_rz_receive_hex = CodingUtils.byte2hex(n_rz_receive);
                                        //Log.i(TAG, "接收内部认证服务器返回:" + n_rz_receive_hex);

                                        if (n_rz_receive[2] != (byte) 0x00 || n_rz_receive[3] != (byte) 0x82) {
                                            Log.e(TAG, "******************接收外部认证指令错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "接收外部认证指令错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }
                                        *//**
         * 外部认证
         *//*
                                        byte[] w_rz = new byte[n_rz_receive.length - 2];
                                        System.arraycopy(n_rz_receive, 2, w_rz, 0, w_rz.length);
                                        String w_rz2 = CodingUtils.byte2hex(w_rz);
                                        //Log.i(TAG, "获取本地外部认证指令数据:" + w_rz2);

                                        if (!start_svrdata) {
                                            byte[] bytes_7 = isodep.transceive(w_rz);
                                            String hex_7 = CodingUtils.byte2hex(bytes_7);
                                            //Log.i(TAG, "获取本地外部认证返回:" + hex_7);

                                            if (!CommandUtils.endWith(bytes_7, CommandUtils.success)) {
                                                Log.e(TAG, "******************获取本地外部认证返回错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "获取本地外部认证返回错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }

                                            byte[] bytes_8 = isodep.transceive(CommandUtils.c_8);
                                            String hex_8 = CodingUtils.byte2hex(bytes_8);
                                            //Log.i(TAG, "选择6011MF:" + hex_8);

                                            if (!CommandUtils.endWith(bytes_8, CommandUtils.success)) {
                                                Log.e(TAG, "******************选择6011MF错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "选择6011MF错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }

                                            byte[] bytes_9 = isodep.transceive(CommandUtils.c_9);
                                            if (!CommandUtils.endWith(bytes_9, CommandUtils.success)) {
                                                Log.e(TAG, "******************读取6011数据_1错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "读取6011数据_1错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }
                                            bytes_9 = CommandUtils.subWithEnd(bytes_9, CommandUtils.success);
                                            String hex_9 = CodingUtils.byte2hex(bytes_9);
                                            //Log.i(TAG, "读取6011数据_1:" + hex_9);


                                            byte[] bytes_10 = isodep.transceive(CommandUtils.c_10);
                                            if (!CommandUtils.endWith(bytes_10, CommandUtils.success)) {
                                                Log.e(TAG, "******************读取6011数据_2错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "读取6011数据_2错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }
                                            bytes_10 = CommandUtils.subWithEnd(bytes_10, CommandUtils.success);
                                            String hex_10 = CodingUtils.byte2hex(bytes_10);
                                            //Log.i(TAG, "读取6011数据_2:" + hex_10);


                                            byte[] bytes6011 = new byte[bytes_9.length + bytes_10.length];
                                            System.arraycopy(bytes_9, 0, bytes6011, 0, bytes_9.length);
                                            System.arraycopy(bytes_10, 0, bytes6011, bytes_9.length, bytes_10.length);

                                            byte[] bytes_11 = isodep.transceive(CommandUtils.c_11);
                                            String hex_11 = CodingUtils.byte2hex(bytes_11);
                                            //Log.i(TAG, "选择6012MF:" + hex_11);

                                            if (!CommandUtils.endWith(bytes_11, CommandUtils.success)) {
                                                Log.e(TAG, "******************选择6012MF错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "选择6012MF错误";
                                                mHandler.sendMessage(message);
                                                return;
                                            }

                                            byte[] bytes_12 = isodep.transceive(CommandUtils.c_12);
                                            if (!CommandUtils.endWith(bytes_12, CommandUtils.success)) {
                                                Log.e(TAG, "******************读取6012数据_1错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "读取6012数据_1错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }
                                            bytes_12 = CommandUtils.subWithEnd(bytes_12, CommandUtils.success);
                                            String hex_12 = CodingUtils.byte2hex(bytes_12);
                                            //Log.i(TAG, "读取6012数据_1:" + hex_12);


                                            byte[] bytes_13 = isodep.transceive(CommandUtils.c_13);
                                            if (!CommandUtils.endWith(bytes_13, CommandUtils.success)) {
                                                Log.e(TAG, "******************读取6012数据_2错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "读取6012数据_2错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }
                                            bytes_13 = CommandUtils.subWithEnd(bytes_13, CommandUtils.success);
                                            String hex_13 = CodingUtils.byte2hex(bytes_13);
                                            //Log.i(TAG, "读取6012数据_2:" + hex_13);


                                            byte[] bytes_14 = isodep.transceive(CommandUtils.c_14);
                                            if (!CommandUtils.endWith(bytes_14, CommandUtils.success)) {
                                                Log.e(TAG, "******************读取6012数据_3错误******************");
                                                Message message = new Message();
                                                message.what = NFC_READCARD_FAIL;
                                                message.obj = "读取6012数据_3错误";
                                                mHandler.sendMessage(message);
                                                return;

                                            }
                                            bytes_14 = CommandUtils.subWithEnd(bytes_14, CommandUtils.success);
                                            String hex_14 = CodingUtils.byte2hex(bytes_14);
                                            //Log.i(TAG, "读取6012数据_3:" + hex_14);


                                            byte[] bytes6012 = new byte[bytes_12.length + bytes_13.length + bytes_14.length];
                                            System.arraycopy(bytes_12, 0, bytes6012, 0, bytes_12.length);
                                            System.arraycopy(bytes_13, 0, bytes6012, bytes_12.length, bytes_13.length);
                                            System.arraycopy(bytes_14, 0, bytes6012, bytes_12.length + bytes_13.length, bytes_14.length);

                                            byte[][] params_60_bytes = new byte[][]{
                                                    bytes6011,
                                                    bytes6012
                                            };

                                            byte[] bytes_6011_6012 = CommandUtils.MblToSvr((byte) 0x13, params_60_bytes, 478);
                                            String hex_bytes_6011_6012 = CodingUtils.byte2hex(bytes_6011_6012);
                                            //Log.i(TAG, "6011_6012合并数据:" + hex_bytes_6011_6012);

                                            tcpDataService.sendData(bytes_6011_6012, bytes_6011_6012.length);
                                        }

                                        break;
                                    case SSvrCMD.SVR_CMD_MSG_ACK:
                                        byte[] bytes_15 = isodep.transceive(CommandUtils.c_15);
                                        String hex_15 = CodingUtils.byte2hex(bytes_15);
                                        //Log.i(TAG, "选择主文件6013:" + hex_15);

                                        if (!CommandUtils.endWith(bytes_15, CommandUtils.success)) {
                                            Log.e(TAG, "******************选择主文件6013错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "选择主文件6013错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }

                                        byte[] bytes_16 = isodep.transceive(CommandUtils.c_16);
                                        if (!CommandUtils.endWith(bytes_16, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_1错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_1错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_16 = CommandUtils.subWithEnd(bytes_16, CommandUtils.success);
                                        String hex_16 = CodingUtils.byte2hex(bytes_16);
                                        //Log.i(TAG, "照片数据_1:" + hex_16);


                                        byte[] bytes_17 = isodep.transceive(CommandUtils.c_17);
                                        if (!CommandUtils.endWith(bytes_17, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_2错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_2错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_17 = CommandUtils.subWithEnd(bytes_17, CommandUtils.success);
                                        String hex_17 = CodingUtils.byte2hex(bytes_17);
                                        //Log.i(TAG, "照片数据_2:" + hex_17);


                                        byte[] bytes_18 = isodep.transceive(CommandUtils.c_18);
                                        if (!CommandUtils.endWith(bytes_18, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_3错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_3错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_18 = CommandUtils.subWithEnd(bytes_18, CommandUtils.success);
                                        String hex_18 = CodingUtils.byte2hex(bytes_18);
                                        //Log.i(TAG, "照片数据_3:" + hex_18);


                                        byte[] bytes_19 = isodep.transceive(CommandUtils.c_19);
                                        if (!CommandUtils.endWith(bytes_19, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_4错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_4错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_19 = CommandUtils.subWithEnd(bytes_19, CommandUtils.success);
                                        String hex_19 = CodingUtils.byte2hex(bytes_19);
                                        //Log.i(TAG, "照片数据_4:" + hex_19);


                                        byte[] bytes_20 = isodep.transceive(CommandUtils.c_20);
                                        if (!CommandUtils.endWith(bytes_20, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_5错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_5错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_20 = CommandUtils.subWithEnd(bytes_20, CommandUtils.success);
                                        String hex_20 = CodingUtils.byte2hex(bytes_20);
                                        //Log.i(TAG, "照片数据_5:" + hex_20);


                                        byte[] bytes_21 = isodep.transceive(CommandUtils.c_21);
                                        if (!CommandUtils.endWith(bytes_21, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_6错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_6错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_21 = CommandUtils.subWithEnd(bytes_21, CommandUtils.success);
                                        String hex_21 = CodingUtils.byte2hex(bytes_21);
                                        //Log.i(TAG, "照片数据_6:" + hex_21);


                                        byte[] bytes_22 = isodep.transceive(CommandUtils.c_22);
                                        if (!CommandUtils.endWith(bytes_22, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_7错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_7错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_22 = CommandUtils.subWithEnd(bytes_22, CommandUtils.success);
                                        String hex_22 = CodingUtils.byte2hex(bytes_22);
                                        //Log.i(TAG, "照片数据_7:" + hex_22);


                                        byte[] bytes_23 = isodep.transceive(CommandUtils.c_23);
                                        if (!CommandUtils.endWith(bytes_23, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_8错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_8错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_23 = CommandUtils.subWithEnd(bytes_23, CommandUtils.success);
                                        String hex_23 = CodingUtils.byte2hex(bytes_23);
                                        //Log.i(TAG, "照片数据_8:" + hex_23);


                                        byte[] bytes_24 = isodep.transceive(CommandUtils.c_24);
                                        if (!CommandUtils.endWith(bytes_24, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_9错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_9错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_24 = CommandUtils.subWithEnd(bytes_24, CommandUtils.success);
                                        String hex_24 = CodingUtils.byte2hex(bytes_24);
                                        //Log.i(TAG, "照片数据_9:" + hex_24);


                                        byte[] bytes_25 = isodep.transceive(CommandUtils.c_25);
                                        if (!CommandUtils.endWith(bytes_25, CommandUtils.success)) {
                                            Log.e(TAG, "******************照片数据_10错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "照片数据_10错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_25 = CommandUtils.subWithEnd(bytes_25, CommandUtils.success);
                                        String hex_25 = CodingUtils.byte2hex(bytes_25);
                                        //Log.i(TAG, "照片数据_10:" + hex_25);


                                        bytes_zp = new byte[
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length +
                                                        bytes_20.length +
                                                        bytes_21.length +
                                                        bytes_22.length +
                                                        bytes_23.length +
                                                        bytes_24.length +
                                                        bytes_25.length
                                                ];
                                        System.arraycopy(bytes_16, 0, bytes_zp, 0, bytes_16.length);
                                        System.arraycopy(bytes_17, 0, bytes_zp,
                                                bytes_16.length,
                                                bytes_17.length);
                                        System.arraycopy(bytes_18, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length,
                                                bytes_18.length);
                                        System.arraycopy(bytes_19, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length,
                                                bytes_19.length);
                                        System.arraycopy(bytes_20, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length,
                                                bytes_20.length);
                                        System.arraycopy(bytes_21, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length +
                                                        bytes_20.length,
                                                bytes_21.length);
                                        System.arraycopy(bytes_22, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length +
                                                        bytes_20.length +
                                                        bytes_21.length,
                                                bytes_22.length);
                                        System.arraycopy(bytes_23, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length +
                                                        bytes_20.length +
                                                        bytes_21.length +
                                                        bytes_22.length,
                                                bytes_23.length);
                                        System.arraycopy(bytes_24, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length +
                                                        bytes_20.length +
                                                        bytes_21.length +
                                                        bytes_22.length +
                                                        bytes_23.length,
                                                bytes_24.length);
                                        System.arraycopy(bytes_25, 0, bytes_zp,
                                                bytes_16.length +
                                                        bytes_17.length +
                                                        bytes_18.length +
                                                        bytes_19.length +
                                                        bytes_20.length +
                                                        bytes_21.length +
                                                        bytes_22.length +
                                                        bytes_23.length +
                                                        bytes_24.length,
                                                bytes_25.length);

                                        //Log.i(TAG, "照片数据总长度:" + bytes_zp.length);

                                        byte[] first_zp = CommandUtils.subBytes(bytes_zp, 0, 514);
                                        byte[] first_zp_data = CommandUtils.MblSplitToSvr((byte) 0x24, first_zp, 1024);
                                        //Log.i(TAG, "照片第一包数据长度:" + first_zp_data.length);

                                        tcpDataService.sendData(first_zp_data, first_zp_data.length);
                                        break;
                                    case SSvrCMD.SVR_CMD_RDPHOTO_ACK1:
                                        byte[] second_zp = CommandUtils.subBytes(bytes_zp, 514, bytes_zp.length);
                                        byte[] second_zp_data = CommandUtils.MblSplitToSvr((byte) 0x25, second_zp, 1024);
                                        //Log.i(TAG, "照片第二包数据长度:" + second_zp_data.length);
                                        tcpDataService.sendData(second_zp_data, second_zp_data.length);
                                        break;
                                    case SSvrCMD.SVR_CMD_RDPHOTO_ACK2:
                                        byte[] bytes_26 = isodep.transceive(CommandUtils.c_26);
                                        String hex_26 = CodingUtils.byte2hex(bytes_26);
                                        //Log.i(TAG, "6021选择指纹文件:" + hex_26);

                                        if (!CommandUtils.endWith(bytes_26, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021选择指纹文件错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021选择指纹文件错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }

                                        byte[] bytes_27 = isodep.transceive(CommandUtils.c_27);
                                        if (!CommandUtils.endWith(bytes_27, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹1错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹1错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_27 = CommandUtils.subWithEnd(bytes_27, CommandUtils.success);
                                        String hex_27 = CodingUtils.byte2hex(bytes_27);
                                        //Log.i(TAG, "6021指纹1:" + hex_27);


                                        byte[] bytes_28 = isodep.transceive(CommandUtils.c_28);
                                        if (!CommandUtils.endWith(bytes_28, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹2错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹2错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_28 = CommandUtils.subWithEnd(bytes_28, CommandUtils.success);
                                        String hex_28 = CodingUtils.byte2hex(bytes_28);
                                        //Log.i(TAG, "6021指纹2:" + hex_28);


                                        byte[] bytes_29 = isodep.transceive(CommandUtils.c_29);
                                        if (!CommandUtils.endWith(bytes_29, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹3错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹3错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_29 = CommandUtils.subWithEnd(bytes_29, CommandUtils.success);
                                        String hex_29 = CodingUtils.byte2hex(bytes_29);
                                        //Log.i(TAG, "6021指纹3:" + hex_29);


                                        byte[] bytes_30 = isodep.transceive(CommandUtils.c_30);
                                        if (!CommandUtils.endWith(bytes_30, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹4错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹4错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_30 = CommandUtils.subWithEnd(bytes_30, CommandUtils.success);
                                        String hex_30 = CodingUtils.byte2hex(bytes_30);
                                        //Log.i(TAG, "6021指纹4:" + hex_30);


                                        byte[] bytes_31 = isodep.transceive(CommandUtils.c_31);
                                        if (!CommandUtils.endWith(bytes_31, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹5错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹5错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_31 = CommandUtils.subWithEnd(bytes_31, CommandUtils.success);
                                        String hex_31 = CodingUtils.byte2hex(bytes_31);
                                        //Log.i(TAG, "6021指纹5:" + hex_31);


                                        byte[] bytes_32 = isodep.transceive(CommandUtils.c_32);
                                        if (!CommandUtils.endWith(bytes_32, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹6错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹6错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_32 = CommandUtils.subWithEnd(bytes_32, CommandUtils.success);
                                        String hex_32 = CodingUtils.byte2hex(bytes_32);
                                        //Log.i(TAG, "6021指纹6:" + hex_32);


                                        byte[] bytes_33 = isodep.transceive(CommandUtils.c_33);
                                        if (!CommandUtils.endWith(bytes_33, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹7错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹7错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_33 = CommandUtils.subWithEnd(bytes_33, CommandUtils.success);
                                        String hex_33 = CodingUtils.byte2hex(bytes_33);
                                        //Log.i(TAG, "6021指纹7:" + hex_33);


                                        byte[] bytes_34 = isodep.transceive(CommandUtils.c_34);
                                        if (!CommandUtils.endWith(bytes_34, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹8错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹8错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_34 = CommandUtils.subWithEnd(bytes_34, CommandUtils.success);
                                        String hex_34 = CodingUtils.byte2hex(bytes_34);
                                        //Log.i(TAG, "6021指纹8:" + hex_34);


                                        byte[] bytes_35 = isodep.transceive(CommandUtils.c_35);
                                        if (!CommandUtils.endWith(bytes_35, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹9错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹9错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_35 = CommandUtils.subWithEnd(bytes_35, CommandUtils.success);
                                        String hex_35 = CodingUtils.byte2hex(bytes_35);
                                        //Log.i(TAG, "6021指纹9:" + hex_35);


                                        byte[] bytes_36 = isodep.transceive(CommandUtils.c_36);
                                        if (!CommandUtils.endWith(bytes_36, CommandUtils.success)) {
                                            Log.e(TAG, "******************6021指纹10错误******************");
                                            Message message = new Message();
                                            message.what = NFC_READCARD_FAIL;
                                            message.obj = "6021指纹10错误";
                                            mHandler.sendMessage(message);
                                            return;

                                        }
                                        bytes_36 = CommandUtils.subWithEnd(bytes_36, CommandUtils.success);
                                        String hex_36 = CodingUtils.byte2hex(bytes_36);
                                        //Log.i(TAG, "6021指纹10:" + hex_36);


                                        bytes_zw = new byte[
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length +
                                                        bytes_31.length +
                                                        bytes_32.length +
                                                        bytes_33.length +
                                                        bytes_34.length +
                                                        bytes_35.length +
                                                        bytes_36.length
                                                ];
                                        System.arraycopy(bytes_27, 0, bytes_zw, 0, bytes_27.length);

                                        System.arraycopy(bytes_28, 0, bytes_zw,
                                                bytes_27.length,
                                                bytes_28.length);

                                        System.arraycopy(bytes_29, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length,
                                                bytes_29.length);

                                        System.arraycopy(bytes_30, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length,
                                                bytes_30.length);
                                        System.arraycopy(bytes_31, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length,
                                                bytes_31.length);
                                        System.arraycopy(bytes_32, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length +
                                                        bytes_31.length,
                                                bytes_32.length);
                                        System.arraycopy(bytes_33, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length +
                                                        bytes_31.length +
                                                        bytes_32.length,
                                                bytes_33.length);
                                        System.arraycopy(bytes_34, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length +
                                                        bytes_31.length +
                                                        bytes_32.length +
                                                        bytes_33.length,
                                                bytes_34.length);
                                        System.arraycopy(bytes_35, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length +
                                                        bytes_31.length +
                                                        bytes_32.length +
                                                        bytes_33.length +
                                                        bytes_34.length,
                                                bytes_35.length);
                                        System.arraycopy(bytes_36, 0, bytes_zw,
                                                bytes_27.length +
                                                        bytes_28.length +
                                                        bytes_29.length +
                                                        bytes_30.length +
                                                        bytes_31.length +
                                                        bytes_32.length +
                                                        bytes_33.length +
                                                        bytes_34.length +
                                                        bytes_35.length,
                                                bytes_36.length);


                                        //Log.i(TAG, "指纹数据总长度:" + bytes_zw.length);

                                        byte[] first_zw = CommandUtils.subBytes(bytes_zw, 0, 514);
                                        byte[] first_zw_data = CommandUtils.MblSplitToSvr((byte) 0x28, first_zw, 1024);
                                        //Log.i(TAG, "指纹第一包数据长度:" + first_zw_data.length);

                                        tcpDataService.sendData(first_zw_data, first_zw_data.length);

                                        break;
                                    case SSvrCMD.SVR_CMD_RDFINGERPRINT_ACK1:
                                        byte[] second_zw = CommandUtils.subBytes(bytes_zw, 514, bytes_zw.length);
                                        byte[] second_zw_data = CommandUtils.MblSplitToSvr((byte) 0x29, second_zw, 1024);
                                        //Log.i(TAG, "指纹第二包数据长度:" + second_zw_data.length);
                                        tcpDataService.sendData(second_zw_data, second_zw_data.length);
                                        break;
                                    case SSvrCMD.SVR_CMD_RDFINGERPRINT_ACK2:
                                        *//**
         * 第二次认证
         *//*
                                        byte[][] params_bytes_ = new byte[][]{
                                                bytes_data, bytes_4
                                        };
                                        byte[] b_ = CommandUtils.MblToSvr((byte) 0x21, params_bytes_, 56);
                                        String hex_b_ = CodingUtils.byte2hex(b_);
                                        //Log.i(TAG, "第二次认证确认数据:" + hex_b_);
                                        tcpDataService.sendData(b_, b_.length);
                                        start_svrdata = true;
                                        break;
                                }
                                break;
                            case SSvrCMD.SVR_CMDTYPE_SENDDATA:
                                int packs = 0;
                                int packno = 0;
                                int packlen = 0;
                                byte[] rbyte = new byte[4];

                                r = tcpDataService.recData(rbyte, rbyte.length);
                                if (r == -1) {
                                    Log.e(TAG, "******************网络接收错误******************");
                                    Message message = new Message();
                                    message.what = NFC_NET_CONNECT_ERROR;
                                    message.obj = "网络接收错误";
                                    mHandler.sendMessage(message);
                                    return;
                                }

                                packs = rbyte[0]; //包数
                                packno = rbyte[1];//包序

                                byte[] len = new byte[2];
                                System.arraycopy(rbyte, 2, len, 0, len.length);

                                packlen = len[0] << 8;
                                packlen |= (len[1] & 0x000000ff);


                                if (packs > 0 && packno > 0 && packno <= packs) {
                                    if (packno == 1) {//身份信息
                                        receive_data = new byte[packlen + 6];
                                        byte[] receive_data_temp = new byte[packlen];
                                        r = tcpDataService.recData(receive_data_temp, receive_data_temp.length);
                                        if (r == -1) {
                                            Log.e(TAG, "******************网络接收错误******************");
                                            Message message = new Message();
                                            message.what = NFC_NET_CONNECT_ERROR;
                                            message.obj = "网络接收错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }
                                        System.arraycopy(SSvrCmd, 0, receive_data, 0, SSvrCmd.length);
                                        System.arraycopy(rbyte, 0, receive_data, SSvrCmd.length, rbyte.length);
                                        System.arraycopy(receive_data_temp, 0, receive_data, SSvrCmd.length + rbyte.length, receive_data_temp.length);
                                        String receive_data_ = CodingUtils.byte2hex(receive_data);
                                        //Log.i(TAG, "获取服务器返回数据1:" + receive_data_ + "长度：" + receive_data.length);

                                        byte[] bytes_41 = new byte[]{0x41, 0x00};
                                        tcpDataService.sendData(bytes_41, bytes_41.length);

                                    } else if (packno == 2) {
                                        receive_data_520 = new byte[packlen + 6];
                                        byte[] receive_data_520_temp = new byte[packlen];
                                        r = tcpDataService.recData(receive_data_520_temp, receive_data_520_temp.length);
                                        if (r == -1) {
                                            Log.e(TAG, "******************网络接收错误******************");
                                            Message message = new Message();
                                            message.what = NFC_NET_CONNECT_ERROR;
                                            message.obj = "网络接收错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }

                                        System.arraycopy(SSvrCmd, 0, receive_data_520, 0, SSvrCmd.length);
                                        System.arraycopy(rbyte, 0, receive_data_520, SSvrCmd.length, rbyte.length);
                                        System.arraycopy(receive_data_520_temp, 0, receive_data_520, SSvrCmd.length + rbyte.length, receive_data_520_temp.length);


                                        String receive_data_520_ = CodingUtils.byte2hex(receive_data_520);
                                        //Log.i(TAG, "获取服务器返回数据2:" + receive_data_520_ + "长度：" + receive_data_520.length);

                                        byte[] bytes_42 = new byte[]{0x42, 0x00};
                                        tcpDataService.sendData(bytes_42, bytes_42.length);
                                    } else if (packno == 3) {
                                        receive_data_516 = new byte[packlen + 6];
                                        byte[] receive_data_516_temp = new byte[packlen];
                                        r = tcpDataService.recData(receive_data_516_temp, receive_data_516_temp.length);
                                        if (r == -1) {
                                            Log.e(TAG, "******************网络接收错误******************");
                                            Message message = new Message();
                                            message.what = NFC_NET_CONNECT_ERROR;
                                            message.obj = "网络接收错误";
                                            mHandler.sendMessage(message);
                                            return;
                                        }

                                        System.arraycopy(SSvrCmd, 0, receive_data_516, 0, SSvrCmd.length);
                                        System.arraycopy(rbyte, 0, receive_data_516, SSvrCmd.length, rbyte.length);
                                        System.arraycopy(receive_data_516_temp, 0, receive_data_516, SSvrCmd.length + rbyte.length, receive_data_516_temp.length);

                                        String receive_data_516_ = CodingUtils.byte2hex(receive_data_516);
                                        //Log.i(TAG, "获取服务器返回数据3:" + receive_data_516_ + "长度：" + receive_data_516.length);

                                        loop_recv = false;

                                        byte[] bytes_43 = new byte[]{0x43, 0x00};
                                        tcpDataService.sendData(bytes_43, bytes_43.length);
                                    }
                                }
                                break;
                        }
                    }


                    byte[] msg = new byte[2048];
                    int result = nfcDecode.decryptInfo(receive_data, receive_data_520, receive_data_516, msg);
                    if (result == 0) {
                        callBackFinish(msg);
                    } else {
                        Message message = new Message();
                        message.what = DEC_DATA_ERROR;
                        message.obj = "解码数据出错";
                        mHandler.sendMessage(message);
                        return;
                    }

                } catch (Exception e) {
                    e.printStackTrace();
                    Message message = new Message();
                    message.what = NFC_READCARD_FAIL;
                    message.obj = "NFC读卡失败，出现异常";
                    mHandler.sendMessage(message);
                    return;
                } finally {
                    tcpDataService.disconnect();
                    try {
                        isodep.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();*/
    }
}
