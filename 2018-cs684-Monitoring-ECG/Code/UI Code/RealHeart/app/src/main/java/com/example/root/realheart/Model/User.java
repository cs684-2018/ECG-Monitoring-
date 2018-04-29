package com.example.root.realheart.Model;

/**
 * Created by root on 28/3/18.
 */
/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: User.java
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: getter, setters, constructors
* Global Variables: userName, Name, Password, Phone, noOfAbn
*
*/
public class User {
    private String userName;
    private String Name;
    private String Password;
    private String Phone;
    private String noOfAbn;

    public User() {
    }

    public User(String userName, String name, String password, String phone, String noOfAbn) {
        this.userName = userName;
        Name = name;
        Password = password;
        Phone = phone;
        this.noOfAbn = noOfAbn;
    }

    @Override
    public String toString() {
        return "User{" +
                "userName='" + userName + '\'' +
                ", Name='" + Name + '\'' +
                ", Password='" + Password + '\'' +
                ", Phone='" + Phone + '\'' +
                ", noOfAbn='" + noOfAbn + '\'' +
                '}';
    }

    public String getNoOfAbn() {
        return noOfAbn;
    }

    public void setNoOfAbn(String noOfAbn) {
        this.noOfAbn = noOfAbn;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getName() {
        return Name;
    }

    public void setName(String name) {
        Name = name;
    }

    public String getPassword() {
        return Password;
    }

    public void setPassword(String password) {
        Password = password;
    }

    public String getPhone() {
        return Phone;
    }

    public void setPhone(String phone) {
        Phone = phone;
    }
}
