#ifndef WPP_LAYOUT_STACK_PANEL_HPP
#define WPP_LAYOUT_STACK_PANEL_HPP

#include "panel.hpp"

namespace wpp::layout
{
    // Stack panel - arranges children in a linear sequence (horizontal or vertical)
    class stack_panel : public panel {
    public:
        explicit stack_panel(orientation orient = orientation::vertical);
        virtual ~stack_panel() = default;

        // Add a control to this panel
        void add(control_ptr<> control) override;

        // Layout calculations
        void measure(int available_width, int available_height) override;
        void arrange(int x, int y, int width, int height) override;

        // Configuration
        void set_orientation(orientation orient) { m_orientation = orient; }
        orientation get_orientation() const { return m_orientation; }

		// Spacing between children
        void set_spacing(int spacing) { m_spacing = spacing; }
        int get_spacing() const { return m_spacing; }

        void set_alignment(alignment align) { m_alignment = align; }
        alignment get_alignment() const { return m_alignment; }

    private:
        orientation m_orientation;
        int m_spacing;
        alignment m_alignment;

        // Per-child desired sizes
        std::vector<sizing_t> m_child_sizes;
    };
}

#endif // WPP_LAYOUT_STACK_PANEL_HPP
