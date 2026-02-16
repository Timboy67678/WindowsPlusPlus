#ifndef WPP_WINDOW_HPP
#define WPP_WINDOW_HPP

#include "winplusplus.hpp"
#include "window_base.hpp"
#include "message_loop.hpp"
#include "layout.hpp"

namespace wpp
{
	/// <summary>
	/// Constructs a window class from an existing WNDCLASS structure.
	/// </summary>
	struct window_class {
		friend class window;

		/// <summary>
		/// Default constructor for the window_class.
		/// </summary>
		window_class() = default;

		/// <summary>
		/// Constructs a window class with the specified properties for registering with Windows.
		/// </summary>
		/// <param name="name">The class name for the window.</param>
		/// <param name="instance">The instance handle of the module that contains the window procedure. Defaults to NULL.</param>
		/// <param name="icon">A handle to the class icon. Defaults to NULL.</param>
		/// <param name="cursor">A handle to the class cursor. Defaults to NULL.</param>
		/// <param name="background">A handle to the class background brush. Defaults to COLOR_WINDOW.</param>
		/// <param name="menu">A pointer to a null-terminated string that specifies the resource name of the class menu. Defaults to NULL.</param>
		/// <param name="style">The class style(s). Defaults to 0.</param>
		/// <param name="extra">The number of extra bytes to allocate following the window instance. Defaults to 0.</param>
		window_class(const tstring& name, HINSTANCE instance = NULL, HICON icon = NULL, HCURSOR cursor = NULL,
					 HBRUSH background = (HBRUSH)(COLOR_WINDOW), LPCTSTR menu = NULL, UINT style = 0, int extra = 0) {
			ZeroMemory(&m_window_class, sizeof(WNDCLASSEX));
			m_window_class.cbSize = sizeof(WNDCLASSEX);
			m_window_class.style = style;
			m_window_class.cbClsExtra = 0;
			m_window_class.cbWndExtra = extra;
			m_window_class.hInstance = instance;
			m_window_class.hIcon = icon;
			m_window_class.hCursor = cursor;
			m_window_class.hbrBackground = background;
			m_window_class.lpszMenuName = menu;
			m_window_class.lpszClassName = name.c_str();
			m_window_class.hIconSm = icon;
			m_window_class.lpfnWndProc = nullptr; // Will be set by the window class when registering
		}

		window_class(WNDCLASS window_class) {
			ZeroMemory(&m_window_class, sizeof(WNDCLASSEX));
			std::memcpy(&m_window_class, &window_class, sizeof(WNDCLASS));
		}

	protected:
		/// <summary>
		/// Gets the instance handle associated with the window class.
		/// </summary>
		/// <returns>The HINSTANCE handle of the application or module.</returns>
		HINSTANCE instance() const { return m_window_class.hInstance; }

		/// <summary>
		/// Gets the window class name.
		/// </summary>
		/// <returns>A pointer to the window class name string.</returns>
		LPCTSTR class_name() const { return m_window_class.lpszClassName; }

		/// <summary>
		/// Gets the window class style flags.
		/// </summary>
		/// <returns>The style flags of the window class.</returns>
		UINT style() const { return m_window_class.style; }

		/// <summary>
		/// Registers the window class with the system.
		/// </summary>
		void Register() {
			m_class_atom = ::RegisterClassEx(&m_window_class);
		}

		/// <summary>
		/// Unregisters a window class that was previously registered with the system.
		/// </summary>
		void Unregister() {
			::UnregisterClass(m_window_class.lpszClassName, m_window_class.hInstance);
			m_class_atom = NULL;
		}

		/// <summary>
		/// Gets a reference to the class atom.
		/// </summary>
		/// <returns>A reference to the ATOM member variable.</returns>
		ATOM& atom() { return m_class_atom; }

		/// <summary>
		/// Gets a reference to the window class structure.
		/// </summary>
		/// <returns>A reference to the WNDCLASSEX structure.</returns>
		WNDCLASSEX& get() { return m_window_class; }

	private:
		WNDCLASSEX m_window_class; ///< Window class structure.
		ATOM m_class_atom = NULL; ///< Class atom.
	};

