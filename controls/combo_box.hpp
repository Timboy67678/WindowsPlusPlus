#ifndef __COMBO_BOX_HPP__
#define __COMBO_BOX_HPP__

namespace wpp
{
	class combo_box : public control {
	public:
		using control::control;

		class iterator {
		private:
			const combo_box* m_combo;
			int m_index;

		public:
			iterator(const combo_box* combo, int index) : m_combo(combo), m_index(index) {}

			tstring operator*() const {
				return m_combo->get_item_text(m_index);
			}

			iterator& operator++() {
				++m_index;
				return *this;
			}

			iterator operator++(int) {
				iterator tmp = *this;
				++m_index;
				return tmp;
			}

			bool operator==(const iterator& other) const {
				return m_combo == other.m_combo && m_index == other.m_index;
			}

			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}

			int index() const { return m_index; }
		};

		iterator begin() const { return iterator(this, 0); }
		iterator end() const { return iterator(this, get_count()); }

		combo_box& on_sel_change(command_callback callback) { register_command_callback(CBN_SELCHANGE, std::move(callback)); return *this; }
		combo_box& on_sel_end_ok(command_callback callback) { register_command_callback(CBN_SELENDOK, std::move(callback)); return *this; }
		combo_box& on_sel_end_cancel(command_callback callback) { register_command_callback(CBN_SELENDCANCEL, std::move(callback)); return *this; }
		combo_box& on_dropdown(command_callback callback) { register_command_callback(CBN_DROPDOWN, std::move(callback)); return *this; }
		combo_box& on_close_up(command_callback callback) { register_command_callback(CBN_CLOSEUP, std::move(callback)); return *this; }
		combo_box& on_edit_change(command_callback callback) { register_command_callback(CBN_EDITCHANGE, std::move(callback)); return *this; }
		combo_box& on_edit_update(command_callback callback) { register_command_callback(CBN_EDITUPDATE, std::move(callback)); return *this; }

		int get_count() const {
			return ComboBox_GetCount(m_handle);
		}

		int get_cur_sel() const {
			return ComboBox_GetCurSel(m_handle);
		}

		int set_cur_sel(int index) {
			return ComboBox_SetCurSel(m_handle, index);
		}

		LCID get_locale() {
			return (LCID)SendMessage(m_handle, CB_GETLOCALE, 0, 0L);
		}

		LCID set_locale(LCID locale) {
			return (LCID)SendMessage(m_handle, CB_SETLOCALE, (WPARAM)locale, 0L);
		}

		int get_top_index() {
			return (int)SendMessage(m_handle, CB_GETTOPINDEX, 0, 0L);
		}

		int set_top_index(int index) {
			return (int)SendMessage(m_handle, CB_SETTOPINDEX, index, 0L);
		}

		UINT get_horizontal_extent() {
			return (UINT)SendMessage(m_handle, CB_GETHORIZONTALEXTENT, 0, 0L);
		}

		void set_horizontal_extent(UINT extent) {
			SendMessage(m_handle, CB_SETHORIZONTALEXTENT, extent, 0L);
		}

		int get_dropped_width() const {
			return (int)SendMessage(m_handle, CB_GETDROPPEDWIDTH, 0, 0L);
		}

		int set_dropped_width(UINT width) {
			return (int)SendMessage(m_handle, CB_SETDROPPEDWIDTH, width, 0L);
		}

		BOOL get_info(PCOMBOBOXINFO combobox_info) const {
			return (BOOL)SendMessage(m_handle, CB_GETCOMBOBOXINFO, 0, (LPARAM)combobox_info);
		}

		DWORD get_edit_sel() const {
			return (DWORD)SendMessage(m_handle, CB_GETEDITSEL, 0, 0L);
		}

		BOOL set_edit_sel(int start_index, int end_index) {
			return (BOOL)SendMessage(m_handle, CB_SETEDITSEL, 0, MAKELONG(start_index, end_index));
		}

		DWORD_PTR get_item_data(int index) const {
			return (DWORD_PTR)SendMessage(m_handle, CB_GETITEMDATA, index, 0L);
		}

		int set_item_data(int index, DWORD_PTR item_data) const {
			return (int)SendMessage(m_handle, CB_SETITEMDATA, index, (LPARAM)item_data);
		}

