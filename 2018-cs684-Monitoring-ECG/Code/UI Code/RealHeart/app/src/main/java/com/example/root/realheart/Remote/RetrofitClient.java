package com.example.root.realheart.Remote;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * Created by root on 14/4/18.
 */

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: RetrofitClient.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getter
* Global Variables: retrofit
*
*/
public class RetrofitClient {

    private static Retrofit retrofit=null;

    public static Retrofit getClient(String baseURL)
    {
        if(retrofit==null)
        {
            retrofit =new Retrofit.Builder()
                    .baseUrl(baseURL)
                    .addConverterFactory(GsonConverterFactory.create())
                    .build();
        }
        return retrofit;
    }
}