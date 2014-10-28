/*
 * stateMachines.c
 *
 *  Created on: Oct 9, 2014
 *      Author: Jeff Ravert and Luke Hsiao
 */
#include "stateMachines.h"
#include <stdio.h>
#include <stdlib.h>
#include "render.h"			// Our rendering file.
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "xtmrctr.h" // axi Timer
#include "unistd.h"
#include "render.h"			// Our rendering file.
#include "xuartlite_l.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "spaceInvadersSounds.h"


/////////////////////////////////////
// Handy Timing Functions
/////////////////////////////////////
void startTiming() {
	XTmrCtr_Start(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID);
}

void stopTiming() {
	XTmrCtr_Stop(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID);
	tempWcet = XTmrCtr_GetValue(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID);
	if(tempWcet > maxWcet){ // it the current timer is longer than our worst case prior
		maxWcet = tempWcet; // save that as our worst case timer
		xil_printf("\n\r maxWcet is %d cycles", maxWcet);
		if(maxWcet > 1000000) // if the timer is over our clock tick period
			xil_printf("\n\r TIMER OVERRUN"); // there was timer overrun
	}
}


////////////////////////////////////////
// Initializing State Machine Variables
////////////////////////////////////////
// When fuction is called it initiaizes all the SMs
// also clears the screen
// and initializes the game window and variables
void initStateMachines(){
	// tasks[taski].state holds the current state for the respective SM
	// tasks[taski].state = -1; causes the SM to go to the first state
	// tasks[taski].period sets the number of ticks between each SM call
	// tasks[taski].elapsedTime keeps track of number of ticks since last SM call
	// tasks[taski].TickFct holds the address to the SM function call
	// tasks[taski].wcet holds worst case execution time
	// worst case timing needs to start super low
	// tasks[taski].bcet holds best case execution time
	// best case timing needs to start super high
	int taski;
	taski=0;
	//	0
	// Tank Movement
	tasks[taski].state = -1;
	tasks[taski].period = 1; // 10ms
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &TankMovementAndBullet_SM;
	tasks[taski].wcet = 0;
	tasks[taski].bcet = 100000000;
	++taski;

	//	1
	// Tank Bullet Movement
	tasks[taski].state = -1;
	tasks[taski].period = 5; // 50ms
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &TankBulletUpdate_SM;
	tasks[taski].wcet = 0;
	tasks[taski].bcet = 100000000;
	++taski;

	//	2
	// Aliens Movement
	tasks[taski].state = -1;
	tasks[taski].period = 60; // 600ms but changes as aliens die and move down
	tasks[taski].elapsedTime = 0;
	tasks[taski].TickFct = &AlienMovementAndBullets_SM;
	tasks[taski].wcet = 0;
	tasks[taski].bcet = 100000000;
	++taski;

	//	3
	// Aliens Bullets Movement
	tasks[taski].state = -1;
	tasks[taski].period = 5; // 50ms
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &AlienbulletsUpdate_SM;
	tasks[taski].wcet = 0;
	tasks[taski].bcet = 100000000;
	++taski;

	//	4
	// Spaceship Movement and creation
	tasks[taski].state = -1;
	tasks[taski].period = 1; // 10ms
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &SpaceShipUpdate_SM;
	tasks[taski].wcet = 0;
	tasks[taski].bcet = 100000000;
	++taski;

	//	5
	// Alien Death and Explosion
	tasks[taski].state = -1;
	tasks[taski].period = tasks[3].period; // set to be the same as Alien Movement
	tasks[taski].elapsedTime = tasks[taski].period;
	tasks[taski].TickFct = &AlienDeath_SM;
	tasks[taski].wcet = 0;
	tasks[taski].bcet = 100000000;
	++taski;
	
	// setting up game
	initGlobals();
	blankScreen(); // erase old data
	render();      // draw initialized game
}

