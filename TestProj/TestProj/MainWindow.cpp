#include "MainWindow.hpp"

constexpr auto DEFAULT_WINDOW_WIDTH = 800;
constexpr auto DEFAULT_WINDOW_HEIGHT = 600;

constexpr auto COLUMN_ONE_OFFSET = 10;

MainWindow::MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance)
    : window(window_class{ _T("MainWindowWPP"), instance },
             window_title, x, y, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX)) {

}

LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    center_window();

    m_ButtonOne = create_button(_T("Click Me!"), COLUMN_ONE_OFFSET, 0, 150, 25);
    m_ButtonOne->on_click([this](WPARAM, LPARAM) {
        static int x = 0;
        std::tstring button_counter_str = TEXT("Button Clicked: ") + std::to_tstring(++x);
        m_ButtonOne->set_text(button_counter_str);
    });

    m_CheckBoxOne = create_check_box(_T("Check Me!"), COLUMN_ONE_OFFSET, 30, 150, 25);
    m_CheckBoxOne->on_click([this](WPARAM, LPARAM) {
        m_ButtonOne->set_shield(m_CheckBoxOne->get_checked() == BST_CHECKED);
    });

    m_ComboBoxOne = create_combo_box(COLUMN_ONE_OFFSET, 60, 150, 25);
    m_EditTextOne = create_edit_text(COLUMN_ONE_OFFSET, 90, 150, 25, _T("Edit me!"));
    m_ListViewOne = create_list_view(COLUMN_ONE_OFFSET, 120, 350, 150);

    m_RadioButtonGroup = create_radio_button_group();
    auto radiobuttonone = m_RadioButtonGroup->create_button(_T("Radio 1"), COLUMN_ONE_OFFSET, 270, 150, 25, TRUE);
    auto radiobuttontwo = m_RadioButtonGroup->create_button(_T("Radio 2"), COLUMN_ONE_OFFSET, 295, 150, 25);
    auto radiobuttonthree = m_RadioButtonGroup->create_button(_T("Radio 3"), COLUMN_ONE_OFFSET, 320, 150, 25);

    auto radio_grptwo = create_radio_button_group();
    auto radio_one = radio_grptwo->create_button(_T("Radio 2 1"), COLUMN_ONE_OFFSET, 345, 150, 25, TRUE);
    auto radio_two = radio_grptwo->create_button(_T("Radio 2 2"), COLUMN_ONE_OFFSET, 370, 150, 25);
    auto radio_three = radio_grptwo->create_button(_T("Radio 2 3"), COLUMN_ONE_OFFSET, 395, 150, 25);

    m_LinkControl = create_link_control(_T("<a href=\"https://www.google.com\">Click me!</a>, or better yet, <a href=\"https://facebook.com\">Click Me!</a>"), COLUMN_ONE_OFFSET, 450, 250, 25);
    m_LinkControl->on_click([this](LPNMHDR nm) {
        auto item = reinterpret_cast<PNMLINK>(nm)->item;
        ShellExecuteW(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOWNORMAL);
    });

    m_ComboBoxOne->add(_T("Item 1"));
    m_ComboBoxOne->add(_T("Item 2"));
    m_ComboBoxOne->add(_T("Item 3"));
    m_ComboBoxOne->add(_T("Item 4"));

    m_ListViewOne->add_column(_T("Column 1"), 100);
    m_ListViewOne->add_column(_T("Column 2"), 100);
    m_ListViewOne->add_column(_T("Column 3"), 100);

    m_ListViewOne->add_item(0, 0, _T("Item 1"));
    m_ListViewOne->add_item(0, 1, _T("Column Item 1"));
    m_ListViewOne->add_item(0, 2, _T("Column Item 2"));

    return window::on_create(hWnd, wParam, lParam);
}
