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

  
   long irValue = particleSensor.getIR();
 // long RedValue = particleSensor.getRed();
  //long GreenValue = particleSensor.getGreen() ;

   if (checkForBeat(irValue) == true)
   {
     //We sensed a beat!
     long delta = millis() - lastBeat;
     lastBeat = millis();

     beatsPerMinute = 60 / (delta / 1000.0);

     if (beatsPerMinute < 255 && beatsPerMinute > 20)
     {
       rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
       rateSpot %= RATE_SIZE; //Wrap variable

       //Take average of readings
       beatAvg = 0;
       for (byte x = 0 ; x < RATE_SIZE ; x++)
         beatAvg += rates[x];
       beatAvg /= RATE_SIZE;
     }
   }

sensors_event_t a, g, temp; 
mpu.getEvent(&a, &g, &temp); 
Serial.print("<");   //to sync data in python recording
Serial.print(",IR:");
Serial.print(irValue );
 Serial.print(",BPM:");
 Serial.print(beatsPerMinute);
 Serial.print(",HR:");
 Serial.print(beatAvg);

Serial.println(">");  //to sync data in python recording 


}