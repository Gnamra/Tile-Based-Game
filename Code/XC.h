#ifndef XC_H
#define XC_H

#include <allegro5/allegro.h>

#define XC_EVENT_AXIS			ALLEGRO_EVENT_JOYSTICK_AXIS
#define XC_EVENT_BUTTON_DOWN	ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN
#define XC_EVENT_BUTTON_UP		ALLEGRO_EVENT_JOYSTICK_BUTTON_UP
#define XC_CONFIGURATION		ALLEGRO_EVENT_JOYSTICK_CONFIGURATION

typedef struct {
	// axis values
	float left_stick_x;
	float left_stick_y;
	float right_stick_x;
	float right_stick_y;
	float dpad_x;
	float dpad_y;

	// buttons pressed
	bool button_1;
	bool button_2;
	bool button_3;
	bool button_4;
	bool button_left_stick;
	bool button_right_stick;
	bool button_left_shoulder;
	bool button_right_shoulder;
	bool button_start;
	bool button_back;
	bool button_right_trigger;
	bool button_left_trigger;
	//bool button_left_stick;
	//bool button_right_stick;
	// reference to the underlying joystick object
	ALLEGRO_JOYSTICK *joy;
}
XC_STATE;

bool xc_install();
bool xc_reconfigure();
void xc_clear_state(XC_STATE *state);
void xc_free_state(XC_STATE *state);
void xc_update(ALLEGRO_EVENT event);
XC_STATE *xc_get_state(int num);
ALLEGRO_EVENT_SOURCE *xc_get_event_source();

#endif