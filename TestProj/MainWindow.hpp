#pragma once

#include "stdafx.h"

class MainWindow : public window {
public:
    MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance = NULL);

    message_handler on_create;

private:
    std::shared_ptr<layout::stack_panel> create_top_panel(HWND hWnd);
    std::shared_ptr<layout::stack_panel> create_bottom_panel(HWND hWnd);
    std::shared_ptr<layout::stack_panel> create_left_panel(HWND hWnd);
    std::shared_ptr<layout::stack_panel> create_right_panel(HWND hWnd);
    std::shared_ptr<layout::stack_panel> create_center_panel(HWND hWnd);

    control_ptr<button> m_ButtonOne;
    control_ptr<check_box> m_CheckBoxOne;
    control_ptr<combo_box> m_ComboBoxOne;
    control_ptr<edit_text> m_EditTextOne;
    control_ptr<list_view> m_ListViewOne;
    control_ptr<window::radio_button_group> m_RadioButtonGroup;
    control_ptr<sys_link> m_LinkControl;
};

