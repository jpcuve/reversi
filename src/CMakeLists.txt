add_executable(reversi WIN32
        main.cpp
        Game.cpp
        Game.h
        Position.h
        Position.cpp
        main.cpp
        TestWindow.cpp
        TestWindow.h
        MainWindow.cpp
        MainWindow.h
        BoardWindow.cpp
        BoardWindow.h
        Window.h
        Window.cpp
        main.h
)

set_target_properties(reversi PROPERTIES LINK_FLAGS resources/resources.res)

add_dependencies(reversi resources)

target_precompile_headers(reversi PRIVATE pch.h)
