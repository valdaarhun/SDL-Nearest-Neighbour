#pragma once

#include <memory>
#include <vector>

#include "entity.hpp"

class Entity_list{
    std::vector< std::shared_ptr< Entity > > entities;

    std::shared_ptr< Entity > find_target();

public:
    void push(std::shared_ptr< Entity >);
    std::vector< std::shared_ptr< Entity > > get_entities();

    int get_nearest_neighbour(int *, int *, int *, int *);

    std::shared_ptr < Entity > check_cursor_over_entity(int, int);
};