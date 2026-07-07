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
    int blocks_wide = 16;
    int blocks_long = 16;
    float tile_size = 16.0f;
    float atlas_size = 256.0f;
    int tiles_wide = 16;
    float inset = 0.5f / atlas_size;
    std::vector<int> blocks;
    std::vector<TexList> block_texes;
    std::vector<Vector3> cube_coords;

    BlockGrid();
    Model build_model();
    bool collision_at_point(Vector3 point);
    bool player_collision(Vector3 position, Vector3 velocity, float radius);

};
