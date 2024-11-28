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

void Scene::LoadState() {
	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (!result) {
		LOG("Error loading config.xml: %s", result.description());
		return;
	}

	// Acceder correctamente al nodo <game><player> para obtener la posici�n
	pugi::xml_node playerNode = loadFile.child("game").child("player");

	if (playerNode) {
		Vector2D posPlayer;
		posPlayer.setX(playerNode.child("x").text().as_int());
		posPlayer.setY(playerNode.child("y").text().as_int());

		// Asignar la posici�n al jugador
		player->SetPosition(posPlayer);

		LOG("Loaded Player Position: (%d, %d)", posPlayer.getX(), posPlayer.getY());
	}
	else {
		LOG("Player node not found in config.xml");
	}
}





void Scene::SaveState() {
	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (!result) {
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	// Acceder al nodo <game><player> para guardar la posici�n
	pugi::xml_node playerNode = loadFile.child("game").child("player");

	if (playerNode) {
		// Guardar la posici�n actual del jugador
		playerNode.child("x").text().set(player->GetPosition().getX());
		playerNode.child("y").text().set(player->GetPosition().getY());
	}
	else {
		LOG("Player node not found in config.xml");
		return;
	}

	// Guardar cambios en el archivo XML
	loadFile.save_file("config.xml");
	LOG("Saved Player Position: (%d, %d)", player->GetPosition().getX(), player->GetPosition().getY());
}
















