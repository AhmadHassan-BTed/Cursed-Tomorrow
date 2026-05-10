#pragma once

#include "raylib.h"
#include <map>
#include <string>

namespace CursedTomorrow {
namespace Core {

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    Texture2D LoadTextureResource(const std::string& path);
    void UnloadTextureResource(const std::string& path);

    Music LoadMusicResource(const std::string& path);
    void UnloadMusicResource(const std::string& path);

    void UnloadAll();

private:
    ResourceManager() = default;
    ~ResourceManager();

    std::map<std::string, Texture2D> textures;
    std::map<std::string, Music> music;
};

}
}
