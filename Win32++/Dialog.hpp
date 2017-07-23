#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "winplusplus.h"

#define TIMER_HANDLER(X) virtual void CALLBACK X()
#define MESSAGE_HANDLER( X ) virtual INT_PTR CALLBACK X(HWND hWnd, WPARAM wParam, LPARAM lParam)

#define MESSAGE_ONINITDIALOG() virtual INT_PTR CALLBACK OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONCLOSE() virtual INT_PTR CALLBACK OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONMENUCOMMAND() virtual INT_PTR CALLBACK OnMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONCOMMAND() virtual INT_PTR CALLBACK OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONPAINT() virtual INT_PTR CALLBACK OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONTIMER() virtual INT_PTR CALLBACK OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONSIZE() virtual INT_PTR CALLBACK OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONKEYDOWN() virtual INT_PTR CALLBACK OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONKEYUP() virtual INT_PTR CALLBACK OnKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONNOTIFY() virtual INT_PTR CALLBACK OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONHSCROLL() virtual INT_PTR CALLBACK OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONVSCROLL() virtual INT_PTR CALLBACK OnVScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
#define MESSAGE_ONDROPFILES() virtual INT_PTR CALLBACK OnDropFiles(HWND hWnd, WPARAM wParam, LPARAM lParam)

#define MESSAGE_REF(X) static_cast<WPP::Dialog::DIALOG_MESSAGE_CALLBACK>(X)
#define TIMER_REF(X) static_cast<WPP::Dialog::TIMER_CALLBACK>(X)

#define TIMER_OFFSET_START 0x1374

namespace WPP
{
	class Dialog : public Wnd
	{
	public:
		typedef INT_PTR(CALLBACK Dialog::*DIALOG_MESSAGE_CALLBACK)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		typedef void (CALLBACK Dialog::*TIMER_CALLBACK)();

		Dialog(int resource_id, int menu_id = -1);
		virtual ~Dialog();

		MESSAGE_ONINITDIALOG();
		MESSAGE_ONCLOSE();
		MESSAGE_ONMENUCOMMAND();
		MESSAGE_ONCOMMAND();
		MESSAGE_ONPAINT();
		MESSAGE_ONTIMER();
		MESSAGE_ONSIZE();
		MESSAGE_ONKEYDOWN();
		MESSAGE_ONKEYUP();
		MESSAGE_ONNOTIFY();
		MESSAGE_ONHSCROLL();
		MESSAGE_ONVSCROLL();
		MESSAGE_ONDROPFILES();

		BOOL CenterWindow(HWND hWndCenter = NULL);

		virtual INT_PTR CALLBACK RunDlg(HWND parent = NULL, LPVOID param = NULL);
		virtual INT_PTR DialogProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		void Show(INT show = SW_NORMAL);
		void EndDialog();
		void EnableDragDrop(BOOL state = TRUE);

		template < typename DC >
		void AddTimer(UINT timer_elapse, DC callback)
		{
			const UINT_PTR timer_id = ++m_InternalTimerID + TIMER_OFFSET_START;
			if (::SetTimer(m_hWnd, timer_id, timer_elapse, NULL) != 0)
				m_TimerEvents[timer_id] = TIMER_REF(callback);
		}

		template < typename DC >
		void AddCommandEvent(WORD id, DC pfn)
		{
			m_CommandEvents[id] = COMMAND_REF(pfn);
		}

		template <typename CtrlType = Control>
		bool RegisterControl(UINT control_id, CtrlType** control_out = nullptr)
		{
			CtrlType* ctrl = new (std::nothrow) CtrlType(control_id, m_hWnd);

			if (ctrl != nullptr)
				m_MappedControls.emplace(control_id, static_cast<Control*>(ctrl));

			if (control_out != nullptr)
				*control_out = ctrl;

			return ctrl != nullptr;
		}

		template< typename CtrlType = Control>
		CtrlType* GetControl(UINT control_id)
		{
			return static_cast<CtrlType*>(m_MappedControls[control_id]);
		}

		virtual int MsgBox(LPCTSTR message, LPCTSTR title, UINT type)
		{
			return ::MessageBox(m_hWnd, message, title, type);
		}

		virtual int MsgBoxInfo(LPCTSTR title, LPCTSTR fmt, ...)
		{
			TCHAR buffer[1024 * 6] = {0};
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

		virtual int MsgBoxError(LPCTSTR title, LPCTSTR fmt, ...)
		{
			TCHAR buffer[1024 * 6] = {0};
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

		virtual int MsgBoxWarn(LPCTSTR title, LPCTSTR fmt, ...)
		{
			TCHAR buffer[1024 * 6] = {0};
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
		std::map< UINT, DIALOG_MESSAGE_CALLBACK > m_MessageEvents;
		std::map< WORD, COMMAND_MESSAGE_CALLBACK > m_CommandEvents;
		std::map< UINT_PTR, TIMER_CALLBACK > m_TimerEvents;
		std::map< UINT, Control* > m_MappedControls;

		UINT_PTR m_InternalTimerID;

		int m_MenuID;

		HMENU m_Menu;
	};
}

#endif //__DIALOG_H__
