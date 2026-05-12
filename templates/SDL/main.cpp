#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>
#include <stdexcept>

static constexpr int WINDOW_W = 1280;
static constexpr int WINDOW_H = 720;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        throw std::runtime_error(SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(
        "SDL3 Project",
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_RESIZABLE
    );
    if (!window)
        throw std::runtime_error(SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
        throw std::runtime_error(SDL_GetError());

    bool running = true;
    SDL_Event event;

    while (running)
    {
        // ── Events ──────────────────────────────
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.key == SDLK_ESCAPE)
                running = false;
        }

        // ── Render ──────────────────────────────
        SDL_SetRenderDrawColorFloat(renderer, 0.10f, 0.12f, 0.18f, 1.0f);
        SDL_RenderClear(renderer);

        // Przykładowy prostokąt na środku
        SDL_FRect rect{
            WINDOW_W / 2.0f - 100.0f,
            WINDOW_H / 2.0f - 60.0f,
            200.0f,
            120.0f
        };
        SDL_SetRenderDrawColorFloat(renderer, 0.20f, 0.60f, 1.0f, 1.0f);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
