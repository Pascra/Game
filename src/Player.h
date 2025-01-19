#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Vector2D.h"

// Forward declaration of PhysBody
struct PhysBody;
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
    void SetPosition(Vector2D pos);
    Vector2D GetPosition();

    // Función para restablecer al jugador a la posición inicial
    void ResetToInitialPosition();

    // Configurar parámetros del jugador desde un nodo XML
    void SetParameters(pugi::xml_node parameters);

public:
    // Declaración de parámetros del jugador
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW = 0, texH = 0;
    SDL_Texture* LosingScreen = nullptr;
    // Cuerpo físico del jugador
    PhysBody* pbody = nullptr;

    // Posición lógica del jugador
    Vector2D position;

    // Parámetros de salto
    float jumpForce = 2.5f; // Fuerza de salto
    bool isJumping = false; // Estado del salto

    // ID del efecto de sonido
    int pickCoinFxId = -1;
    int jumpFxId = -1;
    int fallFxId = -1;
    int swordFxId = -1;
    

    // Nodo XML para parámetros del jugador
    pugi::xml_node parameters;

    // Nueva variable: posición inicial del jugador
    Vector2D initialPosition;

    bool pendingToReset = false;
    void DrawLives();
    void ActivateJumpBoost(float boostForce, float duration);
    int GetLives() const; // Obtener vidas actuales
    void AddLife();       // Incrementar vidas
    void RemoveLife();           // Decrementa las vidas
    void ResetLives();
    void RenderLives();
    void AddCoin();         // Incrementa el contador de monedas
    int GetCoinCount() const;
private:
    int lives = 3; // Número de vidas del jugador
    SDL_Texture* heartTexture; // Textura para el corazón
    int coinCount = 0;

    bool isJumpBoostActive = false;   // Indica si el boost de salto está activo
    float jumpBoostTimer = 0.0f;      // Temporizador para el boost
    float originalJumpForce = 0.0f;

};
