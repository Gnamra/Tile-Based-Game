#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_opengl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include "Game.h"
#include "input.h"
#include "Map.h"
#include "Player.h"
#include "Block.h"
#include "Draw.h"
#include "Projectile.h"
#include "Effects.h"
#include "Generation.h"
#include "XC.h"
using namespace std;

#define MAX_MAP_Y 64
#define MAX_MAP_X 240

struct tilePos{
	int x;
	int y;
};

struct vecSize{
	unsigned int VecSize;
};


// X = Tx * Rw/Tw
// Y = Ty * Rh/Th
int getTx(float X, int Tw){
	if(X / Tw < 0) return X / Tw - 1;
	return X / Tw;
};

int getTy(float Y, int Th){
	if(Y / Th < 0) return Y / Th - 1;
	return Y / Th;
};

int getX(int Tx){
	return Tx * 32;
}

int getY(int Ty){
	return Ty * 32;
}

void checkCollisionY(Player *plr, Map *map){

	int PLeg = getY(getTx(plr->Py + plr->Ph+1, 32));
	int PHead = getY(getTx(plr->Py, 32));

	int PRHandF = getX(getTx(plr->Px + plr->Pw - 2, 32));
	int PLHandF = getX(getTx(plr->Px + 2, 32));

	int PLegF2 = getY(getTx(plr->Py + plr->Ph - 4, 32));
	int PHeadF2 = getY(getTx(plr->Py + 4, 32));
	if(plr->Yvel > 40){
			if(map->checkX(PLeg, PLHandF) == 0 || map->checkX(PLeg, PRHandF) == 0 || map->checkX(PLegF2, PLHandF) == 0 || map->checkX(PLegF2, PRHandF) == 0){ // Feet collision
				plr->setFall(0);
				plr->Py += -plr->Yvel;
				plr->Yvel = 0;
				plr->hasLanded = true;
				return;
			}else if(map->checkX(PLeg + 32, PLHandF) == 0 || map->checkX(PLeg + 32, PRHandF) == 0 || map->checkX(PLegF2 + 32, PLHandF) == 0 || map->checkX(PLegF2 + 32, PRHandF) == 0){
				plr->setFall(0);
				plr->Py += -plr->Yvel;
				plr->Yvel = 0;
				plr->hasLanded = true;
				return;
			}else plr->setFall(1);
	};
	if(map->checkX(PLeg, PLHandF) == 0 || map->checkX(PLeg, PRHandF) == 0 || map->checkX(PLegF2, PLHandF) == 0 || map->checkX(PLegF2, PRHandF) == 0){ // Feet collision
		plr->setFall(0);
		plr->Py = PLeg - plr->Ph-1;
		plr->Yvel = 0;
		plr->hasLanded = true;
		return;
	}else plr->setFall(1);

	if(map->checkX(PHead, PRHandF) == 0 || map->checkX(PHead, PLHandF) == 0 || map->checkX(PHeadF2, PRHandF) == 0 || map->checkX(PHeadF2, PLHandF) == 0){ // Head collision
		plr->hasLanded = false;
		plr->Py += -plr->Yvel;
		plr->Yvel = 0;
		return;
	}
};

void checkCollisionX(Player *plr, Map *map){

	int PRHand = getX(getTx(plr->Px + plr->Pw, 32));
	int PLHand = getX(getTx(plr->Px, 32));

	int PLegF = getY(getTx(plr->Py + plr->Ph, 32));
	int PRHandF = getX(getTx(plr->Px + plr->Pw, 32));
	int PLHandF = getX(getTx(plr->Px, 32));
	int PHeadF = getY(getTx(plr->Py, 32));

	if(map->checkX(PHeadF, PRHand) == 0 || map->checkX(PHeadF, PRHandF) == 0 || map->checkX(PLegF, PRHand) == 0 || map->checkX(PLegF, PRHandF) == 0){ // Right side collision
		plr->Px -= plr->Xvel;
		plr->Xvel = 0;
	}else if(map->checkX(PHeadF, PLHand) == 0 || map->checkX(PHeadF, PLHandF) == 0 || map->checkX(PLegF, PLHand) == 0 || map->checkX(PLegF, PLHandF) == 0){ // left side collision
		plr->Px -= plr->Xvel;
		plr->Xvel -= plr->Xvel;
	}
};

