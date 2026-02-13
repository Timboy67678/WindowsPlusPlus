# Layout Manager: Real-World Examples

This document provides complete, working examples of how the layout manager is used in real-world Windows++ applications with the **simplified, layout-first API** (no coordinate positioning).

## Example 1: Simple Settings Dialog

A typical settings dialog with labels, textboxes, checkboxes, and buttons.

```cpp
class SettingsDialog : public wpp::dialog {
public:
    SettingsDialog(HINSTANCE hInstance)
        : dialog(hInstance, IDD_SETTINGS) {}
    
    message_handler on_init_dialog override;
    
private:
    control_ptr<edit_text> m_username;
    control_ptr<edit_text> m_email;
    control_ptr<check_box> m_notifications;
    control_ptr<button> m_ok;
    control_ptr<button> m_cancel;
};

INT_PTR SettingsDialog::on_init_dialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // Create a vertical stack for the entire form
    auto main_stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    main_stack->set_padding(15, 15, 15, 15);
    main_stack->set_spacing(10);
    
    // Create a grid for form fields (2 columns: labels and inputs)
    auto form_grid = std::make_shared<layout::grid>();
    form_grid->add_column({layout::grid::column_definition::size_type::auto_size});
    form_grid->add_column({layout::grid::column_definition::size_type::star, 1.0});
    form_grid->add_row({layout::grid::row_definition::size_type::auto_size});
    form_grid->add_row({layout::grid::row_definition::size_type::auto_size});
    
    // Add form fields - NO coordinates needed!
    auto label_username = create_static_text(_T("Username:"));
    m_username = create_edit_text(_T(""), 200);  // 200px wide
    form_grid->add(label_username, 0, 0);
    form_grid->add(m_username, 0, 1);
    
    auto label_email = create_static_text(_T("Email:"));
    m_email = create_edit_text(_T(""), 200);
    form_grid->add(label_email, 1, 0);
    form_grid->add(m_email, 1, 1);
    
    main_stack->add(form_grid);
    
    // Add checkbox
    m_notifications = create_check_box(_T("Enable notifications"));
    main_stack->add(m_notifications);
    
    // Create button row (right-aligned)
    auto button_row = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
    button_row->set_spacing(5);
    
    m_ok = create_button(_T("OK"));
    m_ok->on_click([this](WPARAM, LPARAM) {
        // Save settings
        end_dialog(IDOK);
    });
    
    m_cancel = create_button(_T("Cancel"));
    m_cancel->on_click([this](WPARAM, LPARAM) {
        end_dialog(IDCANCEL);
    });
    
    button_row->add(m_ok);
    button_row->add(m_cancel);
    
    main_stack->add(button_row, layout::alignment::end);  // Right-align buttons
    
    set_layout(main_stack);
    
    return dialog::on_init_dialog(hWnd, wParam, lParam);
}
```

## Example 2: Text Editor Window

A complete text editor with toolbar, status bar, and main editing area.

```cpp
class TextEditorWindow : public wpp::window {
public:
    TextEditorWindow(LPCTSTR title, HINSTANCE instance)
        : window(window_class{_T("TextEditorWPP"), instance},
                 title, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                 WS_OVERLAPPEDWINDOW) {}
    
    message_handler on_create override;
    
private:
    // Toolbar buttons
    control_ptr<button> m_new_btn;
    control_ptr<button> m_open_btn;
    control_ptr<button> m_save_btn;
    control_ptr<button> m_cut_btn;
    control_ptr<button> m_copy_btn;
    control_ptr<button> m_paste_btn;
    
    // Editor
    control_ptr<edit_text> m_editor;
    
    // Status bar
    control_ptr<static_text> m_status;
};

LRESULT TextEditorWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // Main dock panel for app layout
    auto main_dock = std::make_shared<layout::dock_panel>();
    
    // === Toolbar at top ===
    auto toolbar = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
    toolbar->set_padding(5, 5, 5, 5);
    toolbar->set_spacing(5);
    
    // File operations
    m_new_btn = create_button(_T("New"));
    m_new_btn->on_click([this](WPARAM, LPARAM) {
        m_editor->set_text(_T(""));
    });
    
    m_open_btn = create_button(_T("Open"));
    m_save_btn = create_button(_T("Save"));
    
    toolbar->add(m_new_btn);
    toolbar->add(m_open_btn);
    toolbar->add(m_save_btn);
    
    // Add separator (static text)
    auto separator = create_static_text(_T("|"));
    toolbar->add(separator);
    
    // Edit operations
    m_cut_btn = create_button(_T("Cut"));
    m_copy_btn = create_button(_T("Copy"));
    m_paste_btn = create_button(_T("Paste"));
    
    toolbar->add(m_cut_btn);
    toolbar->add(m_copy_btn);
    toolbar->add(m_paste_btn);
    
    main_dock->add(toolbar, layout::dock_position::top);
    
    // === Status bar at bottom ===
    m_status = create_static_text(_T("Ready"));
    main_dock->add(m_status, layout::dock_position::bottom);
    
    // === Main editor fills remaining space ===
    m_editor = create_edit_text(_T(""), 100,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | 
        ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL);
    
    main_dock->add(m_editor, layout::dock_position::fill);
    
    set_layout(main_dock);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

## Example 3: File Explorer with Sidebar

A file explorer with tree view sidebar and file list.

```cpp
class FileExplorerWindow : public wpp::window {
public:
    FileExplorerWindow(LPCTSTR title, HINSTANCE instance)
        : window(window_class{_T("FileExplorerWPP"), instance},
                 title, CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
                 WS_OVERLAPPEDWINDOW) {}
    
