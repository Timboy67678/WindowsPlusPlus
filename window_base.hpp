#ifndef WPP_WINDOW_BASE_HPP
#define WPP_WINDOW_BASE_HPP

#include "common.hpp"

namespace wpp
{
	/**
	* @class window_base
	* @brief Base class for window-style classes (windows and dialogs).
	* 
	* This class provides common functionality for window-style classes,
	* distinguishing them from simple controls that also inherit from hwnd.
	*/
	class window_base : public hwnd {
	public:
		/**
		* @brief Constructs a window_base object from resource ID.
		* @param resource_id The resource ID.
		* @param parent The parent window handle (default is NULL).
		*/
		window_base(int resource_id, HWND parent = NULL)
			: hwnd(resource_id, parent) {
		}

		/**
		* @brief Constructs a window_base object from an existing window handle.
		* @param handle The window handle.
		*/
		window_base(HWND handle)
			: hwnd(handle) {
		}

		/**
		* @brief Destroys the window_base object.
		*/
		virtual ~window_base() noexcept = default;

		/**
		* @brief Shows the window.
		* @param nCmdShow Show command (default is SW_NORMAL).
		* @return TRUE if successful, FALSE otherwise.
		*/
		virtual BOOL show(int nCmdShow = SW_NORMAL) {
			return ::ShowWindow(m_handle, nCmdShow);
		}

		/**
		* @brief Hides the window.
		* @return TRUE if successful, FALSE otherwise.
		*/
		virtual BOOL hide() {
			return ::ShowWindow(m_handle, SW_HIDE);
		}

		/**
		* @brief Centers the window relative to a specified window or desktop.
		* @param hWndCenter Window to center relative to (default is HWND_DESKTOP).
		* @return TRUE if successful, FALSE otherwise.
		*/
		virtual BOOL center(HWND hWndCenter = HWND_DESKTOP) {
			return center_window(hWndCenter);
		}

		/**
		* @brief Closes the window.
		* @return TRUE if successful, FALSE otherwise.
		*/
		virtual BOOL close() {
			return ::CloseWindow(m_handle);
		}

		/**
		* @brief Displays a message box associated with this window.
		* @param message The message text.
		* @param title The title text.
		* @param type The message box type flags.
		* @return The user's response.
		*/
		virtual int message_box(const tstring& message, const tstring& title, UINT type) {
			return ::MessageBox(m_handle, message.c_str(), title.c_str(), type);
		}

		/**
		* @brief Displays an informational message box.
		* @param title The title text.
		* @param format_str Format string.
		* @param args Format arguments.
		* @return The user's response.
		*/
		template<typename... Args>
		int message_box_info(const tstring& title, tstring_view format_str, Args&&... args) {
			return message_box(format_tstring(format_str, std::forward<Args>(args)...), title, MB_OK | MB_ICONINFORMATION);
		}

		/**
		* @brief Displays a warning message box.
		* @param title The title text.
		* @param format_str Format string.
		* @param args Format arguments.
		* @return The user's response.
		*/
		template<typename... Args>
		int message_box_warn(const tstring& title, tstring_view format_str, Args&&... args) {
			return message_box(format_tstring(format_str, std::forward<Args>(args)...), title, MB_OK | MB_ICONWARNING);
		}

		/**
		* @brief Displays an error message box.
		* @param title The title text.
		* @param format_str Format string.
		* @param args Format arguments.
		* @return The user's response.
		*/
		template<typename... Args>
		int message_box_error(const tstring& title, tstring_view format_str, Args&&... args) {
			return message_box(format_tstring(format_str, std::forward<Args>(args)...), title, MB_OK | MB_ICONERROR);
		}

		/**
		* @brief Adds a timer to the window.
		* @param interval The timer interval in milliseconds.
		* @param fn The callback function to be called when the timer elapses.
		* @return The timer ID.
		*/
		UINT_PTR add_timer(UINT interval, timer::callback fn) {
			static UINT_PTR next_id = 1;
			UINT_PTR id = next_id++;
			m_timers[id] = std::make_shared<timer>(m_handle, id, interval, std::move(fn));
			return id;
		}

		/**
		* @brief Removes a timer from the window.
		* @param id The timer ID.
		*/
		void remove_timer(UINT_PTR id) {
			m_timers.erase(id);
		}

		/**
		* @brief Handles a timer event.
		* @param id The timer ID.
		* @return true if the timer was handled, false otherwise.
		*/
		bool handle_timer(UINT_PTR id) {
			auto it = m_timers.find(id);
			if (it != m_timers.end()) {
				it->second->invoke();
				return true;
			}
			return false;
		}

	protected:
		std::unordered_map<UINT_PTR, std::shared_ptr<timer>> m_timers; ///< Map of active timers associated with the window.
	};
}

#endif // WPP_WINDOW_BASE_HPP
