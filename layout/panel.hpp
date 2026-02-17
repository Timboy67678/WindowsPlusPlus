#ifndef WPP_LAYOUT_PANEL_HPP
#define WPP_LAYOUT_PANEL_HPP

#include "../winplusplus.hpp"
#include "../controls/control.hpp"

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

	// Base class for all layout panels - inherits from control to allow nesting
	class panel : public control {
	public:
		panel(type panel_type, HWND parent = nullptr) 
			: control(create_panel_window(parent)),
			m_panel_type(panel_type) {
			// If no parent provided yet (created before window), m_handle will be nullptr
			// It will be set later when the panel is attached to a window
		}
        virtual ~panel() = default;

		// Add a control to this panel
		virtual void add(control_ptr<> control) = 0;
		virtual void add_window_controls(window_base* window) = 0;

		// Add a panel to this panel (for nesting)
		void add_panel(std::shared_ptr<panel> child_panel) {
			if (child_panel) {
				// Attach the panel's window as a child
				attach_child(child_panel->get_handle());
				// Store in children collection as a control_ptr
				add(std::static_pointer_cast<control>(child_panel));
			}
		}

		// Layout calculations
		virtual void measure(int available_width, int available_height) = 0;
		virtual void arrange(int x, int y, int width, int height) = 0;

		// Optional: custom painting (for advanced panels)
		virtual void paint(HDC hdc) = 0;

		// Check if a child control is actually a panel
		static bool is_panel(const control_ptr<>& child) {
			return std::dynamic_pointer_cast<panel>(child) != nullptr;
		}

		// Get a child as a panel (returns nullptr if not a panel)
		static std::shared_ptr<panel> as_panel(const control_ptr<>& child) {
			return std::dynamic_pointer_cast<panel>(child);
		}

		// Initialize the panel window if it wasn't created with a parent
		void initialize_window(HWND parent) {
			if (!m_handle && parent) {
				m_handle = create_panel_window(parent);
				m_parent_handle = parent;
			}
		}

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

		// Get the desired size (calculated during measure phase)
		sizing_t get_desired_size() const { return m_desired_size; }

		// Get the actual size (set during arrange phase)
		sizing_t get_actual_size() const { return m_actual_size; }

	protected:
		// Helper function to create a panel container window
		inline HWND create_panel_window(HWND parent) {
			if (!parent) return nullptr;
			return ::CreateWindowEx(
				0,
				TEXT("STATIC"),
				TEXT(""),
				WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
				0, 0, 0, 0,
				parent,
				nullptr,
				::GetModuleHandle(nullptr),
				nullptr
			);
		}

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
