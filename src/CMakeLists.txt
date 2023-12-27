add_executable(reversi WIN32
        main.cpp
        Game.cpp
        Game.h
        Position.h
        Position.cpp
        main.cpp
        test_window.cpp
        test_window.h
        main_window.cpp
        main_window.h
        board_window.cpp
        board_window.h
)

set_target_properties(reversi PROPERTIES LINK_FLAGS resources/resources.res)

add_dependencies(reversi resources)

target_precompile_headers(reversi PRIVATE pch.h)
