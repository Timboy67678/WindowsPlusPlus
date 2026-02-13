#include "..\layout\stack_panel.hpp"

namespace wpp::layout
{
    stack_panel::stack_panel(orientation orient)
        : panel(panel_type::stack)
        , m_orientation(orient)
        , m_spacing(0)
        , m_alignment(alignment::start)
    {
    }

    void stack_panel::add(control_ptr<> control)
    {
        if (control) {
            m_children.push_back(control);
            m_child_sizes.push_back({ 0, 0 });
        }
    }

    void stack_panel::measure(int available_width, int available_height)
    {
        // Apply DPI scaling
        int scaled_spacing = static_cast<int>(m_spacing * m_dpi_scale);
        int padding_h = static_cast<int>((m_padding.left + m_padding.right) * m_dpi_scale);
        int padding_v = static_cast<int>((m_padding.top + m_padding.bottom) * m_dpi_scale);
        int margin_h = static_cast<int>((m_margin.left + m_margin.right) * m_dpi_scale);
        int margin_v = static_cast<int>((m_margin.top + m_margin.bottom) * m_dpi_scale);

        // Adjust available space for padding and margin
        int content_width = available_width - padding_h - margin_h;
        int content_height = available_height - padding_v - margin_v;

        int total_width = 0;
        int total_height = 0;
        int max_cross_axis = 0;

        m_child_sizes.resize(m_children.size());

        for (size_t i = 0; i < m_children.size(); ++i) {
            auto& child = m_children[i];
            if (!child || !child->is_valid()) continue;

            // Get current window size as desired size
            RECT rc = child->get_rect();
            int child_width = rc.right - rc.left;
            int child_height = rc.bottom - rc.top;

            m_child_sizes[i] = { child_width, child_height };

            if (m_orientation == orientation::horizontal) {
                // Stack horizontally
                total_width += child_width;
                if (i > 0) total_width += scaled_spacing;
                max_cross_axis = (std::max)(max_cross_axis, child_height);
            }
            else {
                // Stack vertically
                total_height += child_height;
                if (i > 0) total_height += scaled_spacing;
                max_cross_axis = (std::max)(max_cross_axis, child_width);
            }
        }

        // Calculate desired size (including margin and padding)
        if (m_orientation == orientation::horizontal) {
            m_desired_size.width = total_width + padding_h + margin_h;
            m_desired_size.height = max_cross_axis + padding_v + margin_v;
        } else {
            m_desired_size.width = max_cross_axis + padding_h + margin_h;
            m_desired_size.height = total_height + padding_v + margin_v;
        }
    }

    void stack_panel::arrange(int x, int y, int width, int height)
    {
        m_actual_size = { width, height };

        // Apply DPI scaling
        int scaled_spacing = static_cast<int>(m_spacing * m_dpi_scale);
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

        int current_pos = 0;

        for (size_t i = 0; i < m_children.size(); ++i) {
            auto& child = m_children[i];
            if (!child || !child->is_valid()) continue;

            auto& child_size = m_child_sizes[i];
            int child_x = 0, child_y = 0, child_width = 0, child_height = 0;

            if (m_orientation == orientation::horizontal) {
                // Horizontal stacking
                child_x = content_x + current_pos;
                child_width = child_size.width;

                // Handle vertical alignment
                switch (m_alignment) {
                case alignment::start:
                    child_y = content_y;
                    child_height = child_size.height;
                    break;
                case alignment::center:
                    child_y = content_y + (content_height - child_size.height) / 2;
                    child_height = child_size.height;
                    break;
                case alignment::end:
                    child_y = content_y + content_height - child_size.height;
                    child_height = child_size.height;
                    break;
                case alignment::stretch:
                    child_y = content_y;
                    child_height = content_height;
                    break;
                }

                current_pos += child_width + scaled_spacing;
            }
            else {
                // Vertical stacking
                child_y = content_y + current_pos;
                child_height = child_size.height;

                // Handle horizontal alignment
                switch (m_alignment) {
                case alignment::start:
                    child_x = content_x;
                    child_width = child_size.width;
                    break;
                case alignment::center:
                    child_x = content_x + (content_width - child_size.width) / 2;
                    child_width = child_size.width;
                    break;
                case alignment::end:
                    child_x = content_x + content_width - child_size.width;
                    child_width = child_size.width;
                    break;
                case alignment::stretch:
                    child_x = content_x;
                    child_width = content_width;
                    break;
                }

                current_pos += child_height + scaled_spacing;
            }

            // Position the control
            child->move(child_x, child_y, child_width, child_height);
        }
    }
}