		template<typename T>
		T* get_item_data_t(int index) const {
			return reinterpret_cast<T*>(get_item_data(index));
		}

		template<typename T>
		int set_item_data_t(int index, T* data) const {
			return set_item_data(index, reinterpret_cast<DWORD_PTR>(data));
		}

		int get_list_text(int index, LPTSTR string) const {
			return (int)SendMessage(m_handle, CB_GETLBTEXT, index, (LPARAM)string);
		}

		int get_list_text_length(int index) const {
			return (int)SendMessage(m_handle, CB_GETLBTEXTLEN, index, 0L);
		}

		tstring get_selected_text() const {
			int sel = get_cur_sel();
			return sel >= 0 ? get_item_text(sel) : tstring();
		}

		int get_item_height(int index) {
			return (int)SendMessage(m_handle, CB_GETITEMHEIGHT, index, 0L);
		}

		int set_item_height(int index, UINT item_height) {
			return (int)SendMessage(m_handle, CB_SETITEMHEIGHT, index, MAKELONG(item_height, 0));
		}

		BOOL get_extended_ui() {
			return (BOOL)SendMessage(m_handle, CB_GETEXTENDEDUI, 0, 0L);
		}

		int set_extended_ui(BOOL extended = TRUE) {
			return (int)SendMessage(m_handle, CB_SETEXTENDEDUI, extended, 0L);
		}

		void get_dropped_rect(LPRECT lprect) const {
			SendMessage(m_handle, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect);
		}

		BOOL get_dropped_state() const {
			return (BOOL)SendMessage(m_handle, CB_GETDROPPEDSTATE, 0, 0L);
		}

		int get_min_visible() const {
			return (int)SendMessage(m_handle, CB_GETMINVISIBLE, 0, 0L);
		}

		BOOL set_min_visible(int min_vis) {
			return (BOOL)SendMessage(m_handle, CB_SETMINVISIBLE, min_vis, 0L);
		}

		BOOL get_banner_text(LPWSTR lpwText, int cchText) const {
			return (BOOL)SendMessage(m_handle, CB_GETCUEBANNER, (WPARAM)lpwText, cchText);
		}

		BOOL set_banner_text(LPCWSTR lpcwText) {
			return (BOOL)SendMessage(m_handle, CB_SETCUEBANNER, 0, (LPARAM)lpcwText);
		}

		int init_storage(int item_count, UINT byte_count) {
			return (int)SendMessage(m_handle, CB_INITSTORAGE, (WPARAM)item_count, byte_count);
		}

		void reset_content() {
			SendMessage(m_handle, CB_RESETCONTENT, 0, 0L);
		}

		BOOL limit_text(int max_char) {
			return (BOOL)SendMessage(m_handle, CB_LIMITTEXT, max_char, 0L);
		}

		void show_dropdown(BOOL show_dropdown = TRUE) {
			SendMessage(m_handle, CB_SHOWDROPDOWN, show_dropdown, 0L);
		}

		int add(LPCTSTR lpszString) {
			int index = (int)SendMessage(m_handle, CB_ADDSTRING, 0, (LPARAM)lpszString);
			if (index == 0)
				SendMessage(m_handle, CB_SETCURSEL, 0, 0);
			return index;
		}

		int remove(UINT index) {
			return (int)SendMessage(m_handle, CB_DELETESTRING, index, 0L);
		}

		int insert_string(int index, LPCTSTR lpszString) {
			return (int)SendMessage(m_handle, CB_INSERTSTRING, index, (LPARAM)lpszString);
		}

		int add_dir(UINT attr, LPCTSTR lpszWildCard) {
			return (int)SendMessage(m_handle, CB_DIR, attr, (LPARAM)lpszWildCard);
		}

		int find_string(int start_index, LPCTSTR lpszString) const {
			return (int)SendMessage(m_handle, CB_FINDSTRING, start_index, (LPARAM)lpszString);
		}

		int find_string_exact(int start_index, LPCTSTR lpszFind) const {
			return (int)SendMessage(m_handle, CB_FINDSTRINGEXACT, start_index, (LPARAM)lpszFind);
		}

