#포함 <AFMotor.h>

// 센서 핀 (A1~A5)
const int sensorPins[5] = {A1, A2, A3, A4, A5};
int sensorValues[5];

#BASE_SPEED 180 정의
#TURN_SPEED 200 정의
#TURN_Duration 정의 150

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

// 센서값 읽기 및 출력
void readSensors() {
 (int i = 0; i < 5; i++) {에 대해
 sensorValues[i] = 디지털 읽기(sensorPins[i]);
 Serial.print(센서값[i];
 Serial.print(" "";
  }
 Serial.println();
}

// 모터 제어 함수
void moveForward() {
 motor_L.setSpeed(BASE_SPEED);
 motor_R.setSpeed(BASE_SPEED);
 motor_L.run(FORWARD);
 motor_R.run(FORWARD);
}

공허 턴레프트() {
 모터_L.setSpeed (TURN_SPEED);
 모터_R.setSpeed (TURN_SPEED);
 모터_L.run(후진);
 motor_R.run(FORWARD);
 지연(TURN_DURE);
}

void turnRight() {
 모터_L.setSpeed (TURN_SPEED);
 모터_R.setSpeed (TURN_SPEED);
 motor_L.run(FORWARD);
 모터_R.run(후진);
 지연(TURN_DURE);
}

void 샤프레프트() {
 motor_L.setSpeed(0);
 모터_R.setSpeed (TURN_SPEED);
 motor_L.run(릴리스);
 motor_R.run(FORWARD);
 지연(TURN_DURE);
}

void 샤프라이트() {
 모터_L.setSpeed (TURN_SPEED);
 motor_R.setSpeed(0);
 motor_L.run(FORWARD);
 motor_R.run(릴리스);
 지연(TURN_DURE);
}

공허 스톱모터스() {
 motor_L.run(릴리스);
 motor_R.run(릴리스);
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

 지연(50); // 센서 폴링 속도 조절
}
