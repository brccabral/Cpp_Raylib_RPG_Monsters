#include <cstring>
#include <algorithm>
#include <functional>
#include "sprite.h"
#include "dialogsprite.h"
#include "raylib_utils.h"
#include "battle.h"


SimpleSprite::SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups)
{
    for (auto *sprite_group: sprite_groups)
    {
        groups.push_back(sprite_group);
        sprite_group->sprites.push_back(this);
    }
}

// the TMX images are unloaded in ~Game()::UnloadTMX
// if the sub class has its own render/texture, override
// the sub class destructor and unload it there
SimpleSprite::~SimpleSprite() = default;

void SimpleSprite::Draw(const Vector2 offset) const
{
    const Vector2 pos = Vector2Add(rect.pos, offset);

    if (image.texture)
    {
        DrawTextureRec(*image.texture, image.rect.rectangle, pos, WHITE);
    }
    else
    {
        DrawRectangle(pos.x, pos.y, rect.width, rect.height, BLACK);
    }
}

void SimpleSprite::LeaveOtherGroups(const SpriteGroup *sprite_group)
{
    for (const auto group: groups)
    {
        if (group != sprite_group)
        {
            group->sprites.erase(
                    std::remove(group->sprites.begin(), group->sprites.end(), this),
                    group->sprites.end());
        }
    }
}

void SimpleSprite::Kill()
{
    // we add to another vector `to_delete` to delay the deletition to after
    // all group sprites Update(dt)
    if (!groups.empty())
    {
        groups[0]->to_delete.push_back(this);
    }
    for (const auto group: groups)
    {
        group->sprites.erase(
                std::remove(group->sprites.begin(), group->sprites.end(), this),
                group->sprites.end());
    }
    groups.clear();
}

void SimpleSprite::FlipH()
{
    image.rect.width = -image.rect.width;
}

Sprite::Sprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs,
        const int z_)
    : SimpleSprite(sgs), z(z_)
{
    image = img;
    rect = {pos.x, pos.y, image.rect.width, image.rect.height};

    y_sort = GetRectCenter(rect).y;
    type = SPRITE;
    hitbox = rect;
}

MonsterPatchSprite::MonsterPatchSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs,
        std::string bio)
    : Sprite(pos, img, sgs, WORLD_LAYERS["main"]), biome(std::move(bio))
{
    y_sort -= 40;
    // move sand patches to background drawing layer
    if (std::strcmp(biome.c_str(), "sand") == 0)
    {
        z = WORLD_LAYERS["bg"];
    }
}

BorderSprite::BorderSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, img, sgs)
{}

TransitionSprite::TransitionSprite(
        const Vector2 pos, const Vector2 size, std::array<std::string, 2> target,
        const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, {{}, {0, 0, size.x, size.y}}, sgs), target(std::move(target))
{}

CollidableSprite::CollidableSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, img, sgs)
{
    // smaller hitbox for better view perpective on Y axis
    RectInflate(hitbox, 0, -rect.height * 0.6f);
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, const std::vector<TiledTexture> &frms,
        const std::vector<SpriteGroup *> &sgs, const int z)
    : Sprite(position, frms[0], sgs, z), frame_index(0), frames(frms)
{}

void AnimatedSprite::Animate(const double deltaTime)
{
    frame_index += ANIMATION_SPEED * deltaTime;
    image = frames[int(frame_index) % frames.size()];
}

void AnimatedSprite::Update(const double deltaTime)
{
    Animate(deltaTime);
}

void AnimatedSprite::FlipH()
{
    for (auto &[texture, rect]: frames)
    {
        rect.width = -rect.width;
    }
}

MonsterSprite::MonsterSprite(
        const Vector2 position, const std::map<AnimationState, std::vector<TiledTexture>> &frms,
        const std::vector<SpriteGroup *> &sgs, Monster *monster, const int index,
        const int pos_index, const SelectionSide entity, Battle *battle)
    : SimpleSprite(sgs), monster(monster), state_frames(frms), pos_index(pos_index), entity(entity),
      index(index), battle(battle)
{
    image = state_frames[state][int(frame_index)];
    rect = image.rect;
    RectToCenter(rect, position);
    const auto dist = GetRandomUniformDist(-1, 1);
    animation_speed = ANIMATION_SPEED + GetRandomUniform(dist);
    type = MONSTERSPRITE;

    for (auto &[state, tiles]: state_frames)
    {
        std::vector<TiledTexture> highlight_tiles;
        for (auto &[tile_texture, tile_rect]: tiles)
        {
            Texture2D highlight_texture = TextureMaskFromTexture(tile_texture, COLORS["white"]);
            highlight_tiles.push_back({Texture2DToPointer(highlight_texture), {tile_rect}});
        }
        state_frames_highlight[state] = highlight_tiles;
    }

    timers["remove_highlight"] =
            new Timer(0.3f, false, false, std::bind(&MonsterSprite::SetHighlight, this, false));
    timers["kill"] = new Timer(0.6f, false, false, std::bind(&MonsterSprite::Destroy, this));
}

