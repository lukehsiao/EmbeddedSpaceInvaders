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
	for (blockNum = 0; blockNum < 4; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = GREEN;
						framePointer1[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = BLACK;
						framePointer1[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = BLACK;
					}
				}
			}
		}
	}
	for (blockNum = 4; blockNum < 8; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = GREEN;
						framePointer1[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = BLACK;
						framePointer1[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = BLACK;
					}
				}
			}
		}
	}
	for (blockNum = 8; blockNum < 12; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = GREEN;
						framePointer1[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = GREEN;
					}
				}
				else {
					u32 alienPresent = (framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] == WHITE);
					u32 bulletPresent = (framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] == OFFWHITE);
					if (!alienPresent && !bulletPresent) {
						framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = BLACK;
						framePointer1[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = BLACK;
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
		return 0xFF;
	}
	else if (position.x > (bunkerPosition.x + 48) || position.y > (bunkerPosition.y + 36)) {
		return 0xFF;
	}

	u32 blockNum;

	//Check top row intersections
	for (blockNum = 0; blockNum < 4; blockNum++) {
		// If the block is alive
		currentBlockState = getBlockState(bunkerNumber, blockNum);
		if (currentBlockState < 4) {
			//If it's within the horizontal range
			if (position.x >= (bunkerPosition.x+(blockNum)*12) && position.x <= (bunkerPosition.x + (12*(blockNum+1)))) {
				//If it's also within the vertical range
				if (position.y > bunkerPosition.y && position.y < (bunkerPosition.y + 12)) {
					currentBlockState++;
					setBlockState(bunkerNumber, blockNum, currentBlockState);
					renderBunker(bunkerNumber);
					return blockNum;
				}
			}
		}
	}

	//Check Middle row intersections
	for (blockNum = 4; blockNum < 8; blockNum++) {
		currentBlockState = getBlockState(bunkerNumber, blockNum);
		if (currentBlockState < 4) {
			//If it's within the horizontal range
			if (position.x >= (bunkerPosition.x+(blockNum-4)*12) && position.x <= (bunkerPosition.x + (12*(blockNum-3)))) {
				//If it's also within the vertical range
				if (position.y > (bunkerPosition.y+12) && position.y < (bunkerPosition.y + 24)) {
					currentBlockState++;
					setBlockState(bunkerNumber, blockNum, currentBlockState);
					renderBunker(bunkerNumber);
					return blockNum;
				}
			}
		}
	}

	//Check Bottom row intersections
	for (blockNum = 8; blockNum < 12; blockNum++) {
		// If the block is alive
		currentBlockState = getBlockState(bunkerNumber, blockNum);
		if (currentBlockState < 4) {
			//If it's within the horizontal range
			if (position.x >= (bunkerPosition.x+(blockNum-8)*12) && position.x <= (bunkerPosition.x + (12*(blockNum-7)))) {
				//If it's also within the vertical range
				if (position.y > (bunkerPosition.y+24) && position.y < (bunkerPosition.y + 36)) {
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
