#pragma once

#include "Enemy.h"
#include "Vector2D.h"
class FlyingEnemy : public Enemy {
public:
    FlyingEnemy();
    ~FlyingEnemy();
    void ShootProjectile();
    Vector2D GetPosition() const;
    bool Update(float dt) override;
  
private:
    void MoveToTarget(Vector2D targetPosition);
    float shootCooldown;
    float shootTimer;
    float shootInterval;

};
