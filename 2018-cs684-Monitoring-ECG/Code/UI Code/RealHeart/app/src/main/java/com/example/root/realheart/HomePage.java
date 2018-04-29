package com.example.root.realheart;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.root.realheart.Common.Common;
import com.example.root.realheart.Model.Abnormality;
import com.example.root.realheart.Remote.APIService;
import com.example.root.realheart.Service.ChkAbnormalityService;
import com.example.root.realheart.Service.ListenToNotificationService;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.Viewport;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: HomePage.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: checkAbnormality(int epochSum ), getECGValues(), setGraphParameters()  and android standard
* Global Variables: database references, placeholder for abnormality, mService, UI related
*
*/
public class HomePage extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener,MedicineFragment.OnFragmentInteractionListener, EmergencyContactFrag.OnFragmentInteractionListener,SOSFrag.OnFragmentInteractionListener {

    //UI related
    TextView txtUserName, txtName;
    GraphView graph;
    LineGraphSeries<DataPoint> series;

    //database references
    FirebaseDatabase database;
    DatabaseReference ecgReadings;
    DatabaseReference abnormalityReference;
    DatabaseReference userReference;

    //Abnormality detection placeholders
    public static int noItrn = 3, stdMean = 516, upperBound =stdMean + 250, lowerBound = stdMean - 250, preEpoch = 0;
    public int tempItern = noItrn;
    public static int xValue;

