#pragma once

#include "stdafx.h"

class MainWindow : public window {
public:
    MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance = NULL);

    message_handler on_create;

private:
    control_ptr<button> m_ButtonOne;
    control_ptr<check_box> m_CheckBoxOne;
    control_ptr<combo_box> m_ComboBoxOne;
    control_ptr<edit_text> m_EditTextOne;
    control_ptr<list_view> m_ListViewOne;
    control_ptr<window::radio_button_group> m_RadioButtonGroup;
    control_ptr<sys_link> m_LinkControl;
};

