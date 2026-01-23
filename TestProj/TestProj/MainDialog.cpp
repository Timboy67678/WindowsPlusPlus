#include "stdafx.h"
#include "MainDialog.hpp"
#include <memory>

#define MAX_RANGES 0, 100
#define MAX_TAB_ITEMS 7
#define MAX_VISIBLE_ITEMS 8
#define SYSTEM32_PATH TEXT("C:\\Windows\\System32\\*.exe")

INT_PTR MainDialog::on_init_dialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    register_control(IDC_TEST_CHECK, m_check);
    register_control(IDC_LISTBOX_TEST, m_list);
    register_control(IDC_OK_BTN, m_dostuff);
    register_control(IDC_COMB_TEST, m_combo);
    register_control(IDC_RICHEDIT_TEST, m_richedit);
    register_control(IDC_EDIT_SPIN, m_spinedit);
    register_control(IDC_SCROLLBAR_TEST, m_scroll);
    register_control(IDC_LISTVIEW_TEST, m_view);
    register_control(IDC_TREE_TEST, m_tree);
    register_control(IDC_SLIDER_TEST, m_track);
    register_control(IDC_SPIN_TEST, m_spin);
    register_control(IDC_PROGRESS_TEST, m_progress);
    register_control(IDC_TAB_TEST, m_tab);

    register_menu_command(IDM_EXIT, [this](WPARAM, LPARAM) {
        end_dialog();
    });

    register_menu_command(IDM_ABOUT, [this](WPARAM, LPARAM) {
        message_box_info(TEXT("TestProj"), TEXT("TestProj.exe - Test project for use in WinPlusPlus!"));
    });

    m_dostuff->on_click([this](WPARAM, LPARAM) {
        m_list->reset_content();
        m_combo->reset_content();
        m_tree->delete_all_items();

        if (m_tab->get_item_count() < MAX_TAB_ITEMS)
            m_tab->add_item((TEXT("Tab ") + std::to_tstring(m_tab->get_item_count() + 1)).c_str());

        m_list->add_dir(0, SYSTEM32_PATH);
        m_combo->add_dir(0, SYSTEM32_PATH);

        m_combo->set_min_visible(MAX_VISIBLE_ITEMS);

        HTREEITEM trees[MAX_VISIBLE_ITEMS] = { 0 };
        HTREEITEM tree = m_tree->insert_item(TEXT("Main Item"), NULL, NULL);

        for (int i = 0; i < ARRAYSIZE(trees); i++)
            trees[i] = m_tree->insert_item(TEXT("Sub Item"), i == 0 ? tree : trees[i - 1], NULL);

        for (int i = ARRAYSIZE(trees) - 1; i >= 0; i--)
            m_tree->expand(trees[i]);

        m_tree->expand(tree);
    });

    m_check->on_click([this](WPARAM, LPARAM) {
        m_dostuff->set_shield(m_check->get_checked() == BST_CHECKED);
    });

    m_spin->register_notify_callback(UDN_DELTAPOS, [this](LPNMHDR nm) {
        auto updn = reinterpret_cast<LPNMUPDOWN>(nm);
        int minimum, maximum, new_val = updn->iPos + updn->iDelta;
        m_spin->get_range32(minimum, maximum);

        if (new_val >= minimum && new_val <= maximum) {
            m_progress->set_pos(new_val);
            m_track->set_pos(new_val);
            m_scroll->set_scroll_pos(new_val);
            m_spinedit->set_text(std::to_tstring(new_val));
            m_richedit->append_text((TEXT("Delta is ") + std::to_tstring(updn->iDelta) + TEXT("\n")).c_str());
        }
    });

    m_track->register_notify_callback(TRBN_THUMBPOSCHANGING, [this](LPNMHDR nm) {
        auto tbm = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(nm);

        int minimum, maximum, new_val = tbm->dwPos;
        m_track->get_range(minimum, maximum);

        if (new_val >= minimum && new_val <= maximum) {
            m_progress->set_pos(new_val);
            m_spin->set_pos(new_val);
            m_scroll->set_scroll_pos(new_val);
            m_spinedit->set_text(std::to_tstring(new_val));
        }
    });

    m_combo->set_banner_text(L"ComboBox control item test");

    if (!(m_track->get_style() & TBS_NOTIFYBEFOREMOVE))
        m_track->add_style(TBS_NOTIFYBEFOREMOVE);

    m_spin->set_buddy(m_spinedit->get_handle());
    m_spinedit->set_text_limit(4);

    int nMin, nMax;

    m_progress->set_range32(MAX_RANGES);
    m_progress->get_range(nMin, nMax);

    m_track->set_range(nMin, nMax);
    m_spin->set_range(nMin, nMax);
    m_scroll->set_scroll_range(nMin, nMax);

    return dialog::on_init_dialog(hWnd, wParam, lParam);
}
