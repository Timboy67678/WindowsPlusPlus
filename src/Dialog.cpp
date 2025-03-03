#include "../Dialog.hpp"
#include "../Thunk.hpp"

namespace WPP
{
	Dialog::Dialog(HINSTANCE instance, int resource_id, int menu_id)
		: m_MainInstance(instance), Hwnd(resource_id, NULL), m_InternalTimerID(0), m_MenuID(menu_id), m_Menu(NULL)
	{
		m_MessageEvents = {
			{WM_INITDIALOG, &Dialog::OnInitDialog},
			{WM_CLOSE, &Dialog::OnClose},
			{WM_TIMER, &Dialog::OnTimer},
			{WM_NOTIFY, &Dialog::OnNotify},
			{WM_COMMAND, &Dialog::OnCommand},
			{WM_DESTROY, &Dialog::OnDestroy},
			{WM_DISPLAYCHANGE, &Dialog::OnDisplayChange},
			{WM_MOVE, &Dialog::OnMove},
			{WM_MENUCOMMAND, &Dialog::OnMenuCommand},
			{WM_PAINT, &Dialog::OnPaint},
			{WM_SIZE, &Dialog::OnSize},
			{WM_KEYDOWN, &Dialog::OnKeyDown},
			{WM_KEYUP, &Dialog::OnKeyUp},
			{WM_HSCROLL, &Dialog::OnHScroll},
			{WM_VSCROLL, &Dialog::OnVScroll},
			{WM_DROPFILES, &Dialog::OnDropFiles}
		};
	}

	Dialog::Dialog(HWND hWnd)
		: Hwnd(hWnd), m_InternalTimerID(0), m_MenuID(-1), m_Menu(NULL)
	{
		m_MainInstance = (HINSTANCE) ::GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	}

	void Dialog::Show(INT show)
	{
		::ShowWindow(m_hWnd, show);
	}

	void Dialog::EndDialog()
	{
		for (auto& timer : m_TimerEvents)
			::KillTimer(m_hWnd, timer.first);

		for (auto& control_pair : m_Controls)
			control_pair.reset();

		::DestroyMenu(m_Menu);

		::EndDialog(m_hWnd, 0);
	}

	INT_PTR CALLBACK Dialog::RunDlg(HWND parent, LPVOID param)
	{
		m_Parent = parent;
		Win32Thunk<DLGPROC, Dialog> thunk(&Dialog::DialogProc, this);
		return ::DialogBoxParam(m_MainInstance, MAKEINTRESOURCE(m_ItemID), parent, thunk.GetThunk(), (LPARAM) param);
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
				control->OnNotifyCallback(hWnd, nm);
		return TRUE;
	}

	INT_PTR CALLBACK Dialog::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		UINT commandID = LOWORD(wParam);
		UINT notificationCode = HIWORD(wParam);

		// Check if the command is a menu command
		if (notificationCode == 0 && m_MenuCommandEvents.count(commandID) != 0) {
			m_MenuCommandEvents[commandID](hWnd, wParam, lParam);
			return TRUE;
		}

		// Check if the command is from a control
		auto control = GetControl(commandID);
		if (control) {
			control->OnCommandCallback(hWnd, wParam, lParam);
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
			return (this->*it->second)(hWnd, wParam, lParam);
		return FALSE;
	}
#pragma endregion
}
