#ifndef __TOOLBAR_HPP__
#define __TOOLBAR_HPP__

namespace wpp
{
	class toolbar : public control {
	public:
		using control::control;

		toolbar& on_begin_drag(notify_callback callback) { register_notify_callback(TBN_BEGINDRAG, std::move(callback)); return *this; }
		toolbar& on_end_drag(notify_callback callback) { register_notify_callback(TBN_ENDDRAG, std::move(callback)); return *this; }
		toolbar& on_begin_adjust(notify_callback callback) { register_notify_callback(TBN_BEGINADJUST, std::move(callback)); return *this; }
		toolbar& on_end_adjust(notify_callback callback) { register_notify_callback(TBN_ENDADJUST, std::move(callback)); return *this; }
		toolbar& on_reset(notify_callback callback) { register_notify_callback(TBN_RESET, std::move(callback)); return *this; }
		toolbar& on_query_insert(notify_callback callback) { register_notify_callback(TBN_QUERYINSERT, std::move(callback)); return *this; }
		toolbar& on_query_delete(notify_callback callback) { register_notify_callback(TBN_QUERYDELETE, std::move(callback)); return *this; }
		toolbar& on_toolbar_change(notify_callback callback) { register_notify_callback(TBN_TOOLBARCHANGE, std::move(callback)); return *this; }
		toolbar& on_cust_help(notify_callback callback) { register_notify_callback(TBN_CUSTHELP, std::move(callback)); return *this; }
		toolbar& on_dropdown(notify_callback callback) { register_notify_callback(TBN_DROPDOWN, std::move(callback)); return *this; }
		toolbar& on_get_object(notify_callback callback) { register_notify_callback(TBN_GETOBJECT, std::move(callback)); return *this; }
		toolbar& on_hot_item_change(notify_callback callback) { register_notify_callback(TBN_HOTITEMCHANGE, std::move(callback)); return *this; }
		toolbar& on_drag_out(notify_callback callback) { register_notify_callback(TBN_DRAGOUT, std::move(callback)); return *this; }
		toolbar& on_delete_button(notify_callback callback) { register_notify_callback(TBN_DELETINGBUTTON, std::move(callback)); return *this; }
		toolbar& on_get_display_info(notify_callback callback) { register_notify_callback(TBN_GETDISPINFO, std::move(callback)); return *this; }
		toolbar& on_get_info_tip(notify_callback callback) { register_notify_callback(TBN_GETINFOTIP, std::move(callback)); return *this; }
		toolbar& on_get_button_info(notify_callback callback) { register_notify_callback(TBN_GETBUTTONINFO, std::move(callback)); return *this; }
		toolbar& on_restore(notify_callback callback) { register_notify_callback(TBN_RESTORE, std::move(callback)); return *this; }
		toolbar& on_save(notify_callback callback) { register_notify_callback(TBN_SAVE, std::move(callback)); return *this; }
		toolbar& on_init_customize(notify_callback callback) { register_notify_callback(TBN_INITCUSTOMIZE, std::move(callback)); return *this; }
		toolbar& on_wraphotitem(notify_callback callback) { register_notify_callback(TBN_WRAPHOTITEM, std::move(callback)); return *this; }
		toolbar& on_dupaccelerator(notify_callback callback) { register_notify_callback(TBN_DUPACCELERATOR, std::move(callback)); return *this; }
		toolbar& on_wrapaccelerator(notify_callback callback) { register_notify_callback(TBN_WRAPACCELERATOR, std::move(callback)); return *this; }
		toolbar& on_dragover(notify_callback callback) { register_notify_callback(TBN_DRAGOVER, std::move(callback)); return *this; }
		toolbar& on_map_accelerator(notify_callback callback) { register_notify_callback(TBN_MAPACCELERATOR, std::move(callback)); return *this; }

		BOOL is_button_enabled(int nID) const {
			return (BOOL)SendMessage(m_handle, TB_ISBUTTONENABLED, nID, 0L);
		}

		BOOL is_button_checked(int nID) const {
			return (BOOL)SendMessage(m_handle, TB_ISBUTTONCHECKED, nID, 0L);
		}

		BOOL is_button_pressed(int nID) const {
			return (BOOL)SendMessage(m_handle, TB_ISBUTTONPRESSED, nID, 0L);
		}

		BOOL is_button_hidden(int nID) const {
			return (BOOL)SendMessage(m_handle, TB_ISBUTTONHIDDEN, nID, 0L);
		}

