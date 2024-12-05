#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "pugixml.hpp"
#include "Physics.h"

Scene::Scene() : Module()
{
    name = "scene";
    img = nullptr;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
    LOG("Loading Scene");
    bool ret = true;

    // Instantiate the player using the entity manager
    player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);

    // Create a test item
    Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
    item->position = Vector2D(200, 672);

    // Create enemies using the entity manager
    for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
    {
        Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
        enemy->SetParameters(enemyNode);
        enemyList.push_back(enemy);
    }

    return ret;
}

// Called before the first frame
bool Scene::Start()
{
    // Load the map
    Engine::GetInstance().map->Load("Assets/Maps/", "mapa.tmx");
    return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
    return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    LOG("Before Update - Player position: (%d, %d)", player->position.getX(), player->position.getY());

    // Update camera position
    Engine::GetInstance().render.get()->camera.x = -(player->position.getX() - 200);
    Engine::GetInstance().render.get()->camera.y = -(player->position.getY() - 400);

    LOG("After Update - Player position: (%d, %d)", player->position.getX(), player->position.getY());

    // Get mouse position and map coordinates
    int scale = Engine::GetInstance().window.get()->GetScale();
    if (scale <= 0)
    {
        LOG("Invalid scale detected, setting scale to 1.");
        scale = 1;
    }

    Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
    Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(
        mousePos.getX() - Engine::GetInstance().render.get()->camera.x / scale,
        mousePos.getY() - Engine::GetInstance().render.get()->camera.y / scale
    );

    // Render a texture to highlight the tile
    Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(), mouseTile.getY());
    SDL_Rect rect = { 0, 0, 32, 32 };
    Engine::GetInstance().render.get()->DrawTexture(mouseTileTex,
        highlightTile.getX(),
        highlightTile.getY(),
        &rect);

    // Save the tile position for debugging purposes
    if (mouseTile.getX() >= 0 && mouseTile.getY() >= 0 || once)
    {
        tilePosDebug = "[" + std::to_string((int)mouseTile.getX()) + "," + std::to_string((int)mouseTile.getY()) + "] ";
    }

    // Modify enemy position when mouse is clicked
    if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_DOWN)
    {
        if (!enemyList.empty()) // Check if there are enemies in the list
        {
            enemyList[0]->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
            enemyList[0]->ResetPath();
        }
        else
        {
            LOG("Enemy list is empty. Cannot modify enemy position.");
        }
    }

    return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
    bool ret = true;

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
    {
        SaveState(); // Save game state with F5
        LOG("Game state saved.");
    }
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
    {
        LoadState(); // Load game state with F6
        LOG("Game state loaded.");
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
        ret = false;

    return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
    LOG("Freeing scene");
    SDL_DestroyTexture(img);
    return true;
}

// Load game state
void Scene::LoadState()
{
    pugi::xml_document loadFile;
    pugi::xml_parse_result result = loadFile.load_file("config.xml");

    if (!result)
    {
        LOG("Error loading config.xml: %s", result.description());
        return;
    }

    // Access the player node to get the position
    pugi::xml_node playerNode = loadFile.child("game").child("player");
    if (playerNode)
    {
        Vector2D posPlayer;
        posPlayer.setX(playerNode.child("x").text().as_int());
        posPlayer.setY(playerNode.child("y").text().as_int());
        player->SetPosition(posPlayer);
        LOG("Loaded Player Position: (%d, %d)", posPlayer.getX(), posPlayer.getY());
    }
    else
    {
        LOG("Player node not found in config.xml");
    }
}

// Save game state
void Scene::SaveState()
{
    pugi::xml_document loadFile;
    pugi::xml_parse_result result = loadFile.load_file("config.xml");

    if (!result)
    {
        LOG("Could not load file. Pugi error: %s", result.description());
        return;
    }

    // Access the player node to save the position
    pugi::xml_node playerNode = loadFile.child("game").child("player");
    if (playerNode)
    {
        playerNode.child("x").text().set(player->GetPosition().getX());
        playerNode.child("y").text().set(player->GetPosition().getY());
    }
    else
    {
        LOG("Player node not found in config.xml");
        return;
    }

    loadFile.save_file("config.xml");
    LOG("Saved Player Position: (%d, %d)", player->GetPosition().getX(), player->GetPosition().getY());
}

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
    if (player != nullptr)
    {
        return player->GetPosition();
    }
    LOG("Player is null. Returning default position (0, 0).");
    return Vector2D(0, 0);
}
