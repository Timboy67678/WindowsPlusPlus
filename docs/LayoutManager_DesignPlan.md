# Windows++ Layout Manager Design Plan

## Executive Summary

This document outlines the design for a XAML-inspired layout manager system for Windows++. The goal is to eliminate manual coordinate-based positioning and provide a declarative, automatic layout system similar to WPF/XAML panels while maintaining the library's Win32-based, lightweight architecture.

## Current State Analysis

### Existing Positioning System
- **Method**: Manual absolute positioning with pixel coordinates
- **Example**: `create_button(_T("Click Me!"), 10, 30, 150, 25)`
- **Problems**:
  - Tedious manual coordinate calculations
  - Hard to maintain when adding/removing controls
  - No automatic resizing or responsive layouts
  - Difficult to create complex, nested layouts
  - No support for DPI-aware layouts
  - Changes require recalculating all control positions

### Current API Surface
```cpp
// Window class (Window.hpp)
control_ptr<button> create_button(text, x, y, width, height, style, style_ex);
control_ptr<check_box> create_check_box(text, x, y, width, height, style, style_ex);
// ... 20+ similar methods

// Dialog class (Dialog.hpp)
template<typename CtrlType = control>
bool register_control(UINT control_id, control_ptr<CtrlType>& ctrl);
```

## Design Goals

1. **Backward Compatibility**: Keep existing manual positioning API unchanged
2. **XAML-Inspired**: Use familiar layout concepts (StackPanel, Grid, DockPanel, etc.)
3. **Declarative**: Enable fluent, readable layout descriptions
4. **Lightweight**: Maintain Win32 performance with minimal overhead
5. **Type-Safe**: Leverage C++ templates and concepts
6. **DPI-Aware**: Support high-DPI displays automatically
7. **Composable**: Allow nesting panels for complex layouts
8. **Resizable**: Automatically adjust layouts when windows resize

## Proposed Architecture

### 1. Core Components

#### 1.1 Base Layout Panel Class
```cpp
namespace wpp::layout {

    enum class orientation {
        horizontal,
        vertical
    };

    enum class alignment {
        start,      // Left/Top
        center,
        end,        // Right/Bottom
        stretch     // Fill available space
    };

    enum class dock_position {
        left,
        top,
        right,
        bottom,
        fill        // Takes remaining space
    };

    // Base class for all layout panels
    class panel {
    public:
        virtual ~panel() = default;
        
        // Add a control to this panel
        virtual void add(std::shared_ptr<hwnd> control) = 0;
        
        // Layout calculations
        virtual void measure(int available_width, int available_height) = 0;
        virtual void arrange(int x, int y, int width, int height) = 0;
        
        // Parent/child relationships
        void set_parent_window(HWND parent) { m_parent = parent; }
        HWND get_parent_window() const { return m_parent; }
        
        // Margin and padding
        void set_margin(int left, int top, int right, int bottom);
        void set_padding(int left, int top, int right, int bottom);
        
        // DPI support
        void set_dpi_scale(float scale) { m_dpi_scale = scale; }
        
    protected:
        HWND m_parent = nullptr;
        std::vector<std::shared_ptr<hwnd>> m_children;
        
        // Layout properties
        struct margin_t { int left, top, right, bottom; } m_margin{0,0,0,0};
        struct padding_t { int left, top, right, bottom; } m_padding{0,0,0,0};
        float m_dpi_scale = 1.0f;
        
        // Calculated sizes
        struct size_t { int width, height; };
        size_t m_desired_size{0, 0};
        size_t m_actual_size{0, 0};
    };
}
```

