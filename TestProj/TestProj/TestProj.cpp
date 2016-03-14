#include "stdafx.h"
#include "TestProj.h"

#define MAX_RANGES 0, 100

class MainDialog : public Dialog
{
public:
	MainDialog( ) : Dialog( IDD_MAINDLG )
	{
		Dialog::AddCommandEvent( IDC_OK_BTN, &MainDialog::OnOK );
		Dialog::AddCommandEvent( IDC_TEST_CHECK, &MainDialog::OnCheckBoxClicked );
	}
	
	COMMAND_HANDLER( OnOK )
	{
		m_list->ResetContent( );
		m_combo->ResetContent( );
		m_tree->DeleteAllItems( );

		if ( m_tab->GetItemCount( ) < 5 )
			m_tab->AddItem( ( TEXT( "Tab " ) + std::to_tstring( m_tab->GetItemCount( ) + 1 ) ).c_str( ) );

		m_list->AddDir( 0, TEXT( "C:\\Windows\\System32\\*.exe" ) );
		m_combo->AddDir( 0, TEXT( "C:\\Windows\\System32\\*.exe" ) );

		m_combo->SetMinVisible( 8 );

		HTREEITEM trees[ 8 ] = { 0 };
		HTREEITEM tree = m_tree->InsertItem( TEXT( "Main Item" ), NULL, NULL );
		
		for ( int i = 0; i < ARRAYSIZE( trees ); i++ )
			trees[ i ] = m_tree->InsertItem( TEXT( "Sub Item" ), i == 0 ? tree : trees[ i - 1 ], NULL );
		
		for ( int i = ARRAYSIZE( trees ) - 1; i >= 0; i-- )
			m_tree->Expand( trees[ i ] );

		m_tree->Expand( tree );

		return FALSE;
	}

	COMMAND_HANDLER( OnCheckBoxClicked )
	{
		m_dostuff->SetShield( shield_state = !shield_state );
		return FALSE;
	}

	MESSAGE_ONNOTIFY( )
	{
		auto nm = reinterpret_cast<NMHDR *>( lParam );

		switch ( nm->idFrom )
		{
		case IDC_SPIN_TEST:
			{
				if ( nm->code == UDN_DELTAPOS )
				{
					auto updn = reinterpret_cast<NMUPDOWN *>( nm );

					int minimum, maximum, new_val = updn->iPos + updn->iDelta;
					m_spin->GetRange32( minimum, maximum );
					
					if ( new_val >= minimum && new_val <= maximum )
					{
						m_progress->SetPos( new_val );
						m_track->SetPos( new_val );
						m_scroll->SetScrollPos( new_val );
						m_spinedit->SetText( std::to_string( new_val ) );
						m_richedit->AppendText( ( TEXT( "Delta is " ) + std::to_tstring( updn->iDelta ) + TEXT( "\n" ) ).c_str( ) );
					}
				}
				break;
			}
		case IDC_SLIDER_TEST:
			{
				if ( nm->code == TRBN_THUMBPOSCHANGING )
				{
					auto tbm = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>( nm );

					int minimum, maximum, new_val = tbm->dwPos;
					m_track->GetRange( minimum, maximum );
					
					if ( new_val >= minimum && new_val <= maximum )
					{
						m_progress->SetPos( new_val );
						m_spin->SetPos( new_val );
						m_scroll->SetScrollPos( new_val );
						m_spinedit->SetText( std::to_string( new_val ) );
					}
				}
				break;
			}
		}
		return Dialog::OnNotify( hWnd, Msg, wParam, lParam );
	}

	MESSAGE_ONCREATE( )
	{
		m_check = new CheckBox( IDC_TEST_CHECK, hWnd );
		m_list = new ListBox( IDC_LISTBOX_TEST, hWnd );
		m_dostuff = new Button( IDC_OK_BTN, hWnd );
		m_combo = new ComboBox( IDC_COMB_TEST, hWnd );
		m_richedit = new RichEdit( IDC_RICHEDIT_TEST, hWnd );
		m_spinedit = new EditText( IDC_EDIT_SPIN, hWnd );
		m_scroll = new ScrollBar( IDC_SCROLLBAR_TEST, hWnd );
		m_view = new ListView( IDC_LISTVIEW_TEST, hWnd );
		m_tree = new TreeView( IDC_TREE_TEST, hWnd );
		m_track = new TrackBar( IDC_SLIDER_TEST, hWnd );
		m_spin = new SpinControl( IDC_SPIN_TEST, hWnd );
		m_progress = new ProgressBar( IDC_PROGRESS_TEST, hWnd );
		m_tab = new TabControl( IDC_TAB_TEST, hWnd );

		m_combo->SetCueBannerText( L"ComboBox control item test" );

		if ( !( m_track->GetStyle( ) & TBS_NOTIFYBEFOREMOVE ) )
			m_track->AddStyle( TBS_NOTIFYBEFOREMOVE );
		
		m_spin->SetBuddy( m_spinedit->GetHandle( ) );
		m_spinedit->SetTextLimit( 4 );

		int nMin, nMax;
		
		m_progress->SetRange32( MAX_RANGES );
		m_progress->GetRange( nMin, nMax );

		m_track->SetRange( nMin, nMax );
		m_spin->SetRange( nMin, nMax );
		m_scroll->SetScrollRange( nMin, nMax );

		return Dialog::OnCreate( hWnd, Msg, wParam, lParam );
	}

private:
	BOOL shield_state = FALSE;

	ComboBox *m_combo = nullptr;
	Button *m_dostuff = nullptr;
	CheckBox *m_check = nullptr;
	ListBox *m_list = nullptr;
	RichEdit *m_richedit = nullptr;
	EditText *m_spinedit = nullptr;
	ScrollBar *m_scroll = nullptr;
	ListView *m_view = nullptr;
	TreeView *m_tree = nullptr;
	TrackBar *m_track = nullptr;
	SpinControl *m_spin = nullptr;
	ProgressBar *m_progress = nullptr;
	TabControl *m_tab = nullptr;
};

INT APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow )
{
	InitCommonControls( );
	HMODULE hRichEd = LoadLibrary( TEXT( "riched20.dll" ) );
	( new MainDialog( ) )->RunDlg( );
	FreeLibrary( hRichEd );
}
