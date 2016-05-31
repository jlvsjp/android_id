
package com.lz.domain;

public class IdentityCard {
    private String name;
    private String sex;
    private String ethnicity;
    private String birth;
    private String cardNo;
    private String authority;
    private String address;
    private String DN;
    private String period;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public String getEthnicity() {
        return ethnicity;
    }

    public void setEthnicity(String ethnicity) {
        this.ethnicity = ethnicity;
    }

    public String getBirth() {
        return birth;
    }

    public void setBirth(String birth) {
        this.birth = birth;
    }

    public String getCardNo() {
        return cardNo;
    }

    public void setCardNo(String cardNo) {
        this.cardNo = cardNo;
    }

    public String getAuthority() {
        return authority;
    }

    public void setAuthority(String authority) {
        this.authority = authority;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getDN() {
        return DN;
    }

    public void setDN(String DN) {
        this.DN = DN;
    }

    public String getPeriod() {
        return period;
    }

    public void setPeriod(String period) {
        this.period = period;
    }

    public IdentityCard() {
    }

    public IdentityCard(String name, String sex, String ethnicity, String birth, String cardNo, String authority, String address, String DN, String period) {
        this.name = name;
        this.sex = sex;
        this.ethnicity = ethnicity;
        this.birth = birth;
        this.cardNo = cardNo;
        this.authority = authority;
        this.address = address;
        this.DN = DN;
        this.period = period;
    }
}
