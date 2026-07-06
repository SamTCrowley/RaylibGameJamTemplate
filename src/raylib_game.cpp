/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <iostream>
#include <vector>
#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

#include "Gui.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY, 
    SCREEN_ENDING
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 720;
static const int screenHeight = 720;
static RenderTexture2D target = { 0 };  // Render texture to render our game
static Texture2D tex = { 0 };
static Font gui_font = { 0 };
static Vector2 tex_position{100.0f, 100.0f};
static std::vector<Vector2> points = std::vector<Vector2>();
Camera cam = {0};
GUI gui = GUI();
Color dot_color = RED;

// TODO: Define global variables here, recommended to make them static


//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame();      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_ALL);         // Disable raylib trace log messages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib gamejam template");
    
    // TODO: Load resources / Initialize variables at this point
#if defined(PLATFORM_WEB)    
    tex = LoadTexture("resources/DerpDude.png");
    gui_font = LoadFont("resources/ConsolaMono-Bold.ttf");
#else
    tex = LoadTexture("./src/resources/DerpDude.png");
    gui_font = LoadFont("./src/resources/ConsolaMono-Bold.ttf");
#endif

    gui.font = gui_font;
    gui.add_element(PANEL, "RootPanel", "NULLPTR", 100, 50, 160, 30, {});
    gui.add_element(BUTTON, "Button1", "RootPanel", 0, 0, 160, 30, {{"text", "CHANGE DOT COLOR"}, {"red", "255"}, {"green", "0"}, {"blue", "0"}});
    ((GUI_Button*)gui.elements["Button1"])->func = [](){
        dot_color.r = std::rand() % 255;
        dot_color.g = std::rand() % 255;
        dot_color.b = std::rand() % 255;
    };


    cam = Camera3D();
    cam.position = Vector3{-5.0f, 0.0f, 0.0f};
    cam.target = Vector3{0.0f, 0.0f, 0.0f};


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();        
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    
    // TODO: Unload all loaded resources at this point
    UnloadTexture(tex);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------

void UpdateDrawFrame(){
        if(IsKeyPressed(KEY_A)){
            tex_position.x -= 10.0f;
        }
        if(IsKeyPressed(KEY_D)){
            tex_position.x += 10.0f;
        }
        if(IsKeyPressed(KEY_W)){
            tex_position.y -= 10.0f;
        }
        if(IsKeyPressed(KEY_S)){
            tex_position.y += 10.0f;
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            points.push_back(GetMousePosition());
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            points.clear();
        }

        gui.update();

    BeginDrawing();
        ClearBackground(WHITE);
        BeginMode3D(cam);
        DrawCube(Vector3{0.0f, 0.0f, 0.0f}, 1.0f, 1.0f, 1.0f, BLUE);
        DrawCubeWires(Vector3{0.0f, 0.0f, 0.0f}, 1.0f, 1.0f, 1.0f, BLACK);
        EndMode3D();
        if(points.empty() == false){
            for(Vector2 v : points){
                DrawCircle(v.x, v.y, 5, dot_color);
            }
        }
        DrawTexture(tex, tex_position.x, tex_position.y, WHITE);
        gui.draw();
    EndDrawing();
}

