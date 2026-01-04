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

//byte SERVO_ID = 1; // ID of the servo currently being tested.

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
}

// placeholder var for position read and calibrated position (adapted from -100 to 100)
int position, pos;

// helper function to adapt a percentage position to a proper position that will correspond to the motor's range
int tweak(int p) { return map(p, -100, 100, 500, 3260); }

void loop()
{
  // Student Node: Serial.printf does not work and prints weird characters to terminal

  // Print calibrated position of ID 1
  position = servos.getCurrentPosition(1);

  // 500 is the max anti-clockwise position read & 3260 is the max clockwise position read from the test servo
  // for production use, a calibration script must memorize the min and max position values and store them in a var
  // for production use, line 53 will look like this:
  //pos = map(position, min_pos, max_pos, -100, 100);
  // -100 is the max left and 100 is the max right, 0 represents the arm pointing straight forward
  pos = map(position, 500, 3260, -100, 100);

  // Set position of ID 1 to 50% to the right
  servos.setTargetPosition(1, tweak(50));
  delay(500); while (servos.isMoving(1)) delay(50);

  // Set position of ID 1 to 2% to the right
  servos.setTargetPosition(1, tweak(2));
  delay(500); while (servos.isMoving(1)) delay(50);

  // Set position of ID 1 to 80% to the right
  servos.setTargetPosition(1, tweak(80));
  delay(500); while (servos.isMoving(1)) delay(50);

  // Set position of ID 1 to 80% to the left
  servos.setTargetPosition(1, tweak(-80));
  delay(500); while (servos.isMoving(1)) delay(50);

  // Set position of ID 1 to 50% to the left
  servos.setTargetPosition(1, tweak(-50));
  delay(500); while (servos.isMoving(1)) delay(50);

  // You get the idea...
  servos.setTargetPosition(1, tweak(-10));
  delay(500); while (servos.isMoving(1)) delay(50);

  // Many positions were tested to make sure that the servo does not try to turn clockwise when going from the max right (100) to the max left (-100)
  // for clarification, the servo tested with this code NEVER attempts to turn clockwise when rotating from 100 to -100
  // meaning that there should be no danger
  servos.setTargetPosition(1, tweak(100));
  delay(500); while (servos.isMoving(1)) delay(50);

  servos.setTargetPosition(1, tweak(30));
  delay(500); while (servos.isMoving(1)) delay(50);

  servos.setTargetPosition(1, tweak(-100));
  delay(500); while (servos.isMoving(1)) delay(50);
}
