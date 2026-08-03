add_executable(spell_lib_tests
    test.cpp
)

target_link_libraries(spell_lib_tests
    PRIVATE
        spell
)

add_test(
    NAME spell_lib_tests
    COMMAND spell_lib_tests
)

target_compile_options(spell_lib_tests PRIVATE
    -g3
    -O0
)