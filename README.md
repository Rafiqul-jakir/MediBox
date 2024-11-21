# MediBox - An IoT based smart medicine dispenser for optimal healthcare with mobile application assistance and security system.
The Smart Medicine Dispenser System, named MediBox, is a smart medicine dispenser designed to help individuals manage their medication schedule efficiently. This system includes 6 drawers, each controlled by an SG90 servo motor, a buzzer for alarms, an LED indicator for notifications, and an on/off switch to confirm medication intake.

<div style="display: flex; justify-content: space-around;">
  <img src="https://github.com/Rafiqul-jakir/MediBox/blob/main/MediBox_front_side.jpg" alt="Image 1" width="500" />
  <img src="https://github.com/Rafiqul-jakir/MediBox/blob/main/Medibox_backSid.jpg" alt="Image 2" width="500" />
</div>

## Features
**1. Drawer System:** The mediBox is equipped with 6 drawers, each operated by a dedicated SG90 servo motor. These drawers are used to store and dispense medication at scheduled times.

**2. Alarm and Notification:** A buzzer is integrated into the system to provide audible alarms when it's time to take medication. An LED indicator blinks to visually notify the user.

**3. On/Off Switch:** The on/off switch is used to confirm that the patient has taken their medication. It serves as a manual input mechanism to acknowledge the medicine intake.

**4.Mobile App Control:** The mediBox is controlled through an Android app. Users can set the medication schedule, receive reminders, and manually trigger the dispenser through the app.
App Link [Click Here](https://github.com/Kawser-Ahamed/MediBox)

## Usage
**1. Setting Medication Schedule:**
Open the mobile app and set the medication schedule, specifying the times for each drawer to open.

**2. Receiving Alerts:**
When it's time to take medication, the buzzer will sound, and the LED indicator will blink.

**3. Dispensing Medication:**
Open the app to confirm medication intake by pressing the on/off switch. This will also disable the alarm.

**4. Manual Dispensing:**
In case of a missed alarm, users can manually trigger the dispenser through the app.
## Installation
**1. Clone the repository to your local machine:**
```
git clone https://github.com/Rafiqul-jakir/MediBox.git
```
**2. Open the project in your preferred development environment.**

**3. Upload the mediBox code to the microcontroller.**

**4. Install the Android app on your mobile device.**

**5. Connect the app to the mediBox system.**

## Dependencies
- **Arduino IDE 2.2.1**

- **Board Manager**
    - esp8266 by ESP8266 Community 2.7.4

- **Libraries**
    - NTPClient by Fabrice Weinberg 3.2.1
    - ArduinoJson by Benoit Blanchon 5.13.3
    - Firebase ESP8266 Client by Mobizt 3.1.9
    - Time by Michael Margolis 1.6.1

## Contributing
Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

***Happy Coding -_-***
