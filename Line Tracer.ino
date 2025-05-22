#include <AFMotor.h>

// --- 센서 핀 (A1~A5) ---
const int sensorPins[5] = {A1, A2, A3, A4, A5};
int sensorValues[5];

// --- 모터 속도 및 회전 관련 상수 ---
#define BASE_SPEED 255 // 기본 속도
#define TURN_SPEED 160 // 회전 속도
#define CENTER_SENSOR_IDX 2 // 중앙 센서 인덱스 (A3)
#define CENTER_THRESHOLD 300 // 센서 임계값 (환경에 맞게 조정)
#define MAX_TURN_TIME 500 // 회전 최대 시간(ms)

AF_DCMotor motor_L(2); // 왼쪽 모터 (M2)
AF_DCMotor motor_R(3); // 오른쪽 모터 (M3)

void setup() {
Serial.begin(9600);
for (int i = 0; i < 5; i++) {
pinMode(sensorPins[i], INPUT);
}
motor_L.setSpeed(BASE_SPEED);
motor_R.setSpeed(BASE_SPEED);
motor_L.run(RELEASE);
motor_R.run(RELEASE);
delay(500);
}

// --- 센서값 읽기 및 출력 (아날로그 값 기반, 임계값 적용) ---
void readSensors() {
for (int i = 0; i < 5; i++) {
int analogValue = analogRead(sensorPins[i]);
sensorValues[i] = (analogValue > CENTER_THRESHOLD) ? 1 : 0; // 임계값 초과 시 1, 아니면 0
Serial.print(sensorValues[i]);
Serial.print("(");
Serial.print(analogValue);
Serial.print(") ");
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
void turnLeft() {
motor_L.setSpeed(TURN_SPEED);
motor_R.setSpeed(TURN_SPEED);
motor_L.run(BACKWARD);
motor_R.run(FORWARD);

unsigned long startTime = millis();
while (millis() - startTime < MAX_TURN_TIME) {
int centerSensor = analogRead(sensorPins[CENTER_SENSOR_IDX]);
if (centerSensor > CENTER_THRESHOLD) break; // 중앙 센서가 라인 감지 시 종료
delay(5);
}
motor_L.run(RELEASE);
motor_R.run(RELEASE);
}

// --- 우회전: 왼쪽 모터 전진, 오른쪽 모터 후진 (중앙 센서가 라인 감지 시 즉시 정지) ---
void turnRight() {
motor_L.setSpeed(TURN_SPEED);
motor_R.setSpeed(TURN_SPEED);
motor_L.run(FORWARD);
motor_R.run(BACKWARD);

unsigned long startTime = millis();
while (millis() - startTime < MAX_TURN_TIME) {
int centerSensor = analogRead(sensorPins[CENTER_SENSOR_IDX]);
if (centerSensor > CENTER_THRESHOLD) break;
delay(5);
}
motor_L.run(RELEASE);
motor_R.run(RELEASE);
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

void loop() {
    readSensors();

    // 센서별 가중치
    int weights[5] = {-2, -1, 0, 1, 2};
    int sum = 0;
    int count = 0;

    for(int i = 0; i < 5; i++) {
        if(sensorValues[i] == 1) {
            sum += weights[i];
            count++;
        }
    }

    if(count == 0) {
        // 모두 미감지: 정지
        stopMotors();
    } else {
        float avg = (float)sum / count;
        if(avg < -1.5) {
            // 극좌 방향 (급좌회전)
            sharpLeft();
        } else if(avg < -0.5) {
            // 좌 방향 (좌회전)
            turnLeft();
        } else if(avg > 1.5) {
            // 극우 방향 (급우회전)
            sharpRight();
        } else if(avg > 0.5) {
            // 우 방향 (우회전)
            turnRight();
        } else {
            // 중앙 (직진)
            moveForward();
        }
    }
    delay(20);
}
