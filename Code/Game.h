#ifndef GAME_H
#define GAME_H

#include <allegro5\allegro.h>
#include <string>
#include <boost\filesystem.hpp>

using namespace boost::filesystem;
class Game
{
public:
	static int gameState;  // 0 = menu, 1 == game, 2 == editor;
	static bool exit;
	static bool restart;
	static int cameraX;
	static int cameraY;
	static int blockIDs;
	static int selectionRectangleX1;
	static int selectionRectangleX2;
	static int selectionRectangleY1;
	static int selectionRectangleY2;
	static bool drawingSelection;
	static int selectedEffect;
	static int effectsIDs;
	static int selectedTile;
	static bool solidBlocks[255];
	static bool harmingBlocks[32];
	static int menuSelection;
	static int currentProjectile;
	static int saveAmount;
	static int editorTileX;
	static int editorTileY;
	static int editorCamX;
	static int editorCamY;
	static std::string menuInput;
	static std::string mapName;
	static std::string saves[10];
	static bool newGame;
	static bool loadGame;
	static bool options;
	static bool windowed;

	static ALLEGRO_DISPLAY_MODE disp_data;

	static void fetchSaves(){
		path someDir("Maps\\");
		directory_iterator end_iter;

		if ( exists(someDir) && is_directory(someDir))
		{
		  for( directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
		  {
			if (is_regular_file(dir_iter->status()) )
			{
				path p = dir_iter->path();
				std::string s = p.string();
				Game::saves[Game::saveAmount] = s;	
				Game::saveAmount++;

			}
		  }
		}
	};
};

#endif