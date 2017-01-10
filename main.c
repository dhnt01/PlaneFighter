// Make a simple GBA state machine
// Name: HUNG DO

#include "startscreen.h"
#include "winscreen.h"
#include "enemy3.h"
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
MISSLE enemyMissle[200];

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

	char buffer1[1];
	char buffer2[1];
	char buffer3[1];
	int score = 0;
	int mode = 0;
	int missleShipcount = 0;
	int missilEnemycount = 0;
	int enemyShootSpeed = 0;
	int shipShootSpeed = 10;
	int waitSTART = 0;
	int waitSELECTE = 0;
	// int waitShoot = 0;
	int change = 0;
	int level = 1;
	int times = 0;
	int lives = 5;

	int delays = 0;
	int enemySIZE = 18;
	int enemyActive = 0;
	int numKill = 0;

	int cdel = 1;

	while(1) {
		waitForVBlank();



		//
		//
		// Gameboy screen
		//
		//
		if (mode == 0) {
			score = 0;
			lives = 5;
			level = 1;
			numKill = 0;
			if (change < 30) {
				drawScreen(0, gameboy_data);
				drawString(100, 100, "(ENTER)", BLACK);
				change++;
			} else {
				drawString(85, 87, "PRESS START", ORANGE);
				change++;
				if (change >= 60) {
					change = 0;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_START) && waitSTART == 0) {
				fillScreen(BLACK);
				mode = 6;
				waitSTART = 1;
				drawScreen(0, startscreen_data);
			}
		}



		//
		//
		// Playing screen
		//
		//
		if (mode == 1) {

			drawString(150, 163, buffer1, BLACK);
			sprintf(buffer1, "%d", level);
			drawString(150, 125, "Level-", GREEN);
			drawString(150, 163, buffer1, ORANGE);

			drawString(150, 40, buffer2, BLACK);
			sprintf(buffer2, "%d", score);
			drawString(150, 2, "SCORE:", GREEN);
			drawString(150, 40, buffer2, ORANGE);

			drawString(150, 100, buffer3, BLACK);
			sprintf(buffer3, "%d", times);
			drawString(150, 70, "TIME:", GREEN);
			drawString(150, 100, buffer3, ORANGE);

			for (int i = 1; i <= lives; i++) {
				drawObject(148, 175 + i*10, 10, 10, lives_data);
			}

			// for (int i = 0; i < enemySIZE; i++) {
			// 	if (enemy[i].isKilled == 0) {
			// 		drawObject(enemy[i].row, enemy[i].col, 15, 15, enemy3_data);
			// 	}
			// }

			// drawObject(ship.row, ship.col, 15, 15, fighter_data);



			int random = rand();

			// Shoot missil from the enemies
			if (enemyShootSpeed > 60 && enemy[random % enemySIZE].isKilled == 0) {
				enemyMissle[missilEnemycount].isActive = 1;
				enemyMissle[missilEnemycount].row = enemy[random % enemySIZE].row + 15;
				enemyMissle[missilEnemycount].col = enemy[random % enemySIZE].col + 7;
				missilEnemycount++;
				enemyShootSpeed = 0;
			}

			for (int i = 0; i < 200; i++) {
				if (enemyMissle[i].isActive == 1) {
					drawRect(enemyMissle[i].row, enemyMissle[i].col, 3, 2, BLACK);
					// collision go here
					if (collisionSHIP(ship, enemyMissle[i]) == 1) {
						for (int i = 1; i <= lives; i++) {
							drawObject(148, 180 + i*10, 10, 10, BLACK);
						}
						lives--;
						enemyMissle[i].isActive = 0;
					}

					enemyMissle[i].row += 1;
					if (enemyMissle[i].row == 165) {
						enemyMissle[i].isActive = 0;
					}
				}
				if (enemyMissle[i].isActive == 1) {
					drawRect(enemyMissle[i].row, enemyMissle[i].col, 3, 2, WHITE);
				}
			}

			// Shoot missile from the ship
			if (KEY_DOWN_NOW(BUTTON_A)) {
				if (shipShootSpeed > 20) {
					shipMissle[missleShipcount].isActive = 1;
					shipMissle[missleShipcount].row = ship.row - 3;
					shipMissle[missleShipcount].col = ship.col + 7;
					missleShipcount++;
					shipShootSpeed = 0;
				}
				// waitShoot = 1;
			}

			for (int i = 0; i < 100; i++) {

				if (shipMissle[i].isActive == 1) {
					drawRect(shipMissle[i].row, shipMissle[i].col, 3, 1, BLACK);
					for (int j = 0; j < enemySIZE; j++) {
						if (collisionENEMY(enemy[j], shipMissle[i]) == 1 && enemy[j].isKilled == 0) {
							enemy[j].isKilled = 1;
							drawRect(enemy[j].row, enemy[j].col, 15, 15, BLACK);
							shipMissle[i].isActive = 0;
							enemyActive--;
							score += 10;
							numKill++;
						}
					}
					shipMissle[i].row -= 3;
					if (shipMissle[i].row == -2) {
						shipMissle[i].isActive = 0;
					}
				}

				if (shipMissle[i].isActive == 1) {
					drawRect(shipMissle[i].row, shipMissle[i].col, 3, 1, YELLOW);
				}
			}

			// Draw enemy
			int startcol = enemy[0].col;
			if (startcol <= 1 || startcol >= 122) {
				cdel = -cdel;
			}
			for (int i = 0; i < enemySIZE; i++) {
				if (enemy[i].isKilled == 0) {
					drawRect(enemy[i].row, enemy[i].col, 15, 15, BLACK);
				}
				if (delays % 3 == 0) {
					enemy[i].col += cdel;
				}
				if (enemy[i].isKilled == 0) {
					drawObject(enemy[i].row, enemy[i].col, 15, 15, enemy3_data);
				}
			}


			// Draw the ship
			if (KEY_DOWN_NOW(BUTTON_LEFT)) {
				drawRect(ship.row, ship.col, 15, 15, BLACK);
				ship.col -= 1;
				if (ship.col <= 0) {
					ship.col = 0;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
				drawRect(ship.row, ship.col, 15, 15, BLACK);
				ship.col += 1;
				if (ship.col >= 225) {
					ship.col = 225;
				}
			}
			drawObject(ship.row, ship.col, 15, 15, fighter_data);
			// if (KEY_DOWN_NOW(BUTTON_UP)) {
			// 	drawRect(ship.row, ship.col, 15, 15, BLACK);
			// 	ship.row -= 1;
			// 	if (ship.row <= 70) {
			// 		ship.row = 70;
			// 	}
			// }
			// if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			// 	drawRect(ship.row, ship.col, 15, 15, BLACK);
			// 	ship.row += 1;
			// 	if (ship.row >= 130) {
			// 		ship.row = 130;
			// 	}
			// }


			if (delays % 30 == 0) {
				times++;
				if (times == 0) {
					for (int i = 1; i <= lives; i++) {
						drawObject(148, 180 + i*10, 10, 10, BLACK);
					}
					lives--;
					if (level == 1) {
						times = 60;
					} else if (level == 2) {
						times = 80;
					} else {
						times = 100;
					}
				}
			}

			if (lives == 0) {
				mode = 3;
			}

			if (enemyActive == 0) {
				delay(20);
				mode = 2;
				numKill = 0;
				level++;

				if (level > 1) {
					mode = 4;
				}
			}

			// Reset key
			if (KEY_DOWN_NOW(BUTTON_SELECT) && waitSELECTE == 0) {
				mode = 0;
				waitSELECTE = 1;
				score = 0;
				lives = 5;
				level = 1;
				numKill = 0;
			}

			// Pause key
			if (KEY_DOWN_NOW(BUTTON_START) && waitSTART == 0) {
				mode = 5;
				waitSTART = 1;
			}

		}




		//
		//
		// Set screen
		//
		//
		if (mode == 2) {
			mode = 1;
			missleShipcount = 0;
			shipShootSpeed = 0;
			change =  0;
			delays = 0;
			cdel = 1;


			ship.row = 130;
			ship.col = 113;

			for (int i = 0; i < 100; i++) {
				shipMissle[i].row = ship.row;
				shipMissle[i].col = ship.col + 8;
				shipMissle[i].isActive = 0;
			}

			for (int i = 0; i < MAXENEMYSIZE; i++) {
				if (i < 6) {
					enemy[i].row = 15;
					enemy[i].col = 35 + i*20;
				} else if (i < 12) {
					enemy[i].row = 35;
					enemy[i].col = -85 + i*20;
				} else if (i < 18) {
					enemy[i].row = 55;
					enemy[i].col = -205 + i*20;
				}
				enemy[i].isKilled = 0;
				if (i == 7 || i == 10) {
					enemy[i].isKilled = 1;
				}
			}

			if (level == 1) {
				enemySIZE = 18;
				enemyActive = 16;
			}
			// else if (level == 2) {
			// 	enemySIZE = 12;
			// 	enemyActive = 10;
			// } else {
			// 	enemySIZE = 18;
			// 	enemyActive = 16;
			// }

			fillScreen(BLACK);
		}




		//
		//
		// Lost screen
		//
		//
		if (mode == 3) {
			if (change < 30) {
				drawScreen(0, gameover_data);
				change++;
			} else {
				drawString(100, 87, "PRESS START", ORANGE);
				change++;
				if (change >= 60) {
					change = 0;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_START) && waitSTART == 0) {
				mode = 0;
				waitSTART = 1;
			}
		}




		//
		//
		// Win screen
		//
		//
		if (mode == 4) {
			if (change < 30) {
				drawScreen(0, winscreen_data);
				change++;
			} else {
				drawString(85, 87, "PRESS START", GREY);
				change++;
				if (change >= 60) {
					change = 0;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_START) && waitSTART == 0) {
				mode = 0;
				waitSTART = 1;
			}
		}




		//
		//
		// Pause screen
		//
		//
		if (mode == 5) {
			drawString(70, 100, "PAUSE", GREY);
			if (KEY_DOWN_NOW(BUTTON_START) && waitSTART == 0) {
				drawString(70, 100, "PAUSE", BLACK);
				mode = 1;
				waitSTART = 1;
			}
		}




		//
		//
		// Start screen
		//
		//
		if (mode == 6) {
			if (change < 30) {
				drawScreen(0, startscreen_data);
				drawString(105, 90, "(to start)", WHITE);
				change++;
			} else {
				drawString(85, 87, "PRESS START", YELLOW);
				change++;
				if (change >= 60) {
					change = 0;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_START) && waitSTART == 0) {
				mode = 2;
				waitSTART = 1;
			}
		}


		// if (!KEY_DOWN_NOW(BUTTON_A) && waitShoot == 1) {
		// 	waitShoot = 0;
		// }

		if (!KEY_DOWN_NOW(BUTTON_SELECT) && waitSELECTE == 1) {
			waitSELECTE = 0;
		}

		if (!KEY_DOWN_NOW(BUTTON_START) && waitSTART == 1) {
			waitSTART = 0;
		}


		shipShootSpeed++;
		if (missleShipcount >= 99) {
			missleShipcount = 0;
		}

		enemyShootSpeed++;
		if (missilEnemycount >= 199) {
			missilEnemycount = 0;
		}

		delays++;
		if (delays >= 100) {
			delays = 0;
		}

	}

	return 0;
}
