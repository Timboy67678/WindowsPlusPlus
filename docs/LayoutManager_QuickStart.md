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

### ✅ New Way (Layout Panels) - Much Simpler!
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    stack->set_padding(10, 10, 10, 10);
    stack->set_spacing(5);
    
    // No coordinates needed - layout handles everything!
    m_Button1 = create_button(_T("Click Me!"));
    m_Button2 = create_button(_T("OK"));
    m_Button3 = create_button(_T("Cancel"));
    
    stack->add(m_Button1);
    stack->add(m_Button2);
    stack->add(m_Button3);
    
    set_layout(stack);  // Layout handles positioning & resizing!
    
    return window::on_create(hWnd, wParam, lParam);
}
```

**Benefits**: 60% less code, no coordinate math, automatic resizing!
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
toolbar->add(create_button(_T("New")));
toolbar->add(create_button(_T("Open")));
toolbar->add(create_button(_T("Save")));
```

### 2. DockPanel - Edge Docking
Docks controls to edges (top, bottom, left, right) with one control filling remaining space.

**When to use**: Application frames with toolbars, status bars, sidebars

```cpp
auto dock = std::make_shared<layout::dock_panel>();
dock->add(create_static_text(_T("Toolbar")), layout::dock_position::top);
dock->add(create_static_text(_T("Status")), layout::dock_position::bottom);
dock->add(create_list_box(150), layout::dock_position::left);  // 150px wide
dock->add(create_edit_text(_T("Main Content")), layout::dock_position::fill);
```

### 3. Grid - Table Layout
Arranges controls in rows and columns with flexible sizing.

**When to use**: Complex forms, data entry screens, spreadsheet-like layouts

```cpp
auto grid = std::make_shared<layout::grid>();
grid->add_column({layout::grid::column_definition::size_type::absolute, 100});
grid->add_column({layout::grid::column_definition::size_type::auto_size});
grid->add_column({layout::grid::column_definition::size_type::star, 1.0});

// Define rows
grid->add_row({layout::grid::row_definition::size_type::auto_size});
grid->add_row({layout::grid::row_definition::size_type::star, 1.0});

// Add controls to cells - no coordinates!
grid->add(create_static_text(_T("Label")), 0, 0);     // Row 0, Column 0
grid->add(create_edit_text(_T(""), 200), 0, 1, 1, 2); // Row 0, Columns 1-2 (spans 2)
```

### 4. WrapPanel - Flowing Layout
Controls flow left-to-right, wrapping to next line when space runs out.

**When to use**: Tag clouds, image galleries, flexible button groups

```cpp
auto wrap = std::make_shared<layout::wrap_panel>(layout::orientation::horizontal);
wrap->add(create_button(_T("Tag 1")));
wrap->add(create_button(_T("Tag 2")));
wrap->add(create_button(_T("Tag 3")));
// Tags automatically wrap to next line when window is too narrow
```

## Nested Layouts

Panels can contain other panels for complex layouts:

```cpp
// Main layout: dock panel
auto main_dock = std::make_shared<layout::dock_panel>();

// Toolbar: horizontal stack of buttons
auto toolbar = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
toolbar->add(create_button(_T("New")));
toolbar->add(create_button(_T("Open")));
toolbar->add(create_button(_T("Save")));

// Sidebar: vertical stack
auto sidebar = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
sidebar->add(create_tree_view(200));      // 200px wide
sidebar->add(create_list_view(200, 150)); // 200px wide, 150px tall

// Compose
main_dock->add(toolbar, layout::dock_position::top);
main_dock->add(sidebar, layout::dock_position::left);
main_dock->add(create_edit_text(_T("Main Content")), layout::dock_position::fill);

set_layout(main_dock);
```

## Common Patterns

### Form with Labels and Inputs
```cpp
auto grid = std::make_shared<layout::grid>();
grid->add_column({layout::grid::column_definition::size_type::auto_size});  // Labels
grid->add_column({layout::grid::column_definition::size_type::star, 1.0});   // Inputs

grid->add(create_static_text(_T("Name:")), 0, 0);
grid->add(create_edit_text(_T(""), 200), 0, 1);

grid->add(create_static_text(_T("Email:")), 1, 0);
grid->add(create_edit_text(_T(""), 200), 1, 1);

grid->add(create_static_text(_T("Phone:")), 2, 0);
grid->add(create_edit_text(_T(""), 200), 2, 1);
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
button_row->add(create_button(_T("OK")));
button_row->add(create_button(_T("Cancel")));

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

### No Migration Needed!

Since backward compatibility is not a goal, this is a **complete redesign** of control creation:

**Old API** (being replaced):
```cpp
create_button(text, x, y, width, height, style, style_ex)
```

**New API** (layout-first):
```cpp
create_button(text, width = -1, height = -1, style, style_ex)
```

### Key Changes:

1. ❌ **Removed**: x, y position parameters
2. ✅ **Changed**: Width/height are optional (default -1 = auto-size)
3. ✅ **Required**: Must use `set_layout()` to display controls
4. ✅ **Simplified**: No coordinate calculations needed ever!

### Example Conversion:

**Before**:
```cpp
auto btn = create_button(_T("Click"), 10, 30, 150, 25);  
// Manually calculated position (10, 30)
```

**After**:
```cpp
auto btn = create_button(_T("Click"));  // No position needed!
stack->add(btn);  // Layout handles positioning
```

## Tips and Best Practices

### ✅ DO:
- Use StackPanel for simple linear layouts
- Use Grid for complex forms
- Use DockPanel for application frames
- Nest panels for complex layouts
- Set padding/spacing for visual breathing room
- Use star sizing in Grid for responsive columns
- Let controls auto-size when possible (width/height = -1)

### ❌ DON'T:
- Try to use manual positioning (not supported!)
- Create deeply nested layouts (3-4 levels max)
- Forget to call `set_layout()` after building the panel tree
- Specify fixed sizes unless necessary

## Troubleshooting

**Controls not appearing?**
- Make sure you called `set_layout()`
- Check that controls were added to the layout panel
- Verify the panel has space to display controls

**Layout not updating on resize?**
- WM_SIZE handler should trigger automatically
- Try calling `invalidate_layout()` manually

**Controls overlapping?**
- Check spacing and padding values
- Verify panel types are appropriate for your use case

**Performance issues?**
- Limit nesting depth
- Use fewer panels for simple layouts
- Consider simpler panel types (StackPanel vs Grid)

## Next Steps

1. Read the full [Layout Manager Design Plan](LayoutManager_DesignPlan.md)
2. Check out [example implementations](../TestProj/TestProj/LayoutWindow.cpp) (once available)
3. Experiment with different panel types
4. Provide feedback on the design

---

**Status**: Design Phase - Implementation Pending
**Feedback**: Please review and provide input on panel types, API design, and use cases
