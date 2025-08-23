#include <x/sdl_backend.h>
#include <stdbool.h>

sdl_context_t* sdl_open(const char* title, int x, int y, int w, int h, int flag){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Window *window = SDL_CreateWindow(title,x, y,w, h,SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return NULL;
	}

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

    sdl_context_t* ctx = malloc(sizeof(sdl_context_t));
    ctx->window = window;
    ctx->renderer = renderer;
    ctx->surface = surface;
    ctx->framebuffer = malloc(w*h*4);
    ctx->width = w;
    ctx->height = h;
    ctx->format = surface->format;
	return ctx;
}

void sdl_render(sdl_context_t* ctx){
    // 将framebuffer转换为纹理以便渲染
    SDL_LockSurface(ctx->surface);
    memcpy(ctx->surface->pixels, ctx->framebuffer, ctx->width * ctx->height * 4);
    SDL_UnlockSurface(ctx->surface);


    SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx->renderer, ctx->surface);
    if (texture == NULL) {
        printf("纹理创建失败! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(ctx->surface);
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        SDL_Quit();
        return;
    }

    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
    SDL_DestroyTexture(texture);
}

void sdl_close(sdl_context_t *ctx) {
    if(ctx){
        free(ctx->framebuffer);
        SDL_FreeSurface(ctx->surface);
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        SDL_Quit();
        free(ctx);
    }
}