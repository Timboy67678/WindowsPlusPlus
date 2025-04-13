#include "../Dialog.hpp"
#include "../Thunk.hpp"

namespace WPP
{
	Dialog::Dialog(HINSTANCE instance, int resource_id, int menu_id)
		: m_MainInstance(instance), Hwnd(resource_id, NULL), m_InternalTimerID(0), m_MenuID(menu_id), m_Menu(NULL) {
		InitializeMessageEvents();
	}

	Dialog::Dialog(HWND hWnd)
		: Hwnd(hWnd), m_InternalTimerID(0), m_MenuID(-1), m_Menu(NULL) {
		m_MainInstance = (HINSTANCE)::GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		InitializeMessageEvents();
	}

	Dialog::~Dialog() {
		CleanupResources();
	}

	void Dialog::InitializeMessageEvents() 
	{
		using namespace std::placeholders;

		m_MessageEvents = {
			{WM_INITDIALOG, std::bind(&Dialog::OnInitDialog, this, _1, _2, _3)},
			{WM_CLOSE, std::bind(&Dialog::OnClose, this, _1, _2, _3)},
			{WM_QUIT, std::bind(&Dialog::OnQuit, this, _1, _2, _3)},
			{WM_DESTROY, std::bind(&Dialog::OnDestroy, this, _1, _2, _3)},
			{WM_DISPLAYCHANGE, std::bind(&Dialog::OnDisplayChange, this, _1, _2, _3)},
			{WM_MOVE, std::bind(&Dialog::OnMove, this, _1, _2, _3)},
			{WM_COMMAND, std::bind(&Dialog::OnCommand, this, _1, _2, _3)},
			{WM_MENUCOMMAND, std::bind(&Dialog::OnMenuCommand, this, _1, _2, _3)},
			{WM_PAINT, std::bind(&Dialog::OnPaint, this, _1, _2, _3)},
			{WM_TIMER, std::bind(&Dialog::OnTimer, this, _1, _2, _3)},
			{WM_SIZE, std::bind(&Dialog::OnSize, this, _1, _2, _3)},
			{WM_KEYDOWN, std::bind(&Dialog::OnKeyDown, this, _1, _2, _3)},
			{WM_KEYUP, std::bind(&Dialog::OnKeyUp, this, _1, _2, _3)},
			{WM_NOTIFY, std::bind(&Dialog::OnNotify, this, _1, _2, _3)},
			{WM_HSCROLL, std::bind(&Dialog::OnHScroll, this, _1, _2, _3)},
			{WM_VSCROLL, std::bind(&Dialog::OnVScroll, this, _1, _2, _3)},
			{WM_DROPFILES, std::bind(&Dialog::OnDropFiles, this, _1, _2, _3)}
		};
	}

	void Dialog::CleanupResources() {
		for (auto& timer : m_TimerEvents)
			::KillTimer(m_hWnd, timer.first);

		for (auto& control_pair : m_Controls)
			control_pair.reset();

		m_TimerEvents.clear();
		m_MenuCommandEvents.clear();
		m_Controls.clear();

		::DestroyMenu(m_Menu);
	}

	void Dialog::Show() {
		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	}

	void Dialog::Hide() {
		::ShowWindow(m_hWnd, SW_HIDE);
	}

	void Dialog::EndDialog() {
		CleanupResources();
		::EndDialog(m_hWnd, 0);
	}
	
	INT_PTR CALLBACK Dialog::RunDlg(HWND parent, LPVOID param) {
		m_Parent = parent;
		Win32Thunk<DLGPROC, Dialog> thunk{ &Dialog::DialogProc, this };
		return ::DialogBoxParam(m_MainInstance, MAKEINTRESOURCE(m_ItemID), parent, thunk.GetThunk(), (LPARAM)param);
	}

#pragma region Overidables
	INT_PTR CALLBACK Dialog::OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		if (m_MenuID != -1) 
			m_Menu = ::LoadMenu(m_MainInstance, MAKEINTRESOURCE(m_MenuID));
		return TRUE;
	}

	INT_PTR CALLBACK Dialog::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		EndDialog();
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnQuit(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		auto it = m_TimerEvents.find((UINT_PTR)wParam);
		if (it != m_TimerEvents.end())
			it->second();
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		auto nm = reinterpret_cast<LPNMHDR>(lParam);
		for (auto& control : m_Controls)
			if(control && nm->idFrom == control->GetID())
				control->OnNotifyCallback(nm);
		return TRUE;
	}

	INT_PTR CALLBACK Dialog::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		UINT commandID = LOWORD(wParam);
		UINT notificationCode = HIWORD(wParam);

		// Check if the command is a menu command
		if (notificationCode == 0) {
			auto menuCommandIt = m_MenuCommandEvents.find(commandID);
			if (menuCommandIt != m_MenuCommandEvents.end()) {
				menuCommandIt->second(wParam, lParam);
				return TRUE;
			}
		}

		// Check if the command is from a control
		auto control = GetControl(commandID);
		if (control) {
			control->OnCommandCallback(wParam, lParam);
			return TRUE;
		}

		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		EndDialog();
		return TRUE;
	}

	INT_PTR CALLBACK Dialog::OnDisplayChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnVScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}
	
	INT_PTR CALLBACK Dialog::OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	INT_PTR Dialog::DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		m_hWnd = hWnd;
		auto it = m_MessageEvents.find(Msg);
		if (it != m_MessageEvents.end())
			return it->second(hWnd, wParam, lParam);
		return FALSE;
	}
#pragma endregion
}
