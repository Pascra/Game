#pragma once


#include "SDL2/SDL.h"

// Clase para manejar animaciones de sprites
class Animation {
public:
    // Constructor predeterminado
    Animation();

    // Constructor parametrizado
    Animation(SDL_Texture* texture, int frameCount, int frameWidth, int frameHeight, float frameSpeed);

    // Actualiza la animaci�n (se debe llamar en cada frame)
    void Update(float dt);

    // Obtiene el rect�ngulo del frame actual
    SDL_Rect GetCurrentFrameRect() const;

    // Reinicia la animaci�n al primer frame
    void Reset();

    // Establece una nueva textura para la animaci�n
    void SetTexture(SDL_Texture* newTexture, int newFrameCount, int newFrameWidth, int newFrameHeight);

    // Devuelve la textura asociada
    SDL_Texture* GetTexture() const;

private:
    SDL_Texture* texture;    // Textura de la animaci�n
    int frameCount;          // N�mero total de frames
    int currentFrame;        // Frame actual
    int frameWidth;          // Ancho de cada frame
    int frameHeight;         // Alto de cada frame
    float frameSpeed;        // Tiempo entre frames
    float elapsedTime;       // Tiempo acumulado para cambiar de frame
};

