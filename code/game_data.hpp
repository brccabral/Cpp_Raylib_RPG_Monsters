#pragma once
#include <string>
#include <vector>
#include <map>

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
    std::vector<std::string> directions;
    bool look_around;
    bool defeated;
    std::string biome;
    int radius;
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
    ATTACKANIMATION_FIRE = 0,
    ATTACKANIMATION_GREEN,
    ATTACKANIMATION_SCRATCH,
    ATTACKANIMATION_SPLASH,
    ATTACKANIMATION_EXPLOSION,
    ATTACKANIMATION_ICE
};

enum ElementType
{
    ELEMENT_FIRE = 0,
    ELEMENT_PLANT,
    ELEMENT_WATER,
    ELEMENT_NORMAL
};

struct MonsterData
{
    std::string name;
    ElementType element;
    std::map<std::string, float> stats;
    std::vector<std::pair<int, Attack>> abilities;
    std::pair<std::string, int> evolve;
};

enum SelectionSide
{
    PLAYER = 0,
    OPPONENT
};

struct AttackData
{
    std::string name;
    // PLAYER - attack same team (healing/defense) | OPPONENT - attack the other team
    SelectionSide target;
    float amount;
    int cost;
    ElementType element;
    AttackAnimation animation;
};

class GameData
{
public:

    static GameData &GetInstance()
    {
        if (!instance)
        {
            instance = new GameData();
        }
        return *instance;
    }

    static void Destroy()
    {
        delete instance;
    }

