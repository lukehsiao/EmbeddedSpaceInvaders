/*
 * spaceInvadersSounds.h
 *
 *  Created on: Oct 25, 2014
 *      Author: superman
 */

#include "xac97_l.h"

#ifndef SPACEINVADERSSOUNDS_H_
#define SPACEINVADERSSOUNDS_H_

#define SOUND_NUM 8

extern int alienDeath_soundData[];
extern int alienDeath_numberOfSamples;
extern int alienDeath_sampleRate;


extern int spaceship_soundData[];
extern int spaceship_numberOfSamples;
extern int spaceship_sampleRate;

extern int tankDeath_soundData[];
extern int tankDeath_numberOfSamples;
extern int tankDeath_sampleRate;

extern int tankShot_soundData[];
extern int tankShot_numberOfSamples;
extern int tankShot_sampleRate;

extern int shoot_soundData[];
extern int shoot_sampleRate;
extern int shoot_numberOfSamples;

typedef struct
{
	unsigned int active;
	unsigned int sampleRate;
	unsigned int currentSample;
	unsigned int size;
	int *arrayAddress;
} soundData;

soundData sounds[SOUND_NUM];

#endif /* SPACEINVADERSSOUNDS_H_ */