#### 1.2 StackPanel - Linear Layout
```cpp
class stack_panel : public panel {
public:
    stack_panel(orientation orient = orientation::vertical)
        : m_orientation(orient) {}
    
    void add(std::shared_ptr<hwnd> control) override;
    void add(std::shared_ptr<hwnd> control, alignment align);
    
    void measure(int available_width, int available_height) override;
    void arrange(int x, int y, int width, int height) override;
    
    void set_orientation(orientation orient) { m_orientation = orient; }
    void set_spacing(int spacing) { m_spacing = spacing; }
    
private:
    orientation m_orientation;
    int m_spacing = 5;  // Default spacing between controls
    std::vector<alignment> m_child_alignments;
};
```

#### 1.3 Grid - Table Layout
```cpp
class grid : public panel {
public:
    struct column_definition {
        enum class size_type { absolute, star, auto_size };
        size_type type = size_type::star;
        double value = 1.0;  // For star sizing
        int min_width = 0;
        int max_width = INT_MAX;
    };
    
    struct row_definition {
        enum class size_type { absolute, star, auto_size };
        size_type type = size_type::star;
        double value = 1.0;
        int min_height = 0;
        int max_height = INT_MAX;
    };
    
    void add_column(column_definition def);
    void add_row(row_definition def);
    
    void add(std::shared_ptr<hwnd> control, int row, int col) override;
    void add(std::shared_ptr<hwnd> control, int row, int col, 
             int row_span, int col_span);
    
    void measure(int available_width, int available_height) override;
    void arrange(int x, int y, int width, int height) override;
    
private:
    std::vector<column_definition> m_columns;
    std::vector<row_definition> m_rows;
    std::map<std::shared_ptr<hwnd>, std::tuple<int,int,int,int>> m_cell_map; // row, col, rowspan, colspan
};
```

#### 1.4 DockPanel - Edge Docking
```cpp
class dock_panel : public panel {
public:
    void add(std::shared_ptr<hwnd> control, dock_position pos);
    
    void measure(int available_width, int available_height) override;
    void arrange(int x, int y, int width, int height) override;
    
    void set_last_child_fill(bool fill) { m_last_child_fill = fill; }
    
private:
    std::vector<dock_position> m_dock_positions;
    bool m_last_child_fill = true;
};
```

#### 1.5 WrapPanel - Flowing Layout
```cpp
class wrap_panel : public panel {
public:
    wrap_panel(orientation orient = orientation::horizontal)
        : m_orientation(orient) {}
    
    void add(std::shared_ptr<hwnd> control) override;
    
    void measure(int available_width, int available_height) override;
    void arrange(int x, int y, int width, int height) override;
    
    void set_orientation(orientation orient) { m_orientation = orient; }
    void set_item_width(int width) { m_item_width = width; }
    void set_item_height(int height) { m_item_height = height; }
    
private:
    orientation m_orientation;
    int m_item_width = -1;   // -1 = auto
    int m_item_height = -1;  // -1 = auto
};
```

### 2. Integration with Window Class

#### 2.1 Enhanced Window API
```cpp
// In Window.hpp - Add layout support
class window : public hwnd {
public:
    // Existing constructors...
    
    // New layout methods
    void set_layout(std::shared_ptr<layout::panel> layout_panel);
    std::shared_ptr<layout::panel> get_layout() const;
    
    // Trigger layout recalculation
    void invalidate_layout();
    void update_layout();
    
    // Existing methods remain unchanged...
    control_ptr<button> create_button(const std::tstring& text, int x, int y, 
                                     int width, int height, DWORD style, DWORD style_ex);
    
protected:
    // Handle WM_SIZE to trigger layout updates
    virtual message_handler on_size;
    
private:
    std::shared_ptr<layout::panel> m_layout_panel;
    bool m_use_layout = false;
};
```

#### 2.2 Control Creation with Layout Support
```cpp
// New helper methods for layout-managed controls
template<typename ControlType, typename... Args>
std::shared_ptr<ControlType> create_control(Args&&... args) {
    // Create control but don't position it yet
    auto control = std::make_shared<ControlType>(std::forward<Args>(args)...);
    m_controls.emplace_back(control);
    return control;
}
```

### 3. Usage Examples

