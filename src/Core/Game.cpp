#include "CursedTomorrow/Core/Game.hpp"
#include "CursedTomorrow/Core/Config.hpp"
#include "CursedTomorrow/Core/ResourceManager.hpp"
#include <iostream>

namespace CursedTomorrow {
namespace Core {

Game::Game() 
    : currentState(GameState::MainScreen)
    , logPositionY(250.0f)
    , buttonPositionY(380.0f)
    , logMove(false)
    , logMoveOnlyOnce(true)
    , delayMeter(0)
    , variable(2.2f)
    , commonIterator(0)
{
}

Game::~Game() {
    CloseAudioDevice();
    CloseWindow();
}

void Game::Run() {
    Initialize();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

void Game::Initialize() {
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, Config::WindowTitle.c_str());
    InitAudioDevice();
    SetTargetFPS(Config::TargetFPS);

    auto& rm = ResourceManager::GetInstance();
    Music levelMusic = rm.LoadMusicResource("assets/audio/levelMusic.mp3");
    PlayMusicStream(levelMusic);
}

void Game::Update() {
    auto& rm = ResourceManager::GetInstance();
    UpdateMusicStream(rm.LoadMusicResource("assets/audio/levelMusic.mp3"));

    HandleInput();

    if (currentState >= GameState::Level1 && currentState <= GameState::Level6) {
        UpdateLevelLogic();
        player.Update();
    }
}

void Game::UpdateLevelLogic() {
    switch (currentState) {
        case GameState::Level4:
            player.SetPosition({ player.GetPosition().x - 2.0f, player.GetPosition().y });
            break;
        default:
            break;
    }
}

void Game::HandleInput() {
    Vector2 mousePos = GetMousePosition();

    if (currentState == GameState::MainScreen) {
        if (CheckCollisionPointRec(mousePos, { 170, 80, 216, 34 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::Level1;
        }
        if (CheckCollisionPointRec(mousePos, { 210, 160, 118, 33 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::LevelSelect;
        }
        if (CheckCollisionPointRec(mousePos, { 195, 240, 150, 34 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::Credits;
        }
    } else if (currentState == GameState::LevelSelect) {
         if (CheckCollisionPointRec(mousePos, { 60, 40, 96, 55 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::MainScreen;
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentState) {
        case GameState::MainScreen: DrawMainScreen(); break;
        case GameState::LevelSelect: DrawLevelSelect(); break;
        case GameState::Credits: DrawCredits(); break;
        case GameState::Level1: 
        case GameState::Level2:
        case GameState::Level3:
        case GameState::Level4:
        case GameState::Level5:
        case GameState::Level6:
            DrawLevelTemplate();
            player.Draw();
            break;
        case GameState::YouDied: DrawYouDied(); break;
        default: break;
    }

    EndDrawing();
}

void Game::DrawMainScreen() {
    auto& rm = ResourceManager::GetInstance();
    Texture2D mainBg = rm.LoadTextureResource("assets/mainScreen/mainScreen.png");
    DrawTexture(mainBg, 0, 0, WHITE);
    
    DrawTexture(rm.LoadTextureResource("assets/mainScreen/newGame.png"), 170, 80, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/mainScreen/levels.png"), 210, 160, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/mainScreen/credits.png"), 195, 240, WHITE);
}

void Game::DrawLevelSelect() {
    auto& rm = ResourceManager::GetInstance();
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/selectorBackground.png"), 0, 0, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/box1.png"), 200, 200, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/backButton.png"), 60, 40, WHITE);
}

void Game::DrawCredits() {
    auto& rm = ResourceManager::GetInstance();
    DrawTexture(rm.LoadTextureResource("assets/creditScreen/creditBackground.png"), 0, 0, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/creditScreen/backButtonBlack.png"), 60, 40, WHITE);
}

void Game::DrawLevelTemplate() {
    auto& rm = ResourceManager::GetInstance();
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/templateBackground.png"), 0, 0, WHITE);
}

void Game::DrawYouDied() {
    auto& rm = ResourceManager::GetInstance();
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/youDied.png"), 0, 0, WHITE);
}

}
}
