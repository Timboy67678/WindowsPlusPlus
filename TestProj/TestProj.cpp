#include "stdafx.h"
#include "resource.h"
#include "MainDialog.hpp"
#include "MainWindow.hpp"
#include "WindowGridPanel.hpp"

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow) {
    INITCOMMONCONTROLSEX iccex = { 
        sizeof(INITCOMMONCONTROLSEX), 
        ICC_WIN95_CLASSES | 
        ICC_TAB_CLASSES |
        ICC_LISTVIEW_CLASSES |
        ICC_PROGRESS_CLASS | ICC_UPDOWN_CLASS |
        ICC_STANDARD_CLASSES |
        ICC_DATE_CLASSES |
        ICC_TREEVIEW_CLASSES |
        ICC_HOTKEY_CLASS |
        ICC_ANIMATE_CLASS |
        ICC_COOL_CLASSES |
        ICC_USEREX_CLASSES | 
        ICC_INTERNET_CLASSES | 
        ICC_PAGESCROLLER_CLASS |
        ICC_NATIVEFNTCTL_CLASS 
    };
    InitCommonControlsEx(&iccex);
    if (::GetModuleHandle(TEXT("Riched20.dll")) == NULL) //control's only works with richedit2.0 dll loaded
        ::LoadLibrary(TEXT("Riched20.dll"));

    //auto mainwindow = std::make_unique<MainWindow>(TEXT("Test Window"), 0, 0, hInstance);
    auto grid_window = std::make_unique<WindowGridPanel>(TEXT("Grid Panel Demo"), 0, 0, hInstance);
    //auto dialog = std::make_unique<MainDialog>(hInstance);

    //if (!mainwindow->create_window()) {
    //    ::MessageBox(NULL, format_tstring(TEXT("Failed to create main window: 0x{:X}"), ::GetLastError()).c_str(), TEXT("Error"), MB_OK | MB_ICONERROR);
    //    return -1;
    //}

    if (!grid_window->create_window()) {
        ::MessageBox(NULL, format_tstring(TEXT("Failed to create grid window: 0x{:X}"), ::GetLastError()).c_str(), TEXT("Error"), MB_OK | MB_ICONERROR);
        return -1;
    }

    //if (!dialog->create_modeless()) {
    //    ::MessageBox(NULL, format_tstring(TEXT("Failed to create dialog: 0x{:X}"), ::GetLastError()).c_str(), TEXT("Error"), MB_OK | MB_ICONERROR);
    //    return -1;
    //}

    message_loop loop;
    //loop.register_window(*mainwindow);
    loop.register_window(*grid_window);
    //loop.register_window(*dialog);
    loop.run();

    return 0;
}
