#ifndef WPP_WINDOW_BASE_HPP
#define WPP_WINDOW_BASE_HPP

#include "common.hpp"
#include "controls/control.hpp"

namespace wpp
{
	/// <summary>
	/// Base class for window management that provides common window operations and functionality.
	/// </summary>
	class window_base : public hwnd {
	public:
		/// <summary>
		/// Constructs a window_base object with the specified resource ID and optional parent window.
		/// </summary>
		/// <param name="resource_id">The resource identifier for the window.</param>
		/// <param name="parent">An optional handle to the parent window. Defaults to NULL if not specified.</param>
		window_base(int resource_id, HWND parent = NULL)
			: hwnd(resource_id, parent) {
		}

		/// <summary>
		/// Constructs a window_base object with the specified window handle.
		/// </summary>
		/// <param name="handle">The Windows handle (HWND) to associate with this window base object.</param>
		window_base(HWND handle)
			: hwnd(handle) {
		}

		/// <summary>
		/// Virtual destructor for the window_base class.
		/// </summary>
		virtual ~window_base() noexcept = default;

		/// <summary>
		/// Shows or hides the window with the specified display state.
		/// </summary>
		/// <param name="nCmdShow">Controls how the window is to be shown (e.g., SW_NORMAL, SW_HIDE, SW_MAXIMIZE). Defaults to SW_NORMAL.</param>
		/// <returns>Returns TRUE if the window was previously visible, FALSE if it was previously hidden.</returns>
		virtual BOOL show(int nCmdShow = SW_NORMAL) {
			return ::ShowWindow(m_handle, nCmdShow);
		}

		/// <summary>
		/// Hides the window.
		/// </summary>
		/// <returns>Returns TRUE if the window was previously visible, FALSE if it was already hidden.</returns>
		virtual BOOL hide() {
			return ::ShowWindow(m_handle, SW_HIDE);
		}

		/// <summary>
		/// Centers the window relative to another window.
		/// </summary>
		/// <param name="hWndCenter">Handle to the window to center relative to. Defaults to the desktop window.</param>
		/// <returns>TRUE if the window was successfully centered, FALSE otherwise.</returns>
		virtual BOOL center(HWND hWndCenter = HWND_DESKTOP) {
			return center_window(hWndCenter);
		}

		/// <summary>
		/// Closes the window.
		/// </summary>
		/// <returns>TRUE if the window was successfully closed; otherwise, FALSE.</returns>
		virtual BOOL close() {
			return ::CloseWindow(m_handle);
		}

		/// <summary>
		/// Displays a modal message box dialog.
		/// </summary>
		/// <param name="message">The message text to display in the message box.</param>
		/// <param name="title">The text to display in the title bar of the message box.</param>
		/// <param name="type">The message box type, specifying the buttons, icon, and behavior.</param>
		/// <returns>An integer value indicating which button the user clicked.</returns>
		virtual int message_box(const tstring& message, const tstring& title, UINT type) {
			return ::MessageBox(m_handle, message.c_str(), title.c_str(), type);
		}

		/// <summary>
		/// Displays an informational message box with formatted text.
		/// </summary>
		/// <typeparam name="...Args">The types of the format arguments.</typeparam>
		/// <param name="title">The title text for the message box.</param>
		/// <param name="format_str">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		/// <returns>The user's response.</returns>
		template<typename... Args>
		int message_box_info(const tstring& title, tstring_view format_str, Args&&... args) {
			return message_box(format_tstring(format_str, std::forward<Args>(args)...), title, MB_OK | MB_ICONINFORMATION);
		}

		/// <summary>
		/// Displays a warning message box with formatted text.
		/// </summary>
		/// <typeparam name="...Args">The types of the format arguments.</typeparam>
		/// <param name="title">The title text for the message box.</param>
		/// <param name="format_str">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		/// <returns>The user's response.</returns>
		template<typename... Args>
		int message_box_warn(const tstring& title, tstring_view format_str, Args&&... args) {
			return message_box(format_tstring(format_str, std::forward<Args>(args)...), title, MB_OK | MB_ICONWARNING);
		}

		/// <summary>
		/// Displays an error message box with formatted text.
		/// </summary>
		/// <typeparam name="...Args">The types of the format arguments.</typeparam>
		/// <param name="title">The title text for the message box.</param>
		/// <param name="format_str">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		/// <returns>The user's response.</returns>
		template<typename... Args>
		int message_box_error(const tstring& title, tstring_view format_str, Args&&... args) {
			return message_box(format_tstring(format_str, std::forward<Args>(args)...), title, MB_OK | MB_ICONERROR);
		}

		/// <summary>
		/// Adds a new timer with the specified interval and callback function.
		/// </summary>
		/// <param name="interval">The time interval in milliseconds for the timer.</param>
		/// <param name="fn">The callback function to be invoked when the timer fires.</param>
		/// <returns>A unique identifier for the created timer that can be used to reference or remove it later.</returns>
		UINT_PTR add_timer(UINT interval, timer::callback fn) {
			static UINT_PTR next_id = 1;
			UINT_PTR id = next_id++;
			m_timers[id] = std::make_shared<timer>(m_handle, id, interval, std::move(fn));
			return id;
		}

		/// <summary>
		/// Removes a timer with the specified identifier.
		/// </summary>
		/// <param name="id">The identifier of the timer to remove.</param>
		void remove_timer(UINT_PTR id) {
			m_timers.erase(id);
		}

		/// <summary>
		/// Handles a timer event by invoking its associated callback.
		/// </summary>
		/// <param name="id">The identifier of the timer to handle.</param>
		/// <returns>true if the timer was found and invoked; false otherwise.</returns>
		bool handle_timer(UINT_PTR id) {
			auto it = m_timers.find(id);
			if (it != m_timers.end()) {
				it->second->invoke();
				return true;
			}
			return false;
		}

		/// <summary>
		/// Gets a read-only reference to the controls collection.
		/// </summary>
		/// <returns>A const reference to the vector of controls.</returns>
		virtual const std::vector<control_ptr<>>& get_controls() const {
			static const std::vector<control_ptr<>> empty_controls{};
			return empty_controls;
		}

	protected:
		std::unordered_map<UINT_PTR, std::shared_ptr<timer>> m_timers; ///< Map of active timers associated with the window.
	};
}

#endif // WPP_WINDOW_BASE_HPP
