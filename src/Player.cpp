#include "Player.h"

Player::Player(BlockGrid* bg){
    grid = bg;
    forward = Vector3RotateByAxisAngle(forward, up, angle * DEG2RAD);
    right = Vector3RotateByAxisAngle(right, up, angle * DEG2RAD);
}

void Player::update(float time){
    // get user input
    Vector2 mouse_move = GetMouseDelta();
    angle -= mouse_move.x * 30.0f * time;
    angle = Wrap(angle, 0.0f, 360.0f);
    forward = Vector3{1.0f, 0.0f, 0.0f};
    right = Vector3{0.0f, 0.0f, 1.0f};
    forward = Vector3RotateByAxisAngle(forward, up, angle * DEG2RAD);
    right = Vector3RotateByAxisAngle(right, up, angle * DEG2RAD);

    velocity = Vector3{0.0f, 0.0f, 0.0f};
    if(IsKeyDown(KEY_W)){
        velocity = Vector3Add(velocity, forward);
    }
    if(IsKeyDown(KEY_S)){
        velocity = Vector3Subtract(velocity, forward);
    }
    if(IsKeyDown(KEY_A)){
        velocity = Vector3Subtract(velocity, right);
    }
    if(IsKeyDown(KEY_D)){
        velocity = Vector3Add(velocity, right);
    }
    velocity = Vector3Normalize(velocity);
    velocity = Vector3Scale(velocity, max_speed * time);

    // check for collisions...
    if(grid->player_collision(position, Vector3{velocity.x, 0.0f, 0.0f}, radius) == true){
        while(grid->player_collision(Vector3{position.x + std::copysign(0.1f, velocity.x), position.y, position.z}, Vector3{0.0f, 0.0f, 0.0f}, radius) == true){
            if(std::abs(velocity.x) <= 0.01f){ velocity.x = 0.0f; break; }
            velocity.x -= std::copysign(0.01f, velocity.x);
        }
    }
    velocity.y = 0.0f; // Y velocity should always be 0?
    if(grid->player_collision(position, Vector3{0.0f, 0.0f, velocity.z}, radius) == true){
        while(grid->player_collision(Vector3{position.x, position.y, position.z + std::copysign(0.1f, velocity.z)}, Vector3{0.0f, 0.0f, 0.0f}, radius) == true){
            if(std::abs(velocity.z) <= 0.01f){ velocity.z = 0.0f; break; }
            velocity.z -= std::copysign(0.01f, velocity.z);
        }
    }
    position = Vector3Add(position, velocity);
}