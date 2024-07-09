#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "game.h"

Coin coins[SCREEN_WIDTH / (SPRITE_WIDTH_ENEMY1 + COIN_DISTANCE)];



int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("SDL could not set video mode! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    loadSpriteSheet("spritesheet2.png");
    loadEnemyClips();
    loadCoinSpriteSheet("coincbon.png");

    for (int i = 0; i < SCREEN_WIDTH / (SPRITE_WIDTH_ENEMY1 + COIN_DISTANCE); ++i) {
        coins[i].x = i * (SPRITE_WIDTH_ENEMY1 + COIN_DISTANCE-10);
        coins[i].y = SCREEN_HEIGHT - SPRITE_HEIGHT_ENEMY1; // Set y-coordinate to be the same as the enemy's y-coordinate
        coins[i].exists = 1;
        coins[i].hitbox.x = coins[i].x;
        coins[i].hitbox.y = coins[i].y;
        coins[i].hitbox.w = SPRITE_WIDTH_COIN;
        coins[i].hitbox.h = SPRITE_HEIGHT_COIN;
    }

    int enemyX = (SCREEN_WIDTH - SPRITE_WIDTH_ENEMY1) / 2;
    int enemyY = SCREEN_HEIGHT - SPRITE_HEIGHT_ENEMY1;
    int enemyDirection = 1; // 1: move right, -1: move left
    int frame = 0;
    Uint32 startTime = 0;
    int enemyStopped = 0;

    while (1) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                return 0;
            }
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = { mouseX, mouseY, 1, 1 };

        // Check for collision between mouse and enemy hitbox
        SDL_Rect enemyHitbox = { enemyX, enemyY, SPRITE_WIDTH_ENEMY1, SPRITE_HEIGHT_ENEMY1 };
        if (checkCollision(enemyHitbox, mouseRect)) {
            enemyStopped = 1;
        } else {
            enemyStopped = 0;
        }

        // Handle coin collision
        for (int i = 0; i < SCREEN_WIDTH / (SPRITE_WIDTH_ENEMY1 + COIN_DISTANCE); ++i) {
            if (coins[i].exists && checkCollision(coins[i].hitbox, mouseRect)) {
                coins[i].exists = 0;
            }
        }

        // Move the enemy horizontally if it's not stopped
        if (!enemyStopped) {
            enemyX += ENEMY_SPEED * enemyDirection;
            if (enemyX <= 0) {
                enemyDirection = 1; // Change direction to right when reaching the left edge
                frame = 0; // Reset animation frame
            } else if (enemyX >= SCREEN_WIDTH - SPRITE_WIDTH_ENEMY1) {
                enemyDirection = -1; // Change direction to left when reaching the right edge
                frame = 0; // Reset animation frame
            }
        }

        // Update the enemy's animation based on its direction and mouse collision
        if (enemyDirection == 1) {
            if (enemyStopped) {
                applySurface(enemyX, enemyY, spriteSheet, screen, &enemyClips3[frame]); // Third line animation
            } else {
                applySurface(enemyX, enemyY, spriteSheet, screen, &enemyClips1[frame]); // First line animation
            }
        } else {
            if (enemyStopped) {
                applySurface(enemyX, enemyY, spriteSheet, screen, &enemyClips3[frame]); // Third line animation
            } else {
                applySurface(enemyX, enemyY, spriteSheet, screen, &enemyClips2[frame]); // Second line animation
            }
        }

        // Render coins
        for (int i = 0; i < SCREEN_WIDTH / (SPRITE_WIDTH_ENEMY1 + COIN_DISTANCE); ++i) {
            if (coins[i].exists) {
                applySurface(coins[i].x, coins[i].y, coinSpriteSheet, screen, &coinClips[frame]);
            }
        }

        if (SDL_Flip(screen) == -1) {
            printf("SDL could not flip screen! SDL Error: %s\n", SDL_GetError());
            return 1;
        }

        if (SDL_GetTicks() - startTime >= FRAME_DURATION) {
            frame++;

            if (frame >= NUM_FRAMES_ENEMY1) {
                frame = 0;
            }

            startTime = SDL_GetTicks();
        }

        SDL_Delay(10);
    }

    SDL_Quit();
    return 0;
}