    message_handler on_create override;
    
private:
    control_ptr<tree_view> m_folder_tree;
    control_ptr<list_view> m_file_list;
    control_ptr<static_text> m_path_label;
    control_ptr<edit_text> m_path_bar;
    control_ptr<button> m_go_btn;
    control_ptr<static_text> m_status;
};

LRESULT FileExplorerWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto main_dock = std::make_shared<layout::dock_panel>();
    
    // === Address bar at top ===
    auto address_bar = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
    address_bar->set_padding(5, 5, 5, 5);
    address_bar->set_spacing(5);
    
    m_path_label = create_static_text(_T("Path:"));
    m_path_bar = create_edit_text(_T("C:\\"), 600);
    m_go_btn = create_button(_T("Go"));
    
    address_bar->add(m_path_label);
    address_bar->add(m_path_bar);
    address_bar->add(m_go_btn);
    
    main_dock->add(address_bar, layout::dock_position::top);
    
    // === Status bar at bottom ===
    m_status = create_static_text(_T("Ready"));
    main_dock->add(m_status, layout::dock_position::bottom);
    
    // === Sidebar with folder tree ===
    m_folder_tree = create_tree_view(200);
    main_dock->add(m_folder_tree, layout::dock_position::left);
    
    // === File list fills remaining space ===
    m_file_list = create_list_view(600, 400);
    m_file_list->add_column(_T("Name"), 200);
    m_file_list->add_column(_T("Type"), 100);
    m_file_list->add_column(_T("Size"), 100);
    m_file_list->add_column(_T("Modified"), 150);
    
    main_dock->add(m_file_list, layout::dock_position::fill);
    
    set_layout(main_dock);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

## Example 4: Login Dialog with Logo

A login dialog with a logo image at top and centered form.

```cpp
class LoginDialog : public wpp::dialog {
public:
    LoginDialog(HINSTANCE hInstance)
        : dialog(hInstance, IDD_LOGIN) {}
    
    message_handler on_init_dialog override;
    
private:
    control_ptr<static_text> m_logo;
    control_ptr<edit_text> m_username;
    control_ptr<edit_text> m_password;
    control_ptr<button> m_login;
    control_ptr<button> m_cancel;
};

INT_PTR LoginDialog::on_init_dialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto main_stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    main_stack->set_padding(20, 20, 20, 20);
    main_stack->set_spacing(15);
    
    // === Logo at top (centered) ===
    m_logo = create_static_text(_T("MyApp v1.0"));
    main_stack->add(m_logo, layout::alignment::center);
    
    // === Login form ===
    auto form_grid = std::make_shared<layout::grid>();
    form_grid->add_column({layout::grid::column_definition::size_type::auto_size});
    form_grid->add_column({layout::grid::column_definition::size_type::absolute, 200});
    form_grid->add_row({layout::grid::row_definition::size_type::auto_size});
    form_grid->add_row({layout::grid::row_definition::size_type::auto_size});
    
    auto label_user = create_static_text(_T("Username:"));
    m_username = create_edit_text(_T(""), 200);
    form_grid->add(label_user, 0, 0);
    form_grid->add(m_username, 0, 1);
    
    auto label_pass = create_static_text(_T("Password:"));
    m_password = create_edit_text(_T(""), 200,
        WS_CHILD | WS_VISIBLE | ES_PASSWORD);
    form_grid->add(label_pass, 1, 0);
    form_grid->add(m_password, 1, 1);
    
    main_stack->add(form_grid, layout::alignment::center);
    
    // === Buttons (centered) ===
    auto button_row = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
    button_row->set_spacing(10);
    
    m_login = create_button(_T("Login"));
    m_login->on_click([this](WPARAM, LPARAM) {
        // Validate credentials
        end_dialog(IDOK);
    });
    
    m_cancel = create_button(_T("Cancel"));
    m_cancel->on_click([this](WPARAM, LPARAM) {
        end_dialog(IDCANCEL);
    });
    
    button_row->add(m_login);
    button_row->add(m_cancel);
    
    main_stack->add(button_row, layout::alignment::center);
    
    set_layout(main_stack);
    
    return dialog::on_init_dialog(hWnd, wParam, lParam);
}
```

