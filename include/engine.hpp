#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "entity_list.hpp"

class Engine{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;

    Entity_list entities;

    int init();
    void setup_entities();
    int render_entities();
    void poll_event();
    int render_circle(int, int);
    int render_smallest_distance();
    void update();
    int update_render();
    void destroy();

public:
    Engine()
        : window(NULL), renderer(NULL), is_running(true) {}

    int run();
};