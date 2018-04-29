package com.example.root.realheart;

import android.*;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.example.root.realheart.Common.Common;
import com.example.root.realheart.Model.User;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.rengwuxian.materialedittext.MaterialEditText;

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: SignIn.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: android standard
* Global Variables: UI related
*
*/
public class SignIn extends AppCompatActivity {

    //UI related
    EditText edtUserName,edtPassword;
    Button btnSignIn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_in);
        //checkAndroidVersion();
        edtPassword = (MaterialEditText) findViewById(R.id.edtPassword);
        edtUserName = (MaterialEditText) findViewById(R.id.edtUserName);
        btnSignIn = (Button) findViewById(R.id.btnSignIn);

        //Init Firebase
        final FirebaseDatabase database = FirebaseDatabase.getInstance();
        final DatabaseReference table_user = database.getReference("User");

        btnSignIn.setOnClickListener(new View.OnClickListener() {



            @Override
            public void onClick(View v) {
                final ProgressDialog mDialog = new ProgressDialog(SignIn.this);
                mDialog.setMessage("Please Wait ...");
                mDialog.show();
                table_user.addValueEventListener(new ValueEventListener() {


                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        //Check if user exists
                        //Log.w("What is shappening", dataSnapshot.child(edtUserName.getText().toString()).exists()+"");
                        if(dataSnapshot.child(edtUserName.getText().toString()).exists()) {


                            //Get User Information
                            mDialog.dismiss();
                            User user = dataSnapshot.child(edtUserName.getText().toString()).getValue(User.class);
                            user.setPhone(edtUserName.getText().toString());
                            //Log.w("user in sigin up..", user.toString());
                            if(user.getPassword().equals(edtPassword.getText().toString())) {
                                Intent homeIntent =new Intent(SignIn.this,HomePage.class);

                                //Log.w("user in sigin up", user.toString());

                                user.setUserName(edtUserName.getText().toString());
                                Common.currentUser =user;
                                startActivity(homeIntent);
                                finish();
                            }
                            else {
                                Toast.makeText(SignIn.this,"Sign in failed !",Toast.LENGTH_SHORT).show();
                            }}
                        else {
                            mDialog.dismiss();
                            Toast.makeText(SignIn.this,"User not exist in Database !",Toast.LENGTH_SHORT).show();
                        }
                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {

                    }
                });
            }
        });
    }


}
