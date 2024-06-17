#pragma once
#include "settings.h"
#include <string>
#include <vector>


struct TrainerDialog
{
    std::vector<std::string> default_;
    std::vector<std::string> defeated;
};

struct TrainerMonster
{
    std::string name;
    int level;
};

struct CharacterData
{
    std::vector<TrainerMonster> monsters;
    TrainerDialog dialog;
    std::vector<FacingDirection> directions;
    bool look_around;
    bool defeated;
    std::string biome;
    int radius;
};

inline CharacterData o1 = {
        {{"Jacana", 14}, {"Cleaf", 15}}, //
        {{"Hey, how are you?", "Oh, so you want to fight?", "FIGHT!"},
         {"You are very strong!", "Let's fight again sometime?"}}, //
        {DOWN},
        true,
        false,
        "forest",
        0};

inline CharacterData o2 = {
        {{"Atrox", 14}, {"Pouch", 15}, {"Draem", 13}, {"Cindrill", 13}}, //
        {{"I don't like sand", "It's coarse and rough", "oh god, fight"}, {"May the force be with you"}}, //
        {LEFT, DOWN},
        false,
        false,
        "sand",
        0};

inline CharacterData o3 = {
        {{"Atrox", 14}, {"Pouch", 15}, {"Draem", 13}, {"Cindrill", 13}}, //
        {{"I love skating!", "FIGHT!"}, {"Good luck with the boss", "It's so cold in here"}}, //
        {LEFT, RIGHT, UP, DOWN},
        true,
        false,
        "sand",
        0};

inline CharacterData o4 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"I love skating!", "FIGHT!"}, {"Good luck with the boss", "It's so cold in here"}}, //
        {RIGHT},
        true,
        false,
        "forest",
        0};

inline CharacterData o5 = {
        {{"Plumette", 20}, {"Ivieron", 22}, {"Atrox", 24}, {"Pouch", 19}}, //
        {{"So you want to challenge the big ones", "This will be fun!"},
         {"I hope the lawyers will never spot you", "<3"}}, //
        {UP, RIGHT},
        true,
        false,
        "forest",
        0};

inline CharacterData o6 = {
        {{"Finsta", 15}, {"Finsta", 15}, {"Finsta", 15}}, //
        {{"I love skating!", "FIGHT!"}, {"Good luck with the boss", "It's so cold in here"}}, //
        {DOWN},
        false,
        false,
        "ice",
        0};

inline CharacterData o7 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"There are no bugs in the snow!"}, {"Maybe I should check a vulcano...", "It's so cold in here"}}, //
        {RIGHT},
        false,
        false,
        "ice",
        0};

inline CharacterData p1 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        false,
        false,
        "forest",
        0};

inline CharacterData p2 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        false,
        false,
        "forest",
        0};

inline CharacterData p3 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        false,
        false,
        "forest",
        0};

inline CharacterData p4 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        false,
        false,
        "forest",
        0};

inline CharacterData px = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        false,
        false,
        "forest",
        0};

inline CharacterData w1 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"}, {"Good luck with the boss!"}}, //
        {LEFT},
        true,
        false,
        "ice",
        0};

inline CharacterData w2 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        true,
        false,
        "ice",
        0};

inline CharacterData w3 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        true,
        false,
        "ice",
        0};

inline CharacterData w4 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"}, {"Good luck with the boss!"}}, //
        {LEFT},
        true,
        false,
        "ice",
        0};

inline CharacterData w5 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"}, {"Good luck with the boss!"}}, //
        {RIGHT},
        true,
        false,
        "ice",
        0};

inline CharacterData wx = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"I hope you brought rations", "This will be a long journey"}, {"Congratultion!"}}, //
        {DOWN},
        false,
        false,
        "ice",
        0};

inline CharacterData f1 = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
        {RIGHT},
        true,
        false,
        "sand",
        0};

inline CharacterData f2 = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
        {RIGHT, LEFT},
        false,
        false,
        "sand",
        0};

inline CharacterData f3 = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
        {RIGHT, LEFT},
        true,
        false,
        "sand",
        0};

inline CharacterData f4 = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
        {UP, RIGHT},
        true,
        false,
        "sand",
        0};

