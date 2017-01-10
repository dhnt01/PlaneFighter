// Make a simple GBA state machine
// Name: HUNG DO

#include "enemy1.h"
#include "enemy2.h"
#include "myLib.h"
#include "gameboy.h"
#include "gameover.h"
#include "playing.h"
#include "lives.h"
#include "fighter.h"
#include <stdio.h>
#include <stdlib.h>
#define MAXENEMYSIZE 18

ENEMY enemy[MAXENEMYSIZE];
SHIP ship;
MISSLE shipMissle[100];
// MISSLE enemyMissle[enemySIZE];

// State enum definition
enum GBAState {
	START,
	START_NODRAW,
	STATE1,
	STATE2,
	GAMEOVER,
};

int main() {

	REG_DISPCTL = MODE_3 | BG2_EN;

	enum GBAState state = START;

	char buffer[2];
	int score = 0;
	int count = 1;
	int misslecount = 0;
	int missleSpeed = 10;
	int wait = 0;
	int change =  0;
	int lives = 5;
	int delay = 0;
	int enemySIZE = 18;

	ship.row = 130;
	ship.col = 113;

	for (int i = 0; i < 100; i++) {
		shipMissle[i].row = ship.row;
		shipMissle[i].col = ship.col + 8;
		shipMissle[i].isActive = 0;
	}

	for (int i = 0; i < MAXENEMYSIZE; i++) {
		if (i < 10) {
			enemy[i].row = 5;
			enemy[i].col = 5 + i*20;
		} else if (i < 16) {
			enemy[i].row = 20;
			enemy[i].col = -155 + i*20;
		} else if (i < 18) {
			enemy[i].row = 35;
			enemy[i].col = -235 + i*20;
		}
		enemy[i].isKilled = 0;
		// enemyMissle[i].row = enemy[i].row + 14;
		// enemyMissle[i].col = enemy[i].col + 7;
		// enemyMissle[i].isActive = 0;
	}

	int cdel = 1;

	while(1) {
		waitForVBlank();
		switch(state) {


		case START:
			if (change < 30) {
				drawScreen(0, gameboy_data);
				change++;
			} else {
				drawString(85, 87, "PRESS START", ORANGE);
				drawString(100, 100, "(ENTER)", BLACK);
				change++;
				if (change >= 60) {
					change = 0;
				}
			}
			state = START_NODRAW;
			break;


		case START_NODRAW:

			if (KEY_DOWN_NOW(BUTTON_START) && wait == 0) {
				count++;
				if (count > 4) {
					count = 1;
				}
				wait = 1;
			}
			if (count == 1) {
				state = START;
			} else if (count == 2) {
				fillScreen(BLACK);
				state = STATE1;
				break;
			} else if (count == 3) {
				state = STATE2;
				break;
			} else if (count == 4) {
				state = GAMEOVER;
				break;
			}

			break;




		// First Level
		case STATE1:

			enemySIZE = 16;
			drawString(150, 100, "Level-1", GREEN);
			drawString(150, 40, buffer, BLACK);
			sprintf(buffer, "%d", score);
			drawString(150, 2, "SCORE:", GREEN);
			drawString(150, 40, buffer, ORANGE);

			// for (int i = 1; i <= lives; i++) {
			// 	drawObject(150, 180 + i*10, 10, 10, lives_data);
			// }
			for (int i = 0; i < enemySIZE; i++) {
				if (enemy[i].isKilled == 0) {
					if (i < 10) {
						drawObject(enemy[i].row, enemy[i].col, 14, 14, enemy2_data);
					} else if (i >= 10 && i < 16) {
						drawObject(enemy[i].row, enemy[i].col, 14, 14, enemy1_data);
					} else {
						drawObject(enemy[i].row, enemy[i].col, 14, 14, enemy2_data);
					}
				}
				// enemyMissle[i].row = enemy[i].row + 14;
				// enemyMissle[i].col = enemy[i].col + 7;
			}


			if (enemy[0].col <= 0 || enemy[0].col >= 45) {
				cdel = -cdel;
			}
			for (int i = 0; i < enemySIZE; i++) {
				drawRect(enemy[i].row, enemy[i].col, 14, 14, BLACK);
				if (delay % 3 == 0) {
					enemy[i].col += cdel;
				}
				if (enemy[i].isKilled == 0) {
					if (i < 10) {
						drawObject(enemy[i].row, enemy[i].col, 14, 14, enemy2_data);
					} else if (i >= 10 && i < 16) {
						drawObject(enemy[i].row, enemy[i].col, 14, 14, enemy1_data);
					} else {
						drawObject(enemy[i].row, enemy[i].col, 14, 14, enemy2_data);
					}
				}

			}

			drawObject(ship.row, ship.col, 15, 15, fighter_data);
			if (KEY_DOWN_NOW(BUTTON_LEFT)) {
				drawRect(ship.row, ship.col, 15, 15, BLACK);
				ship.col -= 1;
				if (ship.col <= 0) {
					ship.col = 0;
				}
				drawObject(ship.row, ship.col, 15, 15, fighter_data);
			}
			if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
				drawRect(ship.row, ship.col, 15, 15, BLACK);
				ship.col += 1;
				if (ship.col >= 225) {
					ship.col = 225;
				}
				drawObject(ship.row, ship.col, 15, 15, fighter_data);
			}
			if (KEY_DOWN_NOW(BUTTON_UP)) {
				drawRect(ship.row, ship.col, 15, 15, BLACK);
				ship.row -= 1;
				if (ship.row <= 0) {
					ship.row = 0;
				}
				drawObject(ship.row, ship.col, 15, 15, fighter_data);
			}
			if (KEY_DOWN_NOW(BUTTON_DOWN)) {
				drawRect(ship.row, ship.col, 15, 15, BLACK);
				ship.row += 1;
				if (ship.row >= 130) {
					ship.row = 130;
				}
				drawObject(ship.row, ship.col, 15, 15, fighter_data);
			}
			if (KEY_DOWN_NOW(BUTTON_A)) {
				if (missleSpeed > 30) {
					shipMissle[misslecount].isActive = 1;
					shipMissle[misslecount].row = ship.row - 3;
					shipMissle[misslecount].col = ship.col + 7;
					misslecount++;
					missleSpeed = 0;
				}
			}

			for (int i = 0; i < 100; i++) {
				if (shipMissle[i].isActive == 1) {
					drawRect(shipMissle[i].row, shipMissle[i].col, 5, 1, BLACK);
					drawRect(shipMissle[i].row, shipMissle[i].col, 3, 1, RED);
					for (int j = 0; j < enemySIZE; j++) {
						if (collisionENEMY(enemy[j], shipMissle[i]) == 1 && enemy[j].isKilled == 0) {
							enemy[j].isKilled = 1;
							shipMissle[i].isActive = 0;
							score += 10;
							drawRect(shipMissle[i].row, shipMissle[i].col, 5, 1, BLACK);
						}
					}
					shipMissle[i].row -= 2;
					if (shipMissle[i].row == -2) {
						shipMissle[i].isActive = 0;
						drawRect(shipMissle[i].row, shipMissle[i].col, 5, 1, BLACK);
					}
				}
			}


			state = STATE1;

			break;



		// Second Level
		case STATE2:
			drawScreen(0, playing_data);
			drawString(150, 100, "Level-2", GREEN);
			drawString(150, 2, "Score: 00", GREEN);
			drawObject(ship.row, ship.col, 15, 15, fighter_data);
			for (int i = 1; i <= lives; i++) {
				drawObject(150, 180 + i*10, 10, 10, lives_data);
			}
			state = START_NODRAW;
			break;




		// Gameover
		case GAMEOVER:
			if (change < 30) {
				drawScreen(0, gameover_data);
				change++;
			} else {
				drawScreen(0, gameover_data);
				drawString(95, 87, "PRESS START", ORANGE);
				drawString(110, 100, "ENTER", WHITE);
				change++;
				if (change >= 60) {
					change = 0;
				}
			}
			state = START_NODRAW;
			break;

		}


		if (!KEY_DOWN_NOW(BUTTON_START) && wait == 1) {
			wait = 0;
		}

		missleSpeed++;
		if (misslecount >= 99) {
			misslecount = 0;
		}
		delay++;
		if (delay >= 100) {
			delay = 0;
		}
	}

	return 0;
}
