/*
 * inputManager.h
 *
 *  Created on: Nov 22, 2014
 *      Author: superman
 */

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#define SCREEN_PIXEL_WIDTH 640
#define FIFO_LENGTH 100
#define BUFFER_PERCENTAGE 10
#define UNINIT_RIGHT_BOUND 1404119
#define UNINIT_LEFT_BOUND 0
#define SMOOTHING_BUFFER (3*TANK_MOVEMENT_SPEED)

void initFifo();
void push(unsigned int input);
unsigned int averageFifo(unsigned int deep);
void setLeftBound(unsigned int input);
void setRightBound(unsigned int input);
void setBuffers();
char areBuffersSet();

#endif /* INPUTMANAGER_H_ */
