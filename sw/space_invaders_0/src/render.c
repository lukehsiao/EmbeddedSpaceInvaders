/**
 * This implements render.h
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "render.h"

u32* framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;

/**
 * Draws the tank in it's current position
 */
void renderTank() {
	u32 col;
	u32 row;
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 32; col++) {
			framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = getTankPixel(row, col); //Green
		}
	}
}

/**
 * Writes BLACK to the box that the tank is currently in
 */
void unrenderTank() {
	u32 row, col;
	point_t position = getTankPositionGlobal();
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col=0; col<32; col++) {
			framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;  // frame 0 is red here.
		}
	}
}

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
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum); //Green
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = GREEN;
					}
				}
			}
		}
	}
	for (blockNum = 4; blockNum < 8; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum); //Green
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = GREEN;
					}
				}
			}
		}
	}
	for (blockNum = 8; blockNum < 12; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum); //Green
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = GREEN;
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
// Functions for Rendering Aliens
//////////////////////////////////////////////////////////////////

/**
 * Deletes only the necessary pixels when the aliens are moving right or left.
 * Then, blanks the entire block when the aliens drop a row.
 */
void unrenderAliens() {
	u32 col;
	u32 row;
	point_t position;
	position = getAlienBlockPosition();
	static u8 direction;
	direction = getAlienDirection();

//	const u32* arrayToRender;
//	u32 alienNumber;
//	for (alienNumber = 0; alienNumber < 55; alienNumber++) {
//		//algorithm to adjust x and y for drawing
//		if (alienNumber != 0) {
//			position.x = position.x + 32;
//			if (alienNumber % 11 == 0) {	//if end of row is reached, increment
//				position.y = position.y + (ALIEN_HEIGHT + 10);
//				position.x = getAlienBlockPosition().x;
//			}
//		}
//
//		//Rendering each alien
//		arrayToRender = getAlienArray(alienNumber);
//		for (row = 0; row < ALIEN_HEIGHT; row++) {
//			for (col = 0; col < 32; col++) {
//				u8 pixelPresent = (arrayToRender[row] >> (31-col)) & 0x1;
//				if (pixelPresent) {
//					framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
//				}
//			}
//		}
//	}
	//If moving right..
	if(direction == 1){
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < X_SHIFT; col++) {
				if (framePointer0[(position.y + row)*640 + (position.x+col)] != GREEN) {
					framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
				}
			}
		}
	}
	// if X_SHIFT > 6, we'll need to add logic hear to clear the right side as well.

	// If we hit the right edge
	if(position.x + (32*11) >= getRightPad() && direction == 1) {
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < 32*11; col++) {
				if (framePointer0[(position.y + row)*640 + (position.x+col)] != GREEN) {
					framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
				}
			}
		}
	} // Alien Block hit left side
	else if(position.x + getLeftPad() <= 5 && direction == 0) {
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < 32*11; col++) {
				if (framePointer0[(position.y + row)*640 + (position.x+col)] != GREEN) {
					framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
				}
			}
		}
	}
}

/**
 * Contains the algorithm to move the aliens left and right, as well as drop
 * rows when they hit the edge of the screen.
 */
void updateAlienLocation() {
	unrenderAliens();
	u8 direction = getAlienDirection();
	// Update Location each call
	point_t tempAlien = getAlienBlockPosition();

	//Right edge hit
	if (tempAlien.x + (32*11) >= getRightPad() && direction == 1) {
		tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
		setAlienDirection(0);
	}
	//left edge hit
	else if (tempAlien.x + getLeftPad() <= 5 && direction == 0) {
		tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
		setAlienDirection(1);
	}
	else {
		if(direction == 1){
			tempAlien.x = tempAlien.x + X_SHIFT;
		}
		else {
			tempAlien.x = tempAlien.x - X_SHIFT;
		}
	}
	setAlienBlockPosition(tempAlien);
}

/**
 * Writes a black box in the given Alien's location
 */
void killAlien(u32 alienNumber) {
	point_t position = getAlienBlockPosition();
	u32 col;
	u32 row;
	//Adjust X and Y location
	position.y = position.y + (alienNumber/11)*(ALIEN_HEIGHT + 10);
	position.x = position.x + (alienNumber%11)*32;

	for (row = 0; row < ALIEN_HEIGHT; row++) {
		for (col = 0; col < 32; col++) {
			framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
		}
	}
}

/**
 *  Draws all the aliens.  Note that it will toggle the alien guise each time it is called
 *  @param animate 1 = toggle guise and move location, 0 = just refresh
 */
