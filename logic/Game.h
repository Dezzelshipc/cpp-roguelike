#pragma once
#include <fstream>
#include <set>

#include "Block.h"
#include "Collider.h"
#include "Config.h"
#include "Decoration.h"
#include "Enemy.h"
#include "Loader.h"
#include "Object.h"
#include "Player.h"
#include "Render.h"
#include "TimedObjects.h"
#include "Utility.h"

class Game {
  enum Progress { CONTINUE, WIN, GAME_OVER };
  enum InputAction { UPDATE, NOT_UPDATE };
  std::vector<std::shared_ptr<Object>> objects;

  std::vector<std::shared_ptr<Block>> blocks;
  std::vector<std::shared_ptr<Enemy>> enemies;
  std::vector<std::shared_ptr<Decoration>> decorations;
  bool show_decorations = true;
  std::shared_ptr<std::vector<std::shared_ptr<TimedObject>>> timed_objects{};

  std::shared_ptr<Player> player;
  std::shared_ptr<Princess> princess;

  Loader loader;
  Render render;
  Collider collider;
  const Config config;

  int move_count = 0;
  std::shared_ptr<std::vector<std::wstring>> happen_on_turn{};

  template <class TObject>
  std::shared_ptr<TObject> AddTo(int& x, int& y,
                                 std::vector<std::shared_ptr<Enemy>>& vec) {
    TObject obj;
    obj.pos = obj.center = {x, y};
    obj.Configure(config);
    obj.Configure(timed_objects);
    std::shared_ptr<TObject> sharedPtr = std::make_shared<TObject>(obj);
    vec.push_back(sharedPtr);
    return sharedPtr;
  }

  template <class TObject, class TVector>
  std::shared_ptr<TObject> AddTo(int& x, int& y,
                                 std::vector<std::shared_ptr<TVector>>& vec) {
    TObject obj;
    obj.pos = {x, y};
    std::shared_ptr<TObject> sharedPtr = std::make_shared<TObject>(obj);
    vec.push_back(sharedPtr);
    return sharedPtr;
  }

  void AddObject(std::string& str, int x, int y) {
    if (str == "@") {
      Player pl;
      pl.pos = {x, y};
      pl.Configure(config);
      player = std::make_shared<Player>(pl);
    } else if (str == "P") {
      Princess pr;
      pr.pos = {x, y};
      princess = std::make_shared<Princess>(pr);
    } else if (str == "W")
      AddTo<Wall>(x, y, blocks);
    else if (str == ",")
      AddTo<Grass>(x, y, decorations);
    else if (str == "e" || str == "E")
      AddTo<Enemy>(x, y, enemies);
    else if (str == "b")
      AddTo<BombObject>(x, y, objects);
    else if (str == "d")
      AddTo<Door>(x, y, blocks);
    else if (str == "a")
      AddTo<Archer>(x, y, enemies);
    else if (str == "t" || str == "T")
      AddTo<Tank>(x, y, enemies);
    else if (str == "h")
      AddTo<HealthPotion>(x, y, objects)->Configure(config);
    else if (str == "s")
      AddTo<SwordWeapon>(x, y, objects);
    else if (str == "S")
      AddTo<BigSwordWeapon>(x, y, objects);
    else if (str == "o")
      AddTo<BowWeapon>(x, y, objects);
    else if (str == "O")
      AddTo<LongBowWeapon>(x, y, objects);
    else if (str == "B")
      AddTo<Boss>(x, y, enemies);
    else if (str == "q")
      AddTo<Quiver>(x, y, objects);
    else if (str == "w")
      AddTo<WeakWall>(x, y, blocks);
    else if (str == "r")
      AddTo<Water>(x, y, blocks);
  }

  template <class TObject>
  bool MoveAction(TObject& obj, Pos to_pos) {
    if (collider.Collide(obj, to_pos, enemies) == Action::STAY) return false;
    if (collider.Collide(obj, to_pos, blocks) == Action::STAY) return false;
    if (collider.Collide(obj, to_pos, objects) == Action::STAY) return false;
    return true;
  }

 public:
  Game() {
    timed_objects =
        std::make_shared<std::vector<std::shared_ptr<TimedObject>>>();
    happen_on_turn = std::make_shared<std::vector<std::wstring>>();
    collider.story = happen_on_turn;

    player = std::make_shared<Player>();
    player->pos = {-2, -2};
    princess = std::make_shared<Princess>();
    princess->pos = {-2, 2};

    std::fstream map("./configs/map.txt");

    std::string row;

    int x = 0, y = 0;

    while (!map.eof()) {
      getline(map, row);
      for (auto c : row) {
        std::string st;
        st += c;
        AddObject(st, x, y);
        ++x;
      }
      x = 0;
      ++y;
    }

    map.close();
  }

