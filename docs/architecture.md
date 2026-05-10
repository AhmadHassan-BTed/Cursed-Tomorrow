# Architecture Documentation

## Overview
The architecture of *Cursed Tomorrow* is built on the principles of modularity and clear state separation. The engine is divided into three primary layers: the **Core Orchestrator**, **Entity Layer**, and **Resource Management**.

## Core Components

### 1. Game Orchestrator (`Core::Game`)
The `Game` class acts as the central nervous system of the application. It is responsible for the window lifecycle, audio device management, and the high-level state machine that transitions between levels and menus.

### 2. Entity Management (`Entities::Character`)
Entities are self-contained objects that manage their own physics and local state. The `Character` class encapsulates movement logic, jumping mechanics, and gravity application, ensuring that the core game loop remains clean and high-level.

### 3. Resource Management (`Core::ResourceManager`)
Asset loading is centralized through a singleton `ResourceManager`. This ensures that textures and audio streams are only loaded into memory once, regardless of how many times they are requested. This approach prevents memory leaks and optimizes startup time.

## Design Philosophy
- **Minimalism**: Code is written to be as expressive as possible with minimal boilerplate.
- **Predictability**: Unidirectional data flow from input to physics to rendering.
- **Authorship**: This architectural design is implemented by **Ahmad Hassan (B-Ted)** to demonstrate modern C++ practices in game development.
