#include "Engine.h"
#include "Window.h"
#include "Render.h"
#include "Log.h"
#include "SDL2/SDL_image.h"

#define VSYNC true

Render::Render() : Module()
{
    name = "render";
    background.r = 0;
    background.g = 0;
    background.b = 0;
    background.a = 255; // Fondo opaco
    renderer = nullptr;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake()
{
    //a�adir color al fondo 

    background.r = 135; // Rojo
    background.g = 206; // Verde
    background.b = 235; // Azul
    background.a = 255; // Opacidad

    camera.w = Engine::GetInstance().window.get()->width;
    camera.h = Engine::GetInstance().window.get()->height;

    LOG("Create SDL rendering context");
    bool ret = true;

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    // Cargar configuraci�n de vsync
    if (configParameters.child("vsync").attribute("value").as_bool() == true) {
        flags |= SDL_RENDERER_PRESENTVSYNC;
        LOG("Using vsync");
    }

    // Obtener escala de la ventana
    int scale = Engine::GetInstance().window.get()->GetScale();

    if (scale <= 0) {
        LOG("Invalid scale detected in Render module, setting to 1.");
        scale = 1;
    }

    // Crear el renderer utilizando la ventana SDL
    SDL_Window* window = Engine::GetInstance().window.get()->GetSDLWindow(); // Aseg�rate de que `GetSDLWindow()` est� implementado en `Window`
    if (window == nullptr) {
        LOG("SDL_Window is null! Cannot create renderer.");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, flags);

    if (renderer == nullptr) {
        LOG("Could not create the renderer! SDL_Error: %s", SDL_GetError());
        ret = false;
    }
    else {
        int width, height;
        Engine::GetInstance().window.get()->GetWindowSize(width, height);

        camera.w = width * scale;
        camera.h = height * scale;
        camera.x = 0;
        camera.y = 0;
    }

    return ret;
}

// Called before the first frame
bool Render::Start()
{
    LOG("Render start");
    if (renderer != nullptr) {
        SDL_RenderGetViewport(renderer, &viewport);
    }
    return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
    if (renderer != nullptr) {
        SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
        SDL_RenderClear(renderer);
    }
    else {
        LOG("Renderer is null in PreUpdate. Cannot clear the screen.");
    }
    return true;
}

bool Render::Update(float dt)
{
    return true;
}

bool Render::PostUpdate()
{
    if (renderer != nullptr) {
        SDL_RenderPresent(renderer);
    }
    else {
        LOG("Renderer is null in PostUpdate. Cannot present the screen.");
    }
    return true;
}

// Called before quitting
bool Render::CleanUp()
{
    LOG("Destroying SDL render");

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    return true;
}

// Set the background color
void Render::SetBackgroundColor(SDL_Color color)
{
    background = color;
}

// Set the viewport
void Render::SetViewPort(const SDL_Rect& rect)
{
    if (renderer != nullptr) {
        SDL_RenderSetViewport(renderer, &rect);
    }
    else {
        LOG("Renderer is null. Cannot set viewport.");
    }
}

// Reset the viewport
void Render::ResetViewPort()
{
    if (renderer != nullptr) {
        SDL_RenderSetViewport(renderer, &viewport);
    }
    else {
        LOG("Renderer is null. Cannot reset viewport.");
    }
}

// Draw a texture to the screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
    if (renderer == nullptr || texture == nullptr) {
        LOG("Cannot draw texture. Renderer or texture is null.");
        return false;
    }

    int scale = Engine::GetInstance().window.get()->GetScale();
    if (scale <= 0) scale = 1;

    SDL_Rect rect;
    rect.x = (int)(camera.x * speed) + x * scale;
    rect.y = (int)(camera.y * speed) + y * scale;

    if (section != nullptr) {
        rect.w = section->w;
        rect.h = section->h;
    }
    else {
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    }

    rect.w *= scale;
    rect.h *= scale;

    SDL_Point* p = nullptr;
    SDL_Point pivot;

    if (pivotX != INT_MAX && pivotY != INT_MAX) {
        pivot.x = pivotX;
        pivot.y = pivotY;
        p = &pivot;
    }

    if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0) {
        LOG("Cannot blit to screen. SDL_RenderCopyEx error: %s", SDL_GetError());
        return false;
    }

    return true;
}

// Draw a rectangle
bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
    if (renderer == nullptr) {
        LOG("Renderer is null. Cannot draw rectangle.");
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_Rect rec(rect);
    if (use_camera) {
        rec.x = (int)(camera.x + rect.x);
        rec.y = (int)(camera.y + rect.y);
    }

    int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

    if (result != 0) {
        LOG("Cannot draw rectangle. SDL_RenderFillRect error: %s", SDL_GetError());
        return false;
    }

    return true;
}

// Load a texture
SDL_Texture* Render::LoadTexture(const char* path)
{
    if (renderer == nullptr) {
        LOG("Renderer is null. Cannot load texture.");
        return nullptr;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (texture == nullptr) {
        LOG("Unable to load texture from %s! SDL_image Error: %s", path, IMG_GetError());
    }

    return texture;
}
bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
    if (renderer == nullptr) {
        LOG("Renderer is null. Cannot draw line.");
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (use_camera) {
        x1 += camera.x;
        y1 += camera.y;
        x2 += camera.x;
        y2 += camera.y;
    }

    int result = SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    if (result != 0) {
        LOG("Cannot draw line. SDL_RenderDrawLine error: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
    if (renderer == nullptr) {
        LOG("Renderer is null. Cannot draw circle.");
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    const int diameter = (radius * 2);
    int x_pos = (radius - 1);
    int y_pos = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    while (x_pos >= y_pos) {
        // Draw 8 symmetrical points
        if (use_camera) {
            SDL_RenderDrawPoint(renderer, x + x_pos + camera.x, y - y_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x + x_pos + camera.x, y + y_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x - x_pos + camera.x, y - y_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x - x_pos + camera.x, y + y_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x + y_pos + camera.x, y - x_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x + y_pos + camera.x, y + x_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x - y_pos + camera.x, y - x_pos + camera.y);
            SDL_RenderDrawPoint(renderer, x - y_pos + camera.x, y + x_pos + camera.y);
        }
        else {
            SDL_RenderDrawPoint(renderer, x + x_pos, y - y_pos);
            SDL_RenderDrawPoint(renderer, x + x_pos, y + y_pos);
            SDL_RenderDrawPoint(renderer, x - x_pos, y - y_pos);
            SDL_RenderDrawPoint(renderer, x - x_pos, y + y_pos);
            SDL_RenderDrawPoint(renderer, x + y_pos, y - x_pos);
            SDL_RenderDrawPoint(renderer, x + y_pos, y + x_pos);
            SDL_RenderDrawPoint(renderer, x - y_pos, y - x_pos);
            SDL_RenderDrawPoint(renderer, x - y_pos, y + x_pos);
        }

        if (error <= 0) {
            ++y_pos;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x_pos;
            tx += 2;
            error += (tx - diameter);
        }
    }

    return true;
}
