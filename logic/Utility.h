#pragma once
#include <ncurses.h>

#include <cmath>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "Config.h"

using chartype = std::wstring;

enum Action { STAY, ATTACK, MOVE, RANDOM_MOVE };

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

class Pos : public std::pair<int, int> {
 public:
  int& x = first;
  int& y = second;
  Pos() = default;
  Pos(int x_, int y_) { Set(x_, y_); };

  Pos(const Pos& pos_) : Pos(pos_.x, pos_.y){};

  void Set(int x_, int y_) {
    x = x_;
    y = y_;
  }
  void Set(const Pos& pos) { Set(pos.x, pos.y); }

  Pos operator=(const Pos& pos_) {
    Set(pos_);
    return *this;
  }

  [[nodiscard]] int Distance(const Pos& pos_) const {
    Pos diff = *this - pos_;
    return abs(diff.x) + abs(diff.y);
  }

  [[nodiscard]] float MathDistance(const Pos& pos_) const {
    Pos diff = *this - pos_;
    return sqrt(diff.x * diff.x + diff.y * diff.y);
  }

  friend Pos operator-(const Pos& lhs, const Pos& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
  }

  [[nodiscard]] Pos Up() const { return {x, y - 1}; }
  [[nodiscard]] Pos Down() const { return {x, y + 1}; }
  [[nodiscard]] Pos Left() const { return {x - 1, y}; }
  [[nodiscard]] Pos Right() const { return {x + 1, y}; }

  [[nodiscard]] Pos ToPos(Direction dir_) const {
    switch (dir_) {
      case UP:
        return Up();
      case DOWN:
        return Down();
      case LEFT:
        return Left();
      case RIGHT:
        return Right();
      case NONE:
        break;
    }
    return *this;
  }

  [[nodiscard]] Direction ToDirection() const {
    if (abs(x) > abs(y)) {
      if (x > 0)
        return LEFT;
      else
        return RIGHT;
    } else {
      if (y > 0)
        return UP;
      else
        return DOWN;
    }
  }

  [[nodiscard]] bool IsCardinal() const { return x == 0 || y == 0; }

  [[nodiscard]] bool IsInbounds(const Config& config) const {
    return 0 <= x && x < config.size_x && 0 <= y && y < config.size_y;
  }

  Direction FindPath(const Pos& to_pos, int radius, const std::set<Pos>& blocks,
                     const Config& config) const {
    std::queue<Pos> queue, cur_queue;
    std::map<Pos, std::pair<int, Direction>> dirs;

    auto add_start = [&](const Pos& p, Direction d) {
      if (!blocks.contains(p)) {
        if (d == NONE)
          dirs.insert({p, {0, NONE}});
        else {
          dirs.insert({p, {1, d}});
          queue.push(p);
        }
      }
    };
    add_start(Left(), LEFT);
    add_start(Right(), RIGHT);
    add_start(Up(), UP);
    add_start(Down(), DOWN);

    auto add = [&](const Pos& p) {
      if (p.Distance(to_pos) <= radius && !dirs.contains(p) &&
          !blocks.contains(p) && p.IsInbounds(config) &&
          !blocks.contains(cur_queue.front())) {
        dirs.insert({p,
                     {dirs[cur_queue.front()].first + 1,
                      dirs[cur_queue.front()].second}});
        queue.push(p);
      }
    };

    do {
      cur_queue.swap(queue);
      while (!cur_queue.empty()) {
        add(cur_queue.front().Left());
        add(cur_queue.front().Right());
        add(cur_queue.front().Up());
        add(cur_queue.front().Down());
        cur_queue.pop();
      }
    } while (!queue.empty() && !dirs.contains(to_pos));

    if (!dirs.contains(to_pos)) return NONE;
    return dirs[to_pos].second;
  }
};

template <class TObject>
auto FindByPos(const std::vector<std::shared_ptr<TObject>>& objs,
               const Pos& pos) {
  return std::find_if(
      objs.begin(), objs.end(),
      [pos](const std::shared_ptr<TObject>& obj) { return obj->pos == pos; });
}

template <class TObject>
auto FindByPos(const std::vector<std::shared_ptr<TObject>>& objs, int x,
               int y) {
  Pos pos(x, y);
  return FindById(objs, pos);
}

class Entity;

class BaseObject {
 public:
  bool is_to_destroy = false;
  Pos pos;
  chartype pic;

  virtual ~BaseObject() = default;

  virtual bool IsToDestroy() { return is_to_destroy; }

  virtual void Configure(const Config& config){};

  virtual void OnCollide(const std::shared_ptr<Entity>& entity){};

  virtual std::wstring Description() { return L"BaseObject"; }
};

class Entity : public BaseObject {
 public:
  int health{};
  int max_health{};
  int damage{};

  bool is_player = false;

  void MoveUp() { pos = pos.Up(); }
  void MoveDown() { pos = pos.Down(); }
  void MoveLeft() { pos = pos.Left(); }
  void MoveRight() { pos = pos.Right(); }

  void SetEntity(int hp, int dmg) {
    SetHealth(hp);
    damage = dmg;
  }

  void SetHealth(int hp_) { max_health = health = hp_; }

  virtual void TakeDamage(const std::shared_ptr<Entity>& entity) {
    health -= entity->damage;
  }

  virtual void Attack(const std::shared_ptr<Entity>& entity) {
    entity->health -= damage;
  }

  bool IsToDestroy() override { return health <= 0; }
};