package com.lz.nfc.utils;

/**
 * Created by Administrator on 16-1-8.
 */
public class MessageUtils {

    public static String getMsg(int i){
        switch (i) {
            case 1:
                return "网络连接错误";
            case 2:
                return "网络接收错误";
            case 3:
                return "网络发送错误";
            case 4:
                return "NFC打开错误";
            case 5:
                return "NFC读卡错误";
            case 6:
                return "认证失败";
            case 7:
                return "NFC寻卡失败";
            case 8:
                return "启动网络读卡失败";
            case 9:
                return "无效的网络命令";
            case 10:
                return "未知错误";
            case 11:
                return "读取信息成功";
            case 12:
                return "读取照片成功";
            default:
                return "未知错误";
        }
    }

    public static String getMinZu(String paramString){
        String str1 = "未定义";
        String str2 = "00";
        if (paramString.trim().equals(str2)) {
            str1 = "未定义";
        }
        str2 = "01";
        if (paramString.trim().equals(str2)) {
            str1 = "汉";
        }
        str2 = "02";
        if (paramString.trim().equals(str2)) {
            str1 = "蒙古";
        }
        str2 = "03";
        if (paramString.trim().equals(str2)) {
            str1 = "回";
        }
        str2 = "04";
        if (paramString.trim().equals(str2)) {
            str1 = "藏";
        }
        str2 = "05";
        if (paramString.trim().equals(str2)) {
            str1 = "维吾尔";
        }
        str2 = "06";
        if (paramString.trim().equals(str2)) {
            str1 = "苗";
        }
        str2 = "07";
        if (paramString.trim().equals(str2)) {
            str1 = "彝";
        }
        str2 = "08";
        if (paramString.trim().equals(str2)) {
            str1 = "壮";
        }
        str2 = "09";
        if (paramString.trim().equals(str2)) {
            str1 = "布依";
        }
        str2 = "10";
        if (paramString.trim().equals(str2)) {
            str1 = "朝鲜";
        }
        str2 = "11";
        if (paramString.trim().equals(str2)) {
            str1 = "满";
        }
        str2 = "12";
        if (paramString.trim().equals(str2)) {
            str1 = "侗";
        }
        str2 = "13";
        if (paramString.trim().equals(str2)) {
            str1 = "瑶";
        }
        str2 = "14";
        if (paramString.trim().equals(str2)) {
            str1 = "白";
        }
        str2 = "15";
        if (paramString.trim().equals(str2)) {
            str1 = "土家";
        }
        str2 = "16";
        if (paramString.trim().equals(str2)) {
            str1 = "哈尼";
        }
        str2 = "17";
        if (paramString.trim().equals(str2)) {
            str1 = "哈萨克";
        }
        str2 = "18";
        if (paramString.trim().equals(str2)) {
            str1 = "傣";
        }
        str2 = "19";
        if (paramString.trim().equals(str2)) {
            str1 = "黎";
        }
        str2 = "20";
        if (paramString.trim().equals(str2)) {
            str1 = "傈僳";
        }
        str2 = "21";
        if (paramString.trim().equals(str2)) {
            str1 = "佤";
        }
        str2 = "22";
        if (paramString.trim().equals(str2)) {
            str1 = "畲";
        }
        str2 = "23";
        if (paramString.trim().equals(str2)) {
            str1 = "高山";
        }
        str2 = "24";
        if (paramString.trim().equals(str2)) {
            str1 = "拉祜";
        }
        str2 = "25";
        if (paramString.trim().equals(str2)) {
            str1 = "水";
        }
        str2 = "26";
        if (paramString.trim().equals(str2)) {
            str1 = "东乡";
        }
        str2 = "27";
        if (paramString.trim().equals(str2)) {
            str1 = "纳西";
        }
        str2 = "28";
        if (paramString.trim().equals(str2)) {
            str1 = "景颇";
        }
        str2 = "29";
        if (paramString.trim().equals(str2)) {
            str1 = "柯尔克孜";
        }
        str2 = "30";
        if (paramString.trim().equals(str2)) {
            str1 = "土";
        }
        str2 = "31";
        if (paramString.trim().equals(str2)) {
            str1 = "达斡尔";
        }
        str2 = "32";
        if (paramString.trim().equals(str2)) {
            str1 = "仫佬";
        }
        str2 = "33";
        if (paramString.trim().equals(str2)) {
            str1 = "羌";
        }
        str2 = "34";
        if (paramString.trim().equals(str2)) {
            str1 = "布朗";
        }
        str2 = "35";
        if (paramString.trim().equals(str2)) {
            str1 = "撒拉";
        }
        str2 = "36";
        if (paramString.trim().equals(str2)) {
            str1 = "毛南";
        }
        str2 = "37";
        if (paramString.trim().equals(str2)) {
            str1 = "仡佬";
        }
        str2 = "38";
        if (paramString.trim().equals(str2)) {
            str1 = "锡伯";
        }
        str2 = "39";
        if (paramString.trim().equals(str2)) {
            str1 = "阿昌";
        }
        str2 = "4-";
        if (paramString.trim().equals(str2)) {
            str1 = "普米";
        }
        str2 = "41";
        if (paramString.trim().equals(str2)) {
            str1 = "塔吉克";
        }
        str2 = "42";
        if (paramString.trim().equals(str2)) {
            str1 = "怒";
        }
        str2 = "43";
        if (paramString.trim().equals(str2)) {
            str1 = "乌孜别克";
        }
        str2 = "44";
        if (paramString.trim().equals(str2)) {
            str1 = "俄罗斯";
        }
        str2 = "45";
        if (paramString.trim().equals(str2)) {
            str1 = "鄂温克";
        }
        str2 = "46";
        if (paramString.trim().equals(str2)) {
            str1 = "德昂";
        }
        str2 = "47";
        if (paramString.trim().equals(str2)) {
            str1 = "保安";
        }
        str2 = "48";
        if (paramString.trim().equals(str2)) {
            str1 = "裕固";
        }
        str2 = "49";
        if (paramString.trim().equals(str2)) {
            str1 = "京";
        }
        str2 = "50";
        if (paramString.trim().equals(str2)) {
            str1 = "塔塔尔";
        }
        str2 = "51";
        if (paramString.trim().equals(str2)) {
            str1 = "独龙";
        }
        str2 = "52";
        if (paramString.trim().equals(str2)) {
            str1 = "鄂伦春";
        }
        str2 = "53";
        if (paramString.trim().equals(str2)) {
            str1 = "赫哲";
        }
        str2 = "54";
        if (paramString.trim().equals(str2)) {
            str1 = "门巴";
        }
        str2 = "55";
        if (paramString.trim().equals(str2)) {
            str1 = "珞巴";
        }
        str2 = "56";
        if (paramString.trim().equals(str2)) {
            str1 = "基诺";
        }
        str2 = "57";
        if (paramString.trim().equals(str2)) {
            str1 = "其它";
        }
        return str1;
    }

    public static String getSex(String paramString){
        String str1 = "未知";
        String str2 = "0";
        if (paramString.trim().equals(str2)) {
            str1 = "未知";
        }
        str2 = "1";
        if (paramString.trim().equals(str2)) {
            str1 = "男";
        }
        str2 = "2";
        if (paramString.trim().equals(str2)) {
            str1 = "女";
        }
        str2 = "9";
        if (paramString.trim().equals(str2)) {
            str1 = "未说明";
        }
        return str1;
    }
}