// TankMovementAndBullet_SM takes care of the tank and the tank bullet
// this SM has 3 states alive, dead, gameover
//
// alive: 	this state is used for when the tank hasnt been hit and isnt animating death
// 		if gameOver is set to true the SM transitions to gameover
// 		when user presses left the state takes a transition that calls moveTankLeft
// 		when user presses right the state takes a transition that calls moveTankRight
// 		when user presses center the SM makes no state change but calls fireTankBullet
// 		when the tank life has been set to zero the SM transitions to dead
//
// dead:  	this state is when the tank has been hit
// 		this state also animates the tank death using the i counter and the cycle counter
// 		i will count up to TANK_MAP_FLIP_COUNT
// 		cycles will count down from TANK_MAP_FLIP_CYCLES to 0
// 		if gameOver is set to true the SM transitions to gameover
//		the first tank death bit map is shown for 1/2 of the time
//		the second tank death bit map is shown for the other 1/2 of the time
//		for the latter half of the count shows the second bitmap to show the tank dead or a short time
//		after enough cycles this state returns back to alive
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
			if(getGameOver()){ // game is over!!!  you loose!
				state = SM1_gameOver;
			}
			else{
				if(centerButton){ // user is pressing fire!!!
					fireTankBullet();
				}

				if (getTankLife() == 0) { // it the tank is dead TANK DEATH FLAG
					state = SM1_dead;
					cycles = TANK_MAP_FLIP_CYCLES;
				}
				else if(!rightButton && !leftButton){ // user isnt pressing anything
					state = SM1_alive;
				}
				else if(rightButton) { // user is pressing right
					state = SM1_alive;
					moveTankRight();
				}
				else if(leftButton) { // user is pressing left
					state = SM1_alive;
					moveTankLeft();

				}
			}
			break;
		case SM1_dead:
			if(getGameOver()){ // game is over!!!  you loose!
				state = SM1_gameOver;
			}
			else if (cycles <= 0) { 		// if the tank is dead TANK DEATH FLAG
				state = SM1_alive; 		// go to alive
				setTankLife(1); 		// bring the tank life back to true
				cycles = TANK_MAP_FLIP_CYCLES; 	// reset the cycle count for next time
				i = 0; 				// reset the count for next time
				moveTank(TANK_INIT_POSITION_X); // reset the tank to the starting position
				u8 lives = getLives();		// get the lives
				lives--;			// decrament the lives
				if(lives <= 0 || lives > 10){	// setting the bounds of lives
					lives = 0;		// kill the lives
					setGameOver(1);		// set the gameOver flag
				}
				setLives(lives);		// save the lives
				if(!getGameOver())		//  
					renderLives();		// 
			}
			else if (cycles <= TANK_MAP_FLIP_CYCLES/3) { //

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

			if(downButton)
				blankScreen();
			renderGameOverText();
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
			if(downButton) {
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM wcet: \t\t%d clock cycles", tasks[i++].wcet);
				xil_printf("\n\rTankBulletUpdate_SM wcet: \t\t%d clock cycles", tasks[i++].wcet);
				xil_printf("\n\rAlienMovementAndBullets_SM wcet: \t%d clock cycles", tasks[i++].wcet);
				xil_printf("\n\rAlienbulletsUpdate_SM wcet: \t\t%d clock cycles", tasks[i++].wcet);
				xil_printf("\n\rSpaceShipUpdate_SM wcet: \t\t%d clock cycles", tasks[i++].wcet);
				xil_printf("\n\rAlienDeath_SM wcet: \t\t\t%d clock cycles\n\r", tasks[i++].wcet);
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM wcet: \t\t%d ns", tasks[i++].wcet*10);
				xil_printf("\n\rTankBulletUpdate_SM wcet: \t\t%d ns", tasks[i++].wcet*10);
				xil_printf("\n\rAlienMovementAndBullets_SM wcet: \t%d ns", tasks[i++].wcet*10);
				xil_printf("\n\rAlienbulletsUpdate_SM wcet: \t\t%d ns", tasks[i++].wcet*10);
				xil_printf("\n\rSpaceShipUpdate_SM wcet: \t\t%d ns", tasks[i++].wcet*10);
				xil_printf("\n\rAlienDeath_SM wcet: \t\t\t%d ns\n\r", tasks[i++].wcet*10);
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM wcet: \t\t%d us", tasks[i++].wcet/100);
				xil_printf("\n\rTankBulletUpdate_SM wcet: \t\t%d us", tasks[i++].wcet/100);
				xil_printf("\n\rAlienMovementAndBullets_SM wcet: \t%d us", tasks[i++].wcet/100);
				xil_printf("\n\rAlienbulletsUpdate_SM wcet: \t\t%d us", tasks[i++].wcet/100);
				xil_printf("\n\rSpaceShipUpdate_SM wcet: \t\t%d us", tasks[i++].wcet/100);
				xil_printf("\n\rAlienDeath_SM wcet: \t\t\t%d us\n\r", tasks[i++].wcet/100);
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM wcet: \t\t%d ms", tasks[i++].wcet/100000);
				xil_printf("\n\rTankBulletUpdate_SM wcet: \t\t%d ms", tasks[i++].wcet/100000);
				xil_printf("\n\rAlienMovementAndBullets_SM wcet: \t%d ms", tasks[i++].wcet/100000);
				xil_printf("\n\rAlienbulletsUpdate_SM wcet: \t\t%d ms", tasks[i++].wcet/100000);
				xil_printf("\n\rSpaceShipUpdate_SM wcet: \t\t%d ms", tasks[i++].wcet/100000);
				xil_printf("\n\rAlienDeath_SM wcet: \t\t\t%d ms\n\r", tasks[i++].wcet/100000);
				i = 0;
				if(tasks[i++].wcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in TankMovementAndBullet_SM");
				if(tasks[i++].wcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in TankBulletUpdate_SM");
				if(tasks[i++].wcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in AlienMovementAndBullets_SM");
				if(tasks[i++].wcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in AlienbulletsUpdate_SM");
				if(tasks[i++].wcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in SpaceShipUpdate_SM");
				if(tasks[i++].wcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in AlienDeath_SM");
				i = 0;
				int j;
				u32 tempTotalWcet = 0;
				for(j = 0; j < 6; j++){
					tempTotalWcet += tasks[j++].wcet;
				}
				xil_printf("\n\n\rTotal Wcet: %d clock cycles", tempTotalWcet);
				if(tempTotalWcet > XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ/100)
					xil_printf("\n\n\r\t\t\tOverrun in Total Wcet");

				i = 0;
				xil_printf("\n\n\n\n\rTankMovementAndBullet_SM bcet: \t\t%d clock cycles", tasks[i++].bcet);
				xil_printf("\n\rTankBulletUpdate_SM bcet: \t\t%d clock cycles", tasks[i++].bcet);
				xil_printf("\n\rAlienMovementAndBullets_SM bcet: \t%d clock cycles", tasks[i++].bcet);
				xil_printf("\n\rAlienbulletsUpdate_SM bcet: \t\t%d clock cycles", tasks[i++].bcet);
				xil_printf("\n\rSpaceShipUpdate_SM bcet: \t\t%d clock cycles", tasks[i++].bcet);
				xil_printf("\n\rAlienDeath_SM bcet: \t\t\t%d clock cycles\n\r", tasks[i++].bcet);
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM bcet: \t\t%d ns", tasks[i++].bcet*10);
				xil_printf("\n\rTankBulletUpdate_SM bcet: \t\t%d ns", tasks[i++].bcet*10);
				xil_printf("\n\rAlienMovementAndBullets_SM bcet: \t%d ns", tasks[i++].bcet*10);
				xil_printf("\n\rAlienbulletsUpdate_SM bcet: \t\t%d ns", tasks[i++].bcet*10);
				xil_printf("\n\rSpaceShipUpdate_SM bcet: \t\t%d ns", tasks[i++].bcet*10);
				xil_printf("\n\rAlienDeath_SM bcet: \t\t\t%d ns\n\r", tasks[i++].bcet*10);
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM bcet: \t\t%d us", tasks[i++].bcet/100);
				xil_printf("\n\rTankBulletUpdate_SM bcet: \t\t%d us", tasks[i++].bcet/100);
				xil_printf("\n\rAlienMovementAndBullets_SM bcet: \t%d us", tasks[i++].bcet/100);
				xil_printf("\n\rAlienbulletsUpdate_SM bcet: \t\t%d us", tasks[i++].bcet/100);
				xil_printf("\n\rSpaceShipUpdate_SM bcet: \t\t%d us", tasks[i++].bcet/100);
				xil_printf("\n\rAlienDeath_SM bcet: \t\t\t%d us\n\r", tasks[i++].bcet/100);
				i = 0;
				xil_printf("\n\n\rTankMovementAndBullet_SM bcet: \t\t%d ms", tasks[i++].bcet/100000);
				xil_printf("\n\rTankBulletUpdate_SM bcet: \t\t%d ms", tasks[i++].bcet/100000);
				xil_printf("\n\rAlienMovementAndBullets_SM bcet: \t%d ms", tasks[i++].bcet/100000);
				xil_printf("\n\rAlienbulletsUpdate_SM bcet: \t\t%d ms", tasks[i++].bcet/100000);
				xil_printf("\n\rSpaceShipUpdate_SM bcet: \t\t%d ms", tasks[i++].bcet/100000);
				xil_printf("\n\rAlienDeath_SM bcet: \t\t\t%d ms\n\r", tasks[i++].bcet/100000);
				i = 0;

				u32 tempTotalBcet = 0;
				for(j = 0; j < 6; j++){
					tempTotalBcet += tasks[j++].bcet;
				}
				xil_printf("\n\n\rTotal Bcet: %d clock cycles", tempTotalBcet);
			}
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
			if(random < 10){
				//				startTiming();
				random = (char)(rand() % 11);
				fireAlienBullet(random);
				//				stopTiming();
			}

			if(getGameOver()){
				state = SM3_gameOver;
			}
			else if(1){
				if(!getGameOver()){
					renderAliens(1);
				}
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
			newPeriod = newPeriod - (((55-aliensAlive)*4)/10);
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
	static int waitShow;
	static point_t savedPosition;
	u32 buttons = XGpio_DiscreteRead(&gpPB, 1);
	u32 upButton = ((buttons & UP) >> 4) & 0x1;
	u32 downButton = ((buttons & DOWN) >> 2) & 0x1;
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
			else if(getSpaceshipDied()) { // Space Ship is dead (up can be pressed for spaceship Death testing
				if(upButton){
					u32 tempScore = ((rand() % 7)+1) * 50;
					setSpaceshipScore(tempScore);
					xil_printf("\n\r %d cycles", XTmrCtr_GetValue(&Timer0, XPAR_AXI_TIMER_0_DEVICE_ID));
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
		if(upButton && downButton){
			midVol();
		}
		else if(downButton){
			decreaseVol();
		}
		else if(upButton){
			increaseVol();
		}
		case SM5_alive:{
			if(waitShow >= EXTRA_WAIT){
				u32 showRandom;
				showRandom = (char)(rand() % SPACESHIP_START_RATE);
				if(showRandom == 0){
					startSpaceShip();
				}
				waitShow = 0;
			}
			waitShow++;
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
