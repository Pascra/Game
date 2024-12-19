#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

#include <string>

class GuiControlButton : public GuiControl
{
public:
    // Constructor
    GuiControlButton(int id, SDL_Rect bounds, const char* text);

    // Destructor
    virtual ~GuiControlButton();

    // Called each loop iteration
    bool Update(float dt) override;

private:
    // Dibuja el texto centrado en el bot�n
    void DrawCenteredText();

private:
    SDL_Rect bounds;          // L�mites del bot�n
    std::string text;         // Texto que se mostrar� en el bot�n
    bool canClick = true;     // Estado de clic del bot�n
    bool drawBasic = false;   // Dibujar estilo b�sico o avanzado
};
