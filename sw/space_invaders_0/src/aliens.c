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

u32 lowestLiveRow = STARTING_LOWEST_ALIEN_ALIVE_ROW;

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
	u32 totalWidth = ALIEN_ARRAY_WIDTH*MAX_ALIEN_NUMBER_IN_ROW;
	u32 totalHeight = (ALIEN_HEIGHT+ALIEN_RIGHT_SIDE_BLANK)*MAX_ALIEN_NUMBER_IN_COLUMN;
	// If we hit the right edge
	if(position.x + totalWidth >= getRightPad() && direction == 1) {
		for (row = 0; row < totalHeight; row++) {
			for (col = 0; col < totalWidth; col++) {
				if (framePointer1[(position.y + row)*RIGHT_SIDE + (position.x + col)] == GREEN) {
					framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = GREEN;
				}
				else if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] != OFFWHITE){
					if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] != GREEN) {
						framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = BLACK;
					}
				}
			}
		}
	} // Alien Block hit left side
	else if(position.x + getLeftPad() <= MAX_ALIEN_NUMBER_IN_COLUMN && direction == 0) {
		for (row = 0; row < totalHeight; row++) {
			for (col = 0; col < totalWidth; col++) {
				if (framePointer1[(position.y + row)*RIGHT_SIDE + (position.x + col)] == GREEN) {
					framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = GREEN;
				}
				else if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] != OFFWHITE){
					if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] != GREEN) {
						framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = BLACK;
					}
				}
			}
		}
	}

	//If moving right..
	else if(direction == 1){
		for (row = 0; row < totalHeight; row++) {
			for (col = 0; col < X_SHIFT; col++) {
				if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x+col)] != GREEN) {
					if (framePointer1[(position.y + row)*RIGHT_SIDE + (position.x + col)] == GREEN) {
						framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = GREEN;
					}
					else if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] != OFFWHITE){
							framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = BLACK;
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
	// Trigger Alien Move Sound
	static int alienMoveSoundNumber = STARTING_ALIEN_SOUND_NUMBER;
	alienMoveSoundNumber++;
	setActive(alienMoveSoundNumber, ACTIVE);
	setCurrentSampleNum(alienMoveSoundNumber, 0);
	if (alienMoveSoundNumber == MAX_ALIEN_SOUND_NUMBER) {
		alienMoveSoundNumber = 0;
	}

	unrenderAliens();
	u8 direction = getAlienDirection();
	// Update Location each call
	point_t tempAlien = getAlienBlockPosition();

	//Right edge hit
	if (tempAlien.x + (ALIEN_ARRAY_WIDTH*MAX_ALIEN_NUMBER_IN_ROW) >= getRightPad() && direction == 1) {
		tempAlien.y = tempAlien.y + (ALIEN_HEIGHT);
		setAlienDirection(ALIEN_DIRECTION_LEFT);
	}
	//left edge hit
	else if (tempAlien.x + getLeftPad() <= MAX_ALIEN_NUMBER_IN_COLUMN && direction == 0) {
		tempAlien.y = tempAlien.y + (ALIEN_HEIGHT);
		setAlienDirection(ALIEN_DIRECTION_RIGHT);
	}
	else {
		if(direction == ALIEN_DIRECTION_RIGHT){
			tempAlien.x = tempAlien.x + X_SHIFT;
		}
		else {
			tempAlien.x = tempAlien.x - X_SHIFT;
		}
	}
	setAlienBlockPosition(tempAlien);
	// If aliens get below the bottom bunkers, end the game.
	if ((getAlienBlockPosition().y+((lowestLiveRow*(ALIEN_HEIGHT+ALIEN_RIGHT_SIDE_BLANK))+ALIEN_HEIGHT)) > BOTTOM_BORDER) {
		setGameOver(GAME_OVER);
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
	for(col = 0; col < MAX_ALIEN_NUMBER_IN_ROW; col++) {
		for (alienNumber = col; alienNumber < MAX_ALIEN_NUMBER; alienNumber += MAX_ALIEN_NUMBER_IN_ROW) {
			if (getAlienStatus(alienNumber) == ALIVE) {
				setLeftCol(alienNumber % MAX_ALIEN_NUMBER_IN_ROW);
				goto rightside;
			}
		}
	}

	rightside:
	// Iterate over all the right to left until a live alien is found.
	for(col = ALIEN_RIGHT_SIDE_BLANK; col >= 0; col--) {
		for (alienNumber = col; alienNumber < MAX_ALIEN_NUMBER; alienNumber += MAX_ALIEN_NUMBER_IN_ROW) {
			if (getAlienStatus(alienNumber) == ALIVE) {
				setRightCol(alienNumber % MAX_ALIEN_NUMBER_IN_ROW);
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
	position.y = position.y + (alienNumber/MAX_ALIEN_NUMBER_IN_ROW)*(ALIEN_HEIGHT + ALIEN_RIGHT_SIDE_BLANK);
	position.x = position.x + (alienNumber%MAX_ALIEN_NUMBER_IN_ROW)*ALIEN_ARRAY_WIDTH;
	const u32* arrayToRender;
	arrayToRender = getDeadAlienArray();
	for (row = 0; row < ALIEN_HEIGHT; row++) {
		for (col = 0; col < ALIEN_ARRAY_WIDTH; col++) {
			if (((arrayToRender[row] >> (LEFT_BIT-col)) & 0x1) == 1) {
				framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = OFFWHITE;	// We use OFFWHITE so it is not erased by the alien block's render
			}
			else {
				framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = BLACK;
			}
		}
	}
	// Decrement the number of aliens alive variable.
	setNumberAliensAlive(getNumberAliensAlive()-1);
	//If player kills all aliens
	if (getNumberAliensAlive() == DEAD) {
		u32 tempScore = getScore();
		initStateMachines();
		setScore(tempScore);
		renderScore();
		return;
	}

	// Adjust leftPad and rightPad if necessary:
	setAlienStatus(alienNumber, DEAD);
	adjustPadding();

	//Adjust score
	u32 score;
	score = getScore();
	if (alienNumber < MAX_ALIEN_NUMBER_IN_ROW) {	//40pts
		score += TOP_ROW_SCORE;
		setScore(score);
	}
	else if (alienNumber < MAX_ROW_THREE_ALIEN_NUMBER) {
		score += MIDDLE_ROW_SCORE;
		setScore(score);
	}
	else if (alienNumber < MAX_ALIEN_NUMBER) {
		score += ALIEN_RIGHT_SIDE_BLANK;
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
		for (col = 0; col < ALIEN_ARRAY_WIDTH; col++) {
			if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] == OFFWHITE) {
				framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = BLACK;
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
	for (alienNumber = 0; alienNumber < MAX_ALIEN_NUMBER; alienNumber++) {
		//algorithm to adjust x and y for drawing
		if (alienNumber != 0) {
			position.x = position.x + ALIEN_ARRAY_WIDTH;
			if (alienNumber % MAX_ALIEN_NUMBER_IN_ROW == 0) {	//if end of row is reached, increment
				position.y = position.y + (ALIEN_HEIGHT + ALIEN_RIGHT_SIDE_BLANK);
				position.x = getAlienBlockPosition().x;
			}
		}

		//Rendering each alien
		arrayToRender = getAlienArray(alienNumber);
		for (row = 0; row < (ALIEN_HEIGHT); row++) {
			for (col = 0; col < ALIEN_ARRAY_WIDTH; col++) {
				u8 pixelPresent = (arrayToRender[row] >> (LEFT_BIT-col)) & 0x1;
				if (pixelPresent) {
					framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = WHITE;
				}
				else {
					if (framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] == WHITE) {
						if (framePointer1[(position.y + row)*RIGHT_SIDE + (position.x + col)] == GREEN) {
							framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = GREEN;
						}
						else {
							framePointer0[(position.y + row)*RIGHT_SIDE + (position.x + col)] = BLACK;
						}
					}
				}
			}
		}
		if (getAlienStatus(alienNumber) == ALIVE) {
			lowestLiveRow = alienNumber/MAX_ALIEN_NUMBER_IN_ROW;
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

	for (bulletNum = 0; bulletNum < MAX_BULLET_NUM; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		arrayToRender = getAlienBulletArray(bullet.type);
		if (bullet.position.y < BOTTOM) { //if it's not off the screen
			for (row = 0; row < ALIEN_BULLET_HEIGHT; row++) {
				for (col = 0; col < ALIEN_BULLET_WIDTH; col++) {
					bulletGuise = bullet.guise;
					u32 tempCol = col + (bullet.guise * ALIEN_BULLET_WIDTH);	// to mask the guise in the bitmap
					u8 pixelPresent = (arrayToRender[row] >> (LEFT_BIT-tempCol)) & 0x1;
					// Only blank pixels, not the surrounding.
					if (pixelPresent) {
						if (framePointer1[(bullet.position.y + row)*RIGHT_SIDE + (bullet.position.x + col)] == GREEN) {
							framePointer0[(bullet.position.y + row)*RIGHT_SIDE + (bullet.position.x + col)] = GREEN;
						}
						else if (framePointer0[(bullet.position.y + row)*RIGHT_SIDE + (bullet.position.x + col)] != WHITE) {
							framePointer0[(bullet.position.y + row)*RIGHT_SIDE + (bullet.position.x + col)] = BLACK;
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

	for (bulletNum = 0; bulletNum < MAX_BULLET_NUM; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		if (bullet.position.y < BOTTOM) { //if it's not off the screen
			bullet.guise = bullet.guise++;
			if (bullet.guise > NUM_BULLET_GUISE) {
				bullet.guise = 0;
			}
			bullet.position.y = bullet.position.y + ALIEN_BULLET_SPEED;
			setAlienBullet(bullet, bulletNum);
			if (calculateAlienBulletHit(bullet)) {
				bullet.position.y = OFF_THE_SCREEN; //deactivate the bullet
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

	for (bulletNum = 0; bulletNum < MAX_BULLET_NUM; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		arrayToRender = getAlienBulletArray(bullet.type);
		if (bullet.position.y < BOTTOM) { //if it's not off the screen
			for (row = 0; row < ALIEN_BULLET_HEIGHT; row++) {
				for (col = 0; col < ALIEN_BULLET_WIDTH; col++) {
					bulletGuise = bullet.guise;
					u32 tempCol = col + (bullet.guise * ALIEN_BULLET_WIDTH);	// to mask the guise in the bitmap
					u8 pixelPresent = (arrayToRender[row] >> (LEFT_BIT-tempCol)) & 0x1;
					// Only draw pixels, not the black.
					if (pixelPresent) {
						framePointer0[(bullet.position.y + row)*RIGHT_SIDE + (bullet.position.x + col)] = OFFWHITE;
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
	bullet.position.x += ALIEN_BULLET_WIDTH/FIND_MIDDLE_BULLET;
	for (bunkerNum = 0; bunkerNum < MAX_BULLET_NUM; bunkerNum++){
		result = hitBunker(bullet.position, bunkerNum);
		if (result != BUNKER_DEAD) {
			return ALIVE;
		}
	}

	return hitTank(bullet.position);
}

/**
 * Calculates whether a tank bullet hit an alien
 *
 * @param position The position to test
 * @return The number of the alien hit, BUNKER_DEAD if none.
 */
u8 hitAlien(point_t position) {
	point_t alienBlockPosition;
	alienBlockPosition = getAlienBlockPosition();
	signed int alienNumber;	//so we can start from 54 and subtract w/o overflowing

	//If it's outside the alien block
	if (position.x < (alienBlockPosition.x + getLeftPad()) || position.y > (alienBlockPosition.y + ALIEN_ROWS*(ALIEN_HEIGHT+ALIEN_RIGHT_SIDE_BLANK))) {
		return BUNKER_DEAD;
	}
	else if (position.x > (alienBlockPosition.x + MAX_ALIEN_NUMBER_IN_ROW*ALIEN_ARRAY_WIDTH) || position.y < alienBlockPosition.y) {
		return BUNKER_DEAD;
	}
	else {
		//Calculate which alien it hit, if any
		for (alienNumber = MAX_ALIEN_NUMBER; alienNumber >= 0; alienNumber--) {
			//If the alien is alive
			if (getAlienStatus(alienNumber) == ALIVE) {
				point_t alienPosition;
				alienPosition.x = alienBlockPosition.x + ((alienNumber % MAX_ALIEN_NUMBER_IN_ROW)*ALIEN_ARRAY_WIDTH);
				alienPosition.y = alienBlockPosition.y + ((alienNumber/MAX_ALIEN_NUMBER_IN_ROW)*(ALIEN_HEIGHT+ALIEN_RIGHT_SIDE_BLANK));

				// If it's within the alien's block
				if ((position.x < (alienPosition.x + ALIEN_ACTUAL_WIDTH)) && (position.x > alienPosition.x)) {
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
		return BUNKER_DEAD;
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
	for (row = ALIEN_ROWS-1; row >= 0; row--) {
		alienNumber = MAX_ALIEN_NUMBER_IN_ROW*row + col;
		if (getAlienStatus(alienNumber) == ALIVE) {
			return alienNumber;
		}
	}
	//If no alien is alive in that row
	u32 check;
	for (check = 0; check < MAX_ALIEN_NUMBER_IN_ROW; check++) {
		col++;	//check next column
		if (col > ALIEN_RIGHT_SIDE_BLANK) {
			col = 0;
		}
		for (row = ALIEN_ROWS-1; row >= 0; row--) {
			alienNumber = MAX_ALIEN_NUMBER_IN_ROW*row + col;
			if (getAlienStatus(alienNumber) == ALIVE) {
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

	for (bulletNum = 0; bulletNum < MAX_BULLET_NUM; bulletNum++) {
		bullet = getAlienBullet(bulletNum);
		if (bullet.position.y >= BOTTOM) {	//if a bullet is available
			//find random bottom row alien and fire from his location
			alienNumber = findFiringAlien(randomCol);
			position.x = getAlienBlockPosition().x + ((alienNumber%MAX_ALIEN_NUMBER_IN_ROW)*ALIEN_ARRAY_WIDTH) + 9; //8 to center bullet
			position.y = getAlienBlockPosition().y + (alienNumber/MAX_ALIEN_NUMBER_IN_ROW)*(ALIEN_HEIGHT+ALIEN_RIGHT_SIDE_BLANK) + (ALIEN_HEIGHT);
			bullet.position = position;
			//If it's low enough on the screen
			if ((bullet.position.y + ALIEN_BULLET_HEIGHT) > BULLET_NEAR_BUNKERS) {
				calculateAlienBulletHit(bullet);
			}
			bullet.type = (randomCol % NUM_BULLET_GUISE) & 0x1;
			bullet.guise = 0;
			setAlienBullet(bullet, bulletNum);
			if(!getGameOver()){ // these were causing a flash of Aliens after the game ends
				renderAlienBullet(DEAD);
				renderAliens(WITHOUT_MOVING); //redraw aliens to mask overlap
			}
			return;
		}
	}
}
