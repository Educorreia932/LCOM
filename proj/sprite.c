#include <lcom/lcf.h>
#include <stdarg.h>  // Already inside lcf, but this here just fixed the compilations errors
// Plz don't kill us Souto
#include "sprite.h"
#include "bitmap.h"
#include "geometry.h"

/* SPRITE */

struct Sprite {
    Bitmap_t **bmps;
    uint8_t size;
    float x_offset, y_offset;
};

Sprite_t* new_sprite(float x_offset, float y_offset, int num, ...) {
  // Check if the number of sprites to animate is valid
  if (num < 1 || num > 255) {
    printf("new_sprite: Num must be [1, 255]\n");
    return NULL;
  }

  // Allocate space for the new object
  Sprite_t *obj = (Sprite_t*) malloc(sizeof(Sprite_t));
  if (obj == NULL) {
    printf("New_sprite: Malloc Failed\n");
    return NULL;
  }

  obj->size = num;
  obj->x_offset = x_offset;
  obj->y_offset = y_offset;
  
  // Load all bitmaps
  obj->bmps = (Bitmap_t**) malloc(sizeof(Bitmap_t*) * num);
  if (obj->bmps == NULL) {
    printf("new_sprite: Failed to allocate space for bitmaps\n");
    return NULL;
  }
  __va_list va;

  /* initialize valist for num number of arguments */
  va_start(va, num);

  /* access all the arguments assigned to valist */
  for (int i = 0; i < num; ++i) {
    obj->bmps[i] = new_bitmap(va_arg(va, const char*));

    if (obj->bmps[i] == NULL) {
      printf("new_sprite: Failed to create the new bmp object of index %d\n", i);
      return NULL;
    }
  }

  /* clean memory reserved for valist */
  va_end(va);

  return obj;    
}

void free_sprite(Sprite_t *s) {
  for (uint8_t i = 0; i < s->size; ++i) {
    free_bitmap(s->bmps[i]);
  }
  free(s->bmps);
  free(s);
}


inline void draw_sprite(Sprite_t *s, Rect_t *r, uint8_t animState, uint32_t color_to_multiply) {
  draw_sprite_floats(s, r->x, r->y, animState, color_to_multiply);
}

void draw_sprite_floats(Sprite_t *s, float x, float y, uint8_t animState, uint32_t color_to_multiply) {

  if (animState > s->size) {
    printf("draw_sprite_ints: Refusing to draw, animation state %d is larger than possible (max is %d)\n", animState, s->size);
    return;
  }
  
  draw_bitmap(s->bmps[animState], (int32_t) (x + s->x_offset), (int32_t) (y + s->y_offset), ALIGN_LEFT, color_to_multiply);
}

inline uint16_t sprite_get_width(Sprite_t *s) {
  return bitmap_get_width(s->bmps[0]);
}

inline uint16_t sprite_get_height(Sprite_t *s) {
  return bitmap_get_height(s->bmps[0]);
}


/* SPRITE DYNAMIC */

struct SpriteDynamic {
    Bitmap_t *bmp;
    uint16_t dynamic_slice_size;
    float x_offset, y_offset;
};

SpriteDynamic_t* new_sprite_dynamic(const char* file_name, uint16_t dynamic_slice_size, uint16_t x_offset, uint16_t y_offset) {

  // Allocate space for the new object
  SpriteDynamic_t *obj = (SpriteDynamic_t*) malloc(sizeof(SpriteDynamic_t));
  if (obj == NULL) {
    printf("New_sprite_dynamic: Malloc Failed\n");
    return NULL;
  }

  obj->dynamic_slice_size = dynamic_slice_size;
  obj->x_offset = x_offset;
  obj->y_offset = y_offset;
  
  // Load all bitmaps
  obj->bmp = new_bitmap(file_name);
  if (obj->bmp == NULL) {
    printf("New_sprite_dynamic: Failed to create Bitmap object\n");
    return NULL;
  }

  return obj;
}

void free_sprite_dynamic(SpriteDynamic_t* sd) {
  free_bitmap(sd->bmp);
  free(sd);
}

void draw_sprite_dynamic(SpriteDynamic_t *sd, Rect_t *r, uint32_t color_to_multiply) {
    // printf("x: %d   y: %d\n", (int) (r->x + s->x_offset), (int) (r->y + s->y_offset));
  draw_bitmap_dynamic(sd->bmp, sd->dynamic_slice_size, (int) (r->x + sd->x_offset), (int) (r->y + sd->y_offset), (int) r->w, (int) r->h, ALIGN_LEFT, color_to_multiply);
}