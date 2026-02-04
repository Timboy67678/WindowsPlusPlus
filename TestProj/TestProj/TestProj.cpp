#include "stdafx.h"
#include "resource.h"
#include "MainDialog.hpp"
#include "MainWindow.hpp"

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow) {
    InitCommonControls();
    if (::GetModuleHandle(TEXT("Riched20.dll")) == NULL) //control's only works with richedit2.0 dll loaded
        ::LoadLibrary(TEXT("Riched20.dll"));

    auto mainwindow = std::make_unique<MainWindow>(TEXT("Test Window"), 0, 0, hInstance);
    auto dialog = std::make_unique<MainDialog>(hInstance);
    mainwindow->create_window();
    dialog->create_modeless();

    message_loop loop;
    loop.register_window(*mainwindow);
    loop.register_window(*dialog);
    loop.run();

    return 0;
}
