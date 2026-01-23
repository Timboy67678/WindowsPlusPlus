#ifndef __LIST_BOX_HPP__
#define __LIST_BOX_HPP__

namespace wpp
{
	class list_box : public control {
	public:
		using control::control;

		list_box& on_dbl_click(command_callback cb) { register_command_callback(LBN_DBLCLK, cb); return *this; }
		list_box& on_err_space(command_callback cb) { register_command_callback(LBN_ERRSPACE, cb); return *this; }
		list_box& on_kill_focus(command_callback cb) { register_command_callback(LBN_KILLFOCUS, cb); return *this; }
		list_box& on_sel_change(command_callback cb) { register_command_callback(LBN_SELCHANGE, cb); return *this; }
		list_box& on_sel_cancel(command_callback cb) { register_command_callback(LBN_SELCANCEL, cb); return *this; }
		list_box& on_set_focus(command_callback cb) { register_command_callback(LBN_SETFOCUS, cb); return *this; }

		int get_list_count() {
			return ListBox_GetCount(m_handle);
		}

		int get_horizon_extent() {
			return ListBox_GetHorizontalExtent(m_handle);
		}

		void set_horizon_extent(int extent) {
			ListBox_SetHorizontalExtent(m_handle, extent);
		}

		int get_top_index() const {
			return ListBox_GetTopIndex(m_handle);
		}

		int set_top_index(int top_index) {
			return ListBox_SetTopIndex(m_handle, top_index);
		}

		LCID get_locale() {
			return (LCID)SendMessage(m_handle, LB_GETLOCALE, 0, 0L);
		}

		LCID set_locale(LCID locale) {
			return (LCID)SendMessage(m_handle, LB_SETLOCALE, (WPARAM)locale, 0L);
		}

		DWORD get_info() {
			return (DWORD)SendMessage(m_handle, LB_GETLISTBOXINFO, 0, 0L);
		}

		int get_selected() const {
			return ListBox_GetCurSel(m_handle);
		}

		int set_selected(int index) {
			return ListBox_SetCurSel(m_handle, index);
		}

		int get_multi_sel(int index) {
			return ListBox_GetSel(m_handle, index);
		}

		int set_multi_sel(int index, BOOL selected = TRUE) {
			return ListBox_SetSel(m_handle, selected, index);
		}

		int get_multi_sel_count() const {
			return ListBox_GetSelCount(m_handle);
		}

		int get_select_items(int max_count, int* return_indexs) const {
			return ListBox_GetSelItems(m_handle, max_count, return_indexs);
		}

		int get_anchor_index() {
			return (int)SendMessage(m_handle, LB_GETANCHORINDEX, 0, 0L);
		}

		void set_anchor_index(int nIndex) {
			SendMessage(m_handle, LB_SETANCHORINDEX, nIndex, 0L);
		}

		int get_caret_index() {
			return ListBox_GetCaretIndex(m_handle);
		}

		int set_caret_index(int index, BOOL scroll_to = TRUE) {
			return (int)SendMessage(m_handle, LB_SETCARETINDEX, index, MAKELONG(scroll_to, 0));
		}

		DWORD_PTR get_item_data(int index) const {
			return ListBox_GetItemData(m_handle, index);
		}

		int set_item_data(int index, DWORD_PTR item_data) const {
			return ListBox_SetItemData(m_handle, index, item_data);
		}

		template<typename R>
		R* get_item_data_t(int index) const {
			return reinterpret_cast<R*>(get_item_data(index));
		}

		template<typename T>
		int set_item_data(int index, T* data) const {
			return set_item_data(index, reinterpret_cast<DWORD_PTR>(data));
		}

		int get_item_rect(int index, LPRECT rect_out) {
			return ListBox_GetItemRect(m_handle, index, rect_out);
		}

		int get_item_text_length(int index) {
			return ListBox_GetTextLen(m_handle, index);
		}

		int get_item_height(int index) const {
			return ListBox_GetItemHeight(m_handle, index);
		}

		int set_item_height(int index, int item_height) {
			return ListBox_SetItemHeight(m_handle, index, item_height);
		}

		void set_column_width(int width) {
			ListBox_SetColumnWidth(m_handle, width);
		}

		BOOL set_tab_stops(int tab_stop_num, int* tab_stops) {
			return ListBox_SetTabStops(m_handle, tab_stop_num, tab_stops);
		}

		BOOL set_tab_stops() {
			return ListBox_SetTabStops(m_handle, 0, 0L);
		}

		BOOL set_tab_stops(const int& each_step) {
			return ListBox_SetTabStops(m_handle, 1, &each_step);
		}

