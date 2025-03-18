#include "..\Window.hpp"
#include "..\Thunk.hpp"

namespace WPP
{
	Window::Window(Class wnd_class, LPCTSTR window_name, int x_pos, int y_pos, int width, int height, DWORD style,
				   int menu_id, HMENU menu, HFONT font, DWORD style_ex)
		: Hwnd(NULL), m_WindowClass(wnd_class), m_WindowName(window_name), m_XPos(x_pos), m_YPos(y_pos),
		m_Width(width), m_Height(height), m_Style(style), m_MenuID(menu_id), m_Menu(menu), m_Font(font), m_StyleEx(style_ex)
	{
		InitializeMessageEvents();
	}

	Window::~Window() {
		CleanupResources();
	}

	void Window::InitializeMessageEvents()
	{
		using namespace std::placeholders;

		m_MessageEvents = {
			{WM_CREATE, std::bind(&Window::OnCreate, this, _1, _2, _3)},
			{WM_CLOSE, std::bind(&Window::OnClose, this, _1, _2, _3)},
			{WM_DESTROY, std::bind(&Window::OnDestroy, this, _1, _2, _3)},
			{WM_DISPLAYCHANGE, std::bind(&Window::OnDisplayChange, this, _1, _2, _3)},
			{WM_MOVE, std::bind(&Window::OnMove, this, _1, _2, _3)},
			{WM_COMMAND, std::bind(&Window::OnCommand, this, _1, _2, _3)},
			{WM_MENUCOMMAND, std::bind(&Window::OnMenuCommand, this, _1, _2, _3)},
			{WM_PAINT, std::bind(&Window::OnPaint, this, _1, _2, _3)},
			{WM_TIMER, std::bind(&Window::OnTimer, this, _1, _2, _3)},
			{WM_SIZE, std::bind(&Window::OnSize, this, _1, _2, _3)},
			{WM_KEYDOWN, std::bind(&Window::OnKeyDown, this, _1, _2, _3)},
			{WM_KEYUP, std::bind(&Window::OnKeyUp, this, _1, _2, _3)},
			{WM_NOTIFY, std::bind(&Window::OnNotify, this, _1, _2, _3)},
			{WM_HSCROLL, std::bind(&Window::OnHScroll, this, _1, _2, _3)},
			{WM_VSCROLL, std::bind(&Window::OnVScroll, this, _1, _2, _3)},
			{WM_DROPFILES, std::bind(&Window::OnDropFiles, this, _1, _2, _3)}
		};
	}

	void Window::CleanupResources()
	{
		for (auto& timer : m_TimerEvents)
			::KillTimer(m_hWnd, timer.first);
		for (auto& control_pair : m_Controls) {
			::DestroyWindow(control_pair->GetHandle());
			control_pair.reset();
		}

		::DeleteObject(m_Font);
		m_Font = NULL;

		::DestroyMenu(m_Menu);
		::DestroyWindow(m_hWnd);
	}

	bool Window::RunWindow(HWND parent_window, LPVOID param)
	{
		m_Parent = parent_window;

		if (m_WindowClass.atom() != NULL)
			m_WindowClass.Unregister();

		Win32Thunk<WNDPROC, Window> thunk{ &Window::WindowProc, this };
		m_WindowClass.get().lpfnWndProc = thunk.GetThunk();
		m_WindowClass.Register();

		m_hWnd = ::CreateWindowEx(m_StyleEx, m_WindowClass.class_name(), m_WindowName.c_str(), m_Style,
								  m_XPos, m_YPos, m_Width, m_Height, m_Parent, m_Menu, m_WindowClass.instance(), param);

		if (!m_hWnd)
			return false;

		Show(SW_SHOWNORMAL);

		if (m_Font == NULL)
			m_Font = ::CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
								  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("MS Shell Dlg"));

		SetFont(m_Font);

		for (auto& control : m_Controls)
			control->SetFont(m_Font);

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
			ret = it->second(hWnd, wParam, lParam);

		if (ret == FALSE) //was handled? otherwise send to default os handler
			return ::DefWindowProc(hWnd, Msg, wParam, lParam);
		else
			return ret;
	}

