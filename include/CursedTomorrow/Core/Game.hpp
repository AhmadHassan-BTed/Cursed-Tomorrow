#pragma once

#include "raylib.h"
#include "CursedTomorrow/Entities/Character.hpp"
#include <memory>

namespace CursedTomorrow {
namespace Core {

enum class GameState {
    MainScreen,
    LevelSelect,
    Credits,
    Level1, AfterLevel1,
    Level2, AfterLevel2,
    Level3, AfterLevel3,
    Level4, AfterLevel4,
    Level5, AfterLevel5,
    Level6,
    GameEnd,
    YouDied
};

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    void Initialize();
    void Update();
    void UpdateLevelLogic();
    void Draw();
    void HandleInput();
    void CheckCollisions();
    void ResetLevel();

    void DrawMainScreen();
    void DrawLevelSelect();
    void DrawCredits();
    void DrawLevelTemplate();
    void DrawYouDied();
    void DrawGameEnd();

    GameState currentState;
    Entities::Character player;
    
    float logPositionY;
    float buttonPositionY;
    bool logMove;
    bool logMoveOnlyOnce;
    bool warningScreen;
    bool level1Start;
    bool level5Start;
    bool showWelcome;
    bool alive;
    int delayMeter;
    float variable;
    int commonIterator;
    float spaceLayers;
    float pingMeter;
    float tempPositionx;
    float tempPositiony;
};

}
}
