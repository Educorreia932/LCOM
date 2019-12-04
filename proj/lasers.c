#include "lasers.h"
#include "geometry.h"
#include "sprite.h"

struct Laser {
    Rect_t rect;
    uint8_t link_id;
};

struct Lasers {
    Laser_t* lasers;
    uint8_t num_lasers;
    uint8_t cur_link_id;
    // The number of colors is also the number of possible link ids
    uint16_t* colors;
    uint8_t num_colors;
    SpriteDynamic_t *sprite;
};

Laser_t laser(Rect_t rect, uint8_t link_id) {
    const Laser_t l = {rect, link_id,};
    return l;
}

Lasers_t* prototype_lasers() {
    Lasers_t* lasers = (Lasers_t*) malloc(sizeof(Lasers_t));

    // Sprite
    lasers->sprite = new_sprite_dynamic("/home/lcom/labs/proj/assets/laser.bmp", 8, 0, 0);

    lasers->num_lasers = 4;
    lasers->cur_link_id = 2;

    lasers->lasers = (Laser_t*) malloc(sizeof(Laser_t) * lasers->num_lasers);

    lasers->lasers[0] = laser(rect(732, 632, 16, 112), 0);
    lasers->lasers[1] = laser(rect(804, 632, 16, 112), 1);
    lasers->lasers[2] = laser(rect(876, 632, 16, 112), 0);
    lasers->lasers[3] = laser(rect(512, 224, 16, 144), 2);

    // Colors
    lasers->num_colors = 3;

    lasers->colors = (uint16_t*) malloc(sizeof(uint16_t) * lasers->num_lasers);

    lasers->colors[0] = RED;
    lasers->colors[1] = BLUE;
    lasers->colors[2] = PINK;

    return lasers;
}

void free_lasers(Lasers_t* lasers) {
    if (lasers == NULL) {
        printf("free_lasers: Cannot free a NULL pointer\n");
        return;
    }
    free_sprite_dynamic(lasers->sprite);
    free(lasers->lasers);
    free(lasers->colors);
    free(lasers);
}


inline bool lasers_set_link_id(Lasers_t *lasers, uint8_t link) {
    if (link >= lasers->num_colors)
        return false;
    lasers->cur_link_id = link;
    return true;
}

inline void lasers_cycle_link_id(Lasers_t *lasers) {
    lasers->cur_link_id = (lasers->cur_link_id + 1) % lasers->num_colors;
}

void render_lasers(Lasers_t* lasers) {
    Laser_t* aux = lasers->lasers;

    for (uint32_t i = 0; i < lasers->num_lasers; i++) {
        if (lasers->cur_link_id != aux->link_id)
            draw_sprite_dynamic(lasers->sprite, &aux->rect, lasers->colors[aux->link_id]);
        
        ++aux;
    }
}

bool player_is_dead(Lasers_t* lasers, Rect_t* rect) {
    Laser_t* aux = lasers->lasers;

    for (uint32_t i = 0; i < lasers->num_lasers; i++) {
        if (lasers->cur_link_id != aux->link_id)
            if (rect_collision(&aux->rect, rect))
                return true;
        ++aux;
    }

    return false;
}
