#include "Dialog.hpp"

namespace WPP
{
	Dialog::Dialog( int resource_id )
		: Base( resource_id, NULL )
	{
		m_MessageEvents[ WM_INITDIALOG ] = &Dialog::OnCreate;
		m_MessageEvents[ WM_COMMAND ] = &Dialog::OnCommand;
		m_MessageEvents[ WM_CLOSE ] = &Dialog::OnClose;
		m_MessageEvents[ WM_PAINT ] = &Dialog::OnPaint;
		m_MessageEvents[ WM_TIMER ] = &Dialog::OnTimer;
		m_MessageEvents[ WM_SIZE ] = &Dialog::OnSize;
		m_MessageEvents[ WM_KEYDOWN ] = &Dialog::OnKeyDown;
		m_MessageEvents[ WM_KEYUP ] = &Dialog::OnKeyUp;
		m_MessageEvents[ WM_NOTIFY ] = &Dialog::OnNotify;
		m_MessageEvents[ WM_HSCROLL ] = &Dialog::OnHScroll;
		m_MessageEvents[ WM_VSCROLL ] = &Dialog::OnVScroll;
	}

	void Dialog::Show( int show )
	{
		ShowWindow( m_hWnd, show );
	}

	void Dialog::Close( )
	{
		EndDialog( m_hWnd, 0 );
	}

	INT_PTR Dialog::RunDlg( HWND parent, LPVOID param )
	{
		m_Parent = parent;
		Win32Thunk<DLGPROC, Dialog> thunk( &Dialog::DialogProc, this );
		return DialogBoxParam( NULL, MAKEINTRESOURCE( m_ItemID ), parent, thunk.GetThunk( ), (LPARAM) param );
	}

#pragma region Overidables
	INT_PTR CALLBACK Dialog::OnCreate( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		m_hWnd = hWnd;
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnClose( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		Close( );
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnPaint( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnTimer( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnSize( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnKeyDown( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnKeyUp( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnNotify( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnHScroll( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnVScroll( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		return FALSE;
	}

	INT_PTR CALLBACK Dialog::OnCommand( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		if ( m_CommandEvents.count( LOWORD( wParam ) ) > 0 )
			return ( this->*m_CommandEvents[ LOWORD( wParam ) ] )( hWnd, wParam, lParam );
		return FALSE;
	}

	INT_PTR Dialog::DialogProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
	{
		if ( m_MessageEvents.count( Msg ) > 0 )
			return ( this->*m_MessageEvents[ Msg ] )( hWnd, Msg, wParam, lParam );
		return FALSE;
	}
#pragma endregion

	//Functions From ATL
	BOOL Dialog::CenterWindow( HWND hWndCenter )
	{
		// determine owner window to center against
		DWORD dwStyle = GetStyle( );
		if ( hWndCenter == NULL )
		{
			if ( dwStyle & WS_CHILD )
				hWndCenter = ::GetParent( m_hWnd );
			else
				hWndCenter = ::GetWindow( m_hWnd, GW_OWNER );
		}

		// get coordinates of the window relative to its parent
		RECT rcDlg;
		::GetWindowRect( m_hWnd, &rcDlg );
		RECT rcArea;
		RECT rcCenter;
		HWND hWndParent;

		if ( !( dwStyle & WS_CHILD ) )
		{
			// don't center against invisible or minimized windows
			if ( hWndCenter != NULL )
			{
				DWORD dwStyleCenter = ::GetWindowLong( hWndCenter, GWL_STYLE );
				if ( !( dwStyleCenter & WS_VISIBLE ) || ( dwStyleCenter & WS_MINIMIZE ) )
					hWndCenter = NULL;
			}

			// center within screen coordinates
			HMONITOR hMonitor = NULL;
			if ( hWndCenter != NULL )
				hMonitor = ::MonitorFromWindow( hWndCenter, MONITOR_DEFAULTTONEAREST );
			else
				hMonitor = ::MonitorFromWindow( m_hWnd, MONITOR_DEFAULTTONEAREST );

			MONITORINFO minfo;
			minfo.cbSize = sizeof( MONITORINFO );
			BOOL bResult = ::GetMonitorInfo( hMonitor, &minfo );
			rcArea = minfo.rcWork;

			if ( hWndCenter == NULL )
				rcCenter = rcArea;
			else
				::GetWindowRect( hWndCenter, &rcCenter );
		} else {
			// center within parent client coordinates
			hWndParent = ::GetParent( m_hWnd );
			::GetClientRect( hWndParent, &rcArea );
			::GetClientRect( hWndCenter, &rcCenter );
			::MapWindowPoints( hWndCenter, hWndParent, (POINT*) &rcCenter, 2 );
		}

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// find dialog's upper left based on rcCenter
		int xLeft = ( rcCenter.left + rcCenter.right ) / 2 - DlgWidth / 2;
		int yTop = ( rcCenter.top + rcCenter.bottom ) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if ( xLeft + DlgWidth > rcArea.right )
			xLeft = rcArea.right - DlgWidth;
		if ( xLeft < rcArea.left )
			xLeft = rcArea.left;

		if ( yTop + DlgHeight > rcArea.bottom )
			yTop = rcArea.bottom - DlgHeight;
		if ( yTop < rcArea.top )
			yTop = rcArea.top;

		// map screen coordinates to child coordinates
		return ::SetWindowPos( m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}
}
