add_executable(spell_tests
    test.cpp
)

target_link_libraries(spell_tests PRIVATE spell)

target_compile_options(spell_tests PRIVATE
    -g3
    -O0
)