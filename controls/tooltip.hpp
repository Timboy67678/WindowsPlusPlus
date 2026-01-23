#ifndef __TOOLTIP_HPP__
#define __TOOLTIP_HPP__

namespace wpp
{
	struct tool_info_t : public TOOLINFO {
	public:
		tool_info_t(UINT nFlags, HWND hWnd, UINT_PTR nIDTool = 0, LPRECT lpRect = NULL, LPTSTR lpstrText = LPSTR_TEXTCALLBACK, LPARAM lUserParam = NULL) {
			init(nFlags, hWnd, nIDTool, lpRect, lpstrText, lUserParam);
		}

		operator LPTOOLINFO() { return this; }

		operator LPARAM() { return (LPARAM)this; }

		void init(UINT nFlags, HWND hWnd, UINT_PTR nIDTool = 0, LPRECT lpRect = NULL, LPTSTR lpstrText = LPSTR_TEXTCALLBACK, LPARAM lUserParam = NULL) {
			memset(this, 0, sizeof(TOOLINFO));
			cbSize = sizeof(TOOLINFO);
			uFlags = nFlags;
			if (nIDTool == 0) {
				hwnd = ::GetParent(hWnd);
				uFlags |= TTF_IDISHWND;
				uId = (UINT_PTR)hWnd;
			} else {
				hwnd = hWnd;
				uId = nIDTool;
			}
			if (lpRect != NULL)
				rect = *lpRect;
			hinst = GetModuleHandle(NULL);
			lpszText = lpstrText;
			lParam = lUserParam;
		}
	};

	class tooltip : public control {
	public:
		using control::control;

		tooltip& on_show(notify_callback callback) { register_notify_callback(TTN_SHOW, std::move(callback)); return *this; }
		tooltip& on_pop(notify_callback callback) { register_notify_callback(TTN_POP, std::move(callback)); return *this; }
		tooltip& on_link_click(notify_callback callback) { register_notify_callback(TTN_LINKCLICK, std::move(callback)); return *this; }
		tooltip& on_get_disp_info(notify_callback callback) { register_notify_callback(TTN_GETDISPINFO, std::move(callback)); return *this; }
		tooltip& on_need_text(notify_callback callback) { return on_get_disp_info(callback); }

		void get_text(LPTOOLINFO lpToolInfo) const {
			SendMessage(m_handle, TTM_GETTEXT, 0, (LPARAM)lpToolInfo);
		}

		void get_text(LPTSTR lpstrText, HWND hWnd, UINT_PTR nIDTool = 0) const {
			tool_info_t ti(0, hWnd, nIDTool, NULL, lpstrText);
			SendMessage(m_handle, TTM_GETTEXT, 0, ti);
		}

		BOOL get_tool_info(LPTOOLINFO lpToolInfo) const {
			return (BOOL)SendMessage(m_handle, TTM_GETTOOLINFO, 0, (LPARAM)lpToolInfo);
		}

		BOOL get_tool_info(HWND hWnd, UINT_PTR nIDTool, UINT* puFlags, LPRECT lpRect, LPTSTR lpstrText) const {
			tool_info_t ti(0, hWnd, nIDTool, NULL, lpstrText);
			BOOL bRet = (BOOL)SendMessage(m_handle, TTM_GETTOOLINFO, 0, ti);
			if (bRet != FALSE) {
				*puFlags = ti.uFlags;
				*lpRect = ti.rect;
			}
			return bRet;
		}

		void set_tool_info(LPTOOLINFO lpToolInfo) {
			SendMessage(m_handle, TTM_SETTOOLINFO, 0, (LPARAM)lpToolInfo);
		}

		void set_tool_rect(LPTOOLINFO lpToolInfo) {
			SendMessage(m_handle, TTM_NEWTOOLRECT, 0, (LPARAM)lpToolInfo);
		}

		void set_tool_rect(HWND hWnd, UINT_PTR nIDTool, LPCRECT lpRect) {
			tool_info_t ti(0, hWnd, nIDTool, (LPRECT)lpRect, NULL);
			SendMessage(m_handle, TTM_NEWTOOLRECT, 0, ti);
		}

		int get_tool_count() const {
			return (int)SendMessage(m_handle, TTM_GETTOOLCOUNT, 0, 0L);
		}

