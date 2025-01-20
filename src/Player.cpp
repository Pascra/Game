#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "tracy/Tracy.hpp"

Player::Player() : Entity(EntityType::PLAYER), lives(3) {
	name = "Player";
}


Player::~Player() {}

bool Player::Awake()
{
	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(0, 0);
	return true;
}

bool Player::Start()
{
	lives = 3;
	heartTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Heart.png");

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/player1.png");

	// L08 TODO 5: Add physics to the player - initialize physics body
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	// Initialize audio effect
	pickCoinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	jumpFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Salto.ogg");
	fallFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Caer.ogg");
	swordFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Espada.ogg");


	// Set initial position
	initialPosition = Vector2D(50, 50); // Adjust to your initial map position
	SetPosition(initialPosition);

	return true;
}

bool Player::Update(float dt)
{
	ZoneScoped;
	if (pendingToReset) {
		ResetToInitialPosition();
		pendingToReset = false;
	}
	if (isJumpBoostActive) {
		jumpBoostTimer -= dt;
		if (jumpBoostTimer < 0) {
			jumpBoostTimer = 0; // Evita valores negativos
		}
		

		if (jumpBoostTimer == 0) {
			jumpForce = originalJumpForce;
			isJumpBoostActive = false;
			LOG("Jump Boost Deactivated. Restoring original jump force: %f", jumpForce);
		}
	}





	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	// Move left
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		velocity.x = -0.2 * dt;
	}

	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		velocity.x = 0.2 * dt;
	}

	// Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false)
	{
		LOG("It's Jumping");
		Engine::GetInstance().audio.get()->PlayFx(jumpFxId);
		// Apply an initial upward force
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	// If the player is jumping, we don't want to apply gravity; we use the current velocity produced by the jump
	if (isJumping == true)
	{
		velocity.y = pbody->body->GetLinearVelocity().y;
	}

	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);

	// Update player position based on physics body
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	// Check if player has fallen below a certain height
	if (position.getY() > 433) // Adjust this value as needed
	{
		ResetToInitialPosition();
	}

	//Attack
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_SCANCODE_0) == KEY_DOWN) {
		LOG("It's attacking");
		Engine::GetInstance().audio.get()->PlayFx(swordFxId);
	}

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());
	return true;
}
void Player::DrawLives()
{
	int x = 10; // Posición inicial en X (arriba a la izquierda)
	int y = 10; // Posición inicial en Y

	for (int i = 0; i < lives; ++i)
	{
		Engine::GetInstance().render.get()->DrawTexture(heartTexture, x, y, nullptr, 1.0f, 0.0, 0, 0, true); // ignoreCamera = true
		x += 40; // Espacio entre cada corazón
	}
}

void Player::RenderLives() {
	static SDL_Texture* heartTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Heart2.png");

	int heartSize = 32; // Tamaño del corazón
	int xOffset = 10;   // Espaciado entre corazones
	int yOffset = 10;   // Margen superior

	// Dibuja los corazones en la esquina superior izquierda de la pantalla
	for (int i = 0; i < lives; i++) {
		int xPos = xOffset + (heartSize + xOffset) * i;
		Engine::GetInstance().render.get()->DrawTexture(heartTexture, xPos, yOffset);
	}
}





int Player::GetLives() const {
	return lives;
}


void Player::ResetLives() {
	lives = 3;
}

void Player::AddLife() {
	if (lives < 3) {
		lives++;
		LOG("Life added. Current lives: %d", lives);
	}
	else {
		LOG("Lives are already at maximum.");
	}
}




void Player::RemoveLife() {
	if (lives > 0) {
		lives--;
		LOG("Life lost. Current lives: %d", lives);
	}
	if (lives == 0) {
		LOG("No more lives. Resetting to 3.");
		lives = 3; // Reinicia las vidas a 3 cuando se pierden todas
	}
}


bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	Engine::GetInstance().textures.get()->UnLoad(heartTexture); // Descarga la textura del corazón
	return true;
}


// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
	//	LOG("Collision PLATFORM");
		// Reset the jump flag when touching the ground
		isJumping = false;
		break;
	case ColliderType::ITEM:
		//LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
	//	LOG("Collision UNKNOWN");
		break;
	case ColliderType::ENEMY:
	{
	//	LOG("Collision with ENEMY");

		// Obtener la posición del jugador
		Vector2D playerPos = GetPosition();

		// Obtener la posición del enemigo
		b2Vec2 enemyBodyPos = physB->body->GetTransform().p;
		Vector2D enemyPos = Vector2D(METERS_TO_PIXELS(enemyBodyPos.x), METERS_TO_PIXELS(enemyBodyPos.y));

		// Calcular si el jugador está encima del enemigo
		// Considerando la altura del enemigo (texH) y un margen para ajustar la colisión
		if (playerPos.getY() < enemyPos.getY() - (texH / 4)) {
			// El jugador está pisando al enemigo, no se resetea
	//		LOG("Player is above the ENEMY - NO Reset");
		}
		else {
			// El jugador colisiona desde un lado o por debajo, se resetea
			pendingToReset = true;
		//	LOG("Player collided with ENEMY from the side or below - Reset");
		}
		break;
	}

	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		//LOG("End Collision PLATFORM");
		Engine::GetInstance().audio.get()->PlayFx(fallFxId);
		break;
	case ColliderType::ITEM:
		//LOG("End Collision ITEM");
		Engine::GetInstance().audio.get()->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		//LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::SetPosition(Vector2D pos)
{
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Player::GetPosition()
{
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

void Player::ResetToInitialPosition() {
	if (lives > 0) {
		// Si el jugador tiene más de 0 vidas
		lives--;

		if (hasCheckpoint) {
			// Respawn en el último checkpoint
			SetPosition(checkpointPosition);
			LOG("Respawned at checkpoint: (%f, %f)", checkpointPosition.getX(), checkpointPosition.getY());
		}
		else {
			// Respawn en la posición inicial
			SetPosition(initialPosition);
			LOG("Respawned at initial position: (%f, %f)", initialPosition.getX(), initialPosition.getY());
		}

		isJumping = false;
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	}
	else {
		// Sin vidas: Mostrar pantalla de derrota
		LOG("Player ran out of lives! Showing LosingScreen...");
		Scene* scene = Engine::GetInstance().scene.get();
		if (scene != nullptr) {
			scene->ShowLosingScreen(); // Muestra la pantalla de derrota
		}
	}
}



void Player::AddCoin() {
	coinCount++;
	LOG("Coin collected! Total coins: %d", coinCount);
}

int Player::GetCoinCount() const {
	return coinCount;
}
void Player::ActivateJumpBoost(float boostForce, float duration) {
	if (!isJumpBoostActive) {
		originalJumpForce = jumpForce; // Guarda la fuerza original
		jumpForce += boostForce;      // Incrementa la fuerza
		isJumpBoostActive = true;     // Activa el estado
		jumpBoostTimer = duration;    // Establece la duración
		LOG("Jump Boost Activated! New jump force: %f for %f seconds", jumpForce, duration);
	}
}

void Player::SetCheckpoint(const Vector2D& position) {
	checkpointPosition = position;
	hasCheckpoint = true;
	LOG("Checkpoint set at: (%f, %f)", checkpointPosition.getX(), checkpointPosition.getY());
}

