#include <iostream>

#include "board_window.h"
#include "Game.h"
#include "main_window.h"
#include "resource.h"
#include "test_window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    std::cout << "Starting application" << std::endl;
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) return 0;

    TestWndClass test_wnd_class {hInstance};
    MainWndClass main_wnd_class {hInstance};
    BoardWndClass board_wnd_class {hInstance};

    std::cout << "Creating window" << std::endl;
    Game game;
    std::cout << game << std::endl;
    game.ComputeFollowers(TOKEN_WHITE);
    for (const auto& g: game.GetFollowers()) std::cout << g << std::endl;
    game.ComputeFollowers(TOKEN_BLACK);
    for (const auto& g: game.GetFollowers()) std::cout << g << std::endl;
    const auto main_window_handle { MainWndClass::AddWindow(hInstance, &game)};
    const auto board_window_handle = BoardWndClass::AddWindow(main_window_handle, reinterpret_cast<HMENU>(ID_BOARD_WINDOW), &game);
    if (!board_window_handle) throw std::runtime_error("Cannot create board window");
    ShowWindow(board_window_handle, SW_SHOWNORMAL);
    const auto test_window_handle = TestWndClass::AddWindow(main_window_handle, reinterpret_cast<HMENU>(ID_TEST_WINDOW));
    if (!test_window_handle) throw std::runtime_error("Cannot create test window");

    ShowWindow(test_window_handle, SW_SHOWNORMAL);

    std::cout << "Showing window" << std::endl;
    ShowWindow(main_window_handle, nCmdShow);
    UpdateWindow(main_window_handle);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}
