/**
 * Contains all the prototypes for rendering the bunkers.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */


#ifndef BUNKERS_H_
#define BUNKERS_H_

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