## Example 5: Dashboard with Multiple Panels

A dashboard with multiple information panels in a grid.

```cpp
class DashboardWindow : public wpp::window {
public:
    DashboardWindow(LPCTSTR title, HINSTANCE instance)
        : window(window_class{_T("DashboardWPP"), instance},
                 title, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
                 WS_OVERLAPPEDWINDOW) {}
    
    message_handler on_create override;
    
private:
    control_ptr<list_view> m_recent_files;
    control_ptr<list_view> m_tasks;
    control_ptr<edit_text> m_notes;
    control_ptr<static_text> m_stats;
};

LRESULT DashboardWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto main_dock = std::make_shared<layout::dock_panel>();
    
    // === Title bar ===
    auto title = create_static_text(_T("Dashboard"));
    main_dock->add(title, layout::dock_position::top);
    
    // === Main content grid (2x2) ===
    auto content_grid = std::make_shared<layout::grid>();
    content_grid->set_padding(10, 10, 10, 10);
    
    // 2 equal columns
    content_grid->add_column({layout::grid::column_definition::size_type::star, 1.0});
    content_grid->add_column({layout::grid::column_definition::size_type::star, 1.0});
    
    // 2 equal rows
    content_grid->add_row({layout::grid::row_definition::size_type::star, 1.0});
    content_grid->add_row({layout::grid::row_definition::size_type::star, 1.0});
    
    // === Top-left: Recent Files ===
    auto recent_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    recent_panel->set_spacing(5);
    
    auto recent_label = create_static_text(_T("Recent Files"));
    m_recent_files = create_list_view(400, 250);
    m_recent_files->add_column(_T("Name"), 200);
    m_recent_files->add_column(_T("Modified"), 100);
    
    recent_panel->add(recent_label);
    recent_panel->add(m_recent_files);
    
    content_grid->add(recent_panel, 0, 0);
    
    // === Top-right: Tasks ===
    auto tasks_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    tasks_panel->set_spacing(5);
    
    auto tasks_label = create_static_text(_T("Tasks"));
    m_tasks = create_list_view(400, 250);
    m_tasks->add_column(_T("Task"), 200);
    m_tasks->add_column(_T("Status"), 100);
    
    tasks_panel->add(tasks_label);
    tasks_panel->add(m_tasks);
    
    content_grid->add(tasks_panel, 0, 1);
    
    // === Bottom-left: Notes ===
    auto notes_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    notes_panel->set_spacing(5);
    
    auto notes_label = create_static_text(_T("Notes"));
    m_notes = create_edit_text(_T(""), 400,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL);
    
    notes_panel->add(notes_label);
    notes_panel->add(m_notes);
    
    content_grid->add(notes_panel, 1, 0);
    
    // === Bottom-right: Statistics ===
    auto stats_panel = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    stats_panel->set_spacing(5);
    
    auto stats_label = create_static_text(_T("Statistics"));
    m_stats = create_static_text(_T("Files: 42\nTasks: 7\nNotes: 15"));
    
    stats_panel->add(stats_label);
    stats_panel->add(m_stats);
    
    content_grid->add(stats_panel, 1, 1);
    
    main_dock->add(content_grid, layout::dock_position::fill);
    
    set_layout(main_dock);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

## Example 6: Responsive Layout with WrapPanel

A toolbar that wraps buttons when window is too narrow.

```cpp
class ResponsiveToolbarWindow : public wpp::window {
public:
    ResponsiveToolbarWindow(LPCTSTR title, HINSTANCE instance)
        : window(window_class{_T("ResponsiveWPP"), instance},
                 title, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                 WS_OVERLAPPEDWINDOW) {}
    
    message_handler on_create override;
};

