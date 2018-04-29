package com.example.root.realheart.Model;

import java.util.List;

/**
 * Created by root on 14/4/18.
 */
/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: MyResponse.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getters and setters, constructor
* Global Variables: multicast_id, success, failure, canonical_ids, results
*
*/

public class MyResponse {
    public long multicast_id;
    public int success;
    public int failure;
    public int canonical_ids;
    public List<Result> results;
}
