#ifndef __STATUS_BAR_HPP__
#define __STATUS_BAR_HPP__

namespace wpp
{
	class statusbar : public control {
	public:
		using control::control;

		statusbar& on_simple_mode_change(notify_callback callback) { register_notify_callback(SBN_SIMPLEMODECHANGE, std::move(callback)); return *this; }

		int get_parts(int nParts, int* pParts) const {
			return (int)SendMessage(m_handle, SB_GETPARTS, nParts, (LPARAM)pParts);
		}

		BOOL set_parts(int nParts, int* pWidths) {
			return (BOOL)SendMessage(m_handle, SB_SETPARTS, nParts, (LPARAM)pWidths);
		}

		int get_text_length(int nPane, int* pType = NULL) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, SB_GETTEXTLENGTH, (WPARAM)nPane, 0L);
			if (pType != NULL)
				*pType = (int)(short)HIWORD(dwRet);
			return (int)(short)LOWORD(dwRet);
		}

		int get_text(int nPane, LPTSTR lpszText, int* pType = NULL) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, SB_GETTEXT, (WPARAM)nPane, (LPARAM)lpszText);
			if (pType != NULL)
				*pType = (int)(short)HIWORD(dwRet);
			return (int)(short)LOWORD(dwRet);
		}

		BOOL set_text(int nPane, LPCTSTR lpszText, int nType = 0) {
			return (BOOL)SendMessage(m_handle, SB_SETTEXT, (nPane | nType), (LPARAM)lpszText);
		}

		BOOL get_rect(int nPane, LPRECT lpRect) const {
			return (BOOL)SendMessage(m_handle, SB_GETRECT, nPane, (LPARAM)lpRect);
		}

		BOOL get_borders(int* pBorders) const {
			return (BOOL)SendMessage(m_handle, SB_GETBORDERS, 0, (LPARAM)pBorders);
		}

		BOOL get_borders(int& nHorz, int& nVert, int& nSpacing) const {
			int borders[3] = { 0, 0, 0 };
			BOOL bResult = (BOOL)SendMessage(m_handle, SB_GETBORDERS, 0, (LPARAM)&borders);
			if (bResult) {
				nHorz = borders[0];
				nVert = borders[1];
				nSpacing = borders[2];
			}
			return bResult;
		}

		void set_min_height(int nMin) {
			SendMessage(m_handle, SB_SETMINHEIGHT, nMin, 0L);
		}

		BOOL set_simple(BOOL bSimple = TRUE) {
			return (BOOL)SendMessage(m_handle, SB_SIMPLE, bSimple, 0L);
		}

		BOOL is_simple() const {
			return (BOOL)SendMessage(m_handle, SB_ISSIMPLE, 0, 0L);
		}

		BOOL get_unicode_format() const {
			return (BOOL)SendMessage(m_handle, SB_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return (BOOL)SendMessage(m_handle, SB_SETUNICODEFORMAT, bUnicode, 0L);
		}

		void get_tip_text(int nPane, LPTSTR lpstrText, int nSize) const {
			SendMessage(m_handle, SB_GETTIPTEXT, MAKEWPARAM(nPane, nSize), (LPARAM)lpstrText);
		}

		void set_tip_text(int nPane, LPCTSTR lpstrText) {
			SendMessage(m_handle, SB_SETTIPTEXT, nPane, (LPARAM)lpstrText);
		}

		COLORREF set_bk_color(COLORREF clrBk) {
			return (COLORREF)SendMessage(m_handle, SB_SETBKCOLOR, 0, (LPARAM)clrBk);
		}

		HICON get_icon(int nPane) const {
			return (HICON)SendMessage(m_handle, SB_GETICON, nPane, 0L);
		}

		BOOL set_icon(int nPane, HICON hIcon) {
			return (BOOL)SendMessage(m_handle, SB_SETICON, nPane, (LPARAM)hIcon);
		}

		int get_part_count() const {
			return get_parts(0, NULL);
		}

		BOOL is_valid_pane(int nPane) const {
			return nPane >= 0 && nPane < get_part_count();
		}

		std::tstring get_text_str(int nPane) const {
			int len = get_text_length(nPane);
			if (len <= 0) return std::tstring();

			std::vector<TCHAR> buffer(len + 1);
			get_text(nPane, buffer.data());
			return std::tstring(buffer.data());
		}

		std::tstring get_tip_text_str(int nPane) const {
			TCHAR buffer[256] = { 0 };
			get_tip_text(nPane, buffer, 256);
			return std::tstring(buffer);
		}

		std::vector<int> get_part_widths() const {
			std::vector<int> widths;
			int count = get_part_count();
			if (count > 0) {
				widths.resize(count);
				get_parts(count, widths.data());
			}
			return widths;
		}

		BOOL set_part_widths(const std::vector<int>& widths) {
			return set_parts((int)widths.size(), const_cast<int*>(widths.data()));
		}

		std::vector<std::tstring> get_all_texts() const {
			std::vector<std::tstring> texts;
			int count = get_part_count();
			for (int i = 0; i < count; i++) {
				texts.push_back(get_text_str(i));
			}
			return texts;
		}

		BOOL create_parts(int nParts, int* pWidths = nullptr) {
			if (pWidths) {
				return set_parts(nParts, pWidths);
			}
			
			RECT rc;
			GetClientRect(m_handle, &rc);
			int width = (rc.right - rc.left) / nParts;
			
			std::vector<int> widths(nParts);
			for (int i = 0; i < nParts; i++) {
				widths[i] = (i + 1) * width;
			}
			widths[nParts - 1] = -1;
			return set_parts(nParts, widths.data());
		}

		BOOL set_text_simple(LPCTSTR lpszText) {
			set_simple(TRUE);
			return set_text(0, lpszText);
		}

		std::tstring get_text_simple() const {
			return is_simple() ? get_text_str(0) : std::tstring();
		}

		BOOL remove_icon(int nPane) {
			return set_icon(nPane, NULL);
		}

		BOOL has_icon(int nPane) const {
			return get_icon(nPane) != NULL;
		}

		void remove_all_icons() {
			int count = get_part_count();
			for (int i = 0; i < count; i++) {
				remove_icon(i);
			}
		}

		RECT get_pane_rect(int nPane) const {
			RECT rc = { 0 };
			get_rect(nPane, &rc);
			return rc;
		}

		int get_pane_width(int nPane) const {
			RECT rc = get_pane_rect(nPane);
			return rc.right - rc.left;
		}

		BOOL set_equal_parts(int nParts) {
			return create_parts(nParts, nullptr);
		}

		BOOL set_parts_with_last_variable(const std::vector<int>& fixedWidths) {
			std::vector<int> widths = fixedWidths;
			widths.push_back(-1);
			return set_parts((int)widths.size(), widths.data());
		}

		void clear_text(int nPane) {
			set_text(nPane, TEXT(""));
		}

		void clear_all_text() {
			int count = get_part_count();
			for (int i = 0; i < count; i++) {
				clear_text(i);
			}
		}

		BOOL set_text_with_icon(int nPane, LPCTSTR lpszText, HICON hIcon) {
			set_icon(nPane, hIcon);
			return set_text(nPane, lpszText);
		}

		BOOL is_empty() const {
			return get_part_count() == 0;
		}

		int get_total_width() const {
			RECT rc;
			GetClientRect(m_handle, &rc);
			return rc.right - rc.left;
		}

		int get_height() const {
			RECT rc;
			GetClientRect(m_handle, &rc);
			return rc.bottom - rc.top;
		}

		void auto_resize_to_parent() {
			SendMessage(m_handle, WM_SIZE, 0, 0);
		}

		BOOL set_part_text_and_tooltip(int nPane, LPCTSTR lpszText, LPCTSTR lpszTooltip = nullptr) {
			BOOL result = set_text(nPane, lpszText);
			if (lpszTooltip) {
				set_tip_text(nPane, lpszTooltip);
			}
			return result;
		}

		void set_default_colors() {
			set_bk_color(CLR_DEFAULT);
		}

		BOOL set_text_centered(int nPane, LPCTSTR lpszText) {
			return set_text(nPane, lpszText, SBT_NOBORDERS);
		}

		BOOL set_text_popup(int nPane, LPCTSTR lpszText) {
			return set_text(nPane, lpszText, SBT_POPOUT);
		}

		BOOL set_text_owner_draw(int nPane, DWORD_PTR dwData) {
			return (BOOL)SendMessage(m_handle, SB_SETTEXT, (nPane | SBT_OWNERDRAW), dwData);
		}

		int get_horizontal_border() const {
			int borders[3] = { 0 };
			get_borders(borders);
			return borders[0];
		}

		int get_vertical_border() const {
			int borders[3] = { 0 };
			get_borders(borders);
			return borders[1];
		}

		int get_spacing() const {
			int borders[3] = { 0 };
			get_borders(borders);
			return borders[2];
		}

		void reset() {
			clear_all_text();
			remove_all_icons();
		}

		BOOL has_parts() const {
			return get_part_count() > 0;
		}

		BOOL set_pane_width(int nPane, int nWidth) {
			std::vector<int> widths = get_part_widths();
			if (nPane < 0 || nPane >= (int)widths.size()) return FALSE;
			
			int diff = nWidth - (widths[nPane] - (nPane > 0 ? widths[nPane - 1] : 0));
			
			for (int i = nPane; i < (int)widths.size(); i++) {
				if (widths[i] != -1) {
					widths[i] += diff;
				}
			}
			return set_part_widths(widths);
		}

		int find_pane_at_point(POINT pt) const {
			int count = get_part_count();
			for (int i = 0; i < count; i++) {
				RECT rc;
				if (get_rect(i, &rc) && PtInRect(&rc, pt)) {
					return i;
				}
			}
			return -1;
		}

		int find_pane_at_point(int x, int y) const {
			POINT pt = { x, y };
			return find_pane_at_point(pt);
		}

		BOOL set_progress_text(int nPane, int nPercent) {
			TCHAR buffer[32];
			_stprintf_s(buffer, 32, TEXT("%d%%"), nPercent);
			return set_text(nPane, buffer);
		}

		BOOL append_text(int nPane, LPCTSTR lpszText) {
			std::tstring current = get_text_str(nPane);
			current += lpszText;
			return set_text(nPane, current.c_str());
		}

		BOOL prepend_text(int nPane, LPCTSTR lpszText) {
			std::tstring current = get_text_str(nPane);
			current.insert(0, lpszText);
			return set_text(nPane, current.c_str());
		}

		void toggle_simple_mode() {
			set_simple(!is_simple());
		}

		BOOL is_text_empty(int nPane) const {
			return get_text_length(nPane) == 0;
		}

		void for_each_pane(std::function<void(int)> func) {
			int count = get_part_count();
			for (int i = 0; i < count; i++) {
				func(i);
			}
		}

		BOOL set_status_text(LPCTSTR lpszText) {
			if (is_simple()) {
				return set_text(0, lpszText);
			}
			
			if (has_parts()) {
				return set_text(0, lpszText);
			}
			
			create_parts(1);
			return set_text(0, lpszText);
		}

		BOOL set_ready() {
			return set_status_text(TEXT("Ready"));
		}

		BOOL set_busy(LPCTSTR lpszMessage = TEXT("Please wait...")) {
			return set_status_text(lpszMessage);
		}

		BOOL create_standard_parts() {
			int widths[] = { 200, 400, -1 };
			return set_parts(3, widths);
		}

		BOOL create_parts_from_percentages(const std::vector<int>& percentages) {
			int total = get_total_width();
			std::vector<int> widths;
			int accumulated = 0;
			
			for (size_t i = 0; i < percentages.size(); i++) {
				if (i == percentages.size() - 1 && percentages[i] == 100) {
					widths.push_back(-1);
				} else {
					accumulated += (total * percentages[i]) / 100;
					widths.push_back(accumulated);
				}
			}
			return set_part_widths(widths);
		}

		int get_last_pane() const {
			return get_part_count() - 1;
		}

		BOOL set_last_pane_text(LPCTSTR lpszText) {
			int last = get_last_pane();
			return last >= 0 ? set_text(last, lpszText) : FALSE;
		}

		std::tstring get_last_pane_text() const {
			int last = get_last_pane();
			return last >= 0 ? get_text_str(last) : std::tstring();
		}

		void refresh_pane(int nPane) {
			RECT rc;
			if (get_rect(nPane, &rc)) {
				InvalidateRect(m_handle, &rc, TRUE);
			}
		}

		void refresh_all() {
			InvalidateRect(m_handle, NULL, TRUE);
		}
	};
}

#endif //__STATUS_BAR_HPP__