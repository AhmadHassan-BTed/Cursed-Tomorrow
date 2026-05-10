#pragma once

#include "raylib.h"

namespace CursedTomorrow {
namespace Entities {

class Character {
public:
    Character();
    
    void Update(bool inverted = false);
    void Draw();

    void SetPosition(Vector2 pos) { position = pos; }
    Vector2 GetPosition() const { return position; }
    void SetPingEffect(bool active) { pingEffect = active; }

    void ResetPosition();
    
    void Jump(bool inverted = false);
    void Move(bool inverted = false);
    void ApplyGravity();
    void StopJumping() { isJumping = false; }

private:
    Vector2 position;
    bool isJumping;
    float jumpCounter;
    float currentSpeed;
    bool pingEffect;
    float pingMeter;
    Vector2 lastKnownPos;
};

}
}
