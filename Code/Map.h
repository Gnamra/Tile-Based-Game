#ifndef MAP_H
#define MAP_H
#include <vector>
#include "Block.h"
#include "Game.h"
#include <iostream>
class Map
{
public:
	Map(){};
	std::vector< std::vector <Block> > blockVector;

	void sortMap(){
		std::vector< std::vector <Block> > blockCopy = blockVector;
		int rows = blockCopy.size();
		int greatest = -9000;
		int smallest = 9000;
		for(int i = 0; i < rows; i++){
			for(std::vector<Block>::iterator it = blockCopy[i].begin(); it != blockCopy[i].end(); it++){
				if(it->y > greatest){
					greatest = it->y;
					swapBlock(i,0,i, (it - blockCopy[i].begin()));
				}
				else if (it->y < smallest){
					smallest = it->y;
					swapBlock(i,0,i, (it - blockCopy[i].begin()));
				};
			};
		}
		blockVector = blockCopy;
	};
	int getID(int y, int x){
		return blockVector[y][x].id;
	};
	int checkX(int y, int x){
		int i = findRowOfY(y);
		if(findRowOfY(y) == -1) return 2;
		for(int j = 0; j < blockVector[i].size(); j++){
			if(blockVector[i][j].x == x && blockVector[i][j].solid){
				return 0; // Collision
			}
			else if (blockVector[i][j].x == x && blockVector[i][j].harming){
				return 1; // Harming block was hit
			}	
			else if (blockVector[i][j].x == x && !blockVector[i][j].solid) return 2; // non solid block was hit
		};
		return 2;
	};


	bool addBlock(int posx, int posy, int id){
		bool solid = Game::solidBlocks[id];
		bool harming = Game::harmingBlocks[id];
		std::cout << id << std::endl;
		std::vector< std::vector <Block > > blockCopy = blockVector;
		int rowOfY = 0;
		if(DoesRowExist(posy)){
			rowOfY = findRowOfY(posy);
			for(int i = 0; i < blockCopy[rowOfY].size(); i++){
				if(posx == blockCopy[rowOfY][i].x){
					replaceBlock(rowOfY, i, id, solid);
					std::cout << "Block replaced" << std::endl;
					return true;
				};
			};
		};
		if(blockCopy.size() == 0){
			std::vector<Block> row;
			row.push_back(Block(posy,posx,id,solid,harming));
			blockCopy.push_back(row);
			std::cout << "inserted block" << std::endl;
			std::cout << "X: " << posx << " Y: " << posy << std::endl;
			blockVector = blockCopy;
			return true;
		}

		int greatest = blockCopy[0].at(0).y;
		int smallest = blockCopy[blockCopy.size()-1].at(0).y;
		int rows = blockCopy.size();
		//if there isn't a block with this y value, we have to create a new row at it's appropriate place.

		if(!DoesRowExist(posy)){
			std::cout << "X: " << posx << " Y: " << posy << std::endl;
			std::vector<Block> row;
			row.push_back(Block(posy,posx,id, solid,harming));
			std::cout << "Created new row" << std::endl;
			for(int i = 0; i < rows; i++){
				if(posy > greatest){
					blockCopy.insert(blockCopy.begin(), row);
					std::cout << "inserted block" << std::endl;
					blockVector = blockCopy;
					return true;
				}else if (posy < smallest){
					blockCopy.push_back(row);
					std::cout << "inserted block" << std::endl;
					blockVector = blockCopy;
					return true;
				}else if(posy < blockCopy[i].at(0).y && posy > blockCopy[i+1].at(0).y){
					blockCopy.insert(blockCopy.begin()+i+1, row);
					std::cout << "inserted block" << std::endl;
					blockVector = blockCopy;
					return true;
				};
			}
		};

		for(int abc = 0; abc < blockCopy.size(); abc++)
		{
			if(posy == blockCopy[abc].at(0).y) rowOfY = abc;
		};

		if(blockCopy[rowOfY].size() == 1){
				if(posx < blockCopy[rowOfY].at(0).x){
					blockCopy[rowOfY].insert(blockCopy[rowOfY].begin(), Block(posy,posx,id, solid,harming));
					blockVector = blockCopy;
					return true;
				}else{
					blockCopy[rowOfY].insert(blockCopy[rowOfY].begin()+1, Block(posy,posx,id, solid,harming));
					blockVector = blockCopy;
					return true;
				}
		}else if(posx < blockCopy[rowOfY].at(0).x){
			blockCopy[rowOfY].insert(blockCopy[rowOfY].begin(), Block(posy,posx,id, solid,harming));
			blockVector = blockCopy;
			return true;
		}else if(posx > blockCopy[rowOfY].at(blockCopy[rowOfY].size()-1).x){
			blockCopy[rowOfY].insert(blockCopy[rowOfY].end(), Block(posy,posx,id, solid,harming));
			blockVector = blockCopy;
			return true;
		}

		for(int i = getPreviousIndexX(rowOfY, posx, 0, blockCopy[rowOfY].size()-1); i < blockCopy[rowOfY].size(); i++){
			std::cout << "in loop" << std::endl;
			if(posx < blockCopy[rowOfY].at(i).x){
				blockCopy[rowOfY].insert(blockCopy[rowOfY].begin()+i, Block(posy,posx,id, solid,harming));
				blockVector = blockCopy;
				return true;
			}
		};
		return false;
	};

