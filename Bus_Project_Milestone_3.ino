

// Robotic Bunny - Combined Ear Flapping and Body Movement
// 1. Flaps ears 3x (1s between flaps)
// 2. Moves body 300° CCW → 2s pause → 300° CW

// Ear motor pins
const int earUp = 10;    // IN3
const int earDown = 11;  // IN4
const int earSpeed = 7;  // ENB

// Body motor pins
const int bodyDir1 = 8;  // IN1
const int bodyDir2 = 9;  // IN2
const int bodySpeed = 6; // ENA

// Ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;

// Settings
const int triggerDistance = 10;  // 10cm trigger distance
const int motorRunTime = 10000;   // 8.33s for 300° (3RPM)
bool isActive = false;

void setup() {
  // Initialize pins
  pinMode(earUp, OUTPUT);
  pinMode(earDown, OUTPUT);
  pinMode(earSpeed, OUTPUT);
  pinMode(bodyDir1, OUTPUT);
  pinMode(bodyDir2, OUTPUT);
  pinMode(bodySpeed, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  stopAll();
}

void loop() {
  if(checkDistance() && !isActive && Serial.available()) {
      char cmd = Serial.read();

      if(cmd == 'W' || cmd == 'w') {
        startSequence();
      }
      else if(cmd == 'D' || cmd == 'd') {
        moveBody();
      }
      else if(cmd == 'A' || cmd == 'a') {
        flapEars();
      }
      else if(cmd == 'C' || cmd == 'c') {
        stopBody();
        Serial.println("Body functions stopped");
      }
      else if(cmd == 'Z' || cmd == 'z') {
        stopEars();
        Serial.println("Ear functions stopped");
      }
      else if(cmd == 'X' || cmd == 'x' && checkDistance() > 9) {
        stopAll();
        Serial.println("All functions stopped");
      }   
  } 
  delay(50);
}

void startSequence() {
  isActive = true;
  Serial.println("SEQUENCE STARTED");
  
  // 1. Ear flapping sequence
  flapEars();
  
  // 2. Body movement sequence
  moveBody();
  
  isActive = false;
  Serial.println("SEQUENCE COMPLETE");
}

void flapEars() {
  Serial.println("FLAPPING EARS");
  for(int i = 0; i < 3; i++) {
    if(i > 0) delay(1000); // 1s delay between flaps
    
    // Single flap (up then down)
    digitalWrite(earUp, HIGH); 
    digitalWrite(earDown, LOW);
    analogWrite(earSpeed, 200);
    delay(500);
    
    digitalWrite(earUp, LOW);
    digitalWrite(earDown, HIGH);
    delay(500);
    
    stopEars();
  }
}

void moveBody() {
  Serial.println("STARTING BODY MOVEMENT");
  
  // Counter-clockwise movement
  Serial.println("MOVING CCW (295°)");
  digitalWrite(bodyDir1, LOW);
  digitalWrite(bodyDir2, HIGH);
  analogWrite(bodySpeed, 200);
  delay(motorRunTime);
  stopBody();
  
  // Pause
  Serial.println("2 SECOND PAUSE");
  delay(2000);
  
  // Clockwise movement
  Serial.println("MOVING CW (295°)");
  digitalWrite(bodyDir1, HIGH);
  digitalWrite(bodyDir2, LOW);
  analogWrite(bodySpeed, 200);
  delay(motorRunTime);
  stopBody();
}

// Utility functions
void stopEars() {
  digitalWrite(earUp, LOW);
  digitalWrite(earDown, LOW);
}

void stopBody() {
  digitalWrite(bodyDir1, LOW);
  digitalWrite(bodyDir2, LOW);
}

void stopAll() {
  stopEars();
  stopBody();
}

bool checkDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long distance = pulseIn(echoPin, HIGH) * 0.034 / 2;
  
  if(distance > 0 && distance < triggerDistance) {
    Serial.print("Triggered at: ");
    Serial.print(distance);
    Serial.println("cm");
    return true;
  }
  return false;
}