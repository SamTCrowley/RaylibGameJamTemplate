#pragma once

#include <vector>
#include <initializer_list>
#include "raylib.h"
#include "raymath.h"

class BlockGrid{
    public:
    int blocks_wide;
    int blocks_long;
    int texes_wide;
    float tex_u_width;
    std::vector<int> blocks;
    std::vector<Vector3> cube_coords;

    BlockGrid();
    Model build_model();


};
