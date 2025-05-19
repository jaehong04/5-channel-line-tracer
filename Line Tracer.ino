#include <AFMotor.h>

// 모터 설정 (M2: 좌측, M3: 우측)
AF_DCMotor motorLeft(2);
AF_DCMotor motorRight(3);

// 센서 핀 (A1~A5)
const int sensorPins[5] = {A1, A2, A3, A4, A5};
int sensorValues[5];

void setup() {
  Serial.begin(9600);
  motorLeft.setSpeed(200);
  motorRight.setSpeed(200);
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void readSensors() {
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
}

void motorControl(int leftSpeed, int rightSpeed, bool leftDir, bool rightDir) {
  motorLeft.setSpeed(leftSpeed);
  motorRight.setSpeed(rightSpeed);
  motorLeft.run(leftDir ? FORWARD : BACKWARD);
  motorRight.run(rightDir ? FORWARD : BACKWARD);
}

void loop() {
  readSensors();

  // 중앙 센서(A3)가 라인 감지 시 직진
  if(sensorValues[2] == 0) {
    motorControl(200, 200, true, true);
  }
  // 좌측 센서(A2) 감지 시 우회전
  else if(sensorValues[1] == 0) {
    motorControl(200, 100, true, true);
  }
  // 우측 센서(A4) 감지 시 좌회전
  else if(sensorValues[3] == 0) {
    motorControl(100, 200, true, true);
  }
  // 극좌(A1) 감지 시 급우회전
  else if(sensorValues[0] == 0) {
    motorControl(200, 0, true, true);
  }
  // 극우(A5) 감지 시 급좌회전
  else if(sensorValues[4] == 0) {
    motorControl(0, 200, true, true);
  }
  // 모두 미감지 시 정지
  else {
    motorControl(0, 0, true, true);
  }
  delay(10);
}
