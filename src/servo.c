#include "servo.h"

//Initialize global servo structure and intitialize the angles
//MAKE SURE THESE ANGLES MATCH THE ONES IN THE ARDUINO CODE
servo_struct servo = {156, 132, 0, 165, 74, 65};

double x = 5;
double y = 15;
bool button_set = false;
bool tower_set = false;
int toggle_count = 0;

const unsigned char *getServoAngles() {
	return servoAngles;
}

/**
	state2angles takes the 'state' byte array and calculates servo angles, stored
	in 'servoAngles', which can then be transmitted over the serial port.

	@param state a byte array containing the state of the controller
	@param servoAngles a byte array containing the desired angles of the servos.
*/
void state2angles(unsigned char *state) {
	int axisRot = 1;
	int yspeed = 1;
	bool reverse = false;
	bool turbo = false;
	static bool stall = false;

	if (toggle_count > 0) {
		toggle_count--;
	}

	// Decide what to do depending on the buttons pressed or axis values
	if (button_is_pressed(state, A)) {
		// A
		if (!button_is_pressed(state, START))
			rotateServo(&servo.claw, 3);
		else
			leaveBlock1(servoAngles);
	} if (button_is_pressed(state, B)) {
		// B
		if (!button_is_pressed(state, START))
			reverse = true;
		else
			leaveBlock2(servoAngles);
	} if (button_is_pressed(state, X)) {
		// X
		if (!button_is_pressed(state, START))
			rotateServo(&servo.claw, -3);
		else
			leaveBlock3(servoAngles);
	}
	if (button_is_pressed(state, Y)) {
		// Y
		if (button_is_pressed(state, START))
			turbo = true;
	} if (button_is_pressed(state, R1)) {
		// R1
		rotateServo(&servo.rot, -1.5);
		if (button_is_pressed(state, SELECT))
			rotateServo(&servo.rot, -3);
	} if (button_is_pressed(state, R3)) {
		// R3
		if (toggle_count == 0) {
			stall = stall ? false : true;
			toggle_count = 10;
		}
	} if (button_is_pressed(state, L1)) {
		// L1
		rotateServo(&servo.rot, 1.5);
		if (button_is_pressed(state, SELECT))
			rotateServo(&servo.rot, 3);
	} if (button_is_pressed(state, XBOX)) {
		// XBOX
		/*if (toggle_count == 0) {
			if (servoAngles[8] < 128)
				servoAngles[8] = 255;
			else
				servoAngles[8] = 0;
			toggle_count = 3;
		}*/
		resetTower();
		fprintf(stderr, "Reset tower!");
		usleep(500000);
	} if (button_is_pressed(state, UP)) {
		// DPAD UP
	} if (button_is_pressed(state, DOWN)) {
		// DPAD DOWN
	} if (button_is_pressed(state, LEFT)) {
		// DPAD LEFT
	} if (button_is_pressed(state, RIGHT)) {
		// DPAD RIGHT
	} if (button_is_pressed(state, START)) {
		// START
	} if (button_is_pressed(state, SELECT)) {
		// SELECT
	} if (button_is_pressed(state, L3)) {
		// L3
	}

	// Axis 1
	if (get_axis_thresh(state, L2, 20) || get_axis_thresh(state, R2, 20)) {
		motorController(&servoAngles[4], &servoAngles[5],
										get_axis(state, L2), get_axis(state, R2),
										reverse, turbo);
	} else {
		servoAngles[4] = 0;
		servoAngles[5] = 0;
	}

	if (stall) {
		stallEngine(&servoAngles[4], &servoAngles[5]);
	}

	// Axis 3
	if (!get_axis_interval(state, LY, 128-AXIS_THRESH, 128+AXIS_THRESH)) {
		rotateServo(&servo.shoulder, (get_axis(state, LY)-128)/70);
		x = x - (get_axis(state, LY)-128)/256.0;
		//fprintf(stderr, "Axis 3 perturbed!\n");
	}

	// Axis 4
	if (!get_axis_interval(state, LX, 128-AXIS_THRESH, 128+AXIS_THRESH)) {
		//fprintf(stderr, "Axis 4 perturbed!\n");
	}

	// Axis 5
	if (!get_axis_interval(state, RY, 128-AXIS_THRESH, 128+AXIS_THRESH)) {
		rotateServo(&servo.elbow, axisRot*(get_axis(state, RY)-128)/70);
		y = y - yspeed*(get_axis(state, RY)-128)/256.0;
		//fprintf(stderr, "Axis 5 perturbed!\n");
	}

	// Axis 6
	if (!get_axis_interval(state, RX, 128-AXIS_THRESH, 128+AXIS_THRESH)) {
		//fprintf(stderr, "Axis 6 perturbed!\n");
	}

	// Axis 7
	if (!get_axis_interval(state, DPAD_Y, 128-AXIS_THRESH, 128+AXIS_THRESH)) {
		rotateServo(&servo.wrist_tilt, (get_axis(state, DPAD_Y)-128)/36);
		//fprintf(stderr, "Axis 7 perturbed!\n");
	}

	// Axis 8
	if (!get_axis_interval(state, DPAD_X, 128-AXIS_THRESH, 128+AXIS_THRESH)) {
		rotateServo(&servo.wrist_rot, (get_axis(state, DPAD_X)-128)/36);
		//fprintf(stderr, "Axis 8 perturbed!\n");
	}

	getServoFromAngles(true);
	setServoState(servoAngles);
}

