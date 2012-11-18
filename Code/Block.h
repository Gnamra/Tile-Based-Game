#ifndef BLOCK_H
#define BLOCK_H
#include <allegro5\allegro.h>

class Block
{
public:
	Block(){};
	Block(int newY, int newX, int Id, bool Solid, bool Harming) :
		y(newY),
		x(newX),
		id(Id),
		solid(Solid),
		harming(Harming) {};
	int id;
	int x, y;
	bool solid, harming;
};

#endif