		BOOL is_button_indeterminate(int nID) const {
			return (BOOL)SendMessage(m_handle, TB_ISBUTTONINDETERMINATE, nID, 0L);
		}

		int get_state(int nID) const {
			return (int)SendMessage(m_handle, TB_GETSTATE, nID, 0L);
		}

		BOOL set_state(int nID, UINT nState) {
			return (BOOL)SendMessage(m_handle, TB_SETSTATE, nID, MAKELPARAM(nState, 0));
		}

		BOOL get_button(int nIndex, LPTBBUTTON lpButton) const {
			return (BOOL)SendMessage(m_handle, TB_GETBUTTON, nIndex, (LPARAM)lpButton);
		}

		int get_button_count() const {
			return (int)SendMessage(m_handle, TB_BUTTONCOUNT, 0, 0L);
		}

		BOOL get_item_rect(int nIndex, LPRECT lpRect) const {
			return (BOOL)SendMessage(m_handle, TB_GETITEMRECT, nIndex, (LPARAM)lpRect);
		}

		void set_button_struct_size(int nSize = sizeof(TBBUTTON)) {
			SendMessage(m_handle, TB_BUTTONSTRUCTSIZE, nSize, 0L);
		}

		BOOL set_button_size(SIZE size) {
			return (BOOL)SendMessage(m_handle, TB_SETBUTTONSIZE, 0, MAKELPARAM(size.cx, size.cy));
		}

		BOOL set_button_size(int cx, int cy) {
			return (BOOL)SendMessage(m_handle, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy));
		}

		BOOL set_bitmap_size(SIZE size) {
			return (BOOL)SendMessage(m_handle, TB_SETBITMAPSIZE, 0, MAKELPARAM(size.cx, size.cy));
		}

		BOOL set_bitmap_size(int cx, int cy) {
			return (BOOL)SendMessage(m_handle, TB_SETBITMAPSIZE, 0, MAKELPARAM(cx, cy));
		}

		tooltip get_tool_tips() const {
			return tooltip((HWND)SendMessage(m_handle, TB_GETTOOLTIPS, 0, 0L));
		}

		void set_tool_tips(tooltip hWndToolTip) {
			SendMessage(m_handle, TB_SETTOOLTIPS, (WPARAM)hWndToolTip.get_handle(), 0L);
		}

		void set_notify_wnd(HWND hWnd) {
			SendMessage(m_handle, TB_SETPARENT, (WPARAM)hWnd, 0L);
		}

		int get_rows() const {
			return (int)SendMessage(m_handle, TB_GETROWS, 0, 0L);
		}

		void set_rows(int nRows, BOOL bLarger, LPRECT lpRect) {
			SendMessage(m_handle, TB_SETROWS, MAKELPARAM(nRows, bLarger), (LPARAM)lpRect);
		}

		BOOL set_cmd_id(int nIndex, UINT nID) {
			return (BOOL)SendMessage(m_handle, TB_SETCMDID, nIndex, nID);
		}

		DWORD get_bitmap_flags() const {
			return (DWORD)SendMessage(m_handle, TB_GETBITMAPFLAGS, 0, 0L);
		}

		int get_bitmap(int nID) const {
			return (int)SendMessage(m_handle, TB_GETBITMAP, nID, 0L);
		}

		int get_button_text(int nID, LPTSTR lpstrText) const {
			return (int)SendMessage(m_handle, TB_GETBUTTONTEXT, nID, (LPARAM)lpstrText);
		}

