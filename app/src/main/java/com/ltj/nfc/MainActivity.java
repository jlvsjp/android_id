package com.ltj.nfc;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import com.lz.domain.IdentityCard;
import com.lz.nfc.jni.SelfCallbackNfc;
import com.lz.nfc.utils.Server;

import java.lang.reflect.Method;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    private Intent intent = null;
    public static final int MESSAGE_VALID_NFCBUTTON = 11; //开始读卡
    private Context context;
    private TextView name;
    private TextView nametext;
    private TextView sex;
    private TextView sextext;
    private TextView mingzu;
    private TextView mingzutext;
    private TextView birthday;
    private TextView birthdaytext;
    private TextView address;
    private TextView addresstext;
    private TextView number;
    private TextView numbertext;
    private TextView qianfa;
    private TextView qianfatext;
    private TextView validity;
    private TextView validitytext;
    private TextView dncode;
    private TextView dncodetext;
    private ImageView idimg;
    private TextView Readingtext;
    private SelfCallbackNfc selfCallbackNfc;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.nfc_layout);

        name = (TextView) findViewById(R.id.name);
        nametext = (TextView) findViewById(R.id.nametext);
        sex = (TextView) findViewById(R.id.sex);
        sextext = (TextView) findViewById(R.id.sextext);
        mingzu = (TextView) findViewById(R.id.mingzu);
        mingzutext = (TextView) findViewById(R.id.mingzutext);
        birthday = (TextView) findViewById(R.id.birthday);
        birthdaytext = (TextView) findViewById(R.id.birthdaytext);
        address = (TextView) findViewById(R.id.address);
        addresstext = (TextView) findViewById(R.id.addresstext);
        number = (TextView) findViewById(R.id.number);
        numbertext = (TextView) findViewById(R.id.numbertext);
        qianfa = (TextView) findViewById(R.id.qianfa);
        qianfatext = (TextView) findViewById(R.id.qianfatext);
        validity = (TextView) findViewById(R.id.validity);
        validitytext = (TextView) findViewById(R.id.validitytext);
        dncode = (TextView) findViewById(R.id.dncode);
        dncodetext = (TextView) findViewById(R.id.dncodetext);

        Readingtext = (TextView) findViewById(R.id.Readingtext);
        name.setText("姓名：");
        sex.setText("性别：");
        mingzu.setText("民族：");
        birthday.setText("出生年月：");
        address.setText("地址：");
        number.setText("身份证号码：");
        qianfa.setText("签发机关：");
        validity.setText("有效时间：");
        dncode.setText("DN码：");
        idimg = (ImageView) findViewById(R.id.idimg);
        Readingtext.setVisibility(View.GONE);
        Readingtext.setText("      正在读卡，请稍候...");
        Readingtext.setTextColor(Color.RED);

        this.context = this;
        selfCallbackNfc= new SelfCallbackNfc(mHandler,context);

        //读取配置
        SharedPreferences mSharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        String remote_ip = mSharedPrefs.getString("remote_ip", Server.otg_server);
        String remote_port = mSharedPrefs.getString("remote_port",Server.otg_port);


        selfCallbackNfc.setTheServer(remote_ip, Integer.parseInt(remote_port));
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            /*case R.id.action_settings:
                startActivity(new Intent(this, Otg.class));
//				finish();
                return true;*/
            case R.id.server_settings:
                startActivityForResult(new Intent(MainActivity.this, SetOtgActivity.class),1);
