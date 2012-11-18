#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	Player(float y, float x) :
		Px(x),
		Py(y),
		Pw(16),
		Ph(16),
		Xvel(0),
		Yvel(0),
		hasLanded(0),
		inWall(0),
		isFalling(1){};
	void setFall(int i){
		switch(i){
		case 0:
			isFalling = false;
			break;
		case 1:
			isFalling = true;
			break;
		};
	};
	void setPosition(int x, int y){
		Px = x;
		Py = y;
	};
	float Px, Py, Pw, Ph, Xvel, Yvel;
	bool isFalling;
	bool hasLanded;
	bool canMoveRight;
	bool canMoveLeft;
	bool inWall;
private:
};


#endif