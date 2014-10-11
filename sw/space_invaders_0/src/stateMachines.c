/*
 * stateMachines.c
 *
 *  Created on: Oct 9, 2014
 *      Author: superman
 */
#include "stateMachines.h"
#include "globals.h"
#include "render.h"


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

			if(centerButton){
				fireTankBullet();
			}

			if (tankLife == 0) { // it the tank is dead TANK DEATH FLAG
				state = SM1_dead;
				cycles = TANK_MAP_FLIP_CYCLES;
				i = 0;
//				xil_printf("\n\rDead Tank!!!");
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
			break;
		case SM1_dead:
			if (cycles <= 0) { // it the tank is dead TANK DEATH FLAG
				state = SM1_alive;
				tankLife = 1;
				cycles = TANK_MAP_FLIP_CYCLES;
				i = 0;
				moveTank(TANK_INIT_POSITION_X);
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
		default:
			SM1_State = SM1_alive;
		} // Transitions

		switch(state) { // State actions
		case SM1_alive:{

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
			if (0) { // if there are no bullets
				state = SM2_bullet;
			}
			else if(1){
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
			if(random < 10){
				fireAlienBullet(random);
			}
			else {
				// dont fire Alien Bullet
			}

			if(1){
				//renderAliens(1);
				state = SM3_alien;
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
			if (0) { // if there are no bulletss
				state = SM4_bullets;
			}
			else if(1){
				renderAlienBullet(1);
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
			if(upButton) { // Space Ship is dead
				state = SM5_dead;
			}
			else {
				state = SM5_alive;
				renderSpaceShip();
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