		image_list get_image_list(int nIndex = 0) const {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TB_GETIMAGELIST, nIndex, 0L) };
		}

		image_list set_image_list(HIMAGELIST hImageList, int nIndex = 0) {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TB_SETIMAGELIST, nIndex, (LPARAM)hImageList) };
		}

		image_list get_disabled_image_list(int nIndex = 0) const {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TB_GETDISABLEDIMAGELIST, nIndex, 0L) };
		}

		image_list set_disabled_image_list(HIMAGELIST hImageList, int nIndex = 0) {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TB_SETDISABLEDIMAGELIST, nIndex, (LPARAM)hImageList) };
		}

		image_list get_hot_image_list(int nIndex = 0) const {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TB_GETHOTIMAGELIST, nIndex, 0L) };
		}

		image_list set_hot_image_list(HIMAGELIST hImageList, int nIndex = 0) {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, TB_SETHOTIMAGELIST, nIndex, (LPARAM)hImageList) };
		}

		DWORD get_style() const {
			return send_message<DWORD>(TB_GETSTYLE, 0, 0L);
		}

		void set_style_ex(DWORD dwStyle) {
			send_message(TB_SETSTYLE, 0, dwStyle);
		}

		DWORD get_button_size() const {
			return send_message<DWORD>(TB_GETBUTTONSIZE, 0, 0L);
		}

		void get_button_size(SIZE& size) const {
			DWORD dwRet = send_message<DWORD>(TB_GETBUTTONSIZE, 0, 0L);
			size.cx = LOWORD(dwRet);
			size.cy = HIWORD(dwRet);
		}

		BOOL get_rect(int nID, LPRECT lpRect) const {
			return send_message<BOOL>(TB_GETRECT, nID, (LPARAM)lpRect);
		}

		int get_text_rows() const {
			return send_message<int>(TB_GETTEXTROWS, 0, 0L);
		}

		BOOL set_button_width(int cxMin, int cxMax) {
			return send_message<BOOL>(TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax));
		}

		BOOL set_indent(int nIndent) {
			return send_message<BOOL>(TB_SETINDENT, nIndent, 0L);
		}

		BOOL set_max_text_rows(int nMaxTextRows) {
			return send_message<BOOL>(TB_SETMAXTEXTROWS, nMaxTextRows, 0L);
		}

		BOOL get_anchor_highlight() const {
			return send_message<BOOL>(TB_GETANCHORHIGHLIGHT, 0, 0L);
		}

		BOOL set_anchor_highlight(BOOL bEnable = TRUE) {
			return send_message<BOOL>(TB_SETANCHORHIGHLIGHT, bEnable, 0L);
		}

		int get_button_info(int nID, LPTBBUTTONINFO lptbbi) const {
			return send_message<int>(TB_GETBUTTONINFO, nID, (LPARAM)lptbbi);
		}

		BOOL set_button_info(int nID, LPTBBUTTONINFO lptbbi) {
			return send_message<BOOL>(TB_SETBUTTONINFO, nID, (LPARAM)lptbbi);
		}

		BOOL set_button_info(int nID, DWORD dwMask, BYTE Style, BYTE State, LPCTSTR lpszItem,
							 int iImage, WORD cx, int iCommand, DWORD_PTR lParam) {
			TBBUTTONINFO tbbi = { 0 };
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = dwMask;
			tbbi.idCommand = iCommand;
			tbbi.iImage = iImage;
			tbbi.fsState = State;
			tbbi.fsStyle = Style;
			tbbi.cx = cx;
			tbbi.pszText = (LPTSTR)lpszItem;
			tbbi.lParam = lParam;
			return send_message<BOOL>(TB_SETBUTTONINFO, nID, (LPARAM)&tbbi);
		}

		int get_hot_item() const {
			return send_message<int>(TB_GETHOTITEM, 0, 0L);
		}

		int set_hot_item(int nItem) {
			return send_message<int>(TB_SETHOTITEM, nItem, 0L);
		}

		BOOL is_button_highlighted(int nButtonID) const {
			return send_message<BOOL>(TB_ISBUTTONHIGHLIGHTED, nButtonID, 0L);
		}

		DWORD set_draw_text_flags(DWORD dwMask, DWORD dwFlags) {
			return send_message<DWORD>(TB_SETDRAWTEXTFLAGS, dwMask, dwFlags);
		}

		BOOL get_color_scheme(LPCOLORSCHEME lpcs) const {
			return send_message<BOOL>(TB_GETCOLORSCHEME, 0, (LPARAM)lpcs);
		}

		void set_color_scheme(LPCOLORSCHEME lpcs) {
			send_message(TB_SETCOLORSCHEME, 0, (LPARAM)lpcs);
		}

		DWORD get_extended_style() const {
			return send_message<DWORD>(TB_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD set_extended_style(DWORD dwStyle) {
			return send_message<DWORD>(TB_SETEXTENDEDSTYLE, 0, dwStyle);
		}

		void get_insert_mark(LPTBINSERTMARK lptbim) const {
			send_message(TB_GETINSERTMARK, 0, (LPARAM)lptbim);
		}

		void set_insert_mark(LPTBINSERTMARK lptbim) {
			send_message(TB_SETINSERTMARK, 0, (LPARAM)lptbim);
		}

		COLORREF get_insert_mark_color() const {
			return send_message<COLORREF>(TB_GETINSERTMARKCOLOR, 0, 0L);
		}

		COLORREF set_insert_mark_color(COLORREF clr) {
			return send_message<COLORREF>(TB_SETINSERTMARKCOLOR, 0, (LPARAM)clr);
		}

		BOOL get_max_size(LPSIZE lpSize) const {
			return send_message<BOOL>(TB_GETMAXSIZE, 0, (LPARAM)lpSize);
		}

		void get_padding(LPSIZE lpSizePadding) const {
			DWORD dwRet = send_message<DWORD>(TB_GETPADDING, 0, 0L);
			lpSizePadding->cx = GET_X_LPARAM(dwRet);
			lpSizePadding->cy = GET_Y_LPARAM(dwRet);
		}

		void set_padding(int cx, int cy, LPSIZE lpSizePadding = NULL) {
			DWORD dwRet = send_message<DWORD>(TB_SETPADDING, 0, MAKELPARAM(cx, cy));
			if (lpSizePadding != NULL) {
				lpSizePadding->cx = GET_X_LPARAM(dwRet);
				lpSizePadding->cy = GET_Y_LPARAM(dwRet);
			}
		}

		BOOL get_unicode_format() const {
			return send_message<BOOL>(TB_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return send_message<BOOL>(TB_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int get_string(int nString, LPTSTR lpstrString, int cchMaxLen) const {
			return send_message<int>(TB_GETSTRING, MAKEWPARAM(cchMaxLen, nString), (LPARAM)lpstrString);
		}

		void get_metrics(LPTBMETRICS lptbm) const {
			send_message(TB_GETMETRICS, 0, (LPARAM)lptbm);
		}

		void set_metrics(LPTBMETRICS lptbm) {
			send_message(TB_SETMETRICS, 0, (LPARAM)lptbm);
		}

		void set_window_theme(LPCWSTR lpstrTheme) {
			send_message(TB_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
		}

		image_list get_pressed_image_list(int nIndex = 0) const {
			return image_list{ send_message<HIMAGELIST>(TB_GETPRESSEDIMAGELIST, nIndex, 0L) };
		}

		image_list set_pressed_image_list(HIMAGELIST hImageList, int nIndex = 0) {
			return image_list{ send_message<HIMAGELIST>(TB_SETPRESSEDIMAGELIST, nIndex, (LPARAM)hImageList) };
		}

		BOOL get_item_drop_down_rect(int nIndex, LPRECT lpRect) const {
			return send_message<BOOL>(TB_GETITEMDROPDOWNRECT, nIndex, (LPARAM)lpRect);
		}

		BOOL enable_button(int nID, BOOL bEnable = TRUE) {
			return send_message<BOOL>(TB_ENABLEBUTTON, nID, MAKELPARAM(bEnable, 0));
		}

		BOOL check_button(int nID, BOOL bCheck = TRUE) {
			return send_message<BOOL>(TB_CHECKBUTTON, nID, MAKELPARAM(bCheck, 0));
		}

		BOOL press_button(int nID, BOOL bPress = TRUE) {
			return send_message<BOOL>(TB_PRESSBUTTON, nID, MAKELPARAM(bPress, 0));
		}

		BOOL hide_button(int nID, BOOL bHide = TRUE) {
			return send_message<BOOL>(TB_HIDEBUTTON, nID, MAKELPARAM(bHide, 0));
		}

		BOOL indeterminate(int nID, BOOL bIndeterminate = TRUE) {
			return send_message<BOOL>(TB_INDETERMINATE, nID, MAKELPARAM(bIndeterminate, 0));
		}

		int add_bitmap(int nNumButtons, UINT nBitmapID) {
			TBADDBITMAP tbab = { 0 };
			tbab.hInst = GetModuleHandle(0);
			tbab.nID = nBitmapID;
			return send_message<int>(TB_ADDBITMAP, (WPARAM)nNumButtons, (LPARAM)&tbab);
		}

		int add_bitmap(int nNumButtons, HBITMAP hBitmap) {
			TBADDBITMAP tbab = { 0 };
			tbab.hInst = NULL;
			tbab.nID = (UINT_PTR)hBitmap;
			return send_message<int>(TB_ADDBITMAP, (WPARAM)nNumButtons, (LPARAM)&tbab);
		}

		BOOL add_buttons(int nNumButtons, LPTBBUTTON lpButtons) {
			return send_message<BOOL>(TB_ADDBUTTONS, nNumButtons, (LPARAM)lpButtons);
		}

		BOOL insert_button(int nIndex, LPTBBUTTON lpButton) {
			return send_message<BOOL>(TB_INSERTBUTTON, nIndex, (LPARAM)lpButton);
		}

		BOOL insert_button(int nIndex, int iCommand, BYTE Style, BYTE State, int iBitmap,
						   INT_PTR iString, DWORD_PTR lParam) {
			TBBUTTON tbb = { 0 };
			tbb.fsStyle = Style;
			tbb.fsState = State;
			tbb.idCommand = iCommand;
			tbb.iBitmap = iBitmap;
			tbb.iString = iString;
			tbb.dwData = lParam;
			return send_message<BOOL>(TB_INSERTBUTTON, nIndex, (LPARAM)&tbb);
		}

		BOOL insert_button(int nIndex, int iCommand, BYTE Style, BYTE State, int iBitmap,
						   LPCTSTR lpszItem, DWORD_PTR lParam) {
			return insert_button(nIndex, iCommand, Style, State, iBitmap, (INT_PTR)lpszItem, lParam);
		}

		BOOL add_button(LPTBBUTTON lpButton) {
			return insert_button(-1, lpButton);
		}

		BOOL add_button(int iCommand, BYTE Style, BYTE State, int iBitmap, INT_PTR iString, DWORD_PTR lParam) {
			return insert_button(-1, iCommand, Style, State, iBitmap, iString, lParam);
		}

		BOOL add_button(int iCommand, BYTE Style, BYTE State, int iBitmap, LPCTSTR lpszItem, DWORD_PTR lParam) {
			return insert_button(-1, iCommand, Style, State, iBitmap, lpszItem, lParam);
		}

		BOOL delete_button(int nIndex) {
			return send_message<BOOL>(TB_DELETEBUTTON, nIndex, 0L);
		}

		BOOL insert_separator(int nIndex, int cxWidth = 8) {
			return insert_button(nIndex, 0, BTNS_SEP, 0, cxWidth, (INT_PTR)0, 0);
		}

		BOOL add_separator(int cxWidth = 8) {
			return add_button(0, BTNS_SEP, 0, cxWidth, (INT_PTR)0, 0);
		}

		int command_to_index(UINT nID) const {
			return send_message<int>(TB_COMMANDTOINDEX, nID, 0L);
		}

		void save_state(HKEY hKeyRoot, LPCTSTR lpszSubKey, LPCTSTR lpszValueName) {
			TBSAVEPARAMS tbs = { 0 };
			tbs.hkr = hKeyRoot;
			tbs.pszSubKey = lpszSubKey;
			tbs.pszValueName = lpszValueName;
			send_message(TB_SAVERESTORE, (WPARAM)TRUE, (LPARAM)&tbs);
		}

		void restore_state(HKEY hKeyRoot, LPCTSTR lpszSubKey, LPCTSTR lpszValueName) {
			TBSAVEPARAMS tbs = { 0 };
			tbs.hkr = hKeyRoot;
			tbs.pszSubKey = lpszSubKey;
			tbs.pszValueName = lpszValueName;
			send_message(TB_SAVERESTORE, (WPARAM)FALSE, (LPARAM)&tbs);
		}

		void customize() {
			send_message(TB_CUSTOMIZE, 0, 0L);
		}

		int add_string(UINT nStringID) {
			return send_message<int>(TB_ADDSTRING, (WPARAM)GetModuleHandle(NULL), (LPARAM)nStringID);
		}

		int add_strings(LPCTSTR lpszStrings) {
			return send_message<int>(TB_ADDSTRING, 0, (LPARAM)lpszStrings);
		}

		void auto_size() {
			send_message(TB_AUTOSIZE, 0, 0L);
		}

		BOOL change_bitmap(int nID, int nBitmap) {
			return send_message<BOOL>(TB_CHANGEBITMAP, nID, MAKELPARAM(nBitmap, 0));
		}

		int load_images(int nBitmapID) {
			return send_message<int>(TB_LOADIMAGES, nBitmapID, (LPARAM)GetModuleHandle(NULL));
		}

		int load_std_images(int nBitmapID) {
			return send_message<int>(TB_LOADIMAGES, nBitmapID, (LPARAM)HINST_COMMCTRL);
		}

		BOOL replace_bitmap(LPTBREPLACEBITMAP ptbrb) {
			return send_message<BOOL>(TB_REPLACEBITMAP, 0, (LPARAM)ptbrb);
		}

		int hit_test(LPPOINT lpPoint) const {
			return send_message<int>(TB_HITTEST, 0, (LPARAM)lpPoint);
		}

		BOOL insert_mark_hit_test(LPPOINT lpPoint, LPTBINSERTMARK lptbim) const {
			return send_message<BOOL>(TB_INSERTMARKHITTEST, (WPARAM)lpPoint, (LPARAM)lptbim);
		}

		BOOL insert_mark_hit_test(int x, int y, LPTBINSERTMARK lptbim) const {
			POINT pt = { x, y };
			return send_message<BOOL>(TB_INSERTMARKHITTEST, (WPARAM)&pt, (LPARAM)lptbim);
		}

		BOOL map_accelerator(TCHAR chAccel, int& nID) const {
			return send_message<BOOL>(TB_MAPACCELERATOR, (WPARAM)chAccel, (LPARAM)&nID);
		}

		BOOL mark_button(int nID, BOOL bHighlight = TRUE) {
			return send_message<BOOL>(TB_MARKBUTTON, nID, MAKELPARAM(bHighlight, 0));
		}

		BOOL move_button(int nOldPos, int nNewPos) {
			return send_message<BOOL>(TB_MOVEBUTTON, nOldPos, nNewPos);
		}

		HRESULT get_object_from_iid(REFIID iid, LPVOID* ppvObject) {
			return send_message<HRESULT>(TB_GETOBJECT, (WPARAM)&iid, (LPARAM)ppvObject);
		}

		int get_count() const { return get_button_count(); }
		BOOL is_empty() const { return get_button_count() == 0; }
		BOOL is_valid_index(int nIndex) const { return nIndex >= 0 && nIndex < get_button_count(); }
		BOOL is_valid_button(int nID) const { return command_to_index(nID) >= 0; }

		tstring get_button_text_str(int nID) const {
			TCHAR buffer[256] = { 0 };
			get_button_text(nID, buffer);
			return tstring(buffer);
		}

		std::vector<int> get_all_button_ids() const {
			std::vector<int> ids;
			int count = get_button_count();
			for (int i = 0; i < count; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn)) {
					ids.push_back(btn.idCommand);
				}
			}
			return ids;
		}

		std::vector<int> get_enabled_buttons() const {
			std::vector<int> ids;
			int count = get_button_count();
			for (int i = 0; i < count; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn) && is_button_enabled(btn.idCommand)) {
					ids.push_back(btn.idCommand);
				}
			}
			return ids;
		}

		std::vector<int> get_checked_buttons() const {
			std::vector<int> ids;
			int count = get_button_count();
			for (int i = 0; i < count; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn) && is_button_checked(btn.idCommand)) {
					ids.push_back(btn.idCommand);
				}
			}
			return ids;
		}

		void enable_all_buttons(BOOL bEnable = TRUE) {
			std::vector<int> ids = get_all_button_ids();
			for (int id : ids) {
				enable_button(id, bEnable);
			}
		}

		void disable_all_buttons() { enable_all_buttons(FALSE); }

		void check_all_buttons(BOOL bCheck = TRUE) {
			std::vector<int> ids = get_all_button_ids();
			for (int id : ids) {
				check_button(id, bCheck);
			}
		}

		void uncheck_all_buttons() { check_all_buttons(FALSE); }

		int find_button_by_text(LPCTSTR lpszText, BOOL bExact = TRUE) const {
			int count = get_button_count();
			for (int i = 0; i < count; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn)) {
					tstring btnText = get_button_text_str(btn.idCommand);
					if (bExact) {
						if (lstrcmp(btnText.c_str(), lpszText) == 0) return btn.idCommand;
					} else {
						if (_tcsstr(btnText.c_str(), lpszText) != nullptr) return btn.idCommand;
					}
				}
			}
			return -1;
		}

		BOOL button_exists(int nID) const { return is_valid_button(nID); }

		BOOL toggle_button_check(int nID) {
			BOOL checked = is_button_checked(nID);
			return check_button(nID, !checked);
		}

		BOOL toggle_button_enabled(int nID) {
			BOOL enabled = is_button_enabled(nID);
			return enable_button(nID, !enabled);
		}

		int get_separator_count() const {
			int count = 0;
			int total = get_button_count();
			for (int i = 0; i < total; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn) && (btn.fsStyle & BTNS_SEP)) {
					count++;
				}
			}
			return count;
		}

		BOOL is_separator(int nIndex) const {
			TBBUTTON btn = { 0 };
			return get_button(nIndex, &btn) && (btn.fsStyle & BTNS_SEP);
		}

		RECT get_button_rect(int nID) const {
			RECT rc = { 0 };
			get_rect(nID, &rc);
			return rc;
		}

		RECT get_item_rect_ex(int nIndex) const {
			RECT rc = { 0 };
			get_item_rect(nIndex, &rc);
			return rc;
		}

		SIZE get_button_size_ex() const {
			SIZE size = { 0 };
			get_button_size(size);
			return size;
		}

		BOOL has_extended_style(DWORD dwStyle) const {
			return (get_extended_style() & dwStyle) == dwStyle;
		}

		void add_extended_style(DWORD dwStyle) {
			DWORD current = get_extended_style();
			set_extended_style(current | dwStyle);
		}

		void remove_extended_style(DWORD dwStyle) {
			DWORD current = get_extended_style();
			set_extended_style(current & ~dwStyle);
		}

		BOOL is_flat_style() const { return has_extended_style(TBSTYLE_FLAT); }
		BOOL is_transparent_style() const { return has_extended_style(TBSTYLE_TRANSPARENT); }
		BOOL is_list_style() const { return has_extended_style(TBSTYLE_LIST); }

		void enable_flat_style() { add_extended_style(TBSTYLE_FLAT); }
		void enable_transparent() { add_extended_style(TBSTYLE_TRANSPARENT); }
		void enable_list_style() { add_extended_style(TBSTYLE_LIST); }
		void enable_tooltips() { add_extended_style(TBSTYLE_TOOLTIPS); }

		BOOL add_standard_button(int nID, int nBitmapIndex, LPCTSTR lpszText = nullptr) {
			TBBUTTON btn = { 0 };
			btn.iBitmap = nBitmapIndex;
			btn.idCommand = nID;
			btn.fsState = TBSTATE_ENABLED;
			btn.fsStyle = BTNS_BUTTON;
			btn.iString = lpszText ? (INT_PTR)lpszText : -1;
			return add_button(&btn);
		}

		BOOL add_check_button(int nID, int nBitmapIndex, LPCTSTR lpszText = nullptr) {
			TBBUTTON btn = { 0 };
			btn.iBitmap = nBitmapIndex;
			btn.idCommand = nID;
			btn.fsState = TBSTATE_ENABLED;
			btn.fsStyle = BTNS_CHECK;
			btn.iString = lpszText ? (INT_PTR)lpszText : -1;
			return add_button(&btn);
		}

		BOOL add_dropdown_button(int nID, int nBitmapIndex, LPCTSTR lpszText = nullptr) {
			TBBUTTON btn = { 0 };
			btn.iBitmap = nBitmapIndex;
			btn.idCommand = nID;
			btn.fsState = TBSTATE_ENABLED;
			btn.fsStyle = BTNS_DROPDOWN;
			btn.iString = lpszText ? (INT_PTR)lpszText : -1;
			return add_button(&btn);
		}

		BOOL insert_standard_button(int nIndex, int nID, int nBitmapIndex, LPCTSTR lpszText = nullptr) {
			TBBUTTON btn = { 0 };
			btn.iBitmap = nBitmapIndex;
			btn.idCommand = nID;
			btn.fsState = TBSTATE_ENABLED;
			btn.fsStyle = BTNS_BUTTON;
			btn.iString = lpszText ? (INT_PTR)lpszText : -1;
			return insert_button(nIndex, &btn);
		}

		BOOL delete_all_buttons() {
			while (get_button_count() > 0) {
				if (!delete_button(0)) return FALSE;
			}
			return TRUE;
		}

		int get_visible_button_count() const {
			int count = 0;
			int total = get_button_count();
			for (int i = 0; i < total; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn) && !is_button_hidden(btn.idCommand)) {
					count++;
				}
			}
			return count;
		}

		void show_all_buttons() {
			std::vector<int> ids = get_all_button_ids();
			for (int id : ids) {
				hide_button(id, FALSE);
			}
		}

		void hide_all_buttons() {
			std::vector<int> ids = get_all_button_ids();
			for (int id : ids) {
				hide_button(id, TRUE);
			}
		}

		BOOL move_button_to_end(int nID) {
			int index = command_to_index(nID);
			if (index < 0) return FALSE;
			return move_button(index, get_button_count() - 1);
		}

		BOOL move_button_to_start(int nID) {
			int index = command_to_index(nID);
			if (index < 0) return FALSE;
			return move_button(index, 0);
		}

		BOOL swap_buttons(int nID1, int nID2) {
			int idx1 = command_to_index(nID1);
			int idx2 = command_to_index(nID2);
			if (idx1 < 0 || idx2 < 0) return FALSE;

			if (idx1 > idx2) {
				move_button(idx1, idx2);
				move_button(idx2 + 1, idx1);
			} else {
				move_button(idx2, idx1);
				move_button(idx1 + 1, idx2);
			}
			return TRUE;
		}

		int hit_test(POINT pt) const {
			return hit_test(&pt);
		}

		int hit_test(int x, int y) const {
			POINT pt = { x, y };
			return hit_test(&pt);
		}

		SIZE get_max_size_ex() const {
			SIZE size = { 0 };
			get_max_size(&size);
			return size;
		}

		SIZE get_padding_ex() const {
			SIZE size = { 0 };
			get_padding(&size);
			return size;
		}

		void set_default_padding() { set_padding(7, 7); }

		BOOL set_button_image(int nID, int nImageIndex) {
			TBBUTTONINFO tbbi = { 0 };
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = TBIF_IMAGE;
			tbbi.iImage = nImageIndex;
			return set_button_info(nID, &tbbi);
		}

		BOOL set_button_text(int nID, LPCTSTR lpszText) {
			TBBUTTONINFO tbbi = { 0 };
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = TBIF_TEXT;
			tbbi.pszText = (LPTSTR)lpszText;
			return set_button_info(nID, &tbbi);
		}

		BOOL set_button_style(int nID, BYTE style) {
			TBBUTTONINFO tbbi = { 0 };
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = TBIF_STYLE;
			tbbi.fsStyle = style;
			return set_button_info(nID, &tbbi);
		}

		BOOL is_button_dropdown(int nID) const {
			TBBUTTONINFO tbbi = { 0 };
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = TBIF_STYLE;
			if (get_button_info(nID, &tbbi) >= 0) {
				return (tbbi.fsStyle & BTNS_DROPDOWN) == BTNS_DROPDOWN;
			}
			return FALSE;
		}

		BOOL is_button_check_style(int nID) const {
			TBBUTTONINFO tbbi = { 0 };
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = TBIF_STYLE;
			if (get_button_info(nID, &tbbi) >= 0) {
				return (tbbi.fsStyle & BTNS_CHECK) == BTNS_CHECK;
			}
			return FALSE;
		}

		void set_theme_modern() { set_window_theme(L""); }
		void set_theme_explorer() { set_window_theme(L"Explorer"); }

		BOOL refresh_button(int nID) {
			int index = command_to_index(nID);
			if (index < 0) return FALSE;
			RECT rc;
			if (get_item_rect(index, &rc)) {
				::InvalidateRect(m_handle, &rc, TRUE);
				return TRUE;
			}
			return FALSE;
		}

		void refresh_all() {
			::InvalidateRect(m_handle, NULL, TRUE);
			::UpdateWindow(m_handle);
		}

		BOOL has_buttons() const { return !is_empty(); }

		void clear() { delete_all_buttons(); }

		int get_first_button_id() const {
			TBBUTTON btn = { 0 };
			return get_button(0, &btn) ? btn.idCommand : -1;
		}

		int get_last_button_id() const {
			int count = get_button_count();
			if (count == 0) return -1;
			TBBUTTON btn = { 0 };
			return get_button(count - 1, &btn) ? btn.idCommand : -1;
		}

		std::vector<tstring> get_all_button_texts() const {
			std::vector<tstring> texts;
			int count = get_button_count();
			for (int i = 0; i < count; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn)) {
					texts.push_back(get_button_text_str(btn.idCommand));
				}
			}
			return texts;
		}

		BOOL ensure_button_visible(int nID) {
			int index = command_to_index(nID);
			if (index < 0) return FALSE;
			return set_hot_item(index) >= 0;
		}

		int get_checked_count() const {
			int count = 0;
			int total = get_button_count();
			for (int i = 0; i < total; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn) && is_button_checked(btn.idCommand)) {
					count++;
				}
			}
			return count;
		}

		int get_enabled_count() const {
			int count = 0;
			int total = get_button_count();
			for (int i = 0; i < total; i++) {
				TBBUTTON btn = { 0 };
				if (get_button(i, &btn) && is_button_enabled(btn.idCommand)) {
					count++;
				}
			}
			return count;
		}

		BOOL show_button(int nID) { return hide_button(nID, FALSE); }

		BOOL is_button_visible(int nID) const { return !is_button_hidden(nID); }
	};
}

#endif //__TOOLBAR_HPP__