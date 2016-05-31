package com.lz.nfc.utils;

import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;

/**
 * Created by Administrator on 16-1-8.
 */
public class StreamUtils {

    public static byte[] input2byte(String file){
        try {
            FileInputStream fileInputStream = new FileInputStream(file);
            ByteArrayOutputStream swapStream = new ByteArrayOutputStream();
            byte[] buff = new byte[100];
            int rc = 0;
            while ((rc = fileInputStream.read(buff, 0, 100)) > 0) {
                swapStream.write(buff, 0, rc);
            }

            byte[] in2b = swapStream.toByteArray();
            fileInputStream.close();
            swapStream.flush();
            swapStream.close();
            return in2b;
        }catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }


    public static void copyInputStream(InputStream in, String to) {
        FileOutputStream lic_data_out = null;
        try {
            lic_data_out = new FileOutputStream(to);
            byte[] basebytes = new byte[1024];
            int baseread = -1;
            while ((baseread = in.read(basebytes)) != -1) {
                lic_data_out.write(basebytes, 0, baseread);
            }
            lic_data_out.close();
            in.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
