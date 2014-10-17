/**
 * Contains all the prototypes for rendering the spaceship.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#define SPACESHIP_START_RATE 100000000
#define EXTRA_WAIT 25
#define SPACESHIP_SPEED 1
#define SPACESHIP_FLASH_SCORE_CYCLES 2
#define SPACESHIP_SCORE_COUNT 10
#define SPACESHIP_STEADY_SCORE_CYCLES 40

/**
 * Draws the Spaceship and updates it's location
 */
void renderSpaceShip();

/**
 * Call this to put the spaceship in motion
 */
void startSpaceShip();

/**
 *  Renders the number of points passed in at the location of the spaceship
 *
 * @param points The number of points to display
 * @param position The top-left corner of where to draw the points.
 */
void renderPoints(u32 points, point_t position);

/**
 * Blanks the spot where the score is to help with flashing
 *
 * @param position The top-left position of where to start erasing.
 */
void unrenderPoints(point_t position);

/**
 * Calculates whether the spaceship was hit or not.
 *
 * @param bulletPosition The position to test.
 * @return 1 = hit, 0 = not hit
 */
u8 hitSpaceShip(point_t bulletPosition);

#endif /* SPACESHIP_H_ */
