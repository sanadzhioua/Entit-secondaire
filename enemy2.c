#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define PLAYER_SPRITE_WIDTH 121
#define PLAYER_SPRITE_HEIGHT 134
#define PLAYER_NUM_FRAMES 4

#define ENEMY1_SPRITE_WIDTH 121
#define ENEMY1_SPRITE_HEIGHT 134
#define ENEMY1_NUM_FRAMES 4

#define ENEMY2_SPRITE_WIDTH 150
#define ENEMY2_SPRITE_HEIGHT 134
#define ENEMY2_NUM_FRAMES 3

#define ENEMY3_SPRITE_WIDTH 150
#define ENEMY3_SPRITE_HEIGHT 134
#define ENEMY3_NUM_FRAMES 3

#define FRAME_DURATION 100 // in milliseconds
#define FAST_FRAME_DURATION 50 // in milliseconds
#define SLOW_FRAME_DURATION 150 // in milliseconds
#define PLAYER_SLOW_FRAME_DURATION 150 // Adjust this value to change the speed of the player's animation
#define ENEMY_SLOW_FRAME_DURATION 150 // Adjust this value to change the speed of the enemy's animation

SDL_Surface *screen = NULL;
SDL_Surface *spriteSheet = NULL;

SDL_Rect playerClips[PLAYER_NUM_FRAMES];
SDL_Rect enemy1Clips[ENEMY1_NUM_FRAMES];
SDL_Rect enemy2Clips[ENEMY2_NUM_FRAMES];
SDL_Rect enemy3Clips[ENEMY3_NUM_FRAMES];

void initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Walking Character Animation", NULL);
}

void loadMedia() {
    spriteSheet = IMG_Load("spritesheet2.png");
    if (!spriteSheet) {
        printf("Failed to load sprite sheet: %s\n", SDL_GetError());
        exit(1);
    }

    // Define clips for the player character animation
    for (int i = 0; i < PLAYER_NUM_FRAMES; ++i) {
        playerClips[i].x = i * PLAYER_SPRITE_WIDTH;
        playerClips[i].y = 0;
        playerClips[i].w = PLAYER_SPRITE_WIDTH;
        playerClips[i].h = PLAYER_SPRITE_HEIGHT;
    }

    // Define clips for the first type of enemy character animation
    for (int i = 0; i < ENEMY1_NUM_FRAMES; ++i) {
        enemy1Clips[i].x = i * ENEMY1_SPRITE_WIDTH;
        enemy1Clips[i].y = PLAYER_SPRITE_HEIGHT; // Height of player character
        enemy1Clips[i].w = ENEMY1_SPRITE_WIDTH;
        enemy1Clips[i].h = ENEMY1_SPRITE_HEIGHT;
    }

    // Define clips for the second type of enemy character animation
    for (int i = 0; i < ENEMY2_NUM_FRAMES; ++i) {
        enemy2Clips[i].x = i * ENEMY2_SPRITE_WIDTH;
        enemy2Clips[i].y = 2 * PLAYER_SPRITE_HEIGHT; // Height of player and first enemy type
        enemy2Clips[i].w = ENEMY2_SPRITE_WIDTH;
        enemy2Clips[i].h = ENEMY2_SPRITE_HEIGHT;
    }

    // Define clips for the third type of enemy character animation
    for (int i = 0; i < ENEMY3_NUM_FRAMES; ++i) {
        enemy3Clips[i].x = i * ENEMY3_SPRITE_WIDTH;
        enemy3Clips[i].y = 3 * PLAYER_SPRITE_HEIGHT; // Height of player, first and second enemy types
        enemy3Clips[i].w = ENEMY3_SPRITE_WIDTH;
        enemy3Clips[i].h = ENEMY3_SPRITE_HEIGHT;
    }
}

void drawCharacter(int x, int frame, int line) {
    SDL_Rect destRect;

    destRect.x = x;
    destRect.y = line == 1 ? (SCREEN_HEIGHT - PLAYER_SPRITE_HEIGHT) / 2 :
                 line == 2 ? (SCREEN_HEIGHT - ENEMY1_SPRITE_HEIGHT) / 2 :
                 line == 3 ? (SCREEN_HEIGHT - ENEMY2_SPRITE_HEIGHT) / 2 :
                             (SCREEN_HEIGHT - ENEMY3_SPRITE_HEIGHT) / 2; // Center vertically

    SDL_BlitSurface(spriteSheet, line == 1 ? &playerClips[frame] :
                                 line == 2 ? &enemy1Clips[frame] :
                                 line == 3 ? &enemy2Clips[frame] :
                                             &enemy3Clips[frame], screen, &destRect);
}

// New function to handle character movement when reaching the end of the screen
void moveLeft(int *characterX, int *moveRight, int *frame4) {
    *moveRight = 0; // Change direction to left
    *characterX = -1; // Move to the left of the screen
    *frame4 = 0; // Reset frame for the fourth line character
}

int main(int argc, char* args[]) {
    srand(time(NULL)); // Seed the random number generator

    initSDL();
    loadMedia();

    int quit = 0;
    SDL_Event event;

    int characterX = 0;
    int frame1 = 0;
    int frame2 = 0;
    int frame3 = 0;
    int frame4 = 0;
    Uint32 startTime = 0;

    int moveRight = 1; // Flag to indicate the direction of movement
    int mouseMoving = 0; // Flag to indicate mouse movement

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_MOUSEMOTION) {
                // Set mouseMoving flag when mouse motion detected
                mouseMoving = 1;
            }
        }

        // Get mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check if mouse is close to the player character
        if (abs(mouseX - (characterX + PLAYER_SPRITE_WIDTH / 2)) < 100) { // Adjusted distance here
            moveRight = 0; // Stop character movement
        } else if (mouseMoving) {
            moveRight = 1; // Resume character movement only if the mouse is moving
        }

        // Update character animations
        if (SDL_GetTicks() - startTime >= (moveRight ? FRAME_DURATION : SLOW_FRAME_DURATION)) {
            if (moveRight) {
                frame1++;
                if (frame1 >= PLAYER_NUM_FRAMES) {
                    frame1 = 0; // Reset to the first frame
                }
            } else {
                frame3++;
                if (frame3 >= ENEMY2_NUM_FRAMES) {
                    frame3 = 0; // Reset to the first frame
                }
            }
            startTime = SDL_GetTicks();
        }

        // Drawing
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0)); // Clear screen
        drawCharacter(characterX, moveRight ? frame1 : frame3, moveRight ? 1 : 3); // Draw character based on direction of movement
        SDL_Flip(screen); // Update screen

        // Move character
        if (moveRight) {
            characterX += 2; // Move 2 pixels to the right
            if (characterX >= SCREEN_WIDTH) {
                moveLeft(&characterX, &moveRight, &frame4); // Move character to the left when it reaches the end of the screen
            }
        } else {
            characterX -= 2; // Move 2 pixels to the left
            if (characterX <= -PLAYER_SPRITE_WIDTH) {
                characterX = -1; // Move to the left of the screen
                moveRight = -1; // Change direction to right
            }
        }

        SDL_Delay(7); // Delay for smooth animation
    }

    SDL_FreeSurface(spriteSheet);
    SDL_Quit();
    return 0;
}
