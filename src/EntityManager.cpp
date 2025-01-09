#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "Engine.h"
#include "Log.h"
#include "tracy/Tracy.hpp"
#include "Enemy.h"


EntityManager::EntityManager() : Module() {
    name = "entitymanager";
}

EntityManager::~EntityManager() {
    CleanUp(); // Ensure proper cleanup
}

bool EntityManager::Awake() {
    LOG("Loading Entity Manager");
    bool ret = true;

    for (auto entity : entities) {
        if (entity && entity->active) {
            ret = entity->Awake();
        }
    }

    return ret;
}

bool EntityManager::Start() {
    bool ret = true;

    for (auto entity : entities) {
        if (entity && entity->active) {
            ret = entity->Start();
        }
    }

    return ret;
}

bool EntityManager::Update(float dt) {
    ZoneScoped;
    for (auto entity : entities) {
        if (entity && entity->active) {
            if (!entity->Update(dt)) {
                DestroyEntity(entity); // Destruir entidades no activas
            }
        }
    }
    return true;
}


bool EntityManager::CleanUp() {
    LOG("Cleaning up Entity Manager");

    for (auto entity : entities) {
        if (entity) {
            entity->CleanUp();
            delete entity; // Free memory
        }
    }

    entities.clear(); // Clear the container
    return true;
}

Entity* EntityManager::CreateEntity(EntityType type) {
    Entity* entity = nullptr;

    switch (type) {
    case EntityType::PLAYER:
        entity = new Player();
        break;
    case EntityType::ITEM:
        entity = new Item();
        break;
    case EntityType::ENEMY:
        entity = new Enemy();
        break;
    default:
        LOG("Unknown EntityType in CreateEntity");
        return nullptr;
    }

    if (entity) {
        entities.push_back(entity);
    }

    return entity;
}

void EntityManager::DestroyEntity(Entity* entity) {
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (*it == entity) {
            (*it)->CleanUp(); // Limpia los recursos de la entidad
            delete* it;       // Libera la memoria
            entities.erase(it); // Elimina la entidad de la lista
            break;
        }
    }
}


void EntityManager::AddEntity(Entity* entity) {
    if (entity) {
        LOG("Entity added: Type=%d, Position=(%f, %f)", entity->type, entity->position.getX(), entity->position.getY());
        entities.push_back(entity);
    }
}


Player* EntityManager::GetPlayer() {
    for (auto entity : entities) {
        if (entity && entity->type == EntityType::PLAYER) {
            return static_cast<Player*>(entity);
        }
    }
    return nullptr; // Return nullptr if no player is found
}
