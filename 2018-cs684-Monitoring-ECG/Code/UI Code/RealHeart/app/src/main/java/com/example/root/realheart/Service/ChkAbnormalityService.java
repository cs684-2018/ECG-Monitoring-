package com.example.root.realheart.Service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import com.example.root.realheart.Common.Common;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;


/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: ChkAbnormalityService.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: constructor, android standard,
* Global Variables: db, abnormalityReference, user, ecgReading, graph, series
*
*/
public class ChkAbnormalityService extends Service implements ChildEventListener{

    FirebaseDatabase db;
    DatabaseReference abnormalityReference;
    DatabaseReference user;
    DatabaseReference ecgReading;

    GraphView graph;
    LineGraphSeries<DataPoint> series;



    public ChkAbnormalityService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        db = FirebaseDatabase.getInstance();
        abnormalityReference = db.getReference("Abnormality");
        user = db.getReference("User");
        ecgReading = db.getReference("ECGReading");

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        ecgReading.addChildEventListener(this);
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public void onChildAdded(DataSnapshot dataSnapshot, String s) {
        Log.w("---Changing values--",s+"---"+dataSnapshot.getKey());

        if(dataSnapshot.getKey().equals(Common.currentUser.getUserName())){
            Log.w("For whome",Common.currentUser.getUserName());
            //checkAbnormality(dataSnapshot);
        }
    }

    @Override
    public void onChildChanged(DataSnapshot dataSnapshot, String s) {

    }



    @Override
    public void onChildRemoved(DataSnapshot dataSnapshot) {

    }

    @Override
    public void onChildMoved(DataSnapshot dataSnapshot, String s) {

    }

    @Override
    public void onCancelled(DatabaseError databaseError) {

    }
}
