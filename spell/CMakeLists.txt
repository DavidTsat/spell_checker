add_library(spell
    src/algorithm.cpp
    src/text_utils.cpp
)

target_include_directories(spell
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(spell PUBLIC cxx_std_20)