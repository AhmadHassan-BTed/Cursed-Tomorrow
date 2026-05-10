# Deployment Guide

## Build Requirements

- **Windows**: Visual Studio 2019+ or MinGW.
- **Linux**: GCC/Clang and development libraries for X11, OpenGL, and ALSA.
- **macOS**: Xcode or Command Line Tools.

## Local Build

```bash
mkdir build
cd build
cmake .. -D FETCH_RAYLIB=ON
cmake --build . --config Release
```

## Distribution

When distributing the game, ensure the `assets` folder is located in the same directory as the executable.

### Windows
- `CursedTomorrow.exe`
- `raylib.dll` (if linked dynamically)
- `assets/`

### Linux
- `CursedTomorrow`
- `assets/`

## Continuous Integration (CI)

The project uses GitHub Actions to automate builds. Every Pull Request is verified on:
- Ubuntu (latest)
- Windows (latest)

See [.github/workflows/ci.yml](../.github/workflows/ci.yml) for the full pipeline definition.
