#ifndef __EDIT_TEXT_HPP__
#define __EDIT_TEXT_HPP__

namespace wpp
{
	// Selection constants
	static constexpr LONG SELECT_ALL_START = 0;
	static constexpr LONG SELECT_ALL_END = -1;
	static constexpr LONG SELECT_NONE_START = -1;
	static constexpr LONG SELECT_NONE_END = 0;

	class edit_text : public control {
	public:
		using control::control;

		edit_text& on_change(command_callback callback) { register_command_callback(EN_CHANGE, std::move(callback)); return *this; }
		edit_text& on_errspace(command_callback callback) { register_command_callback(EN_ERRSPACE, std::move(callback)); return *this; }
		edit_text& on_maxtext(command_callback callback) { register_command_callback(EN_MAXTEXT, std::move(callback)); return *this; }
		edit_text& on_setfocus(command_callback callback) { register_command_callback(EN_SETFOCUS, std::move(callback)); return *this; }
		edit_text& on_killfocus(command_callback callback) { register_command_callback(EN_KILLFOCUS, std::move(callback)); return *this; }
		edit_text& on_update(command_callback callback) { register_command_callback(EN_UPDATE, std::move(callback)); return *this; }

		BOOL can_undo() {
			return Edit_CanUndo(m_handle);
		}

		int get_line_count() const {
			return Edit_GetLineCount(m_handle);
		}

		BOOL get_modify() const {
			return Edit_GetModify(m_handle);
		}

		void set_modify(BOOL bModified = TRUE) {
			Edit_SetModify(m_handle, bModified);
		}

		void get_rect(LPRECT lpRect) const {
			Edit_GetRect(m_handle, lpRect);
		}

		DWORD get_sel() const {
			return Edit_GetSel(m_handle);
		}

		void get_sel(int& nStartChar, int& nEndChar) const {
			SendMessage(m_handle, EM_GETSEL, (WPARAM)&nStartChar, (LPARAM)&nEndChar);
		}

		HLOCAL get_res_handle() const {
			return Edit_GetHandle(m_handle);
		}

		void set_res_handle(HLOCAL hBuffer) {
			Edit_SetHandle(m_handle, hBuffer);
		}

		DWORD get_margins() const {
			return (DWORD)SendMessage(m_handle, EM_GETMARGINS, 0, 0L);
		}

