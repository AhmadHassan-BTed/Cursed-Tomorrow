#include "CursedTomorrow/Entities/Character.hpp"
#include "CursedTomorrow/Core/Config.hpp"
#include "CursedTomorrow/Core/ResourceManager.hpp"

namespace CursedTomorrow {
namespace Entities {

Character::Character() 
    : isJumping(false)
    , jumpCounter(0)
    , currentSpeed(Config::DefaultSpeed)
{
    ResetPosition();
}

void Character::ResetPosition() {
    position = { (float)Config::ScreenWidth / 1.2f, (float)Config::ScreenHeight / 2.0f };
}

void Character::Update() {
    Move();
    Jump();
    ApplyGravity();
}

void Character::Draw() {
    auto& rm = Core::ResourceManager::GetInstance();
    Texture2D stickMan = rm.LoadTextureResource("assets/mainScreen/StickManPng.png");
    DrawTexture(stickMan, (int)position.x, (int)position.y, WHITE);
}

void Character::Move() {
    if (IsKeyDown(KEY_RIGHT)) position.x += currentSpeed;
    if (IsKeyDown(KEY_LEFT)) position.x -= currentSpeed;

    if (position.x <= 42) position.x -= 0.8f;
    if (position.x >= 1310) position.x -= 1.0f;
    
    if (position.y >= (Config::ScreenHeight - Config::StickManHeight)) 
        position.y = (float)Config::ScreenHeight - Config::StickManHeight;
    
    if (position.y <= 40) position.y = 40;
}

void Character::Jump() {
    jumpCounter += 1.0f;

    if (jumpCounter > Config::JumpDelay && IsKeyDown(KEY_SPACE)) {
        isJumping = true;
        jumpCounter = 0;
    }

    if (jumpCounter >= Config::JumpHeight) {
        isJumping = false;
    }

    if (isJumping) {
        position.y -= 19.0f;
    }
}

void Character::ApplyGravity() {
    position.y += Config::Gravity;
}

}
}
