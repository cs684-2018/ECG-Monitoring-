package com.example.root.realheart.Remote;

import com.example.root.realheart.Model.MyResponse;
import com.example.root.realheart.Model.Sender;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.Headers;
import retrofit2.http.POST;


/**
 * Created by root on 14/4/18.
 */

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: APIService.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: sendNotification
* Global Variables: None
*
*/
public interface APIService {

    @Headers(

            {
                    "Content-Type:application/json",
                    "Authorization:key=AAAAaSvo1wY:APA91bE-NeXUu9ojnCzIdWU44o56kh2p5sKME918-SrPcfGnPd0SKdi5k5slrNpfMNZ9d4XuXc1_n7Owpzl_CD4adT04zLiJVrKj_F1hTB9MwDpCNLJVDss0Ng2u6lfReOPzXGfJgTdV"

            }
    )

    @POST("fcm/send")

    Call<MyResponse> sendNotification(@Body Sender body);

}
