#include "..\Window.hpp"

namespace WPP
{
	Window::Window(Class wnd_class, LPCTSTR window_name, int x_pos, int y_pos, int width, int height, DWORD style,
				   int menu_id, HMENU menu, HFONT font, DWORD style_ex)
		: Hwnd(NULL), m_WindowClass(wnd_class), m_WindowName(window_name), m_XPos(x_pos), m_YPos(y_pos),
		m_Width(width), m_Height(height), m_Style(style), m_MenuID(menu_id), m_Menu(menu), m_Font(font), m_StyleEx(style_ex)
	{
		// Initialize message events
		m_MessageEvents = {
			{WM_CREATE, &Window::OnCreate},
			{WM_CLOSE, &Window::OnClose},
			{WM_DESTROY, &Window::OnDestroy},
			{WM_DISPLAYCHANGE, &Window::OnDisplayChange},
			{WM_MOVE, &Window::OnMove},
			{WM_COMMAND, &Window::OnCommand},
			{WM_MENUCOMMAND, &Window::OnMenuCommand},
			{WM_PAINT, &Window::OnPaint},
			{WM_TIMER, &Window::OnTimer},
			{WM_SIZE, &Window::OnSize},
			{WM_KEYDOWN, &Window::OnKeyDown},
			{WM_KEYUP, &Window::OnKeyUp},
			{WM_NOTIFY, &Window::OnNotify},
			{WM_HSCROLL, &Window::OnHScroll},
			{WM_VSCROLL, &Window::OnVScroll},
			{WM_DROPFILES, &Window::OnDropFiles}
		};
	}

	bool Window::RunWindow(HWND parent_window, LPVOID param)
	{
		m_Parent = parent_window;

		if (m_WindowClass.atom() != NULL)
			m_WindowClass.Unregister();
		m_WindowProcThunk = std::make_unique<Win32Thunk<WNDPROC, Window>>(&Window::WindowProc, this);
		m_WindowClass.get().lpfnWndProc = m_WindowProcThunk->GetThunk();
		m_WindowClass.Register();
		m_hWnd = ::CreateWindowEx(m_StyleEx, m_WindowClass.class_name(), m_WindowName.c_str(), m_Style,
								  m_XPos, m_YPos, m_Width, m_Height, m_Parent, m_Menu, m_WindowClass.instance(), param);

		Show(SW_SHOWNORMAL);

		if (m_Font == NULL)
			m_Font = ::CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
								  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("MS Shell Dlg"));

		SetFont(m_Font);

		for (auto& control : m_MappedControls)
			control.second->SetFont(m_Font);

		::UpdateWindow(m_hWnd);

		m_WindowRunning = true;

		MSG msg;
		while (m_WindowRunning && ::GetMessage(&msg, NULL, NULL, NULL)) {
			if (!::IsDialogMessage(m_hWnd, &msg)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		m_WindowRunning = false;

		return true;
	}

	LRESULT Window::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		m_hWnd = hWnd;
		LRESULT ret = FALSE;
		auto it = m_MessageEvents.find(Msg);
		if (it != m_MessageEvents.end())
			ret = (this->*(it->second))(hWnd, wParam, lParam);

		if (ret == FALSE) //was handled? otherwise send to default os handler
			return ::DefWindowProc(hWnd, Msg, wParam, lParam);
		else
			return ret;
	}

#pragma region Window Control Creators

	std::shared_ptr<RadioButton> Window::RadioButtonGroup::CreateButton(UINT control_id, LPCTSTR text, int x, int y, int width, int height, BOOL initial_state)
	{
		DWORD style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED;
		if (m_RadioButtons.empty()) style |= WS_GROUP;

		HWND radiobutton_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_AUTORADIOBUTTON | style, x, y, width, height, m_Parent->m_hWnd, (HMENU)control_id, m_Parent->m_WindowClass.instance(), NULL);
		if (!radiobutton_handle)
			return nullptr;

		::SendMessage(radiobutton_handle, WM_SETFONT, (WPARAM)m_Parent->m_Font, TRUE);

		auto radiobutton = std::make_shared<RadioButton>(control_id, m_Parent->m_hWnd);
		if (!radiobutton)
			return nullptr;

		radiobutton->SetChecked(initial_state ? BST_CHECKED : BST_UNCHECKED);
		m_RadioButtons.push_back(radiobutton);
		m_Parent->m_MappedControls[control_id] = radiobutton;

