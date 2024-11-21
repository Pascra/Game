#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Vector2D.h"

struct SDL_Texture;

class Player : public Entity
{
public:
    Player();
    virtual ~Player();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();

    // Define funciones de colisión
    void OnCollision(PhysBody* physA, PhysBody* physB);
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

    // Sincroniza la posición lógica con el cuerpo físico
    void SyncPhysicsToPosition();

public:
    // Declaración de parámetros del jugador
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW = 0, texH = 0;

    // Cuerpo físico del jugador
    PhysBody* pbody = nullptr;

    // Posición lógica del jugador
    Vector2D position;

    // Parámetros de salto
    float jumpForce = 2.5f; // Fuerza de salto
    bool isJumping = false; // Estado del salto

    // ID del efecto de sonido
    int pickCoinFxId = -1;
};
