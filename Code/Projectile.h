#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile
{
public:
	Projectile(){
		projectileX = 0;
		projectileY = 0;
		projectileW = 0;
		projectileH = 0;
		projectileVelX = 0;
		projectileVelY = 0;
		hitY = 0;
		hitX = 0;
		hit = true;
		effectID = 0;
	};
	Projectile(float X, float Y, float W, float H, float VELOCITY_X, float VELOCITY_Y):
		projectileX(X),
		projectileY(Y),
		projectileW(W),
		projectileH(H),
		projectileVelX(VELOCITY_X),
		projectileVelY(VELOCITY_Y),
		hitY(0),
		hitX(0),
		effectID(2),
		hit(false){};

	Projectile(float X, float Y, float W, float H, float VELOCITY_X, float VELOCITY_Y, int EFFECTID):
		projectileX(X),
		projectileY(Y),
		projectileW(W),
		projectileH(H),
		projectileVelX(VELOCITY_X),
		projectileVelY(VELOCITY_Y),
		hitY(0),
		hitX(0),
		effectID(EFFECTID),
		hit(false){};

	ALLEGRO_COLOR c;
	float projectileX, projectileY;
	float projectileW, projectileH; 
	float projectileVelX, projectileVelY;
	int hitY, hitX;
	int hitSide;
	int effectID;
	bool hit;
};

#endif