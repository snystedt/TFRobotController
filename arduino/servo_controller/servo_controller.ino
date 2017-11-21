#include<Servo.h>
#include<LiquidCrystal.h>

#define MSG_SIZE 8
#define SERVO_SIZE 1

int read = 0;

byte incomingBytes[MSG_SIZE];

int servo_us = 1800;
Servo servo_array[SERVO_SIZE];
int servo_port[] = {9};

/**/
void clearBytes() {
    for (int i = 0; i < 2; i++) {
      incomingBytes[i] = 0;
    }
    for (int i = 2; i < 4; i++) {
      incomingBytes[i] = 1;  
    }
    for (int i = 4; i < MSG_SIZE; i++) {
      incomingBytes[i] = 128;  
    }
}

/* Tömmer seriella bufferten */
void clearBuffer() {
    while (Serial.available() > 0) {
      char t = Serial.read();  
    }
}

/* Anropas för att invänta nästa meddelande från datorn */
void waitForMsg() {
  while (Serial.available() < MSG_SIZE) {
    delay(10);
  }
}

/* Anropas för att läsa in meddelandet från datorn till */
void readData() {
  for (int i = 0; i < MSG_SIZE; i++) {
    incomingBytes[i] = Serial.read();  
  }
}

void servoSet() {
  for (int i = 0; i < SERVO_SIZE; i++) {
    servo_array[i].writeMicroseconds(servo_us);
  }  
}

void checkInRange(int *servo) {
  if (*servo > 2600) {
    *servo = 2600;
  } else if (*servo < 400) {
    *servo = 400;
  }
}

void servoAttach() {
  for (int i = 0; i < SERVO_SIZE; i++) {
    servo_array[i].attach(servo_port[i]);  
  }
}

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setupDebug() {
  lcd.begin(20, 4);
}

void debugDisplay() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("a[0]=");
  lcd.print(incomingBytes[0]);

  lcd.setCursor(10, 0);
  lcd.print("a[1]=");
  lcd.print(incomingBytes[1]);
  
  lcd.setCursor(0, 1);
  lcd.print("a[2]=");
  lcd.print(incomingBytes[2]);
  
  lcd.setCursor(10, 1);
  lcd.print("a[3]=");
  lcd.print(incomingBytes[3]);
  
  lcd.setCursor(0,2);
  lcd.print("servo_us=");
  lcd.print(servo_us);
  
  lcd.setCursor(0,3);
  lcd.print(read ? "Recieved" : "Nothing");
  
  /*
  lcd.setCursor(0, 2);
  lcd.print("a[4]=");
  lcd.print(incomingBytes[4]);
  
  lcd.setCursor(10, 2);
  lcd.print("a[5]=");
  lcd.print(incomingBytes[5]);
  
  lcd.setCursor(0, 3);
  lcd.print("a[6]=");
  lcd.print(incomingBytes[6]);
  
  lcd.setCursor(10, 3);
  lcd.print("a[7]=");
  lcd.print(incomingBytes[7]);
  */
}

void rotateServo() {
  int speed_ = 1;
  if (incomingBytes[0] & (1 << 0)) {
    speed_ = 4;
  }
  /* Right Button */
  if (incomingBytes[0] & (1 << 4)) {
    servo_us += 1; 
  }
  /* Left Button */
  if (incomingBytes[0] & (1 << 6)) {
    servo_us -= 1;
  }
  /* Left Trigger */
  if (incomingBytes[2] > 30) {
    servo_us -= (incomingBytes[2] / 25)*speed_;
  }
  /* Right Trigger */
  if (incomingBytes[3] > 30) {
    servo_us += (incomingBytes[3] / 25)*speed_;
  }
}

void setup() {
    Serial.begin(9600);
    setupDebug();
    debugDisplay();
    servoAttach();
    servoSet();
}

void loop() {
  //Clear data from last loop
  clearBytes();
  read = 0;

  waitForMsg();
  // Recieve data
  if (Serial.available() >= MSG_SIZE) {
      // read the incoming byte:
      readData();
      read = 1;
  }
  
  rotateServo();
  checkInRange(&servo_us);
  debugDisplay();
  servoSet();
  
  delay(50);
  
  clearBuffer();
}