MonsterSprite::~MonsterSprite()
{
    for (const auto &[key, timer]: timers)
    {
        delete timer;
    }

    // state_frames_highlight are created with Texture2DToPointer(), which calls MemAlloc
    for (auto &[anim_state, frames]: state_frames_highlight)
    {
        for (const auto [texture, rect]: frames)
        {
            MemFree(texture);
        }
    }
}

void MonsterSprite::Animate(const double dt)
{
    frame_index += animation_speed * dt;
    // attack animation has finished
    if (state == ANIMATION_ATTACK && frame_index >= state_frames[ANIMATION_ATTACK].size())
    {
        // apply attack
        state = ANIMATION_IDLE;
        battle->ApplyAttack(target_sprite, current_attack, monster->GetBaseDamage(current_attack));
    }
    adjusted_frame_index = int(frame_index) % state_frames[state].size();
    image = state_frames[state][adjusted_frame_index];

    if (highlight)
    {
        image = state_frames_highlight[state][adjusted_frame_index];
    }
}

void MonsterSprite::Update(const double dt)
{
    for (auto &[key, timer]: timers)
    {
        timer->Update();
    }
    Animate(dt);
    monster->Update(dt);
}

void MonsterSprite::FlipH()
{
    for (auto &[state, tiles]: state_frames)
    {
        for (auto &[texture, tile_rect]: tiles)
        {
            tile_rect.width = -tile_rect.width;
        }
    }
    for (auto &[state, tiles]: state_frames_highlight)
    {
        for (auto &[texture, tile_rect]: tiles)
        {
            tile_rect.width = -tile_rect.width;
        }
    }
}

void MonsterSprite::SetHighlight(const bool value)
{
    highlight = value;
    if (highlight)
    {
        timers["remove_highlight"]->Activate();
    }
}

void MonsterSprite::ActivateAttack(MonsterSprite *monster_sprite, const Attack selected_attack)
{
    state = ANIMATION_ATTACK;
    frame_index = 0;
    target_sprite = monster_sprite;
    current_attack = selected_attack;
    monster->ReduceEnergy(selected_attack);
}
void MonsterSprite::Kill()
{
    // the Battle sorts the sprites, the Name and Outline will go behind
    // this main sprite in the list, so, when we delete this, the others
    // still exist in the Group vector. Therefore, we make sure to Kill()
    // all of them here too
    SimpleSprite::Kill();
    if (name_sprite_)
    {
        name_sprite_->Kill();
    }
    if (level_sprite_)
    {
        level_sprite_->Kill();
    }
    if (stats_sprite_)
    {
        stats_sprite_->Kill();
    }
    if (outline_sprite_)
    {
        outline_sprite_->Kill();
    }
}

void MonsterSprite::DelayedKill(Monster *monster, int index, int pos_index, SelectionSide side)
{
    if (!timers["kill"]->active)
    {
        newMonster = monster;
        newIndex = index;
        newPosIndex = pos_index;
        newSide = side;
        timers["kill"]->Activate();
    }
}

void MonsterSprite::Destroy()
{
    if (newMonster)
    {
        battle->AddNewMonster(newMonster, newIndex, newPosIndex, newSide);
        newMonster = nullptr;
    }
    target_sprite = nullptr;
    Kill();
}

void MonsterSprite::SetNameSprite(MonsterNameSprite *name_sprite)
{
    name_sprite_ = name_sprite;
}
void MonsterSprite::SetLevelSprite(MonsterLevelSprite *level_sprite)
{
    level_sprite_ = level_sprite;
}
void MonsterSprite::SetStatsSprite(MonsterStatsSprite *stats_sprite)
{
    stats_sprite_ = stats_sprite;
}
void MonsterSprite::SetOutlineSprite(MonsterOutlineSprite *outline_sprite)
{
    outline_sprite_ = outline_sprite;
}

