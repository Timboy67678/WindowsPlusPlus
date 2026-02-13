# Layout Manager Architecture Diagram

## Class Hierarchy

```
┌─────────────────────────────────────────────────────────────────┐
│                         wpp::hwnd                               │
│                    (Base window handle)                         │
│                  - m_handle: HWND                               │
│                  - set_position()                               │
│                  - set_size()                                   │
│                  - move()                                       │
└──────────────────────────┬──────────────────────────────────────┘
                           │
         ┌─────────────────┴─────────────────┐
         │                                   │
┌────────▼────────┐                 ┌────────▼────────┐
│  wpp::dialog    │                 │   wpp::window   │
│                 │                 │                 │
│ - resource_id   │                 │ - window_class  │
│ - run_dlg()     │                 │ - create()      │
│                 │                 │ + set_layout()  │ ◄── New!
│                 │                 │ + get_layout()  │
└─────────────────┘                 └────────┬────────┘
                                             │
                                             │ has-a
                                             ▼
                                    ┌────────────────┐
                                    │ layout::panel  │ ◄── Abstract base
                                    │ (shared_ptr)   │
                                    └────────┬───────┘
                                             │
                  ┌──────────────────────────┼──────────────────────────┐
                  │                          │                          │
        ┌─────────▼─────────┐    ┌──────────▼──────────┐    ┌─────────▼─────────┐
        │  stack_panel      │    │   dock_panel        │    │     grid          │
        │                   │    │                     │    │                   │
        │ - orientation     │    │ - dock_positions[]  │    │ - columns[]       │
        │ - spacing         │    │ - last_child_fill   │    │ - rows[]          │
        │ + measure()       │    │ + measure()         │    │ - cell_map        │
        │ + arrange()       │    │ + arrange()         │    │ + measure()       │
        └───────────────────┘    └─────────────────────┘    │ + arrange()       │
                                                             └───────────────────┘
                 ┌────────────────┐
                 │  wrap_panel    │
                 │                │
                 │ - orientation  │
                 │ - item_width   │
                 │ - item_height  │
                 │ + measure()    │
                 │ + arrange()    │
                 └────────────────┘
```

## Layout Process Flow

```
┌──────────────────┐
│  Window Created  │
└────────┬─────────┘
         │
         ▼
┌──────────────────────────────┐
│  User calls set_layout()     │
│  with panel (e.g., stack)    │
└────────┬─────────────────────┘
         │
         ▼
┌──────────────────────────────┐
│  WM_SIZE message received    │
└────────┬─────────────────────┘
         │
         ▼
┌──────────────────────────────┐
│  MEASURE PASS                │
│                              │
│  Panel asks each child:      │
│  "How much space do you      │
│   need?"                     │
│                              │
│  panel->measure(w, h)        │
└────────┬─────────────────────┘
         │
         ▼
┌──────────────────────────────┐
│  ARRANGE PASS                │
│                              │
│  Panel tells each child:     │
│  "Here's your position       │
│   and size"                  │
│                              │
│  panel->arrange(x, y, w, h)  │
└────────┬─────────────────────┘
         │
         ▼
┌──────────────────────────────┐
│  Controls positioned via     │
│  SetWindowPos()              │
└──────────────────────────────┘
```

## StackPanel Layout Example

### Vertical Stack (Default)
```
┌─────────────────────────────────┐
│        Window (800x600)         │
│  ┌───────────────────────────┐  │
│  │  Padding: 10px all sides  │  │
│  │  ┌─────────────────────┐  │  │
│  │  │   Button 1 (150x30) │  │  │
│  │  └─────────────────────┘  │  │
│  │     Spacing: 5px          │  │
│  │  ┌─────────────────────┐  │  │
│  │  │   Button 2 (150x30) │  │  │
│  │  └─────────────────────┘  │  │
│  │     Spacing: 5px          │  │
│  │  ┌─────────────────────┐  │  │
│  │  │   Button 3 (150x30) │  │  │
│  │  └─────────────────────┘  │  │
│  └───────────────────────────┘  │
└─────────────────────────────────┘
```