	bool removeBlock(int id){
		std::vector< std::vector <Block > > blockCopy = blockVector;
	};

	bool replaceBlock(int indexY, int indexX, int id, bool Solid){
		blockVector[indexY][indexX].id = id;
		blockVector[indexY][indexX].solid = Solid;
		return true;
	};


	void swapBlock(int ya, int xa, int yb, int xb){
		Block temp = blockVector[ya][xa];
		blockVector[ya][xa] = blockVector[yb][xb];
		blockVector[yb][xb] = temp;
	};

	int findRowOfY(int i)
	{
		for(int abc = 0; abc < blockVector.size(); abc++)
		{
			if(i == blockVector[abc].at(0).y) return abc;
		};
		return -1;
	};
	int getPreviousIndexX(int IndexY, int positionX, int imin, int imax){
		int imid = (imax + imin) / 2;
		if(blockVector[IndexY][imid].x < positionX && blockVector[IndexY][imid+1].x > positionX)
			return imid;
		else if(imid == imax) return imax;
		else if(positionX > blockVector[IndexY][imid].x)
			return getPreviousIndexX(IndexY, positionX, imid+1, imax);
		else if (imid == imin) return 0;
		else if(positionX < blockVector[IndexY][imid].x)
			return getPreviousIndexX(IndexY, positionX, imin, imid-1);
	};

	int getIndexX(int IndexY, int positionX, int imin, int imax){
		int imid = (imax + imin) / 2;
		if(blockVector[IndexY][imid].x <= positionX && blockVector[IndexY][imid].x + 32 >= positionX)
			return imid;
		else if(imid == imax) return imax;
		else if(positionX > blockVector[IndexY][imid].x)
			return getIndexX(IndexY, positionX, imid+1, imax);
		else if (imid == imin) return imin;
		else if(positionX < blockVector[IndexY][imid].x)
			return getIndexX(IndexY, positionX, imin, imid-1);
	};

	bool DoesRowExist(int i){
		for(int a = 0; a < blockVector.size(); a++){
			if(i == blockVector[a][0].y){
				return true;
			};
		};
		return false;
	};

	void printArray(){
		for(int i = 0; i < Map::blockVector.size(); i++){
			std::cout << "Row: " << i << std::endl;
			for(std::vector<Block>::iterator it = Map::blockVector[i].begin(); it != Map::blockVector[i].end(); it++){
				std::cout << "Column: " << std::distance(blockVector[i].begin(), it) << std::endl;
				std::cout << "Y: " << it->y << "X: " << it->x << std::endl << std::endl;
				
			}
		}
	};
};

#endif