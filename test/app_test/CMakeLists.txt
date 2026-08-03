add_executable(spell_app_tests
    test.cpp
)

target_link_libraries(spell_app_tests
    PRIVATE
        spell_app_lib
)

add_test(
    NAME spell_app_tests
    COMMAND spell_app_tests
)

target_compile_options(spell_app_tests PRIVATE
    -g3
    -O0
)