#include "stdafx.h"
#include "MainDialog.hpp"

#define MAX_RANGES 0, 100

INT_PTR CALLBACK MainDialog::OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RegisterControl(IDC_TEST_CHECK, &m_check);
	RegisterControl(IDC_LISTBOX_TEST, &m_list);
	RegisterControl(IDC_OK_BTN, &m_dostuff);
	RegisterControl(IDC_COMB_TEST, &m_combo);
	RegisterControl(IDC_RICHEDIT_TEST, &m_richedit);
	RegisterControl(IDC_EDIT_SPIN, &m_spinedit);
	RegisterControl(IDC_SCROLLBAR_TEST, &m_scroll);
	RegisterControl(IDC_LISTVIEW_TEST, &m_view);
	RegisterControl(IDC_TREE_TEST, &m_tree);
	RegisterControl(IDC_SLIDER_TEST, &m_track);
	RegisterControl(IDC_SPIN_TEST, &m_spin);
	RegisterControl(IDC_PROGRESS_TEST, &m_progress);
	RegisterControl(IDC_TAB_TEST, &m_tab);

	m_combo->SetCueBannerText(L"ComboBox control item test");

	if (!(m_track->GetStyle() & TBS_NOTIFYBEFOREMOVE))
		m_track->AddStyle(TBS_NOTIFYBEFOREMOVE);

	m_spin->SetBuddy(m_spinedit->GetHandle());
	m_spinedit->SetTextLimit(4);

	int nMin, nMax;

	m_progress->SetRange32(MAX_RANGES);
	m_progress->GetRange(nMin, nMax);

	m_track->SetRange(nMin, nMax);
	m_spin->SetRange(nMin, nMax);
	m_scroll->SetScrollRange(nMin, nMax);

	return Dialog::OnInitDialog(hWnd, wParam, lParam);
}

LRESULT CALLBACK MainDialog::OnOK(INT nControlId, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_list->ResetContent();
	m_combo->ResetContent();
	m_tree->DeleteAllItems();

	if (m_tab->GetItemCount() < 5)
		m_tab->AddItem((TEXT("Tab ") + std::to_tstring(m_tab->GetItemCount() + 1)).c_str());

	m_list->AddDir(0, TEXT("C:\\Windows\\System32\\*.exe"));
	m_combo->AddDir(0, TEXT("C:\\Windows\\System32\\*.exe"));

	m_combo->SetMinVisible(8);

	HTREEITEM trees[8] = { 0 };
	HTREEITEM tree = m_tree->InsertItem(TEXT("Main Item"), NULL, NULL);

	for (int i = 0; i < ARRAYSIZE(trees); i++)
		trees[i] = m_tree->InsertItem(TEXT("Sub Item"), i == 0 ? tree : trees[i - 1], NULL);

	for (int i = ARRAYSIZE(trees) - 1; i >= 0; i--)
		m_tree->Expand(trees[i]);

	m_tree->Expand(tree);

	return FALSE;
}

LRESULT CALLBACK MainDialog::OnMenuExit(INT nControlId, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog();
	return FALSE;
}

LRESULT CALLBACK MainDialog::OnMenuAbout(INT nControlID, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	MsgBoxInfo(TEXT("TestProj"), TEXT("TestProj.exe - Test project for use in WinPlusPlus!"));
	return FALSE;
}

LRESULT CALLBACK MainDialog::OnCheckBoxClicked(INT nControlID, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_dostuff->SetShield(shield_state = !shield_state);
	return FALSE;
}

INT_PTR CALLBACK MainDialog::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	auto nm = reinterpret_cast<NMHDR*>(lParam);

	switch (nm->idFrom)
	{
	case IDC_SPIN_TEST:
	{
		if (nm->code == UDN_DELTAPOS)
		{
			auto updn = reinterpret_cast<NMUPDOWN*>(nm);

			int minimum, maximum, new_val = updn->iPos + updn->iDelta;
			m_spin->GetRange32(minimum, maximum);

			if (new_val >= minimum && new_val <= maximum)
			{
				m_progress->SetPos(new_val);
				m_track->SetPos(new_val);
				m_scroll->SetScrollPos(new_val);
				m_spinedit->SetText(std::to_tstring(new_val));
				m_richedit->AppendText((TEXT("Delta is ") + std::to_tstring(updn->iDelta) + TEXT("\n")).c_str());
			}
		}
		break;
	}
	case IDC_SLIDER_TEST:
	{
		if (nm->code == TRBN_THUMBPOSCHANGING)
		{
			auto tbm = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(nm);

			int minimum, maximum, new_val = tbm->dwPos;
			m_track->GetRange(minimum, maximum);

			if (new_val >= minimum && new_val <= maximum)
			{
				m_progress->SetPos(new_val);
				m_spin->SetPos(new_val);
				m_scroll->SetScrollPos(new_val);
				m_spinedit->SetText(std::to_tstring(new_val));
			}
		}
		break;
	}
	}
	return Dialog::OnNotify(hWnd, wParam, lParam);
}