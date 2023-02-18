#pragma once

#include <array>

class Entity{
    int x;
    int y;
    int dim;
    std::array< int, 3> color;
    bool target;

    int c_x, c_y;

public:
    Entity() { }

    Entity(int _x, int _y, int _dim, std::array< int, 3 > _color, bool _target)
        : x(_x), y(_y), dim(_dim), color(_color), target(_target)
    {
        c_x = x + dim / 2;
        c_y = y + dim / 2;
    }

    int get_x();
    int get_y();
    int get_cx();
    int get_cy();
    int get_dim();
    bool is_target();
    std::array< int, 3 > get_color();

    bool point_in_entity(int, int);
    void set_new_coords(int, int);
};