		int select_string(int start_index, LPCTSTR lpszString) {
			return (int)SendMessage(m_handle, CB_SELECTSTRING, start_index, (LPARAM)lpszString);
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
			return get_count() == 0;
		}

		BOOL is_valid_index(int index) const {
			return index >= 0 && index < get_count();
		}

		int get_last_index() const {
			return get_count() - 1;
		}

		BOOL has_selection() const {
			return get_cur_sel() >= 0;
		}

		void clear_selection() {
			set_cur_sel(-1);
		}

		tstring get_item_text(int index) const {
			int len = get_list_text_length(index);
			if (len <= 0) return tstring();

			std::vector<TCHAR> buffer(len + 1);
			get_list_text(index, buffer.data());
			return std::basic_string<TCHAR>(buffer.data());
		}

		tstring get_selected_item_text() const {
			int sel = get_cur_sel();
			return sel >= 0 ? get_item_text(sel) : tstring();
		}

		DWORD_PTR get_selected_data() const {
			int sel = get_cur_sel();
			return sel >= 0 ? get_item_data(sel) : 0;
		}

		template<typename T>
		T* get_selected_data() const {
			int sel = get_cur_sel();
			return sel >= 0 ? get_item_data_t<T>(sel) : nullptr;
		}

		std::vector<tstring> get_all_items() const {
			std::vector<tstring> items;
			int count = get_count();
			for (int i = 0; i < count; i++) {
				items.push_back(get_item_text(i));
			}
			return items;
		}

		int add_items(const std::vector<tstring>& items) {
			int lastIndex = CB_ERR;
			for (const auto& item : items) {
				lastIndex = add(item.c_str());
			}
			return lastIndex;
		}

		int add_items(const TCHAR** items, int count) {
			int lastIndex = CB_ERR;
			for (int i = 0; i < count; i++) {
				lastIndex = add(items[i]);
			}
			return lastIndex;
		}

		BOOL select_first() {
			return get_count() > 0 ? set_cur_sel(0) >= 0 : FALSE;
		}

		BOOL select_last() {
			int count = get_count();
			return count > 0 ? set_cur_sel(count - 1) >= 0 : FALSE;
		}

		BOOL select_next() {
			int current = get_cur_sel();
			int count = get_count();
			if (current >= 0 && current < count - 1) {
				return set_cur_sel(current + 1) >= 0;
			}
			return FALSE;
		}

		BOOL select_prev() {
			int current = get_cur_sel();
			if (current > 0) {
				return set_cur_sel(current - 1) >= 0;
			}
			return FALSE;
		}

		BOOL remove_selected() {
			int sel = get_cur_sel();
			if (sel >= 0) {
				return remove(sel) >= 0;
			}
			return FALSE;
		}

		int find_by_data(DWORD_PTR data) const {
			int count = get_count();
			for (int i = 0; i < count; i++) {
				if (get_item_data(i) == data) {
					return i;
				}
			}
			return CB_ERR;
		}

		template<typename T>
		int find_by_data(T* data) const {
			return find_by_data(reinterpret_cast<DWORD_PTR>(data));
		}

		int find_first(LPCTSTR text) const {
			return find_string(-1, text);
		}

		int find_first_exact(LPCTSTR text) const {
			return find_string_exact(-1, text);
		}

		BOOL item_exists(LPCTSTR text) const {
			return find_first_exact(text) != CB_ERR;
		}

		int append_unique(LPCTSTR text) {
			if (!item_exists(text)) {
				return add(text);
			}
			return find_first_exact(text);
		}

		BOOL select_item_by_text(LPCTSTR text, BOOL exactMatch = TRUE) {
			int index = exactMatch ? find_first_exact(text) : find_first(text);
			if (index != CB_ERR) {
				return set_cur_sel(index) >= 0;
			}
			return FALSE;
		}

		BOOL select_item_by_data(DWORD_PTR data) {
			int index = find_by_data(data);
			if (index != CB_ERR) {
				return set_cur_sel(index) >= 0;
			}
			return FALSE;
		}

		template<typename T>
		BOOL select_item_by_data(T* data) {
			return select_item_by_data(reinterpret_cast<DWORD_PTR>(data));
		}

