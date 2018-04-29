/*
* Team Id: Nil
* Author List: Amit Vhatkar, Suraj Shingh, Aksaht Garag
* Filename: ecg_reading.ino
* Theme: IoT Based EGC Monitoring System For Automatic Detection Of Abnormalities
* Functions: setup() , loop()
* Global Variables: n, counter, epoch, my_value, readingIn, values
*
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// Set these to run example.
#define FIREBASE_HOST "realheart-26433.firebaseio.com"
#define FIREBASE_AUTH "ntxFNz0ODwMqMBTwe5JDIYdsIP3XhWWkfHSMoJxV"
//Change line with your WiFi router name and password
#define WIFI_SSID "amitvhatkar"  
#define WIFI_PASSWORD "Im@iitb17"

int n = 0, counter=0;
int epoch = 100;
int my_value[100];
int readingIn;
String values;

/*
* Function Name: setup
* Input: None
* Output: Does initial  pin configuration and connection to wifi
* Logic: None
* Example Call: No call required
*/
void setup() {
	Serial.begin(9600);
	// connect to wifi.
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	//Serial.print("connecting");
	while (WiFi.status() != WL_CONNECTED) {
		//Serial.print(".");
		delay(500);
	}
	pinMode(16,INPUT_PULLUP);
	pinMode(5,INPUT_PULLUP);
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

/*
* Function Name: loop
* Input: None
* Output: prints the ! if electrodes are not connected to body, else prints 
* values sent to firebase  
* Logic: The code loops forever
* Example Call: No call required
*/
void loop() {

	if((digitalRead(16) == 1)||(digitalRead(5) == 1)){
		Serial.println('!');
	}
	else{
		readingIn= analogRead(A0);
		my_value[counter]=readingIn;
		values = values+","+readingIn;
		if(counter>=epoch)
		{
			values.remove(0,1);
			String json = "{\"value\":\""+values+"\"}";//"{\"value\":\""+String(my_value[i])+"\"}";
			StaticJsonBuffer<1000> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			String  path = "ECGReading/asvhatkar/"+String(n++);
			Firebase.set(path, root);
			if (Firebase.failed()) {
				Serial.print("pushing /logs failed:");
				Serial.println(Firebase.error());  
				return;
			}
			delay(12);
			counter=0;
			values.remove(0);
		}else{
			counter+=1;
		}
		Serial.println(readingIn);
	}
	delay(1);
	if(n == 5000){
		Serial.println(n);
		n = 0;
	}
}
