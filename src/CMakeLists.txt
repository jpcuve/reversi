add_executable(reversi WIN32
        main.cpp
        game.cpp
        game.h
        BoardWndClass.h
        BoardWndClass.cpp
        BoardWndClass.cpp
        ReversiWndClass.h
        ReversiWndClass.cpp
)

set_target_properties(reversi PROPERTIES LINK_FLAGS resources/resources.res)

add_dependencies(reversi resources)

target_precompile_headers(reversi PRIVATE pch.h)
