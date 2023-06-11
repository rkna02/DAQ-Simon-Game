/*
 * Author: Peter Na
 * Student Number: 36734671
 * Lab Section: L1B
 * Date: December 2nd, 2020
 * UBC Email: rkna02@student.ubc.ca
 * Purpose: Write a program in C that implements a modified version of the Simon game
 *          using the simulator.
 */

#define _CRT_SECURE_NO_WARNINGS

/* Header files */
#include<stdio.h>
#include<stdlib.h>
#include<DAQlib.h>
#include<Windows.h>
#include<time.h>

/* LEDs */
#define GREEN 0
#define RED 1
#define YELLOW 2
#define BLUE 3

/* Pushbuttons */
#define BUTTON0 0
#define BUTTON1 1 
#define BUTTON2 2
#define BUTTON3 3

/* Size of maximum sequence */
#define SEQUENCE 5

/* Switch */
#define ON 1
#define OFF 0

/* Boolean values */
#define TRUE 1
#define FALSE 0

/* Time intervals */
#define ONE_SECOND 1000
#define HALF_SECOND 500

/* Simulator set up number*/
#define SIMULATOR 6

/* Function prototypes */
int randInt(int, int);
void generateSequence(int, int[]);
void runSimon(void);
void flashGreen(void);
void flashRed(void);
int readButton(void);
void displayLED(int, int[]);

/* Main function */
int main(void)
{
	/* Initializes the DAQ simulator */
	if (setupDAQ(SIMULATOR) == TRUE) {
		runSimon();
	}
	else {
		printf("Error, DAQ not initialized sucessfully\n");
	}
	system("PAUSE");
	return 0;
}


/* Runs the modified simon game for the user until user quits */
void runSimon(void)
{
	while (continueSuperLoop() == TRUE)
	{
		int index;
		int rounds;
		int win = TRUE;
		int leds[SEQUENCE];

		/* User has 2 seconds to prepare */
		generateSequence(SEQUENCE, leds);
		printf("The game will start in two seconds, be prepared!\n");
		Sleep(2 * ONE_SECOND);

		/* Start of the game */
		for (rounds = 1; rounds <= SEQUENCE; rounds++)
		{
			/* Flashes the corresponding LEDs */
			for (index = 0; index < rounds; index++) {
				displayLED(index, leds);
			}

			/* Checks user's guess */
			for (index = 0; index < rounds; index++) {
				int result = readButton();
				if (leds[index] == result) {
					Sleep(150);
					digitalWrite(leds[index], ON);
					Sleep(150);
					digitalWrite(leds[index], OFF);
					Sleep(HALF_SECOND);
				}
				else if (leds[index] != result) {
					flashRed();
					win = FALSE;
					break;
				}
			}

			/* Decide if user may proceed to next round */
			if (rounds == SEQUENCE) {
				break;
			}
			else if (rounds < SEQUENCE && win == FALSE) {
				break;
			}
			else {
				flashGreen();
			}

		}
	}
}


/* Generates a random integer within range from a given maximum value and a minimum value */
int randInt(int lower, int upper)
{
	int number = rand() % (upper - lower + 1) + lower;
	return number;
}


/* Generates a sequence of random integers based on the given length of sequence */
void generateSequence(int length, int data[])
{
	srand((unsigned)time(NULL)); /* seed for the random number generator */
	for (int i = 0; i < length; i++)
	{
		data[i] = randInt(0, 3);
	}
}


/* Flash the green LED 3 times after successfully repeating a sequence */
void flashGreen(void) {
	for (int i = 1; i <= 3; i++) {
		Sleep(HALF_SECOND);
		digitalWrite(GREEN, ON);
		Sleep(HALF_SECOND);
		digitalWrite(GREEN, OFF);
	}
}


/* Flash the red LED 3 times after unsuccessfully repeating a sequence */
void flashRed(void) {
	for (int i = 1; i <= 3; i++) {
		Sleep(HALF_SECOND);
		digitalWrite(RED, ON);
		Sleep(HALF_SECOND);
		digitalWrite(RED, OFF);
	}
}


/* Reads the user's input button */
int readButton(void) {
	int input;
	while (TRUE)
	{
		if (digitalRead(BUTTON0) == TRUE) {
			return BUTTON0;
		}
		else if (digitalRead(BUTTON1) == TRUE) {
			return BUTTON1;
		}
		else if (digitalRead(BUTTON2) == TRUE) {
			return BUTTON2;
		}
		else if (digitalRead(BUTTON3) == TRUE) {
			return BUTTON3;
		}
	}
}


/* Displays the randomly generated sequence */
void displayLED(int index, int leds[]) {
	Sleep(ONE_SECOND);
	digitalWrite(leds[index], ON);
	Sleep(ONE_SECOND);
	digitalWrite(leds[index], OFF);
}