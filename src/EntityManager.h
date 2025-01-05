#pragma once

#include "Module.h"
#include "Entity.h"
#include <list>
#include "pugixml.hpp"

class EntityManager : public Module
{
public:
    EntityManager();

    // Destructor
    virtual ~EntityManager();

    // Llamado antes de que el render esté disponible
    bool Awake();

    // Llamado después de Awake
    bool Start();

    // Llamado cada frame
    bool Update(float dt);

    // Llamado antes de salir
    bool CleanUp();

    // Crear entidad genérica
    Entity* CreateEntity(EntityType type);

    // Crear entidad con parámetros XML
    Entity* CreateEntity(EntityType type, pugi::xml_node parameters);

    // Destruir una entidad específica
    void DestroyEntity(Entity* entity);

    // Añadir una entidad existente
    void AddEntity(Entity* entity);

public:
    std::list<Entity*> entities; // Lista de entidades
};
