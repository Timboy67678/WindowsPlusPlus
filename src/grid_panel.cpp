#include "..\layout\grid_panel.hpp"
#include <algorithm>
#include <numeric>

namespace wpp::layout
{
    grid_panel::grid_panel(HWND parent)
        : panel(type::grid, parent) {
    }

    void grid_panel::add(control_ptr<> control) {
        if (control && std::find(m_children.begin(), m_children.end(), control) == m_children.end()) {
            m_children.push_back(control);
            
            // Assign to next available position
            auto pos = get_next_available_position();
            m_grid_positions[control] = pos;
            
            // Ensure grid capacity
            ensure_grid_capacity(pos.row + pos.row_span - 1, pos.column + pos.column_span - 1);
        }
    }

    void grid_panel::add(control_ptr<> control, int row, int column, int row_span, int column_span) {
        if (control && std::find(m_children.begin(), m_children.end(), control) == m_children.end()) {
            m_children.push_back(control);
            grid_position pos{ row, column, row_span, column_span };
            m_grid_positions[control] = pos;
            ensure_grid_capacity(row + row_span - 1, column + column_span - 1);
        }
    }

    void grid_panel::add(control_ptr<> control, const grid_position& position) {
        add(control, position.row, position.column, position.row_span, position.column_span);
    }

    void grid_panel::add_row_definition(const grid_length& height) {
        m_row_definitions.push_back(height);
    }

    void grid_panel::add_column_definition(const grid_length& width) {
        m_column_definitions.push_back(width);
    }

    void grid_panel::set_row_definitions(const std::vector<grid_length>& rows) {
        m_row_definitions = rows;
    }

    void grid_panel::set_column_definitions(const std::vector<grid_length>& columns) {
        m_column_definitions = columns;
    }

    void grid_panel::set_grid_position(control_ptr<> control, int row, int column, int row_span, int column_span) {
        if (control) {
            grid_position pos{ row, column, row_span, column_span };
            m_grid_positions[control] = pos;
            ensure_grid_capacity(row + row_span - 1, column + column_span - 1);
        }
    }

    void grid_panel::set_grid_position(control_ptr<> control, const grid_position& position) {
        set_grid_position(control, position.row, position.column, position.row_span, position.column_span);
    }

    grid_position grid_panel::get_grid_position(control_ptr<> control) const {
        auto it = m_grid_positions.find(control);
        if (it != m_grid_positions.end()) {
            return it->second;
        }
        return { 0, 0, 1, 1 };
    }

    void grid_panel::set_alignment(control_ptr<> control, alignment horizontal, alignment vertical) {
        if (control) {
            m_alignments[control] = { horizontal, vertical };
        }
    }

    void grid_panel::set_alignment(control_ptr<> control, const grid_alignment& align) {
        if (control) {
            m_alignments[control] = align;
        }
    }

    grid_alignment grid_panel::get_alignment(control_ptr<> control) const {
        auto it = m_alignments.find(control);
        if (it != m_alignments.end()) {
            return it->second;
        }
        return { alignment::stretch, alignment::stretch };
    }

    void grid_panel::measure(int available_width, int available_height) {
        // Apply DPI scaling
        int padding_h = static_cast<int>((m_padding.left + m_padding.right) * m_dpi_scale);
        int padding_v = static_cast<int>((m_padding.top + m_padding.bottom) * m_dpi_scale);
        int margin_h = static_cast<int>((m_margin.left + m_margin.right) * m_dpi_scale);
        int margin_v = static_cast<int>((m_margin.top + m_margin.bottom) * m_dpi_scale);
        int scaled_row_spacing = static_cast<int>(m_row_spacing * m_dpi_scale);
        int scaled_column_spacing = static_cast<int>(m_column_spacing * m_dpi_scale);

        int content_width = available_width - padding_h - margin_h;
        int content_height = available_height - padding_v - margin_v;

        // Ensure we have at least one row and one column
        int row_count = (std::max)(1, static_cast<int>(m_row_definitions.size()));
        int column_count = (std::max)(1, static_cast<int>(m_column_definitions.size()));

        // Calculate spacing overhead
        int total_row_spacing = (row_count > 1) ? scaled_row_spacing * (row_count - 1) : 0;
        int total_column_spacing = (column_count > 1) ? scaled_column_spacing * (column_count - 1) : 0;

        content_width -= total_column_spacing;
        content_height -= total_row_spacing;

        // Initialize size arrays
        m_row_heights.assign(row_count, 0);
        m_column_widths.assign(column_count, 0);

        // First pass: measure auto-sized cells
        std::vector<int> auto_row_heights(row_count, 0);
        std::vector<int> auto_column_widths(column_count, 0);

        for (auto& child : m_children) {
            if (!child || !child->is_valid()) continue;

            auto pos = get_grid_position(child);
            
            // Clamp position to valid grid range
            pos.row = (std::min)(pos.row, row_count - 1);
            pos.column = (std::min)(pos.column, column_count - 1);
            pos.row_span = (std::min)(pos.row_span, row_count - pos.row);
            pos.column_span = (std::min)(pos.column_span, column_count - pos.column);

            int child_width = 0;
            int child_height = 0;

            // Measure child
            if (auto child_panel = as_panel(child)) {
                child_panel->measure(content_width, content_height);
                auto desired = child_panel->get_desired_size();
                child_width = desired.width;
                child_height = desired.height;
            } else {
                RECT rc = child->get_rect();
                child_width = rc.right - rc.left;
                child_height = rc.bottom - rc.top;
            }

            // For single-cell items, update auto sizes
            if (pos.row_span == 1 && pos.column_span == 1) {
                auto_row_heights[pos.row] = (std::max)(auto_row_heights[pos.row], child_height);
                auto_column_widths[pos.column] = (std::max)(auto_column_widths[pos.column], child_width);
            }
        }

        // Calculate final track sizes
        calculate_track_sizes(m_row_definitions, content_height, scaled_row_spacing, auto_row_heights, m_row_heights);
        calculate_track_sizes(m_column_definitions, content_width, scaled_column_spacing, auto_column_widths, m_column_widths);

        // Calculate desired size
        int total_width = std::accumulate(m_column_widths.begin(), m_column_widths.end(), 0) + total_column_spacing;
        int total_height = std::accumulate(m_row_heights.begin(), m_row_heights.end(), 0) + total_row_spacing;

        m_desired_size.width = total_width + padding_h + margin_h;
        m_desired_size.height = total_height + padding_v + margin_v;
    }

