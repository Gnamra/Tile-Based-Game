#ifndef EFFECTS_H
#define EFFECTS_H
#include "Map.h"

class Effect
{
public:
	static void effect01(int y, int x, Map *map)
	{
		map->addBlock(x, y, 0);
	};

	static void effect02(int y, int x, Map *map)
	{
		map->addBlock(x, y, Game::selectedTile);
	};
};

#endif