		BOOL is_dropdown() const {
			return (get_style() & CBS_DROPDOWN) == CBS_DROPDOWN;
		}

		BOOL is_dropdownlist() const {
			return (get_style() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST;
		}

		BOOL is_simple() const {
			return (get_style() & CBS_SIMPLE) == CBS_SIMPLE;
		}

		BOOL is_sorted() const {
			return (get_style() & CBS_SORT) == CBS_SORT;
		}

		BOOL has_edit_control() const {
			return is_dropdown() || is_simple();
		}

		void open_dropdown() {
			show_dropdown(TRUE);
		}

		void close_dropdown() {
			show_dropdown(FALSE);
		}

		void toggle_dropdown() {
			show_dropdown(!get_dropped_state());
		}

		BOOL is_dropdown_open() const {
			return get_dropped_state();
		}

		tstring get_edit_text() const {
			if (!has_edit_control()) return tstring();
			return get_text();
		}

		void set_edit_text(LPCTSTR text) {
			set_text(text);
		}

		BOOL select_all_edit_text() {
			int len = get_text_length();
			return set_edit_sel(0, len);
		}

		void clear_edit_text() {
			if (has_edit_control()) {
				set_text(TEXT(""));
			}
		}

		void replace_item(int index, LPCTSTR newText) {
			if (is_valid_index(index)) {
				DWORD_PTR data = get_item_data(index);
				BOOL selected = (get_cur_sel() == index);

				remove(index);
				insert_string(index, newText);
				set_item_data(index, data);

				if (selected) {
					set_cur_sel(index);
				}
			}
		}

		BOOL swap_items(int index1, int index2) {
			if (!is_valid_index(index1) || !is_valid_index(index2) || index1 == index2) {
				return FALSE;
			}

			std::basic_string<TCHAR> text1 = get_item_text(index1);
			std::basic_string<TCHAR> text2 = get_item_text(index2);
			DWORD_PTR data1 = get_item_data(index1);
			DWORD_PTR data2 = get_item_data(index2);

			remove(index1);
			insert_string(index1, text2.c_str());
			set_item_data(index1, data2);

			remove(index2);
			insert_string(index2, text1.c_str());
			set_item_data(index2, data1);

			return TRUE;
		}

		BOOL move_item_up(int index) {
			if (index > 0 && is_valid_index(index)) {
				return swap_items(index, index - 1);
			}
			return FALSE;
		}

		BOOL move_item_down(int index) {
			if (is_valid_index(index) && index < get_count() - 1) {
				return swap_items(index, index + 1);
			}
			return FALSE;
		}

		void set_all_item_height(int height) {
			int count = get_count();
			for (int i = 0; i < count; i++) {
				set_item_height(i, height);
			}
		}

		int get_edit_selection_start() const {
			DWORD sel = get_edit_sel();
			return LOWORD(sel);
		}

		int get_edit_selection_end() const {
			DWORD sel = get_edit_sel();
			return HIWORD(sel);
		}

		BOOL has_edit_selection() const {
			DWORD sel = get_edit_sel();
			return LOWORD(sel) != HIWORD(sel);
		}

		tstring get_edit_selected_text() const {
			if (!has_edit_control() || !has_edit_selection()) {
				return tstring();
			}

			tstring fullText = get_edit_text();
			int start = get_edit_selection_start();
			int end = get_edit_selection_end();

			if (start >= 0 && end <= (int)fullText.length() && start < end) {
				return fullText.substr(start, end - start);
			}
			return tstring();
		}

		void ensure_visible(int index) {
			if (is_valid_index(index)) {
				set_top_index(index);
			}
		}

		RECT get_dropdown_rect() const {
			RECT rc = { 0 };
			get_dropped_rect(&rc);
			return rc;
		}

		SIZE get_dropdown_size() const {
			RECT rc = get_dropdown_rect();
			SIZE size = { rc.right - rc.left, rc.bottom - rc.top };
			return size;
		}

		BOOL set_dropdown_height(int height) {
			return set_min_visible(height / get_item_height(0));
		}

		int add_with_data(LPCTSTR text, DWORD_PTR data) {
			int index = add(text);
			if (index >= 0) {
				set_item_data(index, data);
			}
			return index;
		}

		template<typename T>
		int add_with_data(LPCTSTR text, T* data) {
			return add_with_data(text, reinterpret_cast<DWORD_PTR>(data));
		}

		int insert_with_data(int index, LPCTSTR text, DWORD_PTR data) {
			int newIndex = insert_string(index, text);
			if (newIndex >= 0) {
				set_item_data(newIndex, data);
			}
			return newIndex;
		}

		template<typename T>
		int insert_with_data(int index, LPCTSTR text, T* data) {
			return insert_with_data(index, text, reinterpret_cast<DWORD_PTR>(data));
		}

		void focus_edit() const {
			if (has_edit_control()) {
				COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
				if (get_info(&info) && info.hwndItem) {
					::SetFocus(info.hwndItem);
				}
			}
		}

		HWND get_edit_handle() const {
			COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
			if (get_info(const_cast<PCOMBOBOXINFO>(&info))) {
				return info.hwndItem;
			}
			return NULL;
		}

		HWND get_list_handle() const {
			COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
			if (get_info(const_cast<PCOMBOBOXINFO>(&info))) {
				return info.hwndList;
			}
			return NULL;
		}

		void select_and_open(int index) {
			if (is_valid_index(index)) {
				set_cur_sel(index);
				open_dropdown();
			}
		}

		int get_visible_item_count() const {
			int minVisible = get_min_visible();
			return minVisible > 0 ? minVisible : 30;
		}

		void for_each_item(std::function<void(int, const tstring&)> func) const {
			int count = get_count();
			for (int i = 0; i < count; i++) {
				func(i, get_item_text(i));
			}
		}

		void for_each_item_with_data(std::function<void(int, const tstring&, DWORD_PTR)> func) const {
			int count = get_count();
			for (int i = 0; i < count; i++) {
				func(i, get_item_text(i), get_item_data(i));
			}
		}

		template<typename Predicate>
		int find_if(Predicate pred) const {
			int count = get_count();
			for (int i = 0; i < count; i++) {
				if (pred(i, get_item_text(i))) {
					return i;
				}
			}
			return CB_ERR;
		}

		template<typename Predicate>
		bool remove_if(Predicate pred) {
			int count = get_count();
			bool removed = false;
			for (int i = count - 1; i >= 0; i--) {
				if (pred(i, get_item_text(i), get_item_data(i))) {
					remove(i);
					removed = true;
				}
			}
			return removed;
		}

		std::vector<int> find_all(LPCTSTR text, bool exact_match = false) const {
			std::vector<int> indices;
			int count = get_count();
			for (int i = 0; i < count; i++) {
				tstring item_text = get_item_text(i);
				bool match = exact_match ? (item_text == text) : (item_text.find(text) != tstring::npos);
				if (match) {
					indices.push_back(i);
				}
			}
			return indices;
		}

		void sort_items(bool ascending = true) {
			if (is_sorted()) return;

			std::vector<std::pair<tstring, DWORD_PTR>> items;
			int count = get_count();
			int selected = get_cur_sel();
			tstring selected_text = selected >= 0 ? get_item_text(selected) : tstring();

			for (int i = 0; i < count; i++) {
				items.push_back({ get_item_text(i), get_item_data(i) });
			}

			std::sort(items.begin(), items.end(),
					  [ascending](const auto& a, const auto& b) {
				return ascending ? (a.first < b.first) : (a.first > b.first);
			});

			reset_content();
			for (const auto& item : items) {
				int idx = add(item.first.c_str());
				set_item_data(idx, item.second);
			}

			if (!selected_text.empty()) {
				select_item_by_text(selected_text.c_str());
			}
		}

		bool add_unique(LPCTSTR text, bool select_if_exists = false) {
			int existing = find_first_exact(text);
			if (existing != CB_ERR) {
				if (select_if_exists) {
					set_cur_sel(existing);
				}
				return false;
			}
			add(text);
			return true;
		}

		void populate(const std::vector<tstring>& items, bool clear_first = true) {
			if (clear_first) {
				reset_content();
			}
			for (const auto& item : items) {
				add(item.c_str());
			}
		}

		template<typename T>
		void populate_with_data(const std::vector<std::pair<tstring, T*>>& items, bool clear_first = true) {
			if (clear_first) {
				reset_content();
			}
			for (const auto& item : items) {
				add_with_data(item.first.c_str(), item.second);
			}
		}

		int get_selected_index() const {
			return get_cur_sel();
		}

		bool set_selected_index(int index) {
			return set_cur_sel(index) >= 0;
		}

		std::pair<int, int> get_edit_selection() const {
			DWORD sel = get_edit_sel();
			return { LOWORD(sel), HIWORD(sel) };
		}

		int count() const {
			return get_count();
		}

		bool empty() const {
			return is_empty();
		}
	};

