#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "winplusplus.h"

#define MESSAGE_HANDLER( X ) virtual INT_PTR CALLBACK X ( HWND hWnd, WPARAM wParam, LPARAM lParam )

#define MESSAGE_ONCREATE( ) virtual INT_PTR CALLBACK OnCreate( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONCLOSE( ) virtual INT_PTR CALLBACK OnClose( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONCOMMAND( ) virtual INT_PTR CALLBACK OnCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONPAINT( ) virtual INT_PTR CALLBACK OnPaint( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONSIZE( ) virtual INT_PTR CALLBACK OnSize( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONKEYDOWN( ) virtual INT_PTR CALLBACK OnKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONKEYUP( ) virtual INT_PTR CALLBACK OnKeyUp( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONNOTIFY( ) virtual INT_PTR CALLBACK OnNotify( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONHSCROLL( ) virtual INT_PTR CALLBACK OnHScroll( HWND hWnd, WPARAM wParam, LPARAM lParam )
#define MESSAGE_ONVSCROLL( ) virtual INT_PTR CALLBACK OnVScroll( HWND hWnd, WPARAM wParam, LPARAM lParam )

#define MESSAGE_REF( X ) static_cast<WPP::Dialog::DIALOG_MESSAGE_CALLBACK>( X )
#define TIMER_REF( X ) static_cast<WPP::Dialog::TIMER_CALLBACK>( X )

#define TIMER_OFFSET_START 0x1374

namespace WPP
{
	class Dialog : public Base
	{
	public:
		typedef INT_PTR( CALLBACK Dialog::*DIALOG_MESSAGE_CALLBACK )( HWND hWnd, WPARAM wParam, LPARAM lParam );
		typedef void ( CALLBACK Dialog::*TIMER_CALLBACK )( );

		Dialog( int resource_id );
		virtual ~Dialog( );
		
		MESSAGE_ONCREATE( );
		MESSAGE_ONCLOSE( );
		MESSAGE_ONCOMMAND( );
		MESSAGE_ONPAINT( );
		MESSAGE_ONSIZE( );
		MESSAGE_ONKEYDOWN( );
		MESSAGE_ONKEYUP( );
		MESSAGE_ONNOTIFY( );
		MESSAGE_ONHSCROLL( );
		MESSAGE_ONVSCROLL( );

		BOOL CenterWindow( HWND hWndCenter = NULL );

		virtual INT_PTR RunDlg( HWND parent = NULL, LPVOID param = NULL );
		virtual INT_PTR DialogProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

		void Show( int show = SW_NORMAL );
		void Close( );
		
		template < typename DC > 
		void AddTimer( UINT timer_elapse, DC callback )
		{
			const UINT_PTR timer_id = ++m_InternalTimerID + TIMER_OFFSET_START;
			if ( ::SetTimer( m_hWnd, timer_id, timer_elapse, NULL ) != 0 )
				m_TimerEvents[ timer_id ] = TIMER_REF( callback );
		}

		template < typename DC > 
		void AddCommandEvent( WORD id, DC pfn )
		{
			m_CommandEvents[ id ] = COMMAND_REF( pfn ); 
		}

		virtual int MsgBox( LPCTSTR message, LPCTSTR title, UINT type )
		{ return ::MessageBox( m_hWnd, message, title, type ); }

		virtual int MsgBoxInfo( LPCTSTR message, LPCTSTR title )
		{ return MsgBox( message, title, MB_OK | MB_ICONINFORMATION ); }

		virtual int MsgBoxError( LPCTSTR message, LPCTSTR title )
		{ return MsgBox( message, title, MB_OK | MB_ICONERROR ); }

		virtual int MsgBoxWarn( LPCTSTR message, LPCTSTR title )
		{ return MsgBox( message, title, MB_OK | MB_ICONWARNING ); }

	protected:
		std::map< UINT, DIALOG_MESSAGE_CALLBACK > m_MessageEvents;
		std::map< WORD, COMMAND_MESSAGE_CALLBACK > m_CommandEvents;
		std::map< UINT_PTR, TIMER_CALLBACK > m_TimerEvents;
		UINT_PTR m_InternalTimerID;
	};
}

#endif //__DIALOG_H__
