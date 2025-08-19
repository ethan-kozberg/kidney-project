#include<Arduino.h>
#include<iostream>
#include<limits.h>

// Constants
const float V_REF = 5.172;     // Analog reference voltage (e.g., 5V or 3.3V)
const float R_BITS = 10.0;   // ADC resolution (bits)
const float ADC_STEPS = (1 << int(R_BITS)) - 1; // Number of steps (2^R_BITS - 1)

const int analogPin = A3;
int val = 0;

// Sensor constants
float Omax = 0.9*V_REF; // Output at maximum pressure
float Omin = 0.1*V_REF; // Output at minimum pressure
float Pmax = 1*51.7149; // 1 psi to mmhg
float Pmin = -1*51.7149; // -1 psi to mmhg
float Poffset = 0; // offset from zero
float pressure = 2000; // initial pressure
float pressure_old = 2000;

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
  
  float rawValue = analogRead(analogPin); // Read the analog input
  float voltage = (rawValue / ADC_STEPS) * V_REF; // Convert to voltage
  float output = voltage;
  pressure = ((output-Omin)*(Pmax-Pmin)/(Omax-Omin))+Pmin; // calculate pressure

  pressure_old = pressure;
  recvOneChar();
  showNewData();
  pressure = pressure-Poffset;


  // Print
  Serial.print("{\"Pressure\":");
  Serial.print(pressure,1); // Print pressure with 1 decimal places
  Serial.println("}");
  
  // systolic and diastolic 

  // instant zeroing
  //if(once_counter < 1) {
  //  Poffset = pressure;
  //}
  //once_counter = 1;
  
 delay(1000); 
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