  InputAction Input() {
    int c = getch();
    switch (c) {
      case 'w':
        if (player->pos.y > 0) {
          if (MoveAction(player, player->pos.Up())) {
            player->MoveUp();
          }
        }
        break;
      case 's':
        if (player->pos.y < config.size_y) {
          if (MoveAction(player, player->pos.Down())) {
            player->MoveDown();
          }
        }
        break;
      case 'a':
        if (player->pos.x > 0) {
          if (MoveAction(player, player->pos.Left())) {
            player->MoveLeft();
          }
        }
        break;
      case 'd':
        if (player->pos.x < config.size_x) {
          if (MoveAction(player, player->pos.Right())) {
            player->MoveRight();
          }
        }
        break;
      case 'i':
        player->CreateArrow(UP, timed_objects);
        break;
      case 'k':
        player->CreateArrow(DOWN, timed_objects);
        break;
      case 'j':
        player->CreateArrow(LEFT, timed_objects);
        break;
      case 'l':
        player->CreateArrow(RIGHT, timed_objects);
        break;
      case 'e':
        player->CreateBomb(timed_objects, config);
        break;
      case 't':
        show_decorations = !show_decorations;
        return NOT_UPDATE;
      default:
        break;
    }
    return UPDATE;
  }

  void EnemyAction(std::shared_ptr<Enemy>& enemy) {
    Action act = enemy->MakeAction(*player);
    switch (act) {
      case MOVE: {
        std::set<Pos> poses;
        for (auto& e : enemies) {
          if (e != enemy) poses.insert(e->pos);
        }
        for (auto& b : blocks) {
          if (b->is_solid) poses.insert(b->pos);
        }

        Direction dir =
            enemy->pos.FindPath(player->pos, enemy->radius, poses, config);

        switch (dir) {
          case UP:
            enemy->MoveUp();
            break;
          case DOWN:
            enemy->MoveDown();
            break;
          case LEFT:
            enemy->MoveLeft();
            break;
          case RIGHT:
            enemy->MoveRight();
            break;
          case NONE:
            enemy->RandomMove(blocks, config);
            break;
        }
        break;
      }
      case ATTACK:
        enemy->Attack(player);
        happen_on_turn->emplace_back(
            L"Player took " + std::to_wstring(enemy->damage) + L" damage");
        break;
      case RANDOM_MOVE:
        enemy->RandomMove(blocks, config);
        break;
      default:
        break;
    }
  }

  template <class TObject>
  void DestroyObjects(std::vector<std::shared_ptr<TObject>>& objs) {
    std::erase_if(objs, [&](const auto& item) { return item->IsToDestroy(); });
  }

  Progress Update() {
    ++move_count;
    if (player->pos == princess->pos) return WIN;

    player->Update();

    std::vector<std::shared_ptr<Entity>> entities(enemies.begin(),
                                                  enemies.end());
    entities.insert(entities.end(), player);

    auto cur_tos = *timed_objects;

    for (auto& to : cur_tos) {
      to->MakeAction(entities, blocks, *timed_objects, *happen_on_turn);
    }

    for (auto& enemy : enemies) {
      EnemyAction(enemy);
    }

    if (player->health <= 0) {
      return GAME_OVER;
    }
    DestroyObjects(blocks);
    DestroyObjects(enemies);
    DestroyObjects(*timed_objects);
    DestroyObjects(objects);

    return CONTINUE;
  };

  void Draw() {
    render.ClrAll();

    render.CreateBox();

    render.SetCenter(player->pos);

    if (show_decorations) render.FullDraw(decorations);
    render.FullDraw(objects);
    render.FullDraw(blocks);
    render.FullDraw(enemies);
    render.FullDraw(*timed_objects);

    render.Draw(player);
    render.Draw(princess);

    render.DrawInfo(player, move_count);
    render.DrawLegend();
    render.DrawHappened(happen_on_turn);
    render.DrawControls();

    render.RefAll();
  }

  void Start() {
    Render::DrawScreen("./configs/start.txt");
    Render::Ref();
    getch();

    render.Configure(config);
    render.CreateWindow();

    Progress progress = CONTINUE;
    InputAction inputAction = UPDATE;
    while (progress == CONTINUE) {
      Draw();
      inputAction = Input();
      if (inputAction == UPDATE) progress = Update();
    }

    Render::Clr();
    if (progress == GAME_OVER) {
      Render::DrawScreen("./configs/gameover.txt");
    }
    if (progress == WIN) {
      Render::DrawScreen("./configs/win.txt");
    }
    Render::DrawInfo(player, move_count);
    Render::Ref();
    getch();
  }
};