void rotateServo(double *servo, double angle) {
    *servo = *servo + angle;

    if (*servo < 0) {
      *servo = 0;
    }
    if (*servo > 180) {
      *servo = 180;
    }
}

void setServoState(unsigned char *servoAngles) {
	servoAngles[0] = (int)(servo.rot+0.5);
	servoAngles[1] = (int)(servo.shoulder+0.5);
	servoAngles[2] = (int)(servo.elbow+0.5);
	servoAngles[3] = (int)(servo.claw+0.5);
	servoAngles[6] = (int)(servo.wrist_rot+0.5);
	servoAngles[7] = (int)(servo.wrist_tilt+0.5);
}

void getServoFromAngles(bool wrist) {
	if (x < 0) x = 0;
	if (x > (MAT_WIDTH - 1)) x = MAT_WIDTH - 1;
	if (y < 0) y = 0;
	if (y > (MAT_HEIGHT - 1)) y = MAT_HEIGHT - 1;

	int realTheta = getTheta((int)(x+0.5), (int)(y+0.5));
	int realPhi = getPhi((int)(x+0.5), (int)(y+0.5));

	servo.shoulder = (realTheta - 90)*63/90 + 93;
	servo.elbow = (realPhi - 90)*69/90 + 39;
	if (wrist)
		servo.wrist_tilt = -(- 90 + realPhi + realTheta)*108/90 + 159;

	if (servo.shoulder < 0) servo.shoulder = 0;
	if (servo.shoulder > 180) servo.shoulder = 180;
	if (servo.elbow < 0) servo.elbow = 0;
	if (servo.elbow > 180) servo.elbow = 180;
	if (servo.wrist_tilt < 0) servo.wrist_tilt = 0;
	if (servo.wrist_tilt > 180) servo.wrist_tilt = 180;
}

void incrementAngle(double *servo, int angle, unsigned char *servoAngles) {
	int begin = (int)*servo;
	int final = angle;
	int sign;
	int step = 4;

	if (final > begin) sign = 1;
	else if (final < begin) sign = -1;
	else if (final == begin) return;

	for (int i = begin; i != final; ) {
		if (abs(final-i) < step) i = final;
		else i = i + sign*step;
		*servo = i;
		setServoState(servoAngles);
		write_state(servoAngles, SERVO_NO, 35000);
	}

	return;
}

