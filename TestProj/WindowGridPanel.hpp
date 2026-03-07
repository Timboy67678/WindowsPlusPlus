#pragma once

#include "stdafx.h"

class WindowGridPanel : public window {
public:
    WindowGridPanel(LPCTSTR window_title, int x, int y, HINSTANCE instance = NULL);
    virtual ~WindowGridPanel() = default;

protected:
    message_handler on_create;
    message_handler on_size;

private:
    std::shared_ptr<layout::grid_panel> create_main_grid(HWND hWnd);
    std::shared_ptr<layout::grid_panel> create_header_panel(HWND hWnd, const std::shared_ptr<layout::grid_panel>& main_grid);
    std::shared_ptr<layout::grid_panel> create_form_panel(HWND hWnd);
    std::shared_ptr<layout::grid_panel> create_inventory_panel(HWND hWnd);
    std::shared_ptr<layout::stack_panel> create_activity_panel(HWND hWnd);
    std::shared_ptr<layout::grid_panel> create_showcase_panel(HWND hWnd);
    std::shared_ptr<layout::stack_panel> create_footer_panel(HWND hWnd);
    void arrange_main_grid(const std::shared_ptr<layout::grid_panel>& main_grid);

    // Control references for demo
    control_ptr<button> m_IncrementBtn;
    control_ptr<static_control> m_CounterLabel;
    control_ptr<tab_control> m_TabControl;
    control_ptr<list_view> m_ListViewOne;
    control_ptr<edit_text> m_EditOne;
    control_ptr<edit_text> m_EditTwo;
    control_ptr<edit_text> m_EditThree;

    int m_counter = 0;
};
