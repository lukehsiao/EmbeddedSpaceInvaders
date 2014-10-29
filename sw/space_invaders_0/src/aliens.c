/*
 * Implements aliens.h and contains the bitmaps for the aliens.
 *
 *  @author Luke Hsiao
 *  @author Jeff Revert
 *  @date 10 Oct 2014
 */

#include "stateMachines.h"
#include "render.h"
#include "spaceInvadersSounds.h"

extern u32* framePointer0;
extern u32* framePointer1;

u32 findFiringAlien(u32 randomCol);

u32 lowestLiveRow = 4;

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
	u32 totalWidth = 32*11;
	u32 totalHeight = (ALIEN_HEIGHT+10)*5;
	// If we hit the right edge
	if(position.x + totalWidth >= getRightPad() && direction == 1) {
		for (row = 0; row < totalHeight; row++) {
			for (col = 0; col < totalWidth; col++) {
				if (framePointer1[(position.y + row)*640 + (position.x + col)] == GREEN) {
					framePointer0[(position.y + row)*640 + (position.x + col)] = GREEN;
				}
				else if (framePointer0[(position.y + row)*640 + (position.x + col)] != OFFWHITE){
						framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
				}
			}
		}
	} // Alien Block hit left side
	else if(position.x + getLeftPad() <= 5 && direction == 0) {
		for (row = 0; row < totalHeight; row++) {
			for (col = 0; col < totalWidth; col++) {
				if (framePointer1[(position.y + row)*640 + (position.x + col)] == GREEN) {
					framePointer0[(position.y + row)*640 + (position.x + col)] = GREEN;
				}
				else if (framePointer0[(position.y + row)*640 + (position.x + col)] != OFFWHITE){
						framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
				}
			}
		}
	}

	//If moving right..
	else if(direction == 1){
		for (row = 0; row < totalHeight; row++) {
			for (col = 0; col < X_SHIFT; col++) {
				if (framePointer0[(position.y + row)*640 + (position.x+col)] != GREEN) {
					if (framePointer1[(position.y + row)*640 + (position.x + col)] == GREEN) {
						framePointer0[(position.y + row)*640 + (position.x + col)] = GREEN;
					}
					else if (framePointer0[(position.y + row)*640 + (position.x + col)] != OFFWHITE){
							framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
					}
				}
			}
		}
	}
	// if X_SHIFT > 6, we'll need to add logic here to clear the right side as well when moving left.
	// Currently, the gap on the right side of the bitmaps themselves takes care of it sufficiently.

}

/**
 * Contains the algorithm to move the aliens left and right, as well as drop
 * rows when they hit the edge of the screen.
 */
void updateAlienLocation() {

	int i;
	int soundNum;
	int sample=0;
	while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){//for(i = 0; i < NUM_FIFO_SAMPLES_FILL; i++){
		sample=0;
		for(soundNum = 0; soundNum < SOUND_NUM; soundNum++){
			sample += getCurrentSample(soundNum);
		}
		int totalActive = getTotalActive();
		if(totalActive > 0){
			sample = sample / getTotalActive();
		}
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample | (sample<<16));
	}
	// Trigger Alien Move Sound
	static int alienMoveSoundNumber = 1;
	alienMoveSoundNumber++;
	setActive(alienMoveSoundNumber, ACTIVE);
	setCurrentSampleNum(alienMoveSoundNumber, 0);
	if (alienMoveSoundNumber == 4) {
		alienMoveSoundNumber = 0;
	}

	unrenderAliens();
	u8 direction = getAlienDirection();
	// Update Location each call
	point_t tempAlien = getAlienBlockPosition();

	//Right edge hit
	if (tempAlien.x + (32*11) >= getRightPad() && direction == 1) {
		tempAlien.y = tempAlien.y + (ALIEN_HEIGHT);
		setAlienDirection(0);
	}
	//left edge hit
	else if (tempAlien.x + getLeftPad() <= 5 && direction == 0) {
		tempAlien.y = tempAlien.y + (ALIEN_HEIGHT);
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
	// If aliens get below the bottom bunkers, end the game.
	if ((getAlienBlockPosition().y+((lowestLiveRow*(ALIEN_HEIGHT+10))+ALIEN_HEIGHT)) > BOTTOM_BORDER) {
		setGameOver(1);
		blankScreen();
		renderGameOverText();
	}
}

