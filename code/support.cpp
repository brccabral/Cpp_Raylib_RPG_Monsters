#include "support.h"


std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
CoastImporter(const char *file, const int rows, const int cols)
{
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> result;
    const auto frame = rg::LoadTextureSafe(file);
    // const auto frame = rg::Frames::Load(file, rows, cols);
    const std::vector<std::string> terrains = {"grass", "grass_i", "sand_i", "sand",
                                               "rock",  "rock_i",  "ice",    "ice_i"};
    std::vector<std::pair<std::string, rg::math::Vector2>> sides = {
            {"topleft", {0, 0}}, //
            {"top", {1, 0}}, //
            {"topright", {2, 0}}, //
            {"left", {0, 1}}, //
            {"middle", {1, 1}}, //
            {"right", {2, 1}}, //
            {"bottomleft", {0, 2}}, //
            {"bottom", {1, 2}}, //
            {"bottomright", {2, 2}}, //
    };
    const auto width = frame.width / cols;
    const auto height = frame.height / rows;
    // const auto width = frame->GetRect().width;
    // const auto height = frame->GetRect().height;
    for (int index = 0; index < terrains.size(); ++index)
    {
        const auto &terrain = terrains[index];
        result[terrain] = {};
        for (const auto &[key, pos]: sides)
        {
            result[terrain][key] = std::make_shared<rg::Frames>(width * 4, height, 1, 4);
            for (int row = 0; row < rows; row += 3)
            {
                const auto x = (pos.x + index * 3);
                const auto y = (pos.y + row);
                result[terrain][key]->Blit(
                        frame, {row / 3.0f * width, 0},
                        {(float) x * width, (float) y * height, (float) width, (float) height});
            }
        }
    }
    rg::UnloadTextureSafe(frame);
    return result;
}

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
AllCharacterImport(const char *path)
{
    std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> result{};

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = CharacterImporter(4, 4, entryPath.c_str());
    }
    return result;
}

std::map<std::string, std::shared_ptr<rg::Frames>>
CharacterImporter(const int rows, const int cols, const char *file)
{
    std::map<std::string, std::shared_ptr<rg::Frames>> result{};
    const auto frame_dict = rg::Frames::Load(file, rows, cols);
    const float width = frame_dict->GetRect().width;
    const float height = frame_dict->GetRect().height;
    const std::vector<std::string> directions = {"down", "left", "right", "up"};
    for (int i = 0; i < directions.size(); ++i)
    {
        // result[directions[i]] =
        //         frame_dict->SubSurface({0, i * height, width * cols, height}, 1, cols);
        // std::string idle = directions[i] + "_idle";
        // result[idle] = frame_dict->SubSurface({0, i * height, width, height}, 1, 1);
        result[directions[i]] = frame_dict->SubFrames({0, i * height, width * cols, height});
        std::string idle = directions[i] + "_idle";
        result[idle] = frame_dict->SubFrames({0, i * height, width, height});
    }
    return result;
}

bool CheckConnections(
        const float radius, const std::shared_ptr<Entity> &entity,
        const std::shared_ptr<Entity> &target, const float tolerance)
{
    const auto relation = target->rect.center() - entity->rect.center();
    if (relation.magnitude() < radius)
    {
        // if player is facing left, player is on the right side of target,
        // and in same horizontal plane (not above or below target.y)
        if ((!strcmp(entity->facing_direction.c_str(), "left") && relation.x < 0 &&
             std::abs(relation.y) < tolerance) ||
            (!strcmp(entity->facing_direction.c_str(), "right") && relation.x > 0 &&
             std::abs(relation.y) < tolerance) ||
            (!strcmp(entity->facing_direction.c_str(), "up") && relation.y < 0 &&
             std::abs(relation.x) < tolerance) ||
            (!strcmp(entity->facing_direction.c_str(), "down") && relation.y > 0 &&
             std::abs(relation.x) < tolerance))
        {
            return true;
        };
    }
    return false;
}