void renderAliens(u8 animate) {
	u32 col;
	u32 row;
	u32 alienNumber;
	if (animate) {
		updateAlienLocation();
		toggleAlienGuise();
	}
	point_t position = getAlienBlockPosition();

	const u32* arrayToRender;
	for (alienNumber = 0; alienNumber < 55; alienNumber++) {
		//algorithm to adjust x and y for drawing
		if (alienNumber != 0) {
			position.x = position.x + 32;
			if (alienNumber % 11 == 0) {	//if end of row is reached, increment
				position.y = position.y + (ALIEN_HEIGHT + 10);
				position.x = getAlienBlockPosition().x;
			}
		}

		//Rendering each alien
		arrayToRender = getAlienArray(alienNumber);
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				u8 pixelPresent = (arrayToRender[row] >> (31-col)) & 0x1;
				if (pixelPresent) {
					framePointer0[(position.y + row)*640 + (position.x + col)] = WHITE;
				}
				else {
					if (framePointer0[(position.y + row)*640 + (position.x + col)] == WHITE) {
						framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
					}
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////
// Functions for Rendering Alien Bullets
//////////////////////////////////////////////////////////////////
/**
 * Erases the alien bullet at it's current location
 */
void unrenderAlienBullet() {
	alienBullet bullet;
	u8 bulletNum;
	u32 row, col, bulletGuise;
	const u32* arrayToRender;

	for (bulletNum = 0; bulletNum < 4; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		arrayToRender = getAlienBulletArray(bullet.type);
		if (bullet.position.y < 480) { //if it's not off the screen
			for (row = 0; row < ALIEN_BULLET_HEIGHT; row++) {
				for (col = 0; col < ALIEN_BULLET_WIDTH; col++) {
					bulletGuise = bullet.guise;
					u32 tempCol = col + (bullet.guise * ALIEN_BULLET_WIDTH);	// to mask the guise in the bitmap
					u8 pixelPresent = (arrayToRender[row] >> (31-tempCol)) & 0x1;
					// Only blank pixels, not the surrounding.
					if (pixelPresent) {
						framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = BLACK;
					}
				}
			}
		}
	}
}

/**
 *  Updates the alien bullet locations
 */
void updateAlienBulletPosition() {
	unrenderAlienBullet();
	alienBullet bullet;
	u8 bulletNum;

	for (bulletNum = 0; bulletNum < 4; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		if (bullet.position.y < 480) { //if it's not off the screen
			bullet.guise = bullet.guise++;
			if (bullet.guise > 2) {
				bullet.guise = 0;
			}
			bullet.position.y = bullet.position.y + ALIEN_BULLET_SPEED;
			setAlienBullet(bullet, bulletNum);
		}
	}
}

/**
 *  Draws all the alien bullets.
 */
void renderAlienBullet(u8 animate) {
	if (animate) {
		updateAlienBulletPosition();
	}
	alienBullet bullet;
	u8 bulletNum;
	u32 row, col, bulletGuise;
	const u32* arrayToRender;

	for (bulletNum = 0; bulletNum < 4; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		arrayToRender = getAlienBulletArray(bullet.type);
		if (bullet.position.y < 480) { //if it's not off the screen
			for (row = 0; row < ALIEN_BULLET_HEIGHT; row++) {
				for (col = 0; col < ALIEN_BULLET_WIDTH; col++) {
					bulletGuise = bullet.guise;
					u32 tempCol = col + (bullet.guise * ALIEN_BULLET_WIDTH);	// to mask the guise in the bitmap
					u8 pixelPresent = (arrayToRender[row] >> (31-tempCol)) & 0x1;
					// Only draw pixels, not the black.
					if (pixelPresent) {
						framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = WHITE;
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
// Functions for Rendering Tank Bullets
//////////////////////////////////////////////////////////////////

/**
 *  Erases the tank at it's current position
 */
void unrenderTankBullet() {
	point_t position = getTankBulletPosition();
	u32 row;
	u32 col;
	if (position.y < 480) {
		for (row = 0; row < TANK_BULLET_HEIGHT; row++) {
			for (col = 0; col < TANK_BULLET_WIDTH; col++) {
				framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
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
		setTankBulletPosition(position);
	}
}

/**
 *  Draws the tank's bullet position
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
				framePointer0[(position.y + row)*640 + (position.x + col)] = WHITE;
			}
		}
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
	}
	renderTankBullet(0);
	//renderTank();	//to compensate for automatic single shift.
}

/**
 * Searches up the column to find the lowest live alien.  If there are
 * none, iterate over the other columns.
 * @return the number of the alien to fire the bullet from
 */
u32 findFiringAlien(u32 randomCol) {
	u32 alienNumber;
	alienNumber = 0;
	//Check up the column first
	u32 row, col;
	col = randomCol;
	for (row = 4; row >= 0; row--) {
		alienNumber = 11*row + col;
		if (getAlienStatus(alienNumber) == 1) {
			return alienNumber;
		}
	}

	//If no alien is alive in that row
	u32 check;
	for (check = 0; check < 11; check++) {
		col++;	//check next column
		if (col > 10) {
			col = 0;
		}
		for (row = 4; row >= 0; row--) {
			alienNumber = 11*row + col;
			if (getAlienStatus(alienNumber) == 1) {
				return alienNumber;
			}
		}
	}
	
	//Shouldn't reach here, all aliens are dead?
	return alienNumber;
}

/**
 * Fires an alien bullet if available from a random bottom row alien
 */
void fireAlienBullet(u32 randomCol) {
	alienBullet bullet;
	u8 bulletNum;
	point_t position;
	u32 alienNumber;

	for (bulletNum = 0; bulletNum < 4; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		if (bullet.position.y >= 480) {	//if a bullet is available
			//find random bottom row alien and fire from his location
			alienNumber = findFiringAlien(randomCol);
			position.x = getAlienBlockPosition().x + ((alienNumber%11)*32) + 9; //8 to center bullet
			position.y = getAlienBlockPosition().y + (alienNumber/11)*(ALIEN_HEIGHT+10) + (ALIEN_HEIGHT);
			bullet.position = position;
			bullet.type = (randomCol % 2) & 0x1;
			bullet.guise = 0;
			setAlienBullet(bullet, bulletNum);
			renderAlienBullet(0);
			//renderAliens(0); //redraw aliens to mask overlap
			return;
		}
	}
}

/**
 * Writes Black to the entire screen
 */
void blankScreen() {
	u32 row=0, col=0;
	for( row=0; row<480; row++) {
		for(col=0; col<640; col++) {
			framePointer0[row*640 + col] = BLACK;  // frame 0 is red here.
		}
	}
}

/**
 * Draws the green line at the bottom of the screen
 */
void renderBottomLine() {
	u32 col;
	u32 row;
	row = 479;
	for(col=0; col<640; col++) {
		framePointer0[row*640 + col] = GREEN;  // frame 0 is red here.
	}
}

/**
 * Renders all of the objects on the screen
 */
void render() {
	//blankScreen();
	renderTank();
	renderTankBullet(1);
	renderAlienBullet(1);
	renderAliens(1);
	renderBunker(0);
	renderBunker(1);
	renderBunker(2);
	renderBunker(3);
	renderBottomLine();
}

/**
 * Specifically for Lab 3.  Parses the keyboard inputs.
 */
void parseKey(u8 keyPressed, u32 timerSeed, u32 userInput) {
	point_t temp;
	u8 random;
	u8 blockNumber;
	u8 erosionState;
	switch (keyPressed) {
		case '4':
			unrenderTank();
			temp = getTankPositionGlobal();
			temp.x = temp.x - 4;
			if (temp.x > (640-32)) {
				temp.x = 0;
			}
			setTankPositionGlobal(temp.x);
			renderTank();
			break;
		case '6':
			unrenderTank();
			temp = getTankPositionGlobal();
			temp.x = temp.x + 4;
			if (temp.x > (640-32)) {
				temp.x = (640-32);
			}
			setTankPositionGlobal(temp.x);
			renderTank();
			break;
		case '8':
			renderAliens(1);
			break;
		case '2':
			setAlienStatus(userInput, 0); //kill the random alien
			killAlien(userInput);
			break;
		case '1':
			render();
			break;
		case '5':
			//fire bullet
			fireTankBullet();
			break;
		case '3':
			//fire random alien missile
			random = ((timerSeed * 13) / 3) % 11; //pseudo random number between 0-10
			fireAlienBullet(random);
			break;
		case '9':
			renderTankBullet(1);
			renderAlienBullet(1);
			break;
		case '7':
			for (blockNumber = 0; blockNumber < 12; blockNumber++) {
				erosionState = getBlockState(userInput, blockNumber);
				if (erosionState < 4) {
					setBlockState(userInput, blockNumber, erosionState+1);
					break;
				}
			}
			renderBunker(userInput);
			break;
		default:
			//do nothing
			break;
	}
}




