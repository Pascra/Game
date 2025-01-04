#pragma once

#include "Enemy.h"

class FlyingEnemy : public Enemy {
public:
    FlyingEnemy();
    ~FlyingEnemy();

  
    bool Update(float dt) override;

private:
    void MoveToTarget(Vector2D targetPosition);
};
