#include "CursedTomorrow/Core/ResourceManager.hpp"
#include <iostream>

namespace CursedTomorrow {
namespace Core {

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::~ResourceManager() {
    UnloadAll();
}

Texture2D ResourceManager::LoadTextureResource(const std::string& path) {
    auto it = textures.find(path);
    if (it == textures.end()) {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0) {
            std::cerr << "Resource not found: " << path << std::endl;
        }
        textures[path] = texture;
        return texture;
    }
    return it->second;
}

void ResourceManager::UnloadTextureResource(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

Music ResourceManager::LoadMusicResource(const std::string& path) {
    auto it = music.find(path);
    if (it == music.end()) {
        Music m = LoadMusicStream(path.c_str());
        music[path] = m;
        return m;
    }
    return it->second;
}

void ResourceManager::UnloadMusicResource(const std::string& path) {
    auto it = music.find(path);
    if (it != music.end()) {
        UnloadMusicStream(it->second);
        music.erase(it);
    }
}

void ResourceManager::UnloadAll() {
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();

    for (auto& pair : music) {
        UnloadMusicStream(pair.second);
    }
    music.clear();
}

}
}
