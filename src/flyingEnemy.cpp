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


bool FlyingEnemy::Start() {
    // Llama al método base para inicializar los datos comunes
    Enemy::Start();

    // Asegúrate de que la gravedad esté desactivada
    pbody->body->SetGravityScale(0); // Desactiva la gravedad para enemigos voladores

    return true;
}

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
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    return true;
}

void FlyingEnemy::MoveToTarget(Vector2D targetTile) {
    Vector2D currentTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
    b2Vec2 velocity(0, 0);

    // Calcular diferencia entre el objetivo y la posición actual
    float deltaX = targetTile.getX() - currentTile.getX();
    float deltaY = targetTile.getY() - currentTile.getY();

    // Calcular magnitud para normalizar
    float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);
    if (magnitude > 0) {
        velocity.x = (deltaX / magnitude) * 2.0f; // Velocidad normalizada en X
        velocity.y = (deltaY / magnitude) * 2.0f; // Velocidad normalizada en Y
    }

    // Aplicar la velocidad al cuerpo físico
    pbody->body->SetLinearVelocity(velocity);
}


