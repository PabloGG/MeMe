/* Wii Nunchuk + Servos + Ultrasonic Sensor = MeMe (:O) ver.-1.0.5
* by Scrawny @ http://bricolabs.cc
*
* Show by Serial Monitor the status of:
* - Joy X | Joy Y | Roll | Pitch | Accel X | Accel Y | Accel Z | Buttons - Uncomment if necessary
* - Measure in cm using HC-SR04 Ultrasonic Sensor
*
* Config Serial @ 57600 baud
*
* First steep you need define default idle Zero (90 degrees) in Joy_X and Joy_Y
* in WiiChuck.h file (DEFAULT_ZERO_JOY_X & DEFAULT_ZERO_JOY_Y)
* to get a default value nearest 90 degrees in Joy and servo
*
* Nunchuk attachment:
* GND --> Arduino GND
* VCC --> Arduino 3,3V
* SDA (Data) --> Arduino Analog In A4
* SCL (Clock) --> Arduino Analog In A5
*
* HC-SR04 Ping attachment:
* VCC --> Arduino 5V
* GND --> Arduino GND
* Echo --> Arduino Digital 7 
* Trig --> Arduino Digital 8
*
* Servo1 signal --> Arduino Digital 5
* Servo2 signal --> Arduino Digital 6
* Servo3 signal --> Arduino Digital 9
*
* Nunchuk Servo code adapted from original code by TheMultiIng http://youtu.be/I_N2WGTjWk8
*
* HC-SR04 Ultrasonic Sensor code by ScottC http://arduinobasics.blogspot.com/
**/

#include <math.h>
#include <Wire.h>
#include <WiiChuck.h>
#include <Servo.h> 

// #define MAXANGLE 180
// #define MINANGLE 0
#define echoPin 7 // HC-SR04 Echo Pin
#define trigPin 8 // HC-SR04 Trigger Pin
#define LEDPin 13 // Onboard LED

Servo servo1;
Servo servo2;
Servo servo3;
int limitS1, limitS2, limitS3;
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

WiiChuck chuck = WiiChuck();

void setup() {

  Serial.begin(57600);
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(9);
  chuck.begin();
  chuck.update();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
}

void loop() {
/** Servo Movement **/ 
  delay(15);
  chuck.update(); 
  /* Show Nunchuk status/position by serial */
  
    Serial.print("Joystick X: ");
    Serial.print(chuck.readJoyX());
    Serial.print(" | ");  
    Serial.print("Joystick Y: ");
    Serial.print(chuck.readJoyY());
    Serial.print(" | ");  
    Serial.print("Rotacion eje logitudinal: ");  
    Serial.print(chuck.readRoll());
    Serial.print(" | ");
    Serial.print("Rotacion eje transversal: ");   
    Serial.print(chuck.readPitch());
    Serial.print(" | ");  
    Serial.print("Acelerometro X: "); 
    Serial.print((int)chuck.readAccelX()); 
    Serial.print(" | ");  
    Serial.print("Acelerometro Y: "); 
    Serial.print((int)chuck.readAccelY()); 
    Serial.print(" | ");  
    Serial.print("Acelerometro Z: "); 
    Serial.print((int)chuck.readAccelZ()); 
    Serial.print(" | ");
    Serial.print("Boton: ");
    if (chuck.buttonC) {
     Serial.print("C");
    } else  {
     Serial.print(" - ");
    }
    Serial.print(", ");  
    if (chuck.buttonZ) {
     Serial.print("Z");
    } else  {
     Serial.print(" - ");
    }
    Serial.println();
    
   
    //servo1.write((int)chuck.readJoyY()); //full movement, 0 to 180 degrees
    /* Limit movement servo JoyY */
    limitS1 = chuck.readJoyY();
    limitS1 = map(limitS1, 0, 179, 40, 112); // 45 to 115 degrees
    servo1.write(limitS1);
    /* End Limit movement servo JoyY */
    
    //servo1.write((int)chuck.readRoll()); //full movement, 0 to 180 degrees
    /* Limit movement servo Roll */
    limitS2 = chuck.readRoll();
    limitS2 = map(limitS2, 0, 179, 80, 140); // -20 to 40 degrees
    servo2.write(limitS2);
    /* End Limit movement servo Roll */
    
    
    //servo3.write((int)chuck.readJoyX()); //full movement, 0 to 180 degrees
    /* Limit movement servo JoyX */
    limitS3 = chuck.readJoyX();
    limitS3 = map(limitS3, 0, 179, 179, 0); // full movement, but 180 to 0 degrees
    servo3.write(limitS3);
    /* End Limit movement servo JoyX */
    
    

/** Measure Meter **/ 
  if (chuck.buttonC == HIGH){
    delay(2000);      // time (in ms) that servos are stopped before measure
   // servo1.write((int)chuck.readJoyX());   // stops JoyX position value for full movement
    servo1.write(limitS1);   // stops JoyX position for limit movement

    servo3.write((int)chuck.readJoyY());   // stops JoyY position value for full movement
    
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    //Calculate the distance (in cm) based on the speed of sound.
    distance = duration/58.2;
    
    if (distance >= maximumRange || distance <= minimumRange){
    /* Send a negative number to computer and Turn LED ON to indicate "out of range" */
      Serial.println("-1");
      digitalWrite(LEDPin, HIGH); 
      }
      else {
      /* Send the distance to the computer using Serial protocol, and turn LED OFF to indicate successful reading */
        Serial.print(distance);
        Serial.println(" cm"); 
        digitalWrite(LEDPin, LOW); 
        delay(5000);    // delay to show the measure before start the loop again
        }
    }
}
