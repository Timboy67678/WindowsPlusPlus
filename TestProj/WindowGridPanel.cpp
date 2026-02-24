#include "WindowGridPanel.hpp"

constexpr auto DEFAULT_WINDOW_WIDTH = 900;
constexpr auto DEFAULT_WINDOW_HEIGHT = 650;

WindowGridPanel::WindowGridPanel(LPCTSTR window_title, int x, int y, HINSTANCE instance)
    : window(window_class{ _T("WindowGridPanelWPP"), instance },
             window_title, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {
    set_keep_minimum_resize(true);
}

LRESULT WindowGridPanel::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    center_window();

    // Create a cleaner, simpler grid demo with 4 rows and 3 columns
    auto main_grid = std::make_shared<layout::grid_panel>(hWnd);
    main_grid->set_padding(15);
    main_grid->set_spacing(10);

    // Define rows: header (50px), content (1*), controls (1*), footer (40px)
    main_grid->add_row_definition(layout::grid_length::pixels(50));
    main_grid->add_row_definition(layout::grid_length::star(2.0));
    main_grid->add_row_definition(layout::grid_length::star(1.0));
    main_grid->add_row_definition(layout::grid_length::pixels(40));

    // Define columns: 3 equal star-sized columns
    main_grid->add_column_definition(layout::grid_length::star(1.0));
    main_grid->add_column_definition(layout::grid_length::star(1.0));
    main_grid->add_column_definition(layout::grid_length::star(1.0));

    // ===== ROW 0: Header (spans all 3 columns) =====
    auto header = std::make_shared<layout::stack_panel>(layout::orientation::horizontal, hWnd);
    header->set_spacing(15);
    header->set_padding(8);

    auto title = create_static_control(_T("Grid Panel Layout Demo"), 300, 30);
    header->add(title);

    auto btn_about = create_button(_T("About"), 100, 30);
    btn_about->on_click([this](WPARAM, LPARAM) {
        message_box_info(_T("About"), 
            _T("Grid Panel Demo\n\n")
            _T("Features:\n")
            _T(" Fixed & Star sizing\n")
            _T(" Column/Row spanning\n")
            _T(" Nested layouts\n")
            _T(" Dynamic resizing"));
    });
    header->add(btn_about);

    main_grid->add_panel(header);
    main_grid->set_grid_position(header, 0, 0, 1, 3);

    // ===== ROW 1, COL 0: Form inputs =====
    auto form_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    form_panel->set_spacing(10);
    form_panel->set_padding(10);

    auto form_title = create_static_control(_T("User Information"), 250, 25);
    form_panel->add(form_title);

    auto name_label = create_static_control(_T("Name:"), 250, 20);
    form_panel->add(name_label);

    m_EditOne = create_edit_text(_T(""), 250, 25);
    form_panel->add(m_EditOne);

    auto email_label = create_static_control(_T("Email:"), 250, 20);
    form_panel->add(email_label);

    m_EditTwo = create_edit_text(_T(""), 250, 25);
    form_panel->add(m_EditTwo);

    auto phone_label = create_static_control(_T("Phone:"), 250, 20);
    form_panel->add(phone_label);

    m_EditThree = create_edit_text(_T(""), 250, 25);
    form_panel->add(m_EditThree);

    main_grid->add_panel(form_panel);
    main_grid->set_grid_position(form_panel, 1, 0);

    // ===== ROW 1, COL 1-2: List View (spans 2 columns) =====
    auto listview_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    listview_panel->set_spacing(5);
    listview_panel->set_padding(10);

    auto list_title = create_static_control(_T("Product List (spans 2 columns)"), 400, 25);
    listview_panel->add(list_title);

    m_ListViewOne = create_list_view(500, 200);
    m_ListViewOne->add_column(_T("ID"), 60);
    m_ListViewOne->add_column(_T("Product"), 180);
    m_ListViewOne->add_column(_T("Category"), 120);
    m_ListViewOne->add_column(_T("Price"), 80);

    m_ListViewOne->add_item(0, 0, _T("001"));
    m_ListViewOne->add_item(0, 1, _T("Laptop"));
    m_ListViewOne->add_item(0, 2, _T("Electronics"));
    m_ListViewOne->add_item(0, 3, _T("$999"));

    m_ListViewOne->add_item(1, 0, _T("002"));
    m_ListViewOne->add_item(1, 1, _T("Mouse"));
    m_ListViewOne->add_item(1, 2, _T("Accessories"));
    m_ListViewOne->add_item(1, 3, _T("$29"));

    m_ListViewOne->add_item(2, 0, _T("003"));
    m_ListViewOne->add_item(2, 1, _T("Keyboard"));
    m_ListViewOne->add_item(2, 2, _T("Accessories"));
    m_ListViewOne->add_item(2, 3, _T("$79"));

    m_ListViewOne->add_item(3, 0, _T("004"));
    m_ListViewOne->add_item(3, 1, _T("Monitor"));
    m_ListViewOne->add_item(3, 2, _T("Electronics"));
    m_ListViewOne->add_item(3, 3, _T("$349"));

    listview_panel->add(m_ListViewOne);

    main_grid->add_panel(listview_panel);
    main_grid->set_grid_position(listview_panel, 1, 1, 1, 2);

    // ===== ROW 2, COL 0: Counter Demo =====
    auto counter_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    counter_panel->set_spacing(10);
    counter_panel->set_padding(10);

    auto counter_title = create_static_control(_T("Counter Demo"), 250, 25);
    counter_panel->add(counter_title);

    m_CounterLabel = create_static_control(_T("Count: 0"), 250, 30);
    counter_panel->add(m_CounterLabel);

    m_IncrementBtn = create_button(_T("Increment"), 250, 35);
    m_IncrementBtn->on_click([this](WPARAM, LPARAM) {
        m_counter++;
        m_CounterLabel->set_text(TEXT("Count: ") + to_tstring(m_counter));
    });
    counter_panel->add(m_IncrementBtn);

    auto reset_btn = create_button(_T("Reset"), 250, 35);
    reset_btn->on_click([this](WPARAM, LPARAM) {
        m_counter = 0;
        m_CounterLabel->set_text(_T("Count: 0"));
    });
    counter_panel->add(reset_btn);

    main_grid->add_panel(counter_panel);
    main_grid->set_grid_position(counter_panel, 2, 0);

    // ===== ROW 2, COL 1: Action Buttons =====
    auto button_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    button_panel->set_spacing(10);
    button_panel->set_padding(10);

    auto btn_title = create_static_control(_T("Actions"), 250, 25);
    button_panel->add(btn_title);

    auto btn_save = create_button(_T("Save"), 250, 35);
    button_panel->add(btn_save);

    auto btn_load = create_button(_T("Load"), 250, 35);
    button_panel->add(btn_load);

    auto btn_refresh = create_button(_T("Refresh"), 250, 35);
    btn_refresh->on_click([this](WPARAM, LPARAM) {
        message_box_info(_T("Info"), _T("Data refreshed!"));
    });
    button_panel->add(btn_refresh);

    main_grid->add_panel(button_panel);
    main_grid->set_grid_position(button_panel, 2, 1);

    // ===== ROW 2, COL 2: Options =====
    auto options_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    options_panel->set_spacing(10);
    options_panel->set_padding(10);

    auto options_title = create_static_control(_T("Options"), 250, 25);
    options_panel->add(options_title);

    auto check1 = create_check_box(_T("Enable grid lines drawing"), 250, 25);
    check1->on_click([this, check1, main_grid](WPARAM, LPARAM) {
        main_grid->paint_grid_lines() = check1->is_checked();
        invalidate();
        update_window();
    });
    options_panel->add(check1);

    auto check2 = create_check_box(_T("Enable feature B"), 250, 25);
    options_panel->add(check2);

    auto check3 = create_check_box(_T("Auto-save"), 250, 25);
    options_panel->add(check3);

    main_grid->add_panel(options_panel);
    main_grid->set_grid_position(options_panel, 2, 2);

    // ===== ROW 3: Footer (spans all 3 columns) =====
    auto footer = std::make_shared<layout::stack_panel>(layout::orientation::horizontal, hWnd);
    footer->set_spacing(10);
    footer->set_padding(5);

    auto status = create_static_control(_T("Ready | Grid Layout Demo"), 300, 30);
    footer->add(status);

    auto link = create_link_control(_T("<a href=\"https://github.com/Timboy67678/WindowsPlusPlus\">WindowsPlusPlus on GitHub</a>"), 350, 30);
    link->on_click([this](LPNMHDR nm) {
        auto item = reinterpret_cast<PNMLINK>(nm)->item;
        ShellExecuteW(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOWNORMAL);
    });
    footer->set_margin(15, 15, 0, 0);
    footer->add(link);

    main_grid->add_panel(footer);
    main_grid->set_grid_position(footer, 3, 0, 1, 3);

    // Initial layout
    RECT client_rect = get_client_rect();
    main_grid->measure(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
    main_grid->arrange(0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

    root_panel() = main_grid;

    return window::on_create(hWnd, wParam, lParam);
}