void incrementXY(double *coordinate, int goal, unsigned char *servoAngles, bool wrist) {
	int begin = (int)*coordinate;
	int final = goal;
	int sign;

	if (final > begin) sign = 1;
	else if (final < begin) sign = -1;
	else if (final == begin) return;

	for (int i = begin; i != final; ) {
		i = i + 1*sign;
		*coordinate = i;
		getServoFromAngles(wrist);
		setServoState(servoAngles);
		write_state(servoAngles, SERVO_NO, 35000);
	}

	return;
}

void motorController(	unsigned char *motorL,
						unsigned char *motorR,
						unsigned char triggerL,
						unsigned char triggerR,
						bool reverse,
						bool turbo) {
	const int thresh = 90;
	int delta = triggerR - triggerL;
	int trueMotorL, trueMotorR;

	// Turn right
	if (delta > thresh && triggerL < 20) {
		trueMotorL = 0.8*delta * (turbo ? 1 : 0.75);
		trueMotorR = -delta * (turbo ? 1 : 0.75);
	} else if (delta < -thresh && triggerR < 20) {
		trueMotorL = delta * (turbo ? 1 : 0.75);
		trueMotorR = -0.8*delta * (turbo ? 1 : 0.75);
	} else {
		if (triggerL > triggerR) {
			trueMotorL = triggerL;
			trueMotorR = triggerL;
		} else {
			trueMotorL = triggerR;
			trueMotorR = triggerR;
		}
		if (reverse) {
			trueMotorL = -trueMotorL;
			trueMotorR = -trueMotorR;
		}
		trueMotorL = (turbo ? 1 : 1) * (reverse ? -1 : 1) * trueMotorL*trueMotorL/255;
		trueMotorR = (turbo ? 1 : 1) * (reverse ? -1 : 1) * trueMotorR*trueMotorR/255;
	}

	if (trueMotorL < 0) {
		*motorL = 128 - trueMotorL/2;
	} else {
		*motorL = trueMotorL/2;
	}

	if (trueMotorR < 0) {
		*motorR = 128 - trueMotorR/2;
	} else {
		*motorR = trueMotorR/2;
	}
}

void stallEngine(unsigned char *motorL, unsigned char *motorR) {
	int trueMotorL = 80;
	int trueMotorR = 80;

	if (trueMotorL < 0) {
		*motorL = 128 - trueMotorL/2;
	} else {
		*motorL = trueMotorL/2;
	}

	if (trueMotorR < 0) {
		*motorR = 128 - trueMotorR/2;
	} else {
		*motorR = trueMotorR/2;
	}
}

void setPosition(unsigned char *servoAngles,
								int rot,
								int new_x,
								int new_y,
								int claw,
								int wrist_rot,
								int wrist_tilt,
								int delay)
{
	if (rot >= 0 && rot <= 180) incrementAngle(&servo.rot, rot, servoAngles);
	if (new_x >= 0 && new_x <= MAT_WIDTH-1) incrementXY(&x, new_x, servoAngles, false);
	if (new_y >= 0 && new_y <= MAT_HEIGHT-1) incrementXY(&y, new_y, servoAngles, false);
	if (claw >= 0 && claw <= 180) servo.claw = claw;
	if (wrist_rot >= 0 && wrist_rot <= 180) servo.wrist_rot = wrist_rot;
	if (wrist_tilt >= 0 && wrist_tilt <= 180) servo.wrist_tilt = wrist_tilt;
	setServoState(servoAngles);
	write_state(servoAngles, SERVO_NO, delay);
}

void resetPosition(unsigned char* servoAngles) {
	x = 5;
	y = 15;
	servo.rot = 156;
	servo.shoulder = 132;
	servo.elbow = 0;
	servo.claw = 165;
	servo.wrist_rot = 74;
	servo.wrist_tilt = 65;

}

