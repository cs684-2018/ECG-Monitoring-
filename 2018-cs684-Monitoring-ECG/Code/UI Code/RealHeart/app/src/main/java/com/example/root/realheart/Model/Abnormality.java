package com.example.root.realheart.Model;

/**
 * Created by root on 14/4/18.
 */

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: Abnormality.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getters and setters, constructor
* Global Variables: userName, abnormalityType, abnormalityDescription, isShown, abnNo
*
*/

public class Abnormality {
    public String userName;
    public String abnormalityType;
    public String abnormalityDescription;
    public String isShown;
    public String abnNo;

    public Abnormality() {
    }

    public Abnormality(String userName, String abnormalityType, String abnormalityDescription, String isShown, String abnNo) {
        this.userName = userName;
        this.abnormalityType = abnormalityType;
        this.abnormalityDescription = abnormalityDescription;
        this.isShown = isShown;
        this.abnNo = abnNo;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getAbnormalityType() {
        return abnormalityType;
    }

    public void setAbnormalityType(String abnormalityType) {
        this.abnormalityType = abnormalityType;
    }

    public String getAbnormalityDescription() {
        return abnormalityDescription;
    }

    public void setAbnormalityDescription(String abnormalityDescription) {
        this.abnormalityDescription = abnormalityDescription;
    }


    public String getAbnNo() {
        return abnNo;
    }

    public void setAbnNo(String abnNo) {
        this.abnNo = abnNo;
    }

    public String getIsShown() {
        return isShown;
    }



    public void setIsShown(String isShown) {
        this.isShown = isShown;
    }
}
