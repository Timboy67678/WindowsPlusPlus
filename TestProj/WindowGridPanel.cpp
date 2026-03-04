#include "WindowGridPanel.hpp"

constexpr auto DEFAULT_WINDOW_WIDTH = 1200;
constexpr auto DEFAULT_WINDOW_HEIGHT = 780;

WindowGridPanel::WindowGridPanel(LPCTSTR window_title, int x, int y, HINSTANCE instance)
    : window(window_class(_T("WindowGridPanelWPP"), instance),
             window_title, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {
    set_keep_minimum_resize(true);
}

LRESULT WindowGridPanel::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    center_window();
    auto main_grid = std::make_shared<layout::grid_panel>(hWnd);
    main_grid->set_padding(14);
    main_grid->set_spacing(10);

    // Main dashboard layout: mixed fixed and star tracks
    main_grid->add_row_definition(layout::grid_length::pixels(64));
    main_grid->add_row_definition(layout::grid_length::star(2.0));
    main_grid->add_row_definition(layout::grid_length::pixels(210));
    main_grid->add_row_definition(layout::grid_length::pixels(42));

    main_grid->add_column_definition(layout::grid_length::star(1.2));
    main_grid->add_column_definition(layout::grid_length::star(2.2));
    main_grid->add_column_definition(layout::grid_length::star(1.2));

    // Header (row span 1, column span 3)
    auto header = std::make_shared<layout::grid_panel>(hWnd);
    header->set_padding(8);
    header->set_spacing(8);
    header->add_row_definition(layout::grid_length::star(1));
    header->add_column_definition(layout::grid_length::star(1.0));
    header->add_column_definition(layout::grid_length::pixels(140));
    header->add_column_definition(layout::grid_length::pixels(140));

    auto title = create_static_control(_T("WindowsPlusPlus Grid Dashboard"), 450, 28);
    header->add(title, 0, 0);

    auto btn_grid_lines = create_check_box(_T("Show Grid Lines"), 130, 28);
    btn_grid_lines->on_click([this, btn_grid_lines, main_grid](WPARAM, LPARAM) {
        main_grid->paint_grid_lines() = btn_grid_lines->is_checked();
        invalidate();
        update_window();
    });
    header->add(btn_grid_lines, 0, 1);
    header->set_alignment(btn_grid_lines, layout::alignment::center, layout::alignment::center);

    auto btn_about = create_button(_T("About Demo"), 130, 30);
    btn_about->on_click([this](WPARAM, LPARAM) {
        message_box_info(
            _T("Grid Features"),
            _T("This demo highlights:\n")
            _T(" - Mixed pixel and star row/column sizing\n")
            _T(" - Row/column spanning\n")
            _T(" - Nested grid and stack panels\n")
            _T(" - Per-control alignment inside cells")
        );
    });
    header->add(btn_about, 0, 2);
    header->set_alignment(btn_about, layout::alignment::center, layout::alignment::center);

    main_grid->add_panel(header);
    main_grid->set_grid_position(header, 0, 0, 1, 3);

    // Left card: form built with grid panel (labels + inputs)
    auto form_grid = std::make_shared<layout::grid_panel>(hWnd);
    form_grid->set_padding(10);
    form_grid->set_spacing(8);

    form_grid->add_row_definition(layout::grid_length::pixels(28));
    form_grid->add_row_definition(layout::grid_length::pixels(28));
    form_grid->add_row_definition(layout::grid_length::pixels(28));
    form_grid->add_row_definition(layout::grid_length::pixels(28));
    form_grid->add_row_definition(layout::grid_length::pixels(28));
    form_grid->add_row_definition(layout::grid_length::star(1));
    form_grid->add_row_definition(layout::grid_length::pixels(34));

    form_grid->add_column_definition(layout::grid_length::pixels(80));
    form_grid->add_column_definition(layout::grid_length::star(1));

    auto form_title = create_static_control(_T("Customer Profile"), 240, 26);
    form_grid->add(form_title, 0, 0, 1, 2);

    auto name_label = create_static_control(_T("Name"), 75, 24);
    form_grid->add(name_label, 1, 0);
    form_grid->set_alignment(name_label, layout::alignment::end, layout::alignment::center);
    m_EditOne = create_edit_text(_T("Ada Lovelace"), 220, 24);
    form_grid->add(m_EditOne, 1, 1);

    auto email_label = create_static_control(_T("Email"), 75, 24);
    form_grid->add(email_label, 2, 0);
    form_grid->set_alignment(email_label, layout::alignment::end, layout::alignment::center);
    m_EditTwo = create_edit_text(_T("ada@analytical.engine"), 220, 24);
    form_grid->add(m_EditTwo, 2, 1);

    auto phone_label = create_static_control(_T("Phone"), 75, 24);
    form_grid->add(phone_label, 3, 0);
    form_grid->set_alignment(phone_label, layout::alignment::end, layout::alignment::center);
    m_EditThree = create_edit_text(_T("+44 20 7946 0958"), 220, 24);
    form_grid->add(m_EditThree, 3, 1);

    auto notes_label = create_static_control(_T("Tier"), 75, 24);
    form_grid->add(notes_label, 4, 0);
    form_grid->set_alignment(notes_label, layout::alignment::end, layout::alignment::center);
    auto tier_combo = create_combo_box(220, 120);
    tier_combo->add(_T("Standard"));
    tier_combo->add(_T("Premium"));
    tier_combo->add(_T("Enterprise"));
    form_grid->add(tier_combo, 4, 1);

    auto save_profile = create_button(_T("Save Profile"), 140, 32);
    form_grid->add(save_profile, 6, 0, 1, 2);
    form_grid->set_alignment(save_profile, layout::alignment::center, layout::alignment::center);

    main_grid->add_panel(form_grid);
    main_grid->set_grid_position(form_grid, 1, 0);

    // Center card: inventory list with toolbar, demonstrating span and nested layout
    auto inventory_grid = std::make_shared<layout::grid_panel>(hWnd);
    inventory_grid->set_padding(10);
    inventory_grid->set_spacing(8);
    inventory_grid->add_row_definition(layout::grid_length::pixels(30));
    inventory_grid->add_row_definition(layout::grid_length::star(1));
    inventory_grid->add_column_definition(layout::grid_length::star(1));
    inventory_grid->add_column_definition(layout::grid_length::pixels(220));

    auto inventory_title = create_static_control(_T("Inventory Overview"), 260, 26);
    inventory_grid->add(inventory_title, 0, 0);
    inventory_grid->set_alignment(inventory_title, layout::alignment::start, layout::alignment::center);

    auto toolbar = std::make_shared<layout::stack_panel>(layout::orientation::horizontal, hWnd);
    toolbar->set_spacing(8);
    auto btn_sync = create_button(_T("Sync"), 70, 26);
    auto btn_export = create_button(_T("Export"), 70, 26);
    auto btn_refresh = create_button(_T("Refresh"), 70, 26);
    btn_refresh->on_click([this](WPARAM, LPARAM) {
        message_box_info(_T("Refreshed"), _T("Inventory data refreshed."));
    });
    toolbar->add(btn_sync);
    toolbar->add(btn_export);
    toolbar->add(btn_refresh);
    inventory_grid->add_panel(toolbar);
    inventory_grid->set_grid_position(toolbar, 0, 1);

    m_ListViewOne = create_list_view(620, 250);
    m_ListViewOne->add_column(_T("SKU"), 90);
    m_ListViewOne->add_column(_T("Product"), 170);
    m_ListViewOne->add_column(_T("Category"), 120);
    m_ListViewOne->add_column(_T("Stock"), 80);
    m_ListViewOne->add_column(_T("Price"), 90);

    m_ListViewOne->add_item(0, 0, _T("LT-100"));
    m_ListViewOne->add_item(0, 1, _T("Laptop Pro 14"));
    m_ListViewOne->add_item(0, 2, _T("Computers"));
    m_ListViewOne->add_item(0, 3, _T("42"));
    m_ListViewOne->add_item(0, 4, _T("$1299"));

    m_ListViewOne->add_item(1, 0, _T("MN-210"));
    m_ListViewOne->add_item(1, 1, _T("32in Monitor"));
    m_ListViewOne->add_item(1, 2, _T("Displays"));
    m_ListViewOne->add_item(1, 3, _T("18"));
    m_ListViewOne->add_item(1, 4, _T("$399"));

    m_ListViewOne->add_item(2, 0, _T("KB-451"));
    m_ListViewOne->add_item(2, 1, _T("Mechanical Keyboard"));
    m_ListViewOne->add_item(2, 2, _T("Accessories"));
    m_ListViewOne->add_item(2, 3, _T("76"));
    m_ListViewOne->add_item(2, 4, _T("$119"));

    m_ListViewOne->add_item(3, 0, _T("MS-307"));
    m_ListViewOne->add_item(3, 1, _T("Wireless Mouse"));
    m_ListViewOne->add_item(3, 2, _T("Accessories"));
    m_ListViewOne->add_item(3, 3, _T("134"));
    m_ListViewOne->add_item(3, 4, _T("$39"));

    inventory_grid->add(m_ListViewOne, 1, 0, 1, 2);

    main_grid->add_panel(inventory_grid);
    main_grid->set_grid_position(inventory_grid, 1, 1);

    // Right card: activity and counters
    auto activity_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    activity_panel->set_spacing(8);
    activity_panel->set_padding(10);
    activity_panel->set_alignment(layout::alignment::stretch);

    auto activity_title = create_static_control(_T("Live Activity"), 220, 24);
    activity_panel->add(activity_title);

    m_CounterLabel = create_static_control(_T("Events processed: 0"), 220, 24);
    activity_panel->add(m_CounterLabel);

    m_IncrementBtn = create_button(_T("Process Event"), 220, 30);
    m_IncrementBtn->on_click([this](WPARAM, LPARAM) {
        ++m_counter;
        m_CounterLabel->set_text(TEXT("Events processed: ") + to_tstring(m_counter));
    });
    activity_panel->add(m_IncrementBtn);

    auto reset_btn = create_button(_T("Reset Counter"), 220, 30);
    reset_btn->on_click([this](WPARAM, LPARAM) {
        m_counter = 0;
        m_CounterLabel->set_text(_T("Events processed: 0"));
    });
    activity_panel->add(reset_btn);

    auto progress_label = create_static_control(_T("Pipeline Health"), 220, 20);
    activity_panel->add(progress_label);
    auto health_bar = create_progress_bar(220, 20);
    health_bar->set_range(0, 100);
    health_bar->set_pos(82);
    activity_panel->add(health_bar);

    auto nav_tree = create_tree_view(220, 170);
    auto dashboards = nav_tree->insert_item(_T("Dashboards"), TVI_ROOT, TVI_LAST);
    nav_tree->insert_item(_T("Sales"), dashboards, TVI_LAST);
    nav_tree->insert_item(_T("Inventory"), dashboards, TVI_LAST);
    nav_tree->insert_item(_T("Operations"), dashboards, TVI_LAST);
    nav_tree->expand(dashboards);
    activity_panel->add(nav_tree);

    main_grid->add_panel(activity_panel);
    main_grid->set_grid_position(activity_panel, 1, 2);

    // Bottom showcase row: three equal sections
    auto showcase_grid = std::make_shared<layout::grid_panel>(hWnd);
    showcase_grid->set_padding(6);
    showcase_grid->set_spacing(8);
    showcase_grid->add_row_definition(layout::grid_length::star(1));
    showcase_grid->add_column_definition(layout::grid_length::star(1));
    showcase_grid->add_column_definition(layout::grid_length::star(1));
    showcase_grid->add_column_definition(layout::grid_length::star(1));

    auto tab_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    tab_panel->set_alignment(layout::alignment::stretch);
    auto tab_title = create_static_control(_T("Tabs"), 180, 22);
    tab_panel->add(tab_title);
    m_TabControl = create_tab_control(280, 150);
    m_TabControl->add_item(_T("Overview"));
    m_TabControl->add_item(_T("Metrics"));
    m_TabControl->add_item(_T("Settings"));
    tab_panel->add(m_TabControl);
    showcase_grid->add_panel(tab_panel);
    showcase_grid->set_grid_position(tab_panel, 0, 0);

    auto notes_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    notes_panel->set_alignment(layout::alignment::stretch);
    auto notes_title = create_static_control(_T("Rich Edit"), 180, 22);
    notes_panel->add(notes_title);
    auto notes = create_rich_edit(_T("Use nested layouts to compose complex UIs quickly.\n")
                                  _T("Grid rows/columns + spans make responsive designs straightforward."),
                                  280,
                                  150);
    notes_panel->add(notes);
    showcase_grid->add_panel(notes_panel);
    showcase_grid->set_grid_position(notes_panel, 0, 1);

    auto queue_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    queue_panel->set_alignment(layout::alignment::stretch);
    auto queue_title = create_static_control(_T("Queue"), 180, 22);
    queue_panel->add(queue_title);
    auto queue = create_list_box(280, 150);
    queue->add(_T("Render dashboard"));
    queue->add(_T("Sync product catalog"));
    queue->add(_T("Send usage report"));
    queue->add(_T("Backup workspace"));
    queue_panel->add(queue);
    showcase_grid->add_panel(queue_panel);
    showcase_grid->set_grid_position(queue_panel, 0, 2);

    main_grid->add_panel(showcase_grid);
    main_grid->set_grid_position(showcase_grid, 2, 0, 1, 3);

    // Footer
    auto footer = std::make_shared<layout::stack_panel>(layout::orientation::horizontal, hWnd);
    footer->set_spacing(12);
    footer->set_padding(5);

    auto status = create_static_control(_T("Ready | Grid layout demo powered by WindowsPlusPlus"), 460, 24);
    footer->add(status);

    auto link = create_link_control(_T("<a href=\"https://github.com/Timboy67678/WindowsPlusPlus\">GitHub</a>"), 140, 24);
    link->on_click([this](LPNMHDR nm) {
        auto item = reinterpret_cast<PNMLINK>(nm)->item;
        ShellExecuteW(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOWNORMAL);
    });
    footer->add(link);

    main_grid->add_panel(footer);
    main_grid->set_grid_position(footer, 3, 0, 1, 3);

    // Initial layout
    RECT client_rect = get_client_rect();
    main_grid->measure(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
    main_grid->arrange(0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

    root_panel() = main_grid;

    if (m_TabControl && m_TabControl->is_valid()) {
        m_TabControl->set_top();
        m_TabControl->invalidate();
        m_TabControl->update_window();
    }

    return window::on_create(hWnd, wParam, lParam);
}

LRESULT WindowGridPanel::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto result = window::on_size(hWnd, wParam, lParam);

    if (m_TabControl && m_TabControl->is_valid()) {
        m_TabControl->set_top();
        m_TabControl->invalidate();
    }

    return result;
}

