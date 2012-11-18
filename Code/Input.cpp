#include "Input.h"
#include "Game.h"
#include <iostream>
bool Input::keys[256];
bool Input::mouseButtons[10];
int Input::mouseX = 0;
int Input::mouseY = 0;
int Input::mouseDZ = 0;
float Input::mouseDX = 0;
float Input::mouseDY = 0;

void Input::input(ALLEGRO_EVENT *ev){

	if(ev->type == ALLEGRO_EVENT_KEY_DOWN){
		switch(ev->keyboard.keycode){
		case ALLEGRO_KEY_ESCAPE:
			Game::gameState = 0;
			Game::exit = true;
			break;
		case ALLEGRO_KEY_W:
			Input::keys[0] = true;
			break;
		case ALLEGRO_KEY_S:
			Input::keys[1] = true;
			break;
		case ALLEGRO_KEY_A:
			Input::keys[2] = true;
			break;
		case ALLEGRO_KEY_D:
			Input::keys[3] = true;
			break;
		case ALLEGRO_KEY_F:
			Input::keys[4] = true;
			break;
		case ALLEGRO_KEY_X:
			Input::keys[5] = true;
			break;
		case ALLEGRO_KEY_Z:
			Input::keys[6] = true;
			break;
		case ALLEGRO_KEY_E:
			Input::keys[7] = true;
			break;
		case ALLEGRO_KEY_LSHIFT:
			Input::keys[8] = true;
			break;
		case ALLEGRO_KEY_ENTER:
			Input::keys[9] = true;
			break;
		case ALLEGRO_KEY_BACKSPACE:
			Input::keys[10] = true;
			break;
		}
	}
	else if(ev->type == ALLEGRO_EVENT_KEY_UP){
		switch(ev->keyboard.keycode){
		case ALLEGRO_KEY_ESCAPE:
			Game::gameState = 0;
			break;
		case ALLEGRO_KEY_W:
			Input::keys[0] = false;
			break;
		case ALLEGRO_KEY_S:
			Input::keys[1] = false;
			break;
		case ALLEGRO_KEY_A:
			Input::keys[2] = false;
			break;
		case ALLEGRO_KEY_D:
			Input::keys[3] = false;
			break;
		case ALLEGRO_KEY_F:
			Input::keys[4] = false;
			break;
		case ALLEGRO_KEY_X:
			Input::keys[5] = false;
			break;
		case ALLEGRO_KEY_Z:
			Input::keys[6] = false;
			break;
		case ALLEGRO_KEY_E:
			Input::keys[7] = false;
			break;
		case ALLEGRO_KEY_LSHIFT:
			Input::keys[8] = false;
			break;
		case ALLEGRO_KEY_ENTER:
			Input::keys[9] = false;
			break;
		case ALLEGRO_KEY_BACKSPACE:
			Input::keys[10] = false;
			break;
		}
	}
	if(ev->type == ALLEGRO_EVENT_MOUSE_AXES){
		Input::mouseX = ev->mouse.x;
		Input::mouseY = ev->mouse.y;
		Input::mouseDZ = ev->mouse.dz;
		Input::mouseDX = ev->mouse.dx;
		Input::mouseDY = ev->mouse.dy;
	}
	if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
		Input::mouseDX = ev->mouse.dx;
		Input::mouseDY = ev->mouse.dy;
		switch(ev->mouse.button){
			case 1:
				Input::mouseButtons[0] = true;
				break;
			case 2:
				Input::mouseButtons[1] = true;
				break;
		};
	}
	else if (ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
		Input::mouseDX = ev->mouse.dx;
		Input::mouseDY = ev->mouse.dy;
		switch(ev->mouse.button){
			case 1:
				Input::mouseButtons[0] = false;
				break;
			case 2:
				Input::mouseButtons[1] = false;
				break;
		};
	};

	if(Game::newGame && !Game::loadGame){
		if(ev->type == ALLEGRO_EVENT_KEY_DOWN && ev->type != ALLEGRO_EVENT_KEY_UP && ev->keyboard.keycode > 0 && ev->keyboard.keycode < 37){
			char i = 'a';
			if(Input::keys[8] && ev->keyboard.keycode > 0 && ev->keyboard.keycode < 27 )i = (char)ev->keyboard.keycode+48;
			else if (ev->keyboard.keycode > 26 && ev->keyboard.keycode < 37)i = (char)ev->keyboard.keycode+21;
			else i = (char)ev->keyboard.keycode+96;
			std::cout << i << std::endl;
			Game::menuInput+=i;
		};
	};
};