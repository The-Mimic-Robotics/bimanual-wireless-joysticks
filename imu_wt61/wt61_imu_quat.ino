#include <Wire.h>
#include <JY901.h>
/*
Test on Uno R3.
JY901    UnoR3
SDA <---> SDA
SCL <---> SCL
*/
void setup() 
{
  Serial.begin(115200);
  JY901.StartIIC();
} 

void loop() 
{
  //print received data. Data was received in serialEvent;
  JY901.GetTime();
  // Serial.print("Time:20");Serial.print(JY901.stcTime.ucYear);Serial.print("-");Serial.print(JY901.stcTime.ucMonth);Serial.print("-");Serial.print(JY901.stcTime.ucDay);
  // Serial.print(" ");Serial.print(JY901.stcTime.ucHour);Serial.print(":");Serial.print(JY901.stcTime.ucMinute);Serial.print(":");Serial.println((float)JY901.stcTime.ucSecond+(float)JY901.stcTime.usMiliSecond/1000);
            
  JY901.GetQuaternion();
  Serial.print("Quat: ");
  Serial.print((float)JY901.stcQuater.q0/32768,5);
  Serial.print(" ");
  Serial.print((float)JY901.stcQuater.q1/32768,5);
  Serial.print(" ");
  Serial.print((float)JY901.stcQuater.q2/32768,5);
  Serial.print(" ");
  Serial.println((float)JY901.stcQuater.q3/32768,5);

  JY901.GetAngle();
  //Serial.print("Angle:");
  Serial.print("Yaw: ");
  Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  // Serial.println("");
  delay(50);
}


