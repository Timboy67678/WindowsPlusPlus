#pragma once

#include "stdafx.h"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

class MainWindow : public WPP::Window
{
public:
	MainWindow(LPCTSTR window_title, int x, int y, HINSTANCE instance = NULL);

	WINDOW_MESSAGE_HANDLER(OnCreate);

private:
	std::shared_ptr<WPP::Button> m_ButtonOne;
	std::shared_ptr<WPP::CheckBox> m_CheckBoxOne;
	std::shared_ptr<WPP::ComboBox> m_ComboBoxOne;
	std::shared_ptr<WPP::EditText> m_EditTextOne;
	std::shared_ptr<WPP::ListView> m_ListViewOne;
	std::shared_ptr<WPP::Window::RadioButtonGroup> m_RadioButtonGroup;
	std::shared_ptr<WPP::SysLink> m_LinkControl;
};

