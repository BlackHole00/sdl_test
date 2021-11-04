#pragma once
#include <SDL2/SDL.h>
#include <vx_types.h>
#include <vx_default.h>
#include <vx_functions.h>

typedef void* vx_UserStatePtr;

typedef struct {
    char* title;

    i32 window_x_pos;
    i32 window_y_pos;
    i32 window_width;
    i32 window_height;

    struct {
        bool accelerated: 1;
        bool vsync: 1;
    } renderer_flags;
    struct {
        bool fullscreen: 1;
        bool hidden: 1;
        bool resizable: 1;
        bool minimized: 1;
        bool maximized: 1;
        /*  Only simple flags implemented.
            Look at SDL_WindowFlags for implementing.
        */
    } window_flags;

    VX_CALLBACK(vx_UserStatePtr, SDL_Renderer*) init_fn;
    VX_CALLBACK(vx_UserStatePtr, SDL_Renderer*, f64) logic_fn;
    VX_CALLBACK(const vx_UserStatePtr, SDL_Renderer*) draw_fn;
    VX_CALLBACK(vx_UserStatePtr, SDL_Renderer*) close_fn;
} vx_WindowDescriptor;

VX_CREATE_DEFAULT(vx_WindowDescriptor, 
    .title = "Window",
    .window_x_pos = 100,
    .window_y_pos = 100,
    .window_width = 640,
    .window_height = 480,
    .renderer_flags.accelerated = true,
    .renderer_flags.vsync = true,
    .window_flags.fullscreen = false,
    .window_flags.maximized = false,
    .init_fn = NULL,
    .logic_fn = NULL,
    .draw_fn = NULL,
    .close_fn = NULL
)

typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    struct {
        vx_UserStatePtr data_ptr;

        VX_CALLBACK(vx_UserStatePtr, SDL_Renderer*) init_fn;
        VX_CALLBACK(vx_UserStatePtr, SDL_Renderer*, f64) logic_fn;
        VX_CALLBACK(const vx_UserStatePtr, SDL_Renderer*) draw_fn;
        VX_CALLBACK(vx_UserStatePtr, SDL_Renderer*) close_fn;
    } user_data;
} vx_Window;

vx_Window vx_window_new(const vx_WindowDescriptor* descriptor, vx_UserStatePtr data_ptr);
void vx_window_free(vx_Window* window);

void vx_window_run(vx_Window* window);