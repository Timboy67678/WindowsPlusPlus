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

    // Create main grid panel with 5 rows and 3 columns
    auto main_grid = std::make_shared<layout::grid_panel>(hWnd);
    main_grid->set_padding(10);
    main_grid->set_spacing(10);

    // Define rows: fixed header (40px), auto, star (2*), auto, fixed footer (30px)
    main_grid->add_row_definition(layout::grid_length::pixels(40));   // Header
    main_grid->add_row_definition(layout::grid_length::auto_size());  // Controls row
    main_grid->add_row_definition(layout::grid_length::star(2.0));    // Main content (2*)
    main_grid->add_row_definition(layout::grid_length::star(1.0));    // Secondary content (1*)
    main_grid->add_row_definition(layout::grid_length::pixels(30));   // Footer

    // Define columns: sidebar (200px), main content (2*), side panel (1*)
    main_grid->add_column_definition(layout::grid_length::pixels(200));
    main_grid->add_column_definition(layout::grid_length::star(2.0));
    main_grid->add_column_definition(layout::grid_length::star(1.0));

    // ===== ROW 0: Header spanning all columns =====
    auto header_panel = std::make_shared<layout::stack_panel>(layout::orientation::horizontal, hWnd);
    header_panel->set_spacing(10);
    header_panel->set_padding(5);

    auto title = create_static_control(_T("Grid Panel Layout Showcase"), 400, 30);  
    header_panel->add(title);

    auto btn_about = create_button(_T("About Grid"), 100, 25);
    btn_about->on_click([this](WPARAM, LPARAM) {
        MessageBox(get_handle(), 
            _T("Grid Panel Features:\n")
            _T("  Auto-sized rows/columns\n")
            _T("  Fixed pixel sizes\n")
            _T("  Proportional star (*) sizing\n")
            _T("  Row and column spanning\n")
            _T("  Nested layouts"),
            _T("  Grid Panel Info"), MB_OK | MB_ICONINFORMATION);
    });
    header_panel->add(btn_about);

    main_grid->add_panel(header_panel);
    main_grid->set_grid_position(header_panel, 0, 0, 1, 3); // Span all 3 columns

    // ===== ROW 1, COL 0: Left sidebar with buttons =====
    auto sidebar_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    sidebar_panel->set_spacing(8);
    sidebar_panel->set_padding(5);

    auto sidebar_label = create_static_control(_T("Navigation:"), 180, 20);
    sidebar_panel->add(sidebar_label);

    auto btn_home = create_button(_T("Home"), 180, 30);
    sidebar_panel->add(btn_home);

    auto btn_settings = create_button(_T("Settings"), 180, 30);
    sidebar_panel->add(btn_settings);

    auto btn_help = create_button(_T("Help"), 180, 30);
    sidebar_panel->add(btn_help);

    main_grid->add_panel(sidebar_panel);
    main_grid->set_grid_position(sidebar_panel, 1, 0, 3, 1); // Span rows 1-3

    // ===== ROW 1, COL 1: Form inputs =====
    auto form_grid = std::make_shared<layout::grid_panel>(hWnd);
    form_grid->set_padding(5);
    form_grid->set_row_spacing(8);
    form_grid->set_column_spacing(10);

    // Define form grid: 3 rows (auto), 2 columns (auto, star)
    form_grid->add_row_definition(layout::grid_length::auto_size());
    form_grid->add_row_definition(layout::grid_length::auto_size());
    form_grid->add_row_definition(layout::grid_length::auto_size());
    form_grid->add_column_definition(layout::grid_length::pixels(80));
    form_grid->add_column_definition(layout::grid_length::star(1.0));

    // Row 0: Name input
    auto label_name = create_static_control(_T("Name:"), 70, 20);
    form_grid->add(label_name, 0, 0);
    form_grid->set_alignment(label_name, layout::alignment::start, layout::alignment::center);

    m_EditOne = create_edit_text(_T(""), 200, 25);
    form_grid->add(m_EditOne, 0, 1);
    form_grid->set_alignment(m_EditOne, layout::alignment::stretch, layout::alignment::center);

    // Row 1: Email input
    auto label_email = create_static_control(_T("Email:"), 70, 20);
    form_grid->add(label_email, 1, 0);
    form_grid->set_alignment(label_email, layout::alignment::start, layout::alignment::center);

    m_EditTwo = create_edit_text(_T(""), 200, 25);
    form_grid->add(m_EditTwo, 1, 1);
    form_grid->set_alignment(m_EditTwo, layout::alignment::stretch, layout::alignment::center);

    // Row 2: Phone input
    auto label_phone = create_static_control(_T("Phone:"), 70, 20);
    form_grid->add(label_phone, 2, 0);
    form_grid->set_alignment(label_phone, layout::alignment::start, layout::alignment::center);

    m_EditThree = create_edit_text(_T(""), 200, 25);
    form_grid->add(m_EditThree, 2, 1);
    form_grid->set_alignment(m_EditThree, layout::alignment::stretch, layout::alignment::center);

    main_grid->add_panel(form_grid);
    main_grid->set_grid_position(form_grid, 1, 1);

    // ===== ROW 1, COL 2: Counter demo =====
    auto counter_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    counter_panel->set_spacing(10);
    counter_panel->set_padding(10);

    auto counter_title = create_static_control(_T("Counter Demo:"), 180, 20);
    counter_panel->add(counter_title);

    m_CounterLabel = create_static_control(_T("Count: 0"), 180, 30);
    counter_panel->add(m_CounterLabel);

    m_IncrementBtn = create_button(_T("Increment"), 180, 30);
    m_IncrementBtn->on_click([this](WPARAM, LPARAM) {
        m_counter++;
        tstring text = TEXT("Count: ") + to_tstring(m_counter);
        m_CounterLabel->set_text(text);
    });
    counter_panel->add(m_IncrementBtn);

    auto reset_btn = create_button(_T("Reset"), 180, 30);
    reset_btn->on_click([this](WPARAM, LPARAM) {
        m_counter = 0;
        m_CounterLabel->set_text(_T("Count: 0"));
    });
    counter_panel->add(reset_btn);

    main_grid->add_panel(counter_panel);
    main_grid->set_grid_position(counter_panel, 1, 2);

    // ===== ROW 2, COL 1: List View (spanning 2 columns) =====
    auto listview_container = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    listview_container->set_spacing(5);
    listview_container->set_padding(5);

    auto listview_label = create_static_control(_T("Data Grid (2* height, spans 2 columns):"), 400, 20);
    listview_container->add(listview_label);

    m_ListViewOne = create_list_view(400, 150);
    m_ListViewOne->add_column(_T("ID"), 50);
    m_ListViewOne->add_column(_T("Product"), 150);
    m_ListViewOne->add_column(_T("Category"), 100);
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

    m_ListViewOne->auto_size_columns();
    listview_container->add(m_ListViewOne);

    main_grid->add_panel(listview_container);
    main_grid->set_grid_position(listview_container, 2, 1, 1, 2); // Span 2 columns

    // ===== ROW 3, COL 1: Action buttons =====
    auto button_grid = std::make_shared<layout::grid_panel>(hWnd);
    button_grid->set_padding(5);
    button_grid->set_spacing(8);

    // 2 rows, 3 columns for buttons
    button_grid->add_row_definition(layout::grid_length::auto_size());
    button_grid->add_row_definition(layout::grid_length::auto_size());
    button_grid->add_column_definition(layout::grid_length::star(1.0));
    button_grid->add_column_definition(layout::grid_length::star(1.0));
    button_grid->add_column_definition(layout::grid_length::star(1.0));

    auto btn_save = create_button(_T("Save"), 100, 30);
    button_grid->add(btn_save, 0, 0);
    button_grid->set_alignment(btn_save, layout::alignment::center, layout::alignment::center);

    auto btn_load = create_button(_T("Load"), 100, 30);
    button_grid->add(btn_load, 0, 1);
    button_grid->set_alignment(btn_load, layout::alignment::center, layout::alignment::center);

    auto btn_export = create_button(_T("Export"), 100, 30);
    button_grid->add(btn_export, 0, 2);
    button_grid->set_alignment(btn_export, layout::alignment::center, layout::alignment::center);

    auto btn_delete = create_button(_T("Delete"), 100, 30);
    button_grid->add(btn_delete, 1, 0);
    button_grid->set_alignment(btn_delete, layout::alignment::center, layout::alignment::center);

    auto btn_refresh = create_button(_T("Refresh"), 100, 30);
    btn_refresh->on_click([this](WPARAM, LPARAM) {
        MessageBox(get_handle(), _T("Refreshed!"), _T("Info"), MB_OK | MB_ICONINFORMATION);
    });
    button_grid->add(btn_refresh, 1, 1);
    button_grid->set_alignment(btn_refresh, layout::alignment::center, layout::alignment::center);

    auto btn_clear = create_button(_T("Clear"), 100, 30);
    button_grid->add(btn_clear, 1, 2);
    button_grid->set_alignment(btn_clear, layout::alignment::center, layout::alignment::center);

    main_grid->add_panel(button_grid);
    main_grid->set_grid_position(button_grid, 3, 1);

    // ===== ROW 3, COL 2: Options panel =====
    auto options_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical, hWnd);
    options_panel->set_spacing(8);
    options_panel->set_padding(5);

    auto options_label = create_static_control(_T("Options:"), 150, 20);
    options_panel->add(options_label);

    auto check1 = create_check_box(_T("Enable feature A"), 150, 25);
    options_panel->add(check1);

    auto check2 = create_check_box(_T("Enable feature B"), 150, 25);
    options_panel->add(check2);

    auto check3 = create_check_box(_T("Auto-save"), 150, 25);
    options_panel->add(check3);

    main_grid->add_panel(options_panel);
    main_grid->set_grid_position(options_panel, 3, 2);

    // ===== ROW 4: Footer spanning all columns =====
    auto footer_panel = std::make_shared<layout::stack_panel>(layout::orientation::horizontal, hWnd);
    footer_panel->set_spacing(10);
    footer_panel->set_padding(3);

    auto status = create_static_control(_T("Ready | Grid Layout Demo"), 300, 25);
    footer_panel->add(status);

    auto link = create_link_control(_T("<a href=\"https://github.com/Timboy67678/WindowsPlusPlus\">WindowsPlusPlus on GitHub</a>"), 300, 25);
    link->on_click([this](LPNMHDR nm) {
        auto item = reinterpret_cast<PNMLINK>(nm)->item;
        ShellExecuteW(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOWNORMAL);
    });
    footer_panel->add(link);

    main_grid->add_panel(footer_panel);
    main_grid->set_grid_position(footer_panel, 4, 0, 1, 3); // Span all 3 columns

    // Initial layout
    RECT client_rect = get_client_rect();
    main_grid->measure(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
    main_grid->arrange(0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

    root_panel() = main_grid;

    return window::on_create(hWnd, wParam, lParam);
}

