#include "GuiControlButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{
}

bool GuiControlButton::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

        // Verifica si el ratón está sobre el botón
        if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w &&
            mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h)
        {
            state = GuiControlState::FOCUSED;

            if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
            {
                NotifyObserver();
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
        }

        // Dibuja el botón y el texto basado en el estado
        SDL_Color color;
        switch (state)
        {
        case GuiControlState::DISABLED:
            color = { 200, 200, 200, 255 }; // Gris
            break;
        case GuiControlState::NORMAL:
            color = { 0, 0, 255, 255 }; // Azul
            break;
        case GuiControlState::FOCUSED:
            color = { 0, 0, 20, 255 }; // Azul oscuro
            break;
        case GuiControlState::PRESSED:
            color = { 0, 255, 0, 255 }; // Verde
            break;
        }

        // Dibuja el fondo del botón
        Engine::GetInstance().render->DrawRectangle(bounds, color.r, color.g, color.b, color.a, true, false);

        // Dibuja el texto centrado
        DrawCenteredText();
    }

    return false;
}

void GuiControlButton::DrawCenteredText()
{
    int textWidth = 0, textHeight = 0;

    // Calcula el tamaño del texto
    Engine::GetInstance().render->CalculateTextSize(text.c_str(), textWidth, textHeight);

    // Calcula la posición para centrar el texto
    int textX = bounds.x + (bounds.w - textWidth) / 2;
    int textY = bounds.y + (bounds.h - textHeight) / 2;

    // Dibuja el texto
    SDL_Color textColor = { 255, 255, 255, 255 }; // Blanco
    Engine::GetInstance().render->DrawText(text.c_str(), textX, textY, textColor);
}