#### 3.1 Simple Vertical Stack
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // Create a vertical stack panel
    auto stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    stack->set_padding(10, 10, 10, 10);
    stack->set_spacing(5);
    
    // Create controls and add to stack
    auto btn1 = create_button(_T("Button 1"), 0, 0, 150, 30);
    auto btn2 = create_button(_T("Button 2"), 0, 0, 150, 30);
    auto btn3 = create_button(_T("Button 3"), 0, 0, 150, 30);
    
    stack->add(btn1);
    stack->add(btn2);
    stack->add(btn3);
    
    // Set the layout
    set_layout(stack);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

#### 3.2 Grid Layout
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto grid = std::make_shared<layout::grid>();
    
    // Define 3 columns: fixed, auto, star
    grid->add_column({layout::grid::column_definition::size_type::absolute, 100});
    grid->add_column({layout::grid::column_definition::size_type::auto_size});
    grid->add_column({layout::grid::column_definition::size_type::star, 1.0});
    
    // Define 2 rows
    grid->add_row({layout::grid::row_definition::size_type::auto_size});
    grid->add_row({layout::grid::row_definition::size_type::star, 1.0});
    
    // Add controls to grid cells
    auto label = create_static_text(_T("Name:"), 0, 0, 50, 20);
    auto edit = create_edit_text(0, 0, 200, 20, _T(""));
    auto list = create_list_view(0, 0, 400, 300);
    
    grid->add(label, 0, 0);        // Row 0, Col 0
    grid->add(edit, 0, 1, 1, 2);   // Row 0, Col 1-2 (span 2 columns)
    grid->add(list, 1, 0, 1, 3);   // Row 1, Col 0-2 (span 3 columns)
    
    set_layout(grid);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

#### 3.3 Dock Panel Layout
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto dock = std::make_shared<layout::dock_panel>();
    
    // Create toolbar, status bar, sidebar, and main area
    auto toolbar = create_static_text(_T("Toolbar"), 0, 0, 100, 30);
    auto statusbar = create_static_text(_T("Status Bar"), 0, 0, 100, 25);
    auto sidebar = create_list_box(0, 0, 150, 400);
    auto content = create_edit_text(0, 0, 500, 400, _T("Main Content Area"));
    
    dock->add(toolbar, layout::dock_position::top);
    dock->add(statusbar, layout::dock_position::bottom);
    dock->add(sidebar, layout::dock_position::left);
    dock->add(content, layout::dock_position::fill);  // Takes remaining space
    
    set_layout(dock);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

#### 3.4 Nested Layouts
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // Main dock panel
    auto main_dock = std::make_shared<layout::dock_panel>();
    
    // Top toolbar as horizontal stack
    auto toolbar_stack = std::make_shared<layout::stack_panel>(layout::orientation::horizontal);
    toolbar_stack->set_spacing(5);
    auto btn_new = create_button(_T("New"), 0, 0, 60, 25);
    auto btn_open = create_button(_T("Open"), 0, 0, 60, 25);
    auto btn_save = create_button(_T("Save"), 0, 0, 60, 25);
    toolbar_stack->add(btn_new);
    toolbar_stack->add(btn_open);
    toolbar_stack->add(btn_save);
    
    // Side panel as vertical stack
    auto sidebar_stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
    auto tree = create_tree_view(0, 0, 200, 400);
    auto properties = create_list_view(0, 0, 200, 200);
    sidebar_stack->add(tree);
    sidebar_stack->add(properties);
    
    // Main content area
    auto content = create_edit_text(0, 0, 500, 400, _T(""));
    
    // Compose the layout
    main_dock->add(toolbar_stack, layout::dock_position::top);
    main_dock->add(sidebar_stack, layout::dock_position::left);
    main_dock->add(content, layout::dock_position::fill);
    
    set_layout(main_dock);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

