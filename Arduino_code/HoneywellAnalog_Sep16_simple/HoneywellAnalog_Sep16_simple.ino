#include<Arduino.h>
#include<iostream>
#include<limits.h>

// Constants
const float V_REF = 5.1;     // Analog reference voltage (e.g., 5V or 3.3V)
const float R_BITS = 10.0;   // ADC resolution (bits)
const float ADC_STEPS = (1 << int(R_BITS)) - 1; // Number of steps (2^R_BITS - 1)

const int analogPin = A4;
int val = 0;

// Sensor constants
float Omax = 0.9*V_REF; // Output at maximum pressure
float Omin = 0.1*V_REF; // Output at minimum pressure
float Pmax = 1*51.7149; // 1 psi to mmhg
float Pmin = -1*51.7149; // -1 psi to mmhg
float Poffset = 0; // offset from zero
float pressure = 2000; // initial pressure
float pressure_old = 2000;

// millis setup
unsigned long previousMillis = 0;  // will store last time sampling was updated
unsigned long previousPrintMillis = 0;  // will store last time print sampling was updated
const long interval = 50;  // interval at which to sample (milliseconds)
const long printInterval = 50; // interval at which to print (milliseconds)

int once_counter = 0; // counter for instant zeroing

// Setup pressure sensor zero - character input
char receivedChar = 0;
boolean newData = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}


void loop() {
// put your main code here, to run repeatedly:
  
  unsigned long currentMillis = millis();
  unsigned long currentPrintMillis = millis();

  // Sample
  if (currentMillis - previousMillis >= interval) {
    // save the last time we sampled
    previousMillis = currentMillis;

    float rawValue = analogRead(analogPin); // Read the analog input
    float voltage = (rawValue / ADC_STEPS) * V_REF; // Convert to voltage
    float output = voltage;
    pressure = ((output-Omin)*(Pmax-Pmin)/(Omax-Omin))+Pmin; // calculate pressure

    pressure_old = pressure;
    recvOneChar();
    showNewData();
    pressure = pressure-Poffset;

    // Print
  if (currentPrintMillis - previousPrintMillis >= printInterval) {
  // save the last time we sampled
    previousPrintMillis = currentPrintMillis;
    Serial.print("{\"Pressure\":");
    Serial.print(pressure,1); // Print pressure with 1 decimal places
    Serial.println("}");
    //Serial.println(output);
  }

  }
  
  
  
  // systolic and diastolic 

  // instant zeroing
  //if(once_counter < 1) {
  //  Poffset = pressure;
  //}
  //once_counter = 1;
  
//delay(1000); 
}

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
