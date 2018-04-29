package com.example.root.realheart.Model;

/**
 * Created by root on 14/4/18.
 */


/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: Notification.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getters and setters, constructor
* Global Variables: body, title
*
*/

public class Notification {
    public String body;
    public String title;

    public Notification(String body, String title) {
        this.body = body;
        this.title = title;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }
}
