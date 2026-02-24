#ifndef WPP_LAYOUT_DOCK_PANEL_HPP
#define WPP_LAYOUT_DOCK_PANEL_HPP

#include "panel.hpp"

namespace wpp::layout
{
    // Dock panel - arranges children along edges (left/top/right/bottom) with one fill element
    class dock_panel : public panel {
    public:
        explicit dock_panel(HWND parent = nullptr);
        virtual ~dock_panel() = default;

        // Add a control with dock position
        void add(control_ptr<> control) override;
        void add(control_ptr<> control, dock_position position);

		// Add all controls from a window (used when adding a window to the panel)
        void add_window_controls(window_base* window) override {
            if (window != nullptr) {
                for (const auto& control : window->get_controls())
                    add(control);
            }
        }

        // Layout calculations
        void measure(int available_width, int available_height) override;
        void arrange(int x, int y, int width, int height) override;
        void paint(HDC hdc) override;

        // Configuration
        void set_dock_position(control_ptr<> control, dock_position position);
        dock_position get_dock_position(control_ptr<> control) const;

        // Set last child fills remaining space (default: true)
        void set_last_child_fill(bool fill) { m_last_child_fill = fill; }
        bool get_last_child_fill() const { return m_last_child_fill; }

    private:
        std::unordered_map<control_ptr<>, dock_position> m_dock_positions;
        bool m_last_child_fill = true;

        struct child_measure {
            control_ptr<> control;
            dock_position position = dock_position::fill;
            sizing_t desired_size = {};
        };
        std::vector<child_measure> m_child_measures;
    };
}

#endif // WPP_LAYOUT_DOCK_PANEL_HPP
