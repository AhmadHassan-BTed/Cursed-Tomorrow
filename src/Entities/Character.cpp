#include "CursedTomorrow/Entities/Character.hpp"
#include "CursedTomorrow/Core/Config.hpp"
#include "CursedTomorrow/Core/ResourceManager.hpp"

namespace CursedTomorrow {
namespace Entities {

Character::Character() 
    : isJumping(false)
    , jumpCounter(0)
    , currentSpeed(Config::DefaultSpeed)
    , pingEffect(false)
    , pingMeter(0.0f)
    , lastKnownPos({0, 0})
{
    ResetPosition();
}

void Character::ResetPosition() {
    position = { (float)Config::ScreenWidth / 10.1f, (float)Config::ScreenHeight / 2.0f };
    lastKnownPos = position;
}

void Character::Update(bool inverted) {
    Move(inverted);
    Jump(inverted);
    ApplyGravity();
}

void Character::Draw() {
    auto& rm = Core::ResourceManager::GetInstance();
    Texture2D stickMan = rm.LoadTextureResource("assets/mainScreen/StickManPng.png");

    if (!pingEffect) {
        DrawTexture(stickMan, (int)position.x, (int)position.y, WHITE);
        lastKnownPos = position;
    } else {
        pingMeter += 1.0f;
        if (pingMeter < 30.0f) {
            DrawTexture(stickMan, (int)position.x, (int)position.y, WHITE);
            lastKnownPos = position;
        } else {
            DrawTexture(stickMan, (int)lastKnownPos.x, (int)lastKnownPos.y, WHITE);
            if (pingMeter > 60.0f) pingMeter = 0;
        }
    }
}

void Character::Move(bool inverted) {
    if (!inverted) {
        if (IsKeyDown(KEY_RIGHT)) position.x += currentSpeed;
        if (IsKeyDown(KEY_LEFT)) position.x -= currentSpeed;
        if (IsKeyDown(KEY_UP)) position.y -= currentSpeed;
        if (IsKeyDown(KEY_DOWN)) position.y += currentSpeed;
    } else {
        if (IsKeyDown(KEY_LEFT)) position.x += currentSpeed;
        if (IsKeyDown(KEY_SPACE)) position.x -= currentSpeed;
    }

    // Drift logic from original
    if (position.x <= 42) position.x -= 0.8f;
    if (position.x >= 1310) position.x -= 1.0f;

    // Creepy entry logic from original
    if (position.x <= 0) {
        position.x -= 1.0f;
        if (position.x <= -60) {
            position.x = 1450;
            position.y = 500;
        }
    }
    
    if (position.y >= (Config::ScreenHeight - Config::StickManHeight)) 
        position.y = (float)Config::ScreenHeight - Config::StickManHeight;
    
    if (position.y <= 40) position.y = 40;
}

void Character::Jump(bool inverted) {
    jumpCounter += 1.0f;

    bool jumpKeyPressed = !inverted ? IsKeyDown(KEY_SPACE) : IsKeyDown(KEY_RIGHT);

    // Original logic: if (counter - jumpDelay > jumpHeight && IsKeyDown(KEY_SPACE))
    if (jumpCounter - Config::JumpDelay > Config::JumpHeight && jumpKeyPressed) {
        isJumping = true;
        jumpCounter = 0;
    }

    // Original logic: if (counter == jumpHeight)
    if (jumpCounter == Config::JumpHeight) {
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
