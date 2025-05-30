cmake_minimum_required(VERSION 3.15...4.0)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc -O2")
add_subdirectory(Lua)


add_executable(test src/test.cpp)
target_link_libraries(test lua_static)

add_executable(player_test src/player_test.cpp)
target_link_libraries(player_test lua_static)