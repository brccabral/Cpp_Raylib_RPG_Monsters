#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "settings.hpp"


std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
CoastImporter(const char *file, int rows, int cols);

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
AllCharacterImport(const char *path);

std::map<std::string, std::shared_ptr<rg::Frames>>
CharacterImporter(int rows, int cols, const char *file);

bool CheckConnections(
        float radius, const std::shared_ptr<Entity> &entity, const std::shared_ptr<Entity> &target,
        float tolerance = 30);

std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
MonsterImporter(int cols, int rows, const char *path);

std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>> OutlineCreator(
        const std::map<std::string, std::map<AnimationState, std::shared_ptr<rg::Frames>>>
                &monster_frames,
        float width);

std::map<AttackAnimation, std::shared_ptr<rg::Frames>> AttackImporter(const char *path);

std::map<std::string, std::shared_ptr<rg::mixer::Sound>> AudioImporter(const char *path);
