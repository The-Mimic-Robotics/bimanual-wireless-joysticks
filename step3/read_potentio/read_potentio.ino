void setup()
{
  Serial.begin(115200);
  pinMode(26, INPUT);
  pinMode(25, INPUT);
  pinMode(33, INPUT);
}

void loop()
{
  Serial.print("Read 1: ");
  Serial.print(analogRead(26));

  Serial.print(", Read 2: ");
  Serial.print(analogRead(25));

  Serial.print(", Read 3: ");
  Serial.println(analogRead(33));

  delay(500);
}
