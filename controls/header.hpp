#ifndef __HEADER_HPP__
#define __HEADER_HPP__

namespace wpp
{
	class header : public control {
	public:
		using control::control;

		header& on_begin_drag(notify_callback callback) { register_notify_callback(HDN_BEGINDRAG, callback); return *this; }
		header& on_begin_filter_edit(notify_callback callback) { register_notify_callback(HDN_BEGINFILTEREDIT, callback); return *this; }
		header& on_begin_track(notify_callback callback) { register_notify_callback(HDN_BEGINTRACK, callback); return *this; }
		header& on_divider_double_click(notify_callback callback) { register_notify_callback(HDN_DIVIDERDBLCLICK, callback); return *this; }
		header& on_drop_down(notify_callback callback) { register_notify_callback(HDN_DROPDOWN, callback); return *this; }
		header& on_end_drag(notify_callback callback) { register_notify_callback(HDN_ENDDRAG, callback); return *this; }
		header& on_end_filter_edit(notify_callback callback) { register_notify_callback(HDN_ENDFILTEREDIT, callback); return *this; }
		header& on_end_track(notify_callback callback) { register_notify_callback(HDN_ENDTRACK, callback); return *this; }
		header& on_filter_btn_click(notify_callback callback) { register_notify_callback(HDN_FILTERBTNCLICK, callback); return *this; }
		header& on_filter_change(notify_callback callback) { register_notify_callback(HDN_FILTERCHANGE, callback); return *this; }
		header& on_get_disp_info(notify_callback callback) { register_notify_callback(HDN_GETDISPINFO, callback); return *this; }
		header& on_item_changed(notify_callback callback) { register_notify_callback(HDN_ITEMCHANGED, callback); return *this; }
		header& on_item_changing(notify_callback callback) { register_notify_callback(HDN_ITEMCHANGING, callback); return *this; }
		header& on_item_click(notify_callback callback) { register_notify_callback(HDN_ITEMCLICK, callback); return *this; }
		header& on_item_keydown(notify_callback callback) { register_notify_callback(HDN_ITEMKEYDOWN, callback); return *this; }
		header& on_item_state_icon_click(notify_callback callback) { register_notify_callback(HDN_ITEMSTATEICONCLICK, callback); return *this; }
		header& on_overflow_click(notify_callback callback) { register_notify_callback(HDN_OVERFLOWCLICK, callback); return *this; }
		header& on_track(notify_callback callback) { register_notify_callback(HDN_TRACK, callback); return *this; }
		header& on_custom_draw(notify_callback callback) { register_notify_callback(NM_CUSTOMDRAW, callback); return *this; }
		header& on_rclick(notify_callback callback) { register_notify_callback(NM_RCLICK, callback); return *this; }
		header& on_released_capture(notify_callback callback) { register_notify_callback(NM_RELEASEDCAPTURE, callback); return *this; }

		int get_item_count() const {
			return Header_GetItemCount(m_handle);
		}

		BOOL get_item(int nIndex, LPHDITEM pHeaderItem) const {
			return Header_GetItem(m_handle, nIndex, pHeaderItem);
		}

		BOOL set_item(int nIndex, LPHDITEM pHeaderItem) {
			return Header_SetItem(m_handle, nIndex, pHeaderItem);
		}

		image_list get_image_list() const {
			return image_list{ Header_GetImageList(m_handle) };
		}

		image_list set_image_list(HIMAGELIST hImageList) {
			return image_list{ Header_SetImageList(m_handle, hImageList) };
		}

		BOOL get_order_array(int nSize, int* lpnArray) const {
			return Header_GetOrderArray(m_handle, nSize, lpnArray);
		}

		BOOL set_order_array(int nSize, int* lpnArray) {
			return Header_SetOrderArray(m_handle, nSize, lpnArray);
		}

		BOOL get_item_rect(int nIndex, LPRECT lpItemRect) const {
			return Header_GetItemRect(m_handle, nIndex, lpItemRect);
		}

		int set_hot_divider(BOOL bPos, DWORD dwInputValue) {
			return Header_SetHotDivider(m_handle, bPos, dwInputValue);
		}

		BOOL get_unicode_format() const {
			return Header_GetUnicodeFormat(m_handle);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return Header_SetUnicodeFormat(m_handle, bUnicode);
		}

		int get_bitmap_margin() const {
			return Header_GetBitmapMargin(m_handle);
		}

		int set_bitmap_margin(int nWidth) {
			return Header_SetBitmapMargin(m_handle, nWidth);
		}

		int set_filter_change_timeout(DWORD dwTimeOut) {
			return Header_SetFilterChangeTimeout(m_handle, dwTimeOut);
		}

