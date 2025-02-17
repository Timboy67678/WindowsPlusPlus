#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "winplusplus.h"

#define DIALOG_MESSAGE_HANDLER(X) virtual INT_PTR CALLBACK X(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define DIALOG_NOTIFY_HANDLER(X) virtual INT_PTR CALLBACK X(HWND hWnd, UINT_PTR control_id, LPNMHDR nm)

#define DIALOG_MESSAGE_REF(X) static_cast<WPP::Dialog::DIALOG_MESSAGE_CALLBACK>(X)
#define DIALOG_NOTIFY_REF(X) static_cast<WPP::Dialog::DIALOG_NOTIFY_CALLBACK>(X)
#define DIALOG_TIMER_REF(X) static_cast<WPP::Dialog::TIMER_CALLBACK>(X)

#define DIALOG_TIMER_OFFSET_START 0x1374

namespace WPP
{
	/**
	* @class Dialog
	* @brief A class representing a dialog window.
	*/
	class Dialog : public Hwnd
	{
	public:
		typedef INT_PTR(CALLBACK Dialog::* DIALOG_MESSAGE_CALLBACK)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		typedef INT_PTR(CALLBACK Dialog::* DIALOG_NOTIFY_CALLBACK)(HWND hWnd, UINT_PTR control_id, LPNMHDR nm);
		typedef void (CALLBACK Dialog::* TIMER_CALLBACK)();

		/**
		* @brief Constructs a Dialog object.
		* @param instance The instance handle.
		* @param resource_id The resource ID of the dialog.
		* @param menu_id The menu ID of the dialog (default is -1).
		*/
		Dialog(HINSTANCE instance, int resource_id, int menu_id = -1);

		/**
		* @brief Constructs a Dialog object from an existing window handle.
		* @param hWnd The window handle.
		*/
		Dialog(HWND hWnd);

		/**
		* @brief Destroys the Dialog object.
		*/
		virtual ~Dialog() = default;

		DIALOG_MESSAGE_HANDLER(OnInitDialog);
		DIALOG_MESSAGE_HANDLER(OnClose);
		DIALOG_MESSAGE_HANDLER(OnTimer);
		DIALOG_MESSAGE_HANDLER(OnNotify);
		DIALOG_MESSAGE_HANDLER(OnCommand);
		DIALOG_MESSAGE_HANDLER(OnDestroy);
		DIALOG_MESSAGE_HANDLER(OnDisplayChange);
		DIALOG_MESSAGE_HANDLER(OnMove);
		DIALOG_MESSAGE_HANDLER(OnMenuCommand);
		DIALOG_MESSAGE_HANDLER(OnPaint);
		DIALOG_MESSAGE_HANDLER(OnSize);
		DIALOG_MESSAGE_HANDLER(OnKeyDown);
		DIALOG_MESSAGE_HANDLER(OnKeyUp);
		DIALOG_MESSAGE_HANDLER(OnHScroll);
		DIALOG_MESSAGE_HANDLER(OnVScroll);
		DIALOG_MESSAGE_HANDLER(OnDropFiles);

		/**
		* @brief Centers the dialog window.
		* @param hWndCenter The handle of the window to center relative to (default is NULL).
		* @return TRUE if successful, FALSE otherwise.
		*/
		BOOL CenterWindow(HWND hWndCenter = NULL);

		/**
		* @brief Runs the dialog.
		* @param parent The parent window handle (default is NULL).
		* @param param Additional parameter (default is NULL).
		* @return The result of the dialog procedure.
		*/
		virtual INT_PTR CALLBACK RunDlg(HWND parent = NULL, LPVOID param = NULL);

		/**
		* @brief The dialog procedure.
		* @param hWnd The window handle.
		* @param Msg The message.
		* @param wParam Additional message information.
		* @param lParam Additional message information.
		* @return The result of the message processing.
		*/
		virtual INT_PTR DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/**
		* @brief Shows the dialog.
		* @param show The show state (default is SW_NORMAL).
		*/
		void Show(INT show = SW_NORMAL);

		/**
		* @brief Ends the dialog.
		*/
		void EndDialog();

		/**
		* @brief Adds a timer event.
		* @param timer_elapse The timer elapse time.
		* @param callback The callback function.
		*/
		template < typename DC >
		void AddTimer(INT timer_elapse, DC callback)
		{
			const UINT_PTR timer_id = ++m_InternalTimerID + DIALOG_TIMER_OFFSET_START;
			if (::SetTimer(m_hWnd, timer_id, timer_elapse, NULL) != 0)
				m_TimerEvents[timer_id] = DIALOG_TIMER_REF(callback);
		}

