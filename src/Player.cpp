#include "Player.h"
#include "NPC.h"

Player::Player(BlockGrid* bg){
    grid = bg;
    forward = Vector3RotateByAxisAngle(forward, up, angle * DEG2RAD);
    right = Vector3RotateByAxisAngle(right, up, angle * DEG2RAD);
}

void Player::update(float time, std::vector<NPC*>& list){
    // get user input
    // if space is down, disable movement, but allow cursor.
    if(IsKeyPressed(KEY_SPACE)){
        mouse_lock = false;
        EnableCursor();
        HideCursor();
    }
    if(IsKeyReleased(KEY_SPACE)){
        mouse_lock = true;
        DisableCursor();
    }
    if(mouse_lock){
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
        Vector3 collision_point{position.x + velocity.x, position.y, position.z};
        if(collision_check(collision_point, list)){
            collision_point.x = position.x + std::copysign(0.1f, velocity.x);
            while(collision_check(collision_point, list) == true){
                if(std::abs(velocity.x) <= 0.01f){ velocity.x = 0.0f; break; }
                velocity.x -= std::copysign(0.01f, velocity.x);
                collision_point.x = position.x + std::copysign(0.1f, velocity.x);
            }
        }
        velocity.y = 0.0f; // Y velocity should always be 0?
        collision_point = Vector3{position.x, position.y, position.z + velocity.z};
        if(collision_check(collision_point, list)){
            collision_point.z = position.z + std::copysign(0.1f, velocity.z);
            while(collision_check(collision_point, list) == true){
                if(std::abs(velocity.z) <= 0.01f){ velocity.z = 0.0f; break; }
                velocity.z -= std::copysign(0.01f, velocity.z);
                collision_point.z = position.z + std::copysign(0.1f, velocity.z);
            }
        }

        position = Vector3Add(position, velocity);
    }
}

bool Player::entity_collision(Vector3 point, std::vector<NPC*>& list){
    // check if this NPC collides with other NPCs
    // all of these collisions are based on circle collisions...
    Vector2 vec;
    float mag2;
    float limit;
    // check NPCs...
    for(NPC* other : list){
        vec.x = other->position.x - point.x;
        vec.y = other->position.z - point.z;
        mag2 = Vector2LengthSqr(vec);
        limit = (radius + other->radius) * (radius + other->radius);
        if(mag2 <= limit){ return true; }
    }

    return false;
}

bool Player::collision_check(Vector3 point, std::vector<NPC*>& list){
    bool grid_check = grid->entity_collision(point, radius);
    bool entity_check = entity_collision(point, list);
    return grid_check || entity_check;
}