		int init_storage(int item_count, UINT storage_initial_bytesize) {
			return (int)SendMessage(m_handle, LB_INITSTORAGE, (WPARAM)item_count, (LPARAM)storage_initial_bytesize);
		}

		void reset_content() {
			ListBox_ResetContent(m_handle);
		}

		int item_from_point(POINT position, BOOL& is_outside) {
			DWORD dw = (DWORD)SendMessage(m_handle, LB_ITEMFROMPOINT, 0, MAKELPARAM(position.x, position.y));
			is_outside = (BOOL)HIWORD(dw);
			return (int)LOWORD(dw);
		}

		int remove(int index) {
			return ListBox_DeleteString(m_handle, index);
		}

		int get_item_text(int index, LPCTSTR buffer) {
			return ListBox_GetText(m_handle, index, buffer);
		}

		int add(LPCTSTR string) {
			return ListBox_AddString(m_handle, string);
		}

		int insert(int index, LPCTSTR string) {
			return ListBox_InsertString(m_handle, index, string);
		}

		int add_dir(int attribute, LPCTSTR file_spec) {
			return ListBox_Dir(m_handle, attribute, file_spec);
		}

		int add_file(LPCTSTR file_path) {
			return (int)SendMessage(m_handle, LB_ADDFILE, 0, (LPARAM)file_path);
		}

		int find_string(int start_index, LPCTSTR item_string) const {
			return (int)SendMessage(m_handle, LB_FINDSTRING, start_index, (LPARAM)item_string);
		}

		int find_string_exact(int start_index, LPCTSTR item_string) const {
			return (int)SendMessage(m_handle, LB_FINDSTRINGEXACT, start_index, (LPARAM)item_string);
		}

		int select_string(int start_index, LPCTSTR item_string) {
			return (int)SendMessage(m_handle, LB_SELECTSTRING, start_index, (LPARAM)item_string);
		}

		int select_item_range(BOOL bSelect, int first_item, int last_item) {
			return bSelect ?
				(int)SendMessage(m_handle, LB_SELITEMRANGEEX, first_item, last_item) :
				(int)SendMessage(m_handle, LB_SELITEMRANGEEX, last_item, first_item);
		}

