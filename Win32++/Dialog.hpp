#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "winplusplus.h"

#define MESSAGE_HANDLER( X ) virtual INT_PTR CALLBACK X ( HWND hWnd, WPARAM wParam, LPARAM lParam )

#define MESSAGE_ONCREATE( ) MESSAGE_HANDLER( OnCreate )
#define MESSAGE_ONCLOSE( ) MESSAGE_HANDLER( OnClose )
#define MESSAGE_ONCOMMAND( ) MESSAGE_HANDLER( OnCommand )
#define MESSAGE_ONPAINT( ) MESSAGE_HANDLER( OnPaint )
#define MESSAGE_ONTIMER( ) MESSAGE_HANDLER( OnTimer )
#define MESSAGE_ONSIZE( ) MESSAGE_HANDLER( OnSize )
#define MESSAGE_ONKEYDOWN( ) MESSAGE_HANDLER( OnKeyDown )
#define MESSAGE_ONKEYUP( ) MESSAGE_HANDLER( OnKeyUp )
#define MESSAGE_ONNOTIFY( ) MESSAGE_HANDLER( OnNotify )
#define MESSAGE_ONHSCROLL( ) MESSAGE_HANDLER( OnHScroll )
#define MESSAGE_ONVSCROLL( ) MESSAGE_HANDLER( OnVScroll )

#define MESSAGE_REF( X ) static_cast<WPP::Dialog::DIALOG_MESSAGE_CALLBACK>( X )

namespace WPP
{
	class Dialog : public Base
	{
	public:
		typedef INT_PTR( CALLBACK Dialog::*DIALOG_MESSAGE_CALLBACK )( HWND hWnd, WPARAM wParam, LPARAM lParam );

		Dialog( int resource_id );
		virtual ~Dialog( ) { Close( ); }
		
		MESSAGE_ONCREATE( );
		MESSAGE_ONCLOSE( );
		MESSAGE_ONCOMMAND( );
		MESSAGE_ONPAINT( );
		MESSAGE_ONTIMER( );
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
	};
}

#endif //__DIALOG_H__