### Horizontal Stack
```
┌──────────────────────────────────────────┐
│           Window (800x600)               │
│  ┌────────────────────────────────────┐  │
│  │      Padding: 10px all sides       │  │
│  │  ┌─────┐  ┌─────┐  ┌──────┐       │  │
│  │  │ New │  │Open │  │ Save │       │  │
│  │  │ 60x │  │60x  │  │ 60x  │       │  │
│  │  │ 25  │  │25   │  │ 25   │       │  │
│  │  └─────┘  └─────┘  └──────┘       │  │
│  │     └─5px─┘  └─5px─┘              │  │
│  │           Spacing                  │  │
│  └────────────────────────────────────┘  │
└──────────────────────────────────────────┘
```

## DockPanel Layout Example

```
┌────────────────────────────────────────────────┐
│              Window (800x600)                  │
│  ┌──────────────────────────────────────────┐  │
│  │         Toolbar (dock: top)              │  │
│  │              Height: 30px                │  │
│  └──────────────────────────────────────────┘  │
│  ┌──────┐  ┌────────────────────────────┐     │
│  │      │  │                            │     │
│  │Side  │  │      Main Content          │     │
│  │bar   │  │      (dock: fill)          │     │
│  │      │  │                            │     │
│  │dock: │  │  Takes remaining space     │     │
│  │left  │  │  after other panels        │     │
│  │      │  │                            │     │
│  │Width │  │                            │     │
│  │150px │  │                            │     │
│  │      │  │                            │     │
│  └──────┘  └────────────────────────────┘     │
│  ┌──────────────────────────────────────────┐  │
│  │       Status Bar (dock: bottom)          │  │
│  │              Height: 25px                │  │
│  └──────────────────────────────────────────┘  │
└────────────────────────────────────────────────┘
```

## Grid Layout Example

```
┌────────────────────────────────────────────────┐
│              Window (800x600)                  │
│  ┌──────────────────────────────────────────┐  │
│  │         Grid (2 rows x 3 columns)        │  │
│  │                                          │  │
│  │  Col 0      Col 1 (auto)    Col 2 (*)   │  │
│  │  100px                      fills space  │  │
│  │  ┌─────┐   ┌───────┐   ┌──────────────┐ │  │
│  │  │Label│   │       │   │              │ │  │ Row 0
│  │  │     │   │ Text  │   │   Textbox    │ │  │ (auto)
│  │  └─────┘   └───────┘   └──────────────┘ │  │
│  │  ┌────────────────────────────────────┐ │  │
│  │  │                                    │ │  │
│  │  │         ListView                   │ │  │ Row 1
│  │  │         (spans 3 columns)          │ │  │ (*)
│  │  │                                    │ │  │
│  │  └────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────┘  │
└────────────────────────────────────────────────┘
```

## Nested Layout Example

```
┌────────────────────────────────────────────────┐
│              Window (800x600)                  │
│  ┌──────────────────────────────────────────┐  │
│  │        Main DockPanel                    │  │
│  │  ┌────────────────────────────────────┐  │  │
│  │  │  Toolbar (StackPanel horizontal)   │  │  │ dock: top
│  │  │  [New] [Open] [Save] [Copy]        │  │  │
│  │  └────────────────────────────────────┘  │  │
│  │  ┌──────┐  ┌──────────────────────────┐ │  │
│  │  │      │  │                          │ │  │
│  │  │Stack │  │      Main Content        │ │  │
│  │  │Panel │  │      (dock: fill)        │ │  │
│  │  │Vert. │  │                          │ │  │
│  │  │      │  │                          │ │  │
│  │  │Tree  │  │                          │ │  │ dock: left
│  │  │─────│  │                          │ │  │    & fill
│  │  │Props │  │                          │ │  │
│  │  │      │  │                          │ │  │
│  │  └──────┘  └──────────────────────────┘ │  │
│  └──────────────────────────────────────────┘  │
└────────────────────────────────────────────────┘

Hierarchy:
  DockPanel (main)
  ├── StackPanel (toolbar, horizontal) → dock: top
  │   ├── Button (New)
  │   ├── Button (Open)
  │   ├── Button (Save)
  │   └── Button (Copy)
  ├── StackPanel (sidebar, vertical) → dock: left
  │   ├── TreeView (file tree)
  │   └── ListView (properties)
  └── EditText (content) → dock: fill
```

## Memory Model