#### 3.5 Fluent Builder API (Optional Enhancement)
```cpp
LRESULT MainWindow::on_create(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // Alternative fluent API
    auto layout = layout::builder()
        .stack_vertical()
            .padding(10)
            .spacing(5)
            .add(create_button(_T("Button 1"), 0, 0, 150, 30))
            .add(create_button(_T("Button 2"), 0, 0, 150, 30))
            .add_stack_horizontal()
                .spacing(5)
                .add(create_button(_T("OK"), 0, 0, 75, 25))
                .add(create_button(_T("Cancel"), 0, 0, 75, 25))
            .end()
        .end()
        .build();
    
    set_layout(layout);
    
    return window::on_create(hWnd, wParam, lParam);
}
```

## Implementation Plan

### Phase 1: Core Infrastructure (Week 1-2)
1. Create `layout/` directory with base classes
2. Implement `panel` base class with measure/arrange pattern
3. Add layout support to `window` class
4. Implement WM_SIZE message handler for layout updates
5. Add DPI awareness support

**Files to Create:**
- `layout/panel.hpp` - Base panel class
- `layout/panel.cpp` - Base panel implementation
- `layout/common.hpp` - Common layout types and enums

**Files to Modify:**
- `Window.hpp` - Add layout support methods
- `Window.cpp` - Implement layout integration
- `common.hpp` - May need layout-aware positioning helpers

### Phase 2: Basic Panels (Week 3)
1. Implement `stack_panel` (most commonly used)
2. Implement `dock_panel` (useful for app layouts)
3. Add unit tests for layout calculations
4. Create simple example in TestProj

**Files to Create:**
- `layout/stack_panel.hpp`
- `layout/stack_panel.cpp`
- `layout/dock_panel.hpp`
- `layout/dock_panel.cpp`
- `TestProj/TestProj/LayoutWindow.hpp` (example)
- `TestProj/TestProj/LayoutWindow.cpp` (example)

### Phase 3: Advanced Panels (Week 4)
1. Implement `grid` panel
2. Implement `wrap_panel`
3. Add support for nested panels
4. Enhance examples with complex layouts

**Files to Create:**
- `layout/grid.hpp`
- `layout/grid.cpp`
- `layout/wrap_panel.hpp`
- `layout/wrap_panel.cpp`

### Phase 4: Polish & Documentation (Week 5)
1. Add comprehensive documentation
2. Create tutorial and examples
3. Performance optimization
4. Add layout debugging/visualization tools (optional)

**Files to Create:**
- `docs/LayoutManager_Tutorial.md`
- `docs/LayoutManager_API.md`
- Additional examples in TestProj

## Technical Considerations

### 1. Measure-Arrange Pattern
Following WPF's two-pass layout system:
- **Measure Pass**: Each panel asks children "how much space do you need?"
- **Arrange Pass**: Each panel tells children "here's your final position and size"

### 2. DPI Awareness
```cpp
float get_dpi_scale(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(hwnd, hdc);
    return dpi / 96.0f;  // 96 DPI is baseline
}
```

### 3. Performance
- Layout calculations only on WM_SIZE or explicit `invalidate_layout()`
- Cache calculated positions to avoid recalculation
- Use `SetWindowPos` with `SWP_NOACTIVATE | SWP_NOZORDER` for batch updates

### 4. Backward Compatibility
- All existing APIs remain unchanged
- Layout system is opt-in via `set_layout()`
- Manual positioning still works if layout is not set

### 5. Memory Management
- Use `std::shared_ptr` for panels and controls
- Follow existing Windows++ patterns for memory management
- Ensure proper cleanup in destructors

## Alternative Approaches Considered

### 1. Constraint-Based Layout (like Qt)
**Pros**: Very flexible, can express complex relationships
**Cons**: More complex to implement, harder to understand, performance overhead
**Decision**: Not chosen - XAML panels are simpler and more intuitive

### 2. Flexbox-Style Layout (like CSS)
**Pros**: Modern, web developers familiar with it
**Cons**: Not familiar to Windows developers, doesn't map well to Win32
**Decision**: Stack panel is similar enough to flex for most use cases

