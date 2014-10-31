/**
 * Contains all the prototypes for rendering the bunkers.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */


#ifndef BUNKERS_H_
#define BUNKERS_H_

#define BUNKER_ZERO_POSITION_X 104
#define BUNKER_ONE_POSITION_X 232
#define BUNKER_TWO_POSITION_X 360
#define BUNKER_THREE_POSITION_X 488
#define ALL_BUNKER_POSITION_Y 380

#define TOTAL_BUNKER_NUM 4
#define BUNKERS_NOT_ERODED 0x00000000
#define BUNKER_MASK 0x3FFFFFFF
#define BUNKER_BLOCK_MASK 0x3FFFFFFF
#define BUNKER_MOST_SIGNIFICANT_BIT 30
#define TOP_THREE_ROW_BLOCKS 3
#define NUMBER_TOP_THREE_ROW_BLOCKS 9
#define NUMBER_BOTTOM_RIGHT_BLOCK 11
#define BOTTOM_RIGHT_BLOCK 27
#define DEAD_BLOCK 4

#define BUNKER_ZERO 0
#define BUNKER_ONE 1
#define BUNKER_TWO 2
#define BUNKER_THREE 3

#define BLOCK_ZERO 0
#define BLOCK_ONE 1
#define BLOCK_TWO 2
#define BLOCK_THREE 3
#define BLOCK_FOUR 4
#define BLOCK_FIVE 5
#define BLOCK_SIX 6
#define BLOCK_SEVEN 7
#define BLOCK_EIGHT 8
#define BLOCK_NINE 9
#define BLOCK_TEN 10
#define BLOCK_ELEVEN 11

#define BUNKER_DEAD 0xFF
#define EROSION_MASK 0xFFF00000
#define EROSION_BIT_SHIFT 12
#define BLOCK_WIDTH 12

/**
 * Draws the bunker
 * @param bunkerNumber The bunker to draw (0-3)
 */
void renderBunker(u8 bunkerNumber);

/**
 * Calculates whether the given position falls within the bunker.
 *
 * @param position the (x,y) position to test
 * @param bunkerNumber the number of the bunker to test
 * @return the block number that the position falls in, or 0xFF if it isn't hit
 */
u8 hitBunker(point_t position, u8 bunkerNumber);

#endif /* BUNKERS_H_ */
