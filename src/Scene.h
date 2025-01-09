#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <list>
#include <string>
#include "GuiControlButton.h"
#include "SDL2/SDL_ttf.h"

#ifndef SCENE_H
#define SCENE_H

struct SDL_Texture;

class Scene : public Module
{
public:
    // Constructor
    Scene();

    // Destructor
    virtual ~Scene();

    // Lifecycle methods
    bool Awake(); // Called before render is available
    bool Start(); // Called before the first frame
    bool PreUpdate(); // Called before all Updates
    bool Update(float dt); // Called each loop iteration
    bool PostUpdate(); // Called after all Updates
    bool CleanUp(); // Called before quitting

    // Utility methods
    Vector2D GetPlayerPosition(); // Retrieve the player's current position
    bool OnGuiMouseClickEvent(GuiControl* control); // Handles GUI events
    void ShowLosingScreen(); // Display the losing screen
    void RestartGame(); // Restart the game state
    bool IsGamePaused() const { return gamePaused; }
    void TogglePause();
private:
    // Internal helper methods
    void LoadState(); // Load the game state from a file
    void SaveState(); // Save the game state to a file

private:
    // Textures and UI
    SDL_Texture* img = nullptr; // Placeholder texture
    SDL_Texture* mouseTileTex = nullptr; // Texture for tile highlighting
    SDL_Texture* losingScreenTexture = nullptr; // Texture for losing screen
    GuiControlButton* guiBt = nullptr; // GUI control button
    std::vector<GuiControl*> additionalButtons; // Additional GUI buttons

    // Player and entities
    Player* player = nullptr; // Pointer to the player entity
    std::list<Enemy*> enemyList; // List of enemies

    // Game state
    bool showLosingScreen = false; // Flag to show the losing screen
    bool buttonsVisible = false; // Flag to toggle button visibility
    bool once = false; // Debug helper flag

    // Debugging and UI helpers
    std::string tilePosDebug = "[0,0]"; // Debug information for tile position

    // Counter and fonts
    int counter = 0; // Counter variable
    SDL_Texture* counterTexture = nullptr; // Texture for counter text
    TTF_Font* font = nullptr; // Font for text rendering
    SDL_Color textColor = { 255, 255, 255, 255 }; // Text color

    // Audio
    int birdsFxId = -1; // Sound effect ID for birds
    bool gamePaused = false;
};

#endif // SCENE_H
