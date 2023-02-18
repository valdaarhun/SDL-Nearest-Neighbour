#include <array>

#include "entity.hpp"

extern int height, width;

int Entity::get_x(){
    return x;
}

int Entity::get_y(){
    return y;
}

int Entity::get_cx(){
    return c_x;
}

int Entity::get_cy(){
    return c_y;
}

int Entity::get_dim(){
    return dim;
}

bool Entity::is_target(){
    return target;
}

std::array< int, 3 > Entity::get_color(){
    return color;
}

bool Entity::point_in_entity(int _x, int _y){
    if (_x < c_x - dim / 2 || _x > c_x + dim / 2) return false;
    if (_y < c_y - dim / 2 || _y > c_y + dim / 2) return false;

    return true;
}

void Entity::set_new_coords(int _x, int _y){
    // int diff_x = _x - x, diff_y = _y - y;
    c_x = std::max(dim / 2, std::min(width - dim / 2, _x));
    c_y = std::max(dim / 2, std::min(height - dim / 2, _y));
    x = c_x - dim / 2;
    y = c_y - dim / 2;
}