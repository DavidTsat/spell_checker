add_library(spell_app_lib
    spell_checker.cpp
)

target_include_directories(spell_app_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spell_app_lib
    PUBLIC
        spell
)


add_executable(spell_app
    main.cpp
)

target_link_libraries(spell_app
    PRIVATE
        spell_app_lib
)

target_compile_options(spell_app_lib PRIVATE
    -g3
    -O0
)