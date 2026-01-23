#pragma once

#include "stdafx.h"

class MainDialog : public dialog {
public:
    MainDialog(HINSTANCE hInstance) : dialog(hInstance, IDD_MAINDLG, IDC_TESTPROJ) {
    }

    message_handler on_init_dialog;

private:
    std::shared_ptr<combo_box> m_combo;
    std::shared_ptr<button> m_dostuff;
    std::shared_ptr<check_box> m_check;
    std::shared_ptr<list_box> m_list;
    std::shared_ptr<rich_edit_text> m_richedit;
    std::shared_ptr<edit_text> m_spinedit;
    std::shared_ptr<scroll_bar> m_scroll;
    std::shared_ptr<list_view> m_view;
    std::shared_ptr<tree_view> m_tree;
    std::shared_ptr<track_bar> m_track;
    std::shared_ptr<up_down_control> m_spin;
    std::shared_ptr<progress_bar> m_progress;
    std::shared_ptr<tab_control> m_tab;
};