void checkCollisionProjectile(Projectile *proj, Map *map)
{
	int PRHand = getX(getTx(proj->projectileX + proj->projectileW, 32));
	int PLHand = getX(getTx(proj->projectileX, 32));
	int PLeg = getY(getTx(proj->projectileY + proj->projectileH, 32));
	int PHead = getY(getTx(proj->projectileY, 32));

		if(map->checkX(PHead, PRHand) == 0){ // Right side collision
			cout << "1" << endl;
			proj->hit = true;
			proj->hitY = PHead;
			proj->hitX = PRHand;
			return;
		}else if (map->checkX(PLeg, PRHand) == 0){
			cout << "2" << endl;
			proj->hit = true;
			proj->hitY = PLeg;
			proj->hitX = PRHand;
			return;
		}else if(map->checkX(PHead, PLHand) == 0){ // left side collision
			cout << "3" << endl;
			proj->hit = true;
			proj->hitY = PHead;
			proj->hitX = PLHand;
			return;
		}else if(map->checkX(PLeg, PLHand) == 0){
			cout << "4" << endl;
			proj->hit = true;
			proj->hitY = PLeg;
			proj->hitX = PLHand;
			return;
		};

};

void createMap(Map *map, string s){
	fstream fs;
	fs.open(s.c_str(), fs.out | fs.in | fs.binary);

	if(fs){
		vecSize VSize;
		fs.read((char*)&VSize, sizeof(vecSize));
		int rowNmbr = VSize.VecSize;
		for(int y = 0; y < rowNmbr; y++){
			fs.read((char*)&VSize, sizeof(vecSize));
			vector<Block> row;
			for(int x = 0; x < VSize.VecSize; x++){
				Block b;
				fs.read((char*)&b, sizeof(Block));
				row.push_back(b);
			};
			map->blockVector.push_back(row);
		};
		fs.close();
	};
};

void saveMap(Map *map, string s){
	cout << s << endl;
	fstream fs;
	fs.open(s.c_str(), fs.in | fs.out | fs.binary);
	if(fs){
		cout << s.c_str() << " found, overwriting." << endl;
		fs.seekp(0);
		vecSize vSize;
		vSize.VecSize = map->blockVector.size();
		fs.write((char*)&vSize, sizeof(vecSize));
		for(int y = 0; y < map->blockVector.size(); y++){
			vSize.VecSize = map->blockVector[y].size();
			fs.write((char*)&vSize, sizeof(vecSize));
			for(int x = 0; x < map->blockVector[y].size(); x++){
				fs.write((char*)&map->blockVector[y][x], sizeof(Block));
			}
		};
		fs.close();
	}else{
		ofstream astr(s.c_str(), ios::binary);
		cout << s.c_str() << " created." << endl;
		astr.seekp(0);
		vecSize vSize;
		vSize.VecSize = map->blockVector.size();
		astr.write((char*)&vSize, sizeof(vecSize));
		for(int y = 0; y < map->blockVector.size(); y++){
			vSize.VecSize = map->blockVector[y].size();
			astr.write((char*)&vSize, sizeof(vecSize));
			for(int x = 0; x < map->blockVector[y].size(); x++){
				astr.write((char*)&map->blockVector[y][x], sizeof(Block));
			}
		};
		astr.close();
	};
};


void testGen(Map *map)
{
	int worldHeight = 5120;
	int worldWidth = 10240;
	for(int y = -worldHeight; y < worldHeight; y+=32){
		for(int x = -worldWidth; x < worldWidth; x+=32){
			if(y == 0) map->addBlock(x, y, 2);
			if(y > 0 && y < worldHeight) map->addBlock(x, y, 3);
		};
	};
	Generation::mountain(20,20, map);
};

