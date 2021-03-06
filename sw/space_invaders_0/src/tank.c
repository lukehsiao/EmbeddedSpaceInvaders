/**
 * Implements tank.h and contains the bitmaps for the tank.
 *
 *  @author Luke Hsiao
 *  @author Jeff Revert
 *  @date 10 Oct 2014
 */

#include "globals.h"
#include "render.h"
#include "spaceInvadersSounds.h"

extern u32* framePointer0;
u8 tankLife; // 1 is alive. 0 is dead

/**
 * Draws the tank in it's current position
 */
void renderTank() {
	u32 col;
	u32 row;
	const u32* arrayToRender;
	arrayToRender = getTankArray();
	point_t tempPosition = getTankPositionGlobal();
	tempPosition.x -= TANK_MOVEMENT_SPEED;
	for (row = 0; row < 16; row++) {
		for (col = 0; col < TANK_MOVEMENT_SPEED; col++) {
			if (framePointer0[(tempPosition.y+row)*640 + (tempPosition.x+col)] == GREEN) {
				framePointer0[(tempPosition.y+row)*640 + (tempPosition.x+col)] = BLACK;
			}
		}
	}
	
	// Draw extra black around the edges of the tank so unrenderTank() doesn't
	// need to be called.	
	tempPosition.x += TANK_MOVEMENT_SPEED;
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 32; col++) {
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(tempPosition.y+row)*640 + (tempPosition.x+col)] = GREEN;
			}
			else {
				framePointer0[(tempPosition.y+row)*640 + (tempPosition.x+col)] = BLACK;
			}
		}
	}

	tempPosition.x += 32;
	for (row = 0; row < 16; row++) {
		for (col = 0; col < TANK_MOVEMENT_SPEED; col++) {
			if (framePointer0[(tempPosition.y+row)*640 + (tempPosition.x+col)] == GREEN) {
				framePointer0[(tempPosition.y+row)*640 + (tempPosition.x+col)] = BLACK;
			}
		}
	}
}


/**
 * Writes BLACK to the box that the tank is currently in.
 */
void unrenderTank() {
	u32 row, col;
	point_t position = getTankPositionGlobal();
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col=0; col<32; col++) {
			if (framePointer0[(position.y + row)*640 + (position.x + col)] == GREEN) {
				framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
			}
		}
	}
}

/**
 * Renders the 1st Dead Tank Bitmap at the tank's current location
 */
void deathTank1() {
	u32 row, col;
	const u32* arrayToRender;
	arrayToRender = getDeathTankArray1();
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 32; col++) {
			// If there is a pixel present
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = GREEN;
			}
			else {
				framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = BLACK;
			}
		}
	}
}

/**
 * Renders the 2nd Dead Tank Bitmap at the tank's current location
 */