void leaveBlock1(unsigned char* servoAngles) {
	/*
	// Lift up
	setPosition(servoAngles, -1, 114, 32, 90, 65, 67, 500000);
	// Rotate
	setPosition(servoAngles, 47, -1, -1, -1, -1, -1, 1000000);
	// Straighten out
	setPosition(servoAngles, -1, 90, 56, -1, -1, -1, 500000);
	// Rotate the last bit
	setPosition(servoAngles, 26, -1, -1, -1, -1, -1, 500000);
	// Drop it
	setPosition(servoAngles, -1, -1, -1, 180, -1, -1, 500000);
	// Rotate back
	setPosition(servoAngles, 158, -1, -1, -1, -1, -1, 1000000);
	*/
	// Close claw
	setPosition(servoAngles, -1, -1, -1, 90, -1, -1, 100000);
	// Lift up
	setPosition(servoAngles, -1, -1, 32, -1, -1, 25, 100000);
	// Lift up
	setPosition(servoAngles, -1, 0, -1, 90, 74, 16, 100000);
	// Rotate
	setPosition(servoAngles, 26, -1, -1, -1, -1, -1, 100000);
	// Straighten out
	setPosition(servoAngles, -1, 19, 30, -1, -1, 43, 100000);
	// Go down
	setPosition(servoAngles, -1, -1, 25, -1, -1, 55, 100000);
	// Drop it
	setPosition(servoAngles, -1, -1, -1, 180, -1, -1, 100000);
	// Go up
	setPosition(servoAngles, -1, -1, 30, -1, -1, -1, 100000);
	// Brace
	setPosition(servoAngles, -1, 0, 32, -1, 74, 16, 100000);
	// Rotate back
	setPosition(servoAngles, 156, 10, 25, -1, -1, -1, 100000);
	// Go down
	setPosition(servoAngles, 156, -1, -1, 165, 74, 65, 100000);
	setPosition(servoAngles, -1, -1, 15, -1, -1, -1, 100000);
	setPosition(servoAngles, -1, 5, -1, -1, -1,-1, 100000);
}

void leaveBlock2(unsigned char* servoAngles) {
	// Open Claw
	setPosition(servoAngles, -1, -1, -1, 180, -1, -1, 100000);
	// Lift up
	setPosition(servoAngles, -1, -1, 26, 180, 86, 58, 100000);
	// Go in
	setPosition(servoAngles, -1, 2, -1, -1, -1, -1, 100000);
	// Rotate
	setPosition(servoAngles, 29, -1, -1, -1, -1, -1, 100000);
	// Go down
	setPosition(servoAngles, -1, 0, 22, -1, -1, -1, 100000);
	// Grab cube
	setPosition(servoAngles, -1, -1, -1, 90, -1, -1, 500000);
	// Go up
	setPosition(servoAngles, -1, 0, 24, -1, -1, 52, 100000);
	// Go up
	setPosition(servoAngles, -1, 0, 26, -1, -1, 49, 100000);
	// Go up
	setPosition(servoAngles, -1, 0, 28, -1, -1, 46, 100000);
	// Rotate
	setPosition(servoAngles, 156, -1, -1, -1, -1, 123, 100000);
	// Go down
	if (tower_no == 0)
		setPosition(servoAngles, -1, 10, 4, -1, -1, 125, 100000);
	else if (tower_no == 1)
		setPosition(servoAngles, -1, 10, 14, -1, -1, 86, 100000);
	else if (tower_no == 2)
		setPosition(servoAngles, -1, 10, 20, -1, -1, 60, 100000);
	else if (tower_no == 3)
		setPosition(servoAngles, -1, 10, 24, -1, -1, 47, 100000);
	tower_no++;
}

void leaveBlock3(unsigned char* servoAngles) {
	// Lift up
	setPosition(servoAngles, -1, 2, 33, 90, 81, 8, 100000);
	// Rotate
	setPosition(servoAngles, 156, -1, -1, -1, -1, -1, 100000);
	// Go forward
	setPosition(servoAngles, -1, 22, 33, -1, -1, 14, 100000);
	// Go down
	incrementXY(&y, 2, servoAngles, true);
	// Go neutral
	setPosition(servoAngles, -1, -1, 16, 165, -1, 45, 100000);
	setPosition(servoAngles, -1, 3, -1, -1, -1, 55, 100000);
}

void resetTower(void) {
	tower_no = 0;
	tower_height = 0;
}