    APIService mService;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);
        //user=(LinearLayout)findViewById(R.id.header_user);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setTitle(R.string.app_name);
        setSupportActionBar(toolbar);

        mService =Common.getFCMService();

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        View headerView=navigationView.getHeaderView(0);
        txtName=(TextView)headerView.findViewById(R.id.txtName);
        txtName.setText(Common.currentUser.getName());
        txtName.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(HomePage.this,"Hello",1).show();
                Intent intent=new Intent(getApplicationContext(),UserInfoAct.class);
                startActivity(intent);
            }
        });
        txtUserName=(TextView)headerView.findViewById(R.id.txtUserName);
        txtUserName.setText(Common.currentUser.getUserName());
        txtUserName.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(HomePage.this,"Hello",1).show();
                Intent intent=new Intent(getApplicationContext(),UserInfoAct.class);
                startActivity(intent);
            }
        });
        /*user.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(HomePage.this,"Hello",1).show();
                Intent intent=new Intent(getApplicationContext(),UserInfoAct.class);
                startActivity(intent);
            }
        });*/
        database = FirebaseDatabase.getInstance();
        ecgReadings = database.getReference("ECGReading");
        abnormalityReference = database.getReference("Abnormality");
        userReference= database.getReference("User");

        //Call Service
        Intent notificationService=new Intent(HomePage.this, ListenToNotificationService.class);
        startService(notificationService);

        //Call Service
        /*Intent abnormalityService=new Intent(HomePage.this, ChkAbnormalityService.class);
        startService(abnormalityService);*/

        setGraphParameters();
        getECGValues();


    }

    /*
    * Function Name: getECGValues
    * Input: None
    * Output: none
    * Logic: Change in firebase users values table causes call to this function
    * Example Call: getECGValues();
    */
    private void getECGValues() {
        Log.w("Getting use Name", Common.currentUser.getUserName());


        Query lastQuery = ecgReadings.child(Common.currentUser.getUserName().toString()).orderByKey().limitToLast(300);

        lastQuery.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                xValue = 0;
                series = new LineGraphSeries<DataPoint>(new DataPoint[] {
                        new DataPoint(0, 0)
                });
                for (DataSnapshot dataValue:dataSnapshot.getChildren()) {
                    int epochSum = 0;
                    String ecgReadings[] = dataValue.child("value").getValue().toString().split(",");
                    for (String value: ecgReadings
                         ) {
                        try {
                            //Log.w("Home page line 114: ", value+"----");
                            epochSum = epochSum + Integer.parseInt(value);
                            series.appendData(
                                    new DataPoint(xValue++, Integer.parseInt(value)),
                                    true,
                                    500000
                            );
                        }catch (NumberFormatException e){
                            e.printStackTrace();
                        }
                    }
                    checkAbnormality(epochSum);
                }
                graph.removeAllSeries();
                graph.addSeries(series);
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });

    }


    /*
    * Function Name: checkAbnormality
    * Input: User
    * Output: none
    * Logic: Checking abnormality logic implemented here, sum of all values of one epoch is used.
    * Example Call: checkAbnormality(int epochSum );
    */
    private void checkAbnormality(int epochSum ) {

        int noOfAbn = Integer.parseInt(Common.currentUser.getNoOfAbn());

        Log.w("Checking abnormality","Yes checking it" + noOfAbn);

            if (epochSum > upperBound && preEpoch == 1 ){
                tempItern--;
                preEpoch = 1;
            }else if(epochSum > upperBound && preEpoch != 1){
                tempItern = noItrn-1;
                preEpoch = 1;
            }else if (epochSum < lowerBound && preEpoch == -1){
                tempItern--;
                preEpoch = -1;
            }else if(epochSum < lowerBound && preEpoch != -1){
                tempItern = noItrn-1;
                preEpoch = -1;
            }else{
                tempItern = noItrn;
                preEpoch = 0;
            }

        Log.w("Abnormality test", preEpoch+"-------"+tempItern+"----"+noOfAbn);

        if (preEpoch == -1 && tempItern<= 1){
            Toast.makeText(this, "Abnormal Low Condition Detected!!!( Low )", Toast.LENGTH_SHORT).show();
            noOfAbn++;
            Common.currentUser.setNoOfAbn(noOfAbn+"");
            Abnormality abnormality = new Abnormality(Common.currentUser.getUserName(),"Low Heart Rate", "Heart rate decreased to lowest level", "false", noOfAbn+"");
            abnormalityReference.child((noOfAbn)+"").setValue(abnormality);
            userReference.child(Common.currentUser.getUserName()).setValue(Common.currentUser);

        }else if(preEpoch == 1 && tempItern <= 1){
            Toast.makeText(this, "Abnormal Hart Condition Detected!!!( Heigh )", Toast.LENGTH_SHORT).show();
            noOfAbn++;
            Common.currentUser.setNoOfAbn(noOfAbn+"");
            Abnormality abnormality = new Abnormality(Common.currentUser.getUserName(),"High Heart Rate", "Heart rate increased to highest level", "false", noOfAbn+"");
            abnormalityReference.child((noOfAbn)+"").setValue(abnormality);
            userReference.child(Common.currentUser.getUserName()).setValue(Common.currentUser);
        }
        preEpoch = 0;
        tempItern = noItrn;

    }


    /*
    * Function Name: setGraphParameters
    * Input: None
    * Output: none
    * Logic: sets variois parameters required to show real-time ecg graph
    * Example Call: setGraphParameters();
    */
    private void setGraphParameters() {
        graph = (GraphView) findViewById(R.id.graph);
        series = new LineGraphSeries<DataPoint>(new DataPoint[] {
                new DataPoint(0, 0)
        });

        series.setAnimated(true);
        graph.setTitle("ECG Readings");
        graph.setFocusable(true);
        graph.setSoundEffectsEnabled(true);
        graph.getGridLabelRenderer().setHumanRounding(true);
        //graph.getGridLabelRenderer().setHorizontalAxisTitle("Time");
        //graph.getGridLabelRenderer().setGridStyle( GridLabelRenderer.GridStyle.VERTICAL );
        //graph.getViewport().setDrawBorder(true);
        graph.getGridLabelRenderer().setHorizontalLabelsVisible(false);
        graph.getGridLabelRenderer().setVerticalLabelsVisible(false);
        //graph.getGridLabelRenderer().setNumVerticalLabels(51);
        //graph.getGridLabelRenderer().setNumHorizontalLabels(10);
        //graph.setMinimumWidth(1);
        //graph.setScroll
        Viewport viewport = graph.getViewport();
        viewport.setXAxisBoundsManual(true);
        viewport.setMinX(0);
        viewport.setMaxX(300);
        viewport.setYAxisBoundsManual(true);
        viewport.setMaxY(1200);
        viewport.setMinY(-600);

        viewport.setScalableY(true);
        viewport.setScalable(true);
        viewport.setScrollableY(true);
        viewport.setScrollable(true);
    }


            @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.home_page, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        /*if (id == R.id.action_settings) {
            return true;
        }*/

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.

        android.support.v4.app.Fragment fr=null;
        int id = item.getItemId();
        //Toast.makeText(this, "Loging Id"+id, Toast.LENGTH_SHORT).show();
        if (id == R.id.nav_sensor) {

        } else if (id == R.id.nav_appointment) {
            //Snackbar.make(item.getActionView(), "Yet to implement", Snackbar.LENGTH_SHORT);
            fr=new MedicineFragment();
            //Toast.makeText(this, "Yet to implement", Toast.LENGTH_SHORT).show();
        } else if (id == R.id.nav_reports) {
            fr=new EmergencyContactFrag();
            //Snackbar.make(item.getActionView(), "Yet to implement", Snackbar.LENGTH_SHORT);
            //Toast.makeText(this, "Yet to implement", Toast.LENGTH_SHORT).show();
        } else if (id == R.id.nav_bill) {
            fr=new SOSFrag();
            //Snackbar.make(item.getActionView(), "Yet to implement", Snackbar.LENGTH_SHORT);
            //Toast.makeText(this, "Yet to implement", Toast.LENGTH_SHORT).show();
        }else{


            Log.w("Why Log hpening !!", "Dont know");
            Intent signIn = new Intent(HomePage.this, SignIn.class);
            signIn.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            startActivity(signIn);
        }
        if(fr!=null)
        {
            FragmentManager fragmentManager = getSupportFragmentManager();
            fragmentManager.beginTransaction()
                    .replace(R.id.frame_container, fr).commit();

        }
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }

    @Override
    public void onFragmentInteraction(Uri uri) {

    }
}
