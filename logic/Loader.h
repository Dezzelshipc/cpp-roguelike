#pragma once

#include <filesystem>
#include <fstream>
#include <string>

class Loader {
  std::string config =
      "size_x= 120\n"
      "size_y= 40\n"
      "\n"
      "window_x= 51\n"
      "window_y= 21\n"
      "window_offset_x= 15\n"
      "window_offset_y= 2\n"
      "\n"
      "enemy_hp= 10\n"
      "enemy_radius= 10\n"
      "enemy_moves= 2\n"
      "enemy_damage= 4\n"
      "\n"
      "player_hp= 20\n"
      "player_damage= 4\n"
      "player_arrows= 20\n"
      "player_bombs= 1\n"
      "\n"
      "health_potion= 5\n"
      "quiver_capacity= 20\n"
      "bomb_radius= 4\n"
      "\n"
      "weapon_duration= 100\n"
      "weapon_multiplier= 2";

  std::string gameover =
      "                      /$$$$$$\n"
      "                     /$$__  $$\n"
      "                    | $$  \\__/  /$$$$$$  /$$$$$$/$$$$   /$$$$$$\n"
      "                    | $$ /$$$$ |____  $$| $$_  $$_  $$ /$$__  $$\n"
      "                    | $$|_  $$  /$$$$$$$| $$ \\ $$ \\ $$| $$$$$$$$\n"
      "                    | $$  \\ $$ /$$__  $$| $$ | $$ | $$| $$_____/\n"
      "                    |  $$$$$$/|  $$$$$$$| $$ | $$ | $$|  $$$$$$$\n"
      "                     \\______/  \\_______/|__/ |__/ |__/ \\_______/\n"
      "\n"
      "\n"
      "\n"
      "                          /$$$$$$\n"
      "                         /$$__  $$\n"
      "                        | $$  \\ $$ /$$    /$$ /$$$$$$   /$$$$$$\n"
      "                        | $$  | $$|  $$  /$$//$$__  $$ /$$__  $$\n"
      "                        | $$  | $$ \\  $$/$$/| $$$$$$$$| $$  \\__/\n"
      "                        | $$  | $$  \\  $$$/ | $$_____/| $$\n"
      "                        |  $$$$$$/   \\  $/  |  $$$$$$$| $$\n"
      "                         \\______/     \\_/    \\_______/|__/";

  std::string map =
      "      W          W                  wh  r     t             W\n"
      "  @   d          W                  w  qr                   W\n"
      "      W        E d  h               w o r                   W\n"
      "WwwwWWW          W           ,      rrrrr                  WWW\n"
      "    q W          W   ,              r                    a WWW\n"
      " b    wwwwWWWWWWWW          ,       r           ,          WWW\n"
      "  b   W                 ,           r                       W           "
      "        WWWWWWW\n"
      "WWWWWWW       ,                     r     ,     e           WW          "
      "        Wh a SW\n"
      "                ,                  rr                       "
      "WWWWWWWWWWWWWWWWWWWWWWWdWWWWW\n"
      "   ,                   rrrrrrdddrrrrr                       W           "
      "                W\n"
      "          ,            rr                        ,       a WWW          "
      "                WWWWWWWWWWWWWWWWWwwwwwwww\n"
      "WWWW            ,      rr                                a WWWeaaaaa    "
      "              a W               W       w\n"
      "bq w                   r         ,                          d           "
      "                W               W       w\n"
      "bT d s      ,          r                e                  td           "
      "               Td              Bd a  P  w\n"
      "bq w             ,     r    rrr                       ,     d           "
      "                W               W       w\n"
      "WWWW     ,           rrr   rrrrrrr                       a WWWeaaaaa    "
      "              a W               W       w\n"
      "              ,    rrrrr     rrrrrr   ,                  a WWW          "
      "                WWWWWWWWWWWWWWWWWwwwwwwww\n"
      "rrrrrrrr  rrrrrrrrrrrrrr       rrr              e           W           "
      "                W\n"
      "                     rrr                                    "
      "WWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n"
      "  ,       ,           rr     ,               ,              WW\n"
      "                 ,     r                              ,     W\n"
      "                       r                                   WWW\n"
      "          ,            r                 ,               a WWW\n"
      "   ,                   rr     ,                            WWW\n"
      "                       rr              ,                    W\n"
      "                   ,   rr                                   W\n"
      "                       rrrrrrrrrrrrrrrrrrrrrrrrrrrrWWWwWWWWWW\n"
      "         ,                                       rrW    W\n"
      "                                                   W O  W\n"
      "                        ,                          W hh W\n"
      "                                             ,     WWWWWW\n"
      "                                                   W\n"
      "                                                   W\n"
      "                                      ,            W\n"
      "                                                   W\n"
      "                                                   W\n"
      "            ,                 ,                    W\n"
      "                                                   W";