		void get_margins(UINT& nLeft, UINT& nRight) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, EM_GETMARGINS, 0, 0L);
			nLeft = LOWORD(dwRet);
			nRight = HIWORD(dwRet);
		}

		void set_margins(UINT nLeft, UINT nRight, WORD wFlags = EC_LEFTMARGIN | EC_RIGHTMARGIN) {
			SendMessage(m_handle, EM_SETMARGINS, wFlags, MAKELONG(nLeft, nRight));
		}

		UINT get_text_limit() const {
			return (UINT)SendMessage(m_handle, EM_GETLIMITTEXT, 0, 0L);
		}

		void set_text_limit(UINT nMax) {
			SendMessage(m_handle, EM_SETLIMITTEXT, nMax, 0L);
		}

		POINT pos_from_char(UINT nChar) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, EM_POSFROMCHAR, nChar, 0);
			POINT point = { GET_X_LPARAM(dwRet), GET_Y_LPARAM(dwRet) };
			return point;
		}

		int char_from_pos(POINT pt, int* pLine = NULL) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
			if (pLine != NULL)
				*pLine = (int)(short)HIWORD(dwRet);
			return (int)(short)LOWORD(dwRet);
		}

		int get_line(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const {
			return Edit_GetLine(m_handle, nIndex, lpszBuffer, nMaxLength);
		}

		TCHAR get_password_char() const {
			return Edit_GetPasswordChar(m_handle);
		}

		void set_password_char(TCHAR ch) {
			Edit_SetPasswordChar(m_handle, ch);
		}

		void set_using_password(BOOL bPassword, TCHAR pass_char = '*') {
			set_password_char(bPassword ? pass_char : 0);
		}

		EDITWORDBREAKPROC get_wordbreak_proc() const {
			return Edit_GetWordBreakProc(m_handle);
		}

		void set_wordbreak_proc(EDITWORDBREAKPROC ewbprc) {
			Edit_SetWordBreakProc(m_handle, ewbprc);
		}

		int get_first_visible_line() const {
			return Edit_GetFirstVisibleLine(m_handle);
		}

		int get_thumb() const {
			return (int)SendMessage(m_handle, EM_GETTHUMB, 0, 0L);
		}

		BOOL set_readonly(BOOL bReadOnly = TRUE) {
			return Edit_SetReadOnly(m_handle, bReadOnly);
		}

		UINT get_ime_status(UINT uStatus) const {
			return (UINT)SendMessage(m_handle, EM_GETIMESTATUS, uStatus, 0L);
		}

		UINT set_ime_status(UINT uStatus, UINT uData) {
			return (UINT)SendMessage(m_handle, EM_SETIMESTATUS, uStatus, uData);
		}

		BOOL get_banner_text(LPCWSTR lpstrText, int cchText) const {
			return Edit_GetCueBannerText(m_handle, lpstrText, cchText);
		}

		BOOL set_banner_text(LPCWSTR lpstrText, BOOL bKeepWithFocus = FALSE) {
			return Edit_SetCueBannerTextFocused(m_handle, lpstrText, bKeepWithFocus);
		}

		void empty_undo_buffer() {
			Edit_EmptyUndoBuffer(m_handle);
		}

		BOOL format_lines(BOOL bAddEOL) {
			Edit_FmtLines(m_handle, bAddEOL);
		}

		void limit_text(int nChars = 0) {
			Edit_LimitText(m_handle, nChars);
		}

		int line_from_char(int nIndex = -1) const {
			return Edit_LineFromChar(m_handle, nIndex);
		}

		int line_index(int nLine = -1) const {
			return Edit_LineIndex(m_handle, nLine);
		}

		int line_length(int nLine = -1) const {
			return Edit_LineLength(m_handle, nLine);
		}

		void line_scroll(int nLines, int nChars = 0) {
			Edit_Scroll(m_handle, nLines, nChars);
		}

		void replace_sel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE) {
			::SendMessage(m_handle, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText);
		}

		void set_rect(LPCRECT lpRect) {
			Edit_SetRect(m_handle, lpRect);
		}

		void set_rect_nopaint(LPCRECT lpRect) {
			Edit_SetRectNoPaint(m_handle, lpRect);
		}

		void set_sel(DWORD dwSelection, BOOL bNoScroll = FALSE) {
			Edit_SetSel(m_handle, LOWORD(dwSelection), HIWORD(dwSelection));
			if (!bNoScroll)
				Edit_ScrollCaret(m_handle);
		}

		void set_sel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE) {
			Edit_SetSel(m_handle, nStartChar, nEndChar);
			if (!bNoScroll)
				Edit_ScrollCaret(m_handle);
		}

		void set_sel_all(BOOL bNoScroll = FALSE) {
			set_sel(0, -1, bNoScroll);
		}

		void set_sel_none(BOOL bNoScroll = FALSE) {
			set_sel(-1, 0, bNoScroll);
		}

		void set_tab_stops(int nTabStops, LPINT rgTabStops) {
			Edit_SetTabStops(m_handle, nTabStops, rgTabStops);
		}

		void set_tab_stops() {
			Edit_SetTabStops(m_handle, 0, NULL);
		}

		void set_tab_stops(const int& cxEachStop) {
			Edit_SetTabStops(m_handle, 1, (LPINT)&cxEachStop);
		}

		void scroll_caret() {
			Edit_ScrollCaret(m_handle);
		}

		int scroll(int nScrollAction) {
			LRESULT lRet = SendMessage(m_handle, EM_SCROLL, nScrollAction, 0L);
			if (!(BOOL)HIWORD(lRet)) return -1;
			return (int)(short)LOWORD(lRet);
		}

		void insert_text(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE) {
			set_sel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
			replace_sel(lpstrText, bCanUndo);
		}

		void appent_text(LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE) {
			insert_text(get_text_length(), lpstrText, bNoScroll, bCanUndo);
		}

		void append_text(LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE) {
			insert_text(get_text_length(), lpstrText, bNoScroll, bCanUndo);
		}

		BOOL show_baloon_tip(PEDITBALLOONTIP pEditBaloonTip) {
			return Edit_ShowBalloonTip(m_handle, pEditBaloonTip);
		}

		BOOL hide_baloon_tip() {
			return Edit_HideBalloonTip(m_handle);
		}

		BOOL show_balloon_tip(PEDITBALLOONTIP pEditBalloonTip) {
			return Edit_ShowBalloonTip(m_handle, pEditBalloonTip);
		}

		BOOL hide_balloon_tip() {
			return Edit_HideBalloonTip(m_handle);
		}

		DWORD get_hilite() const {
			return Edit_GetHilite(m_handle);
		}

		void get_hilite(int& nStartChar, int& nEndChar) const {
			DWORD dwRet = (DWORD)get_hilite();
			nStartChar = (int)(short)LOWORD(dwRet);
			nEndChar = (int)(short)HIWORD(dwRet);
		}

		void set_hilite(int nStartChar, int nEndChar) {
			Edit_SetHilite(m_handle, nStartChar, nEndChar);
		}

		BOOL undo() {
			return Edit_Undo(m_handle);
		}

		void clear() {
			SendMessage(m_handle, WM_CLEAR, 0, 0L);
		}

		void copy() {
			SendMessage(m_handle, WM_COPY, 0, 0L);
		}

		void cut() {
			SendMessage(m_handle, WM_CUT, 0, 0L);
		}

		void paste() {
			SendMessage(m_handle, WM_PASTE, 0, 0L);
		}

		BOOL is_empty() const {
			return get_text_length() == 0;
		}

		BOOL is_multiline() const {
			return (get_style() & ES_MULTILINE) == ES_MULTILINE;
		}

		BOOL is_readonly() const {
			return (get_style() & ES_READONLY) == ES_READONLY;
		}

		BOOL is_password() const {
			return (get_style() & ES_PASSWORD) == ES_PASSWORD;
		}

		BOOL is_uppercase() const {
			return (get_style() & ES_UPPERCASE) == ES_UPPERCASE;
		}

		BOOL is_lowercase() const {
			return (get_style() & ES_LOWERCASE) == ES_LOWERCASE;
		}

		BOOL is_number() const {
			return (get_style() & ES_NUMBER) == ES_NUMBER;
		}

		BOOL is_autohscroll() const {
			return (get_style() & ES_AUTOHSCROLL) == ES_AUTOHSCROLL;
		}

		BOOL is_autovscroll() const {
			return (get_style() & ES_AUTOVSCROLL) == ES_AUTOVSCROLL;
		}

		BOOL is_center_aligned() const {
			return (get_style() & ES_CENTER) == ES_CENTER;
		}

		BOOL is_right_aligned() const {
			return (get_style() & ES_RIGHT) == ES_RIGHT;
		}

		BOOL is_left_aligned() const {
			return !is_center_aligned() && !is_right_aligned();
		}

		BOOL has_selection() const {
			int start = 0, end = 0;
			get_sel(start, end);
			return start != end;
		}

		int get_selection_start() const {
			int start = 0, end = 0;
			get_sel(start, end);
			return start;
		}

		int get_selection_end() const {
			int start = 0, end = 0;
			get_sel(start, end);
			return end;
		}

		int get_selection_length() const {
			int start = 0, end = 0;
			get_sel(start, end);
			return end - start;
		}

		tstring get_selected_text() const {
			int start = 0, end = 0;
			get_sel(start, end);

			if (start == end) return tstring();

			tstring fullText = get_text();
			if (start >= 0 && end <= (int)fullText.length() && start < end) {
				return fullText.substr(start, end - start);
			}
			return tstring();
		}

		void delete_selected() {
			replace_sel(TEXT(""), TRUE);
		}

		void select_word_at_caret() {
			int caretPos = get_selection_start();
			tstring text = get_text();
			
			if (caretPos < 0 || caretPos >= (int)text.length()) return;
			
			int start = caretPos;
			int end = caretPos;
			
			while (start > 0 && !_istspace(text[start - 1])) start--;
			while (end < (int)text.length() && !_istspace(text[end])) end++;
			
			set_sel(start, end);
		}

		void select_line_at_caret() {
			int caretPos = get_selection_start();
			int line = line_from_char(caretPos);
			int lineStart = line_index(line);
			int lineLen = line_length(lineStart);
			set_sel(lineStart, lineStart + lineLen);
		}

		tstring get_line_text(int line) const {
			if (line < 0 || line >= get_line_count()) {
				return tstring();
			}
			
			int len = line_length(line_index(line));
			if (len <= 0) return tstring();

			std::vector<TCHAR> buffer(len + 1);
			*(WORD*)buffer.data() = (WORD)len;
			int copied = get_line(line, buffer.data(), len);
			buffer[copied] = 0;
			return tstring(buffer.data());
		}

		std::vector<tstring> get_all_lines() const {
			std::vector<tstring> lines;
			int count = get_line_count();
			for (int i = 0; i < count; i++) {
				lines.push_back(get_line_text(i));
			}
			return lines;
		}

		int get_current_line() const {
			return line_from_char(-1);
		}

		tstring get_current_line_text() const {
			return get_line_text(get_current_line());
		}

		void goto_line(int line) {
			if (line < 0 || line >= get_line_count()) return;
			int charIndex = line_index(line);
			set_sel(charIndex, charIndex);
			scroll_caret();
		}

		void goto_end() {
			int len = get_text_length();
			set_sel(len, len);
			scroll_caret();
		}

		void goto_start() {
			set_sel(0, 0);
			scroll_caret();
		}

		void clear_all() {
			set_text(TEXT(""));
			set_modify(FALSE);
		}

		void delete_line(int line) {
			if (line < 0 || line >= get_line_count()) return;
			
			int lineStart = line_index(line);
			int lineLen = line_length(lineStart);
			
			set_sel(lineStart, lineStart + lineLen);
			if (line < get_line_count() - 1) {
				int nextLineStart = line_index(line + 1);
				set_sel(lineStart, nextLineStart);
			}
			replace_sel(TEXT(""), TRUE);
		}

		void delete_current_line() {
			delete_line(get_current_line());
		}

		int find_text(LPCTSTR searchText, int startPos = 0, BOOL caseSensitive = FALSE) const {
			tstring text = get_text();
			tstring search = searchText;

			if (!caseSensitive) {
				std::transform(text.begin(), text.end(), text.begin(), ::_totupper);
				std::transform(search.begin(), search.end(), search.begin(), ::_totupper);
			}
			
			size_t pos = text.find(search, startPos);
			return pos != tstring::npos ? (int)pos : -1;
		}

		BOOL find_and_select(LPCTSTR searchText, int startPos = 0, BOOL caseSensitive = FALSE) {
			int pos = find_text(searchText, startPos, caseSensitive);
			if (pos >= 0) {
				set_sel(pos, pos + (int)lstrlen(searchText));
				scroll_caret();
				return TRUE;
			}
			return FALSE;
		}

		int replace_text(LPCTSTR searchText, LPCTSTR replaceText, BOOL replaceAll = FALSE, BOOL caseSensitive = FALSE) {
			int count = 0;
			int startPos = 0;
			int searchLen = lstrlen(searchText);
			
			do {
				int pos = find_text(searchText, startPos, caseSensitive);
				if (pos < 0) break;
				
				set_sel(pos, pos + searchLen, TRUE);
				replace_sel(replaceText, TRUE);
				count++;
				
				startPos = pos + lstrlen(replaceText);
			} while (replaceAll);
			
			return count;
		}

		void set_text_ex(LPCTSTR text, BOOL preserveUndo = FALSE) {
			if (preserveUndo) {
				set_sel_all(TRUE);
				replace_sel(text, TRUE);
			} else {
				set_text(text);
			}
		}

		void append_line(LPCTSTR line) {
			if (!is_empty() && is_multiline()) {
				append_text(TEXT("\r\n"));
			}
			append_text(line);
		}

		void insert_line(int line, LPCTSTR text) {
			if (line < 0 || line > get_line_count()) return;
			
			int charIndex = line_index(line);
			if (charIndex < 0) charIndex = get_text_length();
			
			tstring insertText = text;
			if (line < get_line_count()) {
				insertText += TEXT("\r\n");
			} else if (!is_empty()) {
				insertText = tstring(TEXT("\r\n")) + insertText;
			}
			
			insert_text(charIndex, insertText.c_str(), FALSE, TRUE);
		}

		int get_char_at(int pos) const {
			if (pos < 0 || pos >= get_text_length()) return -1;
			tstring text = get_text();
			return text[pos];
		}

		int get_word_count() const {
			tstring text = get_text();
			int count = 0;
			BOOL inWord = FALSE;
			
			for (size_t i = 0; i < text.length(); i++) {
				if (_istspace(text[i])) {
					inWord = FALSE;
				} else if (!inWord) {
					inWord = TRUE;
					count++;
				}
			}
			return count;
		}

		void uppercase_selection() {
			tstring selected = get_selected_text();
			if (!selected.empty()) {
				std::transform(selected.begin(), selected.end(), selected.begin(), ::_totupper);
				replace_sel(selected.c_str(), TRUE);
			}
		}

		void lowercase_selection() {
			tstring selected = get_selected_text();
			if (!selected.empty()) {
				std::transform(selected.begin(), selected.end(), selected.begin(), ::_totlower);
				replace_sel(selected.c_str(), TRUE);
			}
		}

		void trim_whitespace() {
			tstring text = get_text();
			
			size_t start = text.find_first_not_of(TEXT(" \t\r\n"));
			size_t end = text.find_last_not_of(TEXT(" \t\r\n"));
			
			if (start == tstring::npos) {
				clear_all();
			} else {
				set_text(text.substr(start, end - start + 1).c_str());
			}
		}

		BOOL is_valid_number() const {
			if (!is_number()) return FALSE;
			
			tstring text = get_text();
			if (text.empty()) return FALSE;
			
			for (size_t i = 0; i < text.length(); i++) {
				if (!_istdigit(text[i]) && text[i] != TEXT('-') && text[i] != TEXT('+')) {
					return FALSE;
				}
			}
			return TRUE;
		}

		int get_number_value() const {
			tstring text = get_text();
			return text.empty() ? 0 : _ttoi(text.c_str());
		}

		void set_number_value(int value) {
			TCHAR buffer[32];
			_stprintf_s(buffer, 32, TEXT("%d"), value);
			set_text(buffer);
		}

		BOOL scroll_to_line(int line) {
			if (line < 0 || line >= get_line_count()) return FALSE;
			int current = get_first_visible_line();
			line_scroll(line - current, 0);
			return TRUE;
		}

		void scroll_to_top() {
			scroll_to_line(0);
		}

		void scroll_to_bottom() {
			scroll_to_line(get_line_count() - 1);
		}

		BOOL is_line_visible(int line) const {
			if (line < 0 || line >= get_line_count()) return FALSE;
			
			int firstVisible = get_first_visible_line();
			
			RECT rc;
			get_rect(&rc);
			int itemHeight = get_item_height(0);
			int visibleLines = itemHeight > 0 ? (rc.bottom - rc.top) / itemHeight : 1;
			
			return line >= firstVisible && line < firstVisible + visibleLines;
		}

		int get_item_height(int index = 0) const {
			HDC hdc = GetDC(m_handle);
			if (!hdc) return 16;
			
			HFONT hFont = (HFONT)SendMessage(m_handle, WM_GETFONT, 0, 0);
			HFONT hOldFont = hFont ? (HFONT)SelectObject(hdc, hFont) : NULL;
			
			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			int height = tm.tmHeight + tm.tmExternalLeading;
			
			if (hOldFont) SelectObject(hdc, hOldFont);
			ReleaseDC(m_handle, hdc);
			
			return height > 0 ? height : 16;
		}

		void ensure_line_visible(int line) {
			if (!is_line_visible(line)) {
				scroll_to_line(line);
			}
		}

		void duplicate_current_line() {
			int line = get_current_line();
			tstring lineText = get_line_text(line);
			int lineEnd = line_index(line) + line_length(line_index(line));
			insert_text(lineEnd, (TEXT("\r\n") + lineText).c_str(), FALSE, TRUE);
		}

		void move_line_up(int line) {
			if (line <= 0 || line >= get_line_count()) return;
			
			tstring currentLine = get_line_text(line);
			tstring prevLine = get_line_text(line - 1);
			
			delete_line(line);
			delete_line(line - 1);
			insert_line(line - 1, currentLine.c_str());
			insert_line(line, prevLine.c_str());
			
			goto_line(line - 1);
		}

		void move_line_down(int line) {
			if (line < 0 || line >= get_line_count() - 1) return;
			
			tstring currentLine = get_line_text(line);
			tstring nextLine = get_line_text(line + 1);
			
			delete_line(line + 1);
			delete_line(line);
			insert_line(line, nextLine.c_str());
			insert_line(line + 1, currentLine.c_str());
			
			goto_line(line + 1);
		}

		void indent_selection(LPCTSTR indentText = TEXT("\t")) {
			if (!has_selection() || !is_multiline()) return;
			
			int start = 0, end = 0;
			get_sel(start, end);
			
			int startLine = line_from_char(start);
			int endLine = line_from_char(end);
			
			tstring result;
			for (int i = startLine; i <= endLine; i++) {
				result += indentText;
				result += get_line_text(i);
				if (i < endLine) result += TEXT("\r\n");
			}
			
			int lineStart = line_index(startLine);
			int lineEnd = line_index(endLine) + line_length(line_index(endLine));
			
			set_sel(lineStart, lineEnd, TRUE);
			replace_sel(result.c_str(), TRUE);
		}

		void unindent_selection(LPCTSTR indentText = TEXT("\t")) {
			if (!has_selection() || !is_multiline()) return;
			
			int start = 0, end = 0;
			get_sel(start, end);
			
			int startLine = line_from_char(start);
			int endLine = line_from_char(end);
			
			int indentLen = lstrlen(indentText);
			tstring result;
			
			for (int i = startLine; i <= endLine; i++) {
				tstring line = get_line_text(i);
				if (line.length() >= (size_t)indentLen && 
				    line.substr(0, indentLen) == indentText) {
					result += line.substr(indentLen);
				} else {
					result += line;
				}
				if (i < endLine) result += TEXT("\r\n");
			}
			
			int lineStart = line_index(startLine);
			int lineEnd = line_index(endLine) + line_length(line_index(endLine));
			
			set_sel(lineStart, lineEnd, TRUE);
			replace_sel(result.c_str(), TRUE);
		}
	};

	class rich_edit_text : public control {
	public:
		using control::control;

		int get_line_count() const {
			return (int)SendMessage(m_handle, EM_GETLINECOUNT, 0, 0L);
		}

		int get_text_length() const {
			return (int)SendMessage(m_handle, WM_GETTEXTLENGTH, 0, 0L);
		}

		int get_text_length_ex(DWORD flags = GTL_DEFAULT, UINT code_page = CP_ACP) const {
			GETTEXTLENGTHEX gtle = { 0 };
			gtle.codepage = code_page;
			gtle.flags = flags;
			return (int)SendMessage(m_handle, EM_GETTEXTLENGTHEX, (WPARAM)&gtle, 0L);
		}

		int get_line(int index, LPTSTR buffer, int max_length) const {
			return Edit_GetLine(m_handle, index, buffer, max_length);
		}

		tstring get_line(int index) const {
			int length = line_length(line_index(index));
			if (length <= 0) return _T("");

			std::vector<TCHAR> buffer(length + 1);
			*(LPWORD)buffer.data() = (WORD)length;
			int chars = (int)SendMessage(m_handle, EM_GETLINE, index, (LPARAM)buffer.data());
			buffer[chars] = 0;
			return tstring(buffer.data());
		}

		tstring get_text() const {
			int length = get_text_length();
			if (length <= 0) return _T("");

			std::vector<TCHAR> buffer(length + 1);
			GetWindowText(m_handle, buffer.data(), length + 1);
			return tstring(buffer.data());
		}

		int get_text_ex(LPTSTR text, int text_len, DWORD flags = GT_DEFAULT, UINT code_page = CP_ACP,
						LPCSTR default_char = NULL, LPBOOL used_def_char = NULL) const {
			GETTEXTEX gte = { 0 };
			gte.cb = text_len * sizeof(TCHAR);
			gte.codepage = code_page;
			gte.flags = flags;
			gte.lpDefaultChar = default_char;
			gte.lpUsedDefChar = used_def_char;
			return (int)SendMessage(m_handle, EM_GETTEXTEX, (WPARAM)&gte, (LPARAM)text);
		}

		int set_text_ex(LPCTSTR text, DWORD flags = ST_DEFAULT, UINT code_page = CP_ACP) {
			SETTEXTEX ste = { 0 };
			ste.flags = flags;
			ste.codepage = code_page;
			return (int)SendMessage(m_handle, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)text);
		}

		void get_sel(LONG& start_char, LONG& end_char) const {
			CHARRANGE cr = { 0, 0 };
			SendMessage(m_handle, EM_EXGETSEL, 0, (LPARAM)&cr);
			start_char = cr.cpMin;
			end_char = cr.cpMax;
		}

		void get_sel(CHARRANGE& cr) const {
			SendMessage(m_handle, EM_EXGETSEL, 0, (LPARAM)&cr);
		}

		CHARRANGE get_sel() const {
			CHARRANGE cr = { 0, 0 };
			get_sel(cr);
			return cr;
		}

		int set_sel(LONG start_char, LONG end_char) {
			CHARRANGE cr = { start_char, end_char };
			return (int)SendMessage(m_handle, EM_EXSETSEL, 0, (LPARAM)&cr);
		}

		int set_sel(const CHARRANGE& cr) {
			return (int)SendMessage(m_handle, EM_EXSETSEL, 0, (LPARAM)&cr);
		}

		int select_all() {
			return set_sel(SELECT_ALL_START, SELECT_ALL_END);
		}

		int select_none() {
			return set_sel(SELECT_NONE_START, SELECT_NONE_END);
		}

		LONG get_sel_text(LPTSTR buffer) const {
			return (LONG)SendMessage(m_handle, EM_GETSELTEXT, 0, (LPARAM)buffer);
		}

		tstring get_sel_text() const {
			CHARRANGE cr = get_sel();
			int length = cr.cpMax - cr.cpMin;
			if (length <= 0) return _T("");

			std::vector<TCHAR> buffer(length + 1);
			SendMessage(m_handle, EM_GETSELTEXT, 0, (LPARAM)buffer.data());
			return tstring(buffer.data());
		}

		WORD get_selection_type() const {
			return (WORD)SendMessage(m_handle, EM_SELECTIONTYPE, 0, 0L);
		}

		bool has_selection() const {
			CHARRANGE cr = get_sel();
			return cr.cpMin != cr.cpMax;
		}

		void replace_sel(LPCTSTR new_text, bool can_undo = false) {
			SendMessage(m_handle, EM_REPLACESEL, (WPARAM)can_undo, (LPARAM)new_text);
		}

		void replace_sel(const tstring& new_text, bool can_undo = false) {
			replace_sel(new_text.c_str(), can_undo);
		}

		void hide_selection(bool hide = true, bool change_style = false) {
			SendMessage(m_handle, EM_HIDESELECTION, hide, change_style);
		}

		// ===== Text Range Operations =====

		int get_text_range(TEXTRANGE* text_range) const {
			return (int)SendMessage(m_handle, EM_GETTEXTRANGE, 0, (LPARAM)text_range);
		}

		int get_text_range(LONG start_char, LONG end_char, LPTSTR text) const {
			TEXTRANGE tr = { 0 };
			tr.chrg.cpMin = start_char;
			tr.chrg.cpMax = end_char;
			tr.lpstrText = text;
			return (int)SendMessage(m_handle, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
		}

		tstring get_text_range(LONG start_char, LONG end_char) const {
			if (start_char >= end_char) return _T("");

			std::vector<TCHAR> buffer(end_char - start_char + 1);
			TEXTRANGE tr = { 0 };
			tr.chrg.cpMin = start_char;
			tr.chrg.cpMax = end_char;
			tr.lpstrText = buffer.data();
			SendMessage(m_handle, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
			return tstring(buffer.data());
		}

		bool get_modify() const {
			return SendMessage(m_handle, EM_GETMODIFY, 0, 0L) != 0;
		}

		void set_modify(bool modified = true) {
			SendMessage(m_handle, EM_SETMODIFY, modified, 0L);
		}

		bool is_modified() const {
			return get_modify();
		}

		void clear_modified() {
			set_modify(false);
		}

		bool can_undo() const {
			return SendMessage(m_handle, EM_CANUNDO, 0, 0L) != 0;
		}

		bool can_redo() const {
			return SendMessage(m_handle, EM_CANREDO, 0, 0L) != 0;
		}

		bool undo() {
			return SendMessage(m_handle, EM_UNDO, 0, 0L) != 0;
		}

		bool redo() {
			return SendMessage(m_handle, EM_REDO, 0, 0L) != 0;
		}

		void empty_undo_buffer() {
			SendMessage(m_handle, EM_EMPTYUNDOBUFFER, 0, 0L);
		}

		UINT set_undo_limit(UINT undo_limit) {
			return (UINT)SendMessage(m_handle, EM_SETUNDOLIMIT, undo_limit, 0L);
		}

		UNDONAMEID get_undo_name() const {
			return (UNDONAMEID)SendMessage(m_handle, EM_GETUNDONAME, 0, 0L);
		}

		UNDONAMEID get_redo_name() const {
			return (UNDONAMEID)SendMessage(m_handle, EM_GETREDONAME, 0, 0L);
		}

		void stop_group_typing() {
			SendMessage(m_handle, EM_STOPGROUPTYPING, 0, 0L);
		}

		bool can_paste(UINT format = 0) const {
			return SendMessage(m_handle, EM_CANPASTE, format, 0L) != 0;
		}

		void clear() {
			SendMessage(m_handle, WM_CLEAR, 0, 0L);
		}

		void copy() {
			SendMessage(m_handle, WM_COPY, 0, 0L);
		}

		void cut() {
			SendMessage(m_handle, WM_CUT, 0, 0L);
		}

		void paste() {
			SendMessage(m_handle, WM_PASTE, 0, 0L);
		}

		void paste_special(UINT clip_format, DWORD aspect = 0, HMETAFILE h_mf = 0) {
			REPASTESPECIAL reps = { aspect, (DWORD_PTR)h_mf };
			SendMessage(m_handle, EM_PASTESPECIAL, clip_format, (LPARAM)&reps);
		}

		DWORD get_default_char_format(CHARFORMAT& cf) const {
			cf.cbSize = sizeof(CHARFORMAT);
			return (DWORD)SendMessage(m_handle, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
		}

		DWORD get_selection_char_format(CHARFORMAT& cf) const {
			cf.cbSize = sizeof(CHARFORMAT);
			return (DWORD)SendMessage(m_handle, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
		}

		bool set_char_format(CHARFORMAT& cf, WORD flags) {
			cf.cbSize = sizeof(CHARFORMAT);
			return SendMessage(m_handle, EM_SETCHARFORMAT, (WPARAM)flags, (LPARAM)&cf) != 0;
		}

		bool set_default_char_format(CHARFORMAT& cf) {
			cf.cbSize = sizeof(CHARFORMAT);
			return SendMessage(m_handle, EM_SETCHARFORMAT, 0, (LPARAM)&cf) != 0;
		}

		bool set_selection_char_format(CHARFORMAT& cf) {
			cf.cbSize = sizeof(CHARFORMAT);
			return SendMessage(m_handle, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf) != 0;
		}

		bool set_word_char_format(CHARFORMAT& cf) {
			cf.cbSize = sizeof(CHARFORMAT);
			return SendMessage(m_handle, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf) != 0;
		}

		DWORD get_default_char_format(CHARFORMAT2& cf) const {
			cf.cbSize = sizeof(CHARFORMAT2);
			return (DWORD)SendMessage(m_handle, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
		}

		DWORD get_selection_char_format(CHARFORMAT2& cf) const {
			cf.cbSize = sizeof(CHARFORMAT2);
			return (DWORD)SendMessage(m_handle, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
		}

		bool set_char_format(CHARFORMAT2& cf, WORD flags) {
			cf.cbSize = sizeof(CHARFORMAT2);
			return SendMessage(m_handle, EM_SETCHARFORMAT, (WPARAM)flags, (LPARAM)&cf) != 0;
		}

		bool set_default_char_format(CHARFORMAT2& cf) {
			cf.cbSize = sizeof(CHARFORMAT2);
			return SendMessage(m_handle, EM_SETCHARFORMAT, 0, (LPARAM)&cf) != 0;
		}

		bool set_selection_char_format(CHARFORMAT2& cf) {
			cf.cbSize = sizeof(CHARFORMAT2);
			return SendMessage(m_handle, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf) != 0;
		}

		bool set_word_char_format(CHARFORMAT2& cf) {
			cf.cbSize = sizeof(CHARFORMAT2);
			return SendMessage(m_handle, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf) != 0;
		}

		DWORD get_para_format(PARAFORMAT& pf) const {
			pf.cbSize = sizeof(PARAFORMAT);
			return (DWORD)SendMessage(m_handle, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
		}

		bool set_para_format(PARAFORMAT& pf) {
			pf.cbSize = sizeof(PARAFORMAT);
			return SendMessage(m_handle, EM_SETPARAFORMAT, 0, (LPARAM)&pf) != 0;
		}

		DWORD get_para_format(PARAFORMAT2& pf) const {
			pf.cbSize = sizeof(PARAFORMAT2);
			return (DWORD)SendMessage(m_handle, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
		}

		bool set_para_format(PARAFORMAT2& pf) {
			pf.cbSize = sizeof(PARAFORMAT2);
			return SendMessage(m_handle, EM_SETPARAFORMAT, 0, (LPARAM)&pf) != 0;
		}

		COLORREF set_background_color(COLORREF cr) {
			return (COLORREF)SendMessage(m_handle, EM_SETBKGNDCOLOR, 0, cr);
		}

		COLORREF set_background_color_system() {
			return (COLORREF)SendMessage(m_handle, EM_SETBKGNDCOLOR, 1, 0);
		}

		void get_rect(LPRECT rect) const {
			SendMessage(m_handle, EM_GETRECT, 0, (LPARAM)rect);
		}

		void set_rect(LPCRECT rect) {
			SendMessage(m_handle, EM_SETRECT, 0, (LPARAM)rect);
		}

		DWORD get_options() const {
			return (DWORD)SendMessage(m_handle, EM_GETOPTIONS, 0, 0L);
		}

		DWORD set_options(WORD operation, DWORD options) {
			return (DWORD)SendMessage(m_handle, EM_SETOPTIONS, operation, options);
		}

		DWORD get_event_mask() const {
			return (DWORD)SendMessage(m_handle, EM_GETEVENTMASK, 0, 0L);
		}

		DWORD set_event_mask(DWORD event_mask) {
			return (DWORD)SendMessage(m_handle, EM_SETEVENTMASK, 0, event_mask);
		}

		LONG get_limit_text() const {
			return (LONG)SendMessage(m_handle, EM_GETLIMITTEXT, 0, 0L);
		}

		void limit_text(LONG chars = 0) {
			SendMessage(m_handle, EM_EXLIMITTEXT, 0, chars);
		}

		bool set_read_only(bool read_only = true) {
			return SendMessage(m_handle, EM_SETREADONLY, read_only, 0L) != 0;
		}

		bool get_auto_url_detect() const {
			return SendMessage(m_handle, EM_GETAUTOURLDETECT, 0, 0L) != 0;
		}

		bool set_auto_url_detect(bool auto_detect = true) {
			return !SendMessage(m_handle, EM_AUTOURLDETECT, auto_detect, 0L);
		}

		TEXTMODE get_text_mode() const {
			return (TEXTMODE)SendMessage(m_handle, EM_GETTEXTMODE, 0, 0L);
		}

		bool set_text_mode(TEXTMODE text_mode) {
			return !SendMessage(m_handle, EM_SETTEXTMODE, text_mode, 0L);
		}

		int get_edit_style() const {
			return (int)SendMessage(m_handle, EM_GETEDITSTYLE, 0, 0L);
		}

		int set_edit_style(int style, int mask = -1) {
			if (mask == -1) mask = style;
			return (int)SendMessage(m_handle, EM_SETEDITSTYLE, style, mask);
		}

		DWORD get_edit_style_ex() const {
			return (DWORD)SendMessage(m_handle, EM_GETEDITSTYLEEX, 0, 0L);
		}

		DWORD set_edit_style_ex(DWORD style_ex, DWORD mask) {
			return (DWORD)SendMessage(m_handle, EM_SETEDITSTYLEEX, style_ex, mask);
		}

		int get_first_visible_line() const {
			return (int)SendMessage(m_handle, EM_GETFIRSTVISIBLELINE, 0, 0L);
		}

		void get_scroll_pos(LPPOINT point) const {
			SendMessage(m_handle, EM_GETSCROLLPOS, 0, (LPARAM)point);
		}

		void set_scroll_pos(LPPOINT point) {
			SendMessage(m_handle, EM_SETSCROLLPOS, 0, (LPARAM)point);
		}

		POINT get_scroll_pos() const {
			POINT pt = { 0, 0 };
			get_scroll_pos(&pt);
			return pt;
		}

		void set_scroll_pos(const POINT& pt) {
			set_scroll_pos(const_cast<LPPOINT>(&pt));
		}

		bool line_scroll(int lines) {
			return SendMessage(m_handle, EM_LINESCROLL, 0, lines) != 0;
		}

		void scroll_caret() {
			SendMessage(m_handle, EM_SCROLLCARET, 0, 0L);
		}

		void show_scroll_bar(int bar_type, bool visible = true) {
			SendMessage(m_handle, EM_SHOWSCROLLBAR, bar_type, visible);
		}

		bool get_zoom(int& numerator, int& denominator) const {
			return SendMessage(m_handle, EM_GETZOOM, (WPARAM)&numerator, (LPARAM)&denominator) != 0;
		}

		bool set_zoom(int numerator, int denominator) {
			return SendMessage(m_handle, EM_SETZOOM, numerator, denominator) != 0;
		}

		bool set_zoom_off() {
			return SendMessage(m_handle, EM_SETZOOM, 0, 0L) != 0;
		}

		int get_zoom_percent() const {
			int num = 0, den = 0;
			if (get_zoom(num, den) && den != 0) {
				return (num * 100) / den;
			}
			return 100;
		}

		bool set_zoom_percent(int percent) {
			if (percent <= 0) return set_zoom_off();
			return set_zoom(percent, 100);
		}

		bool set_font_size(int font_size_delta) {
			return SendMessage(m_handle, EM_SETFONTSIZE, font_size_delta, 0L) != 0;
		}

		void set_bold(bool bold = true) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_BOLD;
			cf.dwEffects = bold ? CFE_BOLD : 0;
			set_selection_char_format(cf);
		}

		void set_italic(bool italic = true) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_ITALIC;
			cf.dwEffects = italic ? CFE_ITALIC : 0;
			set_selection_char_format(cf);
		}

		void set_underline(bool underline = true) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_UNDERLINE;
			cf.dwEffects = underline ? CFE_UNDERLINE : 0;
			set_selection_char_format(cf);
		}

		void set_strikeout(bool strikeout = true) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_STRIKEOUT;
			cf.dwEffects = strikeout ? CFE_STRIKEOUT : 0;
			set_selection_char_format(cf);
		}

		void set_text_color(COLORREF color) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = color;
			set_selection_char_format(cf);
		}

		void set_font_name(const tstring& font_name) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_FACE;
			_tcsncpy_s(cf.szFaceName, font_name.c_str(), LF_FACESIZE - 1);
			set_selection_char_format(cf);
		}

		void set_font_size_points(int points) {
			CHARFORMAT2 cf = { 0 };
			cf.cbSize = sizeof(cf);
			cf.dwMask = CFM_SIZE;
			cf.yHeight = points * 20; // twips
			set_selection_char_format(cf);
		}

		int line_from_char(LONG index) const {
			return (int)SendMessage(m_handle, EM_EXLINEFROMCHAR, 0, index);
		}

		int line_index(int line = -1) const {
			return (int)SendMessage(m_handle, EM_LINEINDEX, line, 0L);
		}

		int line_length(int line = -1) const {
			return (int)SendMessage(m_handle, EM_LINELENGTH, line, 0L);
		}

		POINT pos_from_char(LONG char_pos) const {
			POINT point = { 0, 0 };
			SendMessage(m_handle, EM_POSFROMCHAR, (WPARAM)&point, char_pos);
			return point;
		}

		int char_from_pos(POINT pt) const {
			POINTL ptl = { pt.x, pt.y };
			return (int)SendMessage(m_handle, EM_CHARFROMPOS, 0, (LPARAM)&ptl);
		}

		int insert_text(LONG insert_after_char, LPCTSTR text, bool can_undo = false) {
			int ret = set_sel(insert_after_char, insert_after_char);
			replace_sel(text, can_undo);
			return ret;
		}

		int insert_text(LONG insert_after_char, const tstring& text, bool can_undo = false) {
			return insert_text(insert_after_char, text.c_str(), can_undo);
		}

		int append_text(LPCTSTR text, bool can_undo = false) {
			return insert_text(get_text_length(), text, can_undo);
		}

		int append_text(const tstring& text, bool can_undo = false) {
			return append_text(text.c_str(), can_undo);
		}

		void append_line(const tstring& text, bool can_undo = false) {
			append_text(L"\r\n" + text, can_undo);
		}

		LONG find_text(DWORD flags, FINDTEXT& ft) const {
#if defined(_UNICODE)
			return (LONG)SendMessage(m_handle, EM_FINDTEXTW, flags, (LPARAM)&ft);
#else
			return (LONG)SendMessage(m_handle, EM_FINDTEXT, flags, (LPARAM)&ft);
#endif
		}

		LONG find_text(DWORD flags, FINDTEXTEX& ft) const {
#if defined(_UNICODE)
			return (LONG)SendMessage(m_handle, EM_FINDTEXTEXW, flags, (LPARAM)&ft);
#else
			return (LONG)SendMessage(m_handle, EM_FINDTEXTEX, flags, (LPARAM)&ft);
#endif
		}

		LONG find_text(const tstring& search_text, LONG start = 0, LONG end = -1, DWORD flags = 0) const {
			FINDTEXTEX ft = { 0 };
			ft.chrg.cpMin = start;
			ft.chrg.cpMax = end;
			ft.lpstrText = const_cast<LPTSTR>(search_text.c_str());
			return find_text(flags, ft);
		}

		bool find_and_select(const tstring& search_text, LONG start = 0, DWORD flags = 0) {
			LONG pos = find_text(search_text, start, -1, flags);
			if (pos != -1) {
				set_sel(pos, pos + (LONG)search_text.length());
				return true;
			}
			return false;
		}

		DWORD find_word_break(int code, LONG start_char) {
			return (DWORD)SendMessage(m_handle, EM_FINDWORDBREAK, code, start_char);
		}

		void set_margins(UINT left, UINT right, WORD flags = EC_LEFTMARGIN | EC_RIGHTMARGIN) {
			SendMessage(m_handle, EM_SETMARGINS, flags, MAKELONG(left, right));
		}

		bool set_tab_stops(int tab_stops, LPINT rg_tab_stops) {
			return SendMessage(m_handle, EM_SETTABSTOPS, tab_stops, (LPARAM)rg_tab_stops) != 0;
		}

		bool set_tab_stops() {
			return SendMessage(m_handle, EM_SETTABSTOPS, 0, 0L) != 0;
		}

		bool set_tab_stops(const int& cx_each_stop) {
			return SendMessage(m_handle, EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cx_each_stop) != 0;
		}

		LONG format_range(FORMATRANGE& fr, bool display = true) {
			return (LONG)SendMessage(m_handle, EM_FORMATRANGE, display, (LPARAM)&fr);
		}

		LONG format_range(FORMATRANGE* format_range, bool display = true) {
			return (LONG)SendMessage(m_handle, EM_FORMATRANGE, display, (LPARAM)format_range);
		}

		bool display_band(LPRECT display_rect) {
			return SendMessage(m_handle, EM_DISPLAYBAND, 0, (LPARAM)display_rect) != 0;
		}

		void request_resize() {
			SendMessage(m_handle, EM_REQUESTRESIZE, 0, 0L);
		}

		bool set_target_device(HDC hdc, int cx_line_width) {
			return SendMessage(m_handle, EM_SETTARGETDEVICE, (WPARAM)hdc, cx_line_width) != 0;
		}

		LONG stream_in(UINT format, EDITSTREAM& es) {
			return (LONG)SendMessage(m_handle, EM_STREAMIN, format, (LPARAM)&es);
		}

		LONG stream_out(UINT format, EDITSTREAM& es) {
			return (LONG)SendMessage(m_handle, EM_STREAMOUT, format, (LPARAM)&es);
		}

		EDITWORDBREAKPROC get_word_break_proc() const {
			return (EDITWORDBREAKPROC)SendMessage(m_handle, EM_GETWORDBREAKPROC, 0, 0L);
		}

		void set_word_break_proc(EDITWORDBREAKPROC ewbprc) {
			SendMessage(m_handle, EM_SETWORDBREAKPROC, 0, (LPARAM)ewbprc);
		}

		IRichEditOle* get_ole_interface() const {
			IRichEditOle* rich_edit_ole = NULL;
			SendMessage(m_handle, EM_GETOLEINTERFACE, 0, (LPARAM)&rich_edit_ole);
			return rich_edit_ole;
		}

		bool set_ole_callback(IRichEditOleCallback* callback) {
			return SendMessage(m_handle, EM_SETOLECALLBACK, 0, (LPARAM)callback) != 0;
		}

		::AutoCorrectProc get_auto_correct_proc() const {
			return (::AutoCorrectProc)SendMessage(m_handle, EM_GETAUTOCORRECTPROC, 0, 0L);
		}

		bool set_auto_correct_proc(::AutoCorrectProc pfn) {
			return SendMessage(m_handle, EM_SETAUTOCORRECTPROC, (WPARAM)pfn, 0L) != 0;
		}

		bool call_auto_correct_proc(WCHAR ch) {
			return SendMessage(m_handle, EM_CALLAUTOCORRECTPROC, (WPARAM)ch, 0L) != 0;
		}

		DWORD get_story_type(int story_index) const {
			return (DWORD)SendMessage(m_handle, EM_GETSTORYTYPE, story_index, 0L);
		}

		DWORD set_story_type(int story_index, DWORD story_type) {
			return (DWORD)SendMessage(m_handle, EM_SETSTORYTYPE, story_index, story_type);
		}

		DWORD get_ellipsis_mode() const {
			DWORD mode = 0;
			SendMessage(m_handle, EM_GETELLIPSISMODE, 0, (LPARAM)&mode);
			return mode;
		}

		bool set_ellipsis_mode(DWORD ellipsis_mode) {
			return SendMessage(m_handle, EM_SETELLIPSISMODE, 0, ellipsis_mode) != 0;
		}

		bool get_ellipsis_state() const {
			return SendMessage(m_handle, EM_GETELLIPSISSTATE, 0, 0L) != 0;
		}

		bool get_touch_options(int touch_options) const {
			return SendMessage(m_handle, EM_GETTOUCHOPTIONS, touch_options, 0L) != 0;
		}

		void set_touch_options(int touch_options, bool enable) {
			SendMessage(m_handle, EM_SETTOUCHOPTIONS, touch_options, enable);
		}

		HRESULT insert_table(TABLEROWPARMS* row_params, TABLECELLPARMS* cell_params) {
			return (HRESULT)SendMessage(m_handle, EM_INSERTTABLE, (WPARAM)row_params, (LPARAM)cell_params);
		}

		HRESULT get_table_params(TABLEROWPARMS* row_params, TABLECELLPARMS* cell_params) const {
			return (HRESULT)SendMessage(m_handle, EM_GETTABLEPARMS, (WPARAM)row_params, (LPARAM)cell_params);
		}

		HRESULT set_table_params(TABLEROWPARMS* row_params, TABLECELLPARMS* cell_params) {
			return (HRESULT)SendMessage(m_handle, EM_SETTABLEPARMS, (WPARAM)row_params, (LPARAM)cell_params);
		}

		HRESULT insert_image(RICHEDIT_IMAGE_PARAMETERS* params) {
			return (HRESULT)SendMessage(m_handle, EM_INSERTIMAGE, 0, (LPARAM)params);
		}

		void set_palette(HPALETTE palette) {
			SendMessage(m_handle, EM_SETPALETTE, (WPARAM)palette, 0L);
		}

		bool set_uia_name(LPCTSTR name) {
			return SendMessage(m_handle, EM_SETUIANAME, 0, (LPARAM)name) != 0;
		}

		bool set_uia_name(const tstring& name) {
			return set_uia_name(name.c_str());
		}

		void clear_all() {
			select_all();
			clear();
		}

		std::vector<tstring> get_all_lines() const {
			std::vector<tstring> lines;
			int count = get_line_count();
			for (int i = 0; i < count; i++) {
				lines.push_back(get_line(i));
			}
			return lines;
		}

		void set_lines(const std::vector<tstring>& lines) {
			tstring text;
			for (size_t i = 0; i < lines.size(); i++) {
				text += lines[i];
				if (i < lines.size() - 1) {
					text += L"\r\n";
				}
			}
			set_text(text);
		}

		void replace_all(const tstring& find_text_needle, const tstring& replace_text, DWORD flags = 0) {
			LONG start = 0;
			while (true) {
				LONG pos = find_text(find_text_needle, start, -1, flags);
				if (pos == -1) break;

				set_sel(pos, pos + (LONG)find_text_needle.length());
				replace_sel(replace_text, true);
				start = pos + (LONG)replace_text.length();
			}
		}

		int get_selection_length() const {
			CHARRANGE cr = get_sel();
			return cr.cpMax - cr.cpMin;
		}

		bool is_empty() const {
			return get_text_length() == 0;
		}

		void goto_line(int line) {
			int pos = line_index(line);
			if (pos >= 0) {
				set_sel(pos, pos);
				scroll_caret();
			}
		}

		void goto_end() {
			int len = get_text_length();
			set_sel(len, len);
			scroll_caret();
		}

		void goto_start() {
			set_sel(0, 0);
			scroll_caret();
		}
	};
}

#endif //__EDIT_TEXT_HPP__