	class combo_box_ex : public control {
	public:
		using control::control;

		combo_box_ex& on_sel_change(command_callback callback) {
			register_command_callback(CBN_SELCHANGE, callback);
			return *this;
		}

		combo_box_ex& on_sel_end_ok(command_callback callback) {
			register_command_callback(CBN_SELENDOK, callback);
			return *this;
		}

		combo_box_ex& on_sel_end_cancel(command_callback callback) {
			register_command_callback(CBN_SELENDCANCEL, callback);
			return *this;
		}

		combo_box_ex& on_dropdown(command_callback callback) {
			register_command_callback(CBN_DROPDOWN, callback);
			return *this;
		}

		combo_box_ex& on_close_up(command_callback callback) {
			register_command_callback(CBN_CLOSEUP, callback);
			return *this;
		}

		combo_box_ex& on_edit_change(command_callback callback) {
			register_command_callback(CBN_EDITCHANGE, callback);
			return *this;
		}

		combo_box_ex& on_edit_update(command_callback callback) {
			register_command_callback(CBN_EDITUPDATE, callback);
			return *this;
		}

		combo_box_ex& on_begin_edit(notify_callback callback) {
			register_notify_callback(CBEN_BEGINEDIT, callback);
			return *this;
		}

		combo_box_ex& on_end_edit(notify_callback callback) {
			register_notify_callback(CBEN_ENDEDIT, callback);
			return *this;
		}

