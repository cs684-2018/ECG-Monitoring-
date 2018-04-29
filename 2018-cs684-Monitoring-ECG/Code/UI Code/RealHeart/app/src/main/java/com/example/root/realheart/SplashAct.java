package com.example.root.realheart;

import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
/*
TeamId: Nill
Author List: Amit Vhatkar, Suraj Singh, Akshat Garg
FileName: SplashAct.java
Functions: android standard functions
Global Variables:UI related variables

This activity is a Splash Screen for our app
 */
public class SplashAct extends AppCompatActivity implements Runnable,Animation.AnimationListener {

    ImageView iv;  // global variable for Image View
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash); // setting UI layout for this activity
        //getSupportActionBar().hide();
        iv=(ImageView)findViewById(R.id.splash_image); //getting UI variables object in our global variable
        Handler handler = new Handler();  //using handler to show animation
        handler.postDelayed(this, 500);

    }

    @Override
    public void run() {
        // TODO Auto-generated method stub

        Animation am= AnimationUtils.loadAnimation(this, R.anim.splah_anim);  //loading animation file for our splash screen animation
        am.setAnimationListener(this);
        iv.startAnimation(am); //starting animation for our ImageView
        iv.setVisibility(View.VISIBLE);  // setting visibility of our image to true


    }

    @Override
    public void onAnimationStart(Animation animation) {
        // TODO Auto-generated method stub

    }

    @Override
    public void onAnimationEnd(Animation animation) {  // this function will be called when animation ends
        // TODO Auto-generated method stub
        finish();  // as the animation ends we will end the activity
        Intent intent = new Intent(this, MainActivity.class);  // and we will call new Activity called MainActivity  as this animation ends
        startActivity(intent);
    }

    @Override
    public void onAnimationRepeat(Animation animation) {
        // TODO Auto-generated method stub
    }
}
