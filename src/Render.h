#pragma once

#include "Module.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Render module for managing 2D drawing with SDL
class Render : public Module
{
public:
    Render();

    // Destructor
    virtual ~Render();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();
    bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, float speed = 1.0f, double angle = 0.0, int pivotX = INT_MAX, int pivotY = INT_MAX, bool ignoreCamera = false);

    // Called before quitting
    bool CleanUp();

    // Set the viewport for rendering
    void SetViewPort(const SDL_Rect& rect);

    // Reset viewport to full screen
    void ResetViewPort();

    // Draw a texture to the screen
    bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;

    // Draw a rectangle to the screen
    bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;

    // Draw a line to the screen
    bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

    // Draw a circle to the screen
    bool DrawCircle(int x1, int y1, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

    // Set the background color of the renderer
    void SetBackgroundColor(SDL_Color color);

    // Load a texture from a file
    SDL_Texture* LoadTexture(const char* path);

    bool DrawText(const char* text, int posX, int posY, int w, int h) const;

    void DrawText(const char* text, int x, int y, SDL_Color color = { 255, 255, 255, 255 });
    void DrawText(TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);

    void CalculateTextSize(const char* text, int& width, int& height);
    

public:
    SDL_Renderer* renderer = nullptr; // SDL renderer for 2D drawing

    SDL_Rect camera;                 // Camera rectangle for rendering
    SDL_Rect viewport;               // Viewport for the renderer
    SDL_Color background = { 0, 0, 0, 255 }; // Background color
    TTF_Font* font;
};
