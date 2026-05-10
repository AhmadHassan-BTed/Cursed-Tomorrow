<div align="center">

# 🕯️ Cursed Tomorrow
### A platforming descent into the atmospheric unknown.

[![CI](https://img.shields.io/github/actions/workflow/status/AhmadHassan-BTed/Cursed-Tomorrow/ci.yml?branch=main&style=flat-square&logo=github)](https://github.com/AhmadHassan-BTed/Cursed-Tomorrow/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-gray.svg?style=flat-square)](LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0--alpha-black.svg?style=flat-square)](CHANGELOG.md)
[![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg?style=flat-square&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![Raylib](https://img.shields.io/badge/Framework-Raylib-orange.svg?style=flat-square)](https://www.raylib.com/)

---

*Cursed Tomorrow* is not just a game; it is a manifestation of tension and the human struggle against an encroaching, atmospheric darkness. Designed and engineered by **Ahmad Hassan (B-Ted)**, this project serves as a technical showcase of modular C++ game design and atmospheric storytelling through interaction.

[Overview](#overview) • [Showcase](#-visual-showcase) • [Architecture](#architecture) • [Systems](#systems) • [Resources](#-academic-resources) • [Development](#development)

</div>

## 🌑 Overview

The core of *Cursed Tomorrow* lies in its simplicity and the weight of its atmosphere. Every movement, jump, and collision is tuned to resonate with the player's experience of navigating a world that feels both fragile and hostile.

### Core Pillars
- **Atmospheric Isolation**: A world designed to feel empty yet occupied.
- **Precision Mechanics**: Character movement that respects physics and player intent.
- **Modular Design**: A robust C++ foundation built for extensibility and performance.

---

## 🖼️ Visual Showcase

<div align="center">

**The World of Tomorrow**
![Main Menu](docs/assets/screenshots/main_menu.jpeg)
*The gateway into the descent.*

<br>

| Gameplay | Level Selection |
| :---: | :---: |
| ![Gameplay](docs/assets/screenshots/gameplay.png) | ![Level Select](docs/assets/screenshots/level_select.jpeg) |
| *Navigating the void.* | *Choosing the path.* |

| Atmosphere | Death |
| :---: | :---: |
| ![Warning](docs/assets/screenshots/warning_screen.png) | ![You Died](docs/assets/screenshots/death_screen.png) |
| *Signs of what's to come.* | *The inevitable end.* |

</div>

---

## 🏗️ Architecture Documentation

The project architecture prioritizes decoupling and clear ownership. It is built to scale from a simple prototype to a complex production-grade game.

### High-Level System Flow

```mermaid
graph TD
    subgraph "Engine Loop"
        Main[main.cpp] --> |Initialize| Game[Core::Game]
        Game --> |Frame Start| Input[HandleInput]
        Input --> |Update| Logic[UpdateLevelLogic]
        Logic --> |Physics| Phys[Entities::Character]
        Phys --> |Render| Draw[Draw]
        Draw --> |Frame End| Game
    end

    subgraph "Asset Management"
        Game -.-> |Request| RM[ResourceManager]
        RM --> |Cache/Load| Disk[(Disk Assets)]
    end
```

### Module Relationship

The interaction between modules is strictly managed through the `CursedTomorrow` namespace.

| Module | Responsibility | Internal Coupling |
| :--- | :--- | :--- |
| **Core** | Window lifecycle, state machine, and main loop orchestration. | High (Orchestrator) |
| **Entities** | Individual game actors, physics, and local state. | Low (Independent) |
| **Resources** | Centralized asset caching and memory safety. | Zero (Utility) |
| **Config** | Global constants and engine parameters. | Static Only |

---

## ⚙️ Internal Systems

### Request & Data Flow
The data flow follows a strict unidirectional pattern during the update phase to ensure deterministic behavior.

```mermaid
sequenceDiagram
    participant P as Player
    participant G as Game
    participant C as Character
    participant RM as ResourceManager

    G->>G: HandleInput()
    G->>C: Update()
    C->>C: Move() & Jump()
    C->>C: ApplyGravity()
    G->>G: UpdateLevelLogic()
    G->>C: Draw()
    C->>RM: LoadTextureResource()
    RM-->>C: Texture Data
    C->>G: Render Complete
```

### Repository Structure

```text
.
├── .github/              # CI/CD Workflows & Templates
├── assets/               # Game Resources (Audio, Textures)
│   ├── audio/            # Atmospheric soundscapes
│   └── mainScreen/       # Visual interface components
├── docs/                 # Detailed Technical Documentation
├── include/              # Header Files
│   └── CursedTomorrow/   # Project Namespace
│       ├── Core/         # Engine & State Logic
│       └── Entities/     # Actor definitions
├── src/                  # Implementation Files
├── scripts/              # Automation and build helpers
└── CMakeLists.txt        # Build System Configuration
```

---

## 🛠️ Development Workflow

The development process is designed to be contributor-friendly while maintaining high engineering standards.

<details>
<summary><b>1. Environment Setup</b></summary>

Ensure a C++17 compiler and CMake (3.15+) are installed. Raylib is the primary dependency and can be automatically fetched during the build process.
</details>

<details>
<summary><b>2. Build Pipeline</b></summary>

```bash
# Configure with external Raylib fetch
cmake -B build -D FETCH_RAYLIB=ON

# Build the project
cmake --build build --config Release

# Run Unit Tests
ctest --test-dir build
```
</details>

### Technical Pipeline

```mermaid
graph LR
    Code[Code Change] --> Lint[Linting Check]
    Lint --> CMake[CMake Config]
    CMake --> Build[Binary Build]
    Build --> Test[Unit Testing]
    Test --> Deploy[Release Artifact]
```

---

## 📚 Academic Resources

This project is supported by extensive documentation and presentation materials detailing its design and theoretical foundations.

| Resource | Description | Format |
| :--- | :--- | :---: |
| **SRS Document** | Software Requirements Specification and Data Structures (DSA) concepts. | [PDF](docs/resources/SRS_Document.pdf) |
| **DSA Presentation** | Technical walkthrough of the concepts implemented in Cursed Tomorrow. | [PPTX](docs/resources/DSA_Presentation.pptx) |

---

## 🌑 Credits & Vision

This project is a solo endeavor by **Ahmad Hassan (B-Ted)**. It represents a commitment to clean code, intentional design, and the pursuit of digital atmosphere.

- **Lead Engineer**: [Ahmad Hassan (B-Ted)](https://github.com/AhmadHassan-BTed)
- **Framework**: Raylib (Special thanks to the community)
- **Vision**: To create a space where code and human emotion intersect.

New people are welcomed to explore, contribute, and fork. The goal is to build a community around intentional game engineering.

---

<div align="center">
    <i>"Tomorrow is coming, but it might be cursed."</i>
</div>
