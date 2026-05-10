#pragma once

#include <string>

namespace CursedTomorrow {
namespace Config {

inline constexpr int ScreenWidth = 1422;
inline constexpr int ScreenHeight = 800;
inline constexpr int TargetFPS = 60;

inline const std::string WindowTitle = "...Cursed---Tomorrow... by B-Ted";

inline constexpr float DefaultSpeed = 5.0f;
inline constexpr float StickManHeight = 102.0f;
inline constexpr float StickManWidth = 60.0f;
inline constexpr float StickManCollisionWidth = 65.0f;
inline constexpr float Gravity = 6.0f;
inline constexpr float JumpHeight = 13.0f;
inline constexpr float JumpDelay = 28.0f;

}
}
