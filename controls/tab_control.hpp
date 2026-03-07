#ifndef __TAB_CONTROL_H__
#define __TAB_CONTROL_H__

namespace wpp
{
	class tab_control : public control {
	public:
		using control::control;

		tab_control& on_selection_change(notify_callback callback) { register_notify_callback(TCN_SELCHANGE, std::move(callback)); return *this; }
		tab_control& on_selection_changing(notify_callback callback) { register_notify_callback(TCN_SELCHANGING, std::move(callback)); return *this; }
		tab_control& on_key_down(notify_callback callback) { register_notify_callback(TCN_KEYDOWN, std::move(callback)); return *this; }
		tab_control& on_get_object(notify_callback callback) { register_notify_callback(TCN_GETOBJECT, std::move(callback)); return *this; }
		tab_control& on_focus_change(notify_callback callback) { register_notify_callback(TCN_FOCUSCHANGE, std::move(callback)); return *this; }

		image_list get_image_list() const {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TCM_GETIMAGELIST, 0, 0L) };
		}

		image_list set_image_list(HIMAGELIST hImageList) {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TCM_SETIMAGELIST, 0, (LPARAM)hImageList) };
		}

		int get_item_count() const {
			return send_message<int>(TCM_GETITEMCOUNT, 0, 0L);
		}

		BOOL get_item(int nItem, LPTCITEM pTabCtrlItem) const {
			return send_message<BOOL>(TCM_GETITEM, nItem, (LPARAM)pTabCtrlItem);
		}

		BOOL set_item(int nItem, LPTCITEM pTabCtrlItem) {
			return send_message<BOOL>(TCM_SETITEM, nItem, (LPARAM)pTabCtrlItem);
		}

		int set_item(int nItem, UINT mask, LPCTSTR lpszItem, DWORD dwState, DWORD dwStateMask, int iImage, LPARAM lParam) {
			TCITEM tci = { 0 };
			tci.mask = mask;
			tci.pszText = (LPTSTR)lpszItem;
			tci.dwState = dwState;
			tci.dwStateMask = dwStateMask;
			tci.iImage = iImage;
			tci.lParam = lParam;
			return send_message<int>(TCM_SETITEM, nItem, (LPARAM)&tci);
		}

		BOOL get_item_rect(int nItem, LPRECT lpRect) const {
			return send_message<BOOL>(TCM_GETITEMRECT, nItem, (LPARAM)lpRect);
		}

		int get_cur_sel() const {
			return send_message<int>(TCM_GETCURSEL, 0, 0L);
		}

		int set_cur_sel(int nItem) {
			int previous = send_message<int>(TCM_SETCURSEL, nItem, 0L);
			sync_managed_pages();
			return previous;
		}

		SIZE set_item_size(SIZE size) {
			DWORD dwSize = (DWORD)send_message<DWORD>(TCM_SETITEMSIZE, 0, MAKELPARAM(size.cx, size.cy));
			SIZE sizeRet = { GET_X_LPARAM(dwSize), GET_Y_LPARAM(dwSize) };
			return sizeRet;
		}

		void set_item_size(int cx, int cy) {
			send_message(TCM_SETITEMSIZE, 0, MAKELPARAM(cx, cy));
		}

		void set_padding(SIZE size) {
			send_message(TCM_SETPADDING, 0, MAKELPARAM(size.cx, size.cy));
		}

		int get_row_count() const {
			return send_message<int>(TCM_GETROWCOUNT, 0, 0L);
		}

		tooltip get_tool_tips() const {
			return tooltip{ send_message<HWND>(TCM_GETTOOLTIPS, 0, 0L) };
		}

		void set_tool_tips(tooltip hWndToolTip) {
			send_message(TCM_SETTOOLTIPS, (WPARAM)hWndToolTip.get_handle(), 0L);
		}

		int get_cur_focus() const {
			return send_message<int>(TCM_GETCURFOCUS, 0, 0L);
		}

		void set_cur_focus(int nItem) {
			send_message(TCM_SETCURFOCUS, nItem, 0L);
		}

		BOOL set_item_extra(int cbExtra) {
			return send_message<BOOL>(TCM_SETITEMEXTRA, cbExtra, 0L);
		}

		int set_min_tab_width(int nWidth = -1) {
			return send_message<int>(TCM_SETMINTABWIDTH, 0, nWidth);
		}

		DWORD get_extended_style() const {
			return send_message<DWORD>(TCM_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD set_extended_style(DWORD dwExMask, DWORD dwExStyle) {
			return send_message<DWORD>(TCM_SETEXTENDEDSTYLE, dwExMask, dwExStyle);
		}

		BOOL get_unicode_format() const {
			return send_message<BOOL>(TCM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return send_message<BOOL>(TCM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int insert_item(int nItem, LPTCITEM pTabCtrlItem) {
			int insertedIndex = send_message<int>(TCM_INSERTITEM, nItem, (LPARAM)pTabCtrlItem);
			if (insertedIndex >= 0) {
				on_tab_inserted(insertedIndex);
				sync_managed_pages();
			}
			return insertedIndex;
		}

		int insert_item(int nItem, UINT mask, LPCTSTR lpszItem, int iImage, LPARAM lParam) {
			TCITEM tci = { 0 };
			tci.mask = mask;
			tci.pszText = (LPTSTR)lpszItem;
			tci.iImage = iImage;
			tci.lParam = lParam;
			int insertedIndex = send_message<int>(TCM_INSERTITEM, nItem, (LPARAM)&tci);
			if (insertedIndex >= 0) {
				on_tab_inserted(insertedIndex);
				sync_managed_pages();
			}
			return insertedIndex;
		}

		int insert_item(int nItem, LPCTSTR lpszItem) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_TEXT;
			tci.pszText = (LPTSTR)lpszItem;
			int insertedIndex = send_message<int>(TCM_INSERTITEM, nItem, (LPARAM)&tci);
			if (insertedIndex >= 0) {
				on_tab_inserted(insertedIndex);
				sync_managed_pages();
			}
			return insertedIndex;
		}

		int add_item(LPTCITEM pTabCtrlItem) {
			return insert_item(get_item_count(), pTabCtrlItem);
		}

		int add_item(UINT mask, LPCTSTR lpszItem, int iImage, LPARAM lParam) {
			return insert_item(get_item_count(), mask, lpszItem, iImage, lParam);
		}

		int add_item(LPCTSTR lpszItem) {
			return insert_item(get_item_count(), lpszItem);
		}

		BOOL delete_item(int nItem) {
			BOOL result = send_message<BOOL>(TCM_DELETEITEM, nItem, 0L);
			if (result) {
				on_tab_deleted(nItem);
				sync_managed_pages();
			}
			return result;
		}

		BOOL delete_all_items() {
			BOOL result = send_message<BOOL>(TCM_DELETEALLITEMS, 0, 0L);
			if (result) {
				m_page_controls.clear();
			}
			return result;
		}

		void adjust_rect(BOOL bLarger, LPRECT lpRect) {
			send_message(TCM_ADJUSTRECT, bLarger, (LPARAM)lpRect);
		}

		void remove_image(int nImage) {
			send_message(TCM_REMOVEIMAGE, nImage, 0L);
		}

		int hit_test(TC_HITTESTINFO* pHitTestInfo) const {
			return send_message<int>(TCM_HITTEST, 0, (LPARAM)pHitTestInfo);
		}

		void deselect_all(BOOL bExcludeFocus = TRUE) {
			send_message(TCM_DESELECTALL, bExcludeFocus, 0L);
		}

		BOOL highlight_item(int nIndex, BOOL bHighlight = TRUE) {
			return send_message<BOOL>(TCM_HIGHLIGHTITEM, nIndex, MAKELPARAM(bHighlight, 0));
		}

		int get_item_text(int nItem, LPTSTR lpszText, int nLen) const {
			TCITEM tci = { 0 };
			tci.mask = TCIF_TEXT;
			tci.pszText = lpszText;
			tci.cchTextMax = nLen;
			return get_item(nItem, &tci) ? lstrlen(lpszText) : 0;
		}

		tstring get_item_text(int nItem) const {
			TCHAR buffer[256] = { 0 };
			get_item_text(nItem, buffer, 256);
			return tstring(buffer);
		}

		BOOL set_item_text(int nItem, LPCTSTR lpszText) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_TEXT;
			tci.pszText = (LPTSTR)lpszText;
			return set_item(nItem, &tci);
		}

		int get_item_image(int nItem) const {
			TCITEM tci = { 0 };
			tci.mask = TCIF_IMAGE;
			return get_item(nItem, &tci) ? tci.iImage : -1;
		}

		BOOL set_item_image(int nItem, int iImage) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_IMAGE;
			tci.iImage = iImage;
			return set_item(nItem, &tci);
		}

		LPARAM get_item_data(int nItem) const {
			TCITEM tci = { 0 };
			tci.mask = TCIF_PARAM;
			return get_item(nItem, &tci) ? tci.lParam : 0;
		}

		BOOL set_item_data(int nItem, LPARAM lParam) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_PARAM;
			tci.lParam = lParam;
			return set_item(nItem, &tci);
		}

		DWORD get_item_state(int nItem, DWORD dwMask = TCIS_BUTTONPRESSED | TCIS_HIGHLIGHTED) const {
			TCITEM tci = { 0 };
			tci.mask = TCIF_STATE;
			tci.dwStateMask = dwMask;
			return get_item(nItem, &tci) ? tci.dwState : 0;
		}

		BOOL set_item_state(int nItem, DWORD dwState, DWORD dwMask = TCIS_BUTTONPRESSED | TCIS_HIGHLIGHTED) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_STATE;
			tci.dwState = dwState;
			tci.dwStateMask = dwMask;
			return set_item(nItem, &tci);
		}

		int insert_item(int nItem, LPCTSTR lpszText, int iImage) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_TEXT | TCIF_IMAGE;
			tci.pszText = (LPTSTR)lpszText;
			tci.iImage = iImage;
			int insertedIndex = send_message<int>(TCM_INSERTITEM, nItem, (LPARAM)&tci);
			if (insertedIndex >= 0) {
				on_tab_inserted(insertedIndex);
				sync_managed_pages();
			}
			return insertedIndex;
		}

		int add_item(LPCTSTR lpszText, int iImage) {
			return insert_item(get_item_count(), lpszText, iImage);
		}

		int add_item(LPCTSTR lpszText, int iImage, LPARAM lParam) {
			TCITEM tci = { 0 };
			tci.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
			tci.pszText = (LPTSTR)lpszText;
			tci.iImage = iImage;
			tci.lParam = lParam;
			return insert_item(get_item_count(), &tci);
		}

		tab_control& add_page_control(int tabIndex, HWND controlHandle, BOOL fitToDisplayRect = TRUE) {
			if (!::IsWindow(controlHandle) || !is_valid_index(tabIndex)) {
				return *this;
			}

			auto& controls = m_page_controls[tabIndex];
			if (std::find(controls.begin(), controls.end(), controlHandle) == controls.end()) {
				controls.push_back(controlHandle);
			}

			if (fitToDisplayRect) {
				fit_control_to_display_rect(controlHandle);
			}

			sync_managed_pages();
			return *this;
		}

		tab_control& add_page_control(int tabIndex, const control& pageControl, BOOL fitToDisplayRect = TRUE) {
			return add_page_control(tabIndex, pageControl.get_handle(), fitToDisplayRect);
		}

		BOOL remove_page_control(int tabIndex, HWND controlHandle) {
			auto pageIt = m_page_controls.find(tabIndex);
			if (pageIt == m_page_controls.end()) {
				return FALSE;
			}

			auto& controls = pageIt->second;
			auto controlIt = std::find(controls.begin(), controls.end(), controlHandle);
			if (controlIt == controls.end()) {
				return FALSE;
			}

			controls.erase(controlIt);
			if (controls.empty()) {
				m_page_controls.erase(pageIt);
			}

			sync_managed_pages();
			return TRUE;
		}

		BOOL remove_page_control(int tabIndex, const control& pageControl) {
			return remove_page_control(tabIndex, pageControl.get_handle());
		}

		void clear_page_controls(int tabIndex) {
			m_page_controls.erase(tabIndex);
			sync_managed_pages();
		}

		void clear_all_page_controls() {
			m_page_controls.clear();
		}

		const std::vector<HWND>& get_page_controls(int tabIndex) const {
			auto it = m_page_controls.find(tabIndex);
			if (it != m_page_controls.end()) {
				return it->second;
			}

			static const std::vector<HWND> empty;
			return empty;
		}

		void fit_page_to_display_rect(int tabIndex) {
			auto it = m_page_controls.find(tabIndex);
			if (it == m_page_controls.end()) {
				return;
			}

			for (HWND handle : it->second) {
				fit_control_to_display_rect(handle);
			}
		}

		void fit_selected_page_to_display_rect() {
			int selected = get_cur_sel();
			if (selected >= 0) {
				fit_page_to_display_rect(selected);
			}
		}

		void sync_managed_pages() {
			int selected = get_cur_sel();
			for (auto& [tabIndex, controls] : m_page_controls) {
				bool show = (tabIndex == selected);
				for (HWND handle : controls) {
					if (!::IsWindow(handle)) {
						continue;
					}

					if (show) {
						fit_control_to_display_rect(handle);
					}

					::ShowWindow(handle, show ? SW_SHOW : SW_HIDE);
				}
			}
		}

		tstring get_selected_text() const {
			int nSel = get_cur_sel();
			return nSel >= 0 ? get_item_text(nSel) : tstring();
		}

		LPARAM get_selected_data() const {
			int nSel = get_cur_sel();
			return nSel >= 0 ? get_item_data(nSel) : 0;
		}

		BOOL select_next() {
			int nCurrent = get_cur_sel();
			int nCount = get_item_count();
			if (nCurrent >= 0 && nCurrent < nCount - 1) {
				return set_cur_sel(nCurrent + 1) >= 0;
			}
			return FALSE;
		}

		BOOL select_prev() {
			int nCurrent = get_cur_sel();
			if (nCurrent > 0) {
				return set_cur_sel(nCurrent - 1) >= 0;
			}
			return FALSE;
		}

		BOOL select_first() {
			return get_item_count() > 0 ? set_cur_sel(0) >= 0 : FALSE;
		}

		BOOL select_last() {
			int nCount = get_item_count();
			return nCount > 0 ? set_cur_sel(nCount - 1) >= 0 : FALSE;
		}

		int find_item(LPCTSTR lpszText, BOOL bExact = TRUE) const {
			int nCount = get_item_count();
			for (int i = 0; i < nCount; i++) {
				tstring itemText = get_item_text(i);
				if (bExact) {
					if (_tccmp(itemText.c_str(), lpszText) == 0)
						return i;
				}
				else {
					if (_tcsstr(itemText.c_str(), lpszText) != nullptr)
						return i;
				}
			}
			return -1;
		}

		int find_item_by_data(LPARAM lParam) const {
			int nCount = get_item_count();
			for (int i = 0; i < nCount; i++) {
				if (get_item_data(i) == lParam)
					return i;
			}
			return -1;
		}

		BOOL is_valid_index(int nItem) const {
			return nItem >= 0 && nItem < get_item_count();
		}

		int hit_test(POINT pt) const {
			TC_HITTESTINFO hti = { 0 };
			hti.pt = pt;
			return hit_test(&hti);
		}

		RECT get_display_rect() {
			RECT rc = get_client_rect();
			adjust_rect(FALSE, &rc);
			return rc;
		}

		void ensure_visible(int nItem) {
			if (is_valid_index(nItem)) {
				RECT rcItem = { 0 };
				get_item_rect(nItem, &rcItem);
				RECT rcClient = get_client_rect();
				if (rcItem.left < rcClient.left || rcItem.right > rcClient.right) {
					set_cur_sel(nItem);
				}
			}
		}

	private:
		void fit_control_to_display_rect(HWND controlHandle) {
			if (!::IsWindow(controlHandle)) {
				return;
			}

			RECT rcDisplay = get_display_rect();
			HWND controlParent = ::GetParent(controlHandle);

			if (controlParent != nullptr && controlParent != m_handle) {
				POINT points[2] = {
					{ rcDisplay.left, rcDisplay.top },
					{ rcDisplay.right, rcDisplay.bottom }
				};
				::MapWindowPoints(m_handle, controlParent, points, 2);
				rcDisplay.left = points[0].x;
				rcDisplay.top = points[0].y;
				rcDisplay.right = points[1].x;
				rcDisplay.bottom = points[1].y;
			}

			::SetWindowPos(
				controlHandle,
				nullptr,
				rcDisplay.left,
				rcDisplay.top,
				rcDisplay.right - rcDisplay.left,
				rcDisplay.bottom - rcDisplay.top,
				SWP_NOZORDER | SWP_NOACTIVATE
			);
		}

		void on_tab_deleted(int deletedIndex) {
			auto erased = m_page_controls.find(deletedIndex);
			if (erased != m_page_controls.end()) {
				m_page_controls.erase(erased);
			}

			std::unordered_map<int, std::vector<HWND>> shifted;
			for (auto& [index, controls] : m_page_controls) {
				shifted[index > deletedIndex ? index - 1 : index] = std::move(controls);
			}
			m_page_controls = std::move(shifted);
		}

		void on_tab_inserted(int insertedIndex) {
			std::unordered_map<int, std::vector<HWND>> shifted;
			for (auto& [index, controls] : m_page_controls) {
				shifted[index >= insertedIndex ? index + 1 : index] = std::move(controls);
			}
			m_page_controls = std::move(shifted);
		}

		std::unordered_map<int, std::vector<HWND>> m_page_controls;
	};
}

#endif //__TAB_CONTROL_H__