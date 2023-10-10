#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>


#define FIREBASE_HOST "medibox-45997-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "jvl3BVlcV7frNx9wmv2yqOodTUlTnoVhaJj1mL4t"
#define WIFI_SSID "Belal" //provide ssid (wifi name)
#define WIFI_PASSWORD "123456780" //wifi password


Servo servo1, servo2, servo3, servo4, servo5, servo6;
int buzzerPin = 16; // D0
int switchPin = 5; // D1
int drawers[] = {4, 0, 2, 14, 12, 13}; // Drawer pins(D2, D3,D4,D5,D6,D7)
int drawer_Open_Duration_Time = 300000; // 5 minutes in milliseconds
int sokale_khawar_age, sokale_khawar_pore, dupure_khawar_age, dupure_khawar_pore, rate_khawar_age, rate_khawar_pore;
int beepDuration = 100;

/*
// Firebase credentials
const char* firebaseHost = "medibox-45997-default-rtdb.firebaseio.com/";
const char* firebaseAuth = "jvl3BVlcV7frNx9wmv2yqOodTUlTnoVhaJj1mL4t";
*/


FirebaseData firebaseData;


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
  
  //connectToWiFi();

    Serial.println("Connecting to WiFi");
  WiFi.begin("Rafiqul","1311109265");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  Firebase.begin(&FIREBASE_HOST, &FIREBASE_AUTH);
}

void loop() {
  if(digitalRead(switchPin) == LOW) {
    // Patient pressed the switch indicating medicine taken

  }

  time_t currentTime  = now();
  if(year(currentTime) >= 2023 && month(currentTime) >= 9 && day(currentTime) >= 25) {
    
    int currentMinute = hour(currentTime) * 60 + minute(currentTime);
    /*
    if(currentMinute == 9*60 + 45) {
      openDrawer(servo1);
    } else if(currentMinute == 10*60 + 15) {
      openDrawer(servo2);
    } 
    */

    sokale_khawar_age = get_target_time("/user/kawser/Time/sokale_khawar_age");
    sokale_khawar_pore = get_target_time("/user/kawser/Time/sokale_khawar_pore");
    dupure_khawar_age = get_target_time("/user/kawser/Time/dupure_khawar_age");
    dupure_khawar_pore = get_target_time("/user/kawser/Time/dupure_khawar_pore");
    rate_khawar_age = get_target_time("/user/kawser/Time/rate_khawar_age");
    rate_khawar_pore = get_target_time("/user/kawser/Time/rate_khawar_pore");
  
    if(currentMinute == sokale_khawar_age) {
      open_Drawer(servo1,55,180);
    } else if(currentMinute == sokale_khawar_pore) {
      open_Drawer(servo2,0,0);
    }  else if(currentMinute == dupure_khawar_age) {
      open_Drawer(servo3,40,180);
    } else if(currentMinute == dupure_khawar_pore) {
      open_Drawer(servo4,40,180);
    } else if(currentMinute == rate_khawar_age) {
      open_Drawer(servo5,35,180);
    } else if(currentMinute == rate_khawar_pore) {
      open_Drawer(servo6,45,180);
    }
  }
}

// void connectToWiFi() {
//   Serial.println("Connecting to WiFi");
//   WiFi.begin("Rafiqul","1311109265");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting...");
//   }
//   Serial.println("Connected to WiFi");

//   // Initialize Firebase
//   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

// }

void open_Drawer(Servo servo, int Min_Angle, int Max_Angle){

    for (int angle = Max_Angle; angle >= Min_Angle; angle -= 1) {
      servo.write(angle);
      delay(15);               
    } 

    while(true) {
      digitalWrite(buzzerPin, HIGH); // Turn on buzzer
      delay(beepDuration);
      digitalWrite(buzzerPin, LOW); 
    if(digitalRead(switchPin) == LOW) {
      // Patient pressed the switch indicating medicine taken

      break; // Exit the loop if switch is pressed
      }
    }  

    close_Drawer(servo,Min_Angle, Max_Angle);
}

void close_Drawer(Servo servo, int Min_Angle, int Max_Angle){

  for (int angle = Min_Angle; angle <= Max_Angle; angle += 1) {
    servo.write(angle);    
    delay(15);
  }

}
int get_target_time(String path){
  if (Firebase.getString(firebaseData, path)) { // "/user/kawser/Time"
    if (firebaseData.dataType() == "string") {
      String timeString = firebaseData.stringData();

      // Extract hours and minutes from the time string
      int hours, minutes;
      char amPm[3];
      sscanf(timeString.c_str(), "%d:%d %2s", &hours, &minutes, amPm);

      // Adjust hours for 12-hour format
      if (strcmp(amPm, "PM") == 0 && hours != 12) {
        hours += 12;
      } else if (strcmp(amPm, "AM") == 0 && hours == 12) {
        hours = 0;
      }

      // Calculate total minutes since midnight
      return  (hours * 60) + minutes;
    }
  } else {
    Serial.println("Error getting value");
    Serial.println(firebaseData.errorReason());
  }
  return 0;

}
