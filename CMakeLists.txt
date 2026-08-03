cmake_minimum_required(VERSION 3.20)

project(spell_checker
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(spell)
add_subdirectory(app)

enable_testing()
add_subdirectory(test)