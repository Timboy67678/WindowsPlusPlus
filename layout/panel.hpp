#ifndef WPP_LAYOUT_PANEL_HPP
#define WPP_LAYOUT_PANEL_HPP

#include "../winplusplus.hpp"

namespace wpp::layout
{
    enum class orientation {
        horizontal,
        vertical
    };

    enum class alignment {
        start,
        center,
        end,
        stretch
    };

    enum class dock_position {
        left,
        top,
        right,
        bottom,
        fill
    };

    enum class type {
        stack,
        dock,
        grid,
        absolute,
        base = -1
	};

    struct margin_t { int left, top, right, bottom; };
    struct padding_t { int left, top, right, bottom; };
	struct sizing_t { int width, height; };

    // Base class for all layout panels
    class panel {
    public:
		panel(type) 
            : m_panel_type(type::base) {}
        virtual ~panel() = default;

        // Add a control to this panel
        virtual void add(control_ptr<> control) = 0;

        // Layout calculations
        virtual void measure(int available_width, int available_height) = 0;
        virtual void arrange(int x, int y, int width, int height) = 0;

		// Optional: custom painting (for advanced panels)
        virtual void paint(HDC hdc) = 0;

        // Parent/child relationships
        void set_parent_window(hwnd parent) { m_parent = parent; }
        hwnd get_parent_window() const { return m_parent; }

        // Margin and padding
        void set_margin(int left, int top, int right, int bottom) {
			m_margin = { left, top, right, bottom };
        }
        void set_margin(int uniform) {
            set_margin(uniform, uniform, uniform, uniform);
        }
        void set_padding(int left, int top, int right, int bottom) {
			m_padding = { left, top, right, bottom };
        }
		void set_padding(int uniform) {
            set_padding(uniform, uniform, uniform, uniform);
		}
		void set_margin(const margin_t& margin) { m_margin = margin; }
		void set_padding(const padding_t& padding) { m_padding = padding; }

        // DPI support
        void set_dpi_scale(float scale) { m_dpi_scale = scale; }

		// Get the type of this panel
        type get_type() const { return m_panel_type; }

    protected:
        hwnd m_parent = nullptr;
        std::vector<control_ptr<>> m_children;

        // Layout properties
        margin_t m_margin{ 0,0,0,0 };
        padding_t m_padding{ 0,0,0,0 };
        float m_dpi_scale = 1.0f;

        // Calculated sizes
        sizing_t m_desired_size{ 0, 0 };
        sizing_t m_actual_size{ 0, 0 };

        type m_panel_type = type::base;
    };
}

#endif // WPP_LAYOUT_PANEL_HPP