    void grid_panel::arrange(int x, int y, int width, int height) {
        m_actual_size = { width, height };

        int margin_left = static_cast<int>(m_margin.left * m_dpi_scale);
        int margin_top = static_cast<int>(m_margin.top * m_dpi_scale);
        int margin_right = static_cast<int>(m_margin.right * m_dpi_scale);
        int margin_bottom = static_cast<int>(m_margin.bottom * m_dpi_scale);
        int padding_left = static_cast<int>(m_padding.left * m_dpi_scale);
        int padding_top = static_cast<int>(m_padding.top * m_dpi_scale);
        int padding_right = static_cast<int>(m_padding.right * m_dpi_scale);
        int padding_bottom = static_cast<int>(m_padding.bottom * m_dpi_scale);
        int scaled_row_spacing = static_cast<int>(m_row_spacing * m_dpi_scale);
        int scaled_column_spacing = static_cast<int>(m_column_spacing * m_dpi_scale);

        int content_x = x + margin_left + padding_left;
        int content_y = y + margin_top + padding_top;

        // Calculate cumulative positions for rows and columns
        std::vector<int> row_positions;
        std::vector<int> column_positions;
        
        int current_y = content_y;
        for (size_t i = 0; i < m_row_heights.size(); ++i) {
            row_positions.push_back(current_y);
            current_y += m_row_heights[i] + scaled_row_spacing;
        }

        int current_x = content_x;
        for (size_t i = 0; i < m_column_widths.size(); ++i) {
            column_positions.push_back(current_x);
            current_x += m_column_widths[i] + scaled_column_spacing;
        }

        // Count valid non-panel children for DeferWindowPos (panels handle their own windows)
        int valid_control_count = 0;
        for (auto& child : m_children) {
            if (child && child->is_valid() && child->get_handle() && !as_panel(child)) {
                valid_control_count++;
            }
        }

        deferred_window_pos dwp(valid_control_count);

        // Arrange children
        for (auto& child : m_children) {
            if (!child || !child->is_valid()) continue;

            auto pos = get_grid_position(child);

            // Clamp position to valid grid range
            int row_count = static_cast<int>(m_row_heights.size());
            int column_count = static_cast<int>(m_column_widths.size());
            pos.row = (std::min)(pos.row, row_count - 1);
            pos.column = (std::min)(pos.column, column_count - 1);
            pos.row_span = (std::min)(pos.row_span, row_count - pos.row);
            pos.column_span = (std::min)(pos.column_span, column_count - pos.column);

            // Calculate cell bounds
            int cell_x = column_positions[pos.column];
            int cell_y = row_positions[pos.row];

            int cell_width = 0;
            for (int i = 0; i < pos.column_span; ++i) {
                cell_width += m_column_widths[pos.column + i];
                if (i > 0) cell_width += scaled_column_spacing;
            }

            int cell_height = 0;
            for (int i = 0; i < pos.row_span; ++i) {
                cell_height += m_row_heights[pos.row + i];
                if (i > 0) cell_height += scaled_row_spacing;
            }

            // Arrange child - panels are arranged, controls are moved
            if (auto child_panel = as_panel(child)) {
                child_panel->arrange(cell_x, cell_y, cell_width, cell_height);
            } else {
                // Get alignment for this control
                auto align = get_alignment(child);

                // Get control's desired size
                RECT child_rect = child->get_rect();
                int desired_width = child_rect.right - child_rect.left;
                int desired_height = child_rect.bottom - child_rect.top;

                int final_x = cell_x;
                int final_y = cell_y;
                int final_width = cell_width;
                int final_height = cell_height;

                // Apply horizontal alignment
                switch (align.horizontal) {
                case alignment::start:
                    final_width = (std::min)(desired_width, cell_width);
                    break;
                case alignment::center:
                    final_width = (std::min)(desired_width, cell_width);
                    final_x = cell_x + (cell_width - final_width) / 2;
                    break;
                case alignment::end:
                    final_width = (std::min)(desired_width, cell_width);
                    final_x = cell_x + cell_width - final_width;
                    break;
                case alignment::stretch:
                    // Use full cell width
                    break;
                }

                // Apply vertical alignment
                switch (align.vertical) {
                case alignment::start:
                    final_height = (std::min)(desired_height, cell_height);
                    break;
                case alignment::center:
                    final_height = (std::min)(desired_height, cell_height);
                    final_y = cell_y + (cell_height - final_height) / 2;
                    break;
                case alignment::end:
                    final_height = (std::min)(desired_height, cell_height);
                    final_y = cell_y + cell_height - final_height;
                    break;
                case alignment::stretch:
                    // Use full cell height
                    break;
                }

                // Position the control using deferred positioning
                if (child->get_handle()) {
                    if (dwp.is_valid()) {
                        dwp.defer(child->get_handle(), final_x, final_y, final_width, final_height);
                    } else {
                        // Fallback to direct move if deferred positioning isn't available
                        child->move(final_x, final_y, final_width, final_height);
                    }
                }
            }
        }

        // Update panel window position
        if (m_handle) {
			set_position(x, y, width, height);
            invalidate();
        }
    }

