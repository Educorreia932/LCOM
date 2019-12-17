#pragma once

#include <lcom/lcf.h>
#include "input_events.h"
#include "platforms.h"
#include "lasers.h"
#include "spikes.h"

typedef struct Player Player_t;

Player_t* new_player();
Player_t* new_testing_player(bool is_single_player);

void free_player(Player_t* player);

void player_movement(Player_t* player, Platforms_t* plat, Lasers_t* lasers, Spikes_t* spikes);

void render_player(Player_t* player);

