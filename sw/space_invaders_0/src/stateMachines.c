/*
 * stateMachines.c
 *
 *  Created on: Oct 9, 2014
 *      Author: superman
 */
#include "stateMachines.h"
#include "globals.h"


int TankMovement_SM() {
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 rightButton = ((buttons & RIGHT) >> RIGHT) & 0x1;
	u32 leftButton = ((buttons & LEFT) >> LEFT) & 0x1;
	if(SM1_State == -1)
	{
		SM1_State = SM1_tank;
	}
	else{
		switch(SM1_State) { // Transitions
		case SM1_tank:
			if (getTankLife()) {
				SM1_State = SM1_tank;
			}
			else if(!rightButton && !leftButton){
				SM1_State = SM1_tank;
			}
			else if(rightButton) {
				// move right
			}
			else if(leftButton) {
				// move left
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

int TankBullet_SM() {
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 centerButton = ((buttons & CENTER) >> CENTER) & 0x1;
	u32 tankBulletThere;
	if(SM2_State == -1)
	{
		SM2_State = SM2_ready;
	}
	else{
		switch(SM2_State) { // Transitions
		case SM2_ready:
			if (getTankLife()) {
				SM2_State = SM2_ready;
			}
			else if(!centerButton){
				SM2_State = SM2_ready;
			}
			else if(centerButton) {
				SM2_State = SM2_fire;
				//create tank bullet
			}
			break;
			case SM2_fire:
			if(tankBulletThere){
				SM2_State = SM2_fire;
				//move tank bullet
			}
			else if(!tankBulletThere) {
				SM2_State = SM2_ready;
				// delete tank bullet
			}
			break;
		default:
			SM2_State = SM2_ready;
		} // Transitions
	}
	return SM2_State;
}
