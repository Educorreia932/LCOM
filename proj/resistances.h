#pragma once

#include <lcom/lcf.h>

#include "geometry.h"

typedef struct Resistances Resistances_t;

Resistances_t* new_testing_resistances();
Resistances_t* prototype_resistances();
void free_resistances(Resistances_t* resistances);

void render_resistances(Resistances_t* resistances);

bool player_touches_resistance(Resistances_t* resistances, Rect_t* player);
