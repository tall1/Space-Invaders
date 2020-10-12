#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
//Tal Lev
//312495955
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "SI.h"

/***** DEFINES *****/

/******************* Static Function Prototypes *******************/
//Fires a bullet from the spaceship
void fireBullet(Spaceship* spaceShip, Bullets* bullets);
//Moves the bullets
unsigned int moveBullets(Bullets* bullets, Aliens* aliens);
//Returns true if the bullet hit an alien, removes the alien.
bool isThereAHit(Bullet* cur_bullet, Aliens* aliens);
//Removes an alien from a list
void removeAlienFromAliensList(Alien* cur_alien, Alien* prev_alien, Aliens* aliens);
//Moves the spaceship
void moveSpaceship(Spaceship* spaceShip, char ch);
//Moves the aliens
bool moveAliens(Aliens* aliens, unsigned int vertical, unsigned int horizontal);
//Prints the spaceship
void printSpaceship(Spaceship* spaceship, char symbol);
//Prints the aliens
void printAliens(Aliens* aliens, char symbol);
//Prints the bullets
void printBullets(Bullets* bullets, char symbol);
//Prints the frame
void printFrame();
//Creates A new spaceship
Spaceship* initNewSpaceShip();
//Initializes the spaceship cells
SpaceshipCell* initNewSpaceShipcell(unsigned int row, unsigned int col);
//Initializes the aliens list
Aliens* initAliens();
//Initializes an alien
Alien* initAlien(unsigned int row, unsigned int col);
//Initializes bullets list
Bullets* initBulletList();
//Frees the game objects
void freeGame(Spaceship* spaceShip, Aliens* aliens);
//Frees spaceship
void freeSpaceShip(Spaceship* spaceShip);
//Frees Aliens
void freeAliens(Aliens* aliens);
//Frees a bullet from head (Pop)
void freeBulletFromHead(Bullets* bullets);
//Frees a given bullet
void freeBullet(Bullets* bullets, Bullet* bullet_to_remove);
//Prints score
void printScore(unsigned int score);
//Prints end message
void endGame();
//Prints victory message
void victory();
//Delays the program
void sleep(float secs);
// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);
// Turn escape codes ON.
void init(void);
//Checks if allocation was successful.
void checkAllocationSuccess(void* ptr);
/******************* Function Implementation *******************/

