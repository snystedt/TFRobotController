#include <Servo.h>

#define MSG_SIZE 8
#define AXIS_THRESH 32

byte incomingBytes[MSG_SIZE];

/* Definition av variablerna som är relaterade till servo vinklar
 * För varje servo som ska användas måste en "int" deklareras. Här
 * nedan initieras vinklarna till 90 grader och är det läge som kommer
 * sättas då Arduinot startar.
 */
int one = 90;
int two = 60;
int three = 120;
int four = 90;

/* Definition av servon. En "Servo" variabel måste deklareras per servo. */
Servo servoOne
Servo servoTwo;
Servo servoThree;
Servo servoFour;

/* Anropas under setup för att tala om för arduinot vilka servon som
 * ska använda och vid vilken pin som de är anslutna.
 *
 * Här måste ni själv ange vilken pin som servot sitter i. 
 * Om ni har servo1 vid pin 3 så lägger ni till raden:
 * "servo1.attach(3)"
 */
void servoAttach() {
        servoOne.attach(10);
        servoOne.write(rot);
        servoTwo.attach(7);
        servoTwo.write(shoulder);
        servoThree.attach(6);
        servoThree.write(elbow);
        servoFour.attach(5);
        servoFour.write(claw);  
}

/* Anropas i slutet av loopen för att sätta servon i den nya vinkeln */
void servoSet() {
  servoOne.write(rot);
  servoTwo.write(shoulder);
  servoThree.write(elbow);
  servoFour.write(claw); 
}

/* Anropas för att internt sätta en ny vinkel på ett servo. Första 
 * argumentet är servo vinkeln som ska ändras. Den skickas med ett
 * &-tecken framför. Andra argumentet är vinkeln som servot ska
 * rotera. För att rotera t ex "shoulder" 2 grader så anropar man
 * rotateServo(&shoulder, 2);
*/
void rotateServo(int *servo, int angle) {
    *servo = *servo + angle;
    
    if (*servo < 0) {
      *servo = 0;
    }
    if (*servo > 180) {
      *servo = 180;  
    }
    
}

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

/* Här sker anropningen av era egna funktioner utifrån
 * meddelandet från datorn.
 */
void processData() {
  int axisRot = 1;
  
  if (incomingBytes[0] & 1) {
      // A 
  } if (incomingBytes[0] & 2) {
      // B
  } if (incomingBytes[0] & 4) {
      // X
  } if (incomingBytes[0] & 8) {
      // Y
  } if (incomingBytes[0] & 16) {
      // R1
  } if (incomingBytes[0] & 32) {
      // R3
  } if (incomingBytes[0] & 64) {
      // L1    
  } if (incomingBytes[0] & 128) {
      // UNMAPPED
  } if (incomingBytes[1] & 1) {
      // DPAD UP
  } if (incomingBytes[1] & 2) {
      // DPAD DOWN
  } if (incomingBytes[1] & 4) {
      // DPAD LEFT
  } if (incomingBytes[1] & 8) {
      // DPAD RIGHT
  } if (incomingBytes[1] & 16) {
      // START
  } if (incomingBytes[1] & 32) {
      // SELECT
  } if (incomingBytes[1] & 64) {
      // L3
  } if (incomingBytes[1] & 128) {
      //Serial.println("UNMAPPED");
  }
  
  //Axis 1 (L2)
  if (incomingBytes[2] > AXIS_THRESH) {

  }
  
  //Axis 2 (R2)
  if (incomingBytes[3] > AXIS_THRESH) {

  }
  
  //Axis 3 (LY)
  if ((incomingBytes[4] > (128 + AXIS_THRESH)) || (incomingBytes[4] < (128 - AXIS_THRESH))) {

  }
  
  //Axis 4 (LX)
  if ((incomingBytes[5] > (128 + AXIS_THRESH)) || (incomingBytes[5] < (128 - AXIS_THRESH))) {
    
  }
  
  //Axis 5 (RY)
  if ((incomingBytes[6] > (128 + AXIS_THRESH)) || (incomingBytes[6] < (128 - AXIS_THRESH))) {

  }
  
  //Axis 6 (RX)
  if ((incomingBytes[7] > (128 + AXIS_THRESH)) || (incomingBytes[7] < (128 - AXIS_THRESH))) {
    
  }
}

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        servoAttach();
        servoSet();
}

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
        
        //Call corresponding functions
        processData();
        //Write servo angles
        servoSet();
        clearBuffer();
}