void loadMap(Map *map, string s){

	if(!s.empty()){
		fstream fs;
		fs.open(s.c_str(), fs.out | fs.in | fs.binary);

		if(fs){
			vecSize VSize;
			fs.read((char*)&VSize, sizeof(vecSize));
			int rowNmbr = VSize.VecSize;
			for(int y = 0; y < rowNmbr; y++){
				fs.read((char*)&VSize, sizeof(vecSize));
				vector<Block> row;
				for(int x = 0; x < VSize.VecSize; x++){
					Block b;
					fs.read((char*)&b, sizeof(Block));
					row.push_back(b);
				};
				map->blockVector.push_back(row);
			};
			fs.close();
		};
		Game::gameState = 2;
	};
};

void placeParticles(Projectile p[20]){
	for(int i = 0; i < 20; i++){
		p->projectileY = rand() % Game::disp_data.height;
		p->projectileX = rand() % Game::disp_data.width;
		p->projectileH = 2;
		p->projectileW = 2;
		p->c = al_map_rgb(255,0,0);
		cout << p->c.b << endl;
		++p;
	};
};

int main(int argc, char *argv[])
{
	bool redraw = false;
	bool doLogic = false;
	bool moving = false;
	bool showHit = true;
	int length = 0;
	int width = 0;
	int rows = 0;
	int columns = 0;
	int editortileX = 0;
	int editortileY = 0;
	int WorldX = 0;
	int WorldY = 0;
	int currentProjectile = 0;
	int MBloop = 0;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *drawTimer;
	ALLEGRO_TIMER *logicTimer;
	Map map;
	Projectile pArray[20];

	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_FRAMELESS);
	al_get_display_mode(al_get_num_display_modes()-1, &Game::disp_data);
	std::cout << "Disp_data width: " << Game::disp_data.width << endl << "Disp data Height: " << Game::disp_data.height << endl;

	xc_install();
	XC_STATE *controller = xc_get_state(0);
	if (!controller) {
		fprintf(stderr, "Failed to grab xbox controller state.\n");
		return 1;
	}

	display = al_create_display(Game::disp_data.width, Game::disp_data.height);
	event_queue = al_create_event_queue();
	drawTimer = al_create_timer(1.0/60.0);
	logicTimer = al_create_timer(1.0/60.0);
	Draw::loadImages();
	Draw::loadFont();
	Game::fetchSaves();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, xc_get_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(drawTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(logicTimer));

	al_start_timer(drawTimer);
	al_start_timer(logicTimer);
	al_set_mouse_xy(display, 500, 500);
	//al_hide_mouse_cursor(display);

	WorldX = 32*columns;
	WorldY = 32*rows;

	Player player(-100, 0);

	for(int i = 0; i < 256; i++){
		Input::keys[i] = false;
	};

	Input::mouseButtons[0] = false;
	placeParticles(pArray);
	while(Game::gameState != 0){
		while(Game::gameState == 3){
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			if(ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == drawTimer){
				redraw = true;
			}else if(ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == logicTimer){
				doLogic = true;
			}
			else Input::input(&ev);

			if(doLogic && al_is_event_queue_empty(event_queue)){
				doLogic = false;
				
				
				if(Input::keys[0]){
					Game::menuSelection--;
					Input::keys[0] = false;
				};
				if(Input::keys[1]){
					Game::menuSelection++;
					Input::keys[1] = false;
				};
				if(Input::keys[9] && !Game::newGame && !Game::loadGame && !Game::options){
					switch(Game::menuSelection){
					case 0:
						Game::newGame = true;
						Input::keys[9] = false;
						break;
					case 1:
						Game::loadGame = true;
						Input::keys[9] = false;
						break;
					case 2:
						Game::options = true;
						Input::keys[9] = false;
						break;
					case 3:
						Game::gameState = 0;
						Input::keys[9] = false;
						break;
					};
				};
				if(Input::keys[10]){
					Input::keys[10] = false;
					Game::newGame = false;
					Game::loadGame = false;
					Game::options = false;
				};
				if(Game::newGame == true){
					if(Input::keys[9] && Game::newGame){
						string t = "Maps//" + Game::menuInput + ".bin";
						Game::mapName = t;
						createMap(&map, t);
						//testGen(&map);
						Game::gameState = 2;
					};
				};
				if(Game::loadGame == true){
					if(Input::keys[9] && Game::loadGame){
						loadMap(&map, Game::saves[Game::menuSelection]);
						Game::mapName = Game::saves[Game::menuSelection];
					};
				};
				if(Game::options == true)
					if(Input::keys[9] && Game::options){
						Input::keys[9] = false;
						switch(Game::menuSelection){
						case 0:
							if(Game::windowed){
								al_destroy_display(display);
								al_set_new_display_flags(ALLEGRO_FULLSCREEN);
								al_get_display_mode(al_get_num_display_modes()-1, &Game::disp_data);
								display = al_create_display(Game::disp_data.width, Game::disp_data.height);
								Draw::loadImages();
								Draw::loadFont();
							}else{
								al_destroy_display(display);
								al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
								display = al_create_display(Game::disp_data.width, Game::disp_data.height);
								al_set_new_window_position(200, 200);
								Draw::loadImages();
								Draw::loadFont();
							};
							Game::windowed = !Game::windowed;
							break;
						}
					};
			};

			if(redraw && al_is_event_queue_empty(event_queue)){
				redraw = false;
				al_clear_to_color(al_map_rgb(0,0,0));
				Draw::fancyMenuStuff(pArray);
				if(Game::newGame) Draw::drawMenuNewGame();
				else if(Game::loadGame) Draw::drawMenuLoadGame();
				else if(Game::options) Draw::drawMenuOptions();
				else Draw::drawMenu();
				al_flip_display();
			};
		};

		while(Game::gameState == 1){
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			if(ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == drawTimer){
				redraw = true;
			}else if(ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == logicTimer){
				doLogic = true;
			}else if(ev.type == XC_EVENT_AXIS)xc_update(ev);
			else if(ev.type == XC_EVENT_BUTTON_DOWN)xc_update(ev);
			else if(ev.type == XC_EVENT_BUTTON_UP)xc_update(ev);
			else{
				Input::input(&ev);
			}

			if(doLogic && al_is_event_queue_empty(event_queue)){
				doLogic = false;
				if(controller->button_2){
					player.Px = 200; 
					player.Py = 200;
				};
				if(Input::keys[0] && player.hasLanded || controller->button_1 && player.hasLanded){
					player.Yvel = 0;
					player.Yvel -= 18;
					player.hasLanded = false;
					player.setFall(0);
				}
				else if (Input::keys[1]) player.Yvel += 0.04;
				else if (Input::keys[2] && player.Xvel > -6) player.Xvel -= 0.4;
				else if (Input::keys[3] && player.Xvel < 6) player.Xvel += 0.4;
				else if (Input::keys[2] == false && player.Xvel <= -0.1) player.Xvel += 0.4;
				else if (Input::keys[3] == false && player.Xvel >= 0.1) player.Xvel -= 0.4;
				else if (Input::keys[6]){
					Input::keys[6] = false;
					Game::gameState = 2;
					Game::editorCamX = player.Px;
					Game::editorCamY = player.Py;
				}else if(Input::keys[7]){
					Input::keys[7] = false;
					if(Game::selectedEffect == Game::effectsIDs)Game::selectedEffect = 0;
					Game::selectedEffect++;
				};

				if(player.Xvel > -6 && player.Xvel < 6)player.Xvel += controller->left_stick_x;

				if(Input::mouseDZ != 0){
					Game::selectedTile += Input::mouseDZ;
					if(Game::selectedTile > Game::blockIDs) Game::selectedTile = 1;
					if(Game::selectedTile <= 0) Game::selectedTile = Game::blockIDs;
					Input::mouseDZ = 0;
				}
				if(Input::mouseButtons[0]){
					if(MBloop == 10){ 
						if(currentProjectile == 10) currentProjectile = 0;
						pArray[currentProjectile].hit = false;
						pArray[currentProjectile].projectileX = player.Px;
						pArray[currentProjectile].projectileY = player.Py;
						pArray[currentProjectile].projectileH = 4;
						pArray[currentProjectile].projectileW = 4;
						pArray[currentProjectile].effectID = 2;
						pArray[currentProjectile].projectileVelX = cos(atan2((Input::mouseY - (player.Py - Game::cameraY)), (Input::mouseX - (player.Px - Game::cameraX)))) * 10;
						pArray[currentProjectile].projectileVelY = sin(atan2((Input::mouseY - (player.Py - Game::cameraY)), (Input::mouseX - (player.Px - Game::cameraX)))) * 10;
						currentProjectile++;
						MBloop = 0;
					};
					MBloop += 1;
				}if(!Input::mouseButtons[0]) MBloop = 10;


				if(player.isFalling && player.Yvel < 60 )player.Yvel += 1;
				if(player.Xvel < 0.2 && player.Xvel > -0.2) player.Xvel = 0.0;

				player.Py += player.Yvel;
				checkCollisionY(&player, &map);
				player.Px += player.Xvel;
				checkCollisionX(&player, &map);



				for(int x = 0; x < 10; x++){
					if(!pArray[x].hit){
						pArray[x].projectileX += pArray[x].projectileVelX;
						pArray[x].projectileY += pArray[x].projectileVelY;
						checkCollisionProjectile(&pArray[x], &map);

						if(pArray[x].hit){
							switch(Game::selectedEffect){
							case 1:
								Effect::effect01(pArray[x].hitY, pArray[x].hitX, &map);
								break;
							case 2:
								Effect::effect02(getX(getTx(pArray[x].projectileY - pArray[x].projectileVelY, 32)) , getX(getTx(pArray[x].projectileX - pArray[x].projectileVelX,32)), &map);
								break;
							};
							pArray[x].projectileX = 0;
							pArray[x].projectileY = 0;
							pArray[x].projectileVelX = 0;
							pArray[x].projectileVelY = 0;

						};
					};
				};

				Game::cameraY = player.Py - Game::disp_data.height / 2;
				Game::cameraX = player.Px - Game::disp_data.width / 2;
			}

			// DRAW
			if(redraw && al_is_event_queue_empty(event_queue)){
				redraw = false;
				al_clear_to_color(al_map_rgb(100, 160, 220));
				Draw::drawBackground(player.Px, player.Py);
				Draw::drawMap(&map, player.Px, player.Py);
				Draw::drawPlayer(&player);
				Draw::projectiles(pArray);
				Draw::currentEffect();
				Draw::controllerstuff(controller);
				

				al_flip_display();
			}
		}

		while(Game::gameState == 2){ // editor
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			if(ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == drawTimer){
				redraw = true;
			}else if(ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == logicTimer){
				doLogic = true;
			}else{
				Input::input(&ev);
			}

			if(doLogic && al_is_event_queue_empty(event_queue)){
				doLogic = false;
				Game::cameraY = Game::editorCamY - Game::disp_data.height / 2;
				Game::cameraX = Game::editorCamX - Game::disp_data.width / 2;
				Game::editorTileX = getTx(Input::mouseX + Game::cameraX, 32);
				Game::editorTileY = getTy(Input::mouseY + Game::cameraY, 32);
				if(Input::keys[0])Game::editorCamY -= 20;
				else if (Input::keys[1]) Game::editorCamY += 20;
				else if (Input::keys[2]) Game::editorCamX -= 20;
				else if (Input::keys[3]) Game::editorCamX += 20;
				else if(Input::mouseButtons[0] || Game::drawingSelection){
					Game::drawingSelection = true;

					if (Input::mouseDX > 0 || Input::mouseDY > 0 || Input::mouseDX < 0 || Input::mouseDY < 0){
						if( Game::selectionRectangleX2 == 0 && Game::selectionRectangleY2 == 0){
							Game::selectionRectangleX1 = getX(Game::editorTileX);
							Game::selectionRectangleY1 = getX(Game::editorTileY);
							Game::selectionRectangleX2 = getX(Game::editorTileX);
							Game::selectionRectangleY2 = getX(Game::editorTileY);
						}
						Game::selectionRectangleX2 = getX(Game::editorTileX);
						Game::selectionRectangleY2 = getX(Game::editorTileY);
					};

					if(!Input::mouseButtons[0] && Game::drawingSelection){
						if(Game::selectionRectangleY1 < Game::selectionRectangleY2){
							if(Game::selectionRectangleX1 < Game::selectionRectangleX2){
								for(int y = Game::selectionRectangleY1; y < Game::selectionRectangleY2; y+=32)
									for(int x = Game::selectionRectangleX1; x < Game::selectionRectangleX2; x+=32)
										map.addBlock(x, y, Game::selectedTile);

							}else if(Game::selectionRectangleX1 > Game::selectionRectangleX2){
								for(int y = Game::selectionRectangleY1; y < Game::selectionRectangleY2; y+=32)
									for(int x = Game::selectionRectangleX2; x < Game::selectionRectangleX1; x+=32)
										map.addBlock(x, y, Game::selectedTile);

							};
					}else if(Game::selectionRectangleY1 > Game::selectionRectangleY2){
						if(Game::selectionRectangleX1 < Game::selectionRectangleX2){
							for(int y = Game::selectionRectangleY2; y < Game::selectionRectangleY1; y+=32)
								for(int x = Game::selectionRectangleX1; x < Game::selectionRectangleX2; x+=32)
									map.addBlock(x, y, Game::selectedTile);
						}else if(Game::selectionRectangleX1 > Game::selectionRectangleX2)
							for(int y = Game::selectionRectangleY2; y < Game::selectionRectangleY1; y+=32)
								for(int x = Game::selectionRectangleX2; x < Game::selectionRectangleX1; x+=32)
									map.addBlock(x, y, Game::selectedTile);
						};

						Game::selectionRectangleX1 = 0;
						Game::selectionRectangleX2 = 0;
						Game::selectionRectangleY1 = 0;
						Game::selectionRectangleY2 = 0;
						Game::drawingSelection = false;
					};

					if(Input::mouseDX == 0 && Input::mouseDY == 0  && Game::drawingSelection){
						map.addBlock(getX(Game::editorTileX), getY(Game::editorTileY), Game::selectedTile);
						Game::drawingSelection = false;
					}
					
				}

				else if(Input::mouseButtons[1]){
					map.addBlock(getX(Game::editorTileX), getY(Game::editorTileY), 0);
				}
				if(Input::mouseDZ != 0){
					Game::selectedTile += Input::mouseDZ;
					if(Game::selectedTile > Game::blockIDs) Game::selectedTile = 1;
					if(Game::selectedTile <= 0) Game::selectedTile = Game::blockIDs;
					Input::mouseDZ = 0;
				}
				if(Input::keys[6]){
					Input::keys[6] = false;
					Game::gameState = 1;
				};
			}

			//DRAW
			if(redraw && al_is_event_queue_empty(event_queue)){
				redraw = false;
				al_clear_to_color(al_map_rgb(100, 160, 220));
				Draw::drawBackground(Game::editorCamX, Game::editorCamY);
				Draw::drawMap(&map, Game::cameraX, Game::cameraY);
				Draw::drawSelectedTile(Game::selectedTile);
				Draw::drawPlayer(&player);
				al_draw_rectangle(Game::selectionRectangleX1 - Game::cameraX, Game::selectionRectangleY1 - Game::cameraY, Game::selectionRectangleX2 - Game::cameraX, Game::selectionRectangleY2 - Game::cameraY, al_map_rgb(255,0,0), 2);
				al_flip_display();
			}
		};

		//SAVE MAP
		if(Game::gameState == 0){
			if(!Game::mapName.empty()) saveMap(&map, Game::mapName);
		};
	};
	return 0;
};

			