void deathTank2() {
	u32 row, col;
	const u32* arrayToRender;
	arrayToRender = getDeathTankArray2();
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 32; col++) {
			// If there is a pixel present
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = GREEN;
			}
			else {
				framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = BLACK;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////
// Functions for Rendering Tank Bullets
//////////////////////////////////////////////////////////////////

/**
 *  Erases the tank bullet at it's current position
 */
void unrenderTankBullet() {
	point_t position = getTankBulletPosition();
	u32 row;
	u32 col;
	if (position.y < 480) {
		for (row = 0; row < TANK_BULLET_HEIGHT; row++) {
			for (col = 0; col < TANK_BULLET_WIDTH; col++) {
				if (framePointer0[(position.y + row)*640 + (position.x + col)] != WHITE) {
					framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
				}
			}
		}
	}
}

/**
 *  Updates the tank's bullet position
 */
void updateTankBulletPosition() {
	unrenderTankBullet();
	point_t position = getTankBulletPosition();
	if (position.y < 480) {
		position.y = position.y - TANK_BULLET_SPEED;
		//Don't let the tank bullet get up to the score/lives
		if ((position.y + TANK_BULLET_HEIGHT) < 35) {
			position.y = 8888;	// just a number to signify it's off the screen
		}
		setTankBulletPosition(position);
		if (calculateTankBulletHit()) {
			position.y = 800; //deactivate the tank bullet
			setTankBulletPosition(position);
		}
	}
}

/**
 *  Draws the tank's bullet position
 *  @param animate 1=update the position 0=refresh only
 */
void renderTankBullet(u8 animate) {
	if (animate) {
		updateTankBulletPosition();
	}
	point_t position = getTankBulletPosition();
	u32 row;
	u32 col;
	if (position.y < 480) {
		for (row = 0; row < TANK_BULLET_HEIGHT; row++) {
			for (col = 0; col < TANK_BULLET_WIDTH; col++) {
				//Don't draw the tank bullet above the line
				if ((position.y + row) > 35) {
					framePointer0[(position.y + row)*640 + (position.x + col)] = OFFWHITE;
				}
			}
		}
	}
}

/**
 * Calculates whether an alien bullet hit the tank
 *
 * @param position The position to test
 * @return 1 if hit, 0 if not
 */
u8 hitTank(point_t position) {
	point_t tankPosition;
	tankPosition = getTankPositionGlobal();

	//If it's outside the tank
	if (position.x < tankPosition.x || position.y < tankPosition.y) {
		return 0;
	}
	else if (position.x > (tankPosition.x+32) || position.y > (tankPosition.y+16)) {
		return 0;
	}
	else {
		// Trigger Tank Death Sound
		setActive(TANK_DEATH_NUM, ACTIVE);

		// Otherwise, it must be in the tank.
		tankLife = 0;
		return 1;
	}
}


//////////////////////////////////////////////////////////////////
// Functions for Shooting
//////////////////////////////////////////////////////////////////

/**
 * Fires the tank bullet from the tank's current position.
 */
void fireTankBullet() {
	//fire bullet
	point_t tankBullet;
	point_t tankPosition;
	tankBullet = getTankBulletPosition();
	tankPosition = getTankPositionGlobal();
	if (tankBullet.y > 490) {	//if it's not on the screen
		tankBullet.y = tankPosition.y - TANK_BULLET_HEIGHT;
		tankBullet.x = tankPosition.x + 15;	//center on turret
		setTankBulletPosition(tankBullet);

		// Trigger Tank Bullet Sound
		setActive(TANK_FIRE_NUM, ACTIVE);
		setCurrentSampleNum(TANK_FIRE_NUM, 0);
	}
	renderTankBullet(0);
}

/**
 * Moves the tank to the x-coordinate position
 * @param position The x coordinate where to place the tank.
 */
void moveTank(u32 position) {
	point_t temp;
	unrenderTank();	//Clear the dead tank
	temp.x = position;
	if (temp.x > (640-32)) {
		temp.x = 0;
	}
	setTankPositionGlobal(temp.x);
	renderTank();
}

/**
 * Moves the tank left.
 */
void moveTankLeft() {
	point_t temp;
	temp = getTankPositionGlobal();
	temp.x = temp.x - TANK_MOVEMENT_SPEED;
	if (temp.x > (640-32)) {
		temp.x = 0;
	}
	setTankPositionGlobal(temp.x);
	renderTank();
}

/**
 * Moves the tank right.
 */
void moveTankRight() {
	point_t temp;
	temp = getTankPositionGlobal();
	temp.x = temp.x + TANK_MOVEMENT_SPEED;
	if (temp.x > (640-32)) {
		temp.x = (640-32);
	}
	setTankPositionGlobal(temp.x);
	renderTank();
}

/**
 * Calculates whether the tank bullet hit the bunkers, an alien, or
 * the spaceship.
 *
 * @return 1=hit, 0=no hit
 */
u8 calculateTankBulletHit() {
	u32 bunkerNum;
	u8 result;
	point_t position = getTankBulletPosition();
	position.x += TANK_BULLET_WIDTH/2;
	for (bunkerNum = 0; bunkerNum < 4; bunkerNum++){
		result = hitBunker(position, bunkerNum);
		if (result != 0xFF) {
			return 1;
		}
	}

	//Check if aliens are hit
	if (hitAlien(position) != 0xFF) {
		return 1;
	}

	//Check if SpaceShip is hit
	if (hitSpaceShip(position) == 1) {
		return 1;
	}

	return 0;
}

void setTankLife(u8 val) {
	tankLife = val;
}
u8 getTankLife() {
	return tankLife;
}

