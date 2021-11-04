#include <stdio.h>
#include <engine.h>
#include <vx_panic.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
	int i;
} gm_State;

void gm_init(gm_State* state, SDL_Renderer* renderer) {
	state->i = 0;
	SDL_SetRenderDrawColor(renderer, 255, 64, 128, 255);
}
void gm_logic(gm_State* state, SDL_Renderer* renderer, f64 delta) {}
void gm_draw(gm_State* state, SDL_Renderer* renderer) {}
void gm_close(gm_State* state, SDL_Renderer* renderer) {}

int main() {
	vx_WindowDescriptor descriptor = VX_DEFAULT(vx_WindowDescriptor);
	descriptor.init_fn = (vx_Callback)gm_init;
	descriptor.logic_fn = (vx_Callback)gm_logic;
	descriptor.draw_fn = (vx_Callback)gm_draw;
	descriptor.close_fn = (vx_Callback)gm_close;

	gm_State state;
	vx_Window window = vx_window_new(&descriptor, &state);
	vx_window_run(&window);
	vx_window_free(&window);
}