LRESULT ResponsiveToolbarWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto main_dock = std::make_shared<layout::dock_panel>();
    
    // === Wrap panel for responsive toolbar ===
    auto toolbar_wrap = std::make_shared<layout::wrap_panel>(layout::orientation::horizontal);
    toolbar_wrap->set_padding(5, 5, 5, 5);
    toolbar_wrap->set_item_width(80);  // Uniform button width
    toolbar_wrap->set_item_height(30); // Uniform button height
    
    // Add many toolbar buttons
    const TCHAR* button_labels[] = {
        _T("New"), _T("Open"), _T("Save"), _T("Close"),
        _T("Cut"), _T("Copy"), _T("Paste"), _T("Delete"),
        _T("Undo"), _T("Redo"), _T("Find"), _T("Replace"),
        _T("Print"), _T("Preview"), _T("Settings"), _T("Help")
    };
    
    for (const auto& label : button_labels) {
        auto btn = create_button(label, 0, 0, 75, 25);
        toolbar_wrap->add(btn);
    }
    
    main_dock->add(toolbar_wrap, layout::dock_position::top);
    
    // === Main content ===
    auto content = create_edit_text(_T("Main Content"), 100,
        WS_CHILD | WS_VISIBLE | ES_MULTILINE);
    main_dock->add(content, layout::dock_position::fill);
    
    set_layout(main_dock);
    
    // When window is wide: all buttons in one row
    // When window is narrow: buttons wrap to multiple rows
    
    return window::on_create(hWnd, wParam, lParam);
}
```

## Example 7: Wizard Dialog

A multi-step wizard with navigation buttons.

```cpp
class WizardDialog : public wpp::dialog {
public:
    WizardDialog(HINSTANCE hInstance)
        : dialog(hInstance, IDD_WIZARD), m_current_page(0) {}
    
    message_handler on_init_dialog override;
    
private:
    int m_current_page;
    control_ptr<static_text> m_page_title;
    control_ptr<static_text> m_page_content;
    control_ptr<button> m_back;
    control_ptr<button> m_next;
    control_ptr<button> m_cancel;
    
    void show_page(int page);
};

INT_PTR WizardDialog::on_init_dialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto main_dock = std::make_shared<layout::dock_panel>();
    
    // === Navigation buttons at bottom ===
    auto nav_buttons = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
    nav_buttons->set_padding(10, 10, 10, 10);
    nav_buttons->set_spacing(5);
    
    m_back = create_button(_T("< Back"));
    m_back->on_click([this](WPARAM, LPARAM) {
        if (m_current_page > 0) {
            show_page(--m_current_page);
        }
    });
    
    m_next = create_button(_T("Next >"));
    m_next->on_click([this](WPARAM, LPARAM) {
        if (m_current_page < 2) {
            show_page(++m_current_page);
        } else {
            end_dialog(IDOK);  // Finish
        }
    });
    
    m_cancel = create_button(_T("Cancel"));
    m_cancel->on_click([this](WPARAM, LPARAM) {
        end_dialog(IDCANCEL);
    });
    
    nav_buttons->add(m_back);
    nav_buttons->add(m_next);
    nav_buttons->add(m_cancel, layout::alignment::end);  // Right-align cancel
    
    main_dock->add(nav_buttons, layout::dock_position::bottom);
    
    // === Page content ===
    auto content_stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    content_stack->set_padding(20, 20, 20, 20);
    content_stack->set_spacing(10);
    
    m_page_title = create_static_text(_T(""));
    m_page_content = create_static_text(_T(""));
    
    content_stack->add(m_page_title);
    content_stack->add(m_page_content);
    
    main_dock->add(content_stack, layout::dock_position::fill);
    
    set_layout(main_dock);
    
    show_page(0);
    
    return dialog::on_init_dialog(hWnd, wParam, lParam);
}

void WizardDialog::show_page(int page) {
    switch (page) {
        case 0:
            m_page_title->set_text(_T("Welcome"));
            m_page_content->set_text(_T("Welcome to the setup wizard..."));
            m_back->enable(false);
            m_next->set_text(_T("Next >"));
            break;
        case 1:
            m_page_title->set_text(_T("Configuration"));
            m_page_content->set_text(_T("Configure your settings..."));
            m_back->enable(true);
            m_next->set_text(_T("Next >"));
            break;
        case 2:
            m_page_title->set_text(_T("Finish"));
            m_page_content->set_text(_T("Setup is complete!"));
            m_back->enable(true);
            m_next->set_text(_T("Finish"));
            break;
    }
}
```

## Key Takeaways

1. **StackPanel** is perfect for simple linear layouts (forms, button rows)
2. **Grid** excels at complex forms with aligned labels and inputs
3. **DockPanel** is ideal for application frames (toolbar, statusbar, sidebar)
4. **WrapPanel** creates responsive layouts that adapt to window size
5. **Nesting panels** enables complex, professional-looking layouts
6. **Alignment options** provide fine control without manual positioning
7. **No coordinate calculations** - just compose panels logically

All these examples would **automatically resize** when the window is resized, making them much more maintainable than manual positioning!

---

**Status**: These are design examples. Implementation pending stakeholder approval.
For more information, see [LayoutManager_DesignPlan.md](LayoutManager_DesignPlan.md).