		combo_box_ex& on_insert_item(notify_callback callback) {
			register_notify_callback(CBEN_INSERTITEM, callback);
			return *this;
		}

		combo_box_ex& on_delete_item(notify_callback callback) {
			register_notify_callback(CBEN_DELETEITEM, callback);
			return *this;
		}

		combo_box_ex& on_get_disp_info(notify_callback callback) {
			register_notify_callback(CBEN_GETDISPINFO, callback);
			return *this;
		}

		image_list get_image_list() const {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, CBEM_GETIMAGELIST, 0, 0L) };
		}

		image_list set_image_list(HIMAGELIST hImageList) {
			return image_list{ (HIMAGELIST)SendMessage(m_handle, CBEM_SETIMAGELIST, 0, (LPARAM)hImageList) };
		}

		DWORD get_extended_style() const {
			return (DWORD)SendMessage(m_handle, CBEM_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD set_extended_style(DWORD dwExMask, DWORD dwExStyle) {
			return (DWORD)SendMessage(m_handle, CBEM_SETEXTENDEDSTYLE, dwExMask, dwExStyle);
		}

		BOOL get_unicode_format() const {
			return (BOOL)SendMessage(m_handle, CBEM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return (BOOL)SendMessage(m_handle, CBEM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		void set_window_theme(LPCWSTR lpstrTheme) {
			SendMessage(m_handle, CBEM_SETWINDOWTHEME, 0, (LPARAM)lpstrTheme);
		}

		int insert_item(const COMBOBOXEXITEM* lpcCBItem) {
			return (int)SendMessage(m_handle, CBEM_INSERTITEM, 0, (LPARAM)lpcCBItem);
		}

		int insert_item(UINT nMask, int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage,
						int iIndent, int iOverlay, LPARAM lParam) {
			COMBOBOXEXITEM cbex = { 0 };
			cbex.mask = nMask;
			cbex.iItem = nIndex;
			cbex.pszText = (LPTSTR)lpszItem;
			cbex.iImage = nImage;
			cbex.iSelectedImage = nSelImage;
			cbex.iIndent = iIndent;
			cbex.iOverlay = iOverlay;
			cbex.lParam = lParam;
			return (int)SendMessage(m_handle, CBEM_INSERTITEM, 0, (LPARAM)&cbex);
		}

		int insert_item(int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, LPARAM lParam = 0) {
			COMBOBOXEXITEM cbex = { 0 };
			cbex.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT | CBEIF_LPARAM;
			cbex.iItem = nIndex;
			cbex.pszText = (LPTSTR)lpszItem;
			cbex.iImage = nImage;
			cbex.iSelectedImage = nSelImage;
			cbex.iIndent = iIndent;
			cbex.lParam = lParam;
			return (int)SendMessage(m_handle, CBEM_INSERTITEM, 0, (LPARAM)&cbex);
		}

		int add_item(UINT nMask, LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, int iOverlay, LPARAM lParam) {
			return insert_item(nMask, -1, lpszItem, nImage, nSelImage, iIndent, iOverlay, lParam);
		}

		int add_item(LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, LPARAM lParam = 0) {
			return insert_item(-1, lpszItem, nImage, nSelImage, iIndent, lParam);
		}

		int delete_item(int nIndex) {
			return (int)SendMessage(m_handle, CBEM_DELETEITEM, nIndex, 0L);
		}

		BOOL get_item(PCOMBOBOXEXITEM pCBItem) const {
			return (BOOL)SendMessage(m_handle, CBEM_GETITEM, 0, (LPARAM)pCBItem);
		}

		BOOL set_item(const COMBOBOXEXITEM* lpcCBItem) {
			return (BOOL)SendMessage(m_handle, CBEM_SETITEM, 0, (LPARAM)lpcCBItem);
		}

		int set_item(int nIndex, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelImage,
					 int iIndent, int iOverlay, LPARAM lParam) {
			COMBOBOXEXITEM cbex = { 0 };
			cbex.mask = nMask;
			cbex.iItem = nIndex;
			cbex.pszText = (LPTSTR)lpszItem;
			cbex.iImage = nImage;
			cbex.iSelectedImage = nSelImage;
			cbex.iIndent = iIndent;
			cbex.iOverlay = iOverlay;
			cbex.lParam = lParam;
			return (int)SendMessage(m_handle, CBEM_SETITEM, 0, (LPARAM)&cbex);
		}

		BOOL get_item_text(int nIndex, LPTSTR lpszItem, int nLen) const {
			COMBOBOXEXITEM cbex = { 0 };
			cbex.mask = CBEIF_TEXT;
			cbex.iItem = nIndex;
			cbex.pszText = lpszItem;
			cbex.cchTextMax = nLen;

			return (BOOL)SendMessage(m_handle, CBEM_GETITEM, 0, (LPARAM)&cbex);
		}

		tstring get_item_text(int nIndex) const {
			TCHAR buffer[1024] = { 0 };
			if (get_item_text(nIndex, buffer, 1024)) {
				return tstring(buffer);
			}
			return tstring();
		}

		BOOL set_item_text(int nIndex, LPCTSTR lpszItem) {
			return set_item(nIndex, CBEIF_TEXT, lpszItem, 0, 0, 0, 0, 0);
		}

		combo_box get_combo_ctrl() {
			return combo_box{ (HWND)SendMessage(m_handle, CBEM_GETCOMBOCONTROL, 0, 0L) };
		}

		edit_text get_edit_ctrl() {
			return edit_text{ (HWND)SendMessage(m_handle, CBEM_GETEDITCONTROL, 0, 0L) };
		}

		BOOL has_edit_changed() {
			return (BOOL)SendMessage(m_handle, CBEM_HASEDITCHANGED, 0, 0L);
		}

		int get_count() {
			return get_combo_ctrl().get_count();
		}

		int get_cur_sel() {
			return get_combo_ctrl().get_cur_sel();
		}

		int set_cur_sel(int index) {
			return get_combo_ctrl().set_cur_sel(index);
		}

		void reset_content() {
			get_combo_ctrl().reset_content();
		}

		bool is_empty() {
			return get_count() == 0;
		}

		bool is_valid_index(int index) {
			return index >= 0 && index < get_count();
		}
	};
}

#endif //__COMBO_BOX_HPP__