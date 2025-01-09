#include "Projectile.h"
#include "Engine.h"
#include "Physics.h"
#include "Render.h"
#include "Player.h"
#include "EntityManager.h"
#include "Log.h"
#include "Config.h"
#include "Textures.h"
#include "Window.h"

// Constructor del proyectil
Projectile::Projectile(Vector2D position, float speed)
    : Entity(EntityType::PROJECTILE), position(position), speed(speed) {
    LOG("Creating Projectile at position (%f, %f) with speed %f", position.getX(), position.getY(), speed);

    // Cargar textura del proyectil
    texture = Engine::GetInstance().textures->Load("Assets/Textures/Projectile.png");
    if (!texture) {
        LOG("Failed to load projectile texture.");
    }

    // Crear el cuerpo físico del proyectil
    pbody = Engine::GetInstance().physics->CreateRectangle(
        position.getX(), position.getY(), 16, 16, bodyType::DYNAMIC
    );
    if (pbody) {
        pbody->ctype = ColliderType::PROJECTILE;
        pbody->listener = this; // Este objeto manejará las colisiones del proyectil
    }

    else {
        LOG("Failed to create projectile physics body.");
    }
}

// Destructor del proyectil
Projectile::~Projectile() {
    if (texture) {
        Engine::GetInstance().textures->UnLoad(texture);
    }
   
}

// Actualización del proyectil
bool Projectile::Update(float dt) {
    LOG("Projectile updating at position (%f, %f)", position.getX(), position.getY());
    position.setY(position.getY() + speed * dt);

    int screenHeight = Engine::GetInstance().window->GetHeight();
    if (position.getY() > screenHeight) {
        LOG("Projectile out of screen. Destroying...");
        Engine::GetInstance().physics->DestroyBody(pbody->body);
        Engine::GetInstance().entityManager->DestroyEntity(this);
        return false;
    }

    Engine::GetInstance().render->DrawTexture(texture, position.getX(), position.getY());
    return true;
}

void Projectile::Draw(Render* render) {
    LOG("Drawing projectile at: (%f, %f)", position.getX(), position.getY());
    render->DrawTexture(texture, (int)position.getX(), (int)position.getY(), nullptr);
}





// Manejo de colisiones
void Projectile::OnCollision(PhysBody* physA, PhysBody* physB) {
    if (physB->ctype == ColliderType::PLAYER) {
        LOG("Projectile collided with Player.");
        Player* player = Engine::GetInstance().entityManager->GetPlayer();
        if (player) {
            player->RemoveLife();
        }
        Engine::GetInstance().entityManager->DestroyEntity(this); // Destruye el proyectil
    }
    else if (physB->ctype == ColliderType::PLATFORM) {
        LOG("Projectile collided with Platform.");
        Engine::GetInstance().entityManager->DestroyEntity(this); // Destruye el proyectil
    }
}

