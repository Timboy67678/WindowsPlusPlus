#include "..\Window.hpp"

namespace WPP 
{
	Window::Window(Class wnd_class, LPCTSTR window_name, int x_pos, int y_pos, int width, int height, DWORD style,
				   int menu_id, HMENU menu, LPVOID param, DWORD style_ex)
		: Hwnd(NULL), m_WindowClass(wnd_class), m_WindowName(window_name), m_XPos(x_pos), m_YPos(y_pos),
		m_Width(width), m_Height(height), m_Style(style), m_MenuID(menu_id), m_Menu(menu), m_Param(param), m_StyleEx(style_ex)
	{
		m_MessageEvents[WM_CREATE] = &Window::OnCreate;
		m_MessageEvents[WM_CLOSE] = &Window::OnClose;
		m_MessageEvents[WM_MOVE] = &Window::OnMove;
		m_MessageEvents[WM_COMMAND] = &Window::OnCommand;
		m_MessageEvents[WM_MENUCOMMAND] = &Window::OnMenuCommand;
		m_MessageEvents[WM_PAINT] = &Window::OnPaint;
		m_MessageEvents[WM_TIMER] = &Window::OnTimer;
		m_MessageEvents[WM_SIZE] = &Window::OnSize;
		m_MessageEvents[WM_KEYDOWN] = &Window::OnKeyDown;
		m_MessageEvents[WM_KEYUP] = &Window::OnKeyUp;
		m_MessageEvents[WM_NOTIFY] = &Window::OnNotify;
		m_MessageEvents[WM_HSCROLL] = &Window::OnHScroll;
		m_MessageEvents[WM_VSCROLL] = &Window::OnVScroll;
		m_MessageEvents[WM_DROPFILES] = &Window::OnDropFiles;
	}

	Window::~Window()
	{
		for (auto& control_pair : m_MappedControls)
		{
			if (control_pair.second != nullptr)
			{
				delete control_pair.second;
				control_pair.second = nullptr;
			}
		}
	}

	bool Window::Create(HWND parent_window)
	{
		m_Parent = parent_window;
		m_WindowProcThunk = std::make_unique<Win32Thunk<WNDPROC, Window>>(&Window::WindowProc, this);
		if (m_WindowClass.atom() != NULL)
			m_WindowClass.Unregister();
		m_WindowClass.get().lpfnWndProc = m_WindowProcThunk->GetThunk();
		m_WindowClass.Register();
		m_hWnd = CreateWindowEx(m_StyleEx, m_WindowClass.class_name(), m_WindowName.c_str(), m_Style,
									  m_XPos, m_YPos, m_Width, m_Height, m_Parent, m_Menu, m_WindowClass.instance(), m_Param);
		m_WindowCreated = true;
		Show(SW_SHOWDEFAULT);
		::UpdateWindow(m_hWnd);
		return m_hWnd != NULL;
	}

