#include "stdafx.h"
#include "resource.h"
#include "MainDialog.hpp"
#include "MainWindow.hpp"

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	InitCommonControls();
	HMODULE hRichEd = LoadLibrary(TEXT("riched20.dll"));
	auto dialog = std::make_unique<MainDialog>(hInstance);
	auto mainwindow = std::make_unique<MainWindow>(TEXT("TestProj"), 0, 0, hInstance);
	dialog->RunDlg();
	mainwindow->Create();
	mainwindow->RunWindow();
	FreeLibrary(hRichEd);
	return 0;
}
