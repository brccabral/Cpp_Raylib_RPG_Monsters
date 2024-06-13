#pragma once
#include <cstring>
#include <filesystem>
#include <map>
#include <vector>
#include <raylib.h>
#include "raylib_utils.h"

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

inline std::map<std::string, Texture2D> ImportNamedFolder(const char *path)
{
    std::map<std::string, Texture2D> textures;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        textures[filename] = LoadTexture(dirEntry.path().c_str());
    }
    return textures;
}

typedef std::map<std::string, std::vector<Rectangle>> tilerect_name;
typedef std::map<FacingDirection, std::vector<Rectangle>> tilerect_face;

inline std::map<std::string, tilerect_name> coast_rects()
{
    std::map<std::string, tilerect_name> new_dict;
    const std::vector<std::string> terrains = {"grass", "grass_i", "sand_i", "sand", "rock", "rock_i", "ice", "ice_i"};
    const std::map<std::string, std::tuple<int, int>> sides = {
            {"topleft", {0, 0}}, //
            {"top", {1, 0}}, //
            {"topright", {2, 0}}, //
            {"left", {0, 1}}, //
            {"middle", {1, 1}}, //
            {"right", {2, 1}}, //
            {"bottomleft", {0, 2}}, //
            {"bottom", {1, 2}}, //
            {"bottomright", {2, 2}} //
    };
    for (int index = 0; index < terrains.size(); ++index)
    {
        const std::string &terrain = terrains[index];
        new_dict[terrain] = {};
        for (const auto &[key, pos]: sides)
        {
            for (int row = 0; row < 4; ++row)
            {
                // new_dict[terrain][key].push_back(frame_dict[{std::get<0>(pos) + index * 3, std::get<1>(pos) + row}]);
                new_dict[terrain][key].push_back(
                        {(std::get<0>(pos) + index * 3) * 64.0f, (std::get<1>(pos) + row * 3) * 64.0f, 64.0f, 64.0f});
            }
        }
    }

    return new_dict;
}

inline tilerect_face CharacterImporter(const int cols, const int rows, const char *path)
{
    tilerect_face new_dic = {};
    // same order as in image set
    const std::vector directions = {DOWN, LEFT, RIGHT, UP};
    const std::vector idle_directions = {DOWN_IDLE, LEFT_IDLE, RIGHT_IDLE, UP_IDLE};
    for (int row = 0; row < directions.size(); ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            new_dic[directions[row]].push_back({col * 128.0f, row * 128.0f, 128.0f, 128.0f});
            FacingDirection idle_name = idle_directions[row];
            new_dic[idle_name] = {{0, row * 128.0f, 128.0f, 128.0f}};
        }
    }
    return new_dic;
}

inline std::map<std::string, tilerect_face> all_character_import(const char *path)
{
    std::map<std::string, tilerect_face> new_dict = {};

    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        if (dirEntry.is_regular_file())
        {
            auto filename = dirEntry.path().stem().string();
            new_dict[filename] = CharacterImporter(4, 4, dirEntry.path().c_str());
        }
    }

    return new_dict;
}

inline bool CheckConnections(const float radius, const Entity *entity, const Entity *target, float tolerance = 30)
{
    const Vector2 relation = Vector2Subtract(GetRectCenter(target->rect), GetRectCenter(entity->rect));
    // player is close to target
    if (Vector2Length(relation) < radius)
    {
        // if player is facing left, player is on the right side of target,
        // and in same horizontal plane (not above or below target.y)
        if ((entity->facing_direction == LEFT || entity->facing_direction == LEFT_IDLE) && relation.x < 0 &&
            abs(relation.y) < tolerance)
        {
            return true;
        }
        if ((entity->facing_direction == RIGHT || entity->facing_direction == RIGHT_IDLE) && relation.x > 0 &&
            abs(relation.y) < tolerance)
        {
            return true;
        }
        if ((entity->facing_direction == UP || entity->facing_direction == UP_IDLE) && relation.y < 0 &&
            abs(relation.x) < tolerance)
        {
            return true;
        }
        if ((entity->facing_direction == DOWN || entity->facing_direction == DOWN_IDLE) && relation.y > 0 &&
            abs(relation.x) < tolerance)
        {
            return true;
        }
    }
    return false;
}
