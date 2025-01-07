#include "Scene.h"
#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Log.h"
#include "EntityManager.h"
#include "Map.h"
#include "Item.h"
#include "Physics.h"
#include "tracy/Tracy.hpp"
#include "GuiControl.h"
#include "GuiManager.h"
#include "flyingEnemy.h"

Scene::Scene() : Module()
{
    name = "scene";
    img = nullptr;
}

Scene::~Scene() {}

bool Scene::Awake() {
    LOG("Loading Scene");
    bool ret = true;

    player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);

    Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
    item->position = Vector2D(200, 672);
    Item* heart = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
    heart->SetItemType(ItemType::HEART);
    heart->SetPosition(Vector2D(1293, 272));

    // Crear enemigos (terrestres y voladores)
    for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy");
        enemyNode;
        enemyNode = enemyNode.next_sibling("enemy")) {
        std::string name = enemyNode.attribute("name").as_string();
        if (name == "flyingguy") {
            // Crear un enemigo volador
            FlyingEnemy* flyingEnemy = (FlyingEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
            flyingEnemy->SetParameters(enemyNode);
            enemyList.push_back(flyingEnemy);
        }
        else {
            // Crear un enemigo terrestre
            Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
            enemy->SetParameters(enemyNode);
            enemyList.push_back(enemy);
        }
    }

    // Crear el botón original con el texto "MENU"
    SDL_Rect btPos = { 1170, 120, 80, 30 }; // Tamaño ajustado para acomodar el texto
    guiBt = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MENU", btPos, this);

    return ret;
}


bool Scene::Start()
{
    losingScreenTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Lose.png");
    Engine::GetInstance().audio.get()->PlayMusic("Assets/Audio/Music/Pixel10.ogg");
    Engine::GetInstance().map->Load("Assets/Maps/", "mapa.tmx");
    birdsFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Birds.ogg");
    Engine::GetInstance().audio.get()->PlayFx(birdsFxId);
    
    return true;
}

bool Scene::PreUpdate()
{
    ZoneScoped;
    return true;
}

bool Scene::Update(float dt)
{
    ZoneScoped;
    //LOG("Before Update - Player position: (%d, %d)", player->position.getX(), player->position.getY());

    Engine::GetInstance().render.get()->camera.x = -(player->position.getX() - 200);
    Engine::GetInstance().render.get()->camera.y = -(player->position.getY() - 400);

    //LOG("After Update - Player position: (%d, %d)", player->position.getX(), player->position.getY());

    int scale = Engine::GetInstance().window.get()->GetScale();
    if (scale <= 0)
    {
        LOG("Invalid scale detected, setting scale to 1.");
        scale = 1;
    }
    if (showLosingScreen)
    {
        // Renderiza la pantalla de derrota
        int windowWidth, windowHeight;
        Engine::GetInstance().window.get()->GetWindowSize(windowWidth, windowHeight);
        SDL_Rect destRect = { 0, 0, windowWidth, windowHeight };
        Engine::GetInstance().render.get()->DrawTexture(losingScreenTexture, 0, 0, &destRect, 1.0f, 0.0, 0, 0, true);

        // Reinicia el juego si se presiona una tecla
        if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
        {
             //Reiniciar juego
        }

        return true; // Evita que otros elementos de la escena se actualicen
    }
    Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
    Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(
        mousePos.getX() - Engine::GetInstance().render.get()->camera.x / scale,
        mousePos.getY() - Engine::GetInstance().render.get()->camera.y / scale
    );

    Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(), mouseTile.getY());
    SDL_Rect rect = { 0, 0, 32, 32 };
    Engine::GetInstance().render.get()->DrawTexture(mouseTileTex, highlightTile.getX(), highlightTile.getY(), &rect);

    //if (!enemyList.empty() && Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_DOWN)
    //{
    //    enemyList[0]->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
    //    enemyList[0]->ResetPath();
    //}
    player->DrawLives();
    return true;
}

bool Scene::PostUpdate()
{
    ZoneScoped;
    bool ret = true;

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
    {
        SaveState();
        LOG("Game state saved.");
    }
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
    {
        LoadState();
        LOG("Game state loaded.");
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
        ret = false;

    return ret;
}

bool Scene::CleanUp()
{
    LOG("Freeing scene");
    SDL_DestroyTexture(img);
    return true;
    if (losingScreenTexture != nullptr)
    {
        Engine::GetInstance().textures.get()->UnLoad(losingScreenTexture);
        losingScreenTexture = nullptr;
    }
}
void Scene::ShowLosingScreen()
{
    showLosingScreen = true; // Activa la bandera para mostrar la pantalla de derrota
}

void Scene::LoadState()
{
    pugi::xml_document loadFile;
    pugi::xml_parse_result result = loadFile.load_file("config.xml");

    if (!result)
    {
        LOG("Error loading config.xml: %s", result.description());
        return;
    }

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

void Scene::SaveState()
{
    pugi::xml_document loadFile;
    pugi::xml_parse_result result = loadFile.load_file("config.xml");

    if (!result)
    {
        LOG("Could not load file. Pugi error: %s", result.description());
        return;
    }

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
int mapWidth = Engine::GetInstance().map.get()->GetWidth(); // Ancho del mapa en píxeles
int mapHeight = Engine::GetInstance().map.get()->GetHeight(); // Alto del mapa en píxeles


bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
    LOG("Press Gui Control: %d", control->id);

    // Detecta si el botón original fue presionado
    if (control->id == 1) // ID del botón original
    {
        if (buttonsVisible)
        {
            // Ocultar los botones: eliminarlos del vector
            for (GuiControl* button : additionalButtons)
            {
                Engine::GetInstance().guiManager->DestroyGuiControl(button);
            }
            additionalButtons.clear();
            LOG("Additional buttons hidden");
        }
        else
        {
            // Mostrar los botones adicionales con sus textos
            SDL_Rect button1Rect = { 1100, 200, 100, 40 }; // Ajusta tamaño según texto
            SDL_Rect button2Rect = { 1100, 250, 100, 40 };
            SDL_Rect button3Rect = { 1100, 300, 100, 40 };

            GuiControl* button1 = Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "RESPAWN", button1Rect, this);
            GuiControl* button2 = Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "LEVELS", button2Rect, this);
            GuiControl* button3 = Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "EXIT", button3Rect, this);

            additionalButtons.push_back(button1);
            additionalButtons.push_back(button2);
            additionalButtons.push_back(button3);

            LOG("Additional buttons shown");
        }

        // Cambia el estado de visibilidad
        buttonsVisible = !buttonsVisible;
    }

    // Detecta si uno de los nuevos botones fue presionado
    if (control->id == 2) // Botón "RESPAWN"
    {
        LOG("Respawn Button Pressed");
        player->ResetToInitialPosition(); // Llama al método para reiniciar la posición del jugador
    }
    else if (control->id == 4) // Botón "EXIT"
    {
        LOG("Exit Button Pressed");
        Engine::GetInstance().Exit(); // Llama al método para cerrar el juego
    }

    return true;
}
void Scene::RestartGame()
{
    LOG("Restarting game...");
    showLosingScreen = false; // Oculta la pantalla de derrota
    player->ResetLives();     // Reinicia las vidas del jugador
    player->SetPosition(Vector2D(50, 50)); // Reinicia la posición inicial del jugador
}
