#include "WindowGridPanel.hpp"

constexpr auto DEFAULT_WINDOW_WIDTH = 1200;
constexpr auto DEFAULT_WINDOW_HEIGHT = 780;

namespace {
    void add_row_definitions(const std::shared_ptr<layout::grid_panel>& panel,
                             std::initializer_list<layout::grid_length> rows) {
        for (const auto& row : rows) {
            panel->add_row_definition(row);
        }
    }

    void add_column_definitions(const std::shared_ptr<layout::grid_panel>& panel,
                                std::initializer_list<layout::grid_length> columns) {
        for (const auto& column : columns) {
            panel->add_column_definition(column);
        }
    }

    tstring make_counter_text(int counter) {
        return _T("Events processed: ") + to_tstring(counter);
    }

    void add_inventory_item_row(const control_ptr<list_view>& list_view,
                                int row,
                                LPCTSTR sku,
                                LPCTSTR product,
                                LPCTSTR category,
                                LPCTSTR stock,
                                LPCTSTR price) {
        list_view->add_item(row, 0, sku);
        list_view->add_item(row, 1, product);
        list_view->add_item(row, 2, category);
        list_view->add_item(row, 3, stock);
        list_view->add_item(row, 4, price);
    }
}

WindowGridPanel::WindowGridPanel(LPCTSTR window_title, int x, int y, HINSTANCE instance)
    : window(window_class(_T("WindowGridPanelWPP"), instance),
             window_title, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {
    set_keep_minimum_resize(true);
}

LRESULT WindowGridPanel::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    center_window();
    auto main_grid = create_main_grid(hWnd);

    auto header = create_header_panel(hWnd, main_grid);
    main_grid->add_panel(header);
    main_grid->set_grid_position(header, 0, 0, 1, 3);

    auto form_grid = create_form_panel(hWnd);
    main_grid->add_panel(form_grid);
    main_grid->set_grid_position(form_grid, 1, 0);

    auto inventory_grid = create_inventory_panel(hWnd);
    main_grid->add_panel(inventory_grid);
    main_grid->set_grid_position(inventory_grid, 1, 1);

    auto activity_panel = create_activity_panel(hWnd);
    main_grid->add_panel(activity_panel);
    main_grid->set_grid_position(activity_panel, 1, 2);

    auto showcase_grid = create_showcase_panel(hWnd);
    main_grid->add_panel(showcase_grid);
    main_grid->set_grid_position(showcase_grid, 2, 0, 1, 3);

    auto footer = create_footer_panel(hWnd);
    main_grid->add_panel(footer);
    main_grid->set_grid_position(footer, 3, 0, 1, 3);

    arrange_main_grid(main_grid);

    root_panel() = main_grid;

    return window::on_create(hWnd, wParam, lParam);
}

LRESULT WindowGridPanel::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    LRESULT result = window::on_size(hWnd, wParam, lParam);
    if (m_TabControl) {
        m_TabControl->fit_selected_page_to_display_rect();
    }
    return result;
}

std::shared_ptr<layout::grid_panel> WindowGridPanel::create_main_grid(HWND hWnd) {
    auto main_grid = std::make_shared<layout::grid_panel>(hWnd);
    main_grid->set_padding(14);
    main_grid->set_spacing(10);

    add_row_definitions(main_grid,
                        { layout::grid_length::pixels(64),
                          layout::grid_length::star(2.0),
                          layout::grid_length::pixels(210),
                          layout::grid_length::pixels(42) });
    add_column_definitions(main_grid,
                           { layout::grid_length::star(1.2),
                             layout::grid_length::star(2.2),
                             layout::grid_length::star(1.2) });

    return main_grid;
}

std::shared_ptr<layout::grid_panel> WindowGridPanel::create_header_panel(
    HWND hWnd,
    const std::shared_ptr<layout::grid_panel>& main_grid) {
    auto header = std::make_shared<layout::grid_panel>(hWnd);
    header->set_padding(8);
    header->set_spacing(8);
    add_row_definitions(header, { layout::grid_length::star(1) });
    add_column_definitions(header,
                           { layout::grid_length::star(1.0),
                             layout::grid_length::pixels(140),
                             layout::grid_length::pixels(140) });

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

    return header;
}

std::shared_ptr<layout::grid_panel> WindowGridPanel::create_form_panel(HWND hWnd) {
    auto form_grid = std::make_shared<layout::grid_panel>(hWnd);
    form_grid->set_padding(10);
    form_grid->set_spacing(8);

    add_row_definitions(form_grid,
                        { layout::grid_length::pixels(28),
                          layout::grid_length::pixels(28),
                          layout::grid_length::pixels(28),
                          layout::grid_length::pixels(28),
                          layout::grid_length::pixels(28),
                          layout::grid_length::star(1),
                          layout::grid_length::pixels(34) });
    add_column_definitions(form_grid,
                           { layout::grid_length::pixels(80),
                             layout::grid_length::star(1) });

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

    return form_grid;
}

