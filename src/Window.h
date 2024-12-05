#pragma once

#include "Module.h"
#include "SDL2/SDL.h"

class Window : public Module
{
public:
    Window();

    // Destructor
    virtual ~Window();

    // Called before render is available
    bool Awake();

    // Called before quitting
    bool CleanUp();

    // Change title
    void SetTitle(const char* title);

    // Retrieve window size
    void GetWindowSize(int& width, int& height) const;

    int GetWidth() const;
    int GetHeight() const;

    // Retrieve window scale
    int GetScale() const;

    // Retrieve SDL_Window pointer
    SDL_Window* GetSDLWindow() const;

    int width = 1280;
    int height = 720;

private:
    SDL_Window* window;  // SDL window object
    std::string title;
    
    int scale = 1;
};
