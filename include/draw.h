#ifndef DRAW_H_
#define DRAW_H_

#include "simulation.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    int x1, y1, x2, y2;
    int r, g, b, a;
} Line;

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define MARGIN (int)(SCREEN_WIDTH * 0.05)
#define WORK_WIDTH SCREEN_WIDTH - MARGIN * 2
#define WORK_HEIGHT SCREEN_HEIGHT - MARGIN * 2
#define PREAMBLE_PADD (SCREEN_WIDTH - MARGIN * 2) / (MAX_PREAMBLES - 1)

// Рисует основу (номера абонентов, станцию...)
int DrawBase(SDL_Renderer *renderer, TTF_Font *font, int abonent_count, int padding);

// Каждый кадр обновляет экран
int UpdateScreen(SDL_Renderer *renderer, TTF_Font *font,
                 int abonent_count, int padding, List *list, char colored, int attemption_number);

#endif // DRAW_H_
