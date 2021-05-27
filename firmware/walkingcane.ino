// Define dependancies
#include "Arduino.h"
#include "VibrationMotor.h"

// Define Ultrasonic Sensor Pins
#define lecho  2
#define ltrig  3
#define mecho  4
#define mtrig  5
#define recho  6
#define rtrig  7

// Define Water Level Sensor Pin
#define wlvsig  A3

// Define Haptic Feedback Generator Pins
#define lhaptic  10
#define mhaptic  9
#define rhaptic  11

// Construct Haptic Feedback Objects
VibrationMotor leftout(lhaptic);
VibrationMotor midout(mhaptic);
VibrationMotor rightout(rhaptic);


// Arduino Initialization Runtime
void setup()
{
    // Initialize the Serial Port at 9600 bps
    Serial.begin(9600);

    // Set the Ultrasonic Sensor TRIG Pins to OUTPUT
    pinMode(ltrig, OUTPUT);
    pinMode(mtrig, OUTPUT);
    pinMode(rtrig, OUTPUT);

    // Set the Ultrasonic Sensor ECHO Pins to INPUT
    pinMode(lecho, INPUT);
    pinMode(mecho, INPUT);
    pinMode(recho, INPUT);

    // Set the Water Level Sensor Pin to INPUT
    pinMode(wlvsig , INPUT);

    // Set the Haptic Generator Pins to OUTPUT
    pinMode(lhaptic, OUTPUT);
    pinMode(mhaptic, OUTPUT);
    pinMode(rhaptic, OUTPUT);
}


// A function that calculates the distance of the nearest obstacle
// infront of an Ultrasonic sensor given its TRIG and ECHO pins.
int find_distance(int trigpin, int echopin) {
    // Set the TRIG pin to LOW and wait 2 ms
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);

    // Set the TRIG pin to HIGH and wait for 10 ms
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);

    // Set the TRIG pin to LOW
    digitalWrite(trigpin, LOW);

    // Read the ECHO pin, returns the sound wave travel time in microseconds
    int duration = pulseIn(echopin, HIGH);

    // Calculate the distance
    int distance = duration * 0.034 / 2;

    // Print the distance to Serial and return it
    Serial.println(distance);
    return distance;
}


// A function that returns a bool based on if an 
// obstacle is detected by ultrasonic sensor
bool sonicpoll(int trigpin, int echopin) {
    // Find the distance of the neareast obstacle
    int distance = find_distance(trigpin, echopin);
    delayMicroseconds(10);

    // Check if distance is between 5 and 100 cms away
    if (dist < 100 && dist > 5) {
        return true;
    } else {
        return false;
    }
}


// A function that returns a bool based on if the
// water level sensor detects any water at all
bool waterpoll()
{
    // Read the water level sensor
    int val = analogRead(wlvsig);

    // Check if there is any water 
    if (val > 0) {
        return true;
    } else {
        return false;
    }
}


// A function that activates the Haptics 
// for a given directional motor
void directional_haptic(VibrationMotor haptic) {
    haptic.on();
    delay(500);
    haptic.off();
    delay(500);
}


// A function that activates the Haptics
// for all directional motors
void full_haptic()
{
  leftout.on();
  midout.on();
  rightout.on();
  delay(500);

  leftout.off();
  midout.off();
  rightout.off();
  delay(500);
}


// Arduino Loop Runtime
void loop()
{  
    // Declare some boolean flags
    bool left, middle, right, water;
    
    // Poll the Ultrasonic Sensors
    left = sonicpoll(ltrig, lecho);
    right = sonicpoll(rtrig, recho);
    middle = sonicpoll(mtrig, mecho);
    // Poll the Water Level Sensor
    water = waterpoll();

    // Check the flags
    if (water) {
        // Vibrate all Haptics
        full_haptic();
    } else if (left) {
        // Vibrate the Left Haptic
        directional_haptic(leftout);
    } else if (right) {
        // Vibrate the Right Haptic
        directional_haptic(rightout);
    } else if (middle) {
        // Vibrate the Middle Haptic
        directional_haptic(midout);
    }
}
