#include "EntityManager.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "tracy/Tracy.hpp"
#include "flyingEnemy.h"

EntityManager::EntityManager() : Module()
{
	name = "entitymanager";
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;

	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->CleanUp();
	}

	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, pugi::xml_node parameters)
{
	Entity* entity = nullptr;

	// Verifica el tipo de entidad
	if (type == EntityType::ENEMY)
	{
		std::string name = parameters.attribute("name").as_string();
		if (name == "flyingguy")
		{
			entity = new FlyingEnemy();
		}
		else
		{
			entity = new Enemy();
		}
	}
	else if (type == EntityType::PLAYER)
	{
		entity = new Player();
	}
	else if (type == EntityType::ITEM)
	{
		entity = new Item();
	}

	// Configurar los parámetros y posición inicial
	if (entity != nullptr)
	{
		entity->SetParameters(parameters); // Asigna parámetros XML
		entities.push_back(entity);        // Añade la entidad a la lista
	}

	return entity;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	// Llama a la sobrecarga que utiliza un nodo XML vacío
	pugi::xml_node emptyNode;
	return CreateEntity(type, emptyNode);
}





void EntityManager::DestroyEntity(Entity* entity)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		if (*it == entity) {
			
			(*it)->CleanUp();
			delete* it; // Free the allocated memory
			entities.erase(it); // Remove the entity from the list
			break; // Exit the loop after removing the entity 
			
			
			
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.push_back(entity);
}

bool EntityManager::Update(float dt)
{
	ZoneScoped;
	bool ret = true;
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Update(dt);
	}
	return ret;
}