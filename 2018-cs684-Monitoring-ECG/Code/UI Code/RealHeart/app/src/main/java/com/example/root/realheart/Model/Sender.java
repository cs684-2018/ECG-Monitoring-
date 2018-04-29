package com.example.root.realheart.Model;



/**
 * Created by root on 14/4/18.
 */


/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: Sender.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getters, setters, constructors
* Global Variables: to, notification
*
*/
public class Sender {
    public String to;
    public Notification notification;


    public Sender(String to, Notification notification) {
        this.to = to;
        this.notification = notification;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public Notification getNotification() {
        return notification;
    }

    public void setNotification(Notification notification) {
        this.notification = notification;
    }
}