    CharacterData o1 = {
            .monsters = {{"Jacana", 14}, {"Cleaf", 15}}, //
            .dialog = {.default_ = {"Hey, how are you?", "Oh, so you want to fight?", "FIGHT!"},
                       .defeated = {"You are very strong!", "Let's fight again sometime?"}}, //
            .directions = {"down"},
            .look_around = true,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData o2 = {
            .monsters = {{"Atrox", 14}, {"Pouch", 15}, {"Draem", 13}, {"Cindrill", 13}}, //
            .dialog = {.default_ = {"I don't like sand", "It's coarse and rough", "oh god, fight"},
                       .defeated = {"May the force be with you"}}, //
            .directions = {"left", "down"},
            .look_around = false,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData o3 = {
            .monsters = {{"Atrox", 14}, {"Pouch", 15}, {"Draem", 13}, {"Cindrill", 13}}, //
            .dialog = {.default_ = {"I love skating!", "FIGHT!"},
                       .defeated = {"Good luck with the boss", "It's so cold in here"}}, //
            .directions = {"left", "right", "up", "down"},
            .look_around = true,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData o4 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I love skating!", "FIGHT!"},
                       .defeated = {"Good luck with the boss", "It's so cold in here"}}, //
            .directions = {"right"},
            .look_around = true,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData o5 = {
            .monsters = {{"Plumette", 20}, {"Ivieron", 22}, {"Atrox", 24}, {"Pouch", 19}}, //
            .dialog = {.default_ = {"So you want to challenge the big ones", "This will be fun!"},
                       .defeated = {"I hope the lawyers will never spot you", "<3"}}, //
            .directions = {"up", "right"},
            .look_around = true,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData o6 = {
            .monsters = {{"Finsta", 15}, {"Finsta", 15}, {"Finsta", 15}}, //
            .dialog = {.default_ = {"I love skating!", "FIGHT!"},
                       .defeated = {"Good luck with the boss", "It's so cold in here"}}, //
            .directions = {"down"},
            .look_around = false,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData o7 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"There are no bugs in the snow!"},
                       .defeated = {"Maybe I should check a vulcano...", "It's so cold in here"}},
            //
            .directions = {"right"},
            .look_around = false,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData p1 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I love trees", "and fights"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = false,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData p2 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I love trees", "and fights"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = false,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData p3 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I love trees", "and fights"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = false,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData p4 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I love trees", "and fights"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = false,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData px = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I love trees", "and fights"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = false,
            .defeated = false,
            .biome = "forest",
            .radius = 0};

    CharacterData w1 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"It's so cold in here", "maybe a fight will warm me up"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"left"},
            .look_around = true,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData w2 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"It's so cold in here", "maybe a fight will warm me up"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = true,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData w3 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"It's so cold in here", "maybe a fight will warm me up"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = true,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData w4 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"It's so cold in here", "maybe a fight will warm me up"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"left"},
            .look_around = true,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData w5 = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"It's so cold in here", "maybe a fight will warm me up"},
                       .defeated = {"Good luck with the boss!"}}, //
            .directions = {"right"},
            .look_around = true,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData wx = {
            .monsters = {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            .dialog = {.default_ = {"I hope you brought rations", "This will be a long journey"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"down"},
            .look_around = false,
            .defeated = false,
            .biome = "ice",
            .radius = 0};

    CharacterData f1 = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"This place feels kinda warm...", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"right"},
            .look_around = true,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData f2 = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"This place feels kinda warm...", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"right", "left"},
            .look_around = false,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData f3 = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"This place feels kinda warm...", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"right", "left"},
            .look_around = true,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData f4 = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"This place feels kinda warm...", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"up", "right"},
            .look_around = true,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData f5 = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"This place feels kinda warm...", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"left"},
            .look_around = true,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData f6 = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"This place feels kinda warm...", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"right"},
            .look_around = true,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData fx = {
            .monsters = {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            .dialog = {.default_ = {"Time to bring the heat", "fight!"},
                       .defeated = {"Congratultion!"}}, //
            .directions = {"down"},
            .look_around = false,
            .defeated = false,
            .biome = "sand",
            .radius = 0};

    CharacterData Nurse = {
            .monsters = {}, //
            .dialog = {.default_ = {"Welcome to the hospital", "Your monsters have been healed"},
                       .defeated = {}}, //
            .directions = {"down"},
            .look_around = false,
            .defeated = false,
            .biome = "None",
            .radius = 0};


    std::map<std::string, CharacterData> TRAINER_DATA = {
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

    std::map<ElementType, std::string> NAMES_ELEMENT_TYPES{
            {ELEMENT_FIRE, "fire"},
            {ELEMENT_PLANT, "plant"},
            {ELEMENT_WATER, "water"},
            {ELEMENT_NORMAL, "normal"}};

    std::map<std::string, AttackAnimation> ATTACK_ANIMATION_NAMES = {
            {"fire", ATTACKANIMATION_FIRE}, {"green", ATTACKANIMATION_GREEN},
            {"scratch", ATTACKANIMATION_SCRATCH}, {"splash", ATTACKANIMATION_SPLASH},
            {"explosion", ATTACKANIMATION_EXPLOSION}, {"ice", ATTACKANIMATION_ICE},
    };

    std::map<AttackAnimation, std::string> NAMES_ATTACK_ANIMATION = {
            {ATTACKANIMATION_FIRE, "fire"}, {ATTACKANIMATION_GREEN, "green"},
            {ATTACKANIMATION_SCRATCH, "scratch"}, {ATTACKANIMATION_SPLASH, "splash"},
            {ATTACKANIMATION_EXPLOSION, "explosion"}, {ATTACKANIMATION_ICE, "ice"},
    };

    MonsterData Plumette = {
            .name = "Plumette",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 15.0f}, //
                    {"max_energy", 17.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.0f}, //
                    {"speed", 1.0f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}},
            .evolve = {"Ivieron", 15} //
    };
    MonsterData Ivieron = {
            .name = "Ivieron",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 5.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.2f}, //
                    {"speed", 1.2f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}},
            .evolve = {"Pluma", 32} //
    };
    MonsterData Pluma = {
            .name = "Pluma",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 23.0f}, //
                    {"max_energy", 26.0f}, //
                    {"attack", 6.0f}, //
                    {"defense", 12.0f}, //
                    {"recovery", 1.8f}, //
                    {"speed", 1.8f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}},
            .evolve = {} //
    };
    MonsterData Sparchu = {
            .name = "Sparchu",
            .element = ELEMENT_FIRE,
            .stats = {
                    {"max_health", 15.0f}, //
                    {"max_energy", 7.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.1f}, //
                    {"speed", 1.0f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_FIRE}, //
                          {15, ATTACK_BATTLECRY}, //
                          {26, ATTACK_EXPLOSTION}},
            .evolve = {"Cindrill", 15} //
    };
    MonsterData Cindrill = {
            .name = "Cindrill",
            .element = ELEMENT_FIRE,
            .stats = {
                    {"max_health", 18.0f}, //
                    {"max_energy", 10.0f}, //
                    {"attack", 3.5f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.2f}, //
                    {"speed", 1.1f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_FIRE}, //
                          {15, ATTACK_BATTLECRY}, //
                          {26, ATTACK_EXPLOSTION}},
            .evolve = {"Charmadillo", 33} //
    };
    MonsterData Charmadillo = {
            .name = "Charmadillo",
            .element = ELEMENT_FIRE,
            .stats = {
                    {"max_health", 29.0f}, //
                    {"max_energy", 12.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 17.0f}, //
                    {"recovery", 1.35f}, //
                    {"speed", 1.1f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_FIRE}, //
                          {15, ATTACK_BATTLECRY}, //
                          {26, ATTACK_EXPLOSTION}, //
                          {45, ATTACK_ANNIHILATE}},
            .evolve = {} //
    };
    MonsterData Finsta = {
            .name = "Finsta",
            .element = ELEMENT_WATER,
            .stats = {
                    {"max_health", 13.0f}, //
                    {"max_energy", 17.0f}, //
                    {"attack", 2.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.5f}, //
                    {"speed", 1.8f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {15, ATTACK_SPLASH}, //
                          {20, ATTACK_ICE}, //
                          {25, ATTACK_HEAL}},
            .evolve = {"Gulfin", 34} //
    };
    MonsterData Gulfin = {
            .name = "Gulfin",
            .element = ELEMENT_WATER,
            .stats = {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.8f}, //
                    {"speed", 2.0f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {15, ATTACK_SPLASH}, //
                          {20, ATTACK_ICE}, //
                          {25, ATTACK_HEAL}},
            .evolve = {"Finiette", 45} //
    };
    MonsterData Finiette = {
            .name = "Finiette",
            .element = ELEMENT_WATER,
            .stats = {
                    {"max_health", 27.0f}, //
                    {"max_energy", 23.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 17.0f}, //
                    {"recovery", 2.0f}, //
                    {"speed", 2.5f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {15, ATTACK_SPLASH}, //
                          {20, ATTACK_ICE}, //
                          {25, ATTACK_HEAL}},
            .evolve = {} //
    };
    MonsterData Atrox = {
            .name = "Atrox",
            .element = ELEMENT_FIRE,
            .stats = {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.3f}, //
                    {"speed", 1.9f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {30, ATTACK_FIRE}},
            .evolve = {} //
    };
    MonsterData Pouch = {
            .name = "Pouch",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 23.0f}, //
                    {"max_energy", 25.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 12.0f}, //
                    {"recovery", 1.0f}, //
                    {"speed", 1.5f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {25, ATTACK_HEAL}},
            .evolve = {} //
    };
    MonsterData Draem = {
            .name = "Draem",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 23.0f}, //
                    {"max_energy", 25.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 12.0f}, //
                    {"recovery", 1.2f}, //
                    {"speed", 1.4f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_HEAL}, //
                          {20, ATTACK_EXPLOSTION}, //
                          {25, ATTACK_SPLASH}},
            .evolve = {} //
    };
    MonsterData Larvea = {
            .name = "Larvea",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 15.0f}, //
                    {"max_energy", 17.0f}, //
                    {"attack", 1.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.0f}, //
                    {"speed", 1.0f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}},
            .evolve = {"Cleaf", 4} //
    };
    MonsterData Cleaf = {
            .name = "Cleaf",
            .element = ELEMENT_PLANT,
            .stats = {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.7f}, //
                    {"speed", 1.6f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_HEAL}},
            .evolve = {} //
    };
    MonsterData Jacana = {
            .name = "Jacana",
            .element = ELEMENT_FIRE,
            .stats = {
                    {"max_health", 12.0f}, //
                    {"max_energy", 19.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 2.1f}, //
                    {"speed", 2.6f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {15, ATTACK_BURN}, //
                          {20, ATTACK_EXPLOSTION}, //
                          {25, ATTACK_HEAL}},
            .evolve = {} //
    };
    MonsterData Friolera = {
            .name = "Friolera",
            .element = ELEMENT_WATER,
            .stats = {
                    {"max_health", 13.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 6.0f}, //
                    {"recovery", 1.3f}, //
                    {"speed", 2.0f}, //
            },
            .abilities = {{0, ATTACK_SCRATCH}, //
                          {5, ATTACK_SPARK}, //
                          {15, ATTACK_SPLASH}, //
                          {20, ATTACK_ICE}, //
                          {25, ATTACK_HEAL}},
            .evolve = {} //
    };

    std::map<std::string, MonsterData> MONSTER_DATA = {
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

    std::map<Attack, AttackData> ATTACK_DATA = {
            {ATTACK_BURN,
             {.name = "burn", .target = OPPONENT, .amount = 2.0f, .cost = 15,
              .element = ELEMENT_FIRE, .animation = ATTACKANIMATION_FIRE}}, //
            {ATTACK_HEAL,
             {.name = "heal", .target = PLAYER, .amount = -1.2f, .cost = 600,
              .element = ELEMENT_PLANT, .animation = ATTACKANIMATION_GREEN}}, //
            {ATTACK_BATTLECRY,
             {.name = "battlecry", .target = PLAYER, .amount = -1.4f, .cost = 20,
              .element = ELEMENT_NORMAL, .animation = ATTACKANIMATION_GREEN}}, //
            {ATTACK_SPARK,
             {.name = "spark", .target = OPPONENT, .amount = 1.1f, .cost = 20,
              .element = ELEMENT_FIRE, .animation = ATTACKANIMATION_FIRE}}, //
            {ATTACK_SCRATCH,
             {.name = "scratch", .target = OPPONENT, .amount = 1.2f, .cost = 20,
              .element = ELEMENT_NORMAL, .animation = ATTACKANIMATION_SCRATCH}}, //
            {ATTACK_SPLASH,
             {.name = "splash", .target = OPPONENT, .amount = 2.0f, .cost = 15,
              .element = ELEMENT_WATER, .animation = ATTACKANIMATION_SPLASH}},
            //
            {ATTACK_FIRE,
             {.name = "fire", .target = OPPONENT, .amount = 2.0f, .cost = 15,
              .element = ELEMENT_FIRE, .animation = ATTACKANIMATION_FIRE}}, //
            {ATTACK_EXPLOSTION,
             {.name = "explosion", .target = OPPONENT, .amount = 2.0f, .cost = 90,
              .element = ELEMENT_FIRE, .animation = ATTACKANIMATION_EXPLOSION}}, //
            {ATTACK_ANNIHILATE,
             {.name = "annihilate", .target = OPPONENT, .amount = 3.0f, .cost = 30,
              .element = ELEMENT_FIRE, .animation = ATTACKANIMATION_EXPLOSION}}, //
            {ATTACK_ICE,
             {.name = "ice", .target = OPPONENT, .amount = 2.0f, .cost = 15,
              .element = ELEMENT_WATER, .animation = ATTACKANIMATION_ICE}}, //
    };

private:

    GameData() = default;
    ~GameData() = default;

    static GameData *instance;
};
