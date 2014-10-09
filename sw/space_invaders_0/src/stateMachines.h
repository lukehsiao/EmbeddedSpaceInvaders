/*
 * stateMachines.h
 *
 *  Created on: Oct 9, 2014
 *      Author: superman
 */
#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
//#include "time.h"	//is this needed?
#include "unistd.h"
#include "render.h"			// Our rendering file.
#include "xuartlite_l.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "xintc_l.h"        // Provides handy macros
#include "globals.h"

#ifndef STATEMACHINES_H_
#define STATEMACHINES_H_

#define UP 		0x10
#define DOWN 	0x04
#define LEFT	0x08
#define CENTER	0x01
#define RIGHT	0x02

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

typedef struct task {
	int state; // Current state of the task
	unsigned long period; // Rate at which the task should tick
	unsigned long elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
	int wcet; //WCET of the task
} task;

task tasks[4];

enum SM1_States { SM1_tank } SM1_State;
int ADC_SM();

enum SM2_States { SM2_ready, SM2_fire } SM2_State;
int TankBullet_SM();

#endif /* STATEMACHINES_H_ */
