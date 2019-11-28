#pragma once

#include <lcom/lcf.h>
#include "geometry.h"
#include "mouse_cursor.h"
#include "sprite.h"

/* BUTTONS */

typedef struct Button Button_t;

// Empty string to not have (and never draw) a sprite
Button_t* new_button(const char* sprite_file_name, void (*func)(), Rect_t rect);
// Auto sizes the button, must have a Sprite
Button_t* new_button_auto_size(const char* sprite_file_name, void (*func)(), Vec2d_t pos);
// Reuses a Sprite_t from somewhere else
Button_t* new_button_existing_sprite(Sprite_t *sprite, void (*func)(), Rect_t rect);

void free_button(Button_t* button);
void free_button_without_sprite(Button_t* button);

// Button Methods

void button_hide(Button_t* button);
void button_show(Button_t* button);
void button_activate(Button_t* button);
void button_deactivate (Button_t* button);

void update_button(Button_t* button, MouseCursor_t* cursor);
void render_button(Button_t* button);


/* SLIDERS */

typedef struct Slider Slider_t;


Slider_t* new_slider(const char* bar_sprite_file_name, const char* handle_sprite_file_name, void (*func)(uint8_t), Vec2d_t bar_pos, uint8_t max_state, Vec2d_t start_pos, Vec2d_t end_pos);

void free_slider(Slider_t* slider);


void slider_hide(Slider_t* slider);
void slider_show(Slider_t* slider);
void slider_activate(Slider_t* slider);
void slider_deactivate (Slider_t* slider);

void update_slider(Slider_t* slider, MouseCursor_t* cursor);

void render_slider(Slider_t* slider);
