/* Resources
https://github.com/matthieuvigne/STS_servos/blob/main/examples/SimpleMotion/SimpleMotion.ino
https://github.com/matthieuvigne/STS_servos/blob/main/src/STSServoDriver.h
https://github.com/matthieuvigne/STS_servos/blob/main/src/STSServoDriver.cpp
*/

#include <STSServoDriver.h>

// FE-URT-1 TXD Pin -> ESP32 RX2 Pin (Pin 16)
// FE-URT-1 RXD Pin -> ESP32 TX2 Pin (Pin 17)
// FE-URT-1 GND Pin -> ESP32 GND Pin
// FE-URT-1 5V  Pin -> ESP32 5V  Pin
// FE-URT-1 DTR Pin -> None

#define RX2 17
#define TX2 16

STSServoDriver servos;

void setup() {
  Serial.begin(115200); // print feedback
  Serial1.begin(1000000, SERIAL_8N1, RX2, TX2); // serial channel to control servo motors
  // Student Note: DO NOT USE TX0 and RX0 since this is the main Serial channel to communicate with the ESP32

  delay(1000); // delay for connection

  // this does not print, but servos.init(&Serial1) works
  if (servos.init(&Serial1)) Serial.print("Servos are ready\n\r");

  if (Serial1) Serial.println("Serial1 is ready\n\r"); else Serial.println("Serial1 was not found\n\r");

  //pinMode(13, OUTPUT); digitalWrite(13, LOW); if (!servos.init(2)) digitalWrite(13, HIGH);

  servos.setMode(0xFE, STSMode::POSITION);

  // Input pins for the 3 potentiometers
  pinMode(26, INPUT);
  pinMode(25, INPUT);
  pinMode(33, INPUT);
}

// helper function to adapt a read position
int adapt(int p) { return map(p, 500, 3260, -100, 100); }

// helper function to adapt a percentage position to a position in the range of the servo
int tweak(int p) { return map(p, -100, 100, 500, 3260); }

int m1_min_pos, m1_max_pos, m2_min_pos, m2_max_pos, m3_min_pos, m3_max_pos;

void loop()
{
  // Student Note: Serial.printf does not work and prints weird characters to terminal

  while (Serial.available() == 0)
  {
    m1_min_pos = analogRead(26);
    Serial.println(m1_min_pos);
    delay(500);
  }
  while (Serial.available() > 0) Serial.read();

  while (Serial.available() == 0)
  {
    m1_max_pos = analogRead(26);
    Serial.println(m1_max_pos);
    delay(500);
  }
  while (Serial.available() > 0) Serial.read();

  while (Serial.available() == 0)
  {
    m2_min_pos = analogRead(25);
    Serial.println(m2_min_pos);
    delay(500);
  }
  while (Serial.available() > 0) Serial.read();

  while (Serial.available() == 0)
  {
    m2_max_pos = analogRead(25);
    Serial.println(m2_max_pos);
    delay(500);
  }
  while (Serial.available() > 0) Serial.read();

  while (Serial.available() == 0)
  {
    m3_min_pos = analogRead(33);
    Serial.println(m3_min_pos);
    delay(500);
  }
  while (Serial.available() > 0) Serial.read();

  while (Serial.available() == 0)
  {
    m3_max_pos = analogRead(33);
    Serial.println(m3_max_pos);
    delay(500);
  }
  while (Serial.available() > 0) Serial.read();

  Serial.println("=== Recorded values ===");
  Serial.println(m1_min_pos);
  Serial.println(m1_max_pos);
  Serial.println(m2_min_pos);
  Serial.println(m2_max_pos);
  Serial.println(m3_min_pos);
  Serial.println(m3_max_pos);
  Serial.println("=== EOF ===");

  // Correcting the offset of ID 2
  servos.setPositionOffset(2, 1200);

  // Correcting the offset of ID 3
  servos.setPositionOffset(3, 1000);

  // Get position of ID 1 and print it
  Serial.print("ID 1: ");
  Serial.print(map(servos.getCurrentPosition(1), 500, 3260, 100, -100)); // 500 is the max left position and 3260 is the max right position; these values should be var gotten from a calibration script

  // Get position of ID 2 and print it
  Serial.print(", ID 2: ");
  Serial.print(map(servos.getCurrentPosition(2), 4100, 1900, 100, -100)); // 4100 is the max backward position and 1900 is the max forward position; these values should be variables gotten from a calibration script

  // Get position of ID 3 and print it
  Serial.print(", ID 3: ");
  Serial.println(map(servos.getCurrentPosition(3), 1870, 3970, -100, 100)); // 1870 is the max downward position and 3970 is the max upward position; these values should be variables gotten from a calibration script

  delay(500);
}
