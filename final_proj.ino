#include "Wire.h"
#include "SHT31.h"

#include <PID_v1.h>

double Setpoint, Input, Output;
double Kp = 50, Ki = 0.1, Kd = 0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

#define SHT31_ADDRESS   0x44
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;

int lamppin = 10;
int motorpin = 9;

uint32_t start;
uint32_t stop;

SHT31 sht;
uint32_t connectionFails = 0;

void setup()
{
  Setpoint = 28;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);


  Serial.begin(9600);
  // Serial.println(__FILE__);
  // Serial.print("SHT31_LIB_VERSION: \t");
  // Serial.println(SHT31_LIB_VERSION);

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(lamppin, OUTPUT);
  pinMode(motorpin, OUTPUT);


  Wire.begin();
  sht.begin(SHT31_ADDRESS);
  Wire.setClock(100000);

  uint16_t stat = sht.readStatus();
  //Serial.print(stat, HEX);
  //Serial.println();
}

void loop()
{
  //analogWrite(9,250);
  Setpoint = Serial.readStringUntil('\n').toDouble();
  if (Setpoint == 0) {
    Setpoint = 28;
  }
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  //Serial.print("\n");
  //Serial.print("OUtput:");
  //Serial.print(Output);
  //Serial.print("\n");

  //Serial.print(myPID.GetDirection());

  //armicher
  //myPID.SetControllerDirection(1);

  //lamp
  //myPID.SetControllerDirection(0);


  if (Input - Setpoint < -2 ) {
    myPID.SetControllerDirection(1);
    analogWrite(10, 250);
    analogWrite(9, 0);
  } if (Input - Setpoint > 2) {
    myPID.SetControllerDirection(0);
    analogWrite(10, 0);
    analogWrite(9, Output);

  }



  if ( sht.isConnected()  )


  {
    start = micros();
    sht.read();         // default = true/fast       slow = false
    stop = micros();
    //Serial.print(millis());
    //Serial.print("\t");
    //Serial.print(stop - start);
    //Serial.print("\t");
    //Serial.print(sht.getTemperature(), 1);
    Input = sht.getTemperature();
    //Serial.print("\t");
    myPID.Compute();
  }
  else
  {
    connectionFails++;
    //Serial.print(millis());
    //Serial.print("\tNot connected:\t");
    //Serial.print(connectionFails);
    // sht.reset();
  }
  Serial.println(Input);
  Serial.println(Input - Setpoint);
  delay(100);
}
