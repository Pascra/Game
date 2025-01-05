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

    // Llamado antes de que el render est� disponible
    bool Awake();

    // Llamado despu�s de Awake
    bool Start();

    // Llamado cada frame
    bool Update(float dt);

    // Llamado antes de salir
    bool CleanUp();

    // Crear entidad gen�rica
    Entity* CreateEntity(EntityType type);

    // Crear entidad con par�metros XML
    Entity* CreateEntity(EntityType type, pugi::xml_node parameters);

    // Destruir una entidad espec�fica
    void DestroyEntity(Entity* entity);

    // A�adir una entidad existente
    void AddEntity(Entity* entity);

public:
    std::list<Entity*> entities; // Lista de entidades
};
