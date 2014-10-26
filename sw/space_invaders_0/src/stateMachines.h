/*
 * stateMachines.h
 *
 *  Created on: Oct 9, 2014
 *      Author: superman
 */

#ifndef STATEMACHINES_H_
#define STATEMACHINES_H_

#include "xgpio.h"          // Provides access to PB GPIO driver
#include "globals.h"
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
	int bcet; //WCET of the task
} task;


/////////////////////////////////////
// Handy Timing Functions
/////////////////////////////////////
void startTiming();
void stopTiming();

void initStateMachines();

enum SM1_States { SM1_alive, SM1_dead, SM1_gameOver } SM1_State;
int TankMovementAndBullet_SM();

enum SM2_States { SM2_bullet, SM2_gameOver } SM2_State;
int TankBulletUpdate_SM();

enum SM3_States { SM3_alien, SM3_gameOver } SM3_State;
int AlienMovementAndBullets_SM();

enum SM4_States { SM4_bullets, SM4_gameOver } SM4_State;
int AlienbulletsUpdate_SM();

enum SM5_States { SM5_alive, SM5_dead, SM5_gameOver } SM5_State;
int SpaceShipUpdate_SM();

enum SM6_States { SM6_noDeath, SM6_death, SM6_gameOver } SM6_State;
int AlienDeath_SM();


#endif /* STATEMACHINES_H_ */
