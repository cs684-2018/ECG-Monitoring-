package com.example.root.realheart;

import android.content.Context;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.Toast;

/*
TeamId: Nill
Author List: Amit Vhatkar, Suraj Singh, Akshat Garg
FileName: UserInfoAct.java
Functions: android standard functions
Global Variables:UI related variables

This activity is used to store and show saved user information to the user when he or she visits
 */

public class UserInfoAct extends AppCompatActivity {
    AutoCompleteTextView fname,lname,mno,email,age,bg;  // variable names for textview UI fields used in our app
    Button submit;  // variable names for button UI used in our app
    RadioGroup rg;  // variable names for RadioGroup UI used in our app
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_info);   // setting UI layout for this activity
        fname=(AutoCompleteTextView)findViewById(R.id.firstName);  //getting UI variable from layout and storing it into out defined global variables
        lname=(AutoCompleteTextView)findViewById(R.id.lastName);
        mno=(AutoCompleteTextView)findViewById(R.id.number);
        email=(AutoCompleteTextView)findViewById(R.id.email);
        age=(AutoCompleteTextView)findViewById(R.id.age);
        rg=(RadioGroup)findViewById(R.id.gender);
        bg=(AutoCompleteTextView)findViewById(R.id.bg);
        submit=(Button)findViewById(R.id.submit);     //recieved all the UI variables

        // setting onCLickListener for submit button used in our app
        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Shared Prefrences code goes here

                if(!fname.getText().toString().equals("") && !lname.getText().toString().equals("") && !mno.getText().toString().equals("") && !email.getText().toString().equals("") && !age.getText().toString().equals("") && !bg.getText().toString().equals("") && rg.getCheckedRadioButtonId()!=-1)
                {
                    SharedPreferences sp=getSharedPreferences("ass", Context.MODE_PRIVATE);  //getting shared preference object to retrieve previous stored values
                    SharedPreferences.Editor edit=sp.edit();
                    edit.putString("fname",fname.getText().toString());    //saving the values entered by user in shared prefrences variables
                    edit.putString("lname",lname.getText().toString());
                    edit.putString("mno",mno.getText().toString());
                    edit.putString("email",email.getText().toString());
                    edit.putString("age",age.getText().toString());
                    edit.putString("bg",bg.getText().toString());
                    edit.putInt("rg_id",rg.getCheckedRadioButtonId());
                    edit.commit();  // committing the change in the values
                    Toast.makeText(UserInfoAct.this, "Information saved succesfully", Toast.LENGTH_SHORT).show();  // this is a toast which comes on screen for 1-2 sec at the bottom
                }
                else
                {
                    Toast.makeText(UserInfoAct.this,"Fields can not be left empty",Toast.LENGTH_LONG).show();    // this is a toast which comes on screen for 1-2 sec at the bottom
                }
            }
        });
        //setting onKeyListener for age textview to put constraint in age
        age.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View view, int i, KeyEvent keyEvent) {
                if(!age.getText().toString().equals("") && Integer.parseInt(age.getText().toString())>150)
                {
                    age.setText(age.getText().toString().substring(0,2));  // limiting the value to two digit
                    SharedPreferences sp=getSharedPreferences("ass",Context.MODE_PRIVATE); //getting shared preference object to save age value
                    SharedPreferences.Editor edit=sp.edit();
                    edit.putString("age",age.getText().toString());
                }
                return false;
            }
        });

        SharedPreferences sp=getSharedPreferences("ass",Context.MODE_PRIVATE);  //getting shared  preference to get the saved values  and showing in the UI TextView elements
        fname.setText(sp.getString("fname",null));  //setting textValue for first name
        lname.setText(sp.getString("lname",null));  // setting textValue for last name
        mno.setText(sp.getString("mno",null));      // setting textValue for mobile number
        email.setText(sp.getString("email",null));  // setting textValue for email field
        age.setText(sp.getString("age",null));     // setting textValue for age
        bg.setText(sp.getString("bg",null));       // setting textValue for blood group
        rg.check(sp.getInt("rg_id",-1));            // setting value for radio group

    }
}
