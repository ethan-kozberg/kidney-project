#include<Arduino.h>
#include<iostream>
#include<limits.h>
#include<CircularBuffer.hpp>

// circular buffer
CircularBuffer<int, 64> buffer;
int INTERVAL = 100; // ms interval
unsigned long _time = 0;

// Constants
const float V_REF = 5.0;     // Analog reference voltage (e.g., 5V or 3.3V)
const int analogPin = A0; // esp32-c3 can only do A0-A2

// Sensor constants
float Omax = 0.9*V_REF; // Output at maximum pressure
float Omin = 0.1*V_REF; // Output at minimum pressure
float Pmax = 15*51.7149; // 15 psi to mmhg
float Pmin = 0*51.7149; // 0 psi to mmhg
float Poffset = 0; // offset from zero
float pressure = 2000.0; // initial pressure
float pressure_old = 2000.0;

// Setup pressure sensor zero - character input
char receivedChar = 0;
boolean newData = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(A0, INPUT);
  _time = millis();
}


void loop() {
// put your main code here, to run repeatedly

  float voltage =  analogReadMilliVolts(analogPin)*.001;
  float output = voltage;
  pressure = ((output-Omin)*(Pmax-Pmin)/(Omax-Omin))+Pmin; // calculate pressure

  pressure_old = pressure;
  recvOneChar();
  showNewData();
  pressure = pressure-Poffset;

  buffer.push(pressure);

  if (millis() - _time > INTERVAL) {
    _time = millis();
    float avg = 0.0;
    float sys = 0.0;
    float dia = 0.0;
    float map = 0.0;
    
    // the following ensures using the right type for the index variable
		using index_t = decltype(buffer)::index_t;
    for (index_t i = 0; i < buffer.size(); i++) {
			avg += buffer[i] / (float)buffer.size();
      if (buffer[i] >= sys) {
        sys = buffer[i];
        }
      if (buffer[i] <= dia) {
        dia = buffer[i];
        }
      map = dia + ((sys-dia)/3);
		}

    Serial.print("{\"SYS\":"); 
    Serial.print(sys,1); // Print SYS with 1 decimal places
    Serial.print(",\"DIA\":"); 
    Serial.print(dia,1); // Print DIA with 1 decimal places
    Serial.print(",\"MAP\":"); 
    Serial.print(map,1); // Print MAP with 1 decimal places
    Serial.print(",\"AVG\":"); 
    Serial.print(avg,1); // Print AVG with 1 decimal places
    Serial.println("}");
  }
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
