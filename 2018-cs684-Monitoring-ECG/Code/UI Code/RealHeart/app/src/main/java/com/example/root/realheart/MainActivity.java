package com.example.root.realheart;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: MainActivity.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: checkAndroidVersion and android standard
* Global Variables: btnSignIn, txtSlogan
*
*/

public class MainActivity extends AppCompatActivity {

    Button btnSignIn;
    TextView txtSlogan;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        checkAndroidVersion();
        txtSlogan = (TextView) findViewById(R.id.txtSlogan);
        Typeface face=Typeface.createFromAsset(getAssets(),"fonts/Nabila.ttf");
        txtSlogan.setTypeface(face);

        btnSignIn =(Button) findViewById(R.id.btnSignIn);
        btnSignIn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent singIn=new Intent(MainActivity.this,SignIn.class);
                startActivity(singIn);
            }
        });

    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    /*
    * Function Name: checkAndroidVersion
    * Input: None
    * Output: none
    * Logic: Checks for android version (23 and above)
    * Example Call: checkAndroidVersion();
    */
    public void checkAndroidVersion(){
        //this.mobile= mobile;
        if (Build.VERSION.SDK_INT >= 23) {
            int checkCallPhonePermission = ContextCompat.checkSelfPermission(MainActivity.this, android.Manifest.permission.SEND_SMS);
            if(checkCallPhonePermission != PackageManager.PERMISSION_GRANTED){
                ActivityCompat.requestPermissions(MainActivity.this,new String[]{android.Manifest.permission.SEND_SMS},200);
                return;
            }else{
                //sendSms(mobile);
            }
        } else {
            //sendSms(mobile);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        switch (requestCode) {
            case 200: {
                if(grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // {Some Code}

                }
                else
                {
                    finish();
                }

            }
        }
    }
}