		/**
		* @brief Adds a command event.
		* @param id The command ID.
		* @param callback The callback function.
		*/
		template <typename DC>
		void AddCommandEvent(INT id, DC callback)
		{
			m_CommandEvents[id] = COMMAND_ID_REF(callback);
		}

		/**
		* @brief Adds a notify message event.
		* @param id The notify message ID.
		* @param callback The callback function.
		*/
		template<typename DC>
		void AddNotifyMessage(INT id, DC callback)
		{
			m_NotifyEvents[id] = DIALOG_NOTIFY_REF(callback);
		}

		/**
		* @brief Registers a control.
		* @param control_id The control ID.
		* @param ctrl The control object.
		* @return TRUE if successful, FALSE otherwise.
		*/
		template <typename CtrlType = Control>
		bool RegisterControl(UINT control_id, std::shared_ptr<CtrlType>&& ctrl)
		{
			ctrl = std::make_shared<CtrlType>(control_id, m_hWnd);

			if (ctrl != nullptr)
				m_MappedControls.emplace(control_id, ctrl);

			return ctrl != nullptr;
		}

		/**
		* @brief Gets a control.
		* @param control_id The control ID.
		* @return The control object.
		*/
		template< typename CtrlType = Control>
		std::shared_ptr<CtrlType> GetControl(UINT control_id)
		{
			return m_MappedControls[control_id];
		}

		/**
		* @brief Displays a message box.
		* @param message The message to display.
		* @param title The title of the message box.
		* @param type The type of the message box.
		* @return The result of the message box.
		*/
		virtual int MsgBox(LPCTSTR message, LPCTSTR title, UINT type)
		{
			return ::MessageBox(m_hWnd, message, title, type);
		}

		/**
		* @brief Displays an information message box.
		* @param title The title of the message box.
		* @param fmt The format string.
		* @param ... The format arguments.
		* @return The result of the message box.
		*/
		virtual int MsgBoxInfo(LPCTSTR title, LPCTSTR fmt, ...)
		{
			TCHAR buffer[1024 * 6] = { 0 };
			va_list va;
			va_start(va, fmt);
#ifdef _UNICODE
			_vsnwprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#else
			_vsnprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#endif
			va_end(va);
			return MsgBox(buffer, title, MB_OK | MB_ICONINFORMATION);
		}

		/**
		* @brief Displays an error message box.
		* @param title The title of the message box.
		* @param fmt The format string.
		* @param ... The format arguments.
		* @return The result of the message box.
		*/
		virtual int MsgBoxError(LPCTSTR title, LPCTSTR fmt, ...)
		{
			TCHAR buffer[1024 * 6] = { 0 };
			va_list va;
			va_start(va, fmt);
#ifdef _UNICODE
			_vsnwprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#else
			_vsnprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#endif
			va_end(va);
			return MsgBox(buffer, title, MB_OK | MB_ICONERROR);
		}

		/**
		* @brief Displays a warning message box.
		* @param title The title of the message box.
		* @param fmt The format string.
		* @param ... The format arguments.
		* @return The result of the message box.
		*/
		virtual int MsgBoxWarn(LPCTSTR title, LPCTSTR fmt, ...)
		{
			TCHAR buffer[1024 * 6] = { 0 };
			va_list va;
			va_start(va, fmt);
#ifdef _UNICODE
			_vsnwprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#else
			_vsnprintf_s(buffer, ARRAYSIZE(buffer), fmt, va);
#endif
			va_end(va);
			return MsgBox(buffer, title, MB_OK | MB_ICONWARNING);
		}

	protected:
		std::map<INT, DIALOG_MESSAGE_CALLBACK> m_MessageEvents;
		std::map<INT, COMMAND_ID_MESSAGE_CALLBACK> m_CommandEvents;
		std::map<UINT_PTR, DIALOG_NOTIFY_CALLBACK> m_NotifyEvents;
		std::map<UINT_PTR, TIMER_CALLBACK> m_TimerEvents;
		std::map<UINT, std::shared_ptr<Control>> m_MappedControls;

		UINT_PTR m_InternalTimerID;

		HINSTANCE m_MainInstance;

		int m_MenuID;

		HMENU m_Menu;
	};
}

#endif //__DIALOG_H__