  std::string start =
      "\n"
      " /$$   /$$             /$$            /$$$$$$        /$$$$$$$\n"
      "| $$$ | $$            | $$           /$$__  $$      | $$__  $$\n"
      "| $$$$| $$  /$$$$$$  /$$$$$$        | $$  \\ $$      | $$  \\ $$  "
      "/$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$\n"
      "| $$ $$ $$ /$$__  $$|_  $$_/        | $$$$$$$$      | $$$$$$$/ /$$__  "
      "$$ /$$__  $$| $$  | $$ /$$__  $$\n"
      "| $$  $$$$| $$  \\ $$  | $$          | $$__  $$      | $$__  $$| $$  \\ "
      "$$| $$  \\ $$| $$  | $$| $$$$$$$$\n"
      "| $$\\  $$$| $$  | $$  | $$ /$$      | $$  | $$      | $$  \\ $$| $$  | "
      "$$| $$  | $$| $$  | $$| $$_____/\n"
      "| $$ \\  $$|  $$$$$$/  |  $$$$/      | $$  | $$      | $$  | $$|  "
      "$$$$$$/|  $$$$$$$|  $$$$$$/|  $$$$$$$\n"
      "|__/  \\__/ \\______/    \\___/        |__/  |__/      |__/  |__/ "
      "\\______/  \\____  $$ \\______/  \\_______/\n"
      "                                                                        "
      " /$$  \\ $$\n"
      "                                                                        "
      "|  $$$$$$/\n"
      "                                                                        "
      " \\______/\n"
      "\n"
      "                                    Press any key to Start\n"
      "\n"
      "                                  Find Princess in a Castle!";

  std::string win =
      "\n"
      "\n"
      "\n"
      "\n"
      "             /$$     /$$  /$$$$$$  /$$   /$$       /$$      /$$ /$$$$$$ "
      "/$$   /$$\n"
      "            |  $$   /$$/ /$$__  $$| $$  | $$      | $$  /$ | $$|_  "
      "$$_/| $$$ | $$\n"
      "             \\  $$ /$$/ | $$  \\ $$| $$  | $$      | $$ /$$$| $$  | $$ "
      " | $$$$| $$\n"
      "              \\  $$$$/  | $$  | $$| $$  | $$      | $$/$$ $$ $$  | $$  "
      "| $$ $$ $$\n"
      "               \\  $$/   | $$  | $$| $$  | $$      | $$$$_  $$$$  | $$  "
      "| $$  $$$$\n"
      "                | $$    | $$  | $$| $$  | $$      | $$$/ \\  $$$  | $$  "
      "| $$\\  $$$\n"
      "                | $$    |  $$$$$$/|  $$$$$$/      | $$/   \\  $$ "
      "/$$$$$$| $$ \\  $$\n"
      "                |__/     \\______/  \\______/       |__/     "
      "\\__/|______/|__/  \\__/";

 public:
  bool static IsExists(const char* path) {
    return std::filesystem::exists(path);
  }

  void static TryCreate(const std::string& path, const std::string& file) {
    if (!IsExists(path.c_str())) {
      std::ofstream of(path);
      of.write(file.c_str(), file.size());
      of.close();
    }
  }

  Loader() {
    if (!IsExists("./configs")) {
      std::filesystem::create_directories("./configs");
    }
    TryCreate("./configs/config.txt", config);
    TryCreate("./configs/gameover.txt", gameover);
    TryCreate("./configs/map.txt", map);
    TryCreate("./configs/start.txt", start);
    TryCreate("./configs/win.txt", win);
  }
};
