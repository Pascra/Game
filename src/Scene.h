#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <string>

struct SDL_Texture;

class Scene : public Module
{
public:

    Scene();

    // Destructor
    virtual ~Scene();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called before all Updates
    bool PreUpdate();

    // Called each loop iteration
    bool Update(float dt);

    // Called before all Updates
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    // Retrieve the player's current position
    Vector2D GetPlayerPosition();

private:
    // Handle saving the game state
    void LoadState();
    void SaveState();

private:
    SDL_Texture* img = nullptr; // Placeholder texture
    SDL_Texture* mouseTileTex = nullptr; // Texture for tile highlighting
    Player* player = nullptr; // Reference to the player entity
    std::vector<Enemy*> enemyList; // List of enemies
    std::string tilePosDebug = "[0,0]"; // Debug information for tile position
    bool once = false; // Debug helper flag
};
