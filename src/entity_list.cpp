#include <memory>
#include <limits>
#include <vector>
#include <stdio.h>

#include "entity_list.hpp"
#include "entity.hpp"

void Entity_list::push(std::shared_ptr< Entity > entity){
    entities.emplace_back(entity);
}

std::vector< std::shared_ptr< Entity > > Entity_list::get_entities(){
    return entities;
}

std::shared_ptr< Entity > Entity_list::find_target(){
    for(auto entity: entities){
        if (entity -> is_target()){
            return entity;
        }
    }

    return nullptr;
}

int Entity_list::get_nearest_neighbour(int *x, int *y, int *_x, int *_y){
    std::shared_ptr< Entity > ptr = find_target();
    *_x = ptr -> get_cx();
    *_y = ptr -> get_cy();

    int dist_squared = std::numeric_limits< int >::max();

    for(auto entity: entities){
        if (entity -> is_target()) continue;

        int e_x = entity -> get_cx(), e_y = entity -> get_cy();
        int dist = (e_x - *_x) * (e_x - *_x) + (e_y - *_y) * (e_y - *_y);
        if (dist < dist_squared){
            *x = e_x;
            *y = e_y;
            dist_squared = dist;
        }
    }

    return dist_squared;
}

std::shared_ptr < Entity > Entity_list::check_cursor_over_entity(int x, int y){
    for(auto entity: entities){
        if (entity -> point_in_entity(x, y)){
            return entity;
        }
    }
    return nullptr;
}