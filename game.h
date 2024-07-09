#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SPRITE_WIDTH_ENEMY1 121
#define SPRITE_HEIGHT_ENEMY1 134
#define NUM_FRAMES_ENEMY1 4

#define SPRITE_WIDTH_ENEMY2 121
#define SPRITE_HEIGHT_ENEMY2 134
#define NUM_FRAMES_ENEMY2 4

#define SPRITE_WIDTH_ENEMY3 150
#define SPRITE_HEIGHT_ENEMY3 134
#define NUM_FRAMES_ENEMY3 3

#define SPRITE_WIDTH_COIN 51
#define SPRITE_HEIGHT_COIN 46
#define NUM_FRAMES_COIN 4

#define FRAME_DURATION 100 // in milliseconds
#define COIN_DISTANCE 5 // in pixels
#define COLLISION_THRESHOLD 5 // Adjust this value to change collision sensitivity
#define ENEMY_SPEED 2 // Adjust this value to change enemy movement speed


extern SDL_Surface *spriteSheet;
extern SDL_Surface *coinSpriteSheet;

extern SDL_Rect enemyClips1[];
extern SDL_Rect enemyClips2[];
extern SDL_Rect enemyClips3[];
extern SDL_Rect coinClips[];

typedef struct {
    int x;
    int y;
    int exists;
    SDL_Rect hitbox;
} Coin;

extern Coin coins[];

void loadEnemyClips();
void loadSpriteSheet(char *filename);
void loadCoinSpriteSheet(char *filename);
void applySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip);
int checkCollision(SDL_Rect a, SDL_Rect b);

#endif

