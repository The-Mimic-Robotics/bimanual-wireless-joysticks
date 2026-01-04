/* Resources
https://github.com/matthieuvigne/STS_servos/blob/main/examples/SimpleMotion/SimpleMotion.ino
https://github.com/matthieuvigne/STS_servos/blob/main/src/STSServoDriver.h
https://github.com/matthieuvigne/STS_servos/blob/main/src/STSServoDriver.cpp
*/

#include <BluetoothSerial.h>
#include <STSServoDriver.h>

// FE-URT-1 TXD Pin -> ESP32 RX2 Pin (Pin 16)
// FE-URT-1 RXD Pin -> ESP32 TX2 Pin (Pin 17)
// FE-URT-1 GND Pin -> ESP32 GND Pin
// FE-URT-1 5V  Pin -> ESP32 5V  Pin
// FE-URT-1 DTR Pin -> None

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#define RX2 17
#define TX2 16

STSServoDriver servos;

BluetoothSerial SerialBT;
String device_name = "ESP32-BT-Slave";
bool o = true;

int offsetServo1, offsetServo2, offsetServo3;

void setup() {
  Serial.begin(115200);                           // esp32 (1): controls servers; prints feedback
  Serial1.begin(1000000, SERIAL_8N1, RX2, TX2);   // serial channel to control servo motors
  // Student Note: DO NOT USE TX0 and RX0 since this is the main Serial channel to communicate with the ESP32

  delay(1000);

  // this does not print to Serial, but servos.init(&Serial1) works
  if (servos.init(&Serial1)) Serial.print("Servos are ready\n\r");

  if (Serial1) Serial.println("Serial1 is ready\n\r"); else Serial.println("Serial1 was not found\n\r");

  //pinMode(13, OUTPUT); digitalWrite(13, LOW); if (!servos.init(2)) digitalWrite(13, HIGH);

  servos.setMode(0xFE, STSMode::POSITION);

  SerialBT.begin(device_name);                    // esp32 (2): reads the potentiometer
  pinMode(26, INPUT);
  pinMode(25, INPUT);
  pinMode(33, INPUT);
}

// helper function to adapt a read position
int adapt(int p) { return map(p, 500, 3260, -100, 100); }

// helper function to adapt a percentage position to a position in the range of the servo
int tweak(int p) { return map(p, -100, 100, 500, 3260); }

void loop()
{
  // Student Note: Serial.printf does not work and prints weird characters to terminal

  // Get potentiometers values from esp32 (2) to use for the offsets
  if (SerialBT.available())
  {
    offsetServo1 = SerialBT.parseInt();
    offsetServo2 = SerialBT.parseInt(); //should be 1200
    offsetServo3 = SerialBT.parseInt(); //should be 1000

    while (SerialBT.available() > 0) SerialBT.read();  // Serial flush to stop reading buffer
  }

  servos.setPositionOffset(1, offsetServo1); // Correcting the offset of ID 1
  servos.setPositionOffset(2, offsetServo2); // Correcting the offset of ID 2
  servos.setPositionOffset(3, offsetServo3); // Correcting the offset of ID 3

  Serial.print("ID 1: ");                                                 // Get position of ID 1 and print it
  Serial.print(map(servos.getCurrentPosition(1), 500, 3260, 100, -100));  // 500 is the max left position and 3260 is the max right position; these values should be var gotten from a calibration script

  Serial.print(", ID 2: ");                                               // Get position of ID 2 and print it
  Serial.print(map(servos.getCurrentPosition(2), 4100, 1900, 100, -100)); // 4100 is the max backward position and 1900 is the max forward position; these values should be variables gotten from a calibration script

  Serial.print(", ID 3: ");                                                 // Get position of ID 3 and print it
  Serial.println(map(servos.getCurrentPosition(3), 1870, 3970, -100, 100)); // 1870 is the max downward position and 3970 is the max upward position; these values should be variables gotten from a calibration script

  delay(500);
}
