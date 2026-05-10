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
    , warningScreen(true)
    , level1Start(true)
    , level5Start(true)
    , showWelcome(true)
    , alive(true)
    , delayMeter(0)
    , variable(2.2f)
    , commonIterator(0)
    , spaceLayers(0.0f)
    , pingMeter(0.0f)
    , tempPositionx(0.0f)
    , tempPositiony(0.0f)
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

    if (showWelcome) {
        delayMeter++;
        // Original: if (delayMeter >= (DEALY_SCREEN * 4) && warningScreen) { warningScreen = false; }
        if (delayMeter >= (180 * 4) && warningScreen) {
            warningScreen = false;
        }
    }

    HandleInput();

    if (currentState >= GameState::Level1 && currentState <= GameState::Level6) {
        UpdateLevelLogic();
        bool inverted = (currentState == GameState::Level2);
        player.SetPingEffect(currentState == GameState::Level6);
        player.Update(inverted);
        CheckCollisions();
    } else if (static_cast<int>(currentState) >= static_cast<int>(GameState::AfterLevel1) && static_cast<int>(currentState) <= static_cast<int>(GameState::AfterLevel5)) {
        delayMeter++;
        if (delayMeter >= 180) { // DEALY_SCREEN
            delayMeter = 0;
            if (currentState == GameState::AfterLevel1) currentState = GameState::Level2;
            else if (currentState == GameState::AfterLevel2) currentState = GameState::Level3;
            else if (currentState == GameState::AfterLevel3) currentState = GameState::Level4;
            else if (currentState == GameState::AfterLevel4) currentState = GameState::Level5;
            else if (currentState == GameState::AfterLevel5) currentState = GameState::Level6;
            ResetLevel();
        }
    }
}

void Game::UpdateLevelLogic() {
    auto& rm = ResourceManager::GetInstance();
    
    if (currentState == GameState::Level1 && level1Start) {
        logPositionY = 100.0f;
        level1Start = false;
    }
    if (currentState == GameState::Level5 && level5Start) {
        logPositionY = 100.0f;
        level5Start = false;
    }

    // Button and Log Operation
    Vector2 pos = player.GetPosition();
    Rectangle playerRect = { pos.x, pos.y, Config::StickManCollisionWidth, Config::StickManHeight };
    Rectangle buttonRect = { 667, buttonPositionY, 91 , 23 };
    
    bool touchingButton = CheckCollisionRecs(playerRect, buttonRect);

    if (touchingButton) {
        buttonPositionY += 1.6f;
        
        // Level 1: Button closes the door
        if (currentState == GameState::Level1) {
            logPositionY = 240.0f;
        }

        // Level 2, 4, 6: Button opens the door
        if (buttonPositionY > 350 && (currentState == GameState::Level2 || currentState == GameState::Level4 || currentState == GameState::Level6)) {
            logMove = true;
        }
    } else if (buttonPositionY > 381) {
        buttonPositionY -= 1.6f;
    }

    if (logMove && logMoveOnlyOnce) {
        logPositionY -= 4.0f;
        if (logPositionY <= 80) {
            logMove = false;
            logMoveOnlyOnce = false;
        }
    }

    switch (currentState) {
        case GameState::Level3:
            // Press back button level - handles mouse left click OR backspace
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_BACKSPACE)) {
                currentState = GameState::AfterLevel3;
                ResetLevel();
            }
            break;
        case GameState::Level4:
            player.SetPosition({ player.GetPosition().x - 2.0f, player.GetPosition().y });
            break;
        default:
            break;
    }

    // Goal detection
    if (player.GetPosition().x > 1350) {
        if (currentState == GameState::Level1) currentState = GameState::AfterLevel1;
        else if (currentState == GameState::Level2) currentState = GameState::AfterLevel2;
        else if (currentState == GameState::Level3) {} // Handled above
        else if (currentState == GameState::Level4) currentState = GameState::AfterLevel4;
        else if (currentState == GameState::Level5) currentState = GameState::AfterLevel5;
        else if (currentState == GameState::Level6) currentState = GameState::GameEnd;
        ResetLevel();
    }
}

