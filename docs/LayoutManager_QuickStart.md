# Layout Manager Quick Start Guide

## Overview

The Windows++ Layout Manager eliminates manual coordinate-based positioning by providing XAML-inspired layout panels. Instead of calculating x, y positions for each control, you simply add controls to panels and let the layout system handle positioning automatically.

## Before and After Comparison

### ❌ Old Way (Manual Positioning)
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    m_Button1 = create_button(_T("Click Me!"), 10, 0, 150, 25);
    m_Button2 = create_button(_T("OK"), 10, 30, 150, 25);
    m_Button3 = create_button(_T("Cancel"), 10, 60, 150, 25);
    // If window resizes, buttons stay in fixed positions
    // Adding a new button requires recalculating all positions
    return window::on_create(hWnd, wParam, lParam);
}
```

### ✅ New Way (Layout Panels)
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    stack->set_padding(10, 10, 10, 10);
    stack->set_spacing(5);
    
    m_Button1 = create_button(_T("Click Me!"), 0, 0, 150, 25);
    m_Button2 = create_button(_T("OK"), 0, 0, 150, 25);
    m_Button3 = create_button(_T("Cancel"), 0, 0, 150, 25);
    
    stack->add(m_Button1);
    stack->add(m_Button2);
    stack->add(m_Button3);
    
    set_layout(stack);  // Layout handles positioning & resizing!
    
    return window::on_create(hWnd, wParam, lParam);
}
```

## Available Layout Panels

### 1. StackPanel - Linear Layout
Arranges controls in a single row or column with optional spacing.

**When to use**: Forms, toolbars, button groups, simple lists

```cpp
auto stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
stack->set_spacing(5);  // 5 pixels between controls
stack->add(button1);
stack->add(button2);
stack->add(button3);
```

**Horizontal example**:
```cpp
auto toolbar = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
toolbar->add(new_button);
toolbar->add(open_button);
toolbar->add(save_button);
```

### 2. DockPanel - Edge Docking
Docks controls to edges (top, bottom, left, right) with one control filling remaining space.

**When to use**: Application frames with toolbars, status bars, sidebars

```cpp
auto dock = std::make_shared<layout::dock_panel>();
dock->add(toolbar, layout::dock_position::top);
dock->add(statusbar, layout::dock_position::bottom);
dock->add(sidebar, layout::dock_position::left);
dock->add(main_content, layout::dock_position::fill);  // Takes remaining space
```

### 3. Grid - Table Layout
Arranges controls in rows and columns with flexible sizing.

**When to use**: Complex forms, data entry screens, spreadsheet-like layouts

```cpp
auto grid = std::make_shared<layout::grid>();

// Define columns: 100px, auto-size, fills remaining space
grid->add_column({layout::grid::column_definition::size_type::absolute, 100});
grid->add_column({layout::grid::column_definition::size_type::auto_size});
grid->add_column({layout::grid::column_definition::size_type::star, 1.0});

// Define rows
grid->add_row({layout::grid::row_definition::size_type::auto_size});
grid->add_row({layout::grid::row_definition::size_type::star, 1.0});

// Add controls to cells
grid->add(label, 0, 0);              // Row 0, Column 0
grid->add(textbox, 0, 1, 1, 2);      // Row 0, Columns 1-2 (spans 2 columns)
```

### 4. WrapPanel - Flowing Layout
Controls flow left-to-right, wrapping to next line when space runs out.

**When to use**: Tag clouds, image galleries, flexible button groups

```cpp
auto wrap = std::make_shared<layout::wrap_panel>(layout::orientation::horizontal);
wrap->add(tag1);
wrap->add(tag2);
wrap->add(tag3);
// Tags automatically wrap to next line when window is too narrow
```

## Nested Layouts

Panels can contain other panels for complex layouts:

```cpp
// Main layout: dock panel
auto main_dock = std::make_shared<layout::dock_panel>();

// Toolbar: horizontal stack of buttons
auto toolbar = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
toolbar->add(new_btn);
toolbar->add(open_btn);
toolbar->add(save_btn);

// Sidebar: vertical stack
auto sidebar = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
sidebar->add(tree_view);
sidebar->add(properties_panel);

// Compose
main_dock->add(toolbar, layout::dock_position::top);
main_dock->add(sidebar, layout::dock_position::left);
main_dock->add(main_content, layout::dock_position::fill);

set_layout(main_dock);
```

## Common Patterns

### Form with Labels and Inputs
```cpp
auto grid = std::make_shared<layout::grid>();
grid->add_column({layout::grid::column_definition::size_type::auto_size});  // Labels
grid->add_column({layout::grid::column_definition::size_type::star, 1.0});   // Inputs

grid->add(create_static_text(_T("Name:")), 0, 0);
grid->add(create_edit_text(_T("")), 0, 1);

grid->add(create_static_text(_T("Email:")), 1, 0);
grid->add(create_edit_text(_T("")), 1, 1);

grid->add(create_static_text(_T("Phone:")), 2, 0);
grid->add(create_edit_text(_T("")), 2, 1);
```

