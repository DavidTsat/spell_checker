add_library(spell
    src/algorithm.cpp
)

target_include_directories(spell
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(spell PUBLIC cxx_std_20)

target_compile_options(spell PRIVATE
    -g3
    -O0
)