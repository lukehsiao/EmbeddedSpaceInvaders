/**
 * Contains all the prototypes for rendering the tank.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef TANK_H_
#define TANK_H_

#define TANK_INIT_POSITION_X 70
#define TANK_INIT_POSITION_Y 440

#define TANK_MOVEMENT_SPEED 3    	// # of pixel the tank bullet moves per update
#define TANK_BULLET_SPEED 15    	// # of pixel the tank bullet moves per update
#define TANK_MAP_FLIP_COUNT 12		// how long one cycle is for the tank death bitmap flipping
#define TANK_MAP_FLIP_CYCLES 10 	// # of times the 2 bitmaps will show

/**
 * Draws the tank in it's current position
 */
void renderTank();

/**
 * Writes BLACK to the box that the tank is currently in
 */
void unrenderTank();

/**
 *  Draws the tank's bullet position
 *  @param animate 1=update the position 0=refresh only
 */
void renderTankBullet(u8 animate);

/**
 * Fires the tank bullet from the tank's current position.
 */
void fireTankBullet();

/**
 * Moves the tank to the x-coordinate position
 * @param position The x coordinate where to place the tank.
 */
void moveTank(u32 position);

/**
 * Moves the tank left.
 */
void moveTankLeft();

/**
 * Moves the tank right.
 */
void moveTankRight();

/**
 * Calculates whether an alien bullet hit the tank
 *
 * @param position The position to test
 * @return 1 if hit, 0 if not
 */
u8 hitTank(point_t position);

/**
 * Calculates whether the tank bullet hit the bunkers, an alien, or
 * the spaceship.
 *
 * @return 1=hit, 0=no hit
 */
u8 calculateTankBulletHit();

/**
 * Renders the 1st Dead Tank Bitmap at the tank's current location
 */
void deathTank1();

/**
 * Renders the 2nd Dead Tank Bitmap at the tank's current location
 */
void deathTank2();

void setTankLife(u8 val);
u8 getTankLife();

#endif /* TANK_H_ */
