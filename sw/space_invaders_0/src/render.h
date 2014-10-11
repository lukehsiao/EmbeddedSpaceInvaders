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

void blankScreen();
void renderBottomLine();
void renderScoreText();
void renderScore();
void renderLivesText();
void renderLives();
void render();

//Specifically for Lab 3.
void parseKey(u8 keyPressed, u32 timerSeed, u32 userInput);

#endif /* RENDER_H_ */
