/*
 * inputManager.c
 *
 *  Created on: Nov 22, 2014
 *      Author: superman
 */

#include "inputManager.h"
#include "globals.h"

#define AVERAGE_FIFO_DEPTH 50
#define SCREEN_PIXEL_WIDTH 640
u32 Fifo[FIFO_LENGTH];
unsigned int leftSide;
unsigned int rightSide;
unsigned int cyclesPerPixel;
u8 buffersSet;

u32 tankPixelDesired;

void initFifo(){
	int i = 0;
	for (i = 0; i<FIFO_LENGTH; i++) {
		Fifo[i] = 0;
	}
	leftSide = UNINIT_LEFT_BOUND;
	rightSide = UNINIT_RIGHT_BOUND;
	cyclesPerPixel = 314.4906;
	buffersSet = FALSE;
}

void push(u32 input) {
	//	xil_printf("Distance: %d\n\r", input);
	int i = 0;
	for (i = 1; i<FIFO_LENGTH; i++) {
		Fifo[i] = Fifo[i-1];
	}

	Fifo[0] = input;

	unsigned int rawDistance =  averageFifo(AVERAGE_FIFO_DEPTH);

	if(rawDistance < leftSide)
		tankPixelDesired = 0;
	else if(rawDistance > rightSide)
		tankPixelDesired = SCREEN_PIXEL_WIDTH;
	else
		tankPixelDesired = (rawDistance - leftSide)/cyclesPerPixel;

}

u32 averageFifo(u32 deep){
	if(deep == 0)
		deep = 1;
	if(deep > FIFO_LENGTH)
		deep = FIFO_LENGTH;
	u32 adding = 0;
	int i;
	for(i = 0; i < deep; i++){
		adding += Fifo[i];
	}
	return adding/deep;
}

void setLeftBound(u32 input){
	unsigned int inputF = input;
	leftSide = inputF;
	if(rightSide < UNINIT_RIGHT_BOUND)
		setBuffers();
}

void setRightBound(u32 input){
	unsigned int inputF = input;
	rightSide = inputF;
	if(leftSide > UNINIT_LEFT_BOUND)
		setBuffers();
}

void setBuffers(){
	cyclesPerPixel = (rightSide - leftSide) / SCREEN_PIXEL_WIDTH;
	xil_printf("-------------BOUNDS SET-------------\n\r");
	buffersSet = TRUE;
}

char areBuffersSet(){
	return buffersSet;
}