#pragma region Window Control Creators
#pragma warning(push)
#pragma warning(disable: 4312)
	std::shared_ptr<RadioButton> Window::RadioButtonGroup::CreateButton(LPCTSTR text, int x, int y, int width, int height, BOOL initial_state)
	{
		auto control_id = m_Parent->m_ControlID++;

		DWORD style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED;
		if (m_RadioButtons.empty())
			style |= WS_GROUP;

		HWND radiobutton_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_AUTORADIOBUTTON | style,
												   x, y, width, height, m_Parent->m_hWnd, reinterpret_cast<HMENU>(control_id), m_Parent->m_WindowClass.instance(), NULL);
		if (!radiobutton_handle)
			return nullptr;

		auto radiobutton = std::make_shared<RadioButton>(control_id, m_Parent->m_hWnd);
		if (!radiobutton) {
			::DestroyWindow(radiobutton_handle);
			return nullptr;
		}

		radiobutton->SetChecked(initial_state ? BST_CHECKED : BST_UNCHECKED);
		m_RadioButtons.push_back(radiobutton);
		m_Parent->m_Controls.emplace_back(radiobutton);

		return radiobutton;
	}

	int Window::RadioButtonGroup::SelectedIndex()
	{
		for (size_t index = 0; index < m_RadioButtons.size(); ++index) {
			if (m_RadioButtons[index]->GetChecked() == BST_CHECKED)
				return static_cast<int>(index);
		}
		return -1;
	}

	std::shared_ptr<Window::RadioButtonGroup> Window::CreateRadioButtonGroup()
	{
		return std::make_shared<RadioButtonGroup>(this);
	}

	std::shared_ptr<Button> Window::CreateButton(LPCTSTR text, int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND button_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
											  x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!button_handle)
			return nullptr;

		auto button = std::make_shared<Button>(control_id, m_hWnd);
		if (!button) {
			::DestroyWindow(button_handle);
			return nullptr;
		}

		m_Controls.emplace_back(button);
		return button;
	}

	std::shared_ptr<CheckBox> Window::CreateCheckBox(LPCTSTR text, int x, int y, int width, int height, BOOL initial_state)
	{
		auto control_id = m_ControlID++;

		HWND checkbox_handle = ::CreateWindowEx(0, WC_BUTTON, text, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
												x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!checkbox_handle)
			return nullptr;

		auto checkbox = std::make_shared<CheckBox>(control_id, m_hWnd);
		if (!checkbox) {
			::DestroyWindow(checkbox_handle);
			return nullptr;
		}

		checkbox->SetChecked(initial_state ? BST_CHECKED : BST_UNCHECKED);
		m_Controls.emplace_back(checkbox);
		return checkbox;
	}

	std::shared_ptr<ComboBox> Window::CreateComboBox(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND combobox_handle = ::CreateWindowEx(0, WC_COMBOBOX, _T(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
												x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!combobox_handle)
			return nullptr;

		auto combobox = std::make_shared<ComboBox>(control_id, m_hWnd);
		if (!combobox) {
			::DestroyWindow(combobox_handle);
			return nullptr;
		}

		m_Controls.emplace_back(combobox);
		return combobox;
	}

	std::shared_ptr<EditText> Window::CreateEditText(int x, int y, int width, int height, LPCTSTR initial_text)
	{
		auto control_id = m_ControlID++;

		HWND edittext_handle = ::CreateWindowEx(0, WC_EDIT, initial_text, ES_LEFT | WS_CHILD | WS_BORDER | WS_VISIBLE,
												x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!edittext_handle)
			return nullptr;

		auto edittext = std::make_shared<EditText>(control_id, m_hWnd);
		if (!edittext) {
			::DestroyWindow(edittext_handle);
			return nullptr;
		}

		m_Controls.emplace_back(edittext);
		return edittext;
	}

	std::shared_ptr<ListBox> Window::CreateListBox(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND listbox_handle = ::CreateWindowEx(0, WC_LISTBOX, _T(""), LBS_STANDARD | WS_CHILD | WS_BORDER | WS_VISIBLE,
											   x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!listbox_handle)
			return nullptr;

		auto listbox = std::make_shared<ListBox>(control_id, m_hWnd);
		if (!listbox) {
			::DestroyWindow(listbox_handle);
			return nullptr;
		}

		m_Controls.emplace_back(listbox);
		return listbox;
	}

	std::shared_ptr<ListView> Window::CreateListView(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND listview_handle = ::CreateWindowEx(0, WC_LISTVIEW, _T(""), LVS_REPORT | WS_CHILD | WS_BORDER | WS_VISIBLE,
												x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!listview_handle)
			return nullptr;

		auto listview = std::make_shared<ListView>(control_id, m_hWnd);
		if (!listview) {
			::DestroyWindow(listview_handle);
			return nullptr;
		}

		m_Controls.emplace_back(listview);
		return listview;
	}

	std::shared_ptr<TreeView> Window::CreateTreeView(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND treeview_handle = ::CreateWindowEx(0, WC_TREEVIEW, _T(""), TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CHILD | WS_BORDER | WS_VISIBLE,
												x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!treeview_handle)
			return nullptr;

		auto treeview = std::make_shared<TreeView>(control_id, m_hWnd);
		if (!treeview) {
			::DestroyWindow(treeview_handle);
			return nullptr;
		}

		m_Controls.emplace_back(treeview);
		return treeview;
	}

	std::shared_ptr<TabControl> Window::CreateTabControl(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND tabcontrol_handle = ::CreateWindowEx(0, WC_TABCONTROL, _T(""), TCS_MULTILINE | TCS_BUTTONS | WS_CHILD | WS_BORDER | WS_VISIBLE,
												  x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!tabcontrol_handle)
			return nullptr;

		auto tabcontrol = std::make_shared<TabControl>(control_id, m_hWnd);
		if (!tabcontrol) {
			::DestroyWindow(tabcontrol_handle);
			return nullptr;
		}

		m_Controls.emplace_back(tabcontrol);
		return tabcontrol;
	}

	std::shared_ptr<ProgressBar> Window::CreateProgressBar(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND progressbar_handle = ::CreateWindowEx(0, PROGRESS_CLASS, _T(""), PBS_SMOOTH | WS_CHILD | WS_BORDER | WS_VISIBLE,
												   x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!progressbar_handle)
			return nullptr;

		auto progressbar = std::make_shared<ProgressBar>(control_id, m_hWnd);
		if (!progressbar) {
			::DestroyWindow(progressbar_handle);
			return nullptr;
		}

		m_Controls.emplace_back(progressbar);
		return progressbar;
	}

	std::shared_ptr<UpDownControl> Window::CreateSpinControl(int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND spincontrol_handle = ::CreateWindowEx(0, UPDOWN_CLASS, _T(""), UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS | WS_CHILD | WS_BORDER | WS_VISIBLE,
												   x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!spincontrol_handle)
			return nullptr;

		auto spincontrol = std::make_shared<UpDownControl>(control_id, m_hWnd);
		if (!spincontrol) {
			::DestroyWindow(spincontrol_handle);
			return nullptr;
		}

		m_Controls.emplace_back(spincontrol);
		return spincontrol;
	}

	std::shared_ptr<RichEdit> Window::CreateRichEdit(int x, int y, int width, int height, LPCTSTR initial_text)
	{
		auto control_id = m_ControlID++;

		if (::GetModuleHandle(TEXT("Riched20.dll")) == NULL) {
			if (::LoadLibrary(TEXT("Riched20.dll")) == NULL) {
				return nullptr;
			}
		}

		HWND richedit_handle = ::CreateWindowEx(0, RICHEDIT_CLASS, initial_text, ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_CHILD | WS_BORDER | WS_VISIBLE,
												x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!richedit_handle) {
			return nullptr;
		}

		auto richedit = std::make_shared<RichEdit>(control_id, m_hWnd);
		if (!richedit) {
			::DestroyWindow(richedit_handle);
			return nullptr;
		}

		m_Controls.emplace_back(richedit);
		return richedit;
	}

	std::shared_ptr<SysLink> Window::CreateLinkControl(LPCTSTR text, int x, int y, int width, int height)
	{
		auto control_id = m_ControlID++;

		HWND linkcontrol_handle = ::CreateWindowEx(0, WC_LINK, text, WS_CHILD | WS_VISIBLE,
												   x, y, width, height, m_hWnd, reinterpret_cast<HMENU>(control_id), m_WindowClass.instance(), NULL);
		if (!linkcontrol_handle)
			return nullptr;

		auto linkcontrol = std::make_shared<SysLink>(control_id, m_hWnd);
		if (!linkcontrol) {
			::DestroyWindow(linkcontrol_handle);
			return nullptr;
		}

		m_Controls.emplace_back(linkcontrol);
		return linkcontrol;
	}
#pragma warning(pop)
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
			it->second();
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
		auto nm = reinterpret_cast<LPNMHDR>(lParam);
		for (auto& control : m_Controls)
			if (control && nm->idFrom == control->GetID())
				control->OnNotifyCallback(nm);
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
		UINT commandID = LOWORD(wParam);
		UINT notificationCode = HIWORD(wParam);

		// Check if the command is a menu command
		if (notificationCode == 0 && m_MenuCommandEvents.count(commandID) != 0) {
			m_MenuCommandEvents[commandID](wParam, lParam);
			return TRUE;
		}

		// Check if the command is from a control
		auto control = GetControl(commandID);
		if (control) {
			control->OnCommandCallback(wParam, lParam);
			return TRUE;
		}

		return FALSE;
	}
#pragma endregion

	void Window::Show(INT show)
	{
		::ShowWindow(m_hWnd, show);
	}

	void Window::CloseWindow()
	{
		QuitWindow();
	}

	void Window::QuitWindow(INT exit_code)
	{
		m_WindowRunning = false;	
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
