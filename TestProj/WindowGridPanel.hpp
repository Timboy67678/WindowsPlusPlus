#pragma once

#include "stdafx.h"

class WindowGridPanel : public window {
public:
    WindowGridPanel(LPCTSTR window_title, int x, int y, HINSTANCE instance);
    virtual ~WindowGridPanel() = default;

protected:
    message_handler on_create;

private:
    // Control references for demo
    control_ptr<button> m_IncrementBtn;
    control_ptr<static_control> m_CounterLabel;
    control_ptr<list_view> m_ListViewOne;
    control_ptr<edit_text> m_EditOne;
    control_ptr<edit_text> m_EditTwo;
    control_ptr<edit_text> m_EditThree;

    int m_counter = 0;
};