inline CharacterData f5 = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
        {LEFT},
        true,
        false,
        "sand",
        0};

inline CharacterData f6 = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
        {RIGHT},
        true,
        false,
        "sand",
        0};

inline CharacterData fx = {
        {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
        {{"Time to bring the heat", "fight!"}, {"Congratultion!"}}, //
        {DOWN},
        false,
        false,
        "sand",
        0};

inline CharacterData Nurse = {
        {}, //
        {{"Welcome to the hospital", "Your monsters have been healed"}, {}}, //
        {DOWN},
        false,
        false,
        "None",
        0};


inline std::map<std::string, CharacterData> TRAINER_DATA = {
        {"o1", o1}, //
        {"o2", o2}, //
        {"o3", o3}, //
        {"o4", o4}, //
        {"o5", o5}, //
        {"o6", o6}, //
        {"o7", o7}, //
        {"p1", p1}, //
        {"p2", p2}, //
        {"p3", p3}, //
        {"p4", p4}, //
        {"px", px}, //
        {"w1", w1}, //
        {"w2", w2}, //
        {"w3", w3}, //
        {"w4", w4}, //
        {"w5", w5}, //
        {"wx", wx}, //
        {"f1", f1}, //
        {"f2", f2}, //
        {"f3", f3}, //
        {"f4", f4}, //
        {"f5", f5}, //
        {"f6", f6}, //
        {"fx", fx}, //
        {"Nurse", Nurse}, //
};

struct MonsterData
{
    std::string name;
    std::string element;
    std::map<std::string, int> stats;
    std::map<int, std::string> abilities;
    std::pair<std::string, int> evolve;
};

inline MonsterData Plumette = {
        "Plumette",
        "plant",
        {
                {"max_health", 15}, //
                {"max_energy", 17}, //
                {"attack", 4}, //
                {"defense", 8}, //
                {"recovery", 1}, //
                {"speed", 1}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}},
        {"Ivieron", 15} //
};
inline MonsterData Ivieron = {
        "Ivieron",
        "plant",
        {
                {"max_health", 18}, //
                {"max_energy", 20}, //
                {"attack", 5}, //
                {"defense", 10}, //
                {"recovery", 1.2}, //
                {"speed", 1.2}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}},
        {"Pluma", 32} //
};
inline MonsterData Pluma = {
        "Pluma",
        "plant",
        {
                {"max_health", 23}, //
                {"max_energy", 26}, //
                {"attack", 6}, //
                {"defense", 12}, //
                {"recovery", 1.8}, //
                {"speed", 1.8}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}},
        {} //
};
inline MonsterData Sparchu = {
        "Sparchu",
        "fire",
        {
                {"max_health", 15}, //
                {"max_energy", 7}, //
                {"attack", 3}, //
                {"defense", 8}, //
                {"recovery", 1.1}, //
                {"speed", 1}, //
        },
        {{0, "scratch"}, //
         {5, "fire"}, //
         {15, "battlecry"}, //
         {26, "explosion"}},
        {"Cindrill", 15} //
};
inline MonsterData Cindrill = {
        "Cindrill",
        "fire",
        {
                {"max_health", 18}, //
                {"max_energy", 10}, //
                {"attack", 3.5}, //
                {"defense", 10}, //
                {"recovery", 1.2}, //
                {"speed", 1.1}, //
        },
        {{0, "scratch"}, //
         {5, "fire"}, //
         {15, "battlecry"}, //
         {26, "explosion"}},
        {"Charmadillo", 33} //
};
inline MonsterData Charmadillo = {
        "Charmadillo",
        "fire",
        {
                {"max_health", 29}, //
                {"max_energy", 12}, //
                {"attack", 4}, //
                {"defense", 17}, //
                {"recovery", 1.35}, //
                {"speed", 1.1}, //
        },
        {{0, "scratch"}, //
         {5, "fire"}, //
         {15, "battlecry"}, //
         {26, "explosion"}, //
         {45, "annihilate"}},
        {} //
};
inline MonsterData Finsta = {
        "Finsta",
        "water",
        {
                {"max_health", 13}, //
                {"max_energy", 17}, //
                {"attack", 2}, //
                {"defense", 8}, //
                {"recovery", 1.5}, //
                {"speed", 1.8}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {15, "splash"}, //
         {20, "ice"}, //
         {25, "heal"}},
        {"Gulfin", 34} //
};
inline MonsterData Gulfin = {
        "Gulfin",
        "water",
        {
                {"max_health", 18}, //
                {"max_energy", 20}, //
                {"attack", 3}, //
                {"defense", 10}, //
                {"recovery", 1.8}, //
                {"speed", 2}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {15, "splash"}, //
         {20, "ice"}, //
         {25, "heal"}},
        {"Finiette", 45} //
};
inline MonsterData Finiette = {
        "Finiette",
        "water",
        {
                {"max_health", 27}, //
                {"max_energy", 23}, //
                {"attack", 4}, //
                {"defense", 17}, //
                {"recovery", 2}, //
                {"speed", 2.5}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {15, "splash"}, //
         {20, "ice"}, //
         {25, "heal"}},
        {} //
};
inline MonsterData Atrox = {
        "Atrox",
        "fire",
        {
                {"max_health", 18}, //
                {"max_energy", 20}, //
                {"attack", 3}, //
                {"defense", 10}, //
                {"recovery", 1.3}, //
                {"speed", 1.9}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {30, "fire"}},
        {} //
};
inline MonsterData Pouch = {
        "Pouch",
        "plant",
        {
                {"max_health", 23}, //
                {"max_energy", 25}, //
                {"attack", 4}, //
                {"defense", 12}, //
                {"recovery", 1}, //
                {"speed", 1.5}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {25, "heal"}},
        {} //
};
inline MonsterData Draem = {
        "Draem",
        "plant",
        {
                {"max_health", 23}, //
                {"max_energy", 25}, //
                {"attack", 4}, //
                {"defense", 12}, //
                {"recovery", 1.2}, //
                {"speed", 1.4}, //
        },
        {{0, "scratch"}, //
         {5, "heal"}, //
         {20, "explosion"}, //
         {25, "splash"}},
        {} //
};
inline MonsterData Larvea = {
        "Larvea",
        "plant",
        {
                {"max_health", 15}, //
                {"max_energy", 17}, //
                {"attack", 1}, //
                {"defense", 8}, //
                {"recovery", 1}, //
                {"speed", 1}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}},
        {"Cleaf", 4} //
};
inline MonsterData Cleaf = {
        "Cleaf",
        "plant",
        {
                {"max_health", 18}, //
                {"max_energy", 20}, //
                {"attack", 3}, //
                {"defense", 10}, //
                {"recovery", 1.7}, //
                {"speed", 1.6}, //
        },
        {{0, "scratch"}, //
         {5, "heal"}},
        {} //
};
inline MonsterData Jacana = {
        "Jacana",
        "fire",
        {
                {"max_health", 12}, //
                {"max_energy", 19}, //
                {"attack", 3}, //
                {"defense", 10}, //
                {"recovery", 2.1}, //
                {"speed", 2.6}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {15, "burn"}, //
         {20, "explosion"}, //
         {25, "heal"}},
        {} //
};
inline MonsterData Friolera = {
        "Friolera",
        "water",
        {
                {"max_health", 13}, //
                {"max_energy", 20}, //
                {"attack", 4}, //
                {"defense", 6}, //
                {"recovery", 1.3}, //
                {"speed", 2}, //
        },
        {{0, "scratch"}, //
         {5, "spark"}, //
         {15, "splash"}, //
         {20, "ice"}, //
         {25, "heal"}},
        {} //
};

inline std::map<std::string, MonsterData> MONSTER_DATA = {
        {"Plumette", Plumette}, //
        {"Ivieron", Ivieron}, //
        {"Pluma", Pluma}, //
        {"Sparchu", Sparchu}, //
        {"Cindrill", Cindrill}, //
        {"Charmadillo", Charmadillo}, //
        {"Finsta", Finsta}, //
        {"Gulfin", Gulfin}, //
        {"Finiette", Finiette}, //
        {"Atrox", Atrox}, //
        {"Pouch", Pouch}, //
        {"Draem", Draem}, //
        {"Larvea", Larvea}, //
        {"Cleaf", Cleaf}, //
        {"Jacana", Jacana}, //
        {"Friolera", Friolera}, //
};
