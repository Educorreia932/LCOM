#include "player.h"
#include "level.h"
#include "geometry.h"
#include "math_utils.h"
#include "sprite.h"
#include "ui_elements.h"
#include "game_manager.h"

/* PHYSICS STUFF */
#define BASE_GRAVITY 800.0f
#define FALLING_MULT 1.6f
#define MAX_VELOCITY 800.0f

#define IS_GROUNDED_MARGIN 8.0f

/* PLAYER CONSTANTS */
#define PLAYER_RESPAWN_TIME 30 // Frames
#define PLAYER_BASE_SPEED 300.0f // Raw pixels
#define PLAYER_BASE_JUMP 575.0f

#define PLAYER_DEFAULT_SPEED_MULT 1.0f
#define PLAYER_MIN_SPEED_MULT 0.2f
#define PLAYER_MAX_SPEED_MULT 1.8f
#define PLAYER_SPEED_MULT_STEP 0.01f
#define PLAYER_DEFAULT_JUMP_MULT 1.0f
#define PLAYER_MIN_JUMP_MULT 0.4f
#define PLAYER_MAX_JUMP_MULT 1.6f
#define PLAYER_JUMP_MULT_STEP 0.01f

struct Player {
	Rect_t rect;
	Sprite_t *sprite;
	float speed_mult, jump_mult;
	float y_speed, gravity;
	float x_spawn, y_spawn;
	bool heading_right;
	bool ui_controls, arcade_mode;
	uint8_t respawn_timer; // If != 0, player is dead

	// Single Player UI ~ NULL ptr if multiplayer
	Slider_t *jump_slider, *speed_slider;
	Button_t **laser_buttons;
};

void player_set_speed(uint8_t speed) {

	Player_t* p = get_game_manager()->level->player;
	
	float mult = PLAYER_MIN_SPEED_MULT + (
		(PLAYER_MIN_SPEED_MULT - PLAYER_MAX_SPEED_MULT) /
		(0 - 255)
	) * speed;

	p->speed_mult = mult;

}

void player_set_jump(uint8_t jump) {

	Player_t* p = get_game_manager()->level->player;

	float mult = PLAYER_MAX_JUMP_MULT + (
		(PLAYER_MIN_JUMP_MULT - PLAYER_MAX_JUMP_MULT) /
		(255 - 0)
	) * jump;

	p->jump_mult = mult;

}

void player_set_laser_0() {
	lasers_set_link_id(get_game_manager()->level->lasers, 0);
}

void player_set_laser_1() {
	lasers_set_link_id(get_game_manager()->level->lasers, 1);
}

void player_set_laser_2() {
	lasers_set_link_id(get_game_manager()->level->lasers, 2);
}

