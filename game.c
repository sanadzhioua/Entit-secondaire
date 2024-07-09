#include "game.h"

SDL_Surface *screen = NULL;
SDL_Surface *spriteSheet = NULL;
SDL_Surface *coinSpriteSheet = NULL;

SDL_Rect enemyClips1[NUM_FRAMES_ENEMY1];
SDL_Rect enemyClips2[NUM_FRAMES_ENEMY2];
SDL_Rect enemyClips3[NUM_FRAMES_ENEMY3];
SDL_Rect coinClips[NUM_FRAMES_COIN];
void loadEnemyClips() {
    // Define clips for the first enemy character animation
    for (int i = 0; i < NUM_FRAMES_ENEMY1; ++i) {
        enemyClips1[i].x = i * SPRITE_WIDTH_ENEMY1;
        enemyClips1[i].y = 0;
        enemyClips1[i].w = SPRITE_WIDTH_ENEMY1;
        enemyClips1[i].h = SPRITE_HEIGHT_ENEMY1;
    }

    // Define clips for the second enemy character animation
    for (int i = 0; i < NUM_FRAMES_ENEMY2; ++i) {
        enemyClips2[i].x = i * SPRITE_WIDTH_ENEMY2;
        enemyClips2[i].y = SPRITE_HEIGHT_ENEMY1;
        enemyClips2[i].w = SPRITE_WIDTH_ENEMY2;
        enemyClips2[i].h = SPRITE_HEIGHT_ENEMY2;
    }

    // Define clips for the third enemy character animation
    for (int i = 0; i < NUM_FRAMES_ENEMY3; ++i) {
        enemyClips3[i].x = i * SPRITE_WIDTH_ENEMY3;
        enemyClips3[i].y = 2 * SPRITE_HEIGHT_ENEMY1;
        enemyClips3[i].w = SPRITE_WIDTH_ENEMY3;
        enemyClips3[i].h = SPRITE_HEIGHT_ENEMY3;
    }

    // Define clips for the coin animation
    for (int i = 0; i < NUM_FRAMES_COIN; ++i) {
        coinClips[i].x = i * SPRITE_WIDTH_COIN;
        coinClips[i].y = 0;
        coinClips[i].w = SPRITE_WIDTH_COIN;
        coinClips[i].h = SPRITE_HEIGHT_COIN;
    }
}

void loadSpriteSheet(char *filename) {
    spriteSheet = IMG_Load(filename);
    if (!spriteSheet) {
        printf("Unable to load sprite sheet! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void loadCoinSpriteSheet(char *filename) {
    coinSpriteSheet = IMG_Load(filename);
    if (!coinSpriteSheet) {
        printf("Unable to load coin sprite sheet! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void applySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, clip, destination, &offset);
}

int checkCollision(SDL_Rect a, SDL_Rect b) {
    if (a.x < b.x + b.w - COLLISION_THRESHOLD &&
        a.x + a.w - COLLISION_THRESHOLD > b.x &&
        a.y < b.y + b.h - COLLISION_THRESHOLD &&
        a.y + a.h - COLLISION_THRESHOLD > b.y) {
        return 1;
    }
    return 0;
}