		int get_delay_time(DWORD dwType) const {
			return (int)SendMessage(m_handle, TTM_GETDELAYTIME, dwType, 0L);
		}

		void set_delay_time(DWORD dwType, int nTime) {
			SendMessage(m_handle, TTM_SETDELAYTIME, dwType, MAKELPARAM(nTime, 0));
		}

		void get_margin(LPRECT lpRect) const {
			SendMessage(m_handle, TTM_GETMARGIN, 0, (LPARAM)lpRect);
		}

		void set_margin(LPRECT lpRect) {
			SendMessage(m_handle, TTM_SETMARGIN, 0, (LPARAM)lpRect);
		}

		int get_max_tip_width() const {
			return (int)SendMessage(m_handle, TTM_GETMAXTIPWIDTH, 0, 0L);
		}

		int set_max_tip_width(int nWidth) {
			return (int)SendMessage(m_handle, TTM_SETMAXTIPWIDTH, 0, nWidth);
		}

		COLORREF get_tip_bk_color() const {
			return (COLORREF)SendMessage(m_handle, TTM_GETTIPBKCOLOR, 0, 0L);
		}

		void set_tip_bk_color(COLORREF clr) {
			SendMessage(m_handle, TTM_SETTIPBKCOLOR, (WPARAM)clr, 0L);
		}

		COLORREF get_tip_text_color() const {
			return (COLORREF)SendMessage(m_handle, TTM_GETTIPTEXTCOLOR, 0, 0L);
		}

		void set_tip_text_color(COLORREF clr) {
			SendMessage(m_handle, TTM_SETTIPTEXTCOLOR, (WPARAM)clr, 0L);
		}

		BOOL get_current_tool(LPTOOLINFO lpToolInfo) const {
			return (BOOL)SendMessage(m_handle, TTM_GETCURRENTTOOL, 0, (LPARAM)lpToolInfo);
		}

