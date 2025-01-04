#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

class Enemy : public Entity {
public:
    Enemy();
    virtual ~Enemy();

    virtual bool Awake();
    virtual bool Start();
    virtual bool Update(float dt);
    virtual bool CleanUp();
    virtual void DrawPath();

    virtual void SetParameters(pugi::xml_node parameters);
    void SetPosition(Vector2D pos);
    Vector2D GetPosition();
    void ResetPath();
    virtual void OnCollision(PhysBody* physA, PhysBody* physB);
    virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

protected:
    SDL_Texture* texture;               // Cambiado a protected para acceso en FlyingEnemy
    const char* texturePath;
    int texW, texH;                     // Cambiado a protected
    pugi::xml_node parameters;
    Animation* currentAnimation = nullptr; // Cambiado a protected
    Animation idle;
    PhysBody* pbody;                    // Cambiado a protected
    Pathfinding* pathfinding;           // Cambiado a protected

    bool DrawPathFlag = false;          // Flag to controlar el dibujo del path
};
