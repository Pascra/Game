#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

enum class ItemType {
    COIN,
    HEART
};

class Item : public Entity {
public:
    Item();
    virtual ~Item();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void SetItemType(ItemType type);
    void SetPosition(const Vector2D& position);

private:
    ItemType itemType;
    SDL_Texture* texture;
    const char* texturePath;
    int texW, texH;

    PhysBody* pbody;

public:
    bool isPicked = false; // Atributo para marcar si el ítem ha sido recogido
};
