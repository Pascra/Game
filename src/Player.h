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

    // Define funciones de colisi�n
    void OnCollision(PhysBody* physA, PhysBody* physB);
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

    // Sincroniza la posici�n l�gica con el cuerpo f�sico
    void SetPosition(Vector2D pos);
    Vector2D GetPosition();

    // Funci�n para restablecer al jugador a la posici�n inicial
    void ResetToInitialPosition();

    // Configurar par�metros del jugador desde un nodo XML
    void SetParameters(pugi::xml_node parameters);

public:
    // Declaraci�n de par�metros del jugador
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW = 0, texH = 0;
    SDL_Texture* LosingScreen = nullptr;
    // Cuerpo f�sico del jugador
    PhysBody* pbody = nullptr;

    // Posici�n l�gica del jugador
    Vector2D position;

    // Par�metros de salto
    float jumpForce = 2.5f; // Fuerza de salto
    bool isJumping = false; // Estado del salto

    // ID del efecto de sonido
    int pickCoinFxId = -1;
    int jumpFxId = -1;
    int fallFxId = -1;
    int swordFxId = -1;
    

    // Nodo XML para par�metros del jugador
    pugi::xml_node parameters;

    // Nueva variable: posici�n inicial del jugador
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
    int lives = 3; // N�mero de vidas del jugador
    SDL_Texture* heartTexture; // Textura para el coraz�n
    int coinCount = 0;

    bool isJumpBoostActive = false;   // Indica si el boost de salto est� activo
    float jumpBoostTimer = 0.0f;      // Temporizador para el boost
    float originalJumpForce = 0.0f;

};
