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
//	0
	// Tank Movement
	tasks[taski].state = -1;
	tasks[taski].period = 4;
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &TankMovementAndBullet_SM;
	++taski;

//	1
	// Tank Bullet Movement
	tasks[taski].state = -1;
	tasks[taski].period = 5;
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &TankBulletUpdate_SM;
	++taski;

//	2
	// Aliens Movement
	tasks[taski].state = -1;
	tasks[taski].period = 60;
	tasks[taski].elapsedTime = 0;
	tasks[taski].TickFct = &AlienMovementAndBullets_SM;
	++taski;

//	3
	// Aliens Bullets Movement
	tasks[taski].state = -1;
	tasks[taski].period = 5;
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &AlienbulletsUpdate_SM;
	++taski;

//	4
	// Spaceship Movement and creation
	tasks[taski].state = -1;
	tasks[taski].period = 5;
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &SpaceShipUpdate_SM;
	++taski;

//	5
	// Alien Death and Explosion
	tasks[taski].state = -1;
	tasks[taski].period = tasks[3].period;
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &AlienDeath_SM;
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
	u32 downButton = ((buttons & DOWN) >> 2) & 0x1;
	//	xil_printf("\n\rButtons: %x",buttons);
	if(state == -1)
	{
		state = SM1_alive;
	}
	else{
		switch(state) { // Transitions
		case SM1_alive:
			if(getGameOver()){
				state = SM1_gameOver;
			}
			else{
				if(centerButton)
					fireTankBullet();

				if (getTankLife() == 0 || downButton) { // it the tank is dead TANK DEATH FLAG
					state = SM1_dead;
					cycles = TANK_MAP_FLIP_CYCLES;
					i = 0;
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
			if(getGameOver()){
				state = SM1_gameOver;
			}
			else if (cycles <= 0) { // it the tank is dead TANK DEATH FLAG
				state = SM1_alive;
				setTankLife(1);
				cycles = TANK_MAP_FLIP_CYCLES;
				i = 0;
				moveTank(TANK_INIT_POSITION_X);
				u8 lives = getLives();
				lives--;
				if(lives <= 0 || lives > 10){
					lives = 0;
					setGameOver(1);
				}
				setLives(lives);
				if(!getGameOver())
					renderLives();

			}
			else if (cycles <= TANK_MAP_FLIP_CYCLES/3) {
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
			break;
		case SM1_gameOver:
			blankScreen();
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
			if(getGameOver()){
				state = SM2_gameOver;
			}
			else if (0) { // if there are no bullets
				state = SM2_bullet;
			}
			else if(1){
				if(!getGameOver())
					renderTankBullet(1);
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

			u8 random;
			random = (char)(rand() % ALIEN_BULLET_FIRE_RATE);
			if(random < 10)
				fireAlienBullet(random);

			if(getGameOver()){
				state = SM3_gameOver;
			}
			else if(1){
				if(!getGameOver())
					renderAliens(1);
				state = SM3_alien;
			}
		}
		break;
		case SM3_gameOver:
			break;
		default:
			state = SM3_alien;
		} // Transitions

		switch(state) { // State actions
		case SM3_alien:{
			signed long newPeriod = ALIEN_STATE_MACHINE_RATE_MAX
									 -((getAlienBlockPosition().y-ALIEN_STARTING_Y_POSITION)/ALIEN_HEIGHT)*2;
			u8 aliensAlive = getNumberAliensAlive();
			newPeriod = newPeriod - (((54-aliensAlive)*6)/10);
			if(newPeriod <= 1)
				newPeriod = 1;
			tasks[2].period = newPeriod;
			newPeriod = newPeriod;
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
			if(getGameOver()){
				state = SM4_gameOver;
			}
			else if (0) { // if there are no bullets
				state = SM4_bullets;
			}
			else if(1){
				if(!getGameOver()){
					renderAlienBullet(1);
					renderAliens(0);
				}
				state = SM4_bullets;
			}
			break;
		default:
			state = SM4_bullets;
		} // Transitions

		switch(state) { // State actions
		case SM4_bullets:{
		}
		break;
		default: // ADD default behavior below
			break;
		} // State actions
	}
	return state;
}

int SpaceShipUpdate_SM(int state) {
	static int i;
	static int cycles;
	static point_t savedPosition;
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 upButton = ((buttons & UP) >> 4) & 0x1;
	if(state == -1)
	{
		state = SM5_alive;
	}
	else{
		switch(state) { // Transitions
		case SM5_alive:
			if(getGameOver()){
				state = SM5_gameOver;
			}
			else if(upButton || getSpaceshipDied()) { // Space Ship is dead (up can be pressed for spaceship Death testing
				if(upButton){
					u32 tempScore = ((rand() % 7)+1) * 50;
					setSpaceshipScore(tempScore);
				}
				state = SM5_dead;
				cycles = SPACESHIP_FLASH_SCORE_CYCLES + SPACESHIP_STEADY_SCORE_CYCLES;
				i = 0;
				savedPosition = getSpaceshipPosition();
				setSpaceshipDied(0);
			}
			else {
				state = SM5_alive;
				if(!getGameOver())
					renderSpaceShip();
			}
			break;
		case SM5_dead:
			if(getGameOver()){
				state = SM5_gameOver;
			}
			else if (cycles <= 0) { // Score has Flashed and the Spaceship can come back
				state = SM5_alive;
				unrenderPoints(savedPosition);
				cycles = SPACESHIP_FLASH_SCORE_CYCLES + SPACESHIP_STEADY_SCORE_CYCLES;
				i = 0;
			}
			else if (cycles <= SPACESHIP_STEADY_SCORE_CYCLES) { // Score has flashed but needs to stay visible for a sec
				state = SM5_dead;
				renderPoints(getSpaceshipScore(), savedPosition);
				cycles--;
			}
			else if(i < SPACESHIP_SCORE_COUNT/2) { // dont show the point
				state = SM5_dead;
				unrenderPoints(savedPosition);
				renderPoints(getSpaceshipScore(), savedPosition);
				i++;
			}
			else if(i < SPACESHIP_SCORE_COUNT) { // show the point
				state = SM5_dead;
				unrenderPoints(savedPosition);
				i++;
			}
			else if(i >= SPACESHIP_SCORE_COUNT) { // one cycle has happened
				state = SM5_dead;
				unrenderPoints(savedPosition);
				cycles--;
				i = 0;
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

int AlienDeath_SM(int state) {
	static point_t position;
	static int i;
	if(state == -1)
	{
		state = SM6_noDeath;
	}
	else{
		switch(state) { // Transitions
		case SM6_noDeath:
			if(getGameOver()){
				state = SM6_gameOver;
				setAlienDeath(0);
			}
			else if(getAlienDeath()) {
				state = SM6_death;
				setAlienDeath(0);
				position = getAlienExplosionPosition();
			}
			else {
				state = SM6_noDeath;
			}
			break;
		case SM6_death:
			if(getGameOver()){
				state = SM6_gameOver;
				unrenderDeadAlien(position);
				i = 0;
			}
			else if (i >= ALIEN_EXPLOSION_SHOW_TIME) {
				state = SM6_noDeath;
				unrenderDeadAlien(position);
				i = 0;
			}
			else {
				state = SM6_death;
				i++;
			}
			break;

		default:
			state = SM6_noDeath;
		} // Transitions

		switch(state) { // State actions
		case SM6_noDeath:{
			position = getAlienExplosionPosition();
		}
		break;
		case SM6_death: {
		}
		break;
		default:
			break;
		} // State actions
	}
	return state;
}
