#pragma once

#include "Entity.h"
#include "Vector2D.h"
#include "Render.h"


class Projectile : public Entity {
public:
    Projectile(Vector2D position, float speed);
    ~Projectile();

    bool Update(float dt) override; // Actualiza la posici�n del proyectil
    void OnCollision(PhysBody* physA, PhysBody* physB) override; // Maneja colisiones
    virtual void Draw(Render* render) override;

private:
    Vector2D position;          // Posici�n del proyectil
    float speed;                // Velocidad del proyectil
    SDL_Texture* texture;       // Textura del proyectil
    PhysBody* pbody;            // Cuerpo f�sico del proyectil
    
};
