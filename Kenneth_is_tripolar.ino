// This Arduino example demonstrates bidirectional operation of a
// 28BYJ-48, using a VMA401 - ULN2003 interface board to drive the stepper.
// The 28BYJ-48 motor is a 4-phase, 8-beat motor, geared down by
// a factor of 68. One bipolar winding is on motor pins 1 & 3 and
// the other on motor pins 2 & 4. The step angle is 5.625/64 and the
// operating Frequency is 100pps. Current draw is 92mA.
////////////////////////////////////////////////
//declare variables for the motor pins and accelerometer functions
#include<Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
int motorPin1 = 8; // Blue - 28BYJ48 pin 1
int motorPin2 = 9; // Pink - 28BYJ48 pin 2
int motorPin3 = 10; // Yellow - 28BYJ48 pin 3
int motorPin4 = 11; // Orange - 28BYJ48 pin 4
 // Red - 28BYJ48 pin 5 (VCC)
int motorSpeed = 800; // variable to set stepper speed; ms in between steps so
                      // decrease to speed up and increase to slow down
int count = 0; // count of steps made
int countsperrev = 256; // number of steps per full revolution
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//////////////////////////////////////////////////////////////////////////////
void setup() {
//declare the motor pins as outputs
 pinMode(motorPin1, OUTPUT);
 pinMode(motorPin2, OUTPUT);
 pinMode(motorPin3, OUTPUT);
 pinMode(motorPin4, OUTPUT);

//open port for accel + gyro
 Wire.begin();
 Wire.beginTransmission(MPU6050_addr);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);
 Serial.begin(9600);
}
//////////////////////////////////////////////////////////////////////////////
void loop(){
 if(count < countsperrev )
 clockwise();
 else if (count == countsperrev * 2)
 count = 0;
 else
 anticlockwise();
 
  count++;
 Wire.beginTransmission(MPU6050_addr);
 Wire.write(0x3B);
 Wire.endTransmission(false);
 Wire.requestFrom(MPU6050_addr,14,true);
 AccX=Wire.read()<<8|Wire.read();
 AccY=Wire.read()<<8|Wire.read();
 AccZ=Wire.read()<<8|Wire.read();
 Temp=Wire.read()<<8|Wire.read();
 GyroX=Wire.read()<<8|Wire.read();
 GyroY=Wire.read()<<8|Wire.read();
 GyroZ=Wire.read()<<8|Wire.read();
 Serial.print(AccX);
 Serial.print(","); 
 Serial.print(AccY);
 Serial.print(","); 
 Serial.print(AccZ);
 Serial.print(","); 
 Serial.print(GyroX);
 Serial.print(","); 
 Serial.print(GyroY);
 Serial.print(","); 
 Serial.println(GyroZ);
 delay(50);
}
//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void anticlockwise()
{
 for(int i = 0; i < 8; i++)
 {
 setOutput(i);
 delayMicroseconds(motorSpeed);
 }
}
void clockwise()
{
 for(int i = 7; i >= 0; i--)
 {
 setOutput(i);
 delayMicroseconds(motorSpeed);
 }
}
void setOutput(int out)
{
 digitalWrite(motorPin1, bitRead(lookup[out], 0));
 digitalWrite(motorPin2, bitRead(lookup[out], 1));
 digitalWrite(motorPin3, bitRead(lookup[out], 2));
 digitalWrite(motorPin4, bitRead(lookup[out], 3));
}
