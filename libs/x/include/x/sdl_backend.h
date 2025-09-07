#ifndef X_SDL_BACKEND_H
#define X_SDL_BACKEND_H

#include <SDL2/SDL.h>


typedef struct sdl_window{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_PixelFormat *format;
    uint32_t *framebuffer;
    int width;
    int height;
    struct sdl_window *next;
    struct sdl_window *prev;
    void *data;
}sdl_window_t;

typedef struct {
    sdl_window_t *windows;
    int window_count;
}sdl_context_t;

sdl_context_t *sdl_init(void);
sdl_window_t* sdl_open(sdl_context_t* ctx, const char* title, int x, int y, int w, int h, int flag);
void sdl_render(sdl_context_t* ctx);
void sdl_close(sdl_context_t *ctx, sdl_window_t* win);
void sdl_quit(sdl_context_t* ctx);
sdl_window_t* sdl_get_window(sdl_context_t* ctx, int id);
#endif