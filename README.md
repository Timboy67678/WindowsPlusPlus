# WindowsPlusPlus
Library for object oriented dialogs and windows with Win32 API

A modern C++ wrapper for Win32 GUI development that provides object-oriented abstractions for dialogs, windows, and controls.

## Features

- **Object-Oriented Design**: Clean C++ wrapper over Win32 API
- **Rich Control Support**: 20+ built-in control types (buttons, lists, trees, etc.)
- **Message Handling**: Virtual message handlers mapped to class methods
- **Type Safety**: Modern C++ with templates and smart pointers
- **Layout Manager** (In Development): XAML-inspired automatic layout system

## Documentation

- **[Layout Manager Design Plan](docs/LayoutManager_DesignPlan.md)** - New layout system design (in review)
- **[Layout Manager Quick Start](docs/LayoutManager_QuickStart.md)** - Quick reference guide
- **[Documentation Index](docs/README.md)** - All documentation

## Quick Example

```cpp
class MainWindow : public wpp::window {
public:
    MainWindow(LPCTSTR title, int x, int y, HINSTANCE instance)
        : window(window_class{_T("MainWindowWPP"), instance},
                 title, x, y, 800, 600, WS_OVERLAPPEDWINDOW) {}
    
    LRESULT on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) override {
        m_button = create_button(_T("Click Me!"), 10, 10, 150, 30);
        m_button->on_click([this](WPARAM, LPARAM) {
            message_box_info(_T("Hello!"), _T("Button clicked!"));
        });
        return window::on_create(hWnd, wParam, lParam);
    }
    
private:
    control_ptr<button> m_button;
};
```

## Credits

Big thanks to:
- DarthTon's Xenos Injector (https://github.com/DarthTon/Xenos) for his pseudo win32++ impl, helped a ton (especially mapping message values to appropriate callbacks with help from his bound member thunking)
- The creator of WTL (http://wtl.sourceforge.net/) for his functions that I started implementing but half way through just ended up copying his impl
