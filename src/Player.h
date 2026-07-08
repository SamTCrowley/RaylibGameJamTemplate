#pragma once

#include <cmath>

#include "raylib.h"
#include "raymath.h"
#include "BlockGrid.h"

// forward declare
class NPC;

class Player{
    public:
    BlockGrid* grid;
    
    Vector3 position = Vector3{8.5f, 0.5f, 8.5f};
    Vector3 velocity = Vector3{0.0f, 0.0f, 0.0f};
    float radius = 0.2f;
    float max_speed = 5.0f;
    float angle = 0.0f;
    Vector3 forward = Vector3{1.0f, 0.0f, 0.0f};
    Vector3 right = Vector3{0.0f, 0.0f, 1.0f};
    Vector3 up = Vector3{0.0f, 1.0f, 0.0f};
    bool mouse_lock = true;

    Player(BlockGrid* bg);
    void update(float time, std::vector<NPC*>& list);
    bool entity_collision(Vector3 point, std::vector<NPC*>& list);
    bool collision_check(Vector3 point, std::vector<NPC*>& list);

};

