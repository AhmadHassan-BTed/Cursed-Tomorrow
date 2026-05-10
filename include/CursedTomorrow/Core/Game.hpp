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
    Level1,
    Level2,
    Level3,
    Level4,
    Level5,
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

    void DrawMainScreen();
    void DrawLevelSelect();
    void DrawCredits();
    void DrawLevelTemplate();
    void DrawYouDied();

    GameState currentState;
    Entities::Character player;
    
    float logPositionY;
    float buttonPositionY;
    bool logMove;
    bool logMoveOnlyOnce;
    int delayMeter;
    float variable;
    int commonIterator;
};

}
}
