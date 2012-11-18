#ifndef GENERATION_H
#define GENERATION_H

#include "Map.h"

class Generation
{
public:
	static bool mountain(int height, int width, Map *map)
	{
		int mWidth = width*32;
		int tempW = 0;
		for(int y = 0; y > -height*32; y-=32){
			for(int x = -mWidth; x < mWidth; x+=32){
				map->addBlock(x,y,1);
				tempW = x;
			};
			tempW < mWidth/2 ? mWidth = mWidth-3 : mWidth = mWidth-2; 
		};
		return true;
	};
};

#endif