Player_t* new_player(bool ui_controls, bool arcade_mode) {

	Player_t* player = (Player_t*) malloc(sizeof(Player_t));
	
	if (player == NULL) {
		printf("new_player: Failed to allocate memory for the player object\n");
		return NULL;
	}

	player->sprite = new_sprite(0, 0, 1, "watt_tmp.bmp");

	if (player->sprite == NULL) {
		printf("new_player: Failed to create the Sprite object\n");
		free(player);
		return NULL;
	}

	// Customizing player stats
	player->speed_mult = PLAYER_DEFAULT_SPEED_MULT;
	player->jump_mult = PLAYER_DEFAULT_JUMP_MULT;
	player->y_speed = 0.0f;
	player->gravity = BASE_GRAVITY;
	player->x_spawn = 60.0f;
	player->y_spawn = 704.0f;
	player->heading_right = true;

	// Creating plaeyr hitbox
	player->rect = rect(
		player->x_spawn,
		player->y_spawn, 
		(float) sprite_get_width(player->sprite), 
		(float) sprite_get_height(player->sprite)
	);

	player->arcade_mode = arcade_mode;
	player->ui_controls = ui_controls;

	if (ui_controls) {
		
		// Creating the extra single player UI 
		player->jump_slider = new_slider("ui/small_vertical_slider.bmp", "ui/small_slider_handle.bmp", player_set_jump, vec2d(2, 20), 255, vec2d(4, 25), vec2d(4, 90));
		if (player->jump_slider == NULL) {
			printf("new_player: Failed to create jump slider\n");
			free_sprite(player->sprite);
			free(player);
			return NULL;
		}

		player->speed_slider = new_slider("ui/small_horizontal_slider.bmp", "ui/small_slider_handle.bmp", player_set_speed, vec2d(20, 2), 255, vec2d(25, 4), vec2d(90, 4));
		if (player->speed_slider == NULL) {
			printf("new_player: Failed to create speed slider\n");
			free_sprite(player->sprite);
			free_slider(player->jump_slider);
			free(player);
			return NULL;
		}

		player->laser_buttons = (Button_t**) malloc(sizeof(Button_t*) * 3);
		if (player->laser_buttons == NULL) {
			printf("new_player: Failed to allocate memory for the three buttons\n");
			free_sprite(player->sprite);
			free_slider(player->jump_slider);
			free_slider(player->speed_slider);
			free(player);
			return NULL;
		}

		player->laser_buttons[0] = new_button("ui/small_laser_button_red.bmp", player_set_laser_0, rect(130.0f, 4.0f, 16.0f, 16.0f));
		if (player->laser_buttons[0] == NULL) {
			printf("new_player: Failed to create red laser button\n");
			free_sprite(player->sprite);
			free_slider(player->jump_slider);
			free_slider(player->speed_slider);
			free(player->laser_buttons);
			free(player);
			return NULL;
		}
		player->laser_buttons[1] = new_button("ui/small_laser_button_blue.bmp", player_set_laser_1, rect(160.0f, 4.0f, 16.0f, 16.0f));
				if (player->laser_buttons[0] == NULL) {
			printf("new_player: Failed to create red laser button\n");
			free_sprite(player->sprite);
			free_slider(player->jump_slider);
			free_slider(player->speed_slider);
			free_button(player->laser_buttons[0]);
			free(player->laser_buttons);
			free(player);
			return NULL;
		}
		player->laser_buttons[2] = new_button("ui/small_laser_button_pink.bmp", player_set_laser_2, rect(190.0f, 4.0f, 16.0f, 16.0f));
		if (player->laser_buttons[0] == NULL) {
			printf("new_player: Failed to create red laser button\n");
			free_sprite(player->sprite);
			free_slider(player->jump_slider);
			free_slider(player->speed_slider);
			free_button(player->laser_buttons[0]);
			free_button(player->laser_buttons[1]);
			free(player->laser_buttons);
			free(player);
			return NULL;
		}

	}

	// printf("new_testing_player: Finished making player\n");
	return player;
}

void free_player(Player_t* player) {
	if (player == NULL) {
		printf("free_player: Cannot free a NULL pointer\n");
		return;
  	}
	free_sprite(player->sprite);

	if (player->ui_controls) {
		free_slider(player->jump_slider);
		free_slider(player->speed_slider);

		free_button(player->laser_buttons[0]);
		free_button(player->laser_buttons[1]);
		free_button(player->laser_buttons[2]);

		free(player->laser_buttons);
	}
	free(player);
}

// TODO: This is_grounded is very sketchy, improve it later down the road
bool player_is_grounded(Player_t* player, Platforms_t* plat) {
	Rect_t r;
	if (player->gravity > 0) {
		r = rect(
					player->rect.x + 2,
					player->rect.y + player->rect.h,
					player->rect.w - 2,
					IS_GROUNDED_MARGIN
				);
	}
	else {
		r = rect(
			player->rect.x + 2,
			player->rect.y - IS_GROUNDED_MARGIN,
			player->rect.w - 2,
			IS_GROUNDED_MARGIN
		);
	}
	return does_collide_platforms(plat, &r);
}

