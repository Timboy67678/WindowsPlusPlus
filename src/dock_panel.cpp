#include "..\layout\dock_panel.hpp"

namespace wpp::layout
{
    dock_panel::dock_panel(HWND parent)
        : panel(type::dock, parent)
    {
    }

    void dock_panel::add(control_ptr<> control) {
        // Default to fill position
        add(control, dock_position::fill);
    }

    void dock_panel::add(control_ptr<> control, dock_position position) {
        if (control && std::find(m_children.begin(), m_children.end(), control) == m_children.end()) {
            m_children.push_back(control);
            m_dock_positions[control] = position;

            child_measure measure;
            measure.control = control;
            measure.position = position;

            // For nested panels, we'll measure them later
            // For regular controls, use their current size
            if (is_panel(control)) {
                measure.desired_size = { 0, 0 };
            } else {
                RECT rc = control->get_rect();
                int original_width = rc.right - rc.left;
                int original_height = rc.bottom - rc.top;
                measure.desired_size = { original_width, original_height };
            }

            m_child_measures.push_back(measure);
        }
    }

    void dock_panel::set_dock_position(control_ptr<> control, dock_position position) {
        if (control) {
            m_dock_positions[control] = position;
            
            // Update the position in child_measures too
            for (auto& measure : m_child_measures) {
                if (measure.control == control) {
                    measure.position = position;
                    break;
                }
            }
        }
    }

    dock_position dock_panel::get_dock_position(control_ptr<> control) const {
        auto it = m_dock_positions.find(control);
        if (it != m_dock_positions.end()) {
            return it->second;
        }
        return dock_position::fill;
    }

    void dock_panel::measure(int available_width, int available_height) {
        // Apply DPI scaling
        int padding_h = static_cast<int>((m_padding.left + m_padding.right) * m_dpi_scale);
        int padding_v = static_cast<int>((m_padding.top + m_padding.bottom) * m_dpi_scale);
        int margin_h = static_cast<int>((m_margin.left + m_margin.right) * m_dpi_scale);
        int margin_v = static_cast<int>((m_margin.top + m_margin.bottom) * m_dpi_scale);

        // Adjust available space for padding and margin
        int content_width = available_width - padding_h - margin_h;
        int content_height = available_height - padding_v - margin_v;

        int remaining_width = content_width;
        int remaining_height = content_height;

        // Update positions if last child fill is enabled
        for (size_t i = 0; i < m_child_measures.size(); ++i) {
            auto& measure = m_child_measures[i];
            if (!measure.control || !measure.control->is_valid()) continue;

            // If last child and m_last_child_fill is true, override to fill
            if (m_last_child_fill && i == m_child_measures.size() - 1) {
                measure.position = dock_position::fill;
            } else {
                // Use stored position from m_dock_positions
                measure.position = get_dock_position(measure.control);
            }

            int child_width = 0;
            int child_height = 0;

            // If this child is a nested panel, recursively measure it
            if (auto child_panel = as_panel(measure.control)) {
                child_panel->measure(remaining_width, remaining_height);
                auto desired = child_panel->get_desired_size();
                child_width = desired.width;
                child_height = desired.height;
                measure.desired_size = { child_width, child_height };
            } else {
                // Use stored original sizes
                child_width = measure.desired_size.width;
                child_height = measure.desired_size.height;
            }

            // Reduce remaining space based on dock position
            switch (measure.position) {
            case dock_position::left:
            case dock_position::right:
                remaining_width -= child_width;
                break;
            case dock_position::top:
            case dock_position::bottom:
                remaining_height -= child_height;
                break;
            case dock_position::fill:
                // Fill takes remaining space
                break;
            }
        }

        // Desired size is the full available space (including margin and padding)
        m_desired_size.width = content_width + padding_h + margin_h;
        m_desired_size.height = content_height + padding_v + margin_v;
    }

    void dock_panel::arrange(int x, int y, int width, int height) {
        m_actual_size = { width, height };

        // Apply DPI scaling
        int margin_left = static_cast<int>(m_margin.left * m_dpi_scale);
        int margin_top = static_cast<int>(m_margin.top * m_dpi_scale);
        int margin_right = static_cast<int>(m_margin.right * m_dpi_scale);
        int margin_bottom = static_cast<int>(m_margin.bottom * m_dpi_scale);
        int padding_left = static_cast<int>(m_padding.left * m_dpi_scale);
        int padding_top = static_cast<int>(m_padding.top * m_dpi_scale);
        int padding_right = static_cast<int>(m_padding.right * m_dpi_scale);
        int padding_bottom = static_cast<int>(m_padding.bottom * m_dpi_scale);

        // Apply margin offset to starting position
        int margin_x = x + margin_left;
        int margin_y = y + margin_top;

        // Content area after margin and padding
        int content_x = margin_x + padding_left;
        int content_y = margin_y + padding_top;
        int content_width = width - margin_left - margin_right - padding_left - padding_right;
        int content_height = height - margin_top - margin_bottom - padding_top - padding_bottom;

        // Track remaining space as we dock controls
        int remaining_x = content_x;
        int remaining_y = content_y;
        int remaining_width = content_width;
        int remaining_height = content_height;

        for (auto& measure : m_child_measures) {
            if (!measure.control || !measure.control->is_valid()) continue;

            int child_x = 0, child_y = 0, child_width = 0, child_height = 0;

            switch (measure.position) {
            case dock_position::left:
                child_x = remaining_x;
                child_y = remaining_y;
                child_width = measure.desired_size.width;
                child_height = remaining_height;

                // Update remaining space
                remaining_x += child_width;
                remaining_width -= child_width;
                break;

            case dock_position::top:
                child_x = remaining_x;
                child_y = remaining_y;
                child_width = remaining_width;
                child_height = measure.desired_size.height;

                // Update remaining space
                remaining_y += child_height;
                remaining_height -= child_height;
                break;

            case dock_position::right:
                child_width = measure.desired_size.width;
                child_height = remaining_height;
                child_x = remaining_x + remaining_width - child_width;
                child_y = remaining_y;

                // Update remaining space
                remaining_width -= child_width;
                break;

            case dock_position::bottom:
                child_width = remaining_width;
                child_height = measure.desired_size.height;
                child_x = remaining_x;
                child_y = remaining_y + remaining_height - child_height;

                // Update remaining space
                remaining_height -= child_height;
                break;

            case dock_position::fill:
                child_x = remaining_x;
                child_y = remaining_y;
                child_width = remaining_width;
                child_height = remaining_height;
                break;
            }

            // Ensure non-negative dimensions
            child_width = (std::max)(0, child_width);
            child_height = (std::max)(0, child_height);

            // Position the control
            measure.control->move(child_x, child_y, child_width, child_height);

            // If this is a nested panel, arrange its children
            if (auto child_panel = as_panel(measure.control)) {
                child_panel->arrange(child_x, child_y, child_width, child_height);
            }
        }
    }

    void dock_panel::paint(HDC hdc) {
        // For now, we don't do any custom painting for the dock panel itself.
		// Child controls will handle their own painting.
    }
}