		BOOL get_item_dropdown_rect(int nIndex, LPRECT lpRect) const {
			return Header_GetItemDropDownRect(m_handle, nIndex, lpRect);
		}

		BOOL get_overflow_rect(LPRECT lpRect) const {
			return Header_GetOverflowRect(m_handle, lpRect);
		}

		int get_focused_item() const {
			return Header_GetFocusedItem(m_handle);
		}

		BOOL set_focused_item(int nIndex) {
			return Header_SetFocusedItem(m_handle, nIndex);
		}

		int insert_item(int nIndex, LPHDITEM phdi) {
			return Header_InsertItem(m_handle, nIndex, phdi);
		}

		int add_item(LPHDITEM phdi) {
			return insert_item(get_item_count(), phdi);
		}

		BOOL delete_item(int nIndex) {
			return Header_DeleteItem(m_handle, nIndex);
		}

		BOOL get_layout(HD_LAYOUT* pHeaderLayout) {
			return Header_Layout(m_handle, pHeaderLayout);
		}

		int hit_test(LPHDHITTESTINFO lpHitTestInfo) const {
			return (int)SendMessage(m_handle, HDM_HITTEST, 0, (LPARAM)lpHitTestInfo);
		}

		int order_to_index(int nOrder) {
			return Header_OrderToIndex(m_handle, nOrder);
		}

		image_list create_drag_image(int nIndex) {
			return image_list{ Header_CreateDragImage(m_handle, nIndex) };
		}

		int edit_filter(int nColumn, BOOL bDiscardChanges) {
			return Header_EditFilter(m_handle, nColumn, bDiscardChanges);
		}

		int clear_filter(int nColumn) {
			return Header_ClearFilter(m_handle, nColumn);
		}

		int clear_all_filters() {
			return Header_ClearAllFilters(m_handle);
		}

		int get_count() const {
			return get_item_count();
		}

		BOOL is_empty() const {
			return get_item_count() == 0;
		}

		BOOL is_valid_index(int nIndex) const {
			return nIndex >= 0 && nIndex < get_item_count();
		}

		int get_last_index() const {
			return get_item_count() - 1;
		}

		std::tstring get_item_text(int nIndex) const {
			TCHAR buffer[256] = { 0 };
			HDITEM hdi = { 0 };
			hdi.mask = HDI_TEXT;
			hdi.pszText = buffer;
			hdi.cchTextMax = 256;

			if (get_item(nIndex, &hdi)) {
				return std::tstring(buffer);
			}
			return std::tstring();
		}

		BOOL set_item_text(int nIndex, LPCTSTR lpszText) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_TEXT;
			hdi.pszText = (LPTSTR)lpszText;
			return set_item(nIndex, &hdi);
		}

		int get_item_width(int nIndex) const {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_WIDTH;
			return get_item(nIndex, &hdi) ? hdi.cxy : 0;
		}

