#pragma once
#include <locale>
#include <memory>
#include <vector>

#include "Player.h"
#include "Utility.h"

class Render {
  WINDOW* window{};
  Pos center;

  Config config;

  std::map<std::wstring, std::wstring> legend{};

 public:
  Render() {
    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);
    refresh();
  }

  ~Render() {
    getch();
    endwin();
  }

  void Configure(const Config& config_) { config = config_; }

  void CreateWindow() {
    window = newwin(config.window_y, config.window_x, config.window_offset_y,
                    config.window_offset_x);
  }

  void CreateBox() { box(window, 0, 0); }

  void static Clr() { erase(); }
  void ClrAll() {
    Clr();
    werase(window);
  }

  void static Ref() { refresh(); }
  void RefAll() {
    Ref();
    wrefresh(window);
  }

  void Draw(const std::shared_ptr<BaseObject>& object) {
    if (0 < object->pos.y - center.y &&
        object->pos.y - center.y < config.window_y - 1 &&
        0 < object->pos.x - center.x &&
        object->pos.x - center.x < config.window_x - 1) {
      mvwaddwstr(window, object->pos.y - center.y, object->pos.x - center.x,
                 object->pic.c_str());
      legend.insert({object->Description(), object->pic});
    }
  }
  template <class TObject>
  void FullDraw(std::vector<std::shared_ptr<TObject>>& objects) {
    for (std::shared_ptr<TObject>& obj : objects) {
      Draw(obj);
    }
  }

  void SetCenter(const Pos& pos) {
    if (pos.x < config.window_x / 2)
      center.x = -1;
    else if (pos.x > config.size_x - config.window_x / 2 + 1)
      center.x = config.size_x - config.window_x + 2;
    else
      center.x = pos.x - config.window_x / 2;

    if (pos.y < config.window_y / 2)
      center.y = -1;
    else if (pos.y > config.size_y - config.window_y / 2 + 1)
      center.y = config.size_y - config.window_y + 2;
    else
      center.y = pos.y - config.window_y / 2;
  }

  void static DrawScreen(const std::string& path) {
    std::ifstream fin(path);

    std::string row;
    int y = 0;
    while (!fin.eof()) {
      getline(fin, row);
      mvaddstr(y, 0, row.c_str());
      ++y;
    }

    fin.close();
  }

  template <class... Args>
  void static Print(int& x, const char* str, Args... args) {
    mvprintw(x++, 0, str, args...);
  }

  void static DrawInfo(const std::shared_ptr<Player>& player,
                       const int move_count) {
    int x = 0;

    Print(x, "Move: %d", move_count);
    Print(x, "HP: %d", player->health);

    Print(x, "Sword dmg: %d",
          player->damage_default * player->sword.multiplier);
    if (player->sword.max_moves > 0) {
      Print(x, "  Moves: %d", player->sword.max_moves - player->sword.move);
    }

    Print(x, "Bow dmg: %d", player->damage_default * player->bow.multiplier);
    if (player->bow.max_moves > 0) {
      Print(x, "  Moves: %d", player->bow.max_moves - player->bow.move);
    }
    Print(x, "Arrows: %d", player->arrows);
    Print(x, "Bombs: %d", player->bombs);
  }

  void DrawLegend() {
    int y = 0;

    for (auto& item : legend) {
      auto str = item.second + L" " + item.first;
      mvaddwstr(y++, config.window_x + config.window_offset_x + 3, str.c_str());
    }

    legend.clear();
  }

  void DrawHappened(
      const std::shared_ptr<std::vector<std::wstring>>& story) const {
    int y = config.window_offset_y + config.window_y + 2;
    for (auto& str : *story) mvaddwstr(y++, 0, str.c_str());

    story->clear();
  }

  void DrawControls() const {
    int y = config.window_offset_y + config.window_y + 2,
        x = config.window_offset_x + config.window_x - 10;

    mvaddwstr(y + 0, x, L"Move     Fire   Place     Render");
    mvaddwstr(y + 1, x, L"  w       i     Bomb   Decorations");
    mvaddwstr(y + 2, x, L"a s d   j k l     e         t");
  }
};
