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
    // Dibuja el texto centrado en el botón
    void DrawCenteredText();

private:
    SDL_Rect bounds;          // Límites del botón
    std::string text;         // Texto que se mostrará en el botón
    bool canClick = true;     // Estado de clic del botón
    bool drawBasic = false;   // Dibujar estilo básico o avanzado
};
