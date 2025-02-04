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
		m_MessageEvents[WM_DESTROY] = &Window::OnDestroy;
		m_MessageEvents[WM_DISPLAYCHANGE] = &Window::OnDisplayChange;
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
		m_hWnd = ::CreateWindowEx(m_StyleEx, m_WindowClass.class_name(), m_WindowName.c_str(), m_Style,
									  m_XPos, m_YPos, m_Width, m_Height, m_Parent, m_Menu, m_WindowClass.instance(), m_Param);
		m_WindowCreated = true;
		Show(SW_SHOWNORMAL);
		::UpdateWindow(m_hWnd);
		return m_hWnd != NULL;
	}

	void Window::RunWindow()
	{
		if (!m_WindowCreated)
			return;

		m_WindowRunning = true;

		MSG msg;
		while (m_WindowRunning && GetMessage(&msg, NULL, 0, 0)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	LRESULT Window::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		m_hWnd = hWnd;
		LRESULT ret = FALSE;
		auto it = m_MessageEvents.find(Msg);
		if (it != m_MessageEvents.end())
			ret = (this->*(it->second))(hWnd, wParam, lParam);

		if (ret == FALSE) //was handled? otherwise send to default os handler
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		else
			return ret;
	}

#pragma region Window Control Creators

	Button* Window::CreateButton(UINT control_id, LPCTSTR text, int x, int y, int width, int height)
	{
		HWND button_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
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
		HWND checkbox_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!checkbox_handle)
			return nullptr;
		CheckBox* checkbox = new (std::nothrow) CheckBox(control_id, m_hWnd);
		if (checkbox == nullptr)
			return nullptr;
		checkbox->SetChecked(initial_state ? BST_CHECKED : BST_UNCHECKED);
		m_MappedControls.emplace(control_id, checkbox);
		return checkbox;
	}

	ComboBox* Window::CreateComboBox(UINT control_id, int x, int y, int width, int height)
	{
		HWND combobox_handle = ::CreateWindowEx(0, WC_COMBOBOX, _T(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!combobox_handle)
			return nullptr;
		ComboBox* combobox = new (std::nothrow) ComboBox(control_id, m_hWnd);
		if (combobox == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, combobox);
		return combobox;
	}

	EditText* Window::CreateEditText(UINT control_id, int x, int y, int width, int height, LPCTSTR initial_text)
	{
		HWND edittext_handle = ::CreateWindowEx(0, WC_EDIT, initial_text, ES_LEFT | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!edittext_handle)
			return nullptr;
		EditText* edittext = new (std::nothrow) EditText(control_id, m_hWnd);
		if (edittext == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, edittext);
		return edittext;
	}

	ListBox* Window::CreateListBox(UINT control_id, int x, int y, int width, int height)
	{
		HWND listbox_handle = ::CreateWindowEx(0, WC_LISTBOX, _T(""), LBS_STANDARD | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!listbox_handle)
			return nullptr;
		ListBox* listbox = new (std::nothrow) ListBox(control_id, m_hWnd);
		if (listbox == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, listbox);
		return listbox;
	}

	ListView* Window::CreateListView(UINT control_id, int x, int y, int width, int height)
	{
		HWND listview_handle = ::CreateWindowEx(0, WC_LISTVIEW, _T(""), LVS_REPORT | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!listview_handle)
			return nullptr;
		ListView* listview = new (std::nothrow) ListView(control_id, m_hWnd);
		if (listview == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, listview);
		return listview;
	}

	TreeView* Window::CreateTreeView(UINT control_id, int x, int y, int width, int height)
	{
		HWND treeview_handle = ::CreateWindowEx(0, WC_TREEVIEW, _T(""), TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!treeview_handle)
			return nullptr;
		TreeView* treeview = new (std::nothrow) TreeView(control_id, m_hWnd);
		if (treeview == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, treeview);
		return treeview;
	}

	TabControl* Window::CreateTabControl(UINT control_id, int x, int y, int width, int height)
	{
		HWND tabcontrol_handle = ::CreateWindowEx(0, WC_TABCONTROL, _T(""), TCS_MULTILINE | TCS_BUTTONS | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!tabcontrol_handle)
			return nullptr;
		TabControl* tabcontrol = new (std::nothrow) TabControl(control_id, m_hWnd);
		if (tabcontrol == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, tabcontrol);
		return tabcontrol;
	}

	ProgressBar* Window::CreateProgressBar(UINT control_id, int x, int y, int width, int height)
	{
		HWND progressbar_handle = ::CreateWindowEx(0, PROGRESS_CLASS, _T(""), PBS_SMOOTH | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!progressbar_handle)
			return nullptr;
		ProgressBar* progressbar = new (std::nothrow) ProgressBar(control_id, m_hWnd);
		if (progressbar == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, progressbar);
		return progressbar;
	}

	SpinControl* Window::CreateSpinControl(UINT control_id, int x, int y, int width, int height)
	{
		HWND spincontrol_handle = ::CreateWindowEx(0, UPDOWN_CLASS, _T(""), UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!spincontrol_handle)
			return nullptr;
		SpinControl* spincontrol = new (std::nothrow) SpinControl(control_id, m_hWnd);
		if (spincontrol == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, spincontrol);
		return spincontrol;
	}

	RichEdit* Window::CreateRichEdit(UINT control_id, int x, int y, int width, int height, LPCTSTR initial_text)
	{
		HWND richedit_handle = ::CreateWindowEx(0, RICHEDIT_CLASS, initial_text, ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!richedit_handle)
			return nullptr;
		RichEdit* richedit = new (std::nothrow) RichEdit(control_id, m_hWnd);
		if (richedit == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, richedit);
		return richedit;
	}

	LinkControl* Window::CreateLinkControl(UINT control_id, LPCTSTR text, int x, int y, int width, int height)
	{
		HWND linkcontrol_handle = ::CreateWindowEx(0, WC_LINK, text, WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!linkcontrol_handle)
			return nullptr;
		LinkControl* linkcontrol = new (std::nothrow) LinkControl(control_id, m_hWnd);
		if (linkcontrol == nullptr)
			return nullptr;
		m_MappedControls.emplace(control_id, linkcontrol);
		return linkcontrol;
	}
#pragma endregion

#pragma region Overidables
	LRESULT CALLBACK Window::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (m_MenuID != -1)
			m_Menu = ::LoadMenu(m_WindowClass.instance(), MAKEINTRESOURCE(m_MenuID));
		return TRUE;
	}

	LRESULT CALLBACK Window::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		CloseWindow();
		return FALSE;
	}

	LRESULT CALLBACK Window::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		QuitWindow();
		return TRUE;
	}

	LRESULT CALLBACK Window::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnDisplayChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		::InvalidateRect(m_hWnd, NULL, FALSE);
		return TRUE;
	}

	LRESULT CALLBACK Window::OnMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

    LRESULT CALLBACK Window::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        auto it = m_TimerEvents.find(static_cast<UINT_PTR>(wParam));
        if (it != m_TimerEvents.end()) {
            (this->*(it->second))();
            return TRUE;
        }
        return FALSE;
    }

	LRESULT CALLBACK Window::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (lParam) {
			LPNMHDR nm = reinterpret_cast<LPNMHDR>(lParam);
			auto it = m_NotifyEvents.find(nm->idFrom);
			if (it != m_NotifyEvents.end()) 
				return (this->*(it->second))(hWnd, nm->idFrom, nm);
		}
		return FALSE;
	}

	LRESULT CALLBACK Window::OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnVScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	LRESULT CALLBACK Window::OnMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

    LRESULT CALLBACK Window::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        auto it = m_CommandEvents.find(LOWORD(wParam));
        if (it != m_CommandEvents.end())
            return (this->*(it->second))(LOWORD(wParam), hWnd, wParam, lParam);
        return FALSE;
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
