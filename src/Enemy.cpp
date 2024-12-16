#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "EntityManager.h"
#include "tracy/Tracy.hpp"

Enemy::Enemy() : Entity(EntityType::ENEMY), pathfinding(nullptr), pbody(nullptr)
{
}

Enemy::~Enemy() {
    if (pathfinding) {
        delete pathfinding;
    }
}

bool Enemy::Awake() {
    return true;
}

bool Enemy::Start() {
    // Initialize flag for path drawing
    DrawPathFlag = false;

    // Initialize textures
    texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
    position.setX(parameters.attribute("x").as_int());
    position.setY(parameters.attribute("y").as_int());
    texW = parameters.attribute("w").as_int();
    texH = parameters.attribute("h").as_int();

    // Load animations
    idle.LoadAnimations(parameters.child("animations").child("idle"));
    currentAnimation = &idle;

    // Initialize physics body
    pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

    // Assign collider type
    pbody->ctype = ColliderType::ENEMY;
    pbody->listener = this;

    // Set gravity for the physics body
    bool gravity = parameters.attribute("gravity").as_bool(); // Store gravity in a variable to avoid errors
    if (!gravity) pbody->body->SetGravityScale(0);

    // Initialize pathfinding
    pathfinding = new Pathfinding();
    ResetPath();

    return true;
}

bool Enemy::Update(float dt) {
    ZoneScoped;

    // Testing pathfinding inputs for various algorithms
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
        Vector2D pos = GetPosition();
        Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
        pathfinding->ResetPath(tilePos);
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
        pathfinding->PropagateBFS();
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_J) == KEY_REPEAT && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
        pathfinding->PropagateBFS();
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
        pathfinding->PropagateDijkstra();
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_K) == KEY_REPEAT && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
        pathfinding->PropagateDijkstra();
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
        pathfinding->PropagateAStar(MANHATTAN);
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_B) == KEY_REPEAT && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
        pathfinding->PropagateAStar(MANHATTAN);
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
        pathfinding->PropagateAStar(EUCLIDEAN);
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_N) == KEY_REPEAT && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
        pathfinding->PropagateAStar(EUCLIDEAN);
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
        pathfinding->PropagateAStar(SQUARED);
    }

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_M) == KEY_REPEAT && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
        pathfinding->PropagateAStar(SQUARED);
    }

    // Reset pathfinding and find the path
    ResetPath();
    while (pathfinding->pathTiles.empty()) {
        pathfinding->PropagateAStar(SQUARED);
    }

    // Move enemy based on pathfinding
    Vector2D posPath;
    for (int i = 0; i < pathfinding->pathTiles.size() - 1; i++) {
        posPath = pathfinding->pathTiles.front();
        pathfinding->pathTiles.remove(posPath);
    }

    // Update enemy's position using physics
    Vector2D posEnemyTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
    if (posPath.getX() < posEnemyTile.getX()) pbody->body->SetLinearVelocity(b2Vec2(-1.4f, 0));
    if (posPath.getX() > posEnemyTile.getX()) pbody->body->SetLinearVelocity(b2Vec2(1.4f, 0));

    // Update position based on physics body transform
    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    // Draw enemy texture
    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    // Toggle DrawPathFlag when F7 is pressed
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
        DrawPathFlag = !DrawPathFlag;
    }

    // Draw path if the flag is set
    if (DrawPathFlag) {
        pathfinding->DrawPath();
    }

    return true;
}

bool Enemy::CleanUp() {
    Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
    return true;
}

void Enemy::SetPosition(Vector2D pos) {
    pos.setX(pos.getX() + texW / 2);
    pos.setY(pos.getY() + texH / 2);
    b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
    pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Enemy::GetPosition() {
    b2Vec2 bodyPos = pbody->body->GetTransform().p;
    Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
    return pos;
}

void Enemy::ResetPath() {
    Vector2D pos = GetPosition();
    Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
    pathfinding->ResetPath(tilePos);
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
    if (physB->ctype == ColliderType::PLAYER) {
        Vector2D enemyPos = GetPosition();
        b2Vec2 playerBodyPos = physB->body->GetPosition();
        Vector2D playerPos(METERS_TO_PIXELS(playerBodyPos.x), METERS_TO_PIXELS(playerBodyPos.y));

        if (playerPos.getY() < enemyPos.getY() - (texH / 4)) {
            LOG("Player is above the enemy - DESTROY");
            Engine::GetInstance().entityManager.get()->DestroyEntity(this);
        }
        else {
            LOG("Player collided but not from above");
        }
    }
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
    if (physB->ctype == ColliderType::PLAYER) {
        LOG("Collision ended with player");
    }
}

// Implement the SetParameters function
void Enemy::SetParameters(pugi::xml_node parameters) {
    this->parameters = parameters; // Simply assign the XML node to the member variable
}
