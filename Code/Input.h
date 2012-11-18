#ifndef INPUT_H
#define INPUT_H

#include <allegro5\allegro.h>

class Input
{
public:
	static bool keys[256];
	static bool mouseButtons[10];
	static int mouseX;
	static int mouseY;
	static int mouseDZ;
	static float mouseDX;
	static float mouseDY;
	static void input(ALLEGRO_EVENT *ev);
};


#endif