MonsterNameSprite::MonsterNameSprite(
        const Vector2 pos, MonsterSprite *monster_sprite, const std::vector<SpriteGroup *> &sgs,
        Font font)
    : SimpleSprite(sgs), monster_sprite(monster_sprite)
{
    const auto [text_width, text_height] =
            MeasureTextEx(font, monster_sprite->monster->name.c_str(), font.baseSize, 1);
    int padding = 10;
    const Vector2 render_size = {text_width + 2 * padding, text_height + 2 * padding};
    image.rect = {Vector2{0.0f, 0.0f}, render_size};

    // need another render to invert the image
    // https://github.com/raysan5/raylib/issues/3803
    // https://github.com/raysan5/raylib/issues/378
    RenderTexture2D inverted = LoadRenderTextureV(render_size);
    BeginTextureModeC(inverted, WHITE);
    DrawCenteredTextEx(font, monster_sprite->monster->name.c_str(), image.rect, COLORS["black"]);
    EndTextureModeSafe();

    render = LoadRenderTextureV(render_size);
    BeginTextureModeC(render, BLANK);
    DrawTexture(inverted.texture, 0, 0, WHITE);
    EndTextureModeSafe();

    image.texture = &render.texture;

    rect = image.rect;
    rect.pos = pos;

    UnloadRenderTexture(inverted);

    type = MONSTERNAMESPRITE;
}

MonsterNameSprite::~MonsterNameSprite()
{
    UnloadRenderTexture(render);
}

void MonsterLevelSprite::UpdateTexture() const
{
    BeginTextureModeC(inverted, WHITE);
    DrawCenteredTextEx(
            font, TextFormat("Lvl %i", monster_sprite->monster->level), image.rect,
            COLORS["black"]);
    DrawBar(xp_rect, monster_sprite->monster->xp, monster_sprite->monster->level_up,
            COLORS["black"], COLORS["white"]);
    EndTextureModeSafe();

    BeginTextureMode(render);
    DrawTexture(inverted.texture, 0, 0, WHITE);
    EndTextureModeSafe();
}

MonsterLevelSprite::MonsterLevelSprite(
        const SelectionSide entity, const Vector2 pos, MonsterSprite *monster_sprite,
        const std::vector<SpriteGroup *> &sgs, const Font &font)
    : SimpleSprite(sgs), entity(entity), monster_sprite(monster_sprite), font(font)
{
    constexpr Vector2 render_size = {60, 26};
    image.rect = {0, 0, render_size};
    xp_rect = {0, image.rect.height - 2, image.rect.width, 2.0f};

    render = LoadRenderTextureV(render_size);
    inverted = LoadRenderTextureV(render_size);
    image.texture = &render.texture;

    UpdateTexture();

    rect = image.rect;
    if (entity == PLAYER)
    {
        RectToTopLeft(rect, pos);
    }
    else
    {
        RectToTopRight(rect, pos);
    }

    type = MONSTERLEVELSPRITE;
}

MonsterLevelSprite::~MonsterLevelSprite()
{
    UnloadRenderTexture(render);
    UnloadRenderTexture(inverted);
}

void MonsterLevelSprite::Update(const double deltaTime)
{
    UpdateTexture();
}

MonsterStatsSprite::MonsterStatsSprite(
        const Vector2 pos, MonsterSprite *monster_sprite, const Vector2 size,
        const std::vector<SpriteGroup *> &sgs, const Font &font)
    : SimpleSprite(sgs), monster_sprite(monster_sprite), font(font)
{
    render = LoadRenderTextureV(size);
    inverted = LoadRenderTextureV(size);

    image.rect = {0.0f, 0.0f, size};
    image.texture = &render.texture;
    rect = image.rect;
    RectToMidBottom(rect, pos);

    type = MONSTERSTATSSPRITE;
}

MonsterStatsSprite::~MonsterStatsSprite()
{
    UnloadRenderTexture(render);
    UnloadRenderTexture(inverted);
}

void MonsterStatsSprite::Update(double deltaTime)
{
    BeginTextureModeC(inverted, WHITE);

    const auto info = monster_sprite->monster->GetInfo();
    const std::array<Color, 3> colors = {COLORS["red"], COLORS["blue"], COLORS["gray"]};
    for (int index = 0; index < info.size(); ++index)
    {
        const auto [value, max_value] = info[index];
        const auto color = colors[index];
        if (index < 2)
        {
            const Vector2 pos = {rect.width * 0.05f, index * rect.height / 2.0f};
            DrawTextEx(
                    font, TextFormat("%i/%i", int(value), int(max_value)), pos, font.baseSize, 1,
                    COLORS["black"]);
            // height less than 6 there is no roundness
            const RectangleU bar_rect = {pos.x, pos.y + font.baseSize - 2, rect.width * 0.9f, 6};
            DrawBar(bar_rect, value, max_value, color, COLORS["black"], 0.9f, 10);
        }
        else
        {
            const RectangleU init_rect = {0.0f, rect.height - 2, rect.width, 2};
            DrawBar(init_rect, value, max_value, color, COLORS["white"]);
        }
    }

    EndTextureModeSafe();

    BeginTextureModeC(render, WHITE);
    DrawTexture(inverted.texture, 0, 0, WHITE);
    EndTextureModeSafe();
}

