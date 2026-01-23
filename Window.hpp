#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "winplusplus.h"

constexpr auto WINDOW_TIMER_OFFSET_START = 0x2374;

namespace wpp
{
	/**
	* @struct window_class
	* @brief Encapsulates window class information.
	*/
	struct window_class {
		friend class window;

		/**
		 * @brief Default constructor.
		 */
		window_class() = default;

		/**
		 * @brief Parameterized constructor.
		 * @param name Class name.
		 * @param instance Instance handle.
		 * @param icon Icon handle.
		 * @param cursor Cursor handle.
		 * @param background Background brush.
		 * @param menu Menu name.
		 * @param style Class style.
		 * @param extra Extra bytes.
		 */
		window_class(LPCTSTR name, HINSTANCE instance = NULL, HICON icon = NULL, HCURSOR cursor = NULL,
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
			m_window_class.lpszClassName = name;
			m_window_class.hIconSm = icon;
		}

	protected:
		/**
		 * @brief Gets the instance handle.
		 * @return Instance handle.
		 */
		HINSTANCE instance() const { return m_window_class.hInstance; }

		/**
		 * @brief Gets the class name.
		 * @return Class name.
		 */
		LPCTSTR class_name() const { return m_window_class.lpszClassName; }

		/**
		 * @brief Gets the class style.
		 * @return Class style.
		 */
		UINT style() const { return m_window_class.style; }

		/**
		 * @brief Registers the window class.
		 */
		void Register() {
			m_class_atom = RegisterClassEx(&m_window_class);
		}

		/**
		 * @brief Unregisters the window class.
		 */
		void Unregister() {
			UnregisterClass(m_window_class.lpszClassName, m_window_class.hInstance);
			m_class_atom = NULL;
		}

		/**
		 * @brief Gets the class atom.
		 * @return Class atom.
		 */
		ATOM& atom() { return m_class_atom; }

		/**
		 * @brief Gets the window class structure.
		 * @return Window class structure.
		 */
		WNDCLASSEX& get() { return m_window_class; }

	private:
		WNDCLASSEX m_window_class; ///< Window class structure.
		ATOM m_class_atom = NULL; ///< Class atom.
	};

	class window : public hwnd {
	public:
		using menu_callback = std::function<void(WPARAM, LPARAM)>;
		using timer_callback = std::function<void()>;
		using window_message_callback = std::function<LRESULT(HWND, WPARAM, LPARAM)>;
		using message_handler = LRESULT(HWND hWnd, WPARAM wParam, LPARAM lParam);

		/**
		* @class RadioButtonGroup
		* @brief Manages a group of radio buttons within a window.
		*/
		class radio_button_group {
			friend class window;
		public:
			/**
			* @brief Constructor.
			* @param parent Pointer to the parent window.
			*/
			radio_button_group(window* parent)
				: m_parent(parent) {
			}

			virtual ~radio_button_group() = default;

			/**
			* @brief Creates a radio button.
			* @param control_id Control ID.
			* @param text Button text.
			* @param x X position.
			* @param y Y position.
			* @param width Button width.
			* @param height Button height.
			* @param initial_state Initial state of the button.
			* @return Pointer to the created radio button.
			*/
			std::shared_ptr<radio_button> create_button(LPCTSTR text, int x, int y, int width, int height, BOOL initial_state = FALSE);

			/**
			* @brief Gets the index of the selected radio button.
			* @return Index of the selected radio button.
			*/
			int selected_index();

		private:
			std::vector<std::shared_ptr<radio_button>> m_radio_buttons; ///< Vector of radio buttons.
			window* m_parent; ///< Pointer to the parent window.
		};

		/**
		 * @brief Constructor.
		 * @param wnd_class Window class.
		 * @param window_name Window name.
		 * @param x_pos X position.
		 * @param y_pos Y position.
		 * @param width Width.
		 * @param height Height.
		 * @param style Window style.
		 * @param menu_id Menu ID.
		 * @param menu Menu handle.
		 * @param param Additional parameters.
		 * @param style_ex Extended window style.
		 */
		window(window_class wnd_class, LPCTSTR window_name, int x_pos, int y_pos, int width, int height, DWORD style = WS_OVERLAPPEDWINDOW,
			   int menu_id = -1, HMENU menu = NULL, HFONT font = NULL, DWORD style_ex = 0);

		/**
		 * @brief Destructor.
		 */
		virtual ~window() noexcept;

		virtual message_handler on_create;
		virtual message_handler on_close;
		virtual message_handler on_quit;
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
		virtual message_handler on_ctl_color_edit;
		virtual message_handler on_ctl_color_static;

		/**
		 * @brief Runs the window message loop.
		 * @return True if window was created and ran, false otherwise.
		 */
		virtual bool run_window(HWND parent_window = HWND_DESKTOP, LPVOID param = NULL);

		/**
		 * @brief Window procedure.
		 * @param hWnd Window handle.
		 * @param Msg Message.
		 * @param wParam WPARAM.
		 * @param lParam LPARAM.
		 * @return Result of message processing.
		 */
		virtual LRESULT window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/**
		 * @brief Displays a message box.
		 * @param message Message text.
		 * @param title Title text.
		 * @param type Message box type.
		 * @return Result of the message box.
		 */
		virtual int message_box(LPCTSTR message, LPCTSTR title, UINT type);

