#pragma once

#include <random>

inline int generate_random_number(int a, int b){
    std::random_device r;
    std::mt19937 mt(r());
    std::uniform_int_distribution< int > distribution(a, b);

    return distribution(mt);
}