//				finish();
                return true;
        }
        return false;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        if (requestCode==1){
                //读取配置
                SharedPreferences mSharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
                String remote_ip = mSharedPrefs.getString("remote_ip", Server.otg_server);
                String remote_port = mSharedPrefs.getString("remote_port",Server.otg_port);
                selfCallbackNfc.setTheServer(remote_ip, Integer.parseInt(remote_port));

        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    /**
     * 设置menu显示icon
     */
    @Override
    public boolean onMenuOpened(int featureId, Menu menu)
    {

        if (featureId == Window.FEATURE_ACTION_BAR && menu != null)
        {
            if (menu.getClass().getSimpleName().equals("MenuBuilder"))
            {
                try
                {
                    Method m = menu.getClass().getDeclaredMethod(
                            "setOptionalIconsVisible", Boolean.TYPE);
                    m.setAccessible(true);
                    m.invoke(menu, true);
                } catch (Exception e)
                {
                    e.printStackTrace();
                }
            }
        }

        return super.onMenuOpened(featureId, menu);
    }



    @Override
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);

        this.intent = intent;

        nametext.setText("");
        sextext.setText("");
        mingzutext.setText("");
        birthdaytext.setText("");
        addresstext.setText("");
        numbertext.setText("");
        qianfatext.setText("");
        validitytext.setText("");
        dncodetext.setText("");

        idimg.setImageBitmap(null);
        Readingtext.setText("      正在读卡，请稍候...");
        Readingtext.setVisibility(View.VISIBLE);

        mHandler.sendEmptyMessageDelayed(MESSAGE_VALID_NFCBUTTON, 0);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Boolean enablenfc = selfCallbackNfc.EnableSystemNFCMessage();
        if (enablenfc != true) {
            Toast.makeText(getApplicationContext(), "NFC初始化失败", Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        selfCallbackNfc.DisableSystemNFCMessage();
    }

    private final Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MESSAGE_VALID_NFCBUTTON:
                    Boolean enablenfc = selfCallbackNfc.EnableSystemNFCMessage();
                    if (enablenfc == true) {
                        Boolean judgenfc = selfCallbackNfc.isNFC(intent);
                        if (judgenfc == true) {
                            selfCallbackNfc.readCardWithIntent(intent);
                        } else {
//                            Readingtext.setText("      读卡失败，非NFC读卡");
                            Readingtext.setVisibility(View.VISIBLE);
                        }
                    } else {
//                        Readingtext.setText("      读卡失败，未启用NFC");
                        Readingtext.setVisibility(View.VISIBLE);
                    }
                    break;

                case SelfCallbackNfc.READ_IDCARD_SUCCESS:
//                    Readingtext.setText("      读卡成功");
                    Readingtext.setVisibility(View.GONE);
                    IdentityCard identityCard = (IdentityCard) msg.obj;
                    nametext.setText(identityCard.getName());
                    sextext.setText(identityCard.getSex());
                    mingzutext.setText(identityCard.getEthnicity());
                    birthdaytext.setText(identityCard.getBirth());
                    addresstext.setText(identityCard.getAddress());
                    numbertext.setText(identityCard.getCardNo());
                    qianfatext.setText(identityCard.getAuthority());
                    validitytext.setText(identityCard.getPeriod());
                    dncodetext.setText(identityCard.getDN());
                    break;
                case SelfCallbackNfc.READ_PHOTO_SUCCESS:
//                    Readingtext.setText("      读照片成功");
                    Readingtext.setVisibility(View.GONE);
                    byte[] cardbmp = (byte[]) msg.obj;
                    Bitmap bm = BitmapFactory.decodeByteArray(cardbmp, 0, cardbmp.length);
                    idimg.setImageBitmap(bm);
                    break;
                case SelfCallbackNfc.NFC_NET_CONNECT_ERROR:
                    Readingtext.setText("      "+(String) msg.obj);
                    Readingtext.setVisibility(View.VISIBLE);
                    break;
                case SelfCallbackNfc.NFC_NET_STARTREADCARD_FAIL:
                    Readingtext.setText("      "+(String) msg.obj);
                    Readingtext.setVisibility(View.VISIBLE);
                    break;
                case SelfCallbackNfc.NFC_READCARD_FAIL:
                    Readingtext.setText("      "+(String) msg.obj);
                    Readingtext.setVisibility(View.VISIBLE);
                    break;
                case SelfCallbackNfc.PLASE_INIT_SERVER:
                    Readingtext.setText("      "+(String) msg.obj);
                    Readingtext.setVisibility(View.VISIBLE);
                    break;
                case SelfCallbackNfc.DEC_DATA_ERROR:
                    Readingtext.setText("      "+(String) msg.obj);
                    Readingtext.setVisibility(View.VISIBLE);
                    break;
                case SelfCallbackNfc.READ_PHOTO_ERROR:
                    Readingtext.setText("      "+(String) msg.obj);
                    Readingtext.setVisibility(View.VISIBLE);
                    break;
                default:
                    break;
            }
        }
    };
}