### 3. Anchoring/Docking (like WinForms)
**Pros**: Simple to understand, good for simple layouts
**Cons**: Limited composability, hard to create complex nested layouts
**Decision**: Implemented via dock_panel, but not as primary system

## Testing Strategy

### Unit Tests
```cpp
TEST_CASE("StackPanel vertical layout") {
    auto stack = layout::stack_panel(layout::orientation::vertical);
    stack.set_spacing(5);
    
    auto ctrl1 = create_mock_control(100, 30);
    auto ctrl2 = create_mock_control(100, 30);
    
    stack.add(ctrl1);
    stack.add(ctrl2);
    
    stack.measure(200, 200);
    REQUIRE(stack.desired_size().height == 65);  // 30 + 5 + 30
    
    stack.arrange(0, 0, 200, 200);
    REQUIRE(ctrl1.position() == std::make_pair(0, 0));
    REQUIRE(ctrl2.position() == std::make_pair(0, 35));
}
```

### Integration Tests
- Create test windows with various layouts
- Verify controls are positioned correctly
- Test resizing behavior
- Test nested layouts

### Manual Testing
- Create examples in TestProj
- Test on different DPI settings
- Test on different Windows versions
- Verify performance with many controls

## Documentation Requirements

1. **API Documentation**: Complete XML documentation for all public members
2. **Tutorial**: Step-by-step guide to using layout panels
3. **Examples**: At least 5 real-world layout examples
4. **Migration Guide**: How to convert existing manual layouts
5. **Best Practices**: When to use which panel type

## Future Enhancements (Post-MVP)

1. **Canvas Panel**: Absolute positioning within a container
2. **Uniform Grid**: Grid where all cells are equal size
3. **Tab Panel**: Integration with tab control for tabbed layouts
4. **Splitter**: Resizable split containers
5. **Layout Animations**: Smooth transitions when layout changes
6. **Visual Studio Designer**: Design-time layout editor (ambitious)
7. **Layout Templates**: Predefined layouts for common scenarios
8. **Responsive Layouts**: Different layouts based on window size

## Success Criteria

1. ✅ Backward compatible with existing code
2. ✅ Reduces code needed for common layouts by 50%+
3. ✅ Automatically handles window resizing
4. ✅ DPI-aware by default
5. ✅ Performance overhead < 5% compared to manual positioning
6. ✅ Documentation complete and examples provided
7. ✅ No memory leaks or crashes
8. ✅ Works on Windows 7 through Windows 11

## Timeline Estimate

- **Phase 1** (Core Infrastructure): 2 weeks
- **Phase 2** (Basic Panels): 1 week
- **Phase 3** (Advanced Panels): 1 week
- **Phase 4** (Polish & Documentation): 1 week
- **Total**: 5 weeks for full implementation

For initial prototype: Could deliver Phase 1 + basic StackPanel in 1-2 weeks.

## Open Questions for Discussion

1. Should we support layout in `dialog` class or just `window` class?
2. Do we need a visual designer, or is code-based layout sufficient?
3. What level of XAML compatibility is desired? (naming, behavior, etc.)
4. Should we add layout serialization (save/load layouts from files)?
5. Performance target: how many controls should we optimize for?
6. Should we support custom layout panels via inheritance?

## Conclusion

This layout manager system will significantly improve the Windows++ developer experience by eliminating tedious manual positioning while maintaining the library's Win32 foundation and performance characteristics. The XAML-inspired design will be familiar to Windows developers and provides the flexibility needed for complex UIs.

The phased approach allows for incremental delivery and feedback, with the core infrastructure and most useful panels (StackPanel, DockPanel) delivered first, followed by more advanced features.

---

**Document Version**: 1.0
**Date**: 2026-02-13
**Author**: GitHub Copilot
**Status**: Draft - Awaiting Review
