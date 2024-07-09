#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SPRITE_WIDTH_1 170
#define SPRITE_HEIGHT_1 146
#define NUM_FRAMES_1 4

#define SPRITE_WIDTH_2 170
#define SPRITE_HEIGHT_2 146
#define NUM_FRAMES_2 4

#define FRAME_DURATION 100 // in milliseconds
#define FAST_FRAME_DURATION 50 // in milliseconds

SDL_Surface *screen = NULL;
SDL_Surface *spriteSheet = NULL;

SDL_Rect clips_1[NUM_FRAMES_1];
SDL_Rect clips_2[NUM_FRAMES_2];

void initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Walking Character Animation", NULL);
}

void loadMedia() {
    spriteSheet = IMG_Load("8rab.png");
    if (!spriteSheet) {
        printf("Failed to load sprite sheet: %s\n", SDL_GetError());
        exit(1);
    }

    // Define clips for the walking character animation (first line)
    for (int i = 0; i < NUM_FRAMES_1; ++i) {
        clips_1[i].x = i * SPRITE_WIDTH_1;
        clips_1[i].y = 0;
        clips_1[i].w = SPRITE_WIDTH_1;
        clips_1[i].h = SPRITE_HEIGHT_1;
    }

    // Define clips for the second line of characters
    for (int i = 0; i < NUM_FRAMES_2; ++i) {
        clips_2[i].x = i * SPRITE_WIDTH_2;
        clips_2[i].y = SPRITE_HEIGHT_1; // Height of first line of characters
        clips_2[i].w = SPRITE_WIDTH_2;
        clips_2[i].h = SPRITE_HEIGHT_2;
    }
}

void drawCharacter(int x, int frame, int line) {
    SDL_Rect destRect;

    destRect.x = x;
    destRect.y = line == 1 ? (SCREEN_HEIGHT - SPRITE_HEIGHT_1) / 2 : (SCREEN_HEIGHT - SPRITE_HEIGHT_2) / 2; // Center vertically
    SDL_BlitSurface(spriteSheet, line == 1 ? &clips_1[frame] : &clips_2[frame], screen, &destRect);
}

double getAngle(int mouseX, int characterX) {
    return atan2((double)(SCREEN_HEIGHT / 2 - mouseX), (double)(characterX - SCREEN_WIDTH / 2));
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
    Uint32 startTime = 0;

    int moveRight = 1; // Flag to indicate the direction of movement

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Get mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Update character animations based on mouse position
        double angle = getAngle(mouseX, characterX);
        if (angle < 0) {
            frame1++;
            if (frame1 >= NUM_FRAMES_1) {
                frame1 = 0; // Reset to the first frame
            }
        } else {
            frame2++;
            if (frame2 >= NUM_FRAMES_2) {
                frame2 = 0; // Reset to the first frame
            }
        }

        // Drawing
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0)); // Clear screen
        drawCharacter(characterX, angle < 0 ? frame1 : frame2, angle < 0 ? 1 : 2); // Draw character based on mouse position
        SDL_Flip(screen); // Update screen

        // Move character
        if (angle < 0) {
            characterX += 2; // Move 2 pixels to the right
        } else {
            characterX -= 2; // Move 2 pixels to the left
        }

        // Change direction when reaching the screen edges
        if (characterX >= SCREEN_WIDTH) {
            moveRight = 0; // Change direction to left
        } else if (characterX <= -SPRITE_WIDTH_1) {
            moveRight = 1; // Change direction to right
        }

        SDL_Delay(10); // Delay for smooth animation
    }

    SDL_FreeSurface(spriteSheet);
    SDL_Quit();
    return 0;
}