inline void player_respawn(Player_t *player) {
	player->rect.x = player->x_spawn;
	player->rect.y = player->y_spawn;
	player->y_speed = 0;
	player->gravity = BASE_GRAVITY;
	player->heading_right = true;
	// player->speed_mult = PLAYER_DEFAULT_SPEED_MULT;
	// player->jump_mult = PLAYER_DEFAULT_JUMP_MULT;
	player->respawn_timer = 0;
}

inline void player_death_cycle(Player_t *player) {
	if (player->respawn_timer != 1)
		--player->respawn_timer;
	
	else
		player_respawn(player);
}

inline void player_start_death(Player_t *player) {
	player->respawn_timer = PLAYER_RESPAWN_TIME + 1;
}

// TODO: Implement animations depending on movement
void player_movement(Player_t* player, Platforms_t* plat, Lasers_t* lasers, Spikes_t* spikes) {

	Rect_t previous_rect = player->rect;

	// Horizontal Movement
	float h_delta = 0;

	if (player->respawn_timer == 0) {

		if (player->ui_controls) {

			update_slider(player->jump_slider);
			update_slider(player->speed_slider);

			update_button(player->laser_buttons[0]);
			update_button(player->laser_buttons[1]);
			update_button(player->laser_buttons[2]);

			if (get_key_down(KBD_X))
				player->gravity *= -1;
		}

		if (player->arcade_mode) {
			if (get_key_down(KBD_X))
				player->gravity *= -1;
		}

		if (get_key(KBD_ARROW_RIGHT) || get_key(KBD_D)) {
			h_delta = DELTATIME * PLAYER_BASE_SPEED * player->speed_mult;
			player->heading_right = true;
		}
		
		if (get_key(KBD_ARROW_LEFT) || get_key(KBD_A)) {
			h_delta = DELTATIME * -PLAYER_BASE_SPEED * player->speed_mult;
			player->heading_right = false;
		}
	}

	if (h_delta != 0) {
		player->rect.x += h_delta / 2;
		
		if (does_collide_platforms(plat, &player->rect)) {
			player->rect.x -= h_delta / 4;

			if (does_collide_platforms(plat, &player->rect))
				player->rect = previous_rect;
		}

		else {	
			previous_rect = player->rect;
			player->rect.x += h_delta / 2;
			if (does_collide_platforms(plat, &player->rect))
				player->rect = previous_rect;
		}
	}

	// Vertical Movement
	previous_rect = player->rect;

	if (player->y_speed * player->gravity > 0)
		player->y_speed += DELTATIME * player->gravity;
	
	else
		player->y_speed += DELTATIME * FALLING_MULT * player->gravity;

	// Jump button
	if (player->respawn_timer == 0)
		if (get_key_down(KBD_Z) || get_key_down(KBD_SPACEBAR))
			if (player_is_grounded(player, plat))
				player->y_speed = -PLAYER_BASE_JUMP * player->jump_mult * fsign(player->gravity);

	// Kinda like terminal velocity
	player->y_speed = fclampf(player->y_speed, -MAX_VELOCITY, MAX_VELOCITY);

	player->rect.y += player->y_speed * DELTATIME
		+ 0.5f * powf(DELTATIME, 2) * player->gravity;
	
	if (does_collide_platforms(plat, &player->rect)) {
		player->rect = previous_rect;
		player->y_speed /= 3;
	}

	if (player->respawn_timer == 0) {
		if (player_is_dead(lasers, &player->rect) || player_touches_spike(spikes, &player->rect))
			player_start_death(player);
	}
	else
		player_death_cycle(player);
	


}

void render_player(Player_t* player) {
	if (player->respawn_timer == 0)
		draw_sprite(player->sprite, &player->rect, COLOR_NO_MULTIPLY, !player->heading_right);
	else
		draw_sprite(player->sprite, &player->rect, COLOR_RED, !player->heading_right);
	
	if (player->ui_controls) {
		render_slider(player->speed_slider);
		render_slider(player->jump_slider);

		render_button(player->laser_buttons[0]);
		render_button(player->laser_buttons[1]);
		render_button(player->laser_buttons[2]);
	}
}