		return radiobutton;
	}

	int Window::RadioButtonGroup::SelectedIndex()
	{
		for (size_t index = 0; index < m_RadioButtons.size(); ++index) {
			if (m_RadioButtons[index]->GetChecked() == BST_CHECKED)
				return index;
		}
		return -1;
	}

	std::shared_ptr<Window::RadioButtonGroup> Window::CreateRadioButtonGroup()
	{
		return std::make_shared<RadioButtonGroup>(this);
	}

	std::shared_ptr<Button> Window::CreateButton(UINT control_id, LPCTSTR text, int x, int y, int width, int height)
	{
		HWND button_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!button_handle)
			return nullptr;

		auto button = std::make_shared<Button>(control_id, m_hWnd);
		if (!button)
			return nullptr;

		m_MappedControls.emplace(control_id, button);
		return button;
	}

	std::shared_ptr<CheckBox> Window::CreateCheckBox(UINT control_id, LPCTSTR text, int x, int y, int width, int height, BOOL initial_state)
	{
		HWND checkbox_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!checkbox_handle)
			return nullptr;

		auto checkbox = std::make_shared<CheckBox>(control_id, m_hWnd);
		if (!checkbox) {
			::DestroyWindow(checkbox_handle);
			return nullptr;
		}

		checkbox->SetChecked(initial_state ? BST_CHECKED : BST_UNCHECKED);
		m_MappedControls.emplace(control_id, checkbox);
		return checkbox;
	}
	
	std::shared_ptr<ComboBox> Window::CreateComboBox(UINT control_id, int x, int y, int width, int height)
	{
		HWND combobox_handle = ::CreateWindowEx(0, WC_COMBOBOX, _T(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!combobox_handle)
			return nullptr;

		auto combobox = std::make_shared<ComboBox>(control_id, m_hWnd);
		if (!combobox) {
			::DestroyWindow(combobox_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, combobox);
		return combobox;
	}

	std::shared_ptr<EditText> Window::CreateEditText(UINT control_id, int x, int y, int width, int height, LPCTSTR initial_text)
	{
		HWND edittext_handle = ::CreateWindowEx(0, WC_EDIT, initial_text, ES_LEFT | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!edittext_handle)
			return nullptr;

		auto edittext = std::make_shared<EditText>(control_id, m_hWnd);
		if (!edittext) {
			::DestroyWindow(edittext_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, edittext);
		return edittext;
	}

	std::shared_ptr<ListBox> Window::CreateListBox(UINT control_id, int x, int y, int width, int height)
	{
		HWND listbox_handle = ::CreateWindowEx(0, WC_LISTBOX, _T(""), LBS_STANDARD | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!listbox_handle)
			return nullptr;

		auto listbox = std::make_shared<ListBox>(control_id, m_hWnd);
		if (!listbox) {
			::DestroyWindow(listbox_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, listbox);
		return listbox;
	}

	std::shared_ptr<ListView> Window::CreateListView(UINT control_id, int x, int y, int width, int height)
	{
		HWND listview_handle = ::CreateWindowEx(0, WC_LISTVIEW, _T(""), LVS_REPORT | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!listview_handle)
			return nullptr;

		auto listview = std::make_shared<ListView>(control_id, m_hWnd);
		if (!listview) {
			::DestroyWindow(listview_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, listview);
		return listview;
	}

	std::shared_ptr<TreeView> Window::CreateTreeView(UINT control_id, int x, int y, int width, int height)
	{
		HWND treeview_handle = ::CreateWindowEx(0, WC_TREEVIEW, _T(""), TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!treeview_handle)
			return nullptr;

		auto treeview = std::make_shared<TreeView>(control_id, m_hWnd);
		if (!treeview) {
			::DestroyWindow(treeview_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, treeview);
		return treeview;
	}

	std::shared_ptr<TabControl> Window::CreateTabControl(UINT control_id, int x, int y, int width, int height)
	{
		HWND tabcontrol_handle = ::CreateWindowEx(0, WC_TABCONTROL, _T(""), TCS_MULTILINE | TCS_BUTTONS | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!tabcontrol_handle)
			return nullptr;

		auto tabcontrol = std::make_shared<TabControl>(control_id, m_hWnd);
		if (!tabcontrol) {
			::DestroyWindow(tabcontrol_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, tabcontrol);
		return tabcontrol;
	}

	std::shared_ptr<ProgressBar> Window::CreateProgressBar(UINT control_id, int x, int y, int width, int height)
	{
		HWND progressbar_handle = ::CreateWindowEx(0, PROGRESS_CLASS, _T(""), PBS_SMOOTH | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!progressbar_handle)
			return nullptr;

		auto progressbar = std::make_shared<ProgressBar>(control_id, m_hWnd);
		if (!progressbar) {
			::DestroyWindow(progressbar_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, progressbar);
		return progressbar;
	}

	std::shared_ptr<SpinControl> Window::CreateSpinControl(UINT control_id, int x, int y, int width, int height)
	{
		HWND spincontrol_handle = ::CreateWindowEx(0, UPDOWN_CLASS, _T(""), UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!spincontrol_handle)
			return nullptr;

		auto spincontrol = std::make_shared<SpinControl>(control_id, m_hWnd);
		if (!spincontrol) {
			::DestroyWindow(spincontrol_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, spincontrol);
		return spincontrol;
	}

	std::shared_ptr<RichEdit> Window::CreateRichEdit(UINT control_id, int x, int y, int width, int height, LPCTSTR initial_text)
	{
		HWND richedit_handle = ::CreateWindowEx(0, RICHEDIT_CLASS, initial_text, ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_CHILD | WS_BORDER | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!richedit_handle)
			return nullptr;

		auto richedit = std::make_shared<RichEdit>(control_id, m_hWnd);
		if (!richedit) {
			::DestroyWindow(richedit_handle);
			return nullptr;
		}

		m_MappedControls.emplace(control_id, richedit);
		return richedit;
	}

	std::shared_ptr<LinkControl> Window::CreateLinkControl(UINT control_id, LPCTSTR text, int x, int y, int width, int height)
	{
		HWND linkcontrol_handle = ::CreateWindowEx(0, WC_LINK, text, WS_CHILD | WS_VISIBLE, x, y, width, height, m_hWnd, (HMENU)control_id, m_WindowClass.instance(), NULL);
		if (!linkcontrol_handle)
			return nullptr;

		auto linkcontrol = std::make_shared<LinkControl>(control_id, m_hWnd);
		if (!linkcontrol) {
			::DestroyWindow(linkcontrol_handle);
			return nullptr;
		}

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
		DeleteObject(m_Font);
		m_Font = NULL;
		for (auto& timer : m_TimerEvents)
			::KillTimer(m_hWnd, timer.first);
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
		for (auto& control : m_MappedControls)
			control.second.reset();

		::PostQuitMessage(exit_code);
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
