#include<Arduino.h>
#include<iostream>
#include<limits.h>

// Constants
const float V_REF = 5.0;     // Analog reference voltage (e.g., 5V or 3.3V)
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

// Setup pressure sensor zero
int incomingByte = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  Poffset = 8.531;
  // put your main code here, to run repeatedly:
  int rawValue = analogRead(analogPin); // Read the analog input
  float voltage = (rawValue / ADC_STEPS) * V_REF; // Convert to voltage

  Serial.print("Voltage: ");
  Serial.print(voltage, 2); // Print voltage with 3 decimal places
  Serial.println(" V");

  float output = voltage;
  float pressure = ((output-Omin)*(Pmax-Pmin)/(Omax-Omin))+Pmin-Poffset;
  Serial.print("Pressure ");
  Serial.print(pressure, 3); // Print pressure with 3 decimal places
  Serial.println(" mmHg");

  // systolic and diastolic 
  

  // check if zero button is pressed
  if (Serial.available() > 0) {

    // read the incoming byte:
    incomingByte = Serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    Poffset = pressure;
  }

  // if button is pressed (eg digital high)
  // then set poffset to current pressure
  
  delay(3000); // 3 sec delay
}
