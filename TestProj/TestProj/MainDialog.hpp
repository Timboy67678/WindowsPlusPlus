#pragma once

#include "stdafx.h"

class MainDialog : public dialog {
public:
    MainDialog(HINSTANCE hInstance) : dialog(hInstance, IDD_MAINDLG, IDC_TESTPROJ) {
    }

    message_handler on_init_dialog;

private:
    control_ptr<combo_box> m_combo;
    control_ptr<button> m_dostuff;
    control_ptr<check_box> m_check;
    control_ptr<list_box> m_list;
    control_ptr<rich_edit_text> m_richedit;
    control_ptr<edit_text> m_spinedit;
    control_ptr<scroll_bar> m_scroll;
    control_ptr<list_view> m_view;
    control_ptr<tree_view> m_tree;
    control_ptr<track_bar> m_track;
    control_ptr<up_down_control> m_spin;
    control_ptr<progress_bar> m_progress;
    control_ptr<tab_control> m_tab;
};
