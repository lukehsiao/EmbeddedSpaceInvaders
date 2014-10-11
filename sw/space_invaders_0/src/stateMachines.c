/*
 * stateMachines.c
 *
 *  Created on: Oct 9, 2014
 *      Author: superman
 */
#include "stateMachines.h"
#include <stdio.h>
#include <stdlib.h>
#include "render.h"			// Our rendering file.
#include "xgpio.h"          // Provides access to PB GPIO driver.

void initStateMachines(){
	int taski;
 	taski=0;
 	// Tank Movement
 	tasks[taski].state = -1;
 	tasks[taski].period = 4;
 	tasks[taski].elapsedTime = 4;
 	tasks[taski].TickFct = &TankMovementAndBullet_SM;
 	++taski;
 	// Tank Bullet Movement
 	tasks[taski].state = -1;
 	tasks[taski].period = 5;
 	tasks[taski].elapsedTime = 5;
 	tasks[taski].TickFct = &TankBulletUpdate_SM;
 	++taski;

 	// Aliens Bullets Movement
 	tasks[taski].state = -1;
 	tasks[taski].period = 5;
 	tasks[taski].elapsedTime = 1;
 	tasks[taski].TickFct = &AlienbulletsUpdate_SM;
 	++taski;

 	// Aliens Movement
 	tasks[taski].state = -1;
 	tasks[taski].period = 63;
 	tasks[taski].elapsedTime = 0;
 	tasks[taski].TickFct = &AlienMovementAndBullets_SM;
 	++taski;



 	// Aliens Bullets Movement
 	tasks[taski].state = -1;
 	tasks[taski].period = 5;
 	tasks[taski].elapsedTime = 5;
 	tasks[taski].TickFct = &AlienbulletsUpdate_SM;
 	++taski;

 	// Spaceship Movement and creation
 	tasks[taski].state = -1;
 	tasks[taski].period = 5;
 	tasks[taski].elapsedTime = 5;
 	tasks[taski].TickFct = &SpaceShipUpdate_SM;
 	++taski;
 	initGlobals();
    blankScreen(); // erase old data
    render();      // draw initialized game
}

