package com.example.root.realheart.Common;

import com.example.root.realheart.Model.User;
import com.example.root.realheart.Remote.APIService;
import com.example.root.realheart.Remote.RetrofitClient;

/**
 * Created by root on 28/3/18.
 */

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: common.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getFCMService()
* Global Variables: currentUser, BASE_URL
*
*/
public class Common {
    public  static User currentUser;

    private static final String BASE_URL="https://fcm.googleapis.com/";

    public static APIService getFCMService()
    {
        return RetrofitClient.getClient(BASE_URL).create(APIService.class);
    }
}
