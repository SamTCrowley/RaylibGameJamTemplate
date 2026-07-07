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
#include "BlockGrid.h"
#include "Player.h"

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
Shader post_process_shader = { 0 };
int ppshader_resLoc = 0;
float ppshader_res[2];
static Texture2D wall_textures = { 0 };
static Texture2D cursor_textures = { 0 };
enum CURSOR_MODE{CURSOR_HAND, CURSOR_GRAB, CURSOR_POINT};
CURSOR_MODE cursor_mode = CURSOR_POINT;
Rectangle cursor_draw_src{0.0f, 0.0f, 8.0f, 8.0f};
Rectangle cursor_draw_dst{0.0f, 0.0f, 16.0f, 16.0f};
static Font gui_font = { 0 };
Camera cam = {0};
GUI gui = GUI();
BlockGrid grid = BlockGrid();
Model grid_model = {0};
Player player = Player(nullptr);

// TODO: Define global variables here, recommended to make them static


//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame();      // Update and Draw one frame
static void UpdateCursor();
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
    DisableCursor();
    HideCursor();
    
    // TODO: Load resources / Initialize variables at this point
#if defined(PLATFORM_WEB)    
    Image img = LoadImage("resources/textures.png");
    ImageMipmaps(&img);
    wall_textures = LoadTextureFromImage(img);
    SetTextureFilter(wall_textures, TEXTURE_FILTER_POINT);
    SetTextureWrap(wall_textures, TEXTURE_WRAP_CLAMP);
    UnloadImage(img);
    cursor_textures = LoadTexture("resources/cursors.png");
    gui_font = LoadFont("resources/ConsolaMono-Bold.ttf");
    post_process_shader = LoadShader(0, "resources/PostProcessWeb.frag");
    ppshader_resLoc = GetShaderLocation(post_process_shader, "resolution");
    ppshader_res[0] = (float)screenWidth;
    ppshader_res[1] = (float)screenHeight;
    SetShaderValue(post_process_shader, ppshader_resLoc, ppshader_res, SHADER_UNIFORM_VEC2);
#else
    Image img = LoadImage("./resources/textures.png");
    ImageMipmaps(&img);
    wall_textures = LoadTextureFromImage(img);
    SetTextureFilter(wall_textures, TEXTURE_FILTER_POINT);
    SetTextureWrap(wall_textures, TEXTURE_WRAP_CLAMP);
    UnloadImage(img);
    cursor_textures = LoadTexture("./resources/cursors.png");
    gui_font = LoadFont("./resources/ConsolaMono-Bold.ttf");
    post_process_shader = LoadShader(0, "./resources/PostProcess.frag");
    ppshader_resLoc = GetShaderLocation(post_process_shader, "resolution");
    ppshader_res[0] = (float)screenWidth;
    ppshader_res[1] = (float)screenHeight;
    SetShaderValue(post_process_shader, ppshader_resLoc, ppshader_res, SHADER_UNIFORM_VEC2);
    
#endif

    target = LoadRenderTexture(screenWidth, screenHeight);

    /*
    gui.font = gui_font;
    gui.add_element(PANEL, "RootPanel", "NULLPTR", 100, 50, 160, 30, {});
    gui.add_element(BUTTON, "Button1", "RootPanel", 0, 0, 160, 30, {{"text", "CHANGE DOT COLOR"}, {"red", "255"}, {"green", "0"}, {"blue", "0"}});
    ((GUI_Button*)gui.elements["Button1"])->func = [](){

    };
    */

    grid_model = grid.build_model();
    grid_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wall_textures;

    player = Player(&grid);

    cam.position = player.position;
    cam.target = Vector3Add(player.position, player.forward);
    cam.up = Vector3{0.0f, 1.0f, 0.0f};
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    

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
    UnloadTexture(wall_textures);
    UnloadModel(grid_model);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
void UpdateCursor(){
    // update cursor stuff...
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        cursor_mode = CURSOR_GRAB;
    }else{
        cursor_mode = CURSOR_POINT;
    }
    switch(cursor_mode){
        case CURSOR_HAND: cursor_draw_src = Rectangle{0.0f, 0.0f, 8.0f, 8.0f}; break;
        case CURSOR_GRAB: cursor_draw_src = Rectangle{8.0f, 0.0f, 8.0f, 8.0f}; break;
        case CURSOR_POINT: cursor_draw_src = Rectangle{16.0f, 0.0f, 8.0f, 8.0f}; break;
        default: cursor_draw_src = Rectangle{0.0f, 0.0f, 8.0f, 8.0f}; break;
    }
    Vector2 mouse_position = GetMousePosition();
    cursor_draw_dst = Rectangle{mouse_position.x - 8.0f, mouse_position.y - 8.0f, 16.0f, 16.0f};
}


void UpdateDrawFrame(){
    float time = GetFrameTime();
    player.update(time);
    cam.position = player.position;
    cam.target = Vector3Add(player.position, player.forward);
    
    UpdateCursor();

    // rendering...
    BeginTextureMode(target);
        ClearBackground(GRAY);
        BeginMode3D(cam);
            DrawModel(grid_model, Vector3Zeros, 1.0f, WHITE);
        EndMode3D();
        DrawText(TextFormat("COLLISION: %s", grid.player_collision(player.position, player.velocity, player.radius) ? "TRUE" : "FALSE"), 10, 10, 16, BLACK);
        if(player.mouse_lock == false){
            DrawTexturePro(cursor_textures, cursor_draw_src, cursor_draw_dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
        }
    EndTextureMode();
    BeginDrawing();    
    BeginShaderMode(post_process_shader);
            ClearBackground(BLACK);
            DrawTextureRec(target.texture, Rectangle{0.0f, 0.0f, 720.0f, -720.0f}, Vector2{0.0f, 0.0f}, WHITE);
        EndShaderMode();
    EndDrawing();
}

