#include "NPC.h"


NPC::NPC(BlockGrid* bg, Camera* cam, Texture2D tex){
    grid = bg;
    camera = cam;
    texture = tex;

    // movement stuff...
    forward = Vector3RotateByAxisAngle(forward, up, angle * DEG2RAD);
    right = Vector3RotateByAxisAngle(right, up, angle * DEG2RAD);
    max_speed = 1.0f;

    // anim and billboard stuff
    sprite_LUT.push_back(std::vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7 }));
    sprite_LUT.push_back(std::vector<int>({ 7, 0, 1, 2, 3, 4, 5, 6 }));
    sprite_LUT.push_back(std::vector<int>({ 6, 7, 0, 1, 2, 3, 4, 5 }));
    sprite_LUT.push_back(std::vector<int>({ 5, 6, 7, 0, 1, 2, 3, 4 }));
    sprite_LUT.push_back(std::vector<int>({ 4, 5, 6, 7, 0, 1, 2, 3 }));
    sprite_LUT.push_back(std::vector<int>({ 3, 4, 5, 6, 7, 0, 1, 2 }));
    sprite_LUT.push_back(std::vector<int>({ 2, 3, 4, 5, 6, 7, 0, 1 }));
    sprite_LUT.push_back(std::vector<int>({ 1, 2, 3, 4, 5, 6, 7, 0 }));

    anims.insert({ "STOP", new Anim({0}, 1.0f) } );
    current_anim = "STOP";
    billboard_size = Vector2{ (float)(frame_size) / 64.0f, (float)(frame_size) / 64.0f };
    billboard_origin = Vector2Scale(billboard_size, 0.5f);

    // state machine stuff...
    timer = 0.0f;
    time_limit = 3.0f;
    current_state = NPC_IDLE;
    angle = (float)(std::rand() % 360);
}

NPC::~NPC(){
    camera = nullptr;
    for(std::pair<std::string, Anim*> a : anims){
        delete a.second;
        a.second = nullptr;
    }
}

void NPC::update(float time, Player* player, std::vector<NPC*>& npc_list){
    timer += time;
    forward = Vector3{1.0f, 0.0f, 0.0f};
    right = Vector3{0.0f, 0.0f, 1.0f};
    forward = Vector3RotateByAxisAngle(forward, up, angle * DEG2RAD);
    right = Vector3RotateByAxisAngle(right, up, angle * DEG2RAD);
    if(current_state == NPC_IDLE){
        if(timer >= time_limit){
            current_state = NPC_WALKING;
            angle = (float)(std::rand() % 360);
            forward = Vector3{1.0f, 0.0f, 0.0f};
            right = Vector3{0.0f, 0.0f, 1.0f};
            forward = Vector3RotateByAxisAngle(forward, up, angle * DEG2RAD);
            right = Vector3RotateByAxisAngle(right, up, angle * DEG2RAD);
            timer = 0.0f;
        }
    }
    
    if(current_state == NPC_WALKING){
        if(timer >= time_limit){
            current_state = NPC_IDLE;
            timer = 0.0f;
        }
        
        velocity = Vector3Scale(forward, max_speed * time);
        
        // check for collisions...
        Vector3 collision_point{position.x + velocity.x, position.y, position.z};
        if(collision_check(collision_point, player, npc_list)){
            collision_point.x = position.x + std::copysign(0.1f, velocity.x);
            while(collision_check(collision_point, player, npc_list) == true){
                if(std::abs(velocity.x) <= 0.01f){ velocity.x = 0.0f; break; }
                velocity.x -= std::copysign(0.01f, velocity.x);
                collision_point.x = position.x + std::copysign(0.1f, velocity.x);
            }
            current_state = NPC_IDLE;
        }
        
        velocity.y = 0.0f; // Y velocity should always be 0?

        collision_point = Vector3{position.x, position.y, position.z + velocity.z};
        if(collision_check(collision_point, player, npc_list)){
            collision_point.z = position.z + std::copysign(0.1f, velocity.z);
            while(collision_check(collision_point, player, npc_list) == true){
                if(std::abs(velocity.z) <= 0.01f){ velocity.z = 0.0f; break; }
                velocity.z -= std::copysign(0.01f, velocity.z);
                collision_point.z = position.z + std::copysign(0.1f, velocity.z);
            }
            current_state = NPC_IDLE;
        }
        
        position = Vector3Add(position, velocity);
    }
    // anim and billboard updates
    anims[current_anim]->update(time);
    set_anim_frames();
	
}

void NPC::draw(){
    // draw a shadow cylinder?
    Rectangle src{(float)(frame_column * frame_size), (float)(frame_row * frame_size), (float)(frame_size), (float)(frame_size)};
    Vector3 draw_here = Vector3{position.x, position.y - billboard_origin.y, position.z};
    DrawCylinderEx(Vector3{position.x, 0.01f, position.z}, Vector3{position.x, -0.01f, position.z}, 0.15f, 0.15f, 12, Color{0, 0, 0, 200});
    DrawBillboardPro(*camera, texture, src, draw_here, up, billboard_size, billboard_origin, 0.0f, WHITE);
}

void NPC::add_anim(std::string s, Anim* a){
    anims.insert({s, a});
}

void NPC::set_anim_frames(){
    float facing_angle = Vector2Angle(Vector2UnitX, Vector2{forward.x, forward.z}) * RAD2DEG;
    facing_angle = Wrap(facing_angle, 0.0f, 360.0f);
    int facing_index = (int)(facing_angle) / 45;
    Vector2 displacement{ camera->position.x -position.x, camera->position.z - position.z }; // vector from camera to billboard
	float rotation_angle = Vector2Angle(Vector2UnitX, displacement) * RAD2DEG; // get angle between displacemnt vector and x axis
    rotation_angle = Wrap(rotation_angle, 0.0f, 360.0f);
    int rotation_index = (int)(rotation_angle) / 45;
    //frame_column = ((int)Wrap(facing_angle - rotation_angle + 22.5f, 0.0f, 360.0f) / 45) % 8; // not quite sure why this works...
	frame_column = sprite_LUT[rotation_index][facing_index];
    frame_row = anims[current_anim]->current_frame;
}

bool NPC::entity_collision(Vector3 point, Player* player, std::vector<NPC*>& list){
    // check if this NPC collides with Player or other NPCs
    // all of these collisions are based on circle collisions...
    // check player...
    Vector2 vec{player->position.x - point.x, player->position.z - point.z};
    float mag2 = Vector2LengthSqr(vec);
    float limit = (radius + player->radius) * (radius + player->radius);
    if(mag2 <= limit){ return true; }
    // check NPCs...
    for(NPC* other : list){
        if(this != other){
            vec.x = other->position.x - point.x;
            vec.y = other->position.z - point.z;
            mag2 = Vector2LengthSqr(vec);
            limit = (radius + other->radius) * (radius + other->radius);
            if(mag2 <= limit){ return true; }
        }
    }

    return false;
}

bool NPC::collision_check(Vector3 point, Player* player, std::vector<NPC*>& list){
    bool grid_check = grid->entity_collision(point, radius);
    bool entity_check = entity_collision(point, player, list);
    return grid_check || entity_check;
}