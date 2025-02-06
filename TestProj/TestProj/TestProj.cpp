#include "stdafx.h"
#include "resource.h"
#include "MainDialog.hpp"

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	InitCommonControls();
	HMODULE hRichEd = LoadLibrary(TEXT("riched20.dll"));
	(new MainDialog(hInstance))->RunDlg();
	FreeLibrary(hRichEd);
	return 0;
}
