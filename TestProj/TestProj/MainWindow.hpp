#pragma once

#include "stdafx.h"

class MainWindow : public window {
public:
    MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance = NULL);

    message_handler on_create;

private:
    std::shared_ptr<button> m_ButtonOne;
    std::shared_ptr<check_box> m_CheckBoxOne;
    std::shared_ptr<combo_box> m_ComboBoxOne;
    std::shared_ptr<edit_text> m_EditTextOne;
    std::shared_ptr<list_view> m_ListViewOne;
    std::shared_ptr<window::radio_button_group> m_RadioButtonGroup;
    std::shared_ptr<sys_link> m_LinkControl;
};