    void grid_panel::paint(HDC hdc) {
        // Optional: draw grid lines for debugging
        // Base implementation does nothing
    }

    void grid_panel::ensure_grid_capacity(int row, int column) {
        // Ensure we have enough row definitions
        while (static_cast<int>(m_row_definitions.size()) <= row) {
            m_row_definitions.push_back(grid_length::auto_size());
        }

        // Ensure we have enough column definitions
        while (static_cast<int>(m_column_definitions.size()) <= column) {
            m_column_definitions.push_back(grid_length::auto_size());
        }
    }

    grid_position grid_panel::get_next_available_position() const {
        int max_row = 0;
        int max_column = 0;

        // Find the maximum row and column used
        for (const auto& [control, pos] : m_grid_positions) {
            max_row = (std::max)(max_row, pos.row + pos.row_span - 1);
            max_column = (std::max)(max_column, pos.column + pos.column_span - 1);
        }

        // Place in next column, or wrap to next row
        int column_count = (std::max)(1, static_cast<int>(m_column_definitions.size()));
        if (max_column + 1 < column_count) {
            return { max_row, max_column + 1, 1, 1 };
        } else {
            return { max_row + 1, 0, 1, 1 };
        }
    }

    void grid_panel::calculate_track_sizes(
        const std::vector<grid_length>& definitions,
        int available_size,
        int spacing,
        const std::vector<int>& auto_sizes,
        std::vector<int>& out_sizes
    ) {
        int track_count = static_cast<int>(out_sizes.size());
        if (track_count == 0) return;

        // Initialize with auto sizes or 0
        for (int i = 0; i < track_count; ++i) {
            if (i < static_cast<int>(definitions.size())) {
                auto& def = definitions[i];
                if (def.type == grid_length_type::auto_size) {
                    out_sizes[i] = (i < static_cast<int>(auto_sizes.size())) ? auto_sizes[i] : 0;
                } else if (def.type == grid_length_type::pixel) {
                    out_sizes[i] = static_cast<int>(def.value);
                } else {
                    out_sizes[i] = 0; // Will be calculated later for star sizing
                }
            } else {
                out_sizes[i] = (i < static_cast<int>(auto_sizes.size())) ? auto_sizes[i] : 0;
            }
        }

        // Calculate remaining space for star-sized tracks
        int used_space = 0;
        double total_star_value = 0.0;

        for (int i = 0; i < track_count; ++i) {
            if (i < static_cast<int>(definitions.size())) {
                auto& def = definitions[i];
                if (def.type == grid_length_type::star) {
                    total_star_value += def.value;
                } else {
                    used_space += out_sizes[i];
                }
            } else {
                used_space += out_sizes[i];
            }
        }

        int remaining_space = (std::max)(0, available_size - used_space);

        // Distribute remaining space to star-sized tracks
        if (total_star_value > 0 && remaining_space > 0) {
            for (int i = 0; i < track_count; ++i) {
                if (i < static_cast<int>(definitions.size())) {
                    auto& def = definitions[i];
                    if (def.type == grid_length_type::star) {
                        out_sizes[i] = static_cast<int>((remaining_space * def.value) / total_star_value);
                    }
                }
            }
        }
    }
}
