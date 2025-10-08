#include<CSE_CircularBuffer.hpp>
#include<CSE_MillisTimer.h>  // Requires the timer library
#include<Arduino.h>
#include<iostream>
#include<limits.h>
#include<Wire.h>
#include<Adafruit_ADS1X15.h>

// ADC
Adafruit_ADS1115 ads; //construct ads1115
float V_REF = 5; // voltage

// Sensor constants
float Omax = 0.9*V_REF; // Output at maximum pressure
float Omin = 0.1*V_REF; // Output at minimum pressure
float Pmax = 15*51.7149; // 1 psi to mmhg
float Pmin = 0*51.7149; // 0 psi to mmhg
float Poffset = 0; // offset from zero
float pressure = 2000; // initial pressure
float pressure_old = 2000;

// Define buffer
//CSE_CircularBuffer <int> cbuffer(100); // Create a buffer with capacity of 100
//CSE_MillisTimer printTimer(500); // Create a timer with a given interval

// Setup pressure sensor zero - character input
char receivedChar = 0;
boolean newData = false;

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello!");
  Wire.begin(5,4);
  ads.begin();
  //printTimer.start();
}


void loop() {
// put your main code here, to run repeatedly:

  // Sample
    int16_t adc0, adc1, adc2, adc3;
    float volts0, volts1, volts2, volts3;
    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);
    adc2 = ads.readADC_SingleEnded(2);
    adc3 = ads.readADC_SingleEnded(3);

    volts0 = ads.computeVolts(adc0);
    volts1 = ads.computeVolts(adc1);
    volts2 = ads.computeVolts(adc2);
    volts3 = ads.computeVolts(adc3);
    
    float output = volts1;
    pressure = ((output-Omin)*(Pmax-Pmin)/(Omax-Omin))+Pmin; // calculate pressure

    pressure_old = pressure;
    recvOneChar(); // call function to receive zeroing input
    showNewData(); // set zero
    pressure = pressure-Poffset;

    Serial.println(adc1);
    
    /*
    cbuffer.push(pressure); // Push to buffer

    if (printTimer.isElapsed()) { // Print according to timer
      
      Serial.print("{\"Pressure\":");
      Serial.print(pressure,1); // Print pressure with 1 decimal places
      Serial.println("}");

      printTimer.start(); // Reset the timer
    } // end of printing */
    delay(1000);
} // end of void loop

// functions
void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

void showNewData() {
  
    if (newData == true) {
      if (receivedChar == 'z') {
        Poffset = pressure_old;
        //Serial.print("This just in ... ");
        //Serial.println(Poffset);
      }
      newData = false;
    }
    
}
