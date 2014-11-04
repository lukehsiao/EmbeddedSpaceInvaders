/*
 * Implements bunkers.h and contains the bitmaps for the bunkers.
 *
 *  @author Luke Hsiao
 *  @author Jeff Revert
 *  @date 10 Oct 2014
 */
#include "globals.h"
#include "bunkers.h"

extern u32* framePointer0;
extern u32* framePointer1;

/**
 * Draws the bunker
 * @param bunkerNumber The bunker to draw (0-3)
 */
void renderBunker(u8 bunkerNumber){
	u32 col;
	u32 row;
	u32 blockNum;
	point_t bunkerPosition;
	u32 rowsPixels;
	u8 pixelPresent;
	bunkerPosition = getBunkerPosition(bunkerNumber);
	for (blockNum = 0; blockNum < BLOCK_FOUR; blockNum++) {
		for (row = 0; row < BLOCK_WIDTH; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < BLOCK_WIDTH; col++) {
				pixelPresent = (rowsPixels >> (LEFT_BIT - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + row)*RIGHT_SIDE + (bunkerPosition.x+(blockNum*BLOCK_WIDTH))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*RIGHT_SIDE + (bunkerPosition.x+(blockNum*BLOCK_WIDTH))+col] = GREEN;
						framePointer1[(bunkerPosition.y + row)*RIGHT_SIDE + (bunkerPosition.x+(blockNum*BLOCK_WIDTH))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + row)*RIGHT_SIDE + (bunkerPosition.x+(blockNum*BLOCK_WIDTH))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*RIGHT_SIDE + (bunkerPosition.x+(blockNum*BLOCK_WIDTH))+col] = BLACK;
						framePointer1[(bunkerPosition.y + row)*RIGHT_SIDE + (bunkerPosition.x+(blockNum*BLOCK_WIDTH))+col] = BLACK;
					}
				}
			}
		}
	}
	for (blockNum = BLOCK_FOUR; blockNum < BLOCK_EIGHT; blockNum++) {
		for (row = 0; row < BLOCK_WIDTH; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < BLOCK_WIDTH; col++) {
				pixelPresent = (rowsPixels >> (LEFT_BIT - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + BLOCK_WIDTH + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_FOUR)*BLOCK_WIDTH))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + BLOCK_WIDTH + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_FOUR)*BLOCK_WIDTH))+col] = GREEN;
						framePointer1[(bunkerPosition.y + BLOCK_WIDTH + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_FOUR)*BLOCK_WIDTH))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + BLOCK_WIDTH + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_FOUR)*BLOCK_WIDTH))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + BLOCK_WIDTH + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_FOUR)*BLOCK_WIDTH))+col] = BLACK;
						framePointer1[(bunkerPosition.y + BLOCK_WIDTH + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_FOUR)*BLOCK_WIDTH))+col] = BLACK;
					}
				}
			}
		}
	}
	for (blockNum = BLOCK_EIGHT; blockNum < (NUMBER_BOTTOM_RIGHT_BLOCK + 1); blockNum++) {
		for (row = 0; row < BLOCK_WIDTH; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < BLOCK_WIDTH; col++) {
				pixelPresent = (rowsPixels >> (LEFT_BIT - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] = GREEN;
						framePointer1[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] = GREEN;
					}
				}
				else {
					u32 alienPresent = (framePointer0[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] == WHITE);
					u32 bulletPresent = (framePointer0[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] == OFFWHITE);
					if (!alienPresent && !bulletPresent) {
						framePointer0[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] = BLACK;
						framePointer1[(bunkerPosition.y + BLOCK_WIDTH*2 + row)*RIGHT_SIDE + (bunkerPosition.x+((blockNum-BLOCK_EIGHT)*BLOCK_WIDTH))+col] = BLACK;
					}
				}
			}
		}
	}
}

/**
 * Calculates whether the given position falls within the bunker.
 *
 * @param position the (x,y) position to test
 * @param bunkerNumber the number of the bunker to test
 * @return the block number that the position falls in, or 0xFF if it isn't hit
 */
u8 hitBunker(point_t position, u8 bunkerNumber) {
	point_t bunkerPosition;
	bunkerPosition = getBunkerPosition(bunkerNumber);
	u8 currentBlockState;
	// If it's not within the bunker itself
	if (position.x < bunkerPosition.x || position.y < bunkerPosition.y) {
		return BUNKER_DEAD;
	}
	else if (position.x > (bunkerPosition.x + BLOCK_WIDTH*4) || position.y > (bunkerPosition.y + BLOCK_WIDTH*3)) {
		return BUNKER_DEAD;
	}

	u32 blockNum;

	//Check top row intersections
	for (blockNum = 0; blockNum < BLOCK_FOUR; blockNum++) {
		// If the block is alive
		currentBlockState = getBlockState(bunkerNumber, blockNum);
		if (currentBlockState < DEAD_BLOCK) {
			//If it's within the horizontal range
			if (position.x >= (bunkerPosition.x+(blockNum)*BLOCK_WIDTH) && position.x <= (bunkerPosition.x + (BLOCK_WIDTH*(blockNum+1)))) {
				//If it's also within the vertical range
				if (position.y > bunkerPosition.y && position.y < (bunkerPosition.y + BLOCK_WIDTH)) {
					currentBlockState++;
					setBlockState(bunkerNumber, blockNum, currentBlockState);
					renderBunker(bunkerNumber);
					return blockNum;
				}
			}
		}
	}

	//Check Middle row intersections
	for (blockNum = BLOCK_FOUR; blockNum < BLOCK_EIGHT; blockNum++) {
		currentBlockState = getBlockState(bunkerNumber, blockNum);
		if (currentBlockState < DEAD_BLOCK) {
			//If it's within the horizontal range
			if (position.x >= (bunkerPosition.x+(blockNum-BLOCK_FOUR)*BLOCK_WIDTH) && position.x <= (bunkerPosition.x + (BLOCK_WIDTH*(blockNum-BLOCK_THREE)))) {
				//If it's also within the vertical range
				if (position.y > (bunkerPosition.y+BLOCK_WIDTH) && position.y < (bunkerPosition.y + BLOCK_WIDTH*2)) {
					currentBlockState++;
					setBlockState(bunkerNumber, blockNum, currentBlockState);
					renderBunker(bunkerNumber);
					return blockNum;
				}
			}
		}
	}

	//Check Bottom row intersections
	for (blockNum = BLOCK_EIGHT; blockNum < (NUMBER_BOTTOM_RIGHT_BLOCK + 1); blockNum++) {
		// If the block is alive
		currentBlockState = getBlockState(bunkerNumber, blockNum);
		if (currentBlockState < DEAD_BLOCK) {
			//If it's within the horizontal range
			if (position.x >= (bunkerPosition.x+(blockNum-BLOCK_EIGHT)*BLOCK_WIDTH) && position.x <= (bunkerPosition.x + (BLOCK_WIDTH*(blockNum-BLOCK_SEVEN)))) {
				//If it's also within the vertical range
				if (position.y > (bunkerPosition.y+BLOCK_WIDTH*2) && position.y < (bunkerPosition.y + BLOCK_WIDTH*3)) {
					currentBlockState++;
					setBlockState(bunkerNumber, blockNum, currentBlockState);
					renderBunker(bunkerNumber);
					return blockNum;
				}
			}
		}
	}

	// If none of those...
	return 0xFF;
}