/**
 * If the killed alien is on an edge column, traverses up to see if the
 * column is empty and adjusts the global variables so the edge-to-edge
 * movement is possible.
 */
void adjustPadding() {
	u32 col, alienNumber;
	u8 flag;
	flag = 0;
	// Iterate over all the columns from left to right until a live alien is found.
	for(col = 0; col < 11; col++) {
		for (alienNumber = col; alienNumber < 55; alienNumber += 11) {
			if (getAlienStatus(alienNumber) == 1) {
				setLeftCol(alienNumber % 11);
				goto rightside;
			}
		}
	}

	rightside:
	// Iterate over all the right to left until a live alien is found.
	for(col = 10; col >= 0; col--) {
		for (alienNumber = col; alienNumber < 55; alienNumber += 11) {
			if (getAlienStatus(alienNumber) == 1) {
				setRightCol(alienNumber % 11);
				return;
			}
		}
	}
	return;
}


/**
 * Draws the initial death guise for the alien at the specified number and
 * updates it's status.
 */
void killAlien(u8 alienNumber) {
	point_t position = getAlienBlockPosition();
	u32 col;
	u32 row;
	//Adjust X and Y location
	position.y = position.y + (alienNumber/11)*(ALIEN_HEIGHT + 10);
	position.x = position.x + (alienNumber%11)*32;
	const u32* arrayToRender;
	arrayToRender = getDeadAlienArray();
	for (row = 0; row < ALIEN_HEIGHT; row++) {
		for (col = 0; col < 32; col++) {
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(position.y + row)*640 + (position.x + col)] = OFFWHITE;	// We use OFFWHITE so it is not erased by the alien block's render
			}
			else {
				framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
			}
		}
	}
	// Decrement the number of aliens alive variable.
	setNumberAliensAlive(getNumberAliensAlive()-1);
	//If player kills all aliens
	if (getNumberAliensAlive() == 0) {
		u32 tempScore = getScore();
		initStateMachines();
		setScore(tempScore);
		renderScore();
		return;
	}

	// Adjust leftPad and rightPad if necessary:
	setAlienStatus(alienNumber, 0);
	adjustPadding();

	//Adjust score
	u32 score;
	score = getScore();
	if (alienNumber < 11) {	//40pts
		score += 40;
		setScore(score);
	}
	else if (alienNumber < 33) {
		score += 20;
		setScore(score);
	}
	else if (alienNumber < 55) {
		score += 10;
		setScore(score);
	}
	// Raise Black Death Flag!!!!
	setAlienDeath(1);
	// Save the point_t of the explosion
	setAlienExplosionPosition(position);
	renderScore();
}

/**
 * Erases the Death Guise at the position specified.
 */
void unrenderDeadAlien(point_t position) {
	u32 row, col;
	for (row = 0; row < ALIEN_HEIGHT; row++) {
		for (col = 0; col < 32; col++) {
			if (framePointer0[(position.y + row)*640 + (position.x + col)] == OFFWHITE) {
				framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
			}
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

		// Fill the Audio FIFO halfway through our aliens
		if(alienNumber%5 == 0)
			fillFIFO();

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
		for (row = 0; row < (ALIEN_HEIGHT); row++) {
			for (col = 0; col < 32; col++) {
				u8 pixelPresent = (arrayToRender[row] >> (31-col)) & 0x1;
				if (pixelPresent) {
					framePointer0[(position.y + row)*640 + (position.x + col)] = WHITE;
				}
				else {
					if (framePointer0[(position.y + row)*640 + (position.x + col)] == WHITE) {
						if (framePointer1[(position.y + row)*640 + (position.x + col)] == GREEN) {
							framePointer0[(position.y + row)*640 + (position.x + col)] = GREEN;
						}
						else {
							framePointer0[(position.y + row)*640 + (position.x + col)] = BLACK;
						}
					}
				}
			}
		}
		if (getAlienStatus(alienNumber) == 1) {
			lowestLiveRow = alienNumber/11;
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
						if (framePointer1[(bullet.position.y + row)*640 + (bullet.position.x + col)] == GREEN) {
							framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = GREEN;
						}
						else if (framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] != WHITE) {
							framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = BLACK;
						}
					}
				}
			}
		}
	}
}

