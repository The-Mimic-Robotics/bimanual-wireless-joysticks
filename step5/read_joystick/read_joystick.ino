const uint8_t XPOS_PIN = 33;
const uint8_t YPOS_PIN = 25;
const uint8_t PUSH_PIN = 26;

int mapJoystick(int value)
{
  return map(value, 0, 4095, -32767, 32767);
}

void setup()
{
  pinMode(XPOS_PIN, INPUT);
  pinMode(YPOS_PIN, INPUT);
  pinMode(PUSH_PIN, INPUT);

  Serial.begin(115200);
}

void loop()
{
  Serial.print("X: ");
  Serial.print(mapJoystick(analogRead(XPOS_PIN)));

  Serial.print(", Y: ");
  Serial.print(mapJoystick(analogRead(YPOS_PIN)));

  Serial.print(", and PUSH: ");
  Serial.println(digitalRead(PUSH_PIN));

  delay(10);
}