		int get_count() const {
			return ListBox_GetCount(m_handle);
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

		std::tstring get_item_text(int index) const {
			int len = ListBox_GetTextLen(m_handle, index);
			if (len <= 0) return std::tstring();

			std::vector<TCHAR> buffer(len + 1);
			ListBox_GetText(m_handle, index, buffer.data());
			return std::basic_string<TCHAR>(buffer.data());
		}

		std::tstring get_selected_text() const {
			int sel = get_selected();
			return sel >= 0 ? get_item_text(sel) : std::tstring();
		}

		DWORD_PTR get_selected_data() const {
			int sel = get_selected();
			return sel >= 0 ? get_item_data(sel) : 0;
		}

		template<typename T>
		T* get_selected_data() const {
			int sel = get_selected();
			return sel >= 0 ? get_item_data_t<T>(sel) : nullptr;
		}

		std::vector<int> get_selected_items() const {
			std::vector<int> items;
			int count = get_multi_sel_count();
			if (count > 0) {
				items.resize(count);
				get_select_items(count, items.data());
			}
			return items;
		}

		std::vector<std::basic_string<TCHAR>> get_selected_texts() const {
			std::vector<std::basic_string<TCHAR>> texts;
			std::vector<int> indices = get_selected_items();
			for (int idx : indices) {
				texts.push_back(get_item_text(idx));
			}
			return texts;
		}

		std::vector<std::basic_string<TCHAR>> get_all_items() const {
			std::vector<std::basic_string<TCHAR>> items;
			int count = get_count();
			for (int i = 0; i < count; i++) {
				items.push_back(get_item_text(i));
			}
			return items;
		}

		BOOL has_selection() const {
			if (is_multi_select()) {
				return get_multi_sel_count() > 0;
			}
			return get_selected() >= 0;
		}

		void clear_selection() {
			if (is_multi_select()) {
				int count = get_count();
				for (int i = 0; i < count; i++) {
					set_multi_sel(i, FALSE);
				}
			} else {
				set_selected(-1);
			}
		}

		void select_all() {
			if (is_multi_select()) {
				int count = get_count();
				for (int i = 0; i < count; i++) {
					set_multi_sel(i, TRUE);
				}
			}
		}

		void invert_selection() {
			if (is_multi_select()) {
				int count = get_count();
				for (int i = 0; i < count; i++) {
					BOOL selected = get_multi_sel(i);
					set_multi_sel(i, !selected);
				}
			}
		}

		BOOL is_multi_select() const {
			LONG_PTR style = get_style();
			return (style & LBS_MULTIPLESEL) || (style & LBS_EXTENDEDSEL);
		}

		BOOL is_sorted() const {
			return (get_style() & LBS_SORT) == LBS_SORT;
		}

		BOOL has_strings() const {
			return (get_style() & LBS_HASSTRINGS) == LBS_HASSTRINGS;
		}

		int add_items(const std::vector<std::basic_string<TCHAR>>& items) {
			int lastIndex = -1;
			for (const auto& item : items) {
				lastIndex = add(item.c_str());
			}
			return lastIndex;
		}

		int add_items(const TCHAR** items, int count) {
			int lastIndex = -1;
			for (int i = 0; i < count; i++) {
				lastIndex = add(items[i]);
			}
			return lastIndex;
		}

		void clear() {
			reset_content();
		}

		BOOL remove_selected() {
			int sel = get_selected();
			if (sel >= 0) {
				return remove(sel) >= 0;
			}
			return FALSE;
		}

		int remove_selected_items() {
			std::vector<int> indices = get_selected_items();
			int removed = 0;
			for (int i = (int)indices.size() - 1; i >= 0; i--) {
				if (remove(indices[i]) >= 0) {
					removed++;
				}
			}
			return removed;
		}

		BOOL select_first() {
			return get_count() > 0 ? set_selected(0) >= 0 : FALSE;
		}

		BOOL select_last() {
			int count = get_count();
			return count > 0 ? set_selected(count - 1) >= 0 : FALSE;
		}

		BOOL select_next() {
			int current = get_selected();
			int count = get_count();
			if (current >= 0 && current < count - 1) {
				return set_selected(current + 1) >= 0;
			}
			return FALSE;
		}

		BOOL select_prev() {
			int current = get_selected();
			if (current > 0) {
				return set_selected(current - 1) >= 0;
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
			return LB_ERR;
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
			return find_first_exact(text) != LB_ERR;
		}

		BOOL ensure_visible(int index) {
			if (!is_valid_index(index)) return FALSE;

			int topIndex = get_top_index();
			int visibleCount = get_visible_count();

			if (index < topIndex) {
				return set_top_index(index) >= 0;
			} else if (index >= topIndex + visibleCount) {
				return set_top_index(index - visibleCount + 1) >= 0;
			}
			return TRUE;
		}

		int get_visible_count() const {
			RECT rc;
			GetClientRect(m_handle, &rc);
			int itemHeight = get_item_height(0);
			return itemHeight > 0 ? (rc.bottom - rc.top) / itemHeight : 0;
		}

		BOOL scroll_to_bottom() {
			int count = get_count();
			return count > 0 ? set_top_index(count - 1) >= 0 : FALSE;
		}

		BOOL scroll_to_top() {
			return set_top_index(0) >= 0;
		}

		BOOL swap_items(int index1, int index2) {
			if (!is_valid_index(index1) || !is_valid_index(index2) || index1 == index2) {
				return FALSE;
			}

			std::tstring text1 = get_item_text(index1);
			std::tstring text2 = get_item_text(index2);
			DWORD_PTR data1 = get_item_data(index1);
			DWORD_PTR data2 = get_item_data(index2);

			remove(index1);
			insert(index1, text2.c_str());
			set_item_data(index1, data2);

			remove(index2);
			insert(index2, text1.c_str());
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

		int get_total_height() const {
			int count = get_count();
			int total = 0;
			for (int i = 0; i < count; i++) {
				total += get_item_height(i);
			}
			return total;
		}

		BOOL is_item_visible(int index) const {
			if (!is_valid_index(index)) return FALSE;

			int topIndex = get_top_index();
			int visibleCount = get_visible_count();

			return index >= topIndex && index < topIndex + visibleCount;
		}

		int hit_test(POINT pt) const {
			BOOL outside = FALSE;
			int index = (int)SendMessage(m_handle, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
			outside = HIWORD(index);
			return outside ? LB_ERR : LOWORD(index);
		}

		int hit_test_client(int x, int y) const {
			POINT pt = { x, y };
			return hit_test(pt);
		}

		void replace_item(int index, LPCTSTR newText) {
			if (is_valid_index(index)) {
				DWORD_PTR data = get_item_data(index);
				BOOL selected = is_multi_select() ? get_multi_sel(index) : (get_selected() == index);

				remove(index);
				insert(index, newText);
				set_item_data(index, data);

				if (selected) {
					if (is_multi_select()) {
						set_multi_sel(index, TRUE);
					} else {
						set_selected(index);
					}
				}
			}
		}

		int append_unique(LPCTSTR text) {
			if (!item_exists(text)) {
				return add(text);
			}
			return find_first_exact(text);
		}
	};
}

#endif //__LIST_BOX_HPP__