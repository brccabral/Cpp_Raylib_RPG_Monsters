#include "support.hpp"


std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>>
CoastImporter(const char *file, const int rows, const int cols)
{
    std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>> result;
    result["cost"]["all"] = rg::Frames::Load(file, rows, cols);
    const std::vector<std::string> terrains = {"grass", "grass_i", "sand_i", "sand",
                                               "rock", "rock_i", "ice", "ice_i"};
    std::vector<std::pair<std::string, rg::math::Vector2<float>>> sides = {
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
    const auto tex_width = (float) result["cost"]["all"].GetTexture().width;
    const auto tex_height = (float) result["cost"]["all"].GetTexture().height;
    auto width = tex_width / cols;
    auto height = tex_height / rows;
    for (unsigned int index = 0; index < terrains.size(); ++index)
    {
        const auto &terrain = terrains[index];
        for (const auto &[side, pos]: sides)
        {
            result[terrain][side] = result["cost"]["all"].SubFrames(
                    {0.0f, 0.0f, tex_width, tex_height});
            std::vector<rg::Rect> frames{};
            frames.reserve(4);
            for (int row = 0; row < rows; row += 3)
            {
                const auto x = pos.x + index * 3;
                const auto y = pos.y + row;
                frames.emplace_back(x * width, y * height, width, height);
            }
            result[terrain][side].frames = frames;
        }
    }
    return result;
}

std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>> AllCharacterImport(
        const char *path)
{
    std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>> result{};

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = CharacterImporter(4, 4, entryPath.c_str());
    }
    return result;
}

std::unordered_map<std::string, rg::Frames>
CharacterImporter(const int rows, const int cols, const char *file)
{
    std::unordered_map<std::string, rg::Frames> result{};
    // we need to save the original load so that the render/texture is not unloaded
    // as subframes uses the same render/texture
    result["load"] = rg::Frames::Load(file, rows, cols);
    const float width = result["load"].GetRect().width;
    const float height = result["load"].GetRect().height;
    const std::vector<std::string> directions = {"down", "left", "right", "up"};
    for (unsigned int i = 0; i < directions.size(); ++i)
    {
        result[directions[i]] = result["load"].SubFrames({0.0f, i * height, width * cols, height});
        std::string idle = directions[i] + "_idle";
        result[idle] = result["load"].SubFrames({0.0f, i * height, width, height});
    }
    return result;
}

bool CheckConnections(
        const float radius, const Entity *entity,
        const Entity *target, const float tolerance)
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
        }
    }
    return false;
}

std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>>
MonsterImporter(const int cols, const int rows, const char *path)
{
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> result{};

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        // we need to save the original load so that the render/texture is not unloaded
        // as subframes uses the same render/texture
        result[filename + "_load"][ANIMATIONSTATE_IDLE] = rg::Frames::Load(
                entryPath.c_str(), rows, cols);
        const auto tex_width = (float) result[filename + "_load"][ANIMATIONSTATE_IDLE].GetTexture().
                width;
        const auto tex_height = (float) result[filename + "_load"][ANIMATIONSTATE_IDLE].GetTexture()
                .height;
        result[filename][ANIMATIONSTATE_IDLE] =
                result[filename + "_load"][ANIMATIONSTATE_IDLE].SubFrames(
                        {0.0f, 0.0f, tex_width, tex_height / 2.0f});
        result[filename][ANIMATIONSTATE_ATTACK] =
                result[filename + "_load"][ANIMATIONSTATE_IDLE].SubFrames(
                        {0.0f, tex_height / 2.0f, tex_width, tex_height / 2.0f});
    }
    return result;
}

std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> OutlineCreator(
        const std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> &
        monster_frames,
        float width)
{
    std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> result;
    for (auto &[name, state_frames]: monster_frames)
    {
        if (name.find("load") != std::string::npos)
        {
            continue;
        }
        for (auto &[state, frames]: state_frames)
        {
            auto mask_surf =
                    rg::mask::FromSurface(&frames).ToFrames(frames.m_rows, frames.m_cols);
            mask_surf.SetColorKey(rl::BLACK);

            auto new_surf = rg::Frames(
                    frames.GetTexture().width, frames.GetTexture().height, frames.m_rows,
                    frames.m_cols);
            new_surf.SetColorKey(rl::BLACK);
            new_surf.frames = frames.frames;

            // we need to blit the whole texture, not just the current frame
            rg::Rect area = {0.0f, 0.0f, (float) mask_surf.GetTexture().width,
                             -(float) mask_surf.GetTexture().height};

            // topleft
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{-width, -width}, area);
            // topcenter
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{0.0f, -width}, area);
            // topright
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{width, -width}, area);
            // left
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{-width, 0.0f}, area);
            // center
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{0.0f, 0.0f}, area);
            // right
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{width, 0.0f}, area);
            // bottomleft
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{-width, width}, area);
            // bottomcenter
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{0.0f, width}, area);
            // bottomright
            new_surf.Blit(mask_surf.GetTexture(), rg::math::Vector2{width, width}, area);
            new_surf.Draw();
            new_surf.SetAtlas();

            result[name][state] = std::move(new_surf);
        }
    }
    return result;
}

std::unordered_map<AttackAnimation, rg::Frames> AttackImporter(const char *path)
{
    std::unordered_map<AttackAnimation, rg::Frames> result;
    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[GameData::GetInstance().ATTACK_ANIMATION_NAMES[filename]] = rg::Frames::Load(
                entryPath.c_str(), 1, 4);
    }
    return result;
}

std::unordered_map<std::string, rg::mixer::Sound> AudioImporter(const char *path)
{
    std::unordered_map<std::string, rg::mixer::Sound> result;

    const std::string audio_path = path;
    const std::string music_path = audio_path + "/musics";
    const std::string sounds_path = audio_path + "/sounds";

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(music_path.c_str()))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = rg::mixer::Sound(entryPath.c_str(), true);
    }
    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(sounds_path.c_str()))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = rg::mixer::Sound(entryPath.c_str(), false);
    }
    return result;
}
