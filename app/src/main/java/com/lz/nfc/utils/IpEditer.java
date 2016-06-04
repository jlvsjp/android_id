package com.lz.nfc.utils;

import android.content.Context;
import android.graphics.Color;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.widget.EditText;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;


public class IpEditer extends EditText {

	int mMaxLenth = 15;

	int color = Color.BLACK;

	public IpEditer(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.setInputType(InputType.TYPE_CLASS_PHONE);
		this.setFilters(new InputFilter[] { new InputFilter.LengthFilter(mMaxLenth) });
		this.addTextChangedListener(mTextWatcher);
	}

	public String stringFilter(String str) throws PatternSyntaxException {
		String regEx = "[/\\:#*?<>|\"\n\ta-zA-Z]";
		String tmp = null;
		Pattern p = Pattern.compile(regEx);
		Matcher m = p.matcher(str);
		if (m.find()) {
			tmp = m.replaceAll(".");
			for (int i = 0; i < tmp.length(); i++) {
				if (tmp.charAt(i) != str.charAt(i)) {
					changedIndex = i;
					break;
				}
			}
		} else {
			return str;
		}
		return tmp;

	}

	int ipType = IPTYPE_IPV4;
	public static final int IPTYPE_IPV4 = 0;
	public static final int IPTYPE_IPV6 = 1;

	public int getIpType() {
		return ipType;
	}

	public void setIpType(int ipType) {
		this.ipType = ipType;
	}

	TextWatcher mTextWatcher = new TextWatcher() {

		public void onTextChanged(CharSequence s, int start, int before,
				int count) {
		}

		public void beforeTextChanged(CharSequence s, int start, int count,
				int after) {
		}

		public void afterTextChanged(Editable s) {
			String editable = IpEditer.this.getText().toString();
			String str = stringFilter(editable);
			if (!editable.equals(str)) {
				IpEditer.this.setText(str);
				IpEditer.this.setSelection(++changedIndex);
			}
		}
	};

	int changedIndex;

	public int getChangedIndex() {

		return changedIndex;
	}

	/**
	 * Get ip
	 * 
	 * @return ip
	 */
	public String getIpText() {
		return this.getText().toString();
	}

	private String regexIpv4 = "^((25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)\\.){3}(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)$";
	private String regexIpv6 = "^/^\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?\\s*$/$";

	/**
	 * detecting ip format
	 * 
	 * @param text
	 *            the ip would be matched
	 * @return true if the text is ip, else return false
	 */
	public boolean isIpv4Text() {
		return getIpText().matches(regexIpv4);
	}

	public void checkFormat() {

		if (getIpText().matches(regexIpv4)) {
			this.setTextColor(color);
		} else {
			this.setTextColor(Color.RED);
		}
	}

	public boolean isIpv6Text() {
		return getIpText().matches(regexIpv6);
	}

	/**
	 * Set ip into IpEditer
	 * 
	 * @param ipText
	 *            ip
	 */
	public void setIpText(String ipText) {
		if (ipText == null || ipText.length() <= 0) {
			return;
		}
		this.setText(ipText);
	}

}
