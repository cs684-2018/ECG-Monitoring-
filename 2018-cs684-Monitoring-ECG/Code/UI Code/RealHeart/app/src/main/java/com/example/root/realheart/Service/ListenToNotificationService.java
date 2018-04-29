package com.example.root.realheart.Service;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.example.root.realheart.HomePage;
import com.example.root.realheart.Model.Abnormality;
import com.example.root.realheart.Model.User;
import com.example.root.realheart.R;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.Random;

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: ListenToNotificationService.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: constructor, showNotification and android standard
* Global Variables: db, abnormalityReference, userReference
*
*/

public class ListenToNotificationService extends Service implements ChildEventListener {

    FirebaseDatabase db;
    DatabaseReference abnormalityReference;
    DatabaseReference userReference;

    public ListenToNotificationService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        db = FirebaseDatabase.getInstance();
        abnormalityReference = db.getReference("Abnormality");
        userReference  = db.getReference("User");
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        return null;
    }

    @Override
    public void onChildAdded(DataSnapshot dataSnapshot, String s) {

    }


    /*
    * Function Name: showNotification
    * Input: User
    * Output: Notification popup
    * Logic: Change in firebase abnormality table causes call to this function
    * Example Call: showNotification(user);
    */
    private void showNotification(User user) {

        Log.w("In LTN", "Showing notification");

        abnormalityReference.child(user.getNoOfAbn()).addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Abnormality abnormality = dataSnapshot.getValue(Abnormality.class);

                if(abnormality.getIsShown().equals("false")) {
                    //Intent intent = new Intent(getBaseContext(), HomePage.class);
                    //PendingIntent contentIntent;
                    //contentIntent = PendingIntent.getActivity(getBaseContext(), 0, intent, 0);

                    NotificationCompat.Builder builder = new NotificationCompat.Builder(getBaseContext());
                    builder.setAutoCancel(true)
                            .setDefaults(android.app.Notification.DEFAULT_ALL)
                            .setTicker("Real Heart News")
                            .setContentInfo(abnormality.getAbnormalityType())
                            .setContentText(abnormality.getAbnormalityDescription())
                            .setSmallIcon(R.mipmap.ic_launcher);

                    NotificationManager manager = (NotificationManager) getBaseContext().getSystemService(Context.NOTIFICATION_SERVICE);
                    int random = new Random().nextInt(9999 - 1) + 1;
                    manager.notify(random, builder.build());

                    abnormality.setIsShown("true");
                    abnormalityReference.child(abnormality.getAbnNo()).setValue(abnormality);
                }

            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });

    }


    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        userReference.addChildEventListener(this);
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onChildChanged(DataSnapshot dataSnapshot, String s) {
        showNotification(dataSnapshot.getValue(User.class));
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