int TankMovementAndBullet_SM(int state) {
	static int i;
	static int cycles;
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 rightButton = ((buttons & RIGHT) >> 1) & 0x1;
	u32 leftButton = ((buttons & LEFT) >> 3) & 0x1;
	u32 centerButton = (buttons & CENTER) & 0x1;
	//	xil_printf("\n\rButtons: %x",buttons);
	if(state == -1)
	{
		state = SM1_alive;
	}
	else{
		switch(state) { // Transitions
		case SM1_alive:
			if(getLives() <= 0 || getLives() > 10){
				state = SM1_gameOver;
			}
			else {
				if(centerButton)
					fireTankBullet();

				if (tankLife == 0) { // it the tank is dead TANK DEATH FLAG
					state = SM1_dead;
					cycles = TANK_MAP_FLIP_CYCLES;
					i = 0;
					u8 lives = getLives();
					xil_printf("\n\rlives before: %d", lives);
					lives--;
					if(lives > 10){
						lives = 0;
						// Game Over!!!
					}
					setLives(lives);
					renderLives();
					xil_printf("\n\rlives After: %d\n\r", lives);
				}
				else if(!rightButton && !leftButton){
					state = SM1_alive;
				}
				else if(rightButton) {
					state = SM1_alive;
					moveTankRight();
				}
				else if(leftButton) {
					state = SM1_alive;
					moveTankLeft();
				}
			}
			break;
		case SM1_dead:
			if(getLives() <= 0 || getLives() > 10){
				state = SM1_gameOver;
				blankScreen();
			}
			else {
				if (cycles <= 0) { // it the tank is dead TANK DEATH FLAG
					state = SM1_alive;
					tankLife = 1;
					cycles = TANK_MAP_FLIP_CYCLES;
					i = 0;
					moveTank(TANK_INIT_POSITION_X);
				}
				else if (cycles <= TANK_MAP_FLIP_CYCLES/2) {
					state = SM1_dead;
					deathTank2();
					cycles--;
				}
				else if(i < TANK_MAP_FLIP_COUNT/2) {
					state = SM1_dead;
					deathTank1();
					i++;
				}
				else if(i < TANK_MAP_FLIP_COUNT) {
					state = SM1_dead;
					deathTank2();
					i++;
				}
				else if(i >= TANK_MAP_FLIP_COUNT) {
					state = SM1_dead;
					cycles--;
					i = 0;
					//				xil_printf("\n\rCYCLE!!!");
				}
			}
			break;
		case SM1_gameOver:
			if(rightButton || leftButton || centerButton){
				state = SM1_alive;
				initStateMachines();
			}
			break;
		default:
			SM1_State = SM1_alive;
		} // Transitions

		switch(state) { // State actions
		case SM1_gameOver:{

		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return state;
}

int TankBulletUpdate_SM(int state) {
	if(state == -1)
	{
		state = SM2_bullet;
	}
	else{
		switch(state) { // Transitions
		case SM2_bullet:
			if(getLives() <= 0 || getLives() > 10){
				state = SM2_gameOver;
			}
			else {
				if (0) { // if there are no bullets
					state = SM2_bullet;
				}
				else if(1){
					renderTankBullet(1);
				}
			}
			break;
		default:
			state = SM2_bullet;
		} // Transitions

		switch(state) { // State actions
		case SM2_bullet:{
		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return state;
}

int AlienMovementAndBullets_SM(int state) {
	if(state == -1)
	{
		state = SM3_alien;
	}
	else{
		switch(state) { // Transitions
		case SM3_alien:{
			if(getLives() <= 0 || getLives() > 10){
				state = SM3_gameOver;
			}
			else {
				u8 random;
				random = (char)(rand() % ALIEN_BULLET_FIRE_RATE);
				if(random < 10){
					fireAlienBullet(random);
					renderAliens(0);
				}
				else {
					// dont fire Alien Bullet
				}

				if(1){
					renderAliens(1);
					state = SM3_alien;
				}
			}
		}
		break;
		default:
			state = SM3_alien;
		} // Transitions

		switch(state) { // State actions
		case SM3_alien:{

		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return state;
}

int AlienbulletsUpdate_SM(int state) {
	if(state == -1)
	{
		state = SM4_bullets;
	}
	else{
		switch(state) { // Transitions
		case SM4_bullets:
			if(getLives() <= 0 || getLives() > 10){
				state = SM4_gameOver;
			}
			else {
				if (0) { // if there are no bulletss
					state = SM4_bullets;
				}
				else if(1){
					renderAlienBullet(1);
					renderAliens(0);
					state = SM4_bullets;
				}
			}
			break;
		default:
			state = SM4_bullets;
		} // Transitions

		switch(state) { // State actions
		case SM4_bullets:{
		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return state;
}

int SpaceShipUpdate_SM(int state) {
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 upButton = ((buttons & UP) >> 4) & 0x1;
	if(state == -1)
	{
		state = SM5_alive;
	}
	else{
		switch(state) { // Transitions
		case SM5_alive:
			if(getLives() <= 0 || getLives() > 10){
				state = SM5_gameOver;
			}
			else {
				if(upButton) { // Space Ship is dead
					state = SM5_dead;
				}
				else {
					state = SM5_alive;
					renderSpaceShip();
				}
			}
			break;
		default:
			state = SM5_alive;
		} // Transitions

		switch(state) { // State actions
		case SM5_alive:{
			u8 random;
			random = (char)(rand() % SPACESHIP_START_RATE);
			if(random < 1){
				startSpaceShip();
			}
		}
		break;
		case SM5_dead: {

		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return state;
}
