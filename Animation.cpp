#include "Animation.h"

// Constructor predeterminado
Animation::Animation()
    : texture(nullptr), frameCount(0), currentFrame(0),
    frameWidth(0), frameHeight(0), frameSpeed(0.0f), elapsedTime(0.0f) {}

// Constructor parametrizado
Animation::Animation(SDL_Texture* texture, int frameCount, int frameWidth, int frameHeight, float frameSpeed)
    : texture(texture), frameCount(frameCount), currentFrame(0),
    frameWidth(frameWidth), frameHeight(frameHeight), frameSpeed(frameSpeed), elapsedTime(0.0f) {}

// Actualiza la animación
void Animation::Update(float dt) {
    elapsedTime += dt;
    if (elapsedTime >= frameSpeed) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount; // Avanza al siguiente frame en bucle
    }
}

// Devuelve el rectángulo del frame actual
SDL_Rect Animation::GetCurrentFrameRect() const {
    return { currentFrame * frameWidth, 0, frameWidth, frameHeight };
}

// Reinicia la animación al primer frame
void Animation::Reset() {
    currentFrame = 0;
    elapsedTime = 0.0f;
}

// Establece una nueva textura para la animación
void Animation::SetTexture(SDL_Texture* newTexture, int newFrameCount, int newFrameWidth, int newFrameHeight) {
    texture = newTexture;
    frameCount = newFrameCount;
    frameWidth = newFrameWidth;
    frameHeight = newFrameHeight;
    currentFrame = 0;
    elapsedTime = 0.0f;
}

// Devuelve la textura asociada
SDL_Texture* Animation::GetTexture() const {
    return texture;
}
