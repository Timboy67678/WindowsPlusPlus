#pragma once

#include "stdafx.h"

class MainDialog : public Dialog
{
public:
	MainDialog(HINSTANCE hInstance) : Dialog(hInstance, IDD_MAINDLG, IDC_TESTPROJ)
	{
		Dialog::AddCommandEvent(IDC_OK_BTN, &MainDialog::OnOK);
		Dialog::AddCommandEvent(IDC_TEST_CHECK, &MainDialog::OnCheckBoxClicked);
		Dialog::AddCommandEvent(IDM_EXIT, &MainDialog::OnMenuExit);
		Dialog::AddCommandEvent(IDM_ABOUT, &MainDialog::OnMenuAbout);
	}

	MESSAGE_ONINITDIALOG();
	MESSAGE_ONNOTIFY();

	COMMAND_HANDLER(OnMenuAbout);
	COMMAND_HANDLER(OnMenuExit);
	COMMAND_HANDLER(OnOK);
	COMMAND_HANDLER(OnCheckBoxClicked);

private:
	BOOL shield_state = FALSE;

	ComboBox* m_combo = nullptr;
	Button* m_dostuff = nullptr;
	CheckBox* m_check = nullptr;
	ListBox* m_list = nullptr;
	RichEdit* m_richedit = nullptr;
	EditText* m_spinedit = nullptr;
	ScrollBar* m_scroll = nullptr;
	ListView* m_view = nullptr;
	TreeView* m_tree = nullptr;
	TrackBar* m_track = nullptr;
	SpinControl* m_spin = nullptr;
	ProgressBar* m_progress = nullptr;
	TabControl* m_tab = nullptr;
};