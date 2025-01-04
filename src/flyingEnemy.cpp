#include "FlyingEnemy.h"
#include "Engine.h"
#include "Log.h"
#include "Physics.h" 
#include "Map.h"      
#include "Render.h"   
#define METERS_TO_PIXELS(m) ((int)((m) * 100.0f)) 


FlyingEnemy::FlyingEnemy() : Enemy() {
    LOG("FlyingEnemy created");
}

FlyingEnemy::~FlyingEnemy() {}

bool FlyingEnemy::Update(float dt) {
    // Pathfinding logic (same as `Enemy`)
    ResetPath();
    while (pathfinding->pathTiles.empty()) {
        pathfinding->PropagateAStar(SQUARED);
    }

    // Move towards the target tile
    if (!pathfinding->pathTiles.empty()) {
        Vector2D targetTile = pathfinding->pathTiles.front();
        pathfinding->pathTiles.pop_front();

        MoveToTarget(targetTile);
    }

    // Draw texture and animation
    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);
    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    return true;
}

void FlyingEnemy::MoveToTarget(Vector2D targetTile) {
    Vector2D currentTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
    b2Vec2 velocity(0, 0);

    // Ajustar velocidad en el eje X
    if (targetTile.getX() < currentTile.getX()) velocity.x = -2.0f; // Mover a la izquierda
    if (targetTile.getX() > currentTile.getX()) velocity.x = 2.0f;  // Mover a la derecha

    // Ajustar velocidad en el eje Y
    if (targetTile.getY() < currentTile.getY()) velocity.y = -2.0f; // Mover hacia arriba
    if (targetTile.getY() > currentTile.getY()) velocity.y = 2.0f;  // Mover hacia abajo

    // Aplicar la velocidad al cuerpo físico
    pbody->body->SetLinearVelocity(velocity);
}

