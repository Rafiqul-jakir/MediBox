
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST "medibox-45997-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "jvl3BVlcV7frNx9wmv2yqOodTUlTnoVhaJj1mL4t"
#define WIFI_SSID "Belal" //provide ssid (wifi name)
#define WIFI_PASSWORD "123456780" //wifi password


Servo servo1, servo2, servo3, servo4, servo5, servo6;
int buzzerPin = 16; // D0
int switchPin = 5; // D1
int drawers[] = {4, 0, 2, 14, 12, 13}; // Drawer pins(D2, D3,D4,D5,D6,D7)
int drawer_Open_Duration_Time = 300000; // 5 minutes in milliseconds
String sokale_khawar_age, sokale_khawar_pore, dupure_khawar_age, dupure_khawar_pore, rate_khawar_age, rate_khawar_pore;


FirebaseData firebaseData;
FirebaseJson json;
const long utcOffsetInSeconds = 21600; // UTC offset for your timezone (in seconds)
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup() {
  Serial.begin(115200);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  for(int i = 0; i < 6; i++) {
    pinMode(drawers[i], OUTPUT);
  }
  
  servo1.attach(drawers[0],500,2500); // Attach servo to pins
  servo2.attach(drawers[1],500,2500);
  servo3.attach(drawers[2],500,2500);
  servo4.attach(drawers[3],500,2500);
  servo5.attach(drawers[4],500,2500);
  servo6.attach(drawers[5],500,2500);
  
  connectToWiFi();


  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


  timeClient.begin();
  timeClient.update();
}

void loop() {
  timeClient.update();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  String currentTime = convertTo12HourFormat(currentHour,currentMinute);
  if(digitalRead(switchPin) == LOW) {
    // Patient pressed the switch indicating medicine taken

  }
    sokale_khawar_age = get_target_time("/user/kawser/Time/sokale_khawar_age");
    sokale_khawar_pore = get_target_time("/user/kawser/Time/sokale_khawar_pore");
    dupure_khawar_age = get_target_time("/user/kawser/Time/dupure_khawar_age");
    dupure_khawar_pore = get_target_time("/user/kawser/Time/dupure_khawar_pore");
    rate_khawar_age = get_target_time("/user/kawser/Time/rate_khawar_age");
    rate_khawar_pore = get_target_time("/user/kawser/Time/rate_khawar_pore");
  
    if(currentTime.equals(sokale_khawar_age)) {
      open_Drawer(servo1,55,180);
    } else if(currentTime.equals(sokale_khawar_pore) ) {
      open_Drawer(servo2,0,0);
    }  else if(currentTime.equals(dupure_khawar_age) ) {
      open_Drawer(servo3,40,180);
    } else if(currentTime.equals(dupure_khawar_pore)) {
      open_Drawer(servo4,40,180);
    } else if(currentTime.equals(rate_khawar_age) ) {
      open_Drawer(servo4,40,180);
    } else if(currentTime.equals(rate_khawar_pore) ) {
      open_Drawer(servo6,45,180);
    }
  
}


//--------------------------------------------------------------
void connectToWiFi() {
  Serial.println("Connecting to WiFi");
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

}



//--------------------------------------------------------------
void open_Drawer(Servo servo, int Min_Angle, int Max_Angle){

    for (int angle = Max_Angle; angle >= Min_Angle; angle -= 1) {
      servo.write(angle);
      delay(15);               
    } 

    while(true) {
      digitalWrite(buzzerPin, HIGH); // Turn on buzzer
      delay(100);
      digitalWrite(buzzerPin, LOW); // Turn off buzzer
    if(digitalRead(switchPin) == LOW) {
      // Patient pressed the switch indicating medicine taken
      close_Drawer(servo,Min_Angle, Max_Angle);
      break; // Exit the loop if switch is pressed
      }
      delay(drawer_Open_Duration_Time);
      close_Drawer(servo,Min_Angle, Max_Angle);
    }  

}



//-----------------------------------------------------------------------
void close_Drawer(Servo servo, int Min_Angle, int Max_Angle){

  for (int angle = Min_Angle; angle <= Max_Angle; angle += 1) {
    servo.write(angle);    
    delay(15);
  }

}



//------------------------------------------------------------------------
String get_target_time(String path){
  if (Firebase.getString(firebaseData, path)) { // "/user/kawser/Time"
    if (firebaseData.dataType() == "string") {
      String timeString = firebaseData.stringData();

      // Extract hours and minutes from the time string
      int hours, minutes;
      char amPm[3];
      sscanf(timeString.c_str(), "%d:%d %2s", &hours, &minutes, amPm);

      String target_time = String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes) + " " + amPm;
      return  target_time;
    }
  } else {
    Serial.println("Error getting value");
    Serial.println(firebaseData.errorReason());
  }
}



//-----------------------------------------------------------------
String convertTo12HourFormat(int hour, int minute) {
    String period = (hour >= 12) ? "PM" : "AM";

    if (hour > 12)
        hour -= 12;
    else if (hour == 0)
        hour = 12;

    return String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + " " + period;;
}
