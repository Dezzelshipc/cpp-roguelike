#pragma once

#include <fstream>
#include <string>

class Config {
 public:
  Config(const Config& config) = default;

  Config() {
    std::ifstream conf("./configs/config.txt");
    std::string row;

    while (!conf.eof()) {
      conf >> row;
      if (row == "size_x=")
        conf >> size_x;
      else if (row == "size_y=")
        conf >> size_y;
      else if (row == "enemy_hp=")
        conf >> enemy_hp;
      else if (row == "enemy_moves=")
        conf >> enemy_moves;
      else if (row == "enemy_radius=")
        conf >> enemy_radius;
      else if (row == "player_hp=")
        conf >> player_hp;
      else if (row == "enemy_damage=")
        conf >> enemy_damage;
      else if (row == "player_damage=")
        conf >> player_damage;
      else if (row == "health_potion=")
        conf >> health_potion;
      else if (row == "window_x=")
        conf >> window_x;
      else if (row == "window_y=")
        conf >> window_y;
      else if (row == "window_offset_x=")
        conf >> window_offset_x;
      else if (row == "window_offset_y=")
        conf >> window_offset_y;
      else if (row == "weapon_duration=")
        conf >> weapon_duration;
      else if (row == "weapon_multiplier=")
        conf >> weapon_multiplier;
      else if (row == "player_arrows=")
        conf >> player_arrows;
      else if (row == "quiver_capacity=")
        conf >> quiver_capacity;
      else if (row == "player_bombs=")
        conf >> player_bombs;
      else if (row == "bomb_radius=")
        conf >> bomb_radius;
    }

    conf.close();
  }

  int size_x{};
  int size_y{};

  int window_x{};
  int window_y{};
  int window_offset_x{};
  int window_offset_y{};

  int enemy_hp{};
  int enemy_radius{};
  int enemy_moves{};
  int enemy_damage{};

  int player_hp{};
  int player_damage{};
  int player_arrows{};
  int player_bombs{};

  int health_potion{};
  int quiver_capacity{};
  int bomb_radius{};

  int weapon_duration{};
  int weapon_multiplier{};
};