```
┌────────────────────────────┐
│      window instance       │
│                            │
│  std::shared_ptr<panel>    │
│  m_layout_panel ───────────┼──┐
│                            │  │
│  std::vector<control_ptr>  │  │
│  m_controls ───────────────┼─┐│
│                            │ ││
└────────────────────────────┘ ││
                               ││
    ┌──────────────────────────┘│
    │                           │
    ▼                           │
┌────────────────────┐          │
│   stack_panel      │          │
│                    │          │
│  std::vector<      │          │
│   shared_ptr<hwnd>>│          │
│  m_children ───────┼──────────┤
│                    │          │
│  - m_orientation   │          │
│  - m_spacing       │          │
└────────────────────┘          │
                                │
    ┌───────────────────────────┘
    │
    ▼
┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
│  button ctrl 1  │  │  button ctrl 2  │  │  button ctrl 3  │
│  - m_handle     │  │  - m_handle     │  │  - m_handle     │
│  - m_parent     │  │  - m_parent     │  │  - m_parent     │
└─────────────────┘  └─────────────────┘  └─────────────────┘
```

## Sizing Terminology

### Star Sizing (Grid)
```
Grid with 3 columns: [1*] [2*] [1*]

Available width: 400px

Column 0: (1/(1+2+1)) * 400 = 100px
Column 1: (2/(1+2+1)) * 400 = 200px
Column 2: (1/(1+2+1)) * 400 = 100px

┌────────┬──────────────────┬────────┐
│  100px │      200px       │  100px │
│   1*   │       2*         │   1*   │
└────────┴──────────────────┴────────┘
```

### Auto Sizing
```
Auto-sized elements measure their content and request
the minimum space needed.

Example: Label with text "Name:" measures at ~40px
         TextBox measures at preferred 150px

Grid auto-sizes to fit content
```

### Absolute Sizing
```
Fixed pixel dimensions regardless of content or parent size

Column width: 100px (always)
Row height: 50px (always)

Not responsive but useful for fixed elements like icons
```

## Algorithm: StackPanel Measure/Arrange

### Measure Pass (Vertical)
```cpp
void stack_panel::measure(int available_width, int available_height) {
    int total_height = m_padding.top;
    int max_width = 0;
    
    for (auto& child : m_children) {
        // Each child gets full width
        child->measure(available_width - m_padding.left - m_padding.right,
                      available_height);
        
        int child_height = child->get_desired_height();
        int child_width = child->get_desired_width();
        
        total_height += child_height;
        if (child != m_children.back())
            total_height += m_spacing;  // Add spacing between
        
        max_width = max(max_width, child_width);
    }
    
    total_height += m_padding.bottom;
    
    m_desired_size = {max_width + m_padding.left + m_padding.right,
                      total_height};
}
```

### Arrange Pass (Vertical)
```cpp
void stack_panel::arrange(int x, int y, int width, int height) {
    int current_y = y + m_padding.top;
    int content_width = width - m_padding.left - m_padding.right;
    
    for (auto& child : m_children) {
        int child_height = child->get_desired_height();
        
        // Position child
        child->arrange(x + m_padding.left,
                      current_y,
                      content_width,
                      child_height);
        
        // Move to next position
        current_y += child_height + m_spacing;
    }
}
```

## Comparison with Other Layout Systems

```
┌──────────────┬─────────────┬──────────────┬─────────────┐
│   System     │   Approach  │ Flexibility  │ Complexity  │
├──────────────┼─────────────┼──────────────┼─────────────┤
│ Win32 Manual │  Absolute   │     Low      │    Low      │
│  (current)   │  positioning│              │             │
├──────────────┼─────────────┼──────────────┼─────────────┤
│ WPF/XAML     │   Panel     │    High      │   Medium    │
│  Panels      │   based     │              │             │
│ (proposed)   │             │              │             │
├──────────────┼─────────────┼──────────────┼─────────────┤
│ WinForms     │  Anchor/    │   Medium     │    Low      │
│  Anchor/Dock │   Dock      │              │             │
├──────────────┼─────────────┼──────────────┼─────────────┤
│ Qt Layouts   │ Constraint  │    High      │    High     │
│              │   based     │              │             │
├──────────────┼─────────────┼──────────────┼─────────────┤
│ CSS Flexbox  │   Flex      │    High      │   Medium    │
│              │   model     │              │             │
└──────────────┴─────────────┴──────────────┴─────────────┘
```

---

**Note**: All diagrams are illustrative. Actual implementation details may vary.
For complete technical specifications, see [LayoutManager_DesignPlan.md](LayoutManager_DesignPlan.md).
