#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int CHAR_WIDTH = 50;
const int CHAR_HEIGHT = 50;
const int ENEMY_WIDTH = 50;
const int ENEMY_HEIGHT = 50;

// Structure representing a game object
struct GameObject {
    SDL_Rect rect;
    int dx, dy;
};

// Initialize SDL and the window
bool init(SDL_Surface*& screen) {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (!screen) {
        std::cerr << "Unable to set SDL video mode: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_WM_SetCaption("Bounding Box Collision SDL 1.2", nullptr);

    return true;
}

// Load an image from file
SDL_Surface* loadImage(const std::string& filename) {
    SDL_Surface* imageLoaded = IMG_Load(filename.c_str());
    if (!imageLoaded) {
        std::cerr << "Unable to load image " << filename << ": " << IMG_GetError() << std::endl;
    }
    return imageLoaded;
}

// Check if two rectangles overlap
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return a.x + a.w >= b.x &&
           b.x + b.w >= a.x &&
           a.y + a.h >= b.y &&
           b.y + b.h >= a.y;
}

int main(int argc, char* args[]) {
    SDL_Surface* screen = nullptr;

    if (!init(screen)) {
        return 1;
    }

    // Load character and enemy images
    SDL_Surface* characterImage = loadImage("character.png");
    SDL_Surface* enemyImage = loadImage("enemy.png");
    if (!characterImage || !enemyImage) {
        return 1;
    }

    // Create game objects
    GameObject character = {{100, 100, CHAR_WIDTH, CHAR_HEIGHT}, 1, 1}; // Character
    GameObject enemy = {{200, 200, ENEMY_WIDTH, ENEMY_HEIGHT}, -1, -1}; // Enemy

    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update object positions
        character.rect.x += character.dx;
        character.rect.y += character.dy;

        enemy.rect.x += enemy.dx;
        enemy.rect.y += enemy.dy;

        // Check collision between character and enemy
        if (checkCollision(character.rect, enemy.rect)) {
            std::cout << "Collision detected!" << std::endl;
            // Handle collision here
            // For example, stop the game or reduce player health
        }

        // Clear the screen
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

        // Render character and enemy
        SDL_BlitSurface(characterImage, nullptr, screen, &character.rect);
        SDL_BlitSurface(enemyImage, nullptr, screen, &enemy.rect);

        // Update the screen
        SDL_Flip(screen);

        // Delay to control refresh rate
        SDL_Delay(10);
    }

    // Free memory and quit SDL
    SDL_FreeSurface(characterImage);
    SDL_FreeSurface(enemyImage);
    SDL_Quit();

    return 0;
}

