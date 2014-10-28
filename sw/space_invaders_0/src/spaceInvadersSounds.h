/*
 * spaceInvadersSounds.h
 *
 *  Created on: Oct 25, 2014
 *      Author: superman
 */

#include "xac97_l.h"

#ifndef SPACEINVADERSSOUNDS_H_
#define SPACEINVADERSSOUNDS_H_

#define SOUND_NUM 9
#define SPACESHIP_MOVE_NUM 	0 // Lowest Priority
#define ALIEN_MOVE1_NUM 	1
#define ALIEN_MOVE2_NUM 	2
#define ALIEN_MOVE3_NUM 	3
#define ALIEN_MOVE4_NUM 	4
#define TANK_FIRE_NUM 		5
#define ALIEN_DEATH_NUM 	6
#define SPACESHIP_DEATH_NUM 7
#define TANK_DEATH_NUM 		8 // Highest Priority

#define INACTIVE 	0
#define ACTIVE 		1

#define TRUE		1
#define FALSE		0

#define NUM_FIFO_SAMPLES_FILL 250

typedef struct
{
	unsigned int active;
	unsigned int sampleRate;
	unsigned int currentSample;
	unsigned int size;
	int *arrayAddress;
} soundData;

void initSounds();
void updateSampleRate(int soundNum);
int getActive(int soundNum);
void setActive(int soundNum, int active);
int getSampleRate(int soundNum);
int getCurrentSampleNum(int soundNum);
void setCurrentSampleNum(int soundNum, int sample);
int getSize(int soundNum);
int* getArrayAddress(int soundNum);
void deactivateOthers(int soundNum);
int getCurrentSample(int soundNum);
int getTotalActive();

#endif /* SPACEINVADERSSOUNDS_H_ */