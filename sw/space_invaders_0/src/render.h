/**
 * This interface will provide a structured way to handle rendering the general
 * game elements like score, lives, blanking the screen.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#ifndef RENDER_H_
#define RENDER_H_
#include "globals.h"		//to give access to the global variables
#include "aliens.h"
#include "tank.h"
#include "bunkers.h"
#include "spaceship.h"

#define STARTING_LIVES 3
#define DEFAULT_SCORE_POSITION_X 5
#define DEFAULT_SCORE_POSITION_Y 1

#define STARTING_SCORE_POSITION_X 80
#define STARTING_SCORE_POSITION_Y 11
#define SCORE_WIDTH 12

#define STARTING_LIVES_TEXT_POSITION_X 350
#define STARTING_LIVES_TEXT_POSITION_Y 11
#define LIVES_TEST_WIDTH 32

#define STARTING_LIVES_POSITION_X 420
#define STARTING_LIVES_POSITION_Y 5
#define LIVES_TEST_WIDTH 42

void blankScreen();
void renderBottomLine();
void renderScoreText();
void renderScore();
void renderLivesText();
void unrenderLives();
void renderLives();
void render();
void renderGameOverText();

//Specifically for Lab 3.
void parseKey(u8 keyPressed, u32 timerSeed, u32 userInput);

#endif /* RENDER_H_ */
