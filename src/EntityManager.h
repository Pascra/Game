#pragma once

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include <vector>

class EntityManager : public Module {
public:
    EntityManager();
    virtual ~EntityManager();

    bool Awake() override;
    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    // Methods to manage entities
    Entity* CreateEntity(EntityType type);
    void DestroyEntity(Entity* entity);
    void AddEntity(Entity* entity);

    // Retrieve the player instance
    Player* GetPlayer();

private:
    std::vector<Entity*> entities; // Container to hold all entities
};
