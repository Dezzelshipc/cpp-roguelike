CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
project(Roguelike)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncursesw")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include_directories(logic)
include_directories(logic/objects)

add_executable(${PROJECT_NAME}
        main.cpp

        logic/objects/Object.h
        logic/objects/Block.h
        logic/objects/Player.h
        logic/objects/Enemy.h
        logic/objects/Decoration.h

        logic/Collider.h
        logic/Game.h
        logic/Render.h
        logic/Utility.h
        logic/Config.h
        logic/objects/TimedObjects.h logic/objects/Weapon.h)


