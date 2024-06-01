#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int DISPLAY_TIME = 5000; // ms

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Image Loader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gTexture = NULL;
    gRenderer = NULL;
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

bool loadMedia(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return false;
    }

    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (gTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        return false;
    }

    return true;
}

void render() {
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
    SDL_RenderPresent(gRenderer);
}

void closeTexture() {
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
}

void showImagesInDirectory(const char* directoryPath) {
    DIR* directory = opendir(directoryPath);
    if (directory == NULL) {
        printf("Unable to open directory %s\n", directoryPath);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {

        if (entry->d_type != DT_REG || entry->d_name[0] == '.') {
            continue;
        }

        const char* fileName = entry->d_name;
        const char* extension = strrchr(fileName, '.');
        if (extension != NULL && (strcasecmp(extension, ".jpg") == 0 || strcasecmp(extension, ".bmp") == 0 || strcasecmp(extension, ".png") == 0)) {
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, fileName);
            if (!loadMedia(filePath)) {
                printf("Failed to load media: %s\n", filePath);
                continue;
            }

            render();
            SDL_Delay(DISPLAY_TIME);
            closeTexture();
        }
    }

    closedir(directory);
}

int main(int argc, char* args[]) {
    if (argc < 2) {
        printf("Usage: %s <path_to_image_or_directory>\n", args[0]);
        return 1;
    }

    if (!init()) {
        printf("Failed to initialize!\n");
        return 1;
    }

    struct stat path_stat;
    if (stat(args[1], &path_stat) != 0) {
        printf("Error accessing path %s\n", args[1]);
        closeSDL();
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) {

        showImagesInDirectory(args[1]);

    } else if (S_ISREG(path_stat.st_mode)) {

        if (!loadMedia(args[1])) {
            printf("Failed to load media!\n");
            closeSDL();
            return 1;
        }

        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            render();
            SDL_Delay(16);
        }
    } else {
        printf("%s is not a regular file or directory\n", args[1]);
        closeSDL();
        return 1;
    }

    closeSDL();
    return 0;
}
