#include "Game.h"
int Game::gameState = 3;
bool Game::restart = false;
bool Game::exit = false;
bool Game::newGame = false;
bool Game::loadGame = false;
bool Game::options = false;
bool Game::windowed = false;
int Game::cameraX = 0;
int Game::cameraY = 0;
int Game::editorCamX = 0;
int Game::editorCamY = 0;
int Game::blockIDs = 9;
int Game::selectionRectangleX1 = 0;
int Game::selectionRectangleX2 = 0;
int Game::selectionRectangleY1 = 0;
int Game::selectionRectangleY2 = 0;
bool Game::drawingSelection = false;
int Game::effectsIDs = 2;
int Game::selectedEffect = 1;
int Game::selectedTile = 1;
int Game::currentProjectile = 0;
int Game::menuSelection = 0;
int Game::saveAmount = 0;
int Game::editorTileX = 0;
int Game::editorTileY = 0;
std::string Game::menuInput = "";
std::string Game::mapName = "";
std::string Game::saves[10];
bool Game::solidBlocks[255] = {
	solidBlocks[0] = false, // air
	solidBlocks[1] = true, // dirt
	solidBlocks[2] = true, // grass
	solidBlocks[3] = true, // stone
	solidBlocks[4] = false, // blueflower
	solidBlocks[5] = true, // snow
	solidBlocks[6] = false, // hanging grass
	solidBlocks[7] = true, // stone 2
	solidBlocks[8] = true, // stone 3
	solidBlocks[9] = false, // Spikes
};

bool Game::harmingBlocks[32] = {
	harmingBlocks[0] = false, // air
	harmingBlocks[1] = false, // dirt
	harmingBlocks[2] = false, // grass
	harmingBlocks[3] = false, // stone
	harmingBlocks[4] = false, // blueflower
	harmingBlocks[5] = false, // snow
	harmingBlocks[6] = false, // hanging grass
	harmingBlocks[7] = false, // stone 2
	harmingBlocks[8] = false, // stone 3
	harmingBlocks[9] = true, // Spikes
};

ALLEGRO_DISPLAY_MODE Game::disp_data;