void Game::CheckCollisions() {
    Vector2 pos = player.GetPosition();
    Rectangle playerRect = Rectangle{ pos.x, pos.y, Config::StickManCollisionWidth, Config::StickManHeight };

    // --- Hazard collisions (Spikes/Teeth) ---
    if (CheckCollisionRecs(playerRect, Rectangle{ 664, 112, 84, 22 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 664, 425, 84, 22 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 368, 261, 79, 22 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 982, 277, 82, 22 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 942, 41, 82, 22 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }

    if (CheckCollisionRecs(playerRect, Rectangle{ 392, 37, 25, 20 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 1356, 64, 36, 171 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 48, 120, 32, 95 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 48, 515, 30, 186 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }

    if (CheckCollisionRecs(playerRect, Rectangle{ 716, 716, 36, 36 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }
    if (CheckCollisionRecs(playerRect, Rectangle{ 847, 723, 36, 36 })) { currentState = GameState::YouDied; player.SetPosition(Vector2{ 42, 340 }); }

    // --- Environmental Collisions (Solid Geometry) ---
    
    // Top Mid Box
    if (CheckCollisionRecs(playerRect, Rectangle{ 635, 23, 150, 93 })) player.SetPosition(Vector2{ pos.x, 116 });
    if (CheckCollisionRecs(playerRect, Rectangle{ 620, 24, 13, 92 })) player.SetPosition(Vector2{ 620 - Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 788, 24, 13, 92 })) player.SetPosition(Vector2{ 745 + Config::StickManWidth, pos.y });

    // Bottom Bar (Floor)
    if (CheckCollisionRecs(playerRect, Rectangle{ 1, 745, 1421, 234 })) player.SetPosition(Vector2{ pos.x, 640 });

    // Left Tray 1 UP
    if (CheckCollisionRecs(playerRect, Rectangle{ 336, 238, 160, 8 })) player.SetPosition(Vector2{ pos.x, 238 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 326, 262, 174, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 322, 246, 14, 23 })) player.SetPosition(Vector2{ 316 - Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 488, 244, 14, 23 })) player.SetPosition(Vector2{ 444 + Config::StickManWidth, pos.y });

    // Right Tray 1 UP
    if (CheckCollisionRecs(playerRect, Rectangle{ 960, 248, 130, 8 })) player.SetPosition(Vector2{ pos.x, 248 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 959, 261, 131, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 950, 250, 7, 23 })) player.SetPosition(Vector2{ 830 + Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 1095, 250, 7, 23 })) player.SetPosition(Vector2{ 1035 + Config::StickManWidth, pos.y });

    // Left Tray 2 Down
    if (CheckCollisionRecs(playerRect, Rectangle{ 331, 515, 140, 10 })) player.SetPosition(Vector2{ pos.x, 515 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 330, 544, 150, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 322, 520, 14, 23 })) player.SetPosition(Vector2{ 316 - Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 470, 520, 14, 23 })) player.SetPosition(Vector2{ 420 + Config::StickManWidth, pos.y });

    // Right Tray 2 Down
    if (CheckCollisionRecs(playerRect, Rectangle{ 962, 527, 131, 10 })) player.SetPosition(Vector2{ pos.x, 527 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 955, 555, 150, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 952, 534, 5, 23 })) player.SetPosition(Vector2{ 830 + Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 1105, 528, 7, 23 })) player.SetPosition(Vector2{ 1040 + Config::StickManWidth, pos.y });

    // Mid Box Bottom
    if (CheckCollisionRecs(playerRect, Rectangle{ 737, 663, 120, 80 })) player.SetPosition(Vector2{ pos.x, 663 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 727, 663, 13, 80 })) player.SetPosition(Vector2{ 720 - Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 855, 663, 13, 80 })) player.SetPosition(Vector2{ 805 + Config::StickManWidth, pos.y });

    // Bottom Right Box
    if (CheckCollisionRecs(playerRect, Rectangle{ 1230, 620, 200, 50 })) player.SetPosition(Vector2{ pos.x, 620 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 1218, 623, 121, 121 })) player.SetPosition(Vector2{ 1217 - Config::StickManWidth, pos.y });

    // Trays below Door
    if (CheckCollisionRecs(playerRect, Rectangle{ 1230, 444, 210, 10 })) player.SetPosition(Vector2{ pos.x, 444 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 1220, 475, 219, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 1218, 456, 14, 23 })) player.SetPosition(Vector2{ 1218 - Config::StickManWidth, pos.y });

    if (CheckCollisionRecs(playerRect, Rectangle{ 0, 444, 210, 10 })) player.SetPosition(Vector2{ pos.x, 444 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 0, 475, 210, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 210, 468, 14, 23 })) player.SetPosition(Vector2{ 160 + Config::StickManWidth, pos.y });

    // Mid Trays
    if (CheckCollisionRecs(playerRect, Rectangle{ 556, 400, 309, 10 })) player.SetPosition(Vector2{ pos.x, 400 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 555, 423, 319, 10 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 545, 406, 14, 23 })) player.SetPosition(Vector2{ 540 - Config::StickManWidth, pos.y });
    if (CheckCollisionRecs(playerRect, Rectangle{ 858, 406, 14, 23 })) player.SetPosition(Vector2{ 810 + Config::StickManWidth, pos.y });

    // Top Trays
    if (CheckCollisionRecs(playerRect, Rectangle{ 0, 232, 210, 10 })) player.SetPosition(Vector2{ pos.x, 232 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 0, 262, 219, 11 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 210, 238, 14, 23 })) player.SetPosition(Vector2{ 160 + Config::StickManWidth, pos.y });

    if (CheckCollisionRecs(playerRect, Rectangle{ 1228, 232, 210, 10 })) player.SetPosition(Vector2{ pos.x, 232 - Config::StickManHeight });
    if (CheckCollisionRecs(playerRect, Rectangle{ 1218, 262, 219, 11 })) player.StopJumping();
    if (CheckCollisionRecs(playerRect, Rectangle{ 1216, 238, 14, 23 })) player.SetPosition(Vector2{ 1218 - Config::StickManWidth, pos.y });

    // Log/Door (Solid geometry)
    if (CheckCollisionRecs(playerRect, Rectangle{ 1220, logPositionY, 34, 208 })) {
        player.SetPosition(Vector2{ 1220 - Config::StickManWidth, pos.y });
    }
}

