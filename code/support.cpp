#include "support.hpp"


std::map<std::string, std::map<std::string, rg::Frames_Ptr>>
CoastImporter(const char *file, const int rows, const int cols)
{
    std::map<std::string, std::map<std::string, rg::Frames_Ptr>> result;
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
    for (unsigned int index = 0; index < terrains.size(); ++index)
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

std::map<std::string, std::map<std::string, rg::Frames_Ptr>> AllCharacterImport(const char *path)
{
    std::map<std::string, std::map<std::string, rg::Frames_Ptr>> result{};

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = CharacterImporter(4, 4, entryPath.c_str());
    }
    return result;
}

std::map<std::string, rg::Frames_Ptr>
CharacterImporter(const int rows, const int cols, const char *file)
{
    std::map<std::string, rg::Frames_Ptr> result{};
    const auto frame_dict = rg::Frames::Load(file, rows, cols);
    const float width = frame_dict->GetRect().width;
    const float height = frame_dict->GetRect().height;
    const std::vector<std::string> directions = {"down", "left", "right", "up"};
    for (unsigned int i = 0; i < directions.size(); ++i)
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

std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>>
MonsterImporter(const int cols, const int rows, const char *path)
{
    std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> result{};

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        const auto monster_frames = rg::Frames::Load(entryPath.c_str(), rows, cols);
        const auto tex_width = (float) monster_frames->GetTexture().width;
        const auto tex_height = (float) monster_frames->GetTexture().height;
        result[filename][ANIMATIONSTATE_IDLE] =
                monster_frames->SubFrames({0, 0, tex_width, tex_height / 2.0f});
        result[filename][ANIMATIONSTATE_ATTACK] =
                monster_frames->SubFrames({0, tex_height / 2.0f, tex_width, tex_height / 2.0f});
    }
    return result;
}

std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> OutlineCreator(
        const std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> &monster_frames,
        const float width)
{
    std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> result;
    for (auto &[name, state_frames]: monster_frames)
    {
        result[name] = {};
        for (auto &[state, frames]: state_frames)
        {
            const auto mask_surf =
                    rg::mask::FromSurface(frames).ToFrames(frames->rows, frames->cols);
            mask_surf->SetColorKey(rl::BLACK);

            const auto new_surf = std::make_shared<rg::Frames>(
                    frames->render.texture.width, frames->render.texture.height, frames->rows,
                    frames->cols);
            new_surf->frames = frames->frames;
            new_surf->SetColorKey(rl::BLACK);

            new_surf->Blit(
                    mask_surf->render.texture, {-width, -width},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // topleft
            new_surf->Blit(
                    mask_surf->render.texture, {0, -width},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // topcenter
            new_surf->Blit(
                    mask_surf->render.texture, {width, -width},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // topright
            new_surf->Blit(
                    mask_surf->render.texture, {-width, 0},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // left
            new_surf->Blit(
                    mask_surf->render.texture, {0, 0},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // center
            new_surf->Blit(
                    mask_surf->render.texture, {width, 0},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // right
            new_surf->Blit(
                    mask_surf->render.texture, {-width, width},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // bottomleft
            new_surf->Blit(
                    mask_surf->render.texture, {0, width},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // bottomcenter
            new_surf->Blit(
                    mask_surf->render.texture, {width, width},
                    {0, 0, (float) mask_surf->render.texture.width,
                     (float) mask_surf->render.texture.height}); // bottomright
            new_surf->SetAtlas();

            result[name][state] = new_surf;
        }
    }
    return result;
}

std::map<AttackAnimation, rg::Frames_Ptr> AttackImporter(const char *path)
{
    std::map<AttackAnimation, rg::Frames_Ptr> result;
    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[ATTACK_ANIMATION_NAMES[filename]] = rg::Frames::Load(entryPath.c_str(), 1, 4);
    }
    return result;
}

std::map<std::string, std::shared_ptr<rg::mixer::Sound>> AudioImporter(const char *path)
{
    std::map<std::string, std::shared_ptr<rg::mixer::Sound>> result;

    const std::string audio_path = path;
    const std::string music_path = audio_path + "/musics";
    const std::string sounds_path = audio_path + "/sounds";

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(music_path.c_str()))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = std::make_shared<rg::mixer::Sound>(entryPath.c_str(), true);
    }
    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(sounds_path.c_str()))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        result[filename] = std::make_shared<rg::mixer::Sound>(entryPath.c_str(), false);
    }
    return result;
}
