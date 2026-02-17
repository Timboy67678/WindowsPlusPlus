#ifndef WPP_LAYOUT_GRID_PANEL_HPP
#define WPP_LAYOUT_GRID_PANEL_HPP

#include "panel.hpp"
#include <vector>
#include <unordered_map>

namespace wpp::layout
{
    // Grid size units
    enum class grid_length_type {
        auto_size,      // Size to content
        pixel,          // Fixed pixel size
        star            // Proportional size (e.g., 1*, 2*, 3*)
    };

    struct grid_length {
        grid_length_type type = grid_length_type::auto_size;
        double value = 1.0;  // For pixel or star types

        static grid_length auto_size() { return { grid_length_type::auto_size, 0 }; }
        static grid_length pixels(double px) { return { grid_length_type::pixel, px }; }
        static grid_length star(double multiplier = 1.0) { return { grid_length_type::star, multiplier }; }
    };

    // Grid cell position
    struct grid_position {
        int row = 0;
        int column = 0;
        int row_span = 1;
        int column_span = 1;
    };

    // Alignment for controls within grid cells
    struct grid_alignment {
        alignment horizontal = alignment::stretch;
        alignment vertical = alignment::stretch;
    };

    // Grid panel - arranges children in rows and columns
    class grid_panel : public panel {
    public:
        explicit grid_panel(HWND parent = nullptr);
        virtual ~grid_panel() = default;

        // Add a control to this panel (defaults to next available cell)
        void add(control_ptr<> control) override;

        // Add a control at a specific grid position
        void add(control_ptr<> control, int row, int column, int row_span = 1, int column_span = 1);
        void add(control_ptr<> control, const grid_position& position);

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

        // Configure rows and columns
        void add_row_definition(const grid_length& height);
        void add_column_definition(const grid_length& width);

        void set_row_definitions(const std::vector<grid_length>& rows);
        void set_column_definitions(const std::vector<grid_length>& columns);

        // Clear all row/column definitions
        void clear_row_definitions() { m_row_definitions.clear(); }
        void clear_column_definitions() { m_column_definitions.clear(); }

        // Set grid position for a control
        void set_grid_position(control_ptr<> control, int row, int column, int row_span = 1, int column_span = 1);
        void set_grid_position(control_ptr<> control, const grid_position& position);
        grid_position get_grid_position(control_ptr<> control) const;

        // Set alignment for a control within its cell
        void set_alignment(control_ptr<> control, alignment horizontal, alignment vertical);
        void set_alignment(control_ptr<> control, const grid_alignment& align);
        grid_alignment get_alignment(control_ptr<> control) const;

        // Spacing between cells
        void set_row_spacing(int spacing) { m_row_spacing = spacing; }
        int get_row_spacing() const { return m_row_spacing; }

        void set_column_spacing(int spacing) { m_column_spacing = spacing; }
        int get_column_spacing() const { return m_column_spacing; }

        void set_spacing(int spacing) { 
            m_row_spacing = spacing;
            m_column_spacing = spacing;
        }

    private:
        std::vector<grid_length> m_row_definitions;
        std::vector<grid_length> m_column_definitions;
        std::unordered_map<control_ptr<>, grid_position> m_grid_positions;
        std::unordered_map<control_ptr<>, grid_alignment> m_alignments;

        int m_row_spacing = 0;
        int m_column_spacing = 0;

        // Calculated sizes during measure/arrange
        std::vector<int> m_row_heights;
        std::vector<int> m_column_widths;

        // Helper functions
        void ensure_grid_capacity(int row, int column);
        grid_position get_next_available_position() const;
        void calculate_track_sizes(
            const std::vector<grid_length>& definitions,
            int available_size,
            int spacing,
            const std::vector<int>& auto_sizes,
            std::vector<int>& out_sizes
        );
    };
}

#endif // WPP_LAYOUT_GRID_PANEL_HPP
