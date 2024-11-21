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

	//L04: TODO 3b: Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	
	//L08 Create a new item using the entity manager and set the position to (200, 672) to test
	Item* item = (Item*) Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	item->position = Vector2D(200, 672);
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", "mapa.tmx");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt) {
	LOG("Before Update - Player position: (%d, %d)", player->position.getX(), player->position.getY());

	float camSpeed = 1;
	Engine::GetInstance().render.get()->camera.x = -(player->position.getX() - 200);
	Engine::GetInstance().render.get()->camera.y = -(player->position.getY() - 400);

	LOG("After Update - Player position: (%d, %d)", player->position.getX(), player->position.getY());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		SaveState();  // Guardar en f5
		LOG("Game state saved.");
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		LoadState();  // cargar f6
		LOG("Game state loaded.");
	}

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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

bool Scene::LoadState() {
	pugi::xml_parse_result result = configFile.load_file("save.xml");
	if (!result) {
		LOG("Failed to load save.xml: %s", result.description());
		return false;
	}

	pugi::xml_node playerNode = configFile.child("game").child("player");
	if (!playerNode) {
		LOG("Player data not found in save.xml");
		return false;
	}

	int playerX = playerNode.child("x").text().as_int();
	int playerY = playerNode.child("y").text().as_int();

	LOG("Loaded player position: (%d, %d)", playerX, playerY);

	if (player) {
		player->position = Vector2D(playerX, playerY);
		LOG("Player position set to: (%d, %d)", player->position.getX(), player->position.getY());
	}

	// Forzar la cámara a moverse a la posición correcta
	Engine::GetInstance().render.get()->camera.x = -(playerX - 200);
	Engine::GetInstance().render.get()->camera.y = -(playerY - 400);

	return true;
}

bool Scene::SaveState() {
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("save.xml");

	if (!result)
	{
		LOG("Failed to load save.xml, creating a new one.");
	}

	pugi::xml_node rootNode = doc.child("game");
	if (!rootNode)
	{
		rootNode = doc.append_child("game");
	}

	pugi::xml_node playerNode = rootNode.child("player");
	if (!playerNode)
	{
		playerNode = rootNode.append_child("player");
	}

	// Limpia los valores previos
	playerNode.remove_children();

	// Guarda la posición actual del jugador
	playerNode.append_child("x").text().set(player->position.getX());
	playerNode.append_child("y").text().set(player->position.getY());

	doc.save_file("save.xml");

	LOG("Game state saved: (%d, %d)", player->position.getX(), player->position.getY());
	return true;
}