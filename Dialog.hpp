#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "winplusplus.h"

constexpr auto DIALOG_TIMER_OFFSET_START = 0x1374;

namespace wpp
{
	/**
	* @class Dialog
	* @brief A class representing a dialog window.
	*/
	class dialog : public hwnd {
	public:
		using menu_callback = std::function<void(WPARAM, LPARAM)>;
		using timer_callback = std::function<void()>;
		using dialog_message_callback = std::function<INT_PTR(HWND, WPARAM, LPARAM)>;
		using message_handler = INT_PTR(HWND hWnd, WPARAM wParam, LPARAM lParam);

		/**
		* @brief Constructs a Dialog object.
		* @param instance The instance handle.
		* @param resource_id The resource ID of the dialog.
		* @param menu_id The menu ID of the dialog (default is -1).
		*/
		dialog(HINSTANCE instance, int resource_id, int menu_id = -1);

		/**
		* @brief Constructs a Dialog object from an existing window handle.
		* @param hWnd The window handle.
		*/
		dialog(HWND hWnd);

		/**
		* @brief Destroys the Dialog object.
		*/
		virtual ~dialog() noexcept;

		virtual message_handler on_init_dialog;
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

		/**
		* @brief Runs the dialog.
		* @param parent The parent window handle (default is NULL).
		* @param param Additional parameter (default is NULL).
		* @return The result of the dialog procedure.
		*/
		virtual INT_PTR run_dlg(HWND parent = NULL, LPVOID param = NULL);

		/**
		* @brief The dialog procedure.
		* @param hWnd The window handle.
		* @param Msg The message.
		* @param wParam Additional message information.
		* @param lParam Additional message information.
		* @return The result of the message processing.
		*/
		virtual INT_PTR dialog_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/**
		* @brief Shows the dialog.
		*/
		void show_dialog();

		/**
		* @brief Hides the dialog.
		*/
		void hide_dialog();

		/**
		* @brief Ends the dialog.
		*/
		void end_dialog();

		/**
		* @brief Registers a menu command
		*/
		void register_menu_command(UINT_PTR menu_id, menu_callback callback) {
			if (callback)
				m_menu_command_events[menu_id] = std::move(callback);
			else
				m_menu_command_events.erase(menu_id);
		}

		/**
		* @brief Adds a timer event.
		* @param timer_elapse The timer elapse time.
		* @param callback The callback function.
		*/
		template < typename DC >
		void add_timer(INT timer_elapse, DC callback) {
			const UINT_PTR timer_id = ++m_internal_timerid + DIALOG_TIMER_OFFSET_START;
			if (::SetTimer(m_handle, timer_id, timer_elapse, NULL) != 0)
				m_timer_events[timer_id] = DIALOG_TIMER_REF(callback);
		}

		/**
		* @brief Registers a control.
		* @param control_id The control ID.
		* @param ctrl The control object (output parameter).
		* @return TRUE if successful, FALSE otherwise.
		*/
		template<typename CtrlType = control>
		bool register_control(UINT control_id, std::shared_ptr<CtrlType>& ctrl) {
			ctrl = std::make_shared<CtrlType>(control_id, m_handle);

			if (ctrl != nullptr)
				m_controls.emplace_back(ctrl);

			return ctrl != nullptr;
		}

		/**
		* @brief Gets a control.
		* @param control_id The control ID.
		* @return The control object.
		*/
		template<typename CtrlType = control>
		std::shared_ptr<CtrlType> get_control(UINT control_id) {
			for (auto& control : m_controls)
				if (control && control->get_id() == control_id)
					return std::dynamic_pointer_cast<CtrlType>(control);
			return nullptr;
		}

		template<typename CtrlType = control>
		inline std::shared_ptr<CtrlType> get_control_by_handle(HWND handle) {
			auto it = std::find_if(m_controls.begin(), m_controls.end(), [handle](const std::shared_ptr<control>& control) {
				return control && control->get_handle() == handle;
			});
			if (it != m_controls.end() && *it) {
				return std::dynamic_pointer_cast<CtrlType>(*it);
			}
			return nullptr;
		}

		/**
		* @brief Displays a message box.
		* @param message The message to display.
		* @param title The title of the message box.
		* @param type The type of the message box.
		* @return The result of the message box.
		*/
		virtual int message_box(LPCTSTR message, LPCTSTR title, UINT type) {
			return ::MessageBox(m_handle, message, title, type);
		}

		/**
		* @brief Displays an information message box.
		* @param title The title of the message box.
		* @param fmt The format string.
		* @param ... The format arguments.
		* @return The result of the message box.
		*/
		virtual int message_box_info(LPCTSTR title, LPCTSTR fmt, ...) {
			TCHAR buffer[1024 * 6] = { 0 };
			va_list va;
			va_start(va, fmt);
#ifdef _UNICODE
			_vsnwprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#else
			_vsnprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#endif
			va_end(va);
			return message_box(buffer, title, MB_OK | MB_ICONINFORMATION);
		}

		/**
		* @brief Displays an error message box.
		* @param title The title of the message box.
		* @param fmt The format string.
		* @param ... The format arguments.
		* @return The result of the message box.
		*/
		virtual int message_box_error(LPCTSTR title, LPCTSTR fmt, ...) {
			TCHAR buffer[1024 * 6] = { 0 };
			va_list va;
			va_start(va, fmt);
#ifdef _UNICODE
			_vsnwprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#else
			_vsnprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#endif
			va_end(va);
			return message_box(buffer, title, MB_OK | MB_ICONERROR);
		}

		/**
		* @brief Displays a warning message box.
		* @param title The title of the message box.
		* @param fmt The format string.
		* @param ... The format arguments.
		* @return The result of the message box.
		*/
		virtual int message_box_warn(LPCTSTR title, LPCTSTR fmt, ...) {
			TCHAR buffer[1024 * 6] = { 0 };
			va_list va;
			va_start(va, fmt);
#ifdef _UNICODE
			_vsnwprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#else
			_vsnprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#endif
			va_end(va);
			return message_box(buffer, title, MB_OK | MB_ICONWARNING);
		}

	private:
		void init_message_events();
		void cleanup();
		BOOL handle_scroll_message(scroll_orientation orientation, WPARAM wParam, LPARAM lParam);

	protected:
		std::map<UINT_PTR, menu_callback> m_menu_command_events;
		std::map<UINT_PTR, timer_callback> m_timer_events;
		std::map<INT, dialog_message_callback> m_message_events;
		std::vector<std::shared_ptr<control>> m_controls;

		UINT_PTR m_internal_timerid;

		HINSTANCE m_main_instance;

		int m_menu_id;

		HMENU m_menu;
	};
}

#endif //__DIALOG_H__
