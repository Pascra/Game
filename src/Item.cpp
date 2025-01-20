#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Player.h"
#include "tracy/Tracy.hpp"

Item::Item() : Entity(EntityType::ITEM), itemType(ItemType::COIN), isPicked(false)
{
    name = "item";
}


Item::~Item() {}

bool Item::Awake() {
    return true;
}

bool Item::Start() {
    // Cargar textura según el tipo de ítem
    switch (itemType) {
    case ItemType::COIN:
        texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/goldCoin.png");
        break;
    case ItemType::HEART:
        texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Heart2.png");
        break;
        case ItemType::JUMP_BOOST:
        jumpBoostForce = 2.0f;       // Fuerza más moderada
        jumpBoostDuration = 10000.0f;    // Duración más corta
        texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Watermelon.png");
        break;
        case ItemType::CHECKPOINT:
            texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Checkpoint.png");
            break;

    }
    // Configurar tamaño y cuerpo físico
    Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
    pbody = Engine::GetInstance().physics.get()->CreateCircle(
        (int)position.getX() + texH / 2,
        (int)position.getY() + texH / 2,
        texH / 2,
        bodyType::DYNAMIC
    );

    pbody->ctype = ColliderType::ITEM;
    pbody->listener = this; // Asignar el listener
    return true;
}


bool Item::Update(float dt) {
    ZoneScoped;

    // Actualizar la posición del cuerpo físico
    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    // Dibujar la textura
    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());
    return true;
}
void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
    if (!isPicked && physB->ctype == ColliderType::PLAYER) {
        Player* player = Engine::GetInstance().entityManager->GetPlayer();
        if (player != nullptr) {
            if (itemType == ItemType::COIN) {
                player->AddCoin(); // Incrementa el contador de monedas del jugador
            }
            else if (itemType == ItemType::HEART) {
                player->AddLife(); // Incrementa las vidas del jugador
            }
            else if (itemType == ItemType::JUMP_BOOST) {
                player->ActivateJumpBoost(jumpBoostForce, jumpBoostDuration);
            }
            else if (itemType == ItemType::CHECKPOINT) {
                player->SetCheckpoint(position); // Guarda la posición del checkpoint
                LOG("Checkpoint activated at position: (%f, %f)", position.getX(), position.getY());
                isPicked = true; // Marca como recogido
                Engine::GetInstance().entityManager->DestroyEntity(this); // Si el checkpoint desaparece tras activarse
            }


            isPicked = true; // Marca el ítem como recogido
            Engine::GetInstance().entityManager->DestroyEntity(this); // Destruye la entidad
        }
    }
}








bool Item::CleanUp() {
    if (pbody != nullptr) {
        Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
        pbody = nullptr;
    }
    return true;
}


void Item::SetItemType(ItemType type) {
    itemType = type;
}

void Item::SetPosition(const Vector2D& position) {
    this->position = position;
    if (pbody) {
        pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY())), 0);
    }
}