void runGame() {
	init();
	printFrame();
	char ch;
	bool condition = true;
	float time = 0.03;
	unsigned int count1 = 1, count2 = 1, count3 = 1, direction = -1, score = 0;
	Spaceship* spaceShip = initNewSpaceShip();
	Aliens* aliens = initAliens();
	Bullets* bullets = initBulletList();
	printSpaceship(spaceShip, SPACESHIPCELL);
	printAliens(aliens, ALIEN);
	while (aliens->head) {//Every loop is one step	
		if (count1 == 4 ) {
			score +=moveBullets(bullets, aliens);
			printScore(score);
			if (score == ALIENROWS * ALIENCOLS) {
				victory();
				break;
			}
			count1 = 1;
		}
		if (count2 == 70) {
			moveAliens(aliens, 0, direction * 2);
			printAliens(aliens, ALIEN);
			count2 = 1;
			count3++;
		}
		if (count3 == 5) {
			condition = moveAliens(aliens, 1, 0);
			printAliens(aliens, ALIEN);
			direction *= -1;
			count3 = 1;
		}
		if (!condition) {
			endGame();
			break;
		}
		printBullets(bullets, BULLETSYMBOL);
		sleep(time);
		if (_kbhit()) {
			ch = _getch();
			if (ch == FIRE)
				fireBullet(spaceShip, bullets);
			else if (ch == MOVELEFT || ch == MOVERIGHT) {
				moveSpaceship(spaceShip, ch);
				printSpaceship(spaceShip, SPACESHIPCELL);
			}
		}
		count1++;
		count2++;
	}
 	freeGame(spaceShip, aliens);
	sleep(4);
}
void fireBullet(Spaceship* spaceShip, Bullets* bullets) {
	Bullet* new_bullet = (Bullet*)malloc(sizeof(Bullet));
	checkAllocationSuccess(new_bullet);
	new_bullet->row = spaceShip->head->row - 1;
	new_bullet->col = spaceShip->head->col;
	new_bullet->next = NULL;
	if (bullets->head)
		bullets->tail->next = new_bullet;
	else
		bullets->head = new_bullet;

	bullets->tail = new_bullet;
	
}
void moveSpaceship(Spaceship* spaceShip, char ch) {
	//MAKE SURE DOESN'T GO OFF BORDERS
	printSpaceship(spaceShip, SPACE);
	unsigned int steps_direction = (ch == MOVERIGHT) ? 1 : -1;
	SpaceshipCell* cur_cell = spaceShip->head;
	if ((ch == MOVERIGHT && cur_cell->col == COLS - 4) || (ch == MOVELEFT && cur_cell->col == 4))
		return;
	while (cur_cell) {
		cur_cell->col += steps_direction;
		cur_cell = cur_cell->next;
	}
}
bool moveAliens(Aliens* aliens, unsigned int vertical, unsigned int horizontal) {
	printAliens(aliens, SPACE);
	Alien* cur_alien = aliens->head;
	while (cur_alien) {
		cur_alien->row += vertical;
		cur_alien->col += horizontal;
		if (cur_alien->row == ROWS - 4)
			return false;
		cur_alien = cur_alien->next;
	}
	return true;
}
unsigned int moveBullets(Bullets* bullets, Aliens* aliens) {
	printBullets(bullets, SPACE);
	Bullet* cur_bullet = bullets->head, * keep;
	unsigned int score = 0;
	while (cur_bullet) {
		cur_bullet->row--;
		if (isThereAHit(cur_bullet, aliens)) {
			keep = cur_bullet->next;
			freeBullet(bullets, cur_bullet);
			cur_bullet = keep;
			score++;
		}
		else if (cur_bullet->row == 1) {
			freeBulletFromHead(bullets);
			cur_bullet = bullets->head;
		}
		else
			cur_bullet = cur_bullet->next;
	}
	return score;
}
bool isThereAHit(Bullet* cur_bullet, Aliens* aliens) {
	Alien* cur_alien = aliens->head, * prev_alien = NULL;
	while (cur_alien) {
		if (cur_alien->row == cur_bullet->row && cur_alien->col == cur_bullet->col) {
			gotoxy(cur_alien->row, cur_alien->col);
			printf(" ");
			removeAlienFromAliensList(cur_alien, prev_alien, aliens);
			return true;
		}
		prev_alien = cur_alien;
		cur_alien = cur_alien->next;
	}
	return false;
}
void removeAlienFromAliensList(Alien* cur_alien, Alien* prev_alien, Aliens* aliens) {
	if (prev_alien)
		prev_alien->next = cur_alien->next;
	else
		aliens->head = cur_alien->next;
	free(cur_alien);
}
void printSpaceship(Spaceship* spaceship, char symbol) {
	SpaceshipCell* curr_cell = spaceship->head;
	gotoxy(curr_cell->row, curr_cell->col);
	//printf(" ");
	while (curr_cell!= NULL) {
		gotoxy(curr_cell->row, curr_cell->col);
		printf("%c", symbol);
		curr_cell = curr_cell->next;
	}
	gotoxy(ROWS + 1, 1);
}
void printAliens(Aliens* aliens, char symbol) {
	SpaceshipCell* curr_alien = aliens->head;
	gotoxy(curr_alien->row, curr_alien->col);
	printf(" ");
	while (curr_alien != NULL) {
		gotoxy(curr_alien->row, curr_alien->col);
		printf("%c", symbol);
		curr_alien = curr_alien->next;
	}
	gotoxy(ROWS + 1, 1);
}
void printBullets(Bullets* bullets, char symbol) {
	Bullet* curr_bullet= bullets->head;
	while (curr_bullet != NULL) {
		gotoxy(curr_bullet->row, curr_bullet->col);
		printf("%c", symbol);
		curr_bullet = curr_bullet->next;
	}
	gotoxy(ROWS + 1, 1);
}
void printFrame() {
	for (int i = 1; i <= COLS; i++) {
		gotoxy(1, i);
		printf("%c", FRAMESYMBOL);
		gotoxy(ROWS, i);
		printf("%c", FRAMESYMBOL);
		if (i <= ROWS) {
			gotoxy(i, 1);
			printf("%c", FRAMESYMBOL);
			gotoxy(i, COLS);
			printf("%c", FRAMESYMBOL);
		}
	}
}