		SIZE get_bubble_size(LPTOOLINFO lpToolInfo) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, TTM_GETBUBBLESIZE, 0, (LPARAM)lpToolInfo);
			SIZE size = { GET_X_LPARAM(dwRet), GET_Y_LPARAM(dwRet) };
			return size;
		}

		BOOL set_title(UINT_PTR uIcon, LPCTSTR lpstrTitle) {
			return (BOOL)SendMessage(m_handle, TTM_SETTITLE, uIcon, (LPARAM)lpstrTitle);
		}

		BOOL set_title(HICON hIcon, LPCTSTR lpstrTitle) {
			return (BOOL)SendMessage(m_handle, TTM_SETTITLE, (WPARAM)hIcon, (LPARAM)lpstrTitle);
		}

		void get_title(PTTGETTITLE pTTGetTitle) const {
			SendMessage(m_handle, TTM_GETTITLE, 0, (LPARAM)pTTGetTitle);
		}

		void set_window_theme(LPCWSTR lpstrTheme) {
			SendMessage(m_handle, TTM_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
		}

		void activate(BOOL bActivate) {
			SendMessage(m_handle, TTM_ACTIVATE, bActivate, 0L);
		}

		BOOL add_tool(LPTOOLINFO lpToolInfo) {
			return (BOOL)SendMessage(m_handle, TTM_ADDTOOL, 0, (LPARAM)lpToolInfo);
		}

		BOOL add_tool(HWND hWnd, LPTSTR text = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT_PTR nIDTool = 0) {
			tool_info_t ti(0, hWnd, nIDTool, (LPRECT)lpRectTool, (LPTSTR)text);
			return (BOOL)SendMessage(m_handle, TTM_ADDTOOL, 0, ti);
		}

		void del_tool(LPTOOLINFO lpToolInfo) {
			SendMessage(m_handle, TTM_DELTOOL, 0, (LPARAM)lpToolInfo);
		}

		void del_tool(HWND hWnd, UINT_PTR nIDTool = 0) {
			tool_info_t ti(0, hWnd, nIDTool, NULL, NULL);
			SendMessage(m_handle, TTM_DELTOOL, 0, ti);
		}

		BOOL hit_test(LPTTHITTESTINFO lpHitTestInfo) const {
			return (BOOL)SendMessage(m_handle, TTM_HITTEST, 0, (LPARAM)lpHitTestInfo);
		}

		BOOL hit_test(HWND hWnd, POINT pt, LPTOOLINFO lpToolInfo) const {
			TTHITTESTINFO hti = { 0 };
			hti.ti.cbSize = sizeof(TOOLINFO);
			hti.hwnd = hWnd;
			hti.pt = pt;

			if ((BOOL)SendMessage(m_handle, TTM_HITTEST, 0, (LPARAM)&hti) != FALSE) {
				*lpToolInfo = hti.ti;
				return TRUE;
			}
			return FALSE;
		}

		void relay_event(LPMSG lpMsg) {
			SendMessage(m_handle, TTM_RELAYEVENT, 0, (LPARAM)lpMsg);
		}

		void update_tip_text(LPTOOLINFO lpToolInfo) {
			SendMessage(m_handle, TTM_UPDATETIPTEXT, 0, (LPARAM)lpToolInfo);
		}

		void update_tip_text(LPCTSTR text, HWND hWnd, UINT_PTR nIDTool = 0) {
			tool_info_t ti(0, hWnd, nIDTool, NULL, (LPTSTR)text);
			SendMessage(m_handle, TTM_UPDATETIPTEXT, 0, ti);
		}

		BOOL enum_tools(UINT_PTR nTool, LPTOOLINFO lpToolInfo) const {
			return (BOOL)SendMessage(m_handle, TTM_ENUMTOOLS, nTool, (LPARAM)lpToolInfo);
		}

		void pop() {
			SendMessage(m_handle, TTM_POP, 0, 0L);
		}

		void track_activate(LPTOOLINFO lpToolInfo, BOOL bActivate) {
			SendMessage(m_handle, TTM_TRACKACTIVATE, bActivate, (LPARAM)lpToolInfo);
		}

		void track_activate(HWND hWnd, UINT_PTR nIDTool, BOOL bActivate) {
			tool_info_t ti(0, hWnd, nIDTool);
			SendMessage(m_handle, TTM_TRACKACTIVATE, bActivate, ti);
		}

		void track_position(int xPos, int yPos) {
			SendMessage(m_handle, TTM_TRACKPOSITION, 0, MAKELPARAM(xPos, yPos));
		}

		void update() {
			SendMessage(m_handle, TTM_UPDATE, 0, 0L);
		}

		BOOL adjust_rect(LPRECT lpRect, BOOL bLarger = TRUE) {
			return (BOOL)SendMessage(m_handle, TTM_ADJUSTRECT, bLarger, (LPARAM)lpRect);
		}

		void popup() {
			SendMessage(m_handle, TTM_POPUP, 0, 0L);
		}

		std::basic_string<TCHAR> get_tool_text(HWND hWnd, UINT_PTR nIDTool = 0) const {
			TCHAR buffer[512] = { 0 };
			get_text(buffer, hWnd, nIDTool);
			return std::basic_string<TCHAR>(buffer);
		}

		BOOL tool_exists(HWND hWnd, UINT_PTR nIDTool = 0) const {
			tool_info_t ti(0, hWnd, nIDTool);
			return get_tool_info(ti);
		}

		BOOL is_active() const {
			return IsWindowEnabled(m_handle);
		}

		void deactivate() {
			activate(FALSE);
		}

		BOOL is_balloon() const {
			return (get_style() & TTS_BALLOON) == TTS_BALLOON;
		}

		BOOL is_always_tip() const {
			return (get_style() & TTS_ALWAYSTIP) == TTS_ALWAYSTIP;
		}

		BOOL is_no_animate() const {
			return (get_style() & TTS_NOANIMATE) == TTS_NOANIMATE;
		}

		BOOL is_no_fade() const {
			return (get_style() & TTS_NOFADE) == TTS_NOFADE;
		}

		BOOL is_no_prefix() const {
			return (get_style() & TTS_NOPREFIX) == TTS_NOPREFIX;
		}

		BOOL is_close() const {
			return (get_style() & TTS_CLOSE) == TTS_CLOSE;
		}

		int get_autopop_delay() const {
			return get_delay_time(TTDT_AUTOPOP);
		}

		void set_autopop_delay(int nTime) {
			set_delay_time(TTDT_AUTOPOP, nTime);
		}

		int get_initial_delay() const {
			return get_delay_time(TTDT_INITIAL);
		}

		void set_initial_delay(int nTime) {
			set_delay_time(TTDT_INITIAL, nTime);
		}

		int get_reshow_delay() const {
			return get_delay_time(TTDT_RESHOW);
		}

		void set_reshow_delay(int nTime) {
			set_delay_time(TTDT_RESHOW, nTime);
		}

		void set_delay_times(int nAutoPop, int nInitial, int nReshow) {
			set_autopop_delay(nAutoPop);
			set_initial_delay(nInitial);
			set_reshow_delay(nReshow);
		}

		void set_default_delays() {
			set_delay_time(TTDT_AUTOMATIC, -1);
		}

		void set_fast_delays() {
			set_delay_times(2000, 100, 50);
		}

		void set_slow_delays() {
			set_delay_times(10000, 1000, 500);
		}

		BOOL add_tool_simple(HWND hWnd, LPCTSTR text) {
			return add_tool(hWnd, (LPTSTR)text, NULL, 0);
		}

		BOOL add_tool_rect(HWND hWnd, LPCTSTR text, LPCRECT lpRect, UINT_PTR nIDTool) {
			return add_tool(hWnd, (LPTSTR)text, lpRect, nIDTool);
		}

		BOOL update_tool_text_simple(HWND hWnd, LPCTSTR text) {
			update_tip_text(text, hWnd, 0);
			return TRUE;
		}

		void remove_all_tools() {
			int count = get_tool_count();
			for (int i = count - 1; i >= 0; i--) {
				TOOLINFO ti = { sizeof(TOOLINFO) };
				if (enum_tools(i, &ti)) {
					del_tool(&ti);
				}
			}
		}

		std::vector<HWND> get_all_tool_windows() const {
			std::vector<HWND> tools;
			int count = get_tool_count();
			for (int i = 0; i < count; i++) {
				TOOLINFO ti = { sizeof(TOOLINFO) };
				if (enum_tools(i, &ti)) {
					tools.push_back(ti.hwnd);
				}
			}
			return tools;
		}

		BOOL set_info_title(LPCTSTR title) {
			return set_title((UINT_PTR)TTI_INFO, title);
		}

		BOOL set_warning_title(LPCTSTR title) {
			return set_title((UINT_PTR)TTI_WARNING, title);
		}

		BOOL set_error_title(LPCTSTR title) {
			return set_title((UINT_PTR)TTI_ERROR, title);
		}

		BOOL set_info_large_title(LPCTSTR title) {
			return set_title((UINT_PTR)TTI_INFO_LARGE, title);
		}

		BOOL set_warning_large_title(LPCTSTR title) {
			return set_title((UINT_PTR)TTI_WARNING_LARGE, title);
		}

		BOOL set_error_large_title(LPCTSTR title) {
			return set_title((UINT_PTR)TTI_ERROR_LARGE, title);
		}

		BOOL clear_title() {
			return set_title((UINT_PTR)TTI_NONE, NULL);
		}

		void set_colors(COLORREF bkColor, COLORREF textColor) {
			set_tip_bk_color(bkColor);
			set_tip_text_color(textColor);
		}

		void set_default_colors() {
			set_tip_bk_color(RGB(255, 255, 225));
			set_tip_text_color(RGB(0, 0, 0));
		}

		void set_dark_colors() {
			set_tip_bk_color(RGB(45, 45, 48));
			set_tip_text_color(RGB(241, 241, 241));
		}

		void set_light_colors() {
			set_tip_bk_color(RGB(240, 240, 240));
			set_tip_text_color(RGB(30, 30, 30));
		}

		void set_max_width_auto() {
			set_max_tip_width(-1);
		}

		void set_max_width_single_line() {
			set_max_tip_width(0);
		}

		BOOL enable_tool(HWND hWnd, UINT_PTR nIDTool = 0, BOOL bEnable = TRUE) {
			if (bEnable) {
				// If enabling and tool doesn't exist, nothing to do
				if (!tool_exists(hWnd, nIDTool)) {
					return FALSE;
				}
				// Tool already exists and is enabled
				return TRUE;
			}
			else {
				// To disable, we remove the tool
				if (tool_exists(hWnd, nIDTool)) {
					del_tool(hWnd, nIDTool);
					return TRUE;
				}
				return FALSE;
			}
		}

		BOOL disable_tool(HWND hWnd, UINT_PTR nIDTool = 0) {
			return enable_tool(hWnd, nIDTool, FALSE);
		}

		BOOL is_tool_enabled(HWND hWnd, UINT_PTR nIDTool = 0) const {
			return tool_exists(hWnd, nIDTool);
		}

		void show_at(int x, int y) {
			track_position(x, y);
			popup();
		}

		void show_at_cursor() {
			POINT pt;
			GetCursorPos(&pt);
			show_at(pt.x, pt.y);
		}

		void hide() {
			pop();
		}

		BOOL is_visible() const {
			return IsWindowVisible(m_handle);
		}

		void center_track_tooltip(HWND hWnd) {
			tool_info_t ti(TTF_TRACK | TTF_ABSOLUTE, hWnd);
			if (add_tool(ti)) {
				RECT rc;
				GetWindowRect(hWnd, &rc);
				int x = (rc.left + rc.right) / 2;
				int y = (rc.top + rc.bottom) / 2;
				track_position(x, y);
				track_activate(hWnd, 0, TRUE);
			}
		}

		void stop_tracking(HWND hWnd, UINT_PTR nIDTool = 0) {
			track_activate(hWnd, nIDTool, FALSE);
		}

		BOOL add_multiline_tool(HWND hWnd, LPCTSTR text, int maxWidth = 300) {
			if (add_tool_simple(hWnd, text)) {
				set_max_tip_width(maxWidth);
				return TRUE;
			}
			return FALSE;
		}

		RECT get_tool_rect(HWND hWnd, UINT_PTR nIDTool = 0) const {
			RECT rc = { 0 };
			UINT flags = 0;
			TCHAR text[256] = { 0 };
			get_tool_info(hWnd, nIDTool, &flags, &rc, text);
			return rc;
		}

		void set_margin_all(int margin) {
			RECT rc = { margin, margin, margin, margin };
			set_margin(&rc);
		}

		void set_margin(int left, int top, int right, int bottom) {
			RECT rc = { left, top, right, bottom };
			set_margin(&rc);
		}

		RECT get_margin_rect() const {
			RECT rc = { 0 };
			get_margin(&rc);
			return rc;
		}

		SIZE get_current_bubble_size() const {
			TOOLINFO ti = { sizeof(TOOLINFO) };
			if (get_current_tool(&ti)) {
				return get_bubble_size(&ti);
			}
			return { 0, 0 };
		}

		BOOL set_aero_theme() {
			set_window_theme(L"");
			return TRUE;
		}

		BOOL set_explorer_theme() {
			set_window_theme(L"Explorer");
			return TRUE;
		}

		std::basic_string<TCHAR> get_title_text() const {
			TTGETTITLE title = { sizeof(TTGETTITLE) };
			get_title(&title);
			return title.pszTitle ? std::basic_string<TCHAR>(title.pszTitle) : std::basic_string<TCHAR>();
		}

		int get_title_icon_type() const {
			TTGETTITLE title = { sizeof(TTGETTITLE) };
			get_title(&title);
			return title.uTitleBitmap;
		}

		BOOL has_title() const {
			return !get_title_text().empty();
		}

		void force_update() {
			update();
			InvalidateRect(m_handle, NULL, FALSE);
		}

		BOOL add_tool_with_callback(HWND hWnd, UINT_PTR nIDTool = 0) {
			return add_tool(hWnd, LPSTR_TEXTCALLBACK, NULL, nIDTool);
		}

		int get_tool_index(HWND hWnd, UINT_PTR nIDTool = 0) const {
			int count = get_tool_count();
			for (int i = 0; i < count; i++) {
				TOOLINFO ti = { sizeof(TOOLINFO) };
				if (enum_tools(i, &ti)) {
					if (ti.hwnd == hWnd && ti.uId == nIDTool) {
						return i;
					}
				}
			}
			return -1;
		}

		BOOL has_tools() const {
			return get_tool_count() > 0;
		}

		void adjust_for_window_rect(HWND hWnd, LPRECT lpRect) {
			GetWindowRect(hWnd, lpRect);
			adjust_rect(lpRect, TRUE);
		}

		SIZE calculate_bubble_size_for_text(HWND hWnd, LPCTSTR text) {
			tool_info_t ti(0, hWnd, 0, NULL, (LPTSTR)text);
			return get_bubble_size(&ti);
		}
	};
}

#endif //__TOOLTIP_HPP__