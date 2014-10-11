/*
 * stateMachines.c
 *
 *  Created on: Oct 9, 2014
 *      Author: superman
 */
#include "stateMachines.h"
#include "globals.h"


int TankMovementAndBullet_SM() {
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 rightButton = ((buttons & RIGHT) >> 1) & 0x1;
	u32 leftButton = ((buttons & LEFT) >> 3) & 0x1;
	u32 centerButton = (buttons & CENTER) & 0x1;
	//	xil_printf("\n\rButtons: %x",buttons);
	if(SM1_State == -1)
	{
		SM1_State = SM1_tank;
	}
	else{
		switch(SM1_State) { // Transitions
		case SM1_tank:
			if (0) { // it the tank is dead
				SM1_State = SM1_tank;
			}
			else if(centerButton){
				fireTankBullet();
			}
			if (0) { // it the tank is dead
				SM1_State = SM1_tank;
			}
			else if(!rightButton && !leftButton){
				SM1_State = SM1_tank;
			}
			else if(rightButton) {
				SM1_State = SM1_tank;
				moveTankRight();
			}
			else if(leftButton) {
				SM1_State = SM1_tank;
				moveTankLeft();
			}
			break;
		default:
			SM1_State = SM1_tank;
		} // Transitions

		switch(SM1_State) { // State actions
		case SM1_tank:{

		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return SM1_State;
}

int TankBulletUpdate_SM() {
	if(SM2_State == -1)
	{
		SM2_State = SM2_bullet;
	}
	else{
		switch(SM2_State) { // Transitions
		case SM2_bullet:
			if (0) { // if there are no bullets
				SM2_State = SM2_bullet;
			}
			else if(1){
				renderTankBullet(1);
			}
			break;
		default:
			SM2_State = SM2_bullet;
		} // Transitions

		switch(SM2_State) { // State actions
		case SM2_bullet:{
		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return SM2_State;
}

int AlienMovementAndBullets_SM() {
	if(SM3_State == -1)
	{
		SM3_State = SM3_alien;
	}
	else{
		switch(SM3_State) { // Transitions
		case SM3_alien:{
			u8 random;
			random = (char)(rand() % 100);//((timerSeed * 13) / 3) % 11; //pseudo random number between 0-10
			if(random < 10){
				fireAlienBullet(random);
			}
			else {
				// dont fire Alien Bullet
			}

			if(1){
				renderAliens(1);
			}
		}
			break;
		default:
			SM3_State = SM3_alien;
		} // Transitions

		switch(SM3_State) { // State actions
		case SM3_alien:{

		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return SM3_State;
}

int AlienbulletsUpdate_SM() {
	if(SM4_State == -1)
	{
		SM4_State = SM4_bullets;
	}
	else{
		switch(SM4_State) { // Transitions
		case SM4_bullets:
			if (0) { // if there are no bulletss
				SM4_State = SM4_bullets;
			}
			else if(1){
				renderAlienBullet(1);
			}
			break;
		default:
			SM4_State = SM4_bullets;
		} // Transitions

		switch(SM4_State) { // State actions
		case SM4_bullets:{
		}
		break;
		default: // ADD default behaviour below
			break;
		} // State actions
	}
	return SM4_State;
}
