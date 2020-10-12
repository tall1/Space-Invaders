#ifndef ___SI___
#define ___SI___

/***** DEFINES *****/
#define ERROR -1
#define ROWS 25
#define COLS 95
#define FIRSTALIENROW 3
#define FIRSTALIENCOL 15
#define SPACEBETWEENALIENROWS 2
#define SPACEBETWEENALIENCOLS 8

#define ALIENROWS 4
#define ALIENCOLS 10

#define MOVERIGHT 'd'
#define MOVELEFT 'a'
#define FIRE ' '

#define SPACE ' '
#define SPACESHIPCELL '+'
#define ALIEN '@'
#define BULLETSYMBOL '*'
#define FRAMESYMBOL '#'

/***** TYPEDEFS *****/
typedef struct _Aliens {
	struct _Alien* head;
	struct _Alien* tail;
	unsigned int count;
}Aliens;

typedef struct _Alien {
	unsigned int row;
	unsigned int col;
	struct _Alien* next;
}Alien;

typedef struct _SpaceshipCell {
	unsigned int row;
	unsigned int col;
	struct _SpaceshipCell* next;
}SpaceshipCell;

typedef struct _Spaceship {
	SpaceshipCell* head;
}Spaceship;
typedef struct _Bullet {
	unsigned int row;
	unsigned int col;
	struct _Bullet* next;
}Bullet;

typedef struct _Bullets {
	Bullet* head;
	Bullet* tail;
}Bullets;

/******************* Function Prototypes *******************/
//Runs the game
void runGame();

#endif // !___SI___
