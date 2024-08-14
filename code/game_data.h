#pragma once
#include <string>
#include <vector>
#include "settings.h"

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
        {{"I don't like sand", "It's coarse and rough", "oh god, fight"},
         {"May the force be with you"}}, //
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
        "sand",
        0};

inline CharacterData o7 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
        {{"There are no bugs in the snow!"},
         {"Maybe I should check a vulcano...", "It's so cold in here"}}, //
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
        {{"It's so cold in here", "maybe a fight will warm me up"},
         {"Good luck with the boss!"}}, //
        {LEFT},
        true,
        false,
        "ice",
        0};

inline CharacterData w2 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"},
         {"Good luck with the boss!"}}, //
        {RIGHT},
        true,
        false,
        "ice",
        0};

inline CharacterData w3 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"},
         {"Good luck with the boss!"}}, //
        {RIGHT},
        true,
        false,
        "ice",
        0};

inline CharacterData w4 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"},
         {"Good luck with the boss!"}}, //
        {LEFT},
        true,
        false,
        "ice",
        0};

inline CharacterData w5 = {
        {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
        {{"It's so cold in here", "maybe a fight will warm me up"},
         {"Good luck with the boss!"}}, //
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


static std::map<std::string, CharacterData> TRAINER_DATA = {
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

enum Attack
{
    ATTACK_NONE = 0,
    ATTACK_BURN,
    ATTACK_HEAL,
    ATTACK_BATTLECRY,
    ATTACK_SPARK,
    ATTACK_SCRATCH,
    ATTACK_SPLASH,
    ATTACK_FIRE,
    ATTACK_EXPLOSTION,
    ATTACK_ANNIHILATE,
    ATTACK_ICE
};

enum AttackAnimation
{
    ANIMATION_FIRE = 0,
    ANIMATION_GREEN,
    ANIMATION_SCRATCH,
    ANIMATION_SPLASH,
    ANIMATION_EXPLOSION,
    ANIMATION_ICE
};

enum ElementType
{
    ELEMENT_FIRE = 0,
    ELEMENT_PLANT,
    ELEMENT_WATER,
    ELEMENT_NORMAL
};

static std::map<ElementType, std::string> NAMES_ELEMENT_TYPES{
        {ELEMENT_FIRE, "fire"},
        {ELEMENT_PLANT, "plant"},
        {ELEMENT_WATER, "water"},
        {ELEMENT_NORMAL, "normal"}};

static std::map<std::string, AttackAnimation> ATTACK_ANIMATION_NAMES = {
        {"fire", ANIMATION_FIRE},           {"green", ANIMATION_GREEN},
        {"scratch", ANIMATION_SCRATCH},     {"splash", ANIMATION_SPLASH},
        {"explosion", ANIMATION_EXPLOSION}, {"ice", ANIMATION_ICE},
};

static std::map<AttackAnimation, std::string> NAMES_ATTACK_ANIMATION = {
        {ANIMATION_FIRE, "fire"},           {ANIMATION_GREEN, "green"},
        {ANIMATION_SCRATCH, "scratch"},     {ANIMATION_SPLASH, "splash"},
        {ANIMATION_EXPLOSION, "explosion"}, {ANIMATION_ICE, "ice"},
};

struct MonsterData
{
    std::string name;
    ElementType element;
    std::map<std::string, float> stats;
    std::vector<std::pair<int, Attack>> abilities;
    std::pair<std::string, int> evolve;
};

inline MonsterData Plumette = {
        "Plumette",
        ELEMENT_PLANT,
        {
                {"max_health", 15.0f}, //
                {"max_energy", 17.0f}, //
                {"attack", 4.0f}, //
                {"defense", 8.0f}, //
                {"recovery", 1.0f}, //
                {"speed", 1.0f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}},
        {"Ivieron", 15} //
};
inline MonsterData Ivieron = {
        "Ivieron",
        ELEMENT_PLANT,
        {
                {"max_health", 18.0f}, //
                {"max_energy", 20.0f}, //
                {"attack", 5.0f}, //
                {"defense", 10.0f}, //
                {"recovery", 1.2f}, //
                {"speed", 1.2f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}},
        {"Pluma", 32} //
};
inline MonsterData Pluma = {
        "Pluma",
        ELEMENT_PLANT,
        {
                {"max_health", 23.0f}, //
                {"max_energy", 26.0f}, //
                {"attack", 6.0f}, //
                {"defense", 12.0f}, //
                {"recovery", 1.8f}, //
                {"speed", 1.8f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}},
        {} //
};
inline MonsterData Sparchu = {
        "Sparchu",
        ELEMENT_FIRE,
        {
                {"max_health", 15.0f}, //
                {"max_energy", 7.0f}, //
                {"attack", 3.0f}, //
                {"defense", 8.0f}, //
                {"recovery", 1.1f}, //
                {"speed", 1.0f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_FIRE}, //
         {15, ATTACK_BATTLECRY}, //
         {26, ATTACK_EXPLOSTION}},
        {"Cindrill", 15} //
};
inline MonsterData Cindrill = {
        "Cindrill",
        ELEMENT_FIRE,
        {
                {"max_health", 18.0f}, //
                {"max_energy", 10.0f}, //
                {"attack", 3.5f}, //
                {"defense", 10.0f}, //
                {"recovery", 1.2f}, //
                {"speed", 1.1f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_FIRE}, //
         {15, ATTACK_BATTLECRY}, //
         {26, ATTACK_EXPLOSTION}},
        {"Charmadillo", 33} //
};
inline MonsterData Charmadillo = {
        "Charmadillo",
        ELEMENT_FIRE,
        {
                {"max_health", 29.0f}, //
                {"max_energy", 12.0f}, //
                {"attack", 4.0f}, //
                {"defense", 17.0f}, //
                {"recovery", 1.35f}, //
                {"speed", 1.1f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_FIRE}, //
         {15, ATTACK_BATTLECRY}, //
         {26, ATTACK_EXPLOSTION}, //
         {45, ATTACK_ANNIHILATE}},
        {} //
};
inline MonsterData Finsta = {
        "Finsta",
        ELEMENT_FIRE,
        {
                {"max_health", 13.0f}, //
                {"max_energy", 17.0f}, //
                {"attack", 2.0f}, //
                {"defense", 8.0f}, //
                {"recovery", 1.5f}, //
                {"speed", 1.8f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {15, ATTACK_SPLASH}, //
         {20, ATTACK_ICE}, //
         {25, ATTACK_HEAL}},
        {"Gulfin", 34} //
};
inline MonsterData Gulfin = {
        "Gulfin",
        ELEMENT_FIRE,
        {
                {"max_health", 18.0f}, //
                {"max_energy", 20.0f}, //
                {"attack", 3.0f}, //
                {"defense", 10.0f}, //
                {"recovery", 1.8f}, //
                {"speed", 2.0f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {15, ATTACK_SPLASH}, //
         {20, ATTACK_ICE}, //
         {25, ATTACK_HEAL}},
        {"Finiette", 45} //
};
inline MonsterData Finiette = {
        "Finiette",
        ELEMENT_FIRE,
        {
                {"max_health", 27.0f}, //
                {"max_energy", 23.0f}, //
                {"attack", 4.0f}, //
                {"defense", 17.0f}, //
                {"recovery", 2.0f}, //
                {"speed", 2.5f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {15, ATTACK_SPLASH}, //
         {20, ATTACK_ICE}, //
         {25, ATTACK_HEAL}},
        {} //
};
inline MonsterData Atrox = {
        "Atrox",
        ELEMENT_FIRE,
        {
                {"max_health", 18.0f}, //
                {"max_energy", 20.0f}, //
                {"attack", 3.0f}, //
                {"defense", 10.0f}, //
                {"recovery", 1.3f}, //
                {"speed", 1.9f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {30, ATTACK_FIRE}},
        {} //
};
inline MonsterData Pouch = {
        "Pouch",
        ELEMENT_PLANT,
        {
                {"max_health", 23.0f}, //
                {"max_energy", 25.0f}, //
                {"attack", 4.0f}, //
                {"defense", 12.0f}, //
                {"recovery", 1.0f}, //
                {"speed", 1.5f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {25, ATTACK_HEAL}},
        {} //
};
inline MonsterData Draem = {
        "Draem",
        ELEMENT_PLANT,
        {
                {"max_health", 23.0f}, //
                {"max_energy", 25.0f}, //
                {"attack", 4.0f}, //
                {"defense", 12.0f}, //
                {"recovery", 1.2f}, //
                {"speed", 1.4f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_HEAL}, //
         {20, ATTACK_EXPLOSTION}, //
         {25, ATTACK_SPLASH}},
        {} //
};
inline MonsterData Larvea = {
        "Larvea",
        ELEMENT_PLANT,
        {
                {"max_health", 15.0f}, //
                {"max_energy", 17.0f}, //
                {"attack", 1.0f}, //
                {"defense", 8.0f}, //
                {"recovery", 1.0f}, //
                {"speed", 1.0f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}},
        {"Cleaf", 4} //
};
inline MonsterData Cleaf = {
        "Cleaf",
        ELEMENT_PLANT,
        {
                {"max_health", 18.0f}, //
                {"max_energy", 20.0f}, //
                {"attack", 3.0f}, //
                {"defense", 10.0f}, //
                {"recovery", 1.7f}, //
                {"speed", 1.6f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_HEAL}},
        {} //
};
inline MonsterData Jacana = {
        "Jacana",
        ELEMENT_FIRE,
        {
                {"max_health", 12.0f}, //
                {"max_energy", 19.0f}, //
                {"attack", 3.0f}, //
                {"defense", 10.0f}, //
                {"recovery", 2.1f}, //
                {"speed", 2.6f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {15, ATTACK_BURN}, //
         {20, ATTACK_EXPLOSTION}, //
         {25, ATTACK_HEAL}},
        {} //
};
inline MonsterData Friolera = {
        "Friolera",
        ELEMENT_FIRE,
        {
                {"max_health", 13.0f}, //
                {"max_energy", 20.0f}, //
                {"attack", 4.0f}, //
                {"defense", 6.0f}, //
                {"recovery", 1.3f}, //
                {"speed", 2.0f}, //
        },
        {{0, ATTACK_SCRATCH}, //
         {5, ATTACK_SPARK}, //
         {15, ATTACK_SPLASH}, //
         {20, ATTACK_ICE}, //
         {25, ATTACK_HEAL}},
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

enum SelectionSide
{
    PLAYER = 0,
    OPPONENT
};

struct AttackData
{
    std::string name;
    SelectionSide target; // PLAYER - attack same team (healing/defense) | OPPONENT - attack the
    // other team
    float amount;
    int cost;
    ElementType element;
    AttackAnimation animation;
};

inline std::map<Attack, AttackData> ATTACK_DATA = {
        {ATTACK_BURN, {"burn", OPPONENT, 2.0f, 15, ELEMENT_FIRE, ANIMATION_FIRE}}, //
        {ATTACK_HEAL, {"heal", PLAYER, -1.2f, 600, ELEMENT_PLANT, ANIMATION_GREEN}}, //
        {ATTACK_BATTLECRY, {"battlecry", PLAYER, -1.4f, 20, ELEMENT_NORMAL, ANIMATION_GREEN}}, //
        {ATTACK_SPARK, {"spark", OPPONENT, 1.1f, 20, ELEMENT_FIRE, ANIMATION_FIRE}}, //
        {ATTACK_SCRATCH, {"scratch", OPPONENT, 1.2f, 20, ELEMENT_NORMAL, ANIMATION_SCRATCH}}, //
        {ATTACK_SPLASH, {"splash", OPPONENT, 2.0f, 15, ELEMENT_FIRE, ANIMATION_SPLASH}}, //
        {ATTACK_FIRE, {"fire", OPPONENT, 2.0f, 15, ELEMENT_FIRE, ANIMATION_FIRE}}, //
        {ATTACK_EXPLOSTION,
         {"explosion", OPPONENT, 2.0f, 90, ELEMENT_FIRE, ANIMATION_EXPLOSION}}, //
        {ATTACK_ANNIHILATE,
         {"annihilate", OPPONENT, 3.0f, 30, ELEMENT_FIRE, ANIMATION_EXPLOSION}}, //
        {ATTACK_ICE, {"ice", OPPONENT, 2.0f, 15, ELEMENT_FIRE, ANIMATION_ICE}}, //
};
