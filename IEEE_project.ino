#include <Servo.h>
#include <Wire.h>

// Servo Setup
Servo servo1;
Servo servo2;
const int SERVO1_PIN = 9;
const int SERVO2_PIN = 10;

// DC Motor Pins (L298N) 
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

// MPU6050 Setup
const int MPU_ADDR = 0x68;
int16_t ax, ay, az;
int16_t prev_ax = 0, prev_ay = 0, prev_az = 0;

unsigned long lastGestureTime = 0;

void setup() {
  Serial.begin(9600);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  readAccelerometer();

  int16_t deltaZ = az - prev_az;
  int16_t deltaX = ax - prev_ax;
  int16_t deltaY = ay - prev_ay;

  unsigned long now = millis();

  // Gesture 1: Fast up/down motion → Spin fans 
  if (abs(deltaZ) > 12000 && (now - lastGestureTime > 1000)) {
    Serial.println("Gesture: Up/Down → Fans");
    spinFans();
    lastGestureTime = now;
  }

  // Gesture 2: Anticlockwise rotation → Open doors 
  else if (deltaX < -6000 && deltaY > 6000 && (now - lastGestureTime > 1000)) {
    Serial.println("Gesture: Anticlockwise → Open Doors");
    openDoors();
    lastGestureTime = now;
  }

  // Gesture 3: Clockwise rotation → Close doors 
  else if (deltaX > 6000 && deltaY < -6000 && (now - lastGestureTime > 1000)) {
    Serial.println("Gesture: Clockwise → Close Doors");
    closeDoors();
    lastGestureTime = now;
  }

  prev_ax = ax;
  prev_ay = ay;
  prev_az = az;

  delay(100);
}

// Functions

void spinFans() {
  servo1.write(0);
  servo2.write(180);
  delay(500);
  servo1.write(90);
  servo2.write(90);
}

void openDoors() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1000);
  stopMotors();
}

void closeDoors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000);
  stopMotors();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void readAccelerometer() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
}
