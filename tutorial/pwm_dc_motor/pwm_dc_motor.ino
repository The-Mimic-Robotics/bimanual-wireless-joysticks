/* Resources

https://projecthub.arduino.cc/Fouad_Roboticist/dc-motors-control-using-arduino-pwm-with-l298n-h-bridge-25b3b3
*/

#define PWM_PIN 25
#define DIR_PIN 26

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop() {
  //Student Note:
  // PWM needs analogWrite(M1_PWM_PIN, [0,255] );
  // DC needs digitalWrite(M1_DIR_PIN, {0,1}   );

  digitalWrite(DIR_PIN, 1);
  analogWrite(PWM_PIN, 128);
  delay(1000);

  digitalWrite(DIR_PIN, 0);
  analogWrite(PWM_PIN, 128);
  delay(1000);

  digitalWrite(DIR_PIN, 1);
  analogWrite(PWM_PIN, 255);
  delay(1000);

  digitalWrite(DIR_PIN, 0);
  analogWrite(PWM_PIN, -255);
  delay(1000);

  analogWrite(PWM_PIN, 0);
  delay(2000);
}
