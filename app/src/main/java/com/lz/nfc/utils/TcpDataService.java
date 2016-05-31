package com.lz.nfc.utils;

import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class TcpDataService {
    private Socket socketin = null;
    private BufferedInputStream isall = null;
    private BufferedOutputStream osall = null;
    SocketAddress remoteAddr;

    /**
     * 建立TCP连接
     * @param ip
     * @param port
     * @return
     */
    public synchronized int connect(String ip, int port) {
        try {
            this.socketin = new Socket();
            this.remoteAddr = new InetSocketAddress(ip, port);
            this.socketin.connect(this.remoteAddr, 1000 * 2);
            this.isall = new BufferedInputStream(this.socketin.getInputStream());
            this.osall = new BufferedOutputStream(this.socketin.getOutputStream());
            return 1;
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("ERROR", "Socket connect failure, IP:" + ip + ", port:" + port);
            try {
                if (this.isall != null) {
                    this.socketin.shutdownInput();
                    this.isall.close();
                }

                if (this.osall != null) {
                    this.socketin.shutdownOutput();
                    this.osall.close();
                }

                if (this.socketin != null) {
                    this.socketin.close();
                }
            } catch (Exception e1) {
                e1.printStackTrace();
            }
            return 0;
        }
    }

    /**
     * 发送数据
     *
     * @param bytes
     * @param len
     * @return
     */
    public synchronized int sendData(byte[] bytes, int len) {
        try {
            if (this.osall != null) {
                this.osall.write(bytes, 0, len);
                this.osall.flush();
            }
            return 0;
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }

    /**
     * 断开连接
     */
    public void disconnect() {
        try {
            if (this.socketin != null) {
                this.socketin.close();
            }

            if (this.osall != null) {
                this.osall.close();
            }

            if (this.isall != null) {
                this.isall.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 接收数据
     * @param len
     * @return
     * @throws IOException
     */
    public int recData(byte[] bytes, int len) throws IOException {
        int ret = 0;
        int count = 0;
        if (this.isall != null) {
            this.socketin.setSoTimeout(1000 * 5);
            while (len > 0) {
                if ((ret = this.isall.read(bytes, count, len)) == -1)
                    return -1;
                len -= ret;
                count += ret;
            }
        }
        return count;
    }
}
