#ifndef SERVO_H
#define SERVO_H

#include <stdbool.h>
#include "matrixReader.h"
#include "robotJoystick.h"
#include "arduinoController.h"

#define SERVO_NO 8 //Number of servos
#define AXIS_THRESH 32 // Threshold for joystick

typedef struct servo_struct {
	double rot;
	double shoulder;
	double elbow;
	double claw;
	double wrist_rot;
	double wrist_tilt;
} servo_struct;

static unsigned char servoAngles[SERVO_NO];
void state2angles(unsigned char *state);
const unsigned char *getServoAngles();

extern servo_struct servo;
extern double x;
extern double y;
extern bool button_set;
extern bool tower_set;

void rotateServo(double *servo, double angle);
void setServoState(unsigned char *servoAngles);
void getServoFromAngles(bool wrist);

// Imported from controllerScripts.h FIX
static int tower_height = 0;
static int tower_no = 0;

void incrementAngle(double *servo, int angle, unsigned char *servoAngles);
void incrementXY(double *coordinate, int goal, unsigned char *servoAngles, bool wrist);
void motorController(	unsigned char *motorL,
						unsigned char *motorR,
						unsigned char triggerL,
						unsigned char triggerR,
						bool reverse,
						bool turbo);
void stallEngine(unsigned char *motorL, unsigned char *motorR);
void setPosition(	unsigned char *servoAngles,
					int rot,
					int shoulder,
					int elbow,
					int claw,
					int wrist_rot,
					int wrist_tilt,
					int delay);
void resetPosition(unsigned char* servoAngles);
void leaveBlock1(unsigned char* servoAngles);
void leaveBlock2(unsigned char* servoAngles);
void leaveBlock3(unsigned char* servoAngles);
void leaveBlock4(unsigned char* servoAngles);
void resetTower(void);
#endif