	/// <summary>
	/// A Windows GUI window class that provides functionality for creating and managing a window with various controls and event handlers.
	/// </summary>
	class window : public window_base {
	public:
		using menu_callback = std::function<void(WPARAM, LPARAM)>;
		using window_message_callback = std::function<LRESULT(HWND, WPARAM, LPARAM)>;
		using message_handler = LRESULT(HWND hWnd, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Manages a group of radio buttons within a window.
		/// </summary>
		class radio_button_group {
			friend class window;
		public:

			/// <summary>
			/// Constructs a radio button group associated with a parent window.
			/// </summary>
			/// <param name="parent">Pointer to the parent window that will contain this radio button group.</param>
			radio_button_group(window* parent)
				: m_parent(parent) {
			}

			/// <summary>
			/// Destroys the radio_button_group object.
			/// </summary>
			~radio_button_group() = default;

			/// <summary>
			/// Creates a radio button control with the specified text and dimensions.
			/// </summary>
			/// <param name="text">The text label to display on the radio button.</param>
			/// <param name="width">The width of the radio button in pixels. Defaults to 80.</param>
			/// <param name="height">The height of the radio button in pixels. Defaults to 25.</param>
			/// <param name="initial_state">The initial selection state of the radio button. Defaults to FALSE (unselected).</param>
			/// <returns>A smart pointer to the created radio button control.</returns>
			control_ptr<radio_button> create_button(const tstring& text, int width = 80, int height = 25, BOOL initial_state = FALSE);

			/// <summary>
			/// Retrieves the index of the currently selected radio button within the group.
			/// </summary>
			/// <returns>The index of the selected radio button, or -1 if none are selected.</returns>
			int selected_index();

		private:
			std::vector<control_ptr<radio_button>> m_radio_buttons; ///< Vector of radio buttons.
			window* m_parent; ///< Pointer to the parent window.
		};

		/// <summary>
		/// Constructs a window with the specified class, name, dimensions, and style parameters.
		/// </summary>
		/// <param name="wnd_class">The window class that defines the window's characteristics.</param>
		/// <param name="window_name">The name or title of the window.</param>
		/// <param name="width">The width of the window in pixels.</param>
		/// <param name="height">The height of the window in pixels.</param>
		/// <param name="style">The window style flags. Defaults to WS_OVERLAPPEDWINDOW.</param>
		/// <param name="menu_id">The resource identifier for the window's menu. Defaults to -1 (no menu resource).</param>
		/// <param name="menu">A handle to the window's menu. Defaults to NULL.</param>
		/// <param name="font">A handle to the font used by the window. Defaults to NULL.</param>
		/// <param name="style_ex">Extended window style flags. Defaults to 0.</param>
		window(window_class wnd_class, const tstring& window_name, int width, int height, DWORD style = WS_OVERLAPPEDWINDOW,
			   int menu_id = -1, HMENU menu = NULL, HFONT font = NULL, DWORD style_ex = 0);

		/// <summary>
		/// Destroys the window object and releases its resources.
		/// </summary>
		virtual ~window() noexcept;

		virtual message_handler on_create;
		virtual message_handler on_close;
		virtual message_handler on_timer;
		virtual message_handler on_notify;
		virtual message_handler on_command;
		virtual message_handler on_destroy;
		virtual message_handler on_display_change;
		virtual message_handler on_move;
		virtual message_handler on_menu_command;
		virtual message_handler on_paint;
		virtual message_handler on_size;
		virtual message_handler on_key_down;
		virtual message_handler on_key_up;
		virtual message_handler on_h_scroll;
		virtual message_handler on_v_scroll;
		virtual message_handler on_drop_files;
		virtual message_handler on_dpi_changed;
		virtual message_handler on_ctl_color_edit;
		virtual message_handler on_ctl_color_static;
		virtual message_handler on_min_max_info;

		/// <summary>
		/// Creates a window with the specified layout and position.
		/// </summary>
		/// <param name="layout">A shared pointer to the panel layout to use for the window.</param>
		/// <param name="x_pos">The x-coordinate of the window's position. Defaults to 0.</param>
		/// <param name="y_pos">The y-coordinate of the window's position. Defaults to 0.</param>
		/// <param name="parent_window">A handle to the parent window. Defaults to HWND_DESKTOP.</param>
		/// <param name="param">Additional parameter data to pass to the window. Defaults to NULL.</param>
		/// <returns>True if the window was created successfully; otherwise, false.</returns>
		virtual bool create_window(std::shared_ptr<layout::panel> layout, int x_pos = 0, int y_pos = 0, HWND parent_window = HWND_DESKTOP, LPVOID param = NULL);

		/// <summary>
		/// Creates and runs a window with the specified layout and positioning.
		/// </summary>
		/// <param name="layout">A shared pointer to the panel layout to display in the window.</param>
		/// <param name="x_pos">The x-coordinate position of the window. Defaults to 0.</param>
		/// <param name="y_pos">The y-coordinate position of the window. Defaults to 0.</param>
		/// <param name="parent_window">The handle to the parent window. Defaults to HWND_DESKTOP.</param>
		/// <param name="param">An optional parameter to pass to the window. Defaults to NULL.</param>
		/// <returns>True if the window was successfully created and run; otherwise, false.</returns>
		virtual bool run_window(std::shared_ptr<layout::panel> layout, int x_pos = 0, int y_pos = 0, HWND parent_window = HWND_DESKTOP, LPVOID param = NULL);

		/// <summary>
		/// Processes Windows messages for the window.
		/// </summary>
		/// <param name="hWnd">A handle to the window that receives the message.</param>
		/// <param name="Msg">The message identifier indicating the type of message.</param>
		/// <param name="wParam">Additional message-specific information.</param>
		/// <param name="lParam">Additional message-specific information.</param>
		/// <returns>The result of the message processing, depending on the message type.</returns>
		virtual LRESULT window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Quits or closes a window with the specified exit code.
		/// </summary>
		/// <param name="exit_code">The exit code to return when quitting the window. Defaults to 0.</param>
		void quit_window(INT exit_code = 0);

		/// <summary>
		/// Creates a new radio button group control.
		/// </summary>
		/// <returns>A control pointer to the newly created radio button group.</returns>
		control_ptr<window::radio_button_group> create_radio_button_group();

		/// <summary>
		/// Creates a button control with the specified properties.
		/// </summary>
		/// <param name="text">The text to display on the button.</param>
		/// <param name="width">The width of the button in pixels. Defaults to 80.</param>
		/// <param name="height">The height of the button in pixels. Defaults to 25.</param>
		/// <param name="style">The window style flags for the button. Defaults to a standard push button style.</param>
		/// <param name="style_ex">The extended window style flags for the button. Defaults to 0.</param>
		/// <returns>A pointer to the created button control.</returns>
		control_ptr<button> create_button(const tstring& text, int width = 80, int height = 25, DWORD style = BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, DWORD style_ex = 0);

		/// <summary>
		/// Creates a check box control with the specified properties.
		/// </summary>
		/// <param name="text">The text label to display next to the check box.</param>
		/// <param name="width">The width of the check box control in pixels. Default is 100.</param>
		/// <param name="height">The height of the check box control in pixels. Default is 25.</param>
		/// <param name="initial_state">The initial checked state of the check box. TRUE for checked, FALSE for unchecked. Default is FALSE.</param>
		/// <param name="style">The window style flags for the check box control. Default is BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED.</param>
		/// <param name="style_ex">The extended window style flags for the check box control. Default is 0.</param>
		/// <returns>A smart pointer to the created check box control.</returns>
		control_ptr<check_box> create_check_box(const tstring& text, int width = 100, int height = 25, BOOL initial_state = false, DWORD style = BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, DWORD style_ex = 0);

		/// <summary>
		/// Creates a group box control with the specified text and dimensions.
		/// </summary>
		/// <param name="text">The text to display as the group box caption.</param>
		/// <param name="width">The width of the group box in pixels. Defaults to 200.</param>
		/// <param name="height">The height of the group box in pixels. Defaults to 100.</param>
		/// <param name="style">The window style flags for the group box. Defaults to BS_GROUPBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED.</param>
		/// <param name="style_ex">The extended window style flags for the group box. Defaults to 0.</param>
		/// <returns>A smart pointer to the created group box control.</returns>
		control_ptr<group_box> create_group_box(const tstring& text, int width = 200, int height = 100, DWORD style = BS_GROUPBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, DWORD style_ex = 0);

		/// <summary>
		/// Creates a static control with the specified text and properties.
		/// </summary>
		/// <param name="text">The text to display in the static control.</param>
		/// <param name="width">The width of the control in pixels. Defaults to 200.</param>
		/// <param name="height">The height of the control in pixels. Defaults to 20.</param>
		/// <param name="style">The window style flags for the control. Defaults to SS_LEFT | WS_CHILD | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the control. Defaults to 0.</param>
		/// <returns>A smart pointer to the created static control.</returns>
		control_ptr<static_control> create_static_control(const tstring& text, int width = 200, int height = 20, DWORD style = SS_LEFT | WS_CHILD | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a combo box control with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the combo box in pixels. Defaults to 200.</param>
		/// <param name="height">The height of the combo box in pixels. Defaults to 200.</param>
		/// <param name="style">The window style flags for the combo box. Defaults to a dropdown list with vertical scroll, visible as a child window.</param>
		/// <param name="style_ex">The extended window style flags for the combo box. Defaults to 0 (no extended styles).</param>
		/// <returns>A smart pointer to the created combo box control.</returns>
		control_ptr<combo_box> create_combo_box(int width = 200, int height = 200, DWORD style = CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, DWORD style_ex = 0);

		/// <summary>
		/// Creates an edit text control with the specified properties.
		/// </summary>
		/// <param name="initial_text">The initial text to display in the edit control. Defaults to an empty string.</param>
		/// <param name="width">The width of the edit control in pixels. Defaults to 200.</param>
		/// <param name="height">The height of the edit control in pixels. Defaults to 25.</param>
		/// <param name="style">The window style flags for the edit control. Defaults to ES_LEFT | WS_CHILD | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the edit control. Defaults to WS_EX_CLIENTEDGE.</param>
		/// <returns>A smart pointer to the created edit text control.</returns>
		control_ptr<edit_text> create_edit_text(const tstring& initial_text = _T(""), int width = 200, int height = 25, DWORD style = ES_LEFT | WS_CHILD | WS_VISIBLE, DWORD style_ex = WS_EX_CLIENTEDGE);

		/// <summary>
		/// Creates a list box control with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the list box in pixels. Default is 200.</param>
		/// <param name="height">The height of the list box in pixels. Default is 150.</param>
		/// <param name="style">The window style flags for the list box. Default is LBS_STANDARD | WS_CHILD | WS_BORDER | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the list box. Default is 0.</param>
		/// <returns>A smart pointer to the created list box control.</returns>
		control_ptr<list_box> create_list_box(int width = 200, int height = 150, DWORD style = LBS_STANDARD | WS_CHILD | WS_BORDER | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a list view control with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the list view control in pixels. Defaults to 300.</param>
		/// <param name="height">The height of the list view control in pixels. Defaults to 200.</param>
		/// <param name="style">The window style flags for the list view. Defaults to a report-style, visible child window.</param>
		/// <param name="style_ex">The extended window style flags. Defaults to a client edge border.</param>
		/// <returns>A smart pointer to the created list view control.</returns>
		control_ptr<list_view> create_list_view(int width = 300, int height = 200, DWORD style = LVS_REPORT | WS_CHILD | WS_VISIBLE, DWORD style_ex = WS_EX_CLIENTEDGE);

		/// <summary>
		/// Creates a tree view control with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the tree view control in pixels. Defaults to 250.</param>
		/// <param name="height">The height of the tree view control in pixels. Defaults to 200.</param>
		/// <param name="style">The window style flags for the tree view control. Defaults to a combination of TVS_HASLINES, TVS_LINESATROOT, TVS_HASBUTTONS, WS_CHILD, and WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the tree view control. Defaults to WS_EX_CLIENTEDGE.</param>
		/// <returns>A smart pointer to the created tree view control.</returns>
		control_ptr<tree_view> create_tree_view(int width = 250, int height = 200, DWORD style = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CHILD | WS_VISIBLE, DWORD style_ex = WS_EX_CLIENTEDGE);

		/// <summary>
		/// Creates a tab control with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the tab control in pixels. Default is 300.</param>
		/// <param name="height">The height of the tab control in pixels. Default is 200.</param>
		/// <param name="style">The window style flags for the tab control. Default is TCS_MULTILINE | WS_CHILD | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the tab control. Default is 0.</param>
		/// <returns>A smart pointer to the created tab control.</returns>
		control_ptr<tab_control> create_tab_control(int width = 300, int height = 200, DWORD style = TCS_MULTILINE | WS_CHILD | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a progress bar control.
		/// </summary>
		/// <param name="width">The width of the progress bar in pixels. Default is 200.</param>
		/// <param name="height">The height of the progress bar in pixels. Default is 20.</param>
		/// <param name="style">The window style flags for the progress bar. Default is PBS_SMOOTH | WS_CHILD | WS_BORDER | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the progress bar. Default is 0.</param>
		/// <returns>A control pointer to the created progress bar.</returns>
		control_ptr<progress_bar> create_progress_bar(int width = 200, int height = 20, DWORD style = PBS_SMOOTH | WS_CHILD | WS_BORDER | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a spin control (up-down control) with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the control in pixels. Defaults to 50.</param>
		/// <param name="height">The height of the control in pixels. Defaults to 25.</param>
		/// <param name="style">The window style flags for the control. Defaults to a combination of UDS_SETBUDDYINT, UDS_ALIGNRIGHT, UDS_ARROWKEYS, UDS_NOTHOUSANDS, WS_CHILD, WS_BORDER, and WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the control. Defaults to 0.</param>
		/// <returns>A pointer to the created up-down control.</returns>
		control_ptr<up_down_control> create_spin_control(int width = 50, int height = 25, DWORD style = UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS | WS_CHILD | WS_BORDER | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a rich edit text control with the specified properties.
		/// </summary>
		/// <param name="initial_text">The initial text content to display in the rich edit control.</param>
		/// <param name="width">The width of the control in pixels.</param>
		/// <param name="height">The height of the control in pixels.</param>
		/// <param name="style">The window style flags for the control (e.g., ES_MULTILINE, WS_VISIBLE).</param>
		/// <param name="style_ex">The extended window style flags for the control.</param>
		/// <returns>A smart pointer to the created rich edit text control.</returns>
		control_ptr<rich_edit_text> create_rich_edit(const tstring& initial_text = _T(""), int width = 300, int height = 200, DWORD style = ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE, DWORD style_ex = WS_EX_CLIENTEDGE);

		/// <summary>
		/// Creates a system link control with the specified text and dimensions.
		/// </summary>
		/// <param name="text">The text content to display in the link control.</param>
		/// <param name="width">The width of the control in pixels. Defaults to 200.</param>
		/// <param name="height">The height of the control in pixels. Defaults to 20.</param>
		/// <param name="style">The window style flags for the control. Defaults to WS_CHILD | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the control. Defaults to 0.</param>
		/// <returns>A smart pointer to the created system link control.</returns>
		control_ptr<sys_link> create_link_control(const tstring& text, int width = 200, int height = 20, DWORD style = WS_CHILD | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a scroll bar control.
		/// </summary>
		/// <param name="orientation">The orientation of the scroll bar (horizontal or vertical).</param>
		/// <param name="width">The width of the scroll bar in pixels. Default is 200.</param>
		/// <param name="height">The height of the scroll bar in pixels. Default is 20.</param>
		/// <param name="style">The window style flags for the scroll bar. Default is WS_CHILD | WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the scroll bar. Default is 0.</param>
		/// <returns>A pointer to the created scroll bar control.</returns>
		control_ptr<scroll_bar> create_scroll_bar(scroll_orientation orientation, int width = 200, int height = 20, DWORD style = WS_CHILD | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Creates a track bar control (slider).
		/// </summary>
		/// <param name="width">The width of the track bar in pixels. Defaults to 200.</param>
		/// <param name="height">The height of the track bar in pixels. Defaults to 30.</param>
		/// <param name="style">The window style flags for the track bar. Defaults to WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS.</param>
		/// <param name="style_ex">The extended window style flags for the track bar. Defaults to 0.</param>
		/// <returns>A control pointer to the created track bar control.</returns>
		control_ptr<track_bar> create_track_bar(int width = 200, int height = 30, DWORD style = WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, DWORD style_ex = 0);

		/// <summary>
		/// Creates an up-down control (spinner control) with the specified dimensions and styles.
		/// </summary>
		/// <param name="width">The width of the control in pixels. Defaults to 50.</param>
		/// <param name="height">The height of the control in pixels. Defaults to 25.</param>
		/// <param name="style">The window style flags for the control. Defaults to a combination of UDS_SETBUDDYINT, UDS_ALIGNRIGHT, UDS_ARROWKEYS, UDS_NOTHOUSANDS, WS_CHILD, WS_BORDER, and WS_VISIBLE.</param>
		/// <param name="style_ex">The extended window style flags for the control. Defaults to 0.</param>
		/// <returns>A control pointer to the created up-down control.</returns>
		control_ptr<up_down_control> create_updown_control(int width = 50, int height = 25, DWORD style = UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS | WS_CHILD | WS_BORDER | WS_VISIBLE, DWORD style_ex = 0);

		/// <summary>
		/// Registers or unregisters a callback function for a menu command.
		/// </summary>
		/// <param name="menu_id">The unique identifier of the menu command.</param>
		/// <param name="callback">The callback function to register for the menu command, or null to unregister an existing callback.</param>
		inline void register_menu_command(UINT_PTR menu_id, menu_callback callback) {
			if (callback)
				m_menu_command_events[menu_id] = std::move(callback);
			else
				m_menu_command_events.erase(menu_id);
		}

		/// <summary>
		/// Gets a read-only reference to the layout panel.
		/// </summary>
		/// <returns>A const reference to the layout panel.</returns>
		inline const auto& get_layout() const { return m_layout_panel; }
		
		/// <summary>
		/// Sets whether the window should keep its minimum size when resized.
		/// </summary>
		/// <param name="keep_minimum">True to keep the minimum size, false otherwise.</param>
		inline void set_keep_minimum_resize(bool keep_minimum) {
			m_keep_minimum_size = keep_minimum;
			if (m_handle) {
				::SetWindowPos(m_handle, NULL, 0, 0, 0, 0,
							   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			}
		}

		/// <summary>
		/// Sets the minimum size constraints for the window.
		/// </summary>
		/// <param name="width">The minimum width in pixels.</param>
		/// <param name="height">The minimum height in pixels.</param>
		inline void set_minimum_size(int width, int height) {
			m_original_width = width;
			m_original_height = height;
			if (m_handle && m_keep_minimum_size) {
				::SetWindowPos(m_handle, NULL, 0, 0, 0, 0,
							   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			}
		}

		/// <summary>
		/// Retrieves a control from the collection by its ID and casts it to the specified type.
		/// </summary>
		/// <typeparam name="CtrlType">The type of control to cast to. Defaults to the base control type.</typeparam>
		/// <param name="control_id">The unique identifier of the control to retrieve.</param>
		/// <returns>A shared pointer to the control cast as the specified type, or nullptr if the control is not found or the cast fails.</returns>
		template<typename CtrlType = control>
		inline control_ptr<CtrlType> get_control(UINT control_id) {
			auto it = std::find_if(m_controls.begin(), m_controls.end(), [control_id](const control_ptr<control>& control) {
				return control && control->get_id() == control_id;
			});
			if (it != m_controls.end() && *it) {
				return std::dynamic_pointer_cast<CtrlType>(*it);
			}
			return nullptr;
		}

		/// <summary>
		/// Retrieves a control from the collection by its window handle.
		/// </summary>
		/// <typeparam name="CtrlType">The type of control to retrieve. Defaults to the base control type.</typeparam>
		/// <param name="handle">The window handle (HWND) of the control to find.</param>
		/// <returns>A smart pointer to the control with the specified handle, cast to the requested type, or nullptr if not found or if the cast fails.</returns>
		template<typename CtrlType = control>
		inline control_ptr<CtrlType> get_control_by_handle(HWND handle) {
			auto it = std::find_if(m_controls.begin(), m_controls.end(), [handle](const control_ptr<control>& control) {
				return control && control->get_handle() == handle;
			});
			if (it != m_controls.end() && *it) {
				return std::dynamic_pointer_cast<CtrlType>(*it);
			}
			return nullptr;
		}

	private:
		void init_message_events();
		void cleanup();
		void update_layout();
		bool handle_scroll_message(scroll_orientation orientation, WPARAM wParam, LPARAM lParam);

	protected:
		std::unique_ptr<void, void(*)(void*)> m_thunk_storage{ nullptr, +[](void* p) {} }; ///< Thunk storage for window procedure.
		window_class m_window_class; ///< Window class.
		int m_x_pos, m_y_pos; ///< Initial startup position of the window
		int m_original_width, m_original_height; ///< Window original size.
		tstring m_window_name; ///< Window name.
		HMENU m_menu_handle; ///< Menu handle.
		HFONT m_font; ///< Font handle.
		int m_menu_id; ///< Menu ID.
		bool m_keep_minimum_size = false; ///< Flag to keep minimum size when window is resized.
		DWORD m_style, m_style_ex; ///< Window styles.
		UINT m_control_id = WM_USER + 1; ///< Control ID index.
		UINT_PTR m_internal_timer_id = 0; ///< Internal timer ID.
		std::atomic_bool m_window_running = false; ///< Window running flag.
		std::map<INT, window_message_callback> m_message_events; ///< Message events.
		std::map<UINT_PTR, menu_callback> m_menu_command_events; ///< Menu command events.
		std::shared_ptr<layout::panel> m_layout_panel; ///< Layout panel for automatic control arrangement.
		std::vector<control_ptr<>> m_controls; ///< Controls container.
	};
}

#endif // WPP_WINDOW_HPP
