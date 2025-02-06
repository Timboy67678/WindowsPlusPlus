#pragma once

#include "stdafx.h"

#define IDI_BUTTONONE 1001
#define IDI_CHECKBOXONE 1002
#define IDI_COMBOBOXONE 1003
#define IDI_EDITTEXTONE 1004
#define IDI_LISTVIEWONE 1005
#define IDI_RADIOBUTTONONE 1006
#define IDI_RADIOBUTTONTWO 1007
#define IDI_RADIOBUTTONTHREE 1008

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

class MainWindow : public WPP::Window
{
public:
	MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance = NULL);

	WINDOW_MESSAGE_HANDLER(OnCreate);

	COMMAND_HANDLER(OnButtonOne);
	COMMAND_HANDLER(OnCheckBoxOne);

private:
	std::shared_ptr<WPP::Button> m_ButtonOne;
	std::shared_ptr<WPP::CheckBox> m_CheckBoxOne;
	std::shared_ptr<WPP::ComboBox> m_ComboBoxOne;
	std::shared_ptr<WPP::EditText> m_EditTextOne;
	std::shared_ptr<WPP::ListView> m_ListViewOne;
	std::shared_ptr<WPP::Window::RadioButtonGroup> m_RadioButtonGroup;
};