		BOOL set_item_width(int nIndex, int nWidth) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_WIDTH;
			hdi.cxy = nWidth;
			return set_item(nIndex, &hdi);
		}

		int get_item_format(int nIndex) const {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_FORMAT;
			return get_item(nIndex, &hdi) ? hdi.fmt : 0;
		}

		BOOL set_item_format(int nIndex, int nFormat) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_FORMAT;
			hdi.fmt = nFormat;
			return set_item(nIndex, &hdi);
		}

		LPARAM get_item_data(int nIndex) const {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_LPARAM;
			return get_item(nIndex, &hdi) ? hdi.lParam : 0;
		}

		BOOL set_item_data(int nIndex, LPARAM lParam) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_LPARAM;
			hdi.lParam = lParam;
			return set_item(nIndex, &hdi);
		}

		template<typename T>
		T* get_item_data(int nIndex) const {
			return reinterpret_cast<T*>(get_item_data(nIndex));
		}

		template<typename T>
		BOOL set_item_data(int nIndex, T* data) {
			return set_item_data(nIndex, reinterpret_cast<LPARAM>(data));
		}

		int get_item_image(int nIndex) const {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_IMAGE;
			return get_item(nIndex, &hdi) ? hdi.iImage : -1;
		}

		BOOL set_item_image(int nIndex, int iImage) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_IMAGE;
			hdi.iImage = iImage;
			return set_item(nIndex, &hdi);
		}

		int get_item_order(int nIndex) const {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_ORDER;
			return get_item(nIndex, &hdi) ? hdi.iOrder : -1;
		}

		BOOL set_item_order(int nIndex, int iOrder) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_ORDER;
			hdi.iOrder = iOrder;
			return set_item(nIndex, &hdi);
		}

		BOOL is_item_left_aligned(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_LEFT) == HDF_LEFT || (fmt & (HDF_CENTER | HDF_RIGHT)) == 0;
		}

		BOOL is_item_center_aligned(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_CENTER) == HDF_CENTER;
		}

		BOOL is_item_right_aligned(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_RIGHT) == HDF_RIGHT;
		}

		BOOL is_item_sortup(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_SORTUP) == HDF_SORTUP;
		}

		BOOL is_item_sortdown(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_SORTDOWN) == HDF_SORTDOWN;
		}

		BOOL is_item_checkbox(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_CHECKBOX) == HDF_CHECKBOX;
		}

		BOOL is_item_checked(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_CHECKED) == HDF_CHECKED;
		}

		BOOL has_item_image(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_IMAGE) == HDF_IMAGE;
		}

		BOOL has_item_bitmap(int nIndex) const {
			int fmt = get_item_format(nIndex);
			return (fmt & HDF_BITMAP) == HDF_BITMAP;
		}

		BOOL set_item_checked(int nIndex, BOOL bChecked) {
			int fmt = get_item_format(nIndex);
			if (bChecked) {
				fmt |= HDF_CHECKED;
			} else {
				fmt &= ~HDF_CHECKED;
			}
			return set_item_format(nIndex, fmt);
		}

		BOOL set_sort_arrow(int nIndex, BOOL bAscending) {
			int fmt = get_item_format(nIndex);
			fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
			fmt |= bAscending ? HDF_SORTUP : HDF_SORTDOWN;
			return set_item_format(nIndex, fmt);
		}

		BOOL clear_sort_arrow(int nIndex) {
			int fmt = get_item_format(nIndex);
			fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
			return set_item_format(nIndex, fmt);
		}

		void clear_all_sort_arrows() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				clear_sort_arrow(i);
			}
		}

		int find_sorted_column() const {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (is_item_sortup(i) || is_item_sortdown(i)) {
					return i;
				}
			}
			return -1;
		}

		BOOL is_column_sorted(int nIndex) const {
			return is_item_sortup(nIndex) || is_item_sortdown(nIndex);
		}

		int insert_item(int nIndex, LPCTSTR lpszText, int nWidth = 100, int nFormat = HDF_LEFT) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
			hdi.pszText = (LPTSTR)lpszText;
			hdi.cxy = nWidth;
			hdi.fmt = nFormat;
			return insert_item(nIndex, &hdi);
		}

		int add_item(LPCTSTR lpszText, int nWidth = 100, int nFormat = HDF_LEFT) {
			return insert_item(get_item_count(), lpszText, nWidth, nFormat);
		}

		int insert_item_with_image(int nIndex, LPCTSTR lpszText, int iImage, int nWidth = 100) {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT | HDI_IMAGE;
			hdi.pszText = (LPTSTR)lpszText;
			hdi.cxy = nWidth;
			hdi.fmt = HDF_LEFT | HDF_IMAGE;
			hdi.iImage = iImage;
			return insert_item(nIndex, &hdi);
		}

		int add_item_with_image(LPCTSTR lpszText, int iImage, int nWidth = 100) {
			return insert_item_with_image(get_item_count(), lpszText, iImage, nWidth);
		}

		std::vector<std::tstring> get_all_item_texts() const {
			std::vector<std::tstring> texts;
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				texts.push_back(get_item_text(i));
			}
			return texts;
		}

		std::vector<int> get_all_widths() const {
			std::vector<int> widths;
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				widths.push_back(get_item_width(i));
			}
			return widths;
		}

		void set_all_widths(int nWidth) {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				set_item_width(i, nWidth);
			}
		}

		BOOL delete_all_items() {
			int count = get_item_count();
			for (int i = count - 1; i >= 0; i--) {
				if (!delete_item(i)) {
					return FALSE;
				}
			}
			return TRUE;
		}

		int find_item_by_text(LPCTSTR lpszText, BOOL bExact = TRUE) const {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				std::tstring itemText = get_item_text(i);
				if (bExact) {
					if (lstrcmp(itemText.c_str(), lpszText) == 0) {
						return i;
					}
				} else {
					if (_tcsstr(itemText.c_str(), lpszText) != nullptr) {
						return i;
					}
				}
			}
			return -1;
		}

		int find_item_by_data(LPARAM lParam) const {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (get_item_data(i) == lParam) {
					return i;
				}
			}
			return -1;
		}

		template<typename T>
		int find_item_by_data(T* data) const {
			return find_item_by_data(reinterpret_cast<LPARAM>(data));
		}

		RECT get_item_rect(int nIndex) const {
			RECT rc = { 0 };
			get_item_rect(nIndex, &rc);
			return rc;
		}

		int get_item_height() const {
			if (get_item_count() > 0) {
				RECT rc = get_item_rect(0);
				return rc.bottom - rc.top;
			}
			return 0;
		}

		int get_total_width() const {
			int total = 0;
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				total += get_item_width(i);
			}
			return total;
		}

		BOOL swap_items(int nIndex1, int nIndex2) {
			if (!is_valid_index(nIndex1) || !is_valid_index(nIndex2) || nIndex1 == nIndex2) {
				return FALSE;
			}

			int order1 = get_item_order(nIndex1);
			int order2 = get_item_order(nIndex2);

			return set_item_order(nIndex1, order2) && set_item_order(nIndex2, order1);
		}

		BOOL move_item_left(int nIndex) {
			int order = get_item_order(nIndex);
			if (order > 0) {
				int count = get_item_count();
				for (int i = 0; i < count; i++) {
					if (i != nIndex && get_item_order(i) == order - 1) {
						return swap_items(nIndex, i);
					}
				}
			}
			return FALSE;
		}

		BOOL move_item_right(int nIndex) {
			int order = get_item_order(nIndex);
			int maxOrder = get_item_count() - 1;
			if (order < maxOrder) {
				int count = get_item_count();
				for (int i = 0; i < count; i++) {
					if (i != nIndex && get_item_order(i) == order + 1) {
						return swap_items(nIndex, i);
					}
				}
			}
			return FALSE;
		}

		std::vector<int> get_column_order() const {
			std::vector<int> order;
			int count = get_item_count();
			if (count > 0) {
				order.resize(count);
				get_order_array(count, order.data());
			}
			return order;
		}

		BOOL set_column_order(const std::vector<int>& order) {
			return set_order_array((int)order.size(), const_cast<int*>(order.data()));
		}

		BOOL reset_column_order() {
			std::vector<int> order;
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				order.push_back(i);
			}
			return set_column_order(order);
		}

		int hit_test(POINT pt) const {
			HDHITTESTINFO hti = { 0 };
			hti.pt = pt;
			return hit_test(&hti);
		}

		int hit_test_flags(POINT pt, UINT& flags) const {
			HDHITTESTINFO hti = { 0 };
			hti.pt = pt;
			int result = hit_test(&hti);
			flags = hti.flags;
			return result;
		}

		BOOL is_style_buttons() const {
			return (get_style() & HDS_BUTTONS) == HDS_BUTTONS;
		}

		BOOL is_style_hottrack() const {
			return (get_style() & HDS_HOTTRACK) == HDS_HOTTRACK;
		}

		BOOL is_style_hidden() const {
			return (get_style() & HDS_HIDDEN) == HDS_HIDDEN;
		}

		BOOL is_style_dragdrop() const {
			return (get_style() & HDS_DRAGDROP) == HDS_DRAGDROP;
		}

		BOOL is_style_fulldrag() const {
			return (get_style() & HDS_FULLDRAG) == HDS_FULLDRAG;
		}

		BOOL is_style_filterbar() const {
			return (get_style() & HDS_FILTERBAR) == HDS_FILTERBAR;
		}

		BOOL is_style_flat() const {
			return (get_style() & HDS_FLAT) == HDS_FLAT;
		}

		void auto_size_column(int nIndex) {
			RECT rc = get_item_rect(nIndex);
			int width = rc.right - rc.left;

			HDC hdc = GetDC(m_handle);
			if (hdc) {
				std::tstring text = get_item_text(nIndex);
				SIZE size = { 0 };
				GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &size);
				width = size.cx + 20;
				ReleaseDC(m_handle, hdc);
			}

			set_item_width(nIndex, width);
		}

		void auto_size_all_columns() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				auto_size_column(i);
			}
		}

		BOOL has_filter(int nIndex) const {
			HDITEM hdi = { 0 };
			hdi.mask = HDI_FILTER;
			if (get_item(nIndex, &hdi)) {
				return hdi.type != 0;
			}
			return FALSE;
		}

		void toggle_item_checkbox(int nIndex) {
			if (is_item_checkbox(nIndex)) {
				set_item_checked(nIndex, !is_item_checked(nIndex));
			}
		}

		int get_checked_count() const {
			int count = get_item_count();
			int checked = 0;
			for (int i = 0; i < count; i++) {
				if (is_item_checked(i)) {
					checked++;
				}
			}
			return checked;
		}

		std::vector<int> get_checked_items() const {
			std::vector<int> checked;
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (is_item_checked(i)) {
					checked.push_back(i);
				}
			}
			return checked;
		}

		void check_all_items() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (is_item_checkbox(i)) {
					set_item_checked(i, TRUE);
				}
			}
		}

		void uncheck_all_items() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (is_item_checkbox(i)) {
					set_item_checked(i, FALSE);
				}
			}
		}
	};

}

#endif // __HEADER_HPP__
