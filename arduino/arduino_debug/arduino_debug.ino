#include <Servo.h>
#include <LiquidCrystal.h>

#define MSG_SIZE 8

byte incomingBytes[MSG_SIZE];

/* Definition av variablerna som är relaterade till servo vinklar
 * För varje servo som ska användas måste en "int" deklareras. Här
 * nedan initieras vinklarna till 90 grader och är det läge som kommer
 * sättas då Arduinot startar.
 */
int rot = 90;
int shoulder = 60;
int elbow = 120;
int claw = 90;

/* Definition av servon. En "Servo" variabel måste deklareras per servo. */
Servo servoRot;
Servo servoShoulder;
Servo servoElbow;
Servo servoClaw;

int E1 = 3;
int E2 = 5;
int M1 = 2;
int M2 = 4;

/* Tömmer listan som innehåller meddelandet från datorn */
void clearBytes() {
    for (int i = 0; i < 2; i++) {
      incomingBytes[i] = 0;
    }
    for (int i = 2; i < MSG_SIZE; i++) {
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

/* Anropas i slutet av loopen för att sätta servon i den nya vinkeln */
void servoSet() {
  servoRot.write(rot);
  servoShoulder.write(shoulder);
  servoElbow.write(elbow);
  servoClaw.write(claw); 
}

void checkInRange(int *servo) {
    if (*servo < 0) {
      *servo = 0;
    }
    if (*servo > 180) {
      *servo = 180;  
    }
}

/* Anropas under setup för att tala om för arduinot vilka servon som
 * ska använda och vid vilken pin som de är anslutna.
 *
 * Här måste ni själv ange vilken pin som servot sitter i. 
 * Om ni har servo1 vid pin 3 så lägger ni till raden:
 * "servo1.attach(3)"
 */
void servoAttach() {
        servoRot.attach(9);
        servoRot.write(rot);
        servoShoulder.attach(8);
        servoShoulder.write(shoulder);
        servoElbow.attach(7);
        servoElbow.write(elbow);
        servoClaw.attach(6);
        servoClaw.write(claw);  
}

void motorAttach() {
        pinMode(E1, OUTPUT);
        pinMode(E2, OUTPUT);
        pinMode(M1, OUTPUT);
        pinMode(M2, OUTPUT);
        digitalWrite(E1, LOW);
        digitalWrite(E2, LOW);
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
}

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        servoAttach();
        servoSet();
        motorAttach();
        setupDebug();
}

int i = 0;
void loop() {
        //Clear data from last loop
        clearBytes();

        waitForMsg();
        // Recieve data
        if (Serial.available() >= MSG_SIZE) {
            // read the incoming byte:
            readData();
            //replyData();
        }
        
        /*
        //Call corresponding functions
        if (incomingBytes[4] > 10) {
          analogWrite(E1, incomingBytes[4]);
          if (incomingBytes[6] < 128) {
            digitalWrite(M1, HIGH);
          }
          else {
            digitalWrite(M1, LOW);
          }
        } else {
          digitalWrite(E1, 0);
          digitalWrite(M1, LOW); 
        }
        
        if (incomingBytes[5] > 10) {
          analogWrite(E2, incomingBytes[5]);
          if (incomingBytes[6] < 128) {
            digitalWrite(M2, LOW);
          }
          else {
            digitalWrite(M2, HIGH);
          }
        } else {
          digitalWrite(E2, 0);
          digitalWrite(M2, LOW); 
        }
        
        rot = incomingBytes[0];
        shoulder = incomingBytes[1];
        elbow = incomingBytes[2];
        claw = incomingBytes[3];
        checkInRange(&rot);
        checkInRange(&shoulder);
        checkInRange(&elbow);
        checkInRange(&claw);
        //Write servo angles
        servoSet();
        */
        i = i + 1;
        if (i>3){
          debugDisplay();
          i = 0;
        }
        
        clearBuffer();
}