### Dialog with OK/Cancel Buttons
```cpp
auto main_stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
main_stack->set_padding(10, 10, 10, 10);

// Content area
main_stack->add(create_static_text(_T("Are you sure?")));

// Button row
auto button_row = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
button_row->set_spacing(5);
button_row->add(create_button(_T("OK"), 0, 0, 75, 25));
button_row->add(create_button(_T("Cancel"), 0, 0, 75, 25));

main_stack->add(button_row);
set_layout(main_stack);
```

### Application Window
```cpp
auto app_dock = std::make_shared<layout::dock_panel>();

// Menu bar at top
app_dock->add(menu_bar, layout::dock_position::top);

// Status bar at bottom
app_dock->add(status_bar, layout::dock_position::bottom);

// Tool palette on left
app_dock->add(tool_palette, layout::dock_position::left);

// Properties panel on right
app_dock->add(properties_panel, layout::dock_position::right);

// Main canvas fills remaining space
app_dock->add(canvas, layout::dock_position::fill);

set_layout(app_dock);
```

## Layout Properties

### Margin and Padding
```cpp
auto stack = std::make_shared<layout::stack_panel>();

// Padding: space inside the panel
stack->set_padding(10, 10, 10, 10);  // left, top, right, bottom

// Margin: space outside the panel (when nested)
stack->set_margin(5, 5, 5, 5);
```

### Spacing
```cpp
auto stack = std::make_shared<layout::stack_panel>();
stack->set_spacing(5);  // 5 pixels between each control
```

### Alignment
```cpp
auto stack = std::make_shared<layout::stack_panel>();
stack->add(button1, layout::alignment::center);
stack->add(button2, layout::alignment::start);   // Left/Top
stack->add(button3, layout::alignment::end);     // Right/Bottom
stack->add(button4, layout::alignment::stretch); // Fill width
```

## DPI Awareness

Layout panels automatically handle DPI scaling. No additional code needed!

```cpp
// Works correctly on all DPI settings:
// 96 DPI (100%), 120 DPI (125%), 144 DPI (150%), 192 DPI (200%)
auto stack = std::make_shared<layout::stack_panel>();
stack->set_spacing(5);  // Automatically scaled for DPI
```

## Window Resizing

Layouts automatically recalculate when the window is resized. No code needed!

```cpp
// Window resize is handled automatically
set_layout(my_panel);

// If you need to manually trigger layout:
invalidate_layout();  // Mark as needing recalculation
update_layout();      // Force immediate recalculation
```

## Migration from Manual Positioning

### Step 1: Keep x, y coordinates but set them to 0
```cpp
// Before:
auto btn = create_button(_T("Click"), 10, 30, 150, 25);

// After:
auto btn = create_button(_T("Click"), 0, 0, 150, 25);  // x, y not used in layout
```

### Step 2: Create a layout panel
```cpp
auto stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
stack->set_padding(10, 10, 10, 10);  // Add padding like your old margins
```

### Step 3: Add controls to panel
```cpp
stack->add(btn1);
stack->add(btn2);
stack->add(btn3);
```

### Step 4: Set the layout
```cpp
set_layout(stack);
```

## Tips and Best Practices

### ✅ DO:
- Use StackPanel for simple linear layouts
- Use Grid for complex forms
- Use DockPanel for application frames
- Nest panels for complex layouts
- Set padding/spacing for visual breathing room
- Use star sizing in Grid for responsive columns

### ❌ DON'T:
- Mix manual positioning with layout panels
- Create deeply nested layouts (3-4 levels max)
- Forget to call `set_layout()` after building the panel tree
- Manually position controls that are in a layout panel

## Troubleshooting

**Controls not appearing?**
- Make sure you called `set_layout()`
- Check that controls have non-zero width/height
- Verify the panel has space to display controls

**Layout not updating on resize?**
- WM_SIZE handler should trigger automatically
- Try calling `invalidate_layout()` manually

**Controls overlapping?**
- Check spacing and padding values
- Verify you're not mixing manual positioning with layout

**Performance issues?**
- Limit nesting depth
- Use fewer panels for simple layouts
- Consider manual positioning for very large control counts (100+)

## Next Steps

1. Read the full [Layout Manager Design Plan](LayoutManager_DesignPlan.md)
2. Check out [example implementations](../TestProj/TestProj/LayoutWindow.cpp) (once available)
3. Experiment with different panel types
4. Provide feedback on the design

---

**Status**: Design Phase - Implementation Pending
**Feedback**: Please review and provide input on panel types, API design, and use cases
