#pragma once

#include <random>

#include "Player.h"
#include "TimedObjects.h"
#include "Utility.h"

class Enemy : public Entity {
 protected:
  std::shared_ptr<std::vector<std::shared_ptr<TimedObject>>> timed_objects;

  virtual void AddConfigure(){};

 public:
  int max_moves{};
  int move{};
  int radius{};
  bool is_chasing = false;
  Pos center;

  Enemy() { pic = L"E"; }

  void Configure(const Config& config) override {
    SetEntity(config.enemy_hp, config.enemy_damage);
    radius = config.enemy_radius;
    max_moves = config.enemy_moves;
    AddConfigure();
  }

  void Configure(
      std::shared_ptr<std::vector<std::shared_ptr<TimedObject>>>& tos) {
    timed_objects = tos;
  }

  virtual Action MakeAction(Player& player) {
    if (move < max_moves) {
      ++move;
      return STAY;
    }
    int dist = pos.Distance(player.pos);
    if (dist <= radius) {
      is_chasing = true;
      move = 0;

      if (dist > 1) {
        return MOVE;
      } else {
        return ATTACK;
      }
    } else if (is_chasing) {
      center = pos;
      is_chasing = false;
    }
    move = 0;
    return RANDOM_MOVE;
  }

  std::wstring Description() override { return L"Enemy"; }

  void RandomMove(const std::vector<std::shared_ptr<Block>>& blocks,
                  const Config& config) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 3);

    auto dir = static_cast<Direction>(dist(gen));
    if (pos.ToPos(dir).Distance(center) > 2 ||
        !pos.ToPos(dir).IsInbounds(config) ||
        FindByPos(blocks, pos.ToPos(dir)) != blocks.end())
      dir = static_cast<Direction>((dir + dist(gen)) % 4);

    int c = 0;
    while ((pos.ToPos(dir).Distance(center) > 2 ||
            !pos.ToPos(dir).IsInbounds(config) ||
            FindByPos(blocks, pos.ToPos(dir)) != blocks.end()) &&
           c < 4) {
      dir = static_cast<Direction>((dir + 1) % 4);
      ++c;
    }

    if (c < 4) pos = pos.ToPos(dir);
  }
};

class Archer : public Enemy {
  void CreateArrow(Direction dir_) {
    Arrow arrow(dir_, pos.ToPos(dir_), damage);
    timed_objects->push_back(std::make_shared<Arrow>(arrow));
  }

 public:
  Archer() { pic = L"A"; }

  void AddConfigure() override {
    max_moves = static_cast<int>(max_moves * 1.5);
    move = max_moves;
  }

  Action MakeAction(Player& player) override {
    if (move < max_moves) {
      ++move;
      return STAY;
    }

    Pos diff = pos - player.pos;
    if (diff.IsCardinal() && pos.Distance(player.pos) <= radius) {
      if (move <= max_moves)
        ++move;
      else {
        CreateArrow(diff.ToDirection());
        move = 0;
      }
    }
    return STAY;
  }

  std::wstring Description() override { return L"Archer Enemy"; }
};

class Tank : public Enemy {
  void AddConfigure() override {
    max_moves *= 2;
    damage *= 2;
    max_health *= 4;
    health = max_health;
  }

 public:
  Tank() { pic = L"T"; }

  std::wstring Description() override { return L"Tank Enemy"; }
};

class Boss : public Enemy {
  void AddConfigure() override {
    max_moves = 1;
    damage *= 4;
    max_health *= 10;
    radius = static_cast<int>(radius * 1.5);
    health = max_health;
  }

 public:
  Boss() { pic = L"☒"; }

  std::wstring Description() override { return L"BOSS"; }
};
