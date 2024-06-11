#pragma once

#include <filesystem>
#include <iostream>
#include <raylib.h>
#include <vector>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

inline std::vector<Texture2D> ImportFolder(const char *path)
{
    std::vector<Texture2D> frames;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        frames.push_back(LoadTexture(dirEntry.path().c_str()));
    }
    return frames;
}
