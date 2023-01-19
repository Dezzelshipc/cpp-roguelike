#pragma once

#include <string>

#include "Block.h"
#include "Utility.h"

class TimedObject : public BaseObject {
 public:
  int move{};
  int max_moves{};
  int damage{};
  int range{};

  bool is_players = false;

  virtual void MakeAction(
      std::vector<std::shared_ptr<Entity>>& entities,
      std::vector<std::shared_ptr<Block>>& blocks,
      std::vector<std::shared_ptr<TimedObject>>& timed_objects,
      std::vector<std::wstring>& story){};

  bool IsToDestroy() override { return move >= max_moves; }
};

class Arrow : public TimedObject {
 public:
  Direction direction;

  Arrow(Direction dir_, const Pos& pos_, int damage_) {
    pos = pos_;
    damage = damage_;
    max_moves = 1;
    range = 10;

    direction = dir_;
    switch (dir_) {
      case UP:
        pic = L"↥";
        break;
      case DOWN:
        pic = L"↧";
        break;
      case LEFT:
        pic = L"↤";
        break;
      case RIGHT:
        pic = L"↦";
        break;
      case NONE:
        break;
    }
  }

  void MakeAction(std::vector<std::shared_ptr<Entity>>& entities,
                  std::vector<std::shared_ptr<Block>>& blocks,
                  std::vector<std::shared_ptr<TimedObject>>& timed_objects,
                  std::vector<std::wstring>& story) override {
    ++move;
    if (move < max_moves) return;

    Pos pos_ = pos;
    for (int i = 0; i <= range; ++i, pos_ = pos_.ToPos(direction)) {
      auto block = std::find_if(blocks.begin(), blocks.end(),
                                [pos_](const std::shared_ptr<Block>& block) {
                                  return block->pos == pos_;
                                });
      if (block != blocks.end() && block->get()->is_solid) return;

      auto ent = std::find_if(entities.begin(), entities.end(),
                              [pos_](const std::shared_ptr<Entity>& ent) {
                                return ent->pos == pos_;
                              });
      if (ent != entities.end()) {
        if (!is_players && !ent->get()->is_player)
          story.emplace_back(L"Enemy dealt " + std::to_wstring(damage) +
                             L" damage to other Enemy");
        else if (!is_players || ent->get()->is_player)
          story.emplace_back(L"Player took " + std::to_wstring(damage) +
                             L" damage");
        else
          story.emplace_back(L"Player dealt " + std::to_wstring(damage) +
                             L" damage");

        OnCollide(*ent);
        return;
      }
    }
  }

  std::wstring Description() override { return L"Arrow"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    entity->health -= damage;
  }
};

class Smoke : public TimedObject {
 public:
  explicit Smoke(const Pos& pos_) {
    pic = L"▪";
    pos = pos_;
    max_moves = 1;
  }

  void MakeAction(std::vector<std::shared_ptr<Entity>>& entities,
                  std::vector<std::shared_ptr<Block>>& blocks,
                  std::vector<std::shared_ptr<TimedObject>>& timed_objects,
                  std::vector<std::wstring>& story) override {
    ++move;
  }

  std::wstring Description() override { return L"Smoke"; }
};

class Bomb : public TimedObject {
 public:
  Bomb(const Pos& pos_, int damage_, const Config& config_) {
    pic = L"◔";

    pos = pos_;
    damage = damage_;
    range = config_.bomb_radius;
    max_moves = range + 3 + log2(range);
  }

  std::wstring Description() override {
    return L"Fired Bomb: " + std::to_wstring(max_moves - move);
  }

  void MakeAction(std::vector<std::shared_ptr<Entity>>& entities,
                  std::vector<std::shared_ptr<Block>>& blocks,
                  std::vector<std::shared_ptr<TimedObject>>& timed_objects,
                  std::vector<std::wstring>& story) override {
    ++move;
    if (move < max_moves) return;

    std::set<Pos> poses;

    for (int x = pos.x - range; x <= pos.x + range; ++x)
      for (int y = pos.y - range; y <= pos.y + range; ++y)
        if (pos.Distance({x, y}) < range) poses.insert({x, y});

    for (auto& pos_ : poses) {
      bool is_smoke = true;

      auto block = std::find_if(blocks.begin(), blocks.end(),
                                [pos_](const std::shared_ptr<Block>& block) {
                                  return block->pos == pos_;
                                });
      if (block != blocks.end()) {
        block->get()->is_to_destroy = block->get()->is_destructible;
        is_smoke = block->get()->is_destructible;
      }

      auto ent = std::find_if(entities.begin(), entities.end(),
                              [pos_](const std::shared_ptr<Entity>& ent) {
                                return ent->pos == pos_;
                              });
      if (ent != entities.end()) {
        if (!is_players && !ent->get()->is_player)
          story.emplace_back(L"Enemy dealt " + std::to_wstring(damage) +
                             L" damage to other Enemy");
        else if (!is_players || ent->get()->is_player)
          story.emplace_back(L"Player took " + std::to_wstring(damage) +
                             L" damage");
        else
          story.emplace_back(L"Player dealt " + std::to_wstring(damage) +
                             L" damage");
        OnCollide(*ent);
      }

      if (is_smoke) {
        Smoke smoke(pos_);
        timed_objects.push_back(std::make_shared<Smoke>(smoke));
      }
    }
  }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    entity->health -= damage;
  }
};