Spaceship* initNewSpaceShip()
{
	Spaceship* spaceShip = (Spaceship*)malloc(sizeof(Spaceship));
	checkAllocationSuccess(spaceShip);
	spaceShip->head = initNewSpaceShipcell(ROWS - 3, COLS / 2);
	SpaceshipCell* cur_cell = spaceShip->head;
	int i, j, count = 1;
	for (i = 0; i < 3; i++){
		for (j = 0; j < count; j++) {
			if (j == count - 1) {
				if (count == 5)
					break;
				cur_cell->next = initNewSpaceShipcell(cur_cell->row + 1, cur_cell->col - count);
			}
			else
				cur_cell->next = initNewSpaceShipcell(cur_cell->row, cur_cell->col + 1);
			cur_cell = cur_cell->next;
		}
		count += 2;
	}
	return spaceShip;
}
SpaceshipCell* initNewSpaceShipcell(unsigned int row, unsigned int col) {
	SpaceshipCell* cell = (SpaceshipCell*)malloc(sizeof(SpaceshipCell));
	checkAllocationSuccess(cell);
	cell->row = row;
	cell->col = col;
	cell->next = NULL;
	return cell;
}
Aliens* initAliens() {
	Aliens* aliens = (Aliens*)malloc(sizeof(Aliens));
	checkAllocationSuccess(aliens);
	aliens->head = initAlien(FIRSTALIENROW, FIRSTALIENCOL);
	Alien* cur_alien = aliens->head;
	int i, j, keep = FIRSTALIENCOL, offset = SPACEBETWEENALIENCOLS / 2;
	for (i = 0; i < ALIENROWS; i++) {
		for (j = 0; j < ALIENCOLS; j++) {
			if (j == ALIENCOLS - 1) {
				if (i == ALIENROWS - 1)
					break;
				cur_alien->next = initAlien(cur_alien->row + SPACEBETWEENALIENROWS, keep + offset);
			}
			else
				cur_alien->next = initAlien(cur_alien->row, cur_alien->col + SPACEBETWEENALIENCOLS);
			cur_alien = cur_alien->next;
		}
		keep = keep + offset;
		offset *= (-1);
	}
	return aliens;
}
Alien* initAlien(unsigned int row, unsigned int col) {
	Alien* alien = (Alien*)malloc(sizeof(Alien));
	checkAllocationSuccess(alien);
	alien->row = row;
	alien->col = col;
	alien->next = NULL;
	return alien;
}
Bullets* initBulletList() {
	Bullets* bullets = (Bullets*)malloc(sizeof(Bullets));
	checkAllocationSuccess(bullets);
	bullets->head = bullets->tail = NULL;
	return bullets;
}

void freeGame(Spaceship* spaceShip, Aliens* aliens) {
	freeSpaceShip(spaceShip);
	freeAliens(aliens);
}
void freeSpaceShip(Spaceship* spaceShip) {
	SpaceshipCell* cur_cell = spaceShip->head, *next_cell;
	while (cur_cell) {
		next_cell = cur_cell->next;
		free(cur_cell);
		cur_cell = next_cell;
	}
	free(spaceShip);
}
void freeAliens(Aliens* aliens) {
	Alien* cur_alien = aliens->head, * next_alien;
	while (cur_alien) {
		next_alien = cur_alien->next;
		free(cur_alien);
		cur_alien = next_alien;
	}
	free(aliens);
}
void freeBulletFromHead(Bullets* bullets) {
	Bullet* keep = bullets->head->next;
	free(bullets->head);
	if (!keep)
		bullets->head = bullets->tail = NULL;
	else
		bullets->head = keep;
}
void freeBullet(Bullets* bullets,Bullet* bullet_to_remove) {
	Bullet* cur_bullet = bullets->head, * prev_bullet = NULL;
	while (cur_bullet) {
		if (cur_bullet == bullet_to_remove) {
			if (cur_bullet == bullets->head)
				freeBulletFromHead(bullets);
			else {
				prev_bullet->next = cur_bullet->next;
				free(cur_bullet);
			}
			return;
		}
		else {
			prev_bullet = cur_bullet;
			cur_bullet = cur_bullet->next;
		}
	}
}

void printScore(unsigned int score) {
	gotoxy(ROWS + 1, 1);
	printf("SCORE : %d / %d\n", score, ALIENROWS * ALIENCOLS);
}
void endGame() {
	gotoxy(ROWS / 2, COLS / 4);
	printf("G A M E  -  O V E R");
	gotoxy(ROWS+1, 0);
}
void victory() {
	gotoxy(ROWS / 2, COLS / 4);
	printf("!!!! V  I  C  T  O  R  Y !!!!");
	gotoxy(ROWS + 1, 0);
}
void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs * CLOCKS_PER_SEC)
		;
}
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}
void init(void) {
	system("cls");
}
void checkAllocationSuccess(void* ptr)
{
	if (!ptr)
	{
		printf("\aAllocation Error.");
		exit(ERROR);
	}
}
