cmake_minimum_required(VERSION 3.20)
project(retro_fortress_2_goldsrc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(rf2_common STATIC
    src/common/class_profiles.cpp
    src/common/goldsrc_stub.cpp
)

target_include_directories(rf2_common PUBLIC include)
set_target_properties(rf2_common PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(rf2_server SHARED
    src/server/game_manager.cpp
    src/server/logging.cpp
    src/server/mod.cpp
    src/server/player_class.cpp
    src/server/team.cpp
)

target_link_libraries(rf2_server PRIVATE rf2_common)

target_include_directories(rf2_server PUBLIC include)

set_target_properties(rf2_server PROPERTIES
    OUTPUT_NAME "retrofortress2"
)

add_library(rf2_client SHARED
    src/client/client_module.cpp
)

target_link_libraries(rf2_client PRIVATE rf2_common)

target_include_directories(rf2_client PUBLIC include)

set_target_properties(rf2_client PROPERTIES
    OUTPUT_NAME "cl_retrofortress2"
)

