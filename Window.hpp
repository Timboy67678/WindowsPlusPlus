#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "winplusplus.h"
#include "Thunk.hpp"

#define WINDOW_TIMER_HANDLER(X) virtual void CALLBACK X()
#define WINDOW_MESSAGE_HANDLER(X) virtual INT_PTR CALLBACK X(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define WINDOW_NOTIFY_HANDLER(X) virtual INT_PTR CALLBACK X(HWND hWnd, UINT_PTR control_id, LPNMHDR nm)

#define WINDOW_MESSAGE_REF(X) static_cast<WPP::Window::WINDOW_MESSAGE_CALLBACK>(X)
#define WINDOW_NOTIFY_REF(X) static_cast<WPP::Window::WINDOW_NOTIFY_CALLBACK>(X)
#define WINDOW_TIMER_REF(X) static_cast<WPP::Window::TIMER_CALLBACK>(X)

#define WINDOW_TIMER_OFFSET_START 0x2374

namespace WPP
{
	class Window : public Hwnd
	{
	public:
		typedef INT_PTR(CALLBACK Window::* WINDOW_MESSAGE_CALLBACK)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		typedef INT_PTR(CALLBACK Window::* WINDOW_NOTIFY_CALLBACK)(HWND hWnd, UINT_PTR control_id, LPNMHDR nm);
		typedef void (CALLBACK Window::*TIMER_CALLBACK)();

		struct Class {
			friend class Window;

			Class() = default;

			Class(LPCTSTR name, HINSTANCE instance = NULL, HICON icon = NULL, HCURSOR cursor = NULL,
				  HBRUSH background = (HBRUSH)GetStockObject(WHITE_BRUSH), LPCTSTR menu = NULL, UINT style = 0, int extra = 0)
			{
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
			HINSTANCE instance() const { return m_window_class.hInstance; }
			LPCTSTR class_name() const { return m_window_class.lpszClassName; }
			UINT Style() const { return m_window_class.style; }

			void Register() {
				m_class_atom = RegisterClassEx(&m_window_class);
			}

			void Unregister() {
				UnregisterClass(m_window_class.lpszClassName, m_window_class.hInstance);
			}

			ATOM& atom() { return m_class_atom; }
			WNDCLASSEX& get() { return m_window_class; }

		private:
			WNDCLASSEX m_window_class;
			ATOM m_class_atom = NULL;
		};

		Window(Class wnd_class, LPCTSTR window_name, int x_pos, int y_pos, int width, int height, DWORD style = WS_OVERLAPPEDWINDOW,
			   int menu_id = -1, HMENU menu = NULL, LPVOID param = NULL, DWORD style_ex = WS_EX_OVERLAPPEDWINDOW);
		virtual ~Window();

		WINDOW_MESSAGE_HANDLER(OnCreate);
		WINDOW_MESSAGE_HANDLER(OnClose);
		WINDOW_MESSAGE_HANDLER(OnMove);
		WINDOW_MESSAGE_HANDLER(OnMenuCommand);
		WINDOW_MESSAGE_HANDLER(OnCommand);
		WINDOW_MESSAGE_HANDLER(OnPaint);
		WINDOW_MESSAGE_HANDLER(OnTimer);
		WINDOW_MESSAGE_HANDLER(OnSize);
		WINDOW_MESSAGE_HANDLER(OnKeyDown);
		WINDOW_MESSAGE_HANDLER(OnKeyUp);
		WINDOW_MESSAGE_HANDLER(OnNotify);
		WINDOW_MESSAGE_HANDLER(OnHScroll);
		WINDOW_MESSAGE_HANDLER(OnVScroll);
		WINDOW_MESSAGE_HANDLER(OnDropFiles);

		virtual bool WINAPI Create(HWND parent_window = HWND_DESKTOP);
		virtual void WINAPI RunWindow();
		virtual LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		void Show(INT show = SW_NORMAL);
		void CloseWindow();
		void QuitWindow(INT exit_code = 0);
		void EnableDragDrop(BOOL state = TRUE);
		BOOL CenterWindow(HWND hWndCenter = NULL);

		//Create window controls
		Button* CreateButton(UINT control_id, LPCTSTR text, int x, int y, int width, int height);
		CheckBox* CreateCheckBox(UINT control_id, LPCTSTR text, int x, int y, int width, int height, BOOL initial_state = false);

		template<typename DC>
		void AddTimer(INT timer_elapse, DC callback)
		{
			const UINT_PTR timer_id = ++m_InternalTimerID + WINDOW_TIMER_OFFSET_START;
			if (::SetTimer(m_hWnd, timer_id, timer_elapse, NULL) != 0)
				m_TimerEvents[timer_id] = WINDOW_TIMER_REF(callback);
		}

		template<typename DC>
		void AddCommandEvent(UINT control_id, DC callback)
		{
			m_CommandEvents[control_id] = COMMAND_ID_REF(callback);
		}

		template<typename DC>
		void AddNotifyMessage(INT id, DC callback)
		{
			m_NotifyEvents[id] = WINDOW_NOTIFY_REF(callback);
		}

		template<typename CtrlType = Control>
		CtrlType* GetControl(HWND hWnd)
		{
			return static_cast<CtrlType*>(m_MappedControls[hWnd]);
		}

		virtual int MsgBox(LPCTSTR message, LPCTSTR title, UINT type);
		virtual int MsgBoxInfo(LPCTSTR title, LPCTSTR fmt, ...);
		virtual int MsgBoxError(LPCTSTR title, LPCTSTR fmt, ...);
		virtual int MsgBoxWarn(LPCTSTR title, LPCTSTR fmt, ...);

	protected:
		Class m_WindowClass;
		int m_XPos, m_YPos, m_Width, m_Height;
		std::tstring m_WindowName;
		HMENU m_Menu;
		int m_MenuID;
		LPVOID m_Param;
		DWORD m_Style, m_StyleEx;
		UINT_PTR m_InternalTimerID = 0;
		bool m_WindowCreated = false;
		volatile bool m_WindowRunning = false;
		std::unique_ptr<Win32Thunk<WNDPROC, Window>> m_WindowProcThunk;

	private:
		std::map<INT, WINDOW_MESSAGE_CALLBACK> m_MessageEvents;
		std::map<UINT, COMMAND_ID_MESSAGE_CALLBACK> m_CommandEvents;
		std::map<UINT_PTR, WINDOW_NOTIFY_CALLBACK> m_NotifyEvents;
		std::map<UINT_PTR, TIMER_CALLBACK> m_TimerEvents;
		std::map<UINT, Control*> m_MappedControls;
	};
}

#endif //__WINDOW_H__