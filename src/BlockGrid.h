#pragma once

#include <vector>
#include <initializer_list>
#include "raylib.h"
#include "raymath.h"

struct TexList{
    int T;
    int B;
    int N;
    int S;
    int E;
    int W;
    TexList(int t, int b, int n, int s, int e, int w){
        T = t;
        B = b;
        N = n;
        S = s;
        E = e;
        W = w;
    }
};

class BlockGrid{
    public:
    int blocks_wide;
    int blocks_long;
    float tile_size;
    float atlas_size;
    int tiles_wide;
    float inset;
    std::vector<int> blocks;
    std::vector<TexList> block_texes;
    std::vector<Vector3> cube_coords;

    BlockGrid();
    Model build_model();


};