		/**
		 * @brief Displays an information message box.
		 * @param title Title text.
		 * @param fmt Format string.
		 * @param ... Additional arguments.
		 * @return Result of the message box.
		 */
		virtual int message_box_info(LPCTSTR title, LPCTSTR fmt, ...);

		/**
		 * @brief Displays an error message box.
		 * @param title Title text.
		 * @param fmt Format string.
		 * @param ... Additional arguments.
		 * @return Result of the message box.
		 */
		virtual int message_box_error(LPCTSTR title, LPCTSTR fmt, ...);

		/**
		 * @brief Displays a warning message box.
		 * @param title Title text.
		 * @param fmt Format string.
		 * @param ... Additional arguments.
		 * @return Result of the message box.
		 */
		virtual int message_box_warn(LPCTSTR title, LPCTSTR fmt, ...);

		/**
		 * @brief Shows the window.
		 */
		void show_window();

		/**
		 * @brief Hides the window.
		 */
		void hide_window();

		/**
		 * @brief Closes the window.
		 */
		void close_window();

		/**
		 * @brief Quits the window.
		 * @param exit_code Exit code.
		 */
		void quit_window(INT exit_code = 0);

		/**
		* @brief Creates a radio button group.
		* @return Pointer to the created radio button group.
		*/
		std::shared_ptr<window::radio_button_group> create_radio_button_group();

		// Create window controls
		std::shared_ptr<button> create_button(LPCTSTR text, int x, int y, int width, int height);
		std::shared_ptr<check_box> create_check_box(LPCTSTR text, int x, int y, int width, int height, BOOL initial_state = false);
		std::shared_ptr<group_box> create_group_box(LPCTSTR text, int x, int y, int width, int height);
		std::shared_ptr<static_control> create_static_control(LPCTSTR text, int x, int y, int width, int height);
		std::shared_ptr<combo_box> create_combo_box(int x, int y, int width, int height);
		std::shared_ptr<edit_text> create_edit_text(int x, int y, int width, int height, LPCTSTR initial_text = _T(""));
		std::shared_ptr<list_box> create_list_box(int x, int y, int width, int height);
		std::shared_ptr<list_view> create_list_view(int x, int y, int width, int height);
		std::shared_ptr<tree_view> create_tree_view(int x, int y, int width, int height);
		std::shared_ptr<tab_control> create_tab_control(int x, int y, int width, int height);
		std::shared_ptr<progress_bar> create_progress_bar(int x, int y, int width, int height);
		std::shared_ptr<up_down_control> create_spin_control(int x, int y, int width, int height);
		std::shared_ptr<rich_edit_text> create_rich_edit(int x, int y, int width, int height, LPCTSTR initial_text = _T(""));
		std::shared_ptr<sys_link> create_link_control(LPCTSTR text, int x, int y, int width, int height);
		std::shared_ptr<up_down_control> create_updown_control(int x, int y, int width, int height);

		/**
		 * @brief Registers a menu control callback.
		 * @param control_id Control ID.
		 * @param callback Callback for the menu item.
		 */
		void register_menu_command(UINT_PTR menu_id, menu_callback callback) {
			if (callback)
				m_menu_command_events[menu_id] = std::move(callback);
			else
				m_menu_command_events.erase(menu_id);
		}

		/**
		 * @brief Adds a timer event.
		 * @tparam DC Callback type.
		 * @param timer_elapse Timer interval in milliseconds.
		 * @param callback Callback function.
		 */
		template<typename DC>
		void add_timer(INT timer_elapse, DC callback) {
			const UINT_PTR timer_id = ++m_internal_timer_id + WINDOW_TIMER_OFFSET_START;
			if (::SetTimer(m_handle, timer_id, timer_elapse, NULL) != 0)
				m_timer_events[timer_id] = callback;
		}

		/**
		* @brief Gets a control by ID.
		* @tparam CtrlType Control type.
		* @param control_id Control ID.
		* @return Pointer to the control.
		*/
		template<typename CtrlType = control>
		inline std::shared_ptr<CtrlType> get_control(UINT control_id) {
			auto it = std::find_if(m_controls.begin(), m_controls.end(), [control_id](const std::shared_ptr<control>& control) {
				return control && control->get_id() == control_id;
			});

			if (it != m_controls.end() && *it) {
				return std::dynamic_pointer_cast<CtrlType>(*it);
			}
			return nullptr;
		}

	private:
		void init_message_events();
		void cleanup();

	protected:
		window_class m_window_class; ///< Window class.
		int m_x_pos, m_y_pos, m_width, m_height; ///< Window position and size.
		std::tstring m_window_name; ///< Window name.
		HMENU m_menu_handle; ///< Menu handle.
		HFONT m_font; ///< Font handle.
		int m_menu_id; ///< Menu ID.
		DWORD m_style, m_style_ex; ///< Window styles.
		UINT m_control_id = WM_USER + 1; ///< Control ID index.
		UINT_PTR m_internal_timer_id = 0; ///< Internal timer ID.
		std::atomic_bool m_window_running = false; ///< Window running flag.
		std::map<INT, window_message_callback> m_message_events; ///< Message events.
		std::map<UINT_PTR, timer_callback> m_timer_events; ///< Timer events.
		std::map<UINT_PTR, menu_callback> m_menu_command_events; ///< Menu command events.
		std::vector<std::shared_ptr<control>> m_controls; ///< Controls container.
	};
}

#endif //__WINDOW_H__
