#pragma once

#include "stdafx.h"

class MainDialog : public Dialog
{
public:
	MainDialog(HINSTANCE hInstance) : Dialog(hInstance, IDD_MAINDLG, IDC_TESTPROJ)
	{
		//Dialog::AddCommandEvent(IDC_OK_BTN, &MainDialog::OnOK);
		//Dialog::AddCommandEvent(IDC_TEST_CHECK, &MainDialog::OnCheckBoxClicked);
		//Dialog::AddCommandEvent(IDM_EXIT, &MainDialog::OnMenuExit);
		//Dialog::AddCommandEvent(IDM_ABOUT, &MainDialog::OnMenuAbout);
	}

	DIALOG_MESSAGE_HANDLER(OnInitDialog) override;

	COMMAND_HANDLER(OnMenuAbout);
	COMMAND_HANDLER(OnMenuExit);
	COMMAND_HANDLER(OnOK);
	COMMAND_HANDLER(OnCheckBoxClicked);

private:
	std::shared_ptr<ComboBox> m_combo;
	std::shared_ptr<Button> m_dostuff;
	std::shared_ptr<CheckBox> m_check;
	std::shared_ptr<ListBox> m_list;
	std::shared_ptr<RichEdit> m_richedit;
	std::shared_ptr<EditText> m_spinedit;
	std::shared_ptr<ScrollBar> m_scroll;
	std::shared_ptr<ListView> m_view;
	std::shared_ptr<TreeView> m_tree;
	std::shared_ptr<TrackBar> m_track;
	std::shared_ptr<UpDownControl> m_spin;
	std::shared_ptr<ProgressBar> m_progress;
	std::shared_ptr<TabControl> m_tab;
};