#include "window.h"
#include <vx_panic.h>

static bool sdl_initialized = false;
static int init_sdl(u32 flags) {
    if (!sdl_initialized) {
        sdl_initialized = true;
        return SDL_Init(flags);
    }

    return 0;
}
static void deinit_sdl() {
    if (sdl_initialized) {
        sdl_initialized = false;
        SDL_Quit();
    }
}

static u32 gen_window_flags(const vx_WindowDescriptor* descriptor) {
    u32 flags = 0;

    if (descriptor->window_flags.fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (descriptor->window_flags.resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    if (descriptor->window_flags.minimized) {
        flags |= SDL_WINDOW_MINIMIZED;
    }
    if (descriptor->window_flags.maximized) {
        flags |= SDL_WINDOW_MAXIMIZED;
    }
    flags |= ((descriptor->window_flags.hidden) ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN);

    return flags;
}

static u32 gen_renderer_flags(const vx_WindowDescriptor* descriptor) {
    u32 flags = 0;
    
    if (descriptor->renderer_flags.vsync) {
        flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    flags |= ((descriptor->renderer_flags.accelerated) ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE);

    return flags;
}

vx_Window vx_window_new(const vx_WindowDescriptor* descriptor, vx_UserStatePtr data_ptr) {
    VX_ASSERT("Could not init SDL", init_sdl(SDL_INIT_EVERYTHING) == 0);
    
    SDL_Window* win = SDL_CreateWindow(
        descriptor->title, 
        descriptor->window_x_pos, 
        descriptor->window_y_pos,
        descriptor->window_width,
        descriptor->window_height,
        gen_window_flags(descriptor)
    );
    VX_ASSERT("Could not create a SDL window", win != NULL);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, gen_renderer_flags(descriptor));
    VX_ASSERT_EXIT_OP("Could not create renderer", ren != NULL, 
		SDL_DestroyWindow(win); 
		deinit_sdl();
	);

    return (vx_Window){
        .sdl_window = win,
        .sdl_renderer = ren,
        .user_data = {
            .data_ptr = data_ptr,
            .init_fn = descriptor->init_fn,
            .logic_fn = descriptor->logic_fn,
            .draw_fn = descriptor->draw_fn,
            .close_fn = descriptor->close_fn
        }
    };
}

void vx_window_free(vx_Window* window) {
    SDL_DestroyRenderer(window->sdl_renderer);
	SDL_DestroyWindow(window->sdl_window);
    deinit_sdl();
}

void vx_window_run(vx_Window* window) {
    SDL_Event event;
    bool should_quit = false;

    window->user_data.init_fn(window->user_data.data_ptr, window->sdl_renderer);

    while (!should_quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                should_quit = true;
            }
        }

        window->user_data.logic_fn(window->user_data.data_ptr, window->sdl_renderer, 0.0f);
        window->user_data.draw_fn(window->user_data.data_ptr, window->sdl_renderer);

        SDL_RenderClear(window->sdl_renderer);
		SDL_RenderPresent(window->sdl_renderer);
    }

    window->user_data.close_fn(window->user_data.data_ptr, window->sdl_renderer);
}