std::shared_ptr<layout::grid_panel> WindowGridPanel::create_inventory_panel(HWND hWnd) {
    auto inventory_grid = std::make_shared<layout::grid_panel>(hWnd);
    inventory_grid->set_padding(10);
    inventory_grid->set_spacing(8);
    add_row_definitions(inventory_grid,
                        { layout::grid_length::pixels(30),
                          layout::grid_length::star(1) });
    add_column_definitions(inventory_grid,
                           { layout::grid_length::star(1),
                             layout::grid_length::pixels(220) });

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

    add_inventory_item_row(m_ListViewOne, 0, _T("LT-100"), _T("Laptop Pro 14"), _T("Computers"), _T("42"), _T("$1299"));
    add_inventory_item_row(m_ListViewOne, 1, _T("MN-210"), _T("32in Monitor"), _T("Displays"), _T("18"), _T("$399"));
    add_inventory_item_row(m_ListViewOne, 2, _T("KB-451"), _T("Mechanical Keyboard"), _T("Accessories"), _T("76"), _T("$119"));
    add_inventory_item_row(m_ListViewOne, 3, _T("MS-307"), _T("Wireless Mouse"), _T("Accessories"), _T("134"), _T("$39"));

    inventory_grid->add(m_ListViewOne, 1, 0, 1, 2);
    return inventory_grid;
}

std::shared_ptr<layout::stack_panel> WindowGridPanel::create_activity_panel(HWND hWnd) {
    auto activity_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    activity_panel->set_spacing(8);
    activity_panel->set_padding(10);
    activity_panel->set_alignment(layout::alignment::stretch);

    auto activity_title = create_static_control(_T("Live Activity"), 220, 24);
    activity_panel->add(activity_title);

    m_CounterLabel = create_static_control(make_counter_text(0).c_str(), 220, 24);
    activity_panel->add(m_CounterLabel);

    m_IncrementBtn = create_button(_T("Process Event"), 220, 30);
    m_IncrementBtn->on_click([this](WPARAM, LPARAM) {
        ++m_counter;
        m_CounterLabel->set_text(make_counter_text(m_counter));
    });
    activity_panel->add(m_IncrementBtn);

    auto reset_btn = create_button(_T("Reset Counter"), 220, 30);
    reset_btn->on_click([this](WPARAM, LPARAM) {
        m_counter = 0;
        m_CounterLabel->set_text(make_counter_text(m_counter));
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

    return activity_panel;
}

std::shared_ptr<layout::grid_panel> WindowGridPanel::create_showcase_panel(HWND hWnd) {
    auto showcase_grid = std::make_shared<layout::grid_panel>(hWnd);
    showcase_grid->set_padding(6);
    showcase_grid->set_spacing(8);
    add_row_definitions(showcase_grid, { layout::grid_length::star(1) });
    add_column_definitions(showcase_grid,
                           { layout::grid_length::star(1),
                             layout::grid_length::star(1),
                             layout::grid_length::star(1) });

    auto tab_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    tab_panel->set_alignment(layout::alignment::stretch);
    auto tab_title = create_static_control(_T("Tabs"), 180, 22);
    tab_panel->add(tab_title);
    m_TabControl = create_tab_control(280, 150);
    m_TabControl->add_item(_T("Overview"));
    m_TabControl->add_item(_T("Metrics"));
    m_TabControl->add_item(_T("Settings"));

    auto overview = create_rich_edit(_T("Overview\r\n\r\n")
                                     _T("This tab demonstrates managed controls that are attached to tab pages."),
                                     260,
                                     115);

    auto metrics = create_list_view(260, 115);
    metrics->add_column(_T("Metric"), 140);
    metrics->add_column(_T("Value"), 100);
    metrics->add_item(0, 0, _T("Events processed"));
    metrics->add_item(0, 1, to_tstring(m_counter).c_str());
    metrics->add_item(1, 0, _T("Inventory items"));
    metrics->add_item(1, 1, _T("4"));
    metrics->add_item(2, 0, _T("Pipeline health"));
    metrics->add_item(2, 1, _T("82%"));
    metrics->auto_size_columns();

    auto settings = create_list_box(260, 115);
    settings->add(_T("Auto refresh: enabled"));
    settings->add(_T("Telemetry: basic"));
    settings->add(_T("Theme: system"));

    m_TabControl->add_page_control(0, overview->get_handle());
    m_TabControl->add_page_control(1, metrics->get_handle());
    m_TabControl->add_page_control(2, settings->get_handle());
    m_TabControl->on_selection_change([this](LPNMHDR) {
        m_TabControl->sync_managed_pages();
    });
    m_TabControl->set_cur_sel(0);

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

    return showcase_grid;
}

std::shared_ptr<layout::stack_panel> WindowGridPanel::create_footer_panel(HWND hWnd) {
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

    return footer;
}

void WindowGridPanel::arrange_main_grid(const std::shared_ptr<layout::grid_panel>& main_grid) {
    RECT client_rect = get_client_rect();
    main_grid->measure(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
    main_grid->arrange(0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
}


