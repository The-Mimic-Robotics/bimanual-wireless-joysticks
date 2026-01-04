#define VPOWER_PIN 1
#define GROUND_PIN 2
#define X_AXIS_PIN 3
#define Y_AXIS_PIN 4
#define SWITCH_PIN 5

unsigned int xAxis;
unsigned int yAxis;

void setup() {
  Serial.begin(9600);
  Serial.print("X Axis First");
  Seirial.print("Y Axis Second");

  pinMode(X_AXIS_PIN, INPUT);
  pinMode(Y_AXIS_PIN, INPUT);

  analogReadResolution(12);

  /*
   * const int xAxis[4096] = {};
   * const int yAxis[4096] = {};
   *
   * for (int i = 0; i < 4096; i++) {
   *   xAxis[i] = static_cast<int>((i - 2048) * / 4096);
   *   yAxis[i] = static_cast<int>((i - 2048) * / 4096);
   * }
  */
}

void loop() {
  xAxis = analogRead(X_AXIS_PIN);
  yAxis = analogRead(Y_AXIS_PIN);

  Serial.print((xAxis - 2048) * 100 / 4096);
  Serial.println((yAxis - 2048) * 100 / 4096);
}