/**
 *  Updates the alien bullet locations and checks if any of them hit an object
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
			if (calculateAlienBulletHit(bullet)) {
				bullet.position.y = 800; //deactivate the bullet
				setAlienBullet(bullet, bulletNum);
			}
		}
	}
}

/**
 *  Draws all the alien bullets. We use OFFWHITE to eliminate the flashing of the bullets
 *  when they are flying inside the alien block.
 *
 *  @param animate 1=update position, 0=refresh only
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
						framePointer0[(bullet.position.y + row)*640 + (bullet.position.x + col)] = OFFWHITE;
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
// Functions for Calculating Intersections
//////////////////////////////////////////////////////////////////

/**
 * Calculates whether the bullet hit the tank or the bunkers.  This
 * will update global flags so that we can transition to death animations
 *
 * @param bullet The Alien Bullet to evaluate
 * @return 1=hit, 0=no hit
 */
u8 calculateAlienBulletHit(alienBullet bullet) {
	u32 bunkerNum;
	u8 result;
	bullet.position.y += ALIEN_BULLET_HEIGHT;
	bullet.position.x += ALIEN_BULLET_WIDTH/2;
	for (bunkerNum = 0; bunkerNum < 4; bunkerNum++){
		result = hitBunker(bullet.position, bunkerNum);
		if (result != 0xFF) {
			return 1;
		}
	}

	return hitTank(bullet.position);
}

/**
 * Calculates whether a tank bullet hit an alien
 *
 * @param position The position to test
 * @return The number of the alien hit, 0xFF if none.
 */
u8 hitAlien(point_t position) {
	point_t alienBlockPosition;
	alienBlockPosition = getAlienBlockPosition();
	signed int alienNumber;	//so we can start from 54 and subtract w/o overflowing

	//If it's outside the alien block
	if (position.x < (alienBlockPosition.x + getLeftPad()) || position.y > (alienBlockPosition.y + 5*(ALIEN_HEIGHT+10))) {
		return 0xFF;
	}
	else if (position.x > (alienBlockPosition.x + 11*32) || position.y < alienBlockPosition.y) {
		return 0xFF;
	}
	else {
		//Calculate which alien it hit, if any
		for (alienNumber = 55; alienNumber >= 0; alienNumber--) {
			//If the alien is alive
			if (getAlienStatus(alienNumber) == 1) {
				point_t alienPosition;
				alienPosition.x = alienBlockPosition.x + ((alienNumber % 11)*32);
				alienPosition.y = alienBlockPosition.y + ((alienNumber/11)*(ALIEN_HEIGHT+10));

				// If it's within the alien's block
				if ((position.x < (alienPosition.x + 26)) && (position.x > alienPosition.x)) {
					if ((position.y < (alienPosition.y + ALIEN_HEIGHT)) && (position.y > alienPosition.y)) {
						// Start sound for alien death
						setActive(ALIEN_DEATH_NUM, ACTIVE);

						killAlien(alienNumber);
						return alienNumber;
					}
				}
			}
		}
		//No aliens hit
		return 0xFF;
	}
}

//////////////////////////////////////////////////////////////////
// Functions for Shooting
//////////////////////////////////////////////////////////////////

/**
 * Searches up the column to find the lowest live alien.  If there are
 * none, iterate over the other columns.
 * @return the number of the alien to fire the bullet from
 */
u32 findFiringAlien(u32 randomCol) {
	u32 alienNumber;
	alienNumber = 0;
	//Check up the column first
	int row, col;	// chose a signed int in this case rather than a u32 so it wouldn't wrap.
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
			//If it's low enough on the screen
			if ((bullet.position.y + ALIEN_BULLET_HEIGHT) > 379) {
				calculateAlienBulletHit(bullet);
			}
			bullet.type = (randomCol % 2) & 0x1;
			bullet.guise = 0;
			setAlienBullet(bullet, bulletNum);
			if(!getGameOver()){ // these were causing a flash of Aliens after the game ends
				renderAlienBullet(0);
				renderAliens(0); //redraw aliens to mask overlap
			}
			return;
		}
	}
}
