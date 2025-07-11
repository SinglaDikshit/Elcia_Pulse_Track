#include <Wire.h>  //I2C communication library
#include "MAX30105.h"  //max sensor library
#include "heartRate.h"  //heart rate calculation

#include <Adafruit_MPU6050.h>  //MPU library
#include <Adafruit_Sensor.h>// unified sensor

MAX30105 particleSensor;
Adafruit_MPU6050 mpu;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup()
{
  Serial.begin(9600);
    Wire.begin(21, 22);
  Serial.println("MAX30105 Basic Readings Example");

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
   
  // MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  particleSensor.setPulseAmplitudeRed(0x0A);  // 6.4mA
  particleSensor.setPulseAmplitudeGreen(0); 

}

void loop()
{
sensors_event_t a, g, temp; 
mpu.getEvent(&a, &g, &temp);
Serial.print("<");
Serial.print(",Ax:"); 
Serial.print(a.acceleration.x); 
Serial.print(",Ay:"); 
Serial.print(a.acceleration.y); 
Serial.print(",Az:"); 
Serial.print(a.acceleration.z); 
Serial.print(",Rx:"); 
Serial.print(g.gyro.x); 
Serial.print(",Ry:"); 
Serial.print(g.gyro.y); 
Serial.print(",Rz:"); 
Serial.print(g.gyro.z); // data output
Serial.println(">");  //to sync for data recording in python


}
