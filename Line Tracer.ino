#포함 <AFMotor.h>

// --- 센서 핀 (A1~A5) ---
const int sensorPins[5] = {A1, A2, A3, A4, A5};
int sensorValues[5];

// --- 모터 속도 및 회전 관련 상수 ---
#BASE_SPEED 180 // 기본 속도 정의
#TURN_SPEED 190 // 회전 속도 정의
#CENTER_SENSOR 정의_IDX 2 // 중앙 센서 인덱스 (A3)
#중심 정의_THRESHOLD 500 // 센서 임계값 (환경에 맞게 조정)
#MAX_TURN_TIME 500 // 회전 최대 시간(ms) 정의

AF_DCMotor motor_L(2); // 왼쪽 모터 (M2)
AF_DCMotor motor_R(3); // 오른쪽 모터 (M3)

() 설정 무효화
 직렬.시작(9600);
 (int i = 0; i < 5; i++) {에 대해
 핀모드(센서 핀[i], 입력);
  }
 motor_L.setSpeed(BASE_SPEED);
 motor_R.setSpeed(BASE_SPEED);
 motor_L.run(릴리스);
 motor_R.run(릴리스);
 지연(500);
}

// --- 센서값 읽기 및 출력 (아날로그 값 기반, 임계값 적용) ---
void readSensors() {
 (int i = 0; i < 5; i++) {에 대해
 int analogValue = analogRead(센서핀[i]);
 sensorValues[i] = (아날로그 값 > CENTER)_THRESHOLD) ? 1 : 0; // 임계값 초과 시 1, 아니면 0
 Serial.print(센서값[i];
 Serial.print("(")";
 Serial.print(아날로그 값);
 Serial.print(") ";
  }
 Serial.println();
}

// --- 전진: 두 모터 모두 앞으로 ---
void moveForward() {
 motor_L.setSpeed(BASE_SPEED);
 motor_R.setSpeed(BASE_SPEED);
 motor_L.run(FORWARD);
 motor_R.run(FORWARD);
}

// --- 좌회전: 왼쪽 모터 후진, 오른쪽 모터 전진 (중앙 센서가 라인 감지 시 즉시 정지) ---
공허 턴레프트() {
 모터_L.setSpeed (TURN_SPEED);
 모터_R.setSpeed (TURN_SPEED);
 모터_L.run(후진);
 motor_R.run(FORWARD);

 부호 없는 긴 시작 시간 = 밀리 ();
 (밀리스() - 시작 시간 < MAX_TURN_TIME) { 동안
 int centralSensor = analogRead(센서핀[CENTER_SENSOR_IDX]);
 만약 (센터 센서 > 센터)_THRESHOLD) break; // 중앙 센서가 라인 감지 시 종료
 지연 (5);
  }
 motor_L.run(릴리스);
 motor_R.run(릴리스);
}

// --- 우회전: 왼쪽 모터 전진, 오른쪽 모터 후진 (중앙 센서가 라인 감지 시 즉시 정지) ---
void turnRight() {
 모터_L.setSpeed (TURN_SPEED);
 모터_R.setSpeed (TURN_SPEED);
 motor_L.run(FORWARD);
 모터_R.run(후진);

 부호 없는 긴 시작 시간 = 밀리 ();
 (밀리스() - 시작 시간 < MAX_TURN_TIME) { 동안
 int centralSensor = analogRead(센서핀[CENTER_SENSOR_IDX]);
 만약 (센터 센서 > 센터)_임계값) 휴식;
 지연 (5);
  }
 motor_L.run(릴리스);
 motor_R.run(릴리스);
}

// --- 급좌회전: 왼쪽 모터 정지, 오른쪽 모터 전진 (중앙 센서가 라인 감지 시 즉시 정지) ---
void sharpLeft() {
  motor_L.setSpeed(0);
  motor_R.setSpeed(TURN_SPEED);
  motor_L.run(RELEASE);
  motor_R.run(FORWARD);

  unsigned long startTime = millis();
  while (millis() - startTime < MAX_TURN_TIME) {
    int centerSensor = analogRead(sensorPins[CENTER_SENSOR_IDX]);
    if (centerSensor > CENTER_THRESHOLD) break;
    delay(5);
  }
  motor_L.run(RELEASE);
  motor_R.run(RELEASE);
}

// --- 급우회전: 왼쪽 모터 전진, 오른쪽 모터 정지 (중앙 센서가 라인 감지 시 즉시 정지) ---
void sharpRight() {
  motor_L.setSpeed(TURN_SPEED);
  motor_R.setSpeed(0);
  motor_L.run(FORWARD);
  motor_R.run(RELEASE);

  unsigned long startTime = millis();
  while (millis() - startTime < MAX_TURN_TIME) {
    int centerSensor = analogRead(sensorPins[CENTER_SENSOR_IDX]);
    if (centerSensor > CENTER_THRESHOLD) break;
    delay(5);
  }
  motor_L.run(RELEASE);
  motor_R.run(RELEASE);
}

// --- 정지: 두 모터 모두 정지 ---
void stopMotors() {
  motor_L.run(RELEASE);
  motor_R.run(RELEASE);
}

보이드 루프 () {
 읽기 센서();

  // 센서 배열: [A1, A2, A3, A4, A5]
  // A3(2): 중앙, A2(1): 좌, A4(3): 우, A1(0): 극좌, A5(4): 극우

 if(sensorValues[2] == 1) { // 중앙 센서 감지: 직진
 앞으로 이동();
  }
 그렇지 않으면 if(sensorValues[1] == 1) { // 좌측 센서 감지: 좌회전
 좌회전();
  }
 그렇지 않으면 if(sensorValues[3] == 1) { // 우측 센서 감지: 우회전
 우회전();
  }
 그렇지 않으면 if(sensorValues[0] == 1) { // 극좌 센서 감지: 급좌회전
 샤프 레프트();
  }
 그렇지 않으면 if(sensorValues[4] == 1) { // 극우 센서 감지: 급우회전
 샤프라이트();
  }
  또 다른 {                                 // 모두 미감지: 정지
 스톱모터스();
  }

 지연(30); // 센서 폴링 속도 조절
}