void Game::ResetLevel() {
    player.ResetPosition();
    logPositionY = 250.0f;
    buttonPositionY = 380.0f;
    logMove = false;
    logMoveOnlyOnce = true;
    level1Start = true;
    level5Start = true;
    spaceLayers = 0.0f;
    alive = true;
}

void Game::HandleInput() {
    Vector2 mousePos = GetMousePosition();

    if (currentState == GameState::MainScreen) {
        if (CheckCollisionPointRec(mousePos, { 170, 80, 216, 34 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::Level1;
            ResetLevel();
        }
        if (CheckCollisionPointRec(mousePos, { 210, 160, 118, 33 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::LevelSelect;
        }
        if (CheckCollisionPointRec(mousePos, { 195, 240, 150, 34 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::Credits;
        }
    } else if (currentState == GameState::LevelSelect) {
        if (CheckCollisionPointRec(mousePos, { 200, 200, 217, 211 })) { if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { currentState = GameState::Level1; ResetLevel(); } }
        if (CheckCollisionPointRec(mousePos, { 600, 200, 217, 211 })) { if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { currentState = GameState::Level2; ResetLevel(); } }
        if (CheckCollisionPointRec(mousePos, { 1000, 200, 217, 211 })) { if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { currentState = GameState::Level3; ResetLevel(); } }
        if (CheckCollisionPointRec(mousePos, { 200, 500, 217, 211 })) { if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { currentState = GameState::Level4; ResetLevel(); } }
        if (CheckCollisionPointRec(mousePos, { 600, 500, 217, 211 })) { if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { currentState = GameState::Level5; ResetLevel(); } }
        if (CheckCollisionPointRec(mousePos, { 1000, 500, 217, 211 })) { if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { currentState = GameState::Level6; ResetLevel(); } }

        if (CheckCollisionPointRec(mousePos, { 60, 40, 96, 55 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::MainScreen;
            showWelcome = true;
        }
    } else if (currentState == GameState::Credits) {
        if (CheckCollisionPointRec(mousePos, { 60, 40, 96, 55 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GameState::MainScreen;
            showWelcome = true;
        }
    } else if (currentState >= GameState::Level1 && currentState <= GameState::Level6 || currentState == GameState::YouDied) {
        // Universal back button/backspace for levels and death screen
        if ((CheckCollisionPointRec(mousePos, { 660, 32, 96, 55 }) || IsKeyPressed(KEY_BACKSPACE))) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_BACKSPACE)) {
                currentState = GameState::MainScreen;
                showWelcome = true;
                ResetLevel();
            }
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
        case GameState::AfterLevel1: DrawTexture(rm.LoadTextureResource("assets/levelSelector/afterLevel1.png"), 0, 0, WHITE); break;
        case GameState::AfterLevel2: DrawTexture(rm.LoadTextureResource("assets/levelSelector/afterLevel2.png"), 0, 0, WHITE); break;
        case GameState::AfterLevel3: DrawTexture(rm.LoadTextureResource("assets/levelSelector/afterLevel3.png"), 0, 0, WHITE); break;
        case GameState::AfterLevel4: DrawTexture(rm.LoadTextureResource("assets/levelSelector/afterLevel4.png"), 0, 0, WHITE); break;
        case GameState::AfterLevel5: DrawTexture(rm.LoadTextureResource("assets/levelSelector/afterLevel5.png"), 0, 0, WHITE); break;
        case GameState::GameEnd: DrawGameEnd(); break;
        case GameState::YouDied: DrawYouDied(); break;
        default: break;
    }

    EndDrawing();
}

// ... (other draw methods)

void Game::DrawGameEnd() {
    auto& rm = ResourceManager::GetInstance();
    spaceLayers += 0.4f;

    if (spaceLayers <= 80) {
        DrawTexture(rm.LoadTextureResource("assets/levelSelector/afterLevel6.png"), 0, 0, WHITE);
    } else if (spaceLayers <= 140) {
        DrawTexture(rm.LoadTextureResource("assets/levelSelector/1.png"), 0, 0, WHITE);
    } else if (spaceLayers <= 180) {
        DrawTexture(rm.LoadTextureResource("assets/levelSelector/2.png"), 0, 0, WHITE);
    } else if (spaceLayers <= 240) {
        DrawTexture(rm.LoadTextureResource("assets/levelSelector/3.png"), 0, 0, WHITE);
    } else if (spaceLayers <= 300) {
        DrawTexture(rm.LoadTextureResource("assets/levelSelector/behindYou.png"), 0, 0, WHITE);
    } else if (spaceLayers <= 310) {
        DrawTexture(rm.LoadTextureResource("assets/levelSelector/escape.png"), 0, 0, WHITE);
    }
}

void Game::DrawMainScreen() {
    auto& rm = ResourceManager::GetInstance();
    
    if (warningScreen) {
        if (delayMeter <= 180 * 2) {
            DrawTexture(rm.LoadTextureResource("assets/levelSelector/warning.png"), 0, 0, WHITE);
        }
        
        if (delayMeter < 180 * 4) {
            DrawTexture(rm.LoadTextureResource("assets/levelSelector/quote.png"), 0, 0, WHITE);
        }
        return;
    }

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
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/box2.png"), 600, 200, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/box3.png"), 1000, 200, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/box4.png"), 200, 500, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/box5.png"), 600, 500, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelSelector/box6.png"), 1000, 500, WHITE);
    
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

    // Hazard elements (Teeth/Spikes)
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/threeTeethDown.png"), 655, 116, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/threeTeethDown.png"), 655, 425, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/threeTeethDown.png"), 355, 261, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/threeTeethDown.png"), 973, 277, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/threeTeethDown.png"), 933, 41, WHITE);

    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/teethDown.png"), 386, 41, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/fiveTeethLeft.png"), 1344, 61, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/threeTeethRight.png"), 53, 110, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/sixTeethRight.png"), 54, 500, WHITE);

    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/teethUp.png"), 692, 703, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/teethUp.png"), 867, 703, WHITE);

    // Environment Structures
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/topBar.png"), 5, -20, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/topBox.png"), 620, 25, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/barUp.png"), 0, 0, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/barUp.png"), 1378, 0, WHITE);
    
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/longGrass.png"), 43, 195, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/longGrass.png"), 1220, 195, WHITE);
    
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/tray.png"), 0, 232, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/tray.png"), 1220, 232, WHITE);

    // Level 5 "no eyes" trick: Draw at 250 even if collision moves
    if (currentState != GameState::Level5) {
        DrawTexture(rm.LoadTextureResource("assets/levelTemplate/log.png"), 1220, (int)logPositionY, WHITE);
    } else {
        DrawTexture(rm.LoadTextureResource("assets/levelTemplate/log.png"), 1220, 250, WHITE);
    }

    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/trayL1.png"), 320, 233, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/trayR1.png"), 945, 239, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/trayL2.png"), 320, 510, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/trayR2.png"), 948, 520, WHITE);

    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/button.png"), 655, (int)buttonPositionY, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/midTray2.png"), 640, 390, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/midTray1.png"), 550, 400, WHITE);
    
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/tray.png"), 0, 444, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/tray.png"), 1220, 444, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/barDown.png"), 0, 444, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/barDown.png"), 1378, 444, WHITE);

    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/bottom.png"), 1, 625, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/bBox.png"), 730, 660, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/boxRB.png"), 1220, 620, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/longGrass.png"), 1220, 585, WHITE);
    
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/backButton.png"), 660, 32, WHITE);
}

void Game::DrawYouDied() {
    auto& rm = ResourceManager::GetInstance();
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/youDied.png"), 0, 0, WHITE);
    DrawTexture(rm.LoadTextureResource("assets/levelTemplate/backButton.png"), 660, 32, WHITE);
}

}
}
