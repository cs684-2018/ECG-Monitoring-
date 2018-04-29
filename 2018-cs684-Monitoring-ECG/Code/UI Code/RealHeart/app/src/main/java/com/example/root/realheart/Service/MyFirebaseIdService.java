package com.example.root.realheart.Service;

import android.util.Log;

import com.example.root.realheart.Common.Common;
import com.example.root.realheart.Model.Token;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;

/**
 * Created by root on 14/4/18.
 */

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: MyFirebaseIdService.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: updateTokenToFirebase and android standard
* Global Variables: None
*
*/

public class MyFirebaseIdService extends FirebaseInstanceIdService{

    @Override
    public void onTokenRefresh() {
        super.onTokenRefresh();
        String userName= FirebaseInstanceId.getInstance().getToken();
        if(Common.currentUser!=null)
            updateTokenToFirebase(userName);
    }

    /*
    * Function Name: showNotification
    * Input: User
    * Output: update to Abnormality
    * Logic: Once notification is popped for specified username, it status its status is change and updated to firebase
    * Example Call: updateTokenToFirebase(userName);
    */
    private void updateTokenToFirebase(String userName) {

        Log.w("Inside MFS", "Line 26");
        FirebaseDatabase db=FirebaseDatabase.getInstance();
        DatabaseReference tokens=db.getReference("Abnormality");
        Token token=new Token(userName);//false bcauz this token is sent from client
        tokens.child(Common.currentUser.getPhone()).setValue(token);
    }
}
