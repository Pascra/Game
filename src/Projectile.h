#pragma once

#include "Entity.h"
#include "Vector2D.h"
#include "Render.h"


class Projectile : public Entity {
public:
    Projectile(Vector2D position, float speed);
    ~Projectile();

    bool Update(float dt) override; // Actualiza la posición del proyectil
    void OnCollision(PhysBody* physA, PhysBody* physB) override; // Maneja colisiones
    virtual void Draw(Render* render) override;

private:
    Vector2D position;          // Posición del proyectil
    float speed;                // Velocidad del proyectil
    SDL_Texture* texture;       // Textura del proyectil
    PhysBody* pbody;            // Cuerpo físico del proyectil
    
};