	void Window::RunWindow()
	{
		if (!m_WindowCreated)
			return;

		m_WindowRunning = true;

		MSG msg;
		while (m_WindowRunning && GetMessage(&msg, m_hWnd, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Button* Window::CreateButton(UINT control_id, LPCTSTR text, int x, int y, int width, int height)
	{
		HWND button_handle = ::CreateWindowEx(0, _T("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!button_handle)
			return nullptr;
		Button* button = new (std::nothrow) Button(control_id, m_hWnd);
		if (button == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, button);
		return button;
	}

	CheckBox* Window::CreateCheckBox(UINT control_id, LPCTSTR text, int x, int y, int width, int height, BOOL initial_state)
	{
		HWND checkbox_handle = ::CreateWindowEx(0, _T("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!checkbox_handle)
			return nullptr;
		CheckBox* checkbox = new (std::nothrow) CheckBox(control_id, m_hWnd);
		if (checkbox == nullptr)
			return nullptr;
		checkbox->SetChecked(initial_state ? BST_CHECKED : BST_UNCHECKED);
		m_MappedControls.emplace(control_id, checkbox);
		return checkbox;
	}

	LRESULT Window::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		m_hWnd = hWnd;
		if (m_MessageEvents.count(Msg) > 0)
			return (this->*m_MessageEvents[Msg])(hWnd, wParam, lParam);
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

#pragma region Overidables
	INT_PTR CALLBACK Window::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (m_MenuID != -1)
			m_Menu = ::LoadMenu(m_WindowClass.instance(), MAKEINTRESOURCE(m_MenuID));
		return TRUE;
	}

	INT_PTR CALLBACK Window::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		CloseWindow();
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (m_TimerEvents.count((UINT_PTR)wParam) > 0)
			(this->*m_TimerEvents[(UINT_PTR)wParam])();
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		::InvalidateRect(hWnd, NULL, TRUE);
		::UpdateWindow(hWnd);
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (lParam) 
		{
			LPNMHDR nm = (LPNMHDR)lParam;
			if (m_NotifyEvents.count(nm->idFrom) > 0)
				return (this->*m_NotifyEvents[nm->idFrom])(hWnd, nm->idFrom, nm);
		}
		return TRUE;
	}

	INT_PTR CALLBACK Window::OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnVScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Window::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (m_CommandEvents.count(LOWORD(wParam)) > 0)
			return (this->*m_CommandEvents[LOWORD(wParam)])(LOWORD(wParam), hWnd, wParam, lParam);
		return TRUE;
	}
#pragma endregion

	void Window::Show(INT show)
	{
		::ShowWindow(m_hWnd, show);
	}

	void Window::CloseWindow()
	{
		::DestroyWindow(m_hWnd);
	}

	void Window::QuitWindow(INT exit_code)
	{
		m_WindowRunning = false;
		::PostQuitMessage(exit_code);
	}

	void Window::EnableDragDrop(BOOL state)
	{
		::ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
		::ChangeWindowMessageFilterEx(m_hWnd, WM_COPYDATA, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
		::ChangeWindowMessageFilterEx(m_hWnd, WM_COPYGLOBALDATA, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
		::DragAcceptFiles(m_hWnd, state);
	}

	BOOL Window::CenterWindow(HWND hWndCenter)
	{
		// determine owner window to center against
		DWORD dwStyle = GetStyle();
		if (hWndCenter == NULL)
		{
			if (dwStyle & WS_CHILD)
				hWndCenter = ::GetParent(m_hWnd);
			else
				hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
		}

		// get coordinates of the window relative to its parent
		RECT rcDlg;
		::GetWindowRect(m_hWnd, &rcDlg);
		RECT rcArea;
		RECT rcCenter;
		HWND hWndParent;

		if (!(dwStyle & WS_CHILD))
		{
			// don't center against invisible or minimized windows
			if (hWndCenter != NULL)
			{
				DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
				if (!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
					hWndCenter = NULL;
			}

			// center within screen coordinates
			HMONITOR hMonitor = NULL;
			if (hWndCenter != NULL)
				hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
			else
				hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);

			MONITORINFO minfo = { 0 };
			minfo.cbSize = sizeof(MONITORINFO);
			BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
			rcArea = minfo.rcWork;

			if (hWndCenter == NULL)
				rcCenter = rcArea;
			else
				::GetWindowRect(hWndCenter, &rcCenter);
		} else {
			// center within parent client coordinates
			hWndParent = ::GetParent(m_hWnd);
			::GetClientRect(hWndParent, &rcArea);
			::GetClientRect(hWndCenter, &rcCenter);
			::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
		}

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if (xLeft + DlgWidth > rcArea.right)
			xLeft = rcArea.right - DlgWidth;
		if (xLeft < rcArea.left)
			xLeft = rcArea.left;

		if (yTop + DlgHeight > rcArea.bottom)
			yTop = rcArea.bottom - DlgHeight;
		if (yTop < rcArea.top)
			yTop = rcArea.top;

		// map screen coordinates to child coordinates
		return ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

#pragma region Message Box Abstracts
	int Window::MsgBox(LPCTSTR message, LPCTSTR title, UINT type)
	{
		return ::MessageBox(m_hWnd, message, title, type);
	}

	int Window::MsgBoxInfo(LPCTSTR title, LPCTSTR fmt, ...)
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

	int Window::MsgBoxError(LPCTSTR title, LPCTSTR fmt, ...)
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

	int Window::MsgBoxWarn(LPCTSTR title, LPCTSTR fmt, ...)
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
#pragma endregion
}
