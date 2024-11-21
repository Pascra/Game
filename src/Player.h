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

    // Define funciones de colisi�n
    void OnCollision(PhysBody* physA, PhysBody* physB);
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

    // Sincroniza la posici�n l�gica con el cuerpo f�sico
    void SyncPhysicsToPosition();

public:
    // Declaraci�n de par�metros del jugador
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW = 0, texH = 0;

    // Cuerpo f�sico del jugador
    PhysBody* pbody = nullptr;

    // Posici�n l�gica del jugador
    Vector2D position;

    // Par�metros de salto
    float jumpForce = 2.5f; // Fuerza de salto
    bool isJumping = false; // Estado del salto

    // ID del efecto de sonido
    int pickCoinFxId = -1;
};
