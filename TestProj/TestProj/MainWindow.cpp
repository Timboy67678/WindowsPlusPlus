#include "MainWindow.hpp"

MainWindow::MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance)
	: WPP::Window(WPP::Window::Class{ _T("MainWindowWPP"), instance },
				  window_title, x, y, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX))
{

}

LRESULT CALLBACK MainWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_ButtonOne = CreateButton(IDI_BUTTONONE, _T("Click Me!"), 0, 0, 150, 25);
	m_CheckBoxOne = CreateCheckBox(IDI_CHECKBOXONE, _T("Check Me!"), 0, 30, 150, 25);
	m_ComboBoxOne = CreateComboBox(IDI_COMBOBOXONE, 0, 60, 150, 25);
	m_EditTextOne = CreateEditText(IDI_EDITTEXTONE, 0, 90, 150, 25, _T("Edit me!"));
	m_ListViewOne = CreateListView(IDI_LISTVIEWONE, 0, 120, 150, 150);

	m_RadioButtonGroup = CreateRadioButtonGroup();
	auto radiobuttonone = m_RadioButtonGroup->CreateButton(IDI_RADIOBUTTONONE, _T("Radio 1"), 0, 270, 150, 25, TRUE);
	auto radiobuttontwo = m_RadioButtonGroup->CreateButton(IDI_RADIOBUTTONTWO, _T("Radio 2"), 0, 295, 150, 25);
	auto radiobuttonthree = m_RadioButtonGroup->CreateButton(IDI_RADIOBUTTONTHREE, _T("Radio 3"), 0, 320, 150, 25);

	auto radio_grptwo = CreateRadioButtonGroup();
	auto radio_one = radio_grptwo->CreateButton(1009, _T("Radio 2 1"), 0, 345, 150, 25, TRUE);
	auto radio_two = radio_grptwo->CreateButton(1010, _T("Radio 2 2"), 0, 370, 150, 25);
	auto radio_three = radio_grptwo->CreateButton(1011, _T("Radio 2 3"), 0, 395, 150, 25);

	m_ComboBoxOne->Add(_T("Item 1"));
	m_ComboBoxOne->Add(_T("Item 2"));
	m_ComboBoxOne->Add(_T("Item 3"));
	m_ComboBoxOne->Add(_T("Item 4"));

	m_ListViewOne->AddColumn(_T("Column 1"), 100);
	m_ListViewOne->AddColumn(_T("Column 2"), 100);
	m_ListViewOne->AddColumn(_T("Column 3"), 100);

	m_ListViewOne->AddItem(0, 0, _T("Item 1"));
	m_ListViewOne->AddItem(0, 1, _T("Column Item 1"));
	m_ListViewOne->AddItem(0, 2, _T("Column Item 2"));

	AddCommandEvent(IDI_BUTTONONE, &MainWindow::OnButtonOne);
	AddCommandEvent(IDI_CHECKBOXONE, &MainWindow::OnCheckBoxOne);

	return WPP::Window::OnCreate(hWnd, wParam, lParam);
}

LRESULT CALLBACK MainWindow::OnButtonOne(INT controlId, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case BN_CLICKED: {
		static int x = 0;
		std::tstring button_counter_str = TEXT("Button Clicked: ") + std::to_tstring(++x);
		m_ButtonOne->SetText(button_counter_str);
		break;
	}
	default:
		break;
	}

	return FALSE;
}

LRESULT CALLBACK MainWindow::OnCheckBoxOne(INT controlId, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case BN_CLICKED:
		break;
	default:
		break;
	}
	return FALSE;
}
