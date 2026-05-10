#pragma once

#include "raylib.h"

namespace CursedTomorrow {
namespace Entities {

class Character {
public:
    Character();
    
    void Update();
    void Draw();

    void SetPosition(Vector2 pos) { position = pos; }
    Vector2 GetPosition() const { return position; }

    void ResetPosition();
    
    void Jump();
    void Move();
    void ApplyGravity();

private:
    Vector2 position;
    bool isJumping;
    float jumpCounter;
    float currentSpeed;
};

}
}
