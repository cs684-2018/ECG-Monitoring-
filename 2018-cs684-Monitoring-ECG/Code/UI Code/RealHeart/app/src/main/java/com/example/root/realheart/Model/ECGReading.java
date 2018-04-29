package com.example.root.realheart.Model;

/**
 * Created by root on 28/3/18.
 */
/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: ECGReading.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getters and setters, constructor
* Global Variables: userName, id, value
*
*/

public class ECGReading {
    private String userName;
    private String id;
    private String []value;

    public ECGReading() {
    }

    public ECGReading(String userName, String id, String[] value) {
        this.userName = userName;
        this.id = id;
        this.value = value;
    }


    @Override
    public String toString() {
        return "ECGReading{" +
                "userName='" + userName + '\'' +
                ", id='" + id + '\'' +
                ", value='" + value + '\'' +
                '}';
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String[] getValue() {
        return value;
    }

    public void setValue(String[] value) {
        this.value = value;
    }
}
