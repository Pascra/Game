#include "FlyingEnemy.h"
#include "Engine.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "Render.h"
#include "Projectile.h"
#include "EntityManager.h"

#define METERS_TO_PIXELS(m) ((int)((m) * 100.0f))

FlyingEnemy::FlyingEnemy() : Enemy(), shootTimer(0.0f), shootCooldown(2.0f) {
    LOG("FlyingEnemy created");
}

FlyingEnemy::~FlyingEnemy() {}

bool FlyingEnemy::Update(float dt) {
    shootTimer += dt;
    LOG("FlyingEnemy shootTimer: %f", shootTimer);

    if (shootTimer >= shootCooldown) {
        shootTimer = 0.0f;
        ShootProjectile();
    }

    // Resto de la lógica de movimiento
    ResetPath();
    while (pathfinding->pathTiles.empty()) {
        pathfinding->PropagateAStar(SQUARED);
    }

    if (!pathfinding->pathTiles.empty()) {
        Vector2D targetTile = pathfinding->pathTiles.front();
        pathfinding->pathTiles.pop_front();
        MoveToTarget(targetTile);
    }

    // Dibujar enemigo
    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);
    Engine::GetInstance().render->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    return true;
}

void FlyingEnemy::MoveToTarget(Vector2D targetTile) {
    Vector2D currentTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
    b2Vec2 velocity(0, 0);

    if (targetTile.getX() < currentTile.getX()) velocity.x = -2.0f;
    if (targetTile.getX() > currentTile.getX()) velocity.x = 2.0f;

    if (targetTile.getY() < currentTile.getY()) velocity.y = -2.0f;
    if (targetTile.getY() > currentTile.getY()) velocity.y = 2.0f;

    pbody->body->SetLinearVelocity(velocity);
}

void FlyingEnemy::ShootProjectile() {
    LOG("Shooting projectile from position: (%f, %f)", position.getX(), position.getY());
    Vector2D projectilePosition = position;
    projectilePosition.setY(projectilePosition.getY() - 16); // Ajusta la posición

    // Crear y añadir el proyectil
    Projectile* projectile = new Projectile(projectilePosition, -200.0f);
    if (projectile) {
        Engine::GetInstance().entityManager->AddEntity(projectile);
    }
}



Vector2D FlyingEnemy::GetPosition() const {
    LOG("FlyingEnemy position: (%f, %f)", position.getX(), position.getY());
    return position;
}

