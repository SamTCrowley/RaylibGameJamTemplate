#pragma once

#include "raylib.h"
#include "raymath.h"

#include <string>
#include <vector>
#include <map>

struct GUI_Element{
    std::string name;
    Rectangle rect;
    GUI_Element(std::string name_, float x, float y, float w, float h){
        name = name_;
        rect = Rectangle{x, y, w, h};
    }
    void draw(){
        DrawRectangleRec(rect, GRAY);
        DrawRectangleLinesEx(rect, 3, BLUE);
    }
};
