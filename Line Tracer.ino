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

// 센서값 읽기 및 출력 (디버깅용)
void readSensors() {
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

// 모터 제어 함수
void motorControl(int leftSpeed, int rightSpeed, bool leftDir, bool rightDir) {
  motorLeft.setSpeed(leftSpeed);
  motorRight.setSpeed(rightSpeed);
  motorLeft.run(leftDir ? FORWARD : BACKWARD);
  motorRight.run(rightDir ? FORWARD : BACKWARD);
}

void loop() {
  readSensors();

  // 중앙 센서(A3)가 라인(노란색) 감지 시 직진
  if(sensorValues[2] == 1) {
    motorControl(200, 200, true, true);
  }
  // 좌측 센서(A2) 감지 시 좌회전 (왼쪽 빠르게, 오른쪽 느리게)
  else if(sensorValues[1] == 1) {
    motorControl(200, 100, true, true);
  }
  // 우측 센서(A4) 감지 시 우회전 (왼쪽 느리게, 오른쪽 빠르게)
  else if(sensorValues[3] == 1) {
    motorControl(100, 200, true, true);
  }
  // 극좌(A1) 감지 시 급좌회전 (오른쪽만 전진)
  else if(sensorValues[0] == 1) {
    motorControl(200, 0, true, true);
  }
  // 극우(A5) 감지 시 급우회전 (왼쪽만 전진)
  else if(sensorValues[4] == 1) {
    motorControl(0, 200, true, true);
  }
  // 모두 미감지 시 정지
  else {
    motorControl(0, 0, true, true);
  }
  delay(10);
}
