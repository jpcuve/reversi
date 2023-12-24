#include <iostream>

#include "BoardWndClass.h"
#include "Game.h"
#include "MessageWndClass.h"
#include "ReversiWndClass.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    std::cout << "Starting application" << std::endl;
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) return 0;

    ReversiWndClass reversi_wnd_class{hInstance};
    BoardWndClass board_wnd_class{hInstance};
    MessageWndClass message_wnd_class{hInstance};

    std::cout << "Creating window" << std::endl;
    Game game;
    for (const auto offset: game.FindPossibleMoves(TOKEN_WHITE)) game.SetToken(game.GetPosition(offset), TOKEN_TEST);
    std::cout << game << std::endl;
    const auto hWnd = CreateWindowW(
            ReversiWndClass::class_name_,
            L"Reversi",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            hInstance,
            &game);
    if (!hWnd) return 0;

    std::cout << "Showing window" << std::endl;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}
