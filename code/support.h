#pragma once
#include <rygame.h>

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
CoastImporter(const char *file, int rows, int cols);

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
AllCharacterImport(const char *path);

std::map<std::string, std::shared_ptr<rg::Frames>>
CharacterImporter(int rows, int cols, const char *file);
