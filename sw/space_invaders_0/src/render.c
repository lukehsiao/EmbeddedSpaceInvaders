/**
 * This implements render.h
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#define FRAME_BUFFER_ADDR 0xC0000000
#include "render.h"

/**
 * Draws the tank in it's current position
 */
void renderTank() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;

	int col;
	int row;
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 32; col++) {
			framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = getTankPixel(row, col); //Green
		}
	}
}

void renderBunker(u8 bunkerNumber){
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	int blockNum;
	for (blockNum = 0; blockNum < 4; blockNum++) {
		for (row = 0; row < 12; row++) {
			for (col = 0; col < 12; col++) {
				framePointer0[(getBunkerPosition(bunkerNumber).y + row)*640 + (getBunkerPosition(bunkerNumber).x+(blockNum*12)+col)] = getBunkerPixel(row, col, bunkerNumber, blockNum); //Green
			}
		}
	}
	for (blockNum = 4; blockNum < 8; blockNum++) {
		for (row = 0; row < 12; row++) {
			for (col = 0; col < 12; col++) {
				framePointer0[(getBunkerPosition(bunkerNumber).y + 12 + row)*640 + (getBunkerPosition(bunkerNumber).x+((blockNum-4)*12)+col)] = getBunkerPixel(row, col, bunkerNumber, blockNum); //Green
			}
		}
	}
	for (blockNum = 8; blockNum < 12; blockNum++) {
		for (row = 0; row < 12; row++) {
			for (col = 0; col < 12; col++) {
				framePointer0[(getBunkerPosition(bunkerNumber).y + 24 + row)*640 + (getBunkerPosition(bunkerNumber).x+((blockNum-8)*12)+col)] = getBunkerPixel(row, col, bunkerNumber, blockNum); //Green
			}
		}
	}
}


/**
 * Deletes only the necessary pixels when the aliens are moving right or left.
 * Then, blanks the entire block when the aliens drop a row.
 */
void unrenderAliens() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	point_t position;
	position = getAlienBlockPosition();
	static u8 direction;
	direction = getAlienDirection();

	//If moving right..
	if(direction == 1){
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < X_SHIFT; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
			}
		}
	}
	// if X_SHIFT > 6, we'll need to add logic hear to clear the right side as well.

	// If we hit the right edge
	if(position.x >= (640-32*11) && direction == 1) {
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < 32*11; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
			}
		}
	} // Alien Block hit left side
	else if(position.x <= 5 && direction == 0) {
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < 32*11; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
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

	// Alien Block hit Right side
	if(tempAlien.x >= (640-32*11) && direction == 1) {
		tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
		setAlienDirection(0);
	} // Alien Block hit left side
	else if(tempAlien.x <= 5 && direction == 0) {
		tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
		setAlienDirection(1);
	}
	else {
		if(direction == 1){
			tempAlien.x = tempAlien.x + X_SHIFT;
		}
		else {
			tempAlien.x = tempAlien.x - X_SHIFT;
			if(tempAlien.x > 640) {
				tempAlien.x = 0;
			}
		}
	}
	if(tempAlien.y > 480/3*2+16*4) {
		tempAlien.y = 50;
	}
	setAlienBlockPosition(tempAlien);
}

/**
 * Writes a black box in the given Alien's location
 */
void killAlien(u32 alienNumber) {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	point_t position = getAlienBlockPosition();
	int col;
	int row;
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
 */
void renderAliens() {
	updateAlienLocation();
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	int alienNumber;
	point_t position = getAlienBlockPosition();
	toggleAlienGuise();
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
				//if (getAlienStatus(alienNumber) == 1) {//if the alien is alive, otherwise do nothing.
				u8 pixelPresent = (arrayToRender[row] >> (31-col)) & 0x1;
				framePointer0[(position.y + row)*640 + (position.x + col)] = pixelPresent? WHITE : BLACK;
				//}
			}
		}
	}
}

void unrenderAlienBullet() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	alienBullet bullet;
	u8 bulletNum;
	u32 row, col;

	for (bulletNum = 0; bulletNum < 4; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		if (bullet.position.y < 480) { //if it's not off the screen
			for (row = 0; row < ALIEN_BULLET_HEIGHT; row++) {
				for (col = 0; col < ALIEN_BULLET_WIDTH; col++) {
					framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = BLACK;
				}
			}
		}
	}
}

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

void renderAlienBullet() {
	updateAlienBulletPosition();
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
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
					framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = pixelPresent? WHITE : BLACK;;
				}
			}
		}
	}
}


void unrenderTankBullet() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	point_t position = getTankBulletPosition();
	u32 row;
	u32 col;
	for (row = 0; row < TANK_BULLET_HEIGHT; row++) {
		for (col = 0; col < TANK_BULLET_WIDTH; col++) {
			framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
		}
	}
}

void updateTankBulletPosition() {
	unrenderTankBullet();
	point_t position = getTankBulletPosition();
	if (position.y < 480) {
		position.y = position.y - TANK_BULLET_SPEED;
	}
	setTankBulletPosition(position);
}

void renderTankBullet() {
	updateTankBulletPosition();
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	point_t position = getTankBulletPosition();
	u32 row;
	u32 col;
	for (row = 0; row < TANK_BULLET_HEIGHT; row++) {
		for (col = 0; col < TANK_BULLET_WIDTH; col++) {
			framePointer0[(position.y + row)*640 + (position.x + col)] = WHITE;
		}
	}
}

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
		tankBullet.y = tankPosition.y;
		tankBullet.x = tankPosition.x + 15;	//center on turret
		setTankBulletPosition(tankBullet);
	}
	renderTankBullet();
	renderTank();	//to compensate for automatic single shift.
}

/**
 * Fires an alien bullet if available from a random bottom row alien
 */
void fireAlienBullet() {
	alienBullet bullet;
	u8 bulletNum;
	for (bulletNum = 0; bulletNum < 4; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		if (bullet.position.y < 480) {	//if a bullet is available
			//find random bottom row alien and fire from his location

			return;
		}
	}
}

/**
 * Writes Black to the entire screen
 */
void blankScreen() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int row=0, col=0;
	for( row=0; row<480; row++) {
		for(col=0; col<640; col++) {
			framePointer0[row*640 + col] = BLACK;  // frame 0 is red here.
		}
	}
}

/**
 * Writes BLACK to the box that the tank is currently in
 */
void unrenderTank() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int row, col;
	point_t position = getTankPositionGlobal();
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col=0; col<32; col++) {
			framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;  // frame 0 is red here.
		}
	}
}

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
			renderAliens();
			break;
		case '2':
			setAlienStatus(userInput, 0); //kill the random alien
			killAlien(userInput);
			break;
		case '5':
			//fire bullet
			fireTankBullet();
			break;
		case '3':
			//fire random alien missile
			random = ((timerSeed * 13) / 3) % 11; //pseudo random number between 0-10
			renderAlienBullet();
			break;
		case '9':
			renderTankBullet();
			renderAlienBullet();
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

/**
 * Renders all of the objects on the screen
 */
void render() {
	//blankScreen();
	renderTank();
	renderBunker(0);
	renderBunker(1);
	renderBunker(2);
	renderBunker(3);
	renderAliens();
	renderTankBullet();
	renderAlienBullet();
}

void unrender() {
	// Unrender tanks
	//unrenderAliens();
	// unrender aliens
	// unrender bullets
}

