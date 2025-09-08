#include <x/sdl_backend.h>
#include <stdbool.h>

sdl_context_t* sdl_init(void){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}

    sdl_context_t* ctx = malloc(sizeof(sdl_context_t));
    ctx->windows = NULL; 
    ctx->window_count = 0;
	return ctx;
}

sdl_window_t* sdl_open(sdl_context_t* ctx, const char* title, int x, int y, int w, int h, int flag){    

	SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}

    SDL_SetWindowBordered(window, SDL_TRUE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("渲染器创建失败! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }

    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (surface == NULL) {
        printf("Framebuffer创建失败! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    sdl_window_t* win = malloc(sizeof(sdl_window_t));
    win->window = window;
    win->renderer = renderer;
    win->surface = surface;
    win->framebuffer = malloc(w*h*4);
    win->width = w;
    win->height = h;
    win->format = surface->format;
    win->prev = NULL;
    win->next = NULL;

    if(ctx->windows == NULL){
        ctx->windows = win;
    }else{
        sdl_window_t* cur = ctx->windows;
        while(cur->next){
            cur = cur->next;
        }
        cur->next = win;
        win->prev = cur;
    }
    ctx->window_count++;
    return win;
}

void sdl_render(sdl_context_t* ctx){
    for(sdl_window_t* win = ctx->windows; win; win = win->next){
        SDL_LockSurface(win->surface);
        memcpy(win->surface->pixels, win->framebuffer, win->width * win->height * 4);
        SDL_UnlockSurface(win->surface);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(win->renderer, win->surface);
        if (texture == NULL) {
            printf("纹理创建失败! SDL_Error: %s\n", SDL_GetError());
            SDL_FreeSurface(win->surface);
            SDL_DestroyRenderer(win->renderer);
            SDL_DestroyWindow(win->window);
            SDL_Quit();
            return;
        }
        SDL_RenderClear(win->renderer);
        SDL_RenderCopy(win->renderer, texture, NULL, NULL);
        SDL_RenderPresent(win->renderer);
        SDL_DestroyTexture(texture);
    }
}

sdl_window_t* sdl_get_window(sdl_context_t* ctx, int id){
    for(sdl_window_t* w = ctx->windows; w; w = w->next){
        if(SDL_GetWindowID(w->window) == id){
            return w;
        }
    }
    return NULL;
}

void sdl_close(sdl_context_t* ctx, sdl_window_t *win) {
    if(win->prev){
        win->prev->next = win->next;
    }
    if(win->next){
        win->next->prev = win->prev;
    }
    if(ctx->windows == win){
        ctx->windows = win->next;
    }
    ctx->window_count--;
    free(win->framebuffer);
    SDL_FreeSurface(win->surface);
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->window);
    free(win);
}

void sdl_quit(sdl_context_t* ctx) {
    for(sdl_window_t* win = ctx->windows; win; win = win->next)
    {
        free(win->framebuffer);
        SDL_FreeSurface(win->surface);
        SDL_DestroyRenderer(win->renderer);
        SDL_DestroyWindow(win->window);
    }
    free(ctx);
    SDL_Quit();
}
