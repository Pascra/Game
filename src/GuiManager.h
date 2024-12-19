#pragma once

#include "Module.h"
#include "GuiControl.h"
#include <list>
#include <string>

class GuiManager : public Module
{
public:
    // Constructor
    GuiManager();

    // Destructor
    virtual ~GuiManager();

    // Called before the first frame
    bool Start() override;

    // Called each loop iteration
    bool Update(float dt) override;

    // Called before quitting
    bool CleanUp() override;

    // Create a new GUI control and add it to the list
    GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0, 0, 0, 0 });

    // Destroy a GUI control and remove it from the list
    void DestroyGuiControl(GuiControl* control);

    // Additional methods for managing GUI controls
    void DrawGuiControls();

private:
    // List of all active GUI controls
    std::list<GuiControl*> guiControlsList;

    // Texture used for GUI controls (e.g., buttons, sliders)
    SDL_Texture* texture = nullptr;
};

