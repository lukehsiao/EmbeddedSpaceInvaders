/**
 * Implements inputManager.h and contains FIFO functionality
 *
 *  @author Luke Hsiao
 *  @author Jeff Revert
 *  @date 22 Nov 2014
 */


#include "inputManager.h"
#include "globals.h"

#define AVERAGE_FIFO_DEPTH 25
#define SCREEN_PIXEL_WIDTH 640
u32 Fifo[FIFO_LENGTH];
unsigned int leftSide;
unsigned int rightSide;
unsigned int cyclesPerPixel;
u8 buffersSet;

u32 tankPixelDesired;

// Initializes the FIFO with with zeros
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

// Adds an item to the FIFO. If the FIFO is full,
// one item is pushed out as this item is pushed in
void push(u32 input) {
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

// Takes the average value of the last 'deep' FIFO entries.
// @param deep The number of values to average
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

// Saves the distance currently being read as the left border
// If the right side is set also, sets the buffers.
void setLeftBound(u32 input){
	unsigned int inputF = input;
	leftSide = inputF;
	if(rightSide < UNINIT_RIGHT_BOUND)
		setBuffers();
}

// Saves the distance currently being read as the right border
// If the left side is set also, sets the buffers.
void setRightBound(u32 input){
	unsigned int inputF = input;
	rightSide = inputF;
	if(leftSide > UNINIT_LEFT_BOUND)
		setBuffers();
}

// Sets the borders and sets the buffersSet flag.
void setBuffers(){
	cyclesPerPixel = (rightSide - leftSide) / SCREEN_PIXEL_WIDTH;
	xil_printf("-------------BOUNDS SET-------------\n\r");
	buffersSet = TRUE;
}

char areBuffersSet(){
	return buffersSet;
}
