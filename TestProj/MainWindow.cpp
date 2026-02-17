#include "MainWindow.hpp"

constexpr auto DEFAULT_WINDOW_WIDTH = 1000;
constexpr auto DEFAULT_WINDOW_HEIGHT = 700;

MainWindow::MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance)
    : window(window_class{ _T("MainWindowWPP"), instance },
             window_title, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {
    set_keep_minimum_resize(true);
}

LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    center_window();

    auto main_dock = std::make_shared<wpp::layout::dock_panel>(hWnd);
    main_dock->set_padding(10);
    m_root_panel = main_dock;

    // top panel with title and buttons

    auto top_panel = std::make_shared<wpp::layout::stack_panel>(wpp::layout::orientation::horizontal, hWnd);
    top_panel->set_spacing(10);
    top_panel->set_padding(5);

    auto title_label = create_static_control(_T("WindowsPlusPlus Control Showcase"), 400, 30);
    top_panel->add(title_label);

    auto btn_refresh = create_button(_T("Refresh"), 80, 25);
    btn_refresh->on_click([this](WPARAM, LPARAM) {
        MessageBox(get_handle(), _T("Refresh clicked!"), _T("Info"), MB_OK | MB_ICONINFORMATION);
    });
    top_panel->add(btn_refresh);

    auto btn_settings = create_button(_T("Settings"), 80, 25);
    top_panel->add(btn_settings);

    auto btn_about = create_button(_T("About"), 80, 25);
    btn_about->on_click([this](WPARAM, LPARAM) {
        MessageBox(get_handle(), _T("WindowsPlusPlus v1.0\nA modern C++ wrapper for Windows UI"), _T("About"), MB_OK | MB_ICONINFORMATION);
    });
    top_panel->add(btn_about);

    main_dock->add_panel(top_panel);
    main_dock->set_dock_position(top_panel, wpp::layout::dock_position::top);

    // bottom panel with status and link

    auto bottom_panel = std::make_shared<wpp::layout::stack_panel>(wpp::layout::orientation::horizontal, hWnd);
    bottom_panel->set_spacing(10);
    bottom_panel->set_padding(5);

    auto status_label = create_static_control(_T("Ready"), 200, 20);
    bottom_panel->add(status_label);

    m_LinkControl = create_link_control(_T("<a href=\"https://github.com/Timboy67678/WindowsPlusPlus\">GitHub</a>"), 300, 20);
    m_LinkControl->on_click([this](LPNMHDR nm) {
        auto item = reinterpret_cast<PNMLINK>(nm)->item;
        ShellExecuteW(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOWNORMAL);
    });
    bottom_panel->add(m_LinkControl);

    main_dock->add_panel(bottom_panel);
    main_dock->set_dock_position(bottom_panel, wpp::layout::dock_position::bottom);

    auto left_panel = std::make_shared<wpp::layout::stack_panel>(wpp::layout::orientation::vertical, hWnd);
    left_panel->set_spacing(8);
    left_panel->set_padding(10);

    auto btn_section_label = create_static_control(_T("Button Controls:"), 200, 20);
    left_panel->add(btn_section_label);

    m_ButtonOne = create_button(_T("Click Me!"), 200, 30);
    m_ButtonOne->on_click([this](WPARAM, LPARAM) {
        static int x = 0;
        tstring button_counter_str = TEXT("Clicked: ") + to_tstring(++x) + TEXT(" times");
        m_ButtonOne->set_text(button_counter_str);
    });
    left_panel->add(m_ButtonOne);

    m_CheckBoxOne = create_check_box(_T("Enable Shield Icon"), 200, 25);
    m_CheckBoxOne->on_click([this](WPARAM, LPARAM) {
        m_ButtonOne->set_shield(m_CheckBoxOne->get_checked() == BST_CHECKED);
    });
    left_panel->add(m_CheckBoxOne);

    auto checkbox2 = create_check_box(_T("Check Box 2"), 200, 25);
    left_panel->add(checkbox2);

    auto radio_label = create_static_control(_T("Radio Group 1:"), 200, 20);
    left_panel->add(radio_label);

    m_RadioButtonGroup = create_radio_button_group();
    auto radiobuttonone = m_RadioButtonGroup->create_button(_T("Option A"), 200, 25, TRUE);
    left_panel->add(radiobuttonone);

    auto radiobuttontwo = m_RadioButtonGroup->create_button(_T("Option B"), 200, 25);
    left_panel->add(radiobuttontwo);

    auto radiobuttonthree = m_RadioButtonGroup->create_button(_T("Option C"), 200, 25);
    left_panel->add(radiobuttonthree);

    auto radio2_label = create_static_control(_T("Radio Group 2:"), 200, 20);
    left_panel->add(radio2_label);

    auto radio_grptwo = create_radio_button_group();
    auto radio_one = radio_grptwo->create_button(_T("Choice 1"), 200, 25, TRUE);
    left_panel->add(radio_one);

    auto radio_two = radio_grptwo->create_button(_T("Choice 2"), 200, 25);
    left_panel->add(radio_two);

    auto radio_three = radio_grptwo->create_button(_T("Choice 3"), 200, 25);
    left_panel->add(radio_three);

    auto combo_label = create_static_control(_T("Combo Box:"), 200, 20);
    left_panel->add(combo_label);

    m_ComboBoxOne = create_combo_box(200, 100);
    m_ComboBoxOne->add(_T("Windows"));
    m_ComboBoxOne->add(_T("Linux"));
    m_ComboBoxOne->add(_T("macOS"));
    m_ComboBoxOne->add(_T("FreeBSD"));
    left_panel->add(m_ComboBoxOne);

    auto edit_label = create_static_control(_T("Text Input:"), 200, 20);
    left_panel->add(edit_label);

    m_EditTextOne = create_edit_text(_T("Type here..."), 200, 25);
    left_panel->add(m_EditTextOne);

    auto progress_label = create_static_control(_T("Progress Bar:"), 200, 20);
    left_panel->add(progress_label);

    auto progress_bar = create_progress_bar(200, 20);
    progress_bar->set_range(0, 100);
    progress_bar->set_pos(65);
    left_panel->add(progress_bar);

    main_dock->add_panel(left_panel);
    main_dock->set_dock_position(left_panel, wpp::layout::dock_position::left);

    // right-side panel with list view and tree view

    auto right_panel = std::make_shared<wpp::layout::stack_panel>(wpp::layout::orientation::vertical, hWnd);
    right_panel->set_spacing(8);
    right_panel->set_padding(10);

    // List View
    auto listview_label = create_static_control(_T("List View:"), 250, 20);
    right_panel->add(listview_label);

    m_ListViewOne = create_list_view(250, 200);
    m_ListViewOne->add_column(_T("Name"), 100);
    m_ListViewOne->add_column(_T("Type"), 80);
    m_ListViewOne->add_column(_T("Size"), 70);

    m_ListViewOne->add_item(0, 0, _T("Document.txt"));
    m_ListViewOne->add_item(0, 1, _T("Text File"));
    m_ListViewOne->add_item(0, 2, _T("5 KB"));

    m_ListViewOne->add_item(1, 0, _T("Image.png"));
    m_ListViewOne->add_item(1, 1, _T("Image"));
    m_ListViewOne->add_item(1, 2, _T("1.2 MB"));

    m_ListViewOne->add_item(2, 0, _T("Video.mp4"));
    m_ListViewOne->add_item(2, 1, _T("Video"));
    m_ListViewOne->add_item(2, 2, _T("45 MB"));

    m_ListViewOne->auto_size_columns();

    right_panel->add(m_ListViewOne);

    auto treeview_label = create_static_control(_T("Tree View:"), 250, 20);
    right_panel->add(treeview_label);

    auto tree_view = create_tree_view(250, 200);
    auto root = tree_view->insert_item(_T("Root"), TVI_ROOT, TVI_LAST);
    auto folder1 = tree_view->insert_item(_T("Folder 1"), root, TVI_LAST);
    tree_view->insert_item(_T("Item 1.1"), folder1, TVI_LAST);
    tree_view->insert_item(_T("Item 1.2"), folder1, TVI_LAST);
    auto folder2 = tree_view->insert_item(_T("Folder 2"), root, TVI_LAST);
    tree_view->insert_item(_T("Item 2.1"), folder2, TVI_LAST);
    tree_view->insert_item(_T("Item 2.2"), folder2, TVI_LAST);
    tree_view->expand(root);
    right_panel->add(tree_view);

    main_dock->add_panel(right_panel);
    main_dock->set_dock_position(right_panel, wpp::layout::dock_position::right);


    //center'd panel with tab control, list box, and rich edit
    auto center_panel = std::make_shared<wpp::layout::stack_panel>(wpp::layout::orientation::vertical, hWnd);
    center_panel->set_spacing(10);
    center_panel->set_padding(15);

    auto tab_label = create_static_control(_T("Tab Control:"), 400, 25);
    center_panel->add(tab_label);

    auto tab_control = create_tab_control(400, 250);
    tab_control->add_item(_T("Welcome"));
    tab_control->add_item(_T("Details"));
    tab_control->add_item(_T("Settings"));
    center_panel->add(tab_control);

    auto listbox_label = create_static_control(_T("List Box:"), 400, 20);
    center_panel->add(listbox_label);

    auto list_box = create_list_box(400, 100);
    list_box->add(_T("Item Alpha"));
    list_box->add(_T("Item Beta"));
    list_box->add(_T("Item Gamma"));
    list_box->add(_T("Item Delta"));
    list_box->add(_T("Item Epsilon"));
    center_panel->add(list_box);

    auto richedit_label = create_static_control(_T("Rich Edit Control:"), 400, 20);
    center_panel->add(richedit_label);

    auto rich_edit = create_rich_edit(_T("This is a rich edit control.\nYou can type multiple lines here.\nTry editing this text!"), 400, 100);
    center_panel->add(rich_edit);

    main_dock->add_panel(center_panel);
    main_dock->set_dock_position(center_panel, wpp::layout::dock_position::fill);

    RECT client_rect;
    GetClientRect(hWnd, &client_rect);
    main_dock->measure(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
    main_dock->arrange(0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

    return window::on_create(hWnd, wParam, lParam);
}
