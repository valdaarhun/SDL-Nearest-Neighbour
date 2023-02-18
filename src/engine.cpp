#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <memory>
#include <vector>
#include <array>

#include "engine.hpp"
#include "entity.hpp"
#include "utils.hpp"


extern int height, width;

std::shared_ptr< Entity > entity_ptr;
bool entity_selected = false;
int nearest_x, nearest_y, target_x, target_y, shortest_dist;

int Engine::init(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if ((window = SDL_CreateWindow("Nearest Neighbour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE)) == NULL){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

void Engine::setup_entities(){
    int num_entities = generate_random_number(50, 100);
    for(int i = 0 ; i < num_entities + 1 ; i++){
        int x = generate_random_number(10, width - 10);
        int y = generate_random_number(10, height - 10);

        std::array< int, 3 > color = {0, 0, 0};
        bool target;
        if (i == num_entities){
            color[1] = 255;
            target = true;
        }
        else{
            color[0] = 255;
            target = false;
        }

        std::shared_ptr< Entity > entity = std::make_shared< Entity >(x, y, 10, color, target);
        entities.push(entity);
    }
}

void Engine::poll_event(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_Q){
                is_running = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT){
                entity_ptr = entities.check_cursor_over_entity(event.button.x, event.button.y);
                if (entity_ptr != nullptr){
                    entity_selected = true;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT){
                entity_selected = false;
            }
            break;
        case SDL_MOUSEMOTION:
            if (entity_selected){
                entity_ptr -> set_new_coords(event.button.x, event.button.y);
            }
            break;
        default:
            break;
        }
    }
}

int Engine::render_entities(){
    std::vector< std::shared_ptr< Entity > > entity_list = entities.get_entities();
    for(auto entity: entity_list){
        SDL_Rect rect = {
            .x = entity -> get_x(),
            .y = entity -> get_y(),
            .w = entity -> get_dim(),
            .h = entity -> get_dim(),
        };
        std::array< int, 3 > color = entity -> get_color();

        if (entity -> is_target() && shortest_dist < 900){
            color = {0, 102, 204};
        }

        if (SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], SDL_ALPHA_OPAQUE) < 0){
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }

        if (SDL_RenderFillRect(renderer, &rect) < 0){
            fprintf(stderr, "%s\n", SDL_GetError());
            return -1;
        }
    }

    return 0;
}

int Engine::render_circle(int x, int y){
    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm#Variant_with_integer-based_arithmetic
    int _x = 30;
    int _y = 0;
    int r = 30;
    int err = 3 - 2 * r;
    while(_x >= _y){
        SDL_RenderDrawPoint(renderer, x + _x, y + _y);
        SDL_RenderDrawPoint(renderer, x - _x, y + _y);
        SDL_RenderDrawPoint(renderer, x + _x, y - _y);
        SDL_RenderDrawPoint(renderer, x - _x, y -_y);
        SDL_RenderDrawPoint(renderer, x + _y, y + _x);
        SDL_RenderDrawPoint(renderer, x - _y, y + _x);
        SDL_RenderDrawPoint(renderer, x + _y, y - _x);
        SDL_RenderDrawPoint(renderer, x - _y, y - _x);

        if (err > 0){
            err += (5 - 2 * _x + 2 * _y) << 1;
            _x--;
        }
        else{
            err += (3 + 2 * _y) << 1;
        }
        _y++;
    }
}

void Engine::update(){
    shortest_dist = entities.get_nearest_neighbour(&nearest_x, &nearest_y, &target_x, &target_y);
}

int Engine::render_smallest_distance(){
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE) < 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (SDL_RenderDrawLine(renderer, nearest_x, nearest_y, target_x, target_y) < 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 102, 204, SDL_ALPHA_OPAQUE) < 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    render_circle(nearest_x, nearest_y);

    return 0;
}

int Engine::update_render(){
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) < 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (SDL_RenderClear(renderer) < 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    if (render_entities() < 0){
        return -1;
    }

    render_smallest_distance();

    SDL_RenderPresent(renderer);

    return 0;
}

void Engine::destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Engine::run(){
    if (init() < 0) return -1;
    setup_entities();

    Uint32 prev = 0, now = 0;
    bool err = false;
    while(is_running){
        poll_event();

        now = SDL_GetTicks();
        if (now - prev > 10){
            update();
            if (update_render() < 0){
                err = true;
                break;
            }
            prev = now;
        }
    }

    destroy();

    return err ? -1 : 0;
}