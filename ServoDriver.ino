int SERVO_LOGIC_1 = 7;
int SERVO_LOGIC_2 = 8;
int SERVO_PWM = 6;

float SERVO_SPEED = 1; // <0, 1>
float DRIVER_FREQUENCY = 100.0f;
int ANGLE_LIMIT = 10;
int THRESHOLD = 5;

int currentAngle = 90;
int goToAngle = 90;

int AdcToAngle(int x) { 
  int angle = x * 0.176;
  if(angle > 180) angle = 180;
  else if(angle < 0) angle = 0;
  
  return (int)angle;
}

int SpeedToDuty(float servoSpeed) {
  float duty = 250 * servoSpeed;
  return (int)duty;
}

void setup() {
  Serial.begin(9600);

  pinMode(SERVO_LOGIC_1, OUTPUT);
  pinMode(SERVO_LOGIC_2, OUTPUT);
  pinMode(SERVO_PWM, OUTPUT);
  digitalWrite(SERVO_LOGIC_1, LOW);
  digitalWrite(SERVO_LOGIC_2, LOW);
}

void loop() {
  currentAngle = AdcToAngle(analogRead(A0));
  
  if(Serial.available() > 0) {
    String rotateTo = Serial.readStringUntil('\n');
    goToAngle = rotateTo.toInt();
    if(goToAngle > 180 - ANGLE_LIMIT) goToAngle = 180 - ANGLE_LIMIT;
    else if(goToAngle < 0 + ANGLE_LIMIT) goToAngle = 0 + ANGLE_LIMIT;
  }
  
  if(currentAngle >= goToAngle - THRESHOLD && currentAngle <= goToAngle + THRESHOLD) {
    digitalWrite(SERVO_LOGIC_1, LOW);
    digitalWrite(SERVO_LOGIC_2, LOW);
    analogWrite(SERVO_PWM, SpeedToDuty(0));
  }
  else if(currentAngle < goToAngle - THRESHOLD) {
    digitalWrite(SERVO_LOGIC_1, HIGH);
    digitalWrite(SERVO_LOGIC_2, LOW);
    analogWrite(SERVO_PWM, SpeedToDuty(SERVO_SPEED));
  }
  else if(currentAngle > goToAngle + THRESHOLD) {
    digitalWrite(SERVO_LOGIC_1, LOW);
    digitalWrite(SERVO_LOGIC_2, HIGH);
    analogWrite(SERVO_PWM, SpeedToDuty(SERVO_SPEED));
  }

  Serial.println(goToAngle);
  Serial.println(currentAngle);
  Serial.println();
  delay(1000/DRIVER_FREQUENCY);
}