MonsterOutlineSprite::MonsterOutlineSprite(
        MonsterSprite *monster_sprite, const std::vector<SpriteGroup *> &sgs,
        const std::map<AnimationState, std::vector<TiledTexture>> &frms)
    : SimpleSprite(sgs), monster_sprite(monster_sprite), state_frames(frms)
{
    type = MONSTEROUTLINESPRITE;

    image = state_frames[monster_sprite->state][0];
    rect = monster_sprite->rect;
}

void MonsterOutlineSprite::Update(double deltaTime)
{
    image = state_frames[monster_sprite->state][monster_sprite->adjusted_frame_index];
}

void MonsterOutlineSprite::FlipH()
{
    for (auto &[state, tiles]: state_frames)
    {
        for (auto &[texture, tile_rect]: tiles)
        {
            tile_rect.width = -tile_rect.width;
        }
    }
}

AttackSprite::AttackSprite(
        const Vector2 position, const std::vector<TiledTexture> &frms,
        const std::vector<SpriteGroup *> &sgs, const int z)
    : AnimatedSprite(position, frms, sgs, z)
{
    RectToCenter(rect, position);
}

void AttackSprite::Animate(const double deltaTime)
{
    frame_index += ANIMATION_SPEED * deltaTime;
    if (frame_index < frames.size())
    {
        image = frames[int(frame_index) % frames.size()];
    }
    else
    {
        Kill();
    }
}

void SpriteGroup::Draw() const
{
    for (const auto *sprite: sprites)
    {
        sprite->Draw({0, 0});
    }
}

void SpriteGroup::Update(const double deltaTime)
{
    for (auto *sprite: sprites)
    {
        sprite->Update(deltaTime);
    }
    if (!to_delete.empty())
    {
        for (const auto *sprite: to_delete)
        {
            delete sprite;
            sprite = nullptr;
        }
        to_delete.clear();
    }
}

int GetZ(const SimpleSprite *sprite)
{
    int z = 0;

    if (sprite->type == SPRITE)
    {
        z = ((Sprite *) sprite)->z;
    }
    else if (sprite->type == ENTITY)
    {
        z = ((Entity *) sprite)->z;
    }
    else if (sprite->type == DIALOGSPRITE)
    {
        z = ((DialogSprite *) sprite)->z;
    }
    return z;
}

int GetYsort(const SimpleSprite *sprite)
{
    int y = 0;

    if (sprite->type == SPRITE)
    {
        y = ((Sprite *) sprite)->y_sort;
    }
    else if (sprite->type == ENTITY)
    {
        y = ((Entity *) sprite)->y_sort;
    }
    else if (sprite->type == DIALOGSPRITE)
    {
        y = ((DialogSprite *) sprite)->y_sort;
    }
    else if (sprite->type == MONSTERSPRITE)
    {
        y = ((MonsterSprite *) sprite)->z;
    }
    else if (sprite->type == MONSTERNAMESPRITE)
    {
        y = ((MonsterNameSprite *) sprite)->z;
    }
    else if (sprite->type == MONSTERLEVELSPRITE)
    {
        y = ((MonsterLevelSprite *) sprite)->z;
    }
    else if (sprite->type == MONSTERSTATSSPRITE)
    {
        y = ((MonsterStatsSprite *) sprite)->z;
    }
    else if (sprite->type == MONSTEROUTLINESPRITE)
    {
        y = ((MonsterOutlineSprite *) sprite)->z;
    }
    return y;
}

RectangleU GetHitbox(const SimpleSprite *sprite)
{
    RectangleU rect{};

    if (sprite->type == SPRITE)
    {
        rect = ((Sprite *) sprite)->hitbox;
    }
    else if (sprite->type == ENTITY)
    {
        rect = ((Entity *) sprite)->hitbox;
    }
    return rect;
}

SpriteGroup::~SpriteGroup()
{
    for (auto *sprite: sprites)
    {
        // remove sprite from other groups to avoid double `delete`
        sprite->LeaveOtherGroups(this);
        delete sprite;
    }
}
