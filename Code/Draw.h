#ifndef DRAW_H
#define DRAW_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "Block.h"
#include "Map.h"
#include "Game.h"
#include "Input.h"
#include "Player.h"
#include "Projectile.h"
#include "XC.h"
using namespace std;
class Draw
{
public:

	static ALLEGRO_BITMAP	*Draw::blocks[20];
	static ALLEGRO_BITMAP	*Draw::grass;
	static ALLEGRO_BITMAP	*Draw::dirt;
	static ALLEGRO_BITMAP	*Draw::stone;
	static ALLEGRO_BITMAP	*Draw::Flower_Blue;
	static ALLEGRO_BITMAP	*Draw::snow;
	static ALLEGRO_BITMAP	*Draw::grassHanging;
	static ALLEGRO_BITMAP	*Draw::Background;
	static ALLEGRO_FONT		*Draw::times20Font;

	static void Draw::loadImages(){
		Draw::Background = al_load_bitmap("Image//Background.png");
		Draw::blocks[1] = al_load_bitmap("Image//Dirt.png");
		Draw::blocks[2] = al_load_bitmap("Image//Grass.png");
		Draw::blocks[3] = al_load_bitmap("Image//Stone.png");
		Draw::blocks[4] = al_load_bitmap("Image//Blueflower.png");
		Draw::blocks[5] = al_load_bitmap("Image//Snow.png");
		Draw::blocks[6] = al_load_bitmap("Image//HangingGrass.png");
		Draw::blocks[7] = al_load_bitmap("Image//Stone2.png");
		Draw::blocks[8] = al_load_bitmap("Image//Stone3.png");
		Draw::blocks[9] = al_load_bitmap("Image//Spikes.png");
	};

	static void Draw::loadFont(){
		Draw::times20Font = al_load_font("times.ttf", 20, 0);
	};
	static void Draw::controllerstuff(XC_STATE *xc){
		al_draw_textf(times20Font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "left stick: (%f,%f)", xc->left_stick_x, xc->left_stick_y);
		al_draw_textf(times20Font, al_map_rgb(255, 255, 255), 450, 10, ALLEGRO_ALIGN_LEFT, "1: %d", xc->button_1);
	};
	static void Draw::drawMap(Map *map, int PlayerX, int PlayerY)
	{
		for(int i = 0; i < map->blockVector.size(); i++){
			if(map->blockVector[i][0].y > Game::cameraY - 32 && map->blockVector[i][0].y < Game::cameraY + 1080){
				for(std::vector<Block>::iterator it = map->blockVector[i].begin(); it != map->blockVector[i].end();){
					if(it->x > Game::cameraX - 32 && it->x < Game::cameraX + 1920){
						if(it->id != 0) al_draw_bitmap(Draw::blocks[it->id], it->x - Game::cameraX, it->y - Game::cameraY, 0);
					}else if(it->x > Game::cameraX + 1920) break;
					it++;
				}
			};
		}
	};

	static void Draw::drawSelectedTile(int selectedTile)
	{
		al_draw_filled_rectangle(Game::editorTileX * 32 - Game::cameraX, Game::editorTileY * 32 - Game::cameraY, Game::editorTileX * 32 + 32 - Game::cameraX, Game::editorTileY * 32 + 32 - Game::cameraY, al_map_rgba(100,100,100,40));
		al_draw_scaled_bitmap(Draw::blocks[selectedTile], 0,0,32,32, Game::disp_data.width-5, Game::disp_data.height / 2 - 30, - 70,  60, 0 );
	};
	static void Draw::drawPlayer(Player *plr){
		al_draw_filled_rectangle(plr->Px - Game::cameraX, plr->Py - Game::cameraY, plr->Px + plr->Pw - Game::cameraX, plr->Py + plr->Ph - Game::cameraY, al_map_rgb(255,0,0));
	};

	static void Draw::drawBackground(float positionX, float positionY){
		al_draw_bitmap(Draw::Background, 0 - positionX/2, 0 - positionY /2,0);
	};

	static void Draw::projectiles(Projectile p[20]){
		for(int x = 0; x < 10 ; x++){
			if(p->hit == false)al_draw_filled_rectangle(p->projectileX - Game::cameraX, p->projectileY - Game::cameraY, p->projectileX + p->projectileW - Game::cameraX, p->projectileY + p->projectileH - Game::cameraY, al_map_rgb(255,0,0));
			p++;
		};

	};
	static void Draw::currentEffect()
	{
		al_draw_textf(times20Font, al_map_rgb(255,0,0), 100, 100, 0, Game::selectedEffect == 1 ? "Selected effect: Dig" : "Selected effect: Fill");
	}

	static void Draw::drawMenu(){
		al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.2, 0, "New map");
		al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.3, 0, "Load Map");
		al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.4, 0, "Options");
		al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.5, 0, "Exit");
		al_draw_rectangle(	Game::disp_data.width /2 - 20, Game::disp_data.height * 0.2 + (Game::disp_data.height * 0.1) * Game::menuSelection - 10,
							Game::disp_data.width /2 + 100, Game::disp_data.height * 0.2 + (Game::disp_data.height * 0.1) * Game::menuSelection + 30,
							al_map_rgb(255,0,0), 5);
	};
	
	static void Draw::drawMenuNewGame(){
		al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.3, 0, "Type name of map: %s", Game::menuInput.c_str());
	};

	static void Draw::drawMenuLoadGame(){
		for(int i = 0; i < Game::saveAmount; i++)
		{
			al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.15 + (Game::disp_data.height * 0.1 * i) , 0, "Save %i: %s" , i, Game::saves[i].c_str());
		};
		al_draw_rectangle(	Game::disp_data.width /2 - 20, Game::disp_data.height * 0.2 + (Game::disp_data.height * 0.1) * Game::menuSelection - 80,
							Game::disp_data.width /2 + 400, Game::disp_data.height * 0.2 + (Game::disp_data.height * 0.1) * Game::menuSelection,
							al_map_rgb(255,0,0), 5);
	};
	static void Draw::drawMenuOptions(){
	al_draw_textf(times20Font, al_map_rgb(0,0,255), Game::disp_data.width /2, Game::disp_data.height * 0.2, 0, "Windowed: %s" , Game::windowed ? "True" : "False");
			al_draw_rectangle(	Game::disp_data.width /2 - 20, Game::disp_data.height * 0.2 + (Game::disp_data.height * 0.1) * Game::menuSelection - 10,
							Game::disp_data.width /2 + 150, Game::disp_data.height * 0.2 + (Game::disp_data.height * 0.1) * Game::menuSelection + 30,
							al_map_rgb(255,0,0), 5);
	};

	static void Draw::fancyMenuStuff(Projectile p[20]){
		for(int i = 0; i < 20; i++){
			p->projectileY += 1;
			if(p->projectileY > Game::disp_data.height) p->projectileY = rand() % 200 + 5;
			//p->projectileX += rand() % Game::disp_data.width/1.25;
			if(rand() % 10 == 2)p->c = al_map_rgb(rand() %255, rand() %255, rand() %255);
			al_draw_filled_rectangle(p->projectileX, p->projectileY, p->projectileX + p->projectileW, p->projectileY + p->projectileH, p->c);
			++p;
		};
	};
};

#endif