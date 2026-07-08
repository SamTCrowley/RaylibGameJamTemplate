#pragma once

#include <string>
#include <vector>
#include <map>
#include "raylib.h"
#include "raymath.h"
#include "BlockGrid.h"
#include "Player.h"
#include "Anim.h"

class NPC{
    public:
    BlockGrid* grid;
    
    Vector3 position = Vector3{2.5f, 0.5f, 2.5f};
    Vector3 velocity = Vector3{0.0f, 0.0f, 0.0f};
    float radius = 0.2f;
    float max_speed = 3.0f;
    float angle = 0.0f;
    Vector3 forward = Vector3{1.0f, 0.0f, 0.0f};
    Vector3 right = Vector3{0.0f, 0.0f, 1.0f};
    Vector3 up = Vector3{0.0f, 1.0f, 0.0f};

    Camera* camera = nullptr;
    Texture2D texture;

    // animation stuff
    std::map<std::string, Anim*> anims = std::map<std::string, Anim*>();
    std::string current_anim = "STOP";

    // billboard drawing stuff...
    std::vector<std::vector<int> > sprite_LUT = std::vector<std::vector<int> >();
    int frame_size = 32;
    int frame_column = 0;
    int frame_row = 0;
    Vector2 billboard_size{0.5f, 0.5f};
    Vector2 billboard_origin = Vector2Scale(billboard_size, 0.5f);

    // state machine stuff...
    float timer = 0.0f;
    float time_limit = 1.0f;
    enum NPC_STATE{NPC_IDLE, NPC_WALKING};
    NPC_STATE current_state = NPC_IDLE;

    NPC(BlockGrid* bg, Camera* cam, Texture2D tex);
    ~NPC();
    void add_anim(std::string s, Anim* a);
    void update(float time, Player* player, std::vector<NPC*>& list);
    void draw();
    void set_anim_frames();
    bool entity_collision(Vector3 point, Player* player, std::vector<NPC*>& list);
    bool collision_check(Vector3 point, Player* player, std::vector<NPC*>& list);

};
