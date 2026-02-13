# Windows++ Documentation

This directory contains design documents, tutorials, and guides for the Windows++ library.

## Layout Manager Documentation

The layout manager system provides XAML-inspired automatic layout panels for Windows++ applications, eliminating the need for manual coordinate-based positioning.

### Documents

1. **[Layout Manager Design Plan](LayoutManager_DesignPlan.md)** - Comprehensive design document
   - Architecture and components
   - Implementation plan (5 weeks)
   - Technical considerations
   - Usage examples
   - Open questions for discussion

2. **[Layout Manager Quick Start Guide](LayoutManager_QuickStart.md)** - Quick reference
   - Before/after examples
   - Available panel types (Stack, Grid, Dock, Wrap)
   - Common patterns
   - Migration guide
   - Tips and troubleshooting

### Status

**Current Phase**: Design & Planning
- ✅ Design document completed
- ✅ Quick start guide created
- ⏳ Awaiting stakeholder review and feedback
- ⏳ Implementation pending approval

### Key Features (Planned)

- **XAML-Inspired Panels**: StackPanel, Grid, DockPanel, WrapPanel
- **Backward Compatible**: All existing APIs remain unchanged
- **DPI-Aware**: Automatic high-DPI support
- **Resizable**: Auto-adjusts on window resize
- **Composable**: Support for nested layouts

### Quick Example

```cpp
// Create a vertical stack panel
auto stack = std::make_shared<layout::stack_panel>(layout::orientation::vertical);
stack->set_padding(10, 10, 10, 10);
stack->set_spacing(5);

// Add controls
stack->add(create_button(_T("Button 1"), 0, 0, 150, 30));
stack->add(create_button(_T("Button 2"), 0, 0, 150, 30));
stack->add(create_button(_T("Button 3"), 0, 0, 150, 30));

// Set layout - panel handles positioning automatically!
set_layout(stack);
```

### Feedback Welcome

This is a design proposal. Please review the documents and provide feedback on:
- API design and naming
- Panel types and functionality
- Implementation priorities
- Use cases and examples
- Performance requirements

### Next Steps

1. Review design documents
2. Discuss open questions
3. Approve implementation plan
4. Begin Phase 1: Core Infrastructure

---

*For questions or feedback, please open an issue or start a discussion on GitHub.*
