#ifndef __LIST_VIEW_HPP__
#define __LIST_VIEW_HPP__

namespace wpp
{
	class list_view : public control {
	public:
		using control::control;

		list_view& on_click(notify_callback callback) { register_notify_callback(NM_CLICK, std::move(callback)); return *this; }
		list_view& on_double_click(notify_callback callback) { register_notify_callback(NM_DBLCLK, std::move(callback)); return *this; }
		list_view& on_right_click(notify_callback callback) { register_notify_callback(NM_RCLICK, std::move(callback)); return *this; }
		list_view& on_right_double_click(notify_callback callback) { register_notify_callback(NM_RDBLCLK, std::move(callback)); return *this; }
		list_view& on_return(notify_callback callback) { register_notify_callback(NM_RETURN, std::move(callback)); return *this; }
		list_view& on_set_focus(notify_callback callback) { register_notify_callback(NM_SETFOCUS, std::move(callback)); return *this; }
		list_view& on_kill_focus(notify_callback callback) { register_notify_callback(NM_KILLFOCUS, std::move(callback)); return *this; }
		list_view& on_hover(notify_callback callback) { register_notify_callback(NM_HOVER, std::move(callback)); return *this; }
		list_view& on_custom_draw(notify_callback callback) { register_notify_callback(NM_CUSTOMDRAW, std::move(callback)); return *this; }
		list_view& on_begin_drag(notify_callback callback) { register_notify_callback(LVN_BEGINDRAG, std::move(callback)); return *this; }
		list_view& on_begin_right_drag(notify_callback callback) { register_notify_callback(LVN_BEGINRDRAG, std::move(callback)); return *this; }
		list_view& on_begin_label_edit(notify_callback callback) { register_notify_callback(LVN_BEGINLABELEDIT, std::move(callback)); return *this; }
		list_view& on_end_label_edit(notify_callback callback) { register_notify_callback(LVN_ENDLABELEDIT, std::move(callback)); return *this; }
		list_view& on_column_click(notify_callback callback) { register_notify_callback(LVN_COLUMNCLICK, std::move(callback)); return *this; }
		list_view& on_delete_item(notify_callback callback) { register_notify_callback(LVN_DELETEITEM, std::move(callback)); return *this; }
		list_view& on_delete_all_items(notify_callback callback) { register_notify_callback(LVN_DELETEALLITEMS, std::move(callback)); return *this; }
		list_view& on_item_changed(notify_callback callback) { register_notify_callback(LVN_ITEMCHANGED, std::move(callback)); return *this; }
		list_view& on_item_changing(notify_callback callback) { register_notify_callback(LVN_ITEMCHANGING, std::move(callback)); return *this; }
		list_view& on_insert_item(notify_callback callback) { register_notify_callback(LVN_INSERTITEM, std::move(callback)); return *this; }
		list_view& on_item_activate(notify_callback callback) { register_notify_callback(LVN_ITEMACTIVATE, std::move(callback)); return *this; }
		list_view& on_key_down(notify_callback callback) { register_notify_callback(LVN_KEYDOWN, std::move(callback)); return *this; }
		list_view& on_marquee_begin(notify_callback callback) { register_notify_callback(LVN_MARQUEEBEGIN, std::move(callback)); return *this; }
		list_view& on_get_display_info(notify_callback callback) { register_notify_callback(LVN_GETDISPINFO, std::move(callback)); return *this; }
		list_view& on_set_display_info(notify_callback callback) { register_notify_callback(LVN_SETDISPINFO, std::move(callback)); return *this; }
		list_view& on_od_cache_hint(notify_callback callback) { register_notify_callback(LVN_ODCACHEHINT, std::move(callback)); return *this; }
		list_view& on_od_find_item(notify_callback callback) { register_notify_callback(LVN_ODFINDITEM, std::move(callback)); return *this; }
		list_view& on_od_state_changed(notify_callback callback) { register_notify_callback(LVN_ODSTATECHANGED, std::move(callback)); return *this; }
		list_view& on_hot_track(notify_callback callback) { register_notify_callback(LVN_HOTTRACK, std::move(callback)); return *this; }
		list_view& on_get_info_tip(notify_callback callback) { register_notify_callback(LVN_GETINFOTIP, std::move(callback)); return *this; }
		list_view& on_begin_scroll(notify_callback callback) { register_notify_callback(LVN_BEGINSCROLL, std::move(callback)); return *this; }
		list_view& on_end_scroll(notify_callback callback) { register_notify_callback(LVN_ENDSCROLL, std::move(callback)); return *this; }
		list_view& on_link_click(notify_callback callback) { register_notify_callback(LVN_LINKCLICK, std::move(callback)); return *this; }
		list_view& on_get_empty_markup(notify_callback callback) { register_notify_callback(LVN_GETEMPTYMARKUP, std::move(callback)); return *this; }
		list_view& on_column_overflow_click(notify_callback callback) { register_notify_callback(LVN_COLUMNOVERFLOWCLICK, std::move(callback)); return *this; }
		list_view& on_column_dropdown(notify_callback callback) { register_notify_callback(LVN_COLUMNDROPDOWN, std::move(callback)); return *this; }
		list_view& on_incremental_search(notify_callback callback) { register_notify_callback(LVN_INCREMENTALSEARCH, std::move(callback)); return *this; }
		list_view& on_begin_group_rename(notify_callback callback) { register_notify_callback(LVN_BEGINLABELEDIT, std::move(callback)); return *this; }
		list_view& on_end_group_rename(notify_callback callback) { register_notify_callback(LVN_ENDLABELEDIT, std::move(callback)); return *this; }

		COLORREF get_back_color() const {
			return ListView_GetBkColor(m_handle);
		}

		BOOL set_back_color(COLORREF cr) {
			return ListView_SetBkColor(m_handle, cr);
		}

		image_list get_image_list(int nImageListType) const {
			return image_list{ ListView_GetImageList(m_handle, nImageListType) };
		}

		image_list set_image_list(HIMAGELIST hImageList, int nImageList) {
			return image_list{ ListView_SetImageList(m_handle, hImageList, nImageList) };
		}

		int get_item_count() const {
			return ListView_GetItemCount(m_handle);
		}

		BOOL set_item_count(int nItems) {
			return (BOOL)ListView_SetItemCount(m_handle, nItems);
		}

		BOOL get_item(LPLVITEM pItem) const {
			return ListView_GetItem(m_handle, pItem);
		}

		BOOL set_item(const LVITEM* pItem) {
			return ListView_SetItem(m_handle, pItem);
		}

		BOOL set_item(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
					  int nImage, UINT nState, UINT nStateMask, LPARAM lParam) {
			LVITEM lvi = { 0 };
			lvi.mask = nMask;
			lvi.iItem = nItem;
			lvi.iSubItem = nSubItem;
			lvi.stateMask = nStateMask;
			lvi.state = nState;
			lvi.pszText = (LPTSTR)lpszItem;
			lvi.iImage = nImage;
			lvi.lParam = lParam;
			return set_item(&lvi);
		}

		UINT get_item_state(int nItem, UINT nMask) const {
			return ListView_GetItemState(m_handle, nItem, nMask);
		}

		BOOL set_item_state(int nItem, UINT nState, UINT nStateMask) {
			LVITEM lvi = { 0 };
			lvi.state = nState;
			lvi.stateMask = nStateMask;
			return (BOOL)SendMessage(m_handle, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
		}

		BOOL set_item_state(int nItem, LPLVITEM pItem) {
			return (BOOL)SendMessage(m_handle, LVM_SETITEMSTATE, nItem, (LPARAM)pItem);
		}

		int get_item_text(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const {
			LVITEM lvi = { 0 };
			lvi.iSubItem = nSubItem;
			lvi.cchTextMax = nLen;
			lvi.pszText = lpszText;
			return (int)SendMessage(m_handle, LVM_GETITEMTEXT, (WPARAM)nItem, (LPARAM)&lvi);
		}

		BOOL set_item_text(int nItem, int nSubItem, LPCTSTR lpszText) {
			return set_item(nItem, nSubItem, LVIF_TEXT, lpszText, 0, 0, 0, 0);
		}

		DWORD_PTR get_item_data(int nItem) const {
			LVITEM lvi = { 0 };
			lvi.iItem = nItem;
			lvi.mask = LVIF_PARAM;
			BOOL bRet = (BOOL)SendMessage(m_handle, LVM_GETITEM, 0, (LPARAM)&lvi);
			return (DWORD_PTR)(bRet ? lvi.lParam : NULL);
		}

		BOOL set_item_data(int nItem, DWORD_PTR dwData) {
			return set_item(nItem, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)dwData);
		}

		UINT get_callback_mask() const {
			return ListView_GetCallbackMask(m_handle);
		}

		BOOL set_callback_mask(UINT nMask) {
			return ListView_SetCallbackMask(m_handle, nMask);
		}

		BOOL get_item_position(int nItem, LPPOINT lpPoint) const {
			return ListView_GetItemPosition(m_handle, nItem, lpPoint);
		}

		BOOL set_item_position(int nItem, POINT pt) {
			return ListView_SetItemPosition(m_handle, nItem, pt.x, pt.y);
		}

		void set_item_position(int nItem, int x, int y) {
			ListView_SetItemPosition32(m_handle, nItem, x, y);
		}

		int get_string_width(LPCTSTR lpsz) const {
			return ListView_GetStringWidth(m_handle, lpsz);
		}

		edit_text get_edit_control() const {
			return edit_text{ ListView_GetEditControl(m_handle) };
		}

		BOOL get_column(int nCol, LVCOLUMN* pColumn) const {
			return ListView_GetColumn(m_handle, nCol, pColumn);
		}

		BOOL set_column(int nCol, const LVCOLUMN* pColumn) {
			return ListView_SetColumn(m_handle, nCol, pColumn);
		}

		int get_column_width(int nCol) const {
			return ListView_GetColumnWidth(m_handle, nCol);
		}

		BOOL set_column_width(int nCol, int cx) {
			return ListView_SetColumnWidth(m_handle, nCol, cx);
		}

		BOOL get_view_rect(LPRECT lpRect) const {
			return ListView_GetViewRect(m_handle, lpRect);
		}

		COLORREF get_text_color() const {
			return ListView_GetTextColor(m_handle);
		}

		BOOL set_text_color(COLORREF cr) {
			return ListView_SetTextColor(m_handle, cr);
		}

		COLORREF get_text_back_color() const {
			return ListView_GetTextBkColor(m_handle);
		}

		BOOL set_text_back_color(COLORREF cr) {
			return ListView_SetTextBkColor(m_handle, cr);
		}

		int get_top_index() const {
			return ListView_GetTopIndex(m_handle);
		}

		int get_count_per_page() const {
			return ListView_GetCountPerPage(m_handle);
		}

		BOOL get_origin(LPPOINT lpPoint) const {
			return ListView_GetOrigin(m_handle, lpPoint);
		}

		UINT get_selected_count() const {
			return ListView_GetSelectedCount(m_handle);
		}

		BOOL get_item_rect(int nItem, LPRECT lpRect, UINT nCode) const {
			return ListView_GetItemRect(m_handle, nItem, lpRect, nCode);
		}

		HCURSOR get_hot_cursor() const {
			return ListView_GetHotCursor(m_handle);
		}

		HCURSOR set_hot_cursor(HCURSOR hHotCursor) {
			return ListView_SetHotCursor(m_handle, hHotCursor);
		}

		int get_hot_item() const {
			return ListView_GetHotItem(m_handle);
		}

		int set_hot_item(int nIndex) {
			return ListView_SetHotItem(m_handle, nIndex);
		}

		BOOL get_column_order_array(int nCount, int* lpnArray) const {
			return ListView_GetColumnOrderArray(m_handle, nCount, lpnArray);
		}

		BOOL set_column_order_array(int nCount, int* lpnArray) {
			return ListView_SetColumnOrderArray(m_handle, nCount, lpnArray);
		}

		header get_header() const {
			return header{ ListView_GetHeader(m_handle) };
		}

		BOOL get_sub_item_rect(int nItem, int nSubItem, int nFlag, LPRECT lpRect) const {
			return ListView_GetSubItemRect(m_handle, nItem, nSubItem, nFlag, lpRect);
		}

		DWORD set_icon_spacing(int cx, int cy) {
			return ListView_SetIconSpacing(m_handle, cx, cy);
		}

		int get_isearch_string(LPTSTR lpstr) const {
			return ListView_GetISearchString(m_handle, lpstr);
		}

		void get_item_spacing(SIZE& sizeSpacing, BOOL bSmallIconView = FALSE) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, LVM_GETITEMSPACING, bSmallIconView, 0L);
			sizeSpacing.cx = GET_X_LPARAM(dwRet);
			sizeSpacing.cy = GET_Y_LPARAM(dwRet);
		}

		int get_selected_item() const {
			return ListView_GetNextItem(m_handle, -1, LVNI_ALL | LVNI_SELECTED);
		}

		BOOL get_selected_item(LPLVITEM pItem) const {
			pItem->iItem = (int)SendMessage(m_handle, LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));
			if (pItem->iItem == -1) return FALSE;
			return (BOOL)SendMessage(m_handle, LVM_GETITEM, 0, (LPARAM)pItem);
		}

		DWORD get_extended_listview_style() const {
			return ListView_GetExtendedListViewStyle(m_handle);
		}

		DWORD set_extended_listview_style(DWORD dwExStyle, DWORD dwExMask = 0) {
			return ListView_SetExtendedListViewStyleEx(m_handle, dwExMask, dwExStyle);
		}

		BOOL get_check_state(int nIndex) const {
			UINT uRet = get_item_state(nIndex, LVIS_STATEIMAGEMASK);
			return (uRet >> 12) - 1;
		}

		void set_check_state(int nItem, BOOL bCheck) {
			int nCheck = bCheck ? 2 : 1;
			set_item_state(nItem, INDEXTOSTATEIMAGEMASK(nCheck), LVIS_STATEIMAGEMASK);
		}

		DWORD get_view_type() const {
			return (get_style() & LVS_TYPEMASK);
		}

		DWORD set_view_type(DWORD dwType) {
			DWORD dwOldType = get_view_type();
			if (dwType != dwOldType) modify_style(LVS_TYPEMASK, (dwType & LVS_TYPEMASK));
			return dwOldType;
		}

		BOOL get_back_image(LPLVBKIMAGE plvbki) const {
			return ListView_GetBkImage(m_handle, plvbki);
		}

		BOOL set_bk_image(LPLVBKIMAGE plvbki) {
			return ListView_SetBkImage(m_handle, plvbki);
		}

		int get_selection_mark() const {
			return ListView_GetSelectionMark(m_handle);
		}

		int set_selection_mark(int nIndex) {
			return ListView_SetSelectionMark(m_handle, nIndex);
		}

		BOOL get_work_areas(int nWorkAreas, LPRECT lpRect) const {
			return ListView_GetWorkAreas(m_handle, nWorkAreas, lpRect);
		}

		BOOL set_work_areas(int nWorkAreas, LPRECT lpRect) {
			return ListView_SetWorkAreas(m_handle, nWorkAreas, lpRect);
		}

		DWORD get_hover_time() const {
			return ListView_GetHoverTime(m_handle);
		}

		DWORD set_hover_time(DWORD dwHoverTime) {
			return ListView_SetHoverTime(m_handle, dwHoverTime);
		}

		BOOL get_number_work_areas(int* pnWorkAreas) const {
			return ListView_GetNumberOfWorkAreas(m_handle, pnWorkAreas);
		}

		BOOL set_item_count_ex(int nItems, DWORD dwFlags) {
			return (BOOL)ListView_SetItemCountEx(m_handle, nItems, dwFlags);
		}

		tooltip get_tooltips() const {
			return tooltip{ ListView_GetToolTips(m_handle) };
		}

		tooltip set_tooltips(HWND hWndTT) {
			return tooltip{ ListView_SetToolTips(m_handle, hWndTT) };
		}

		BOOL get_unicode_format() const {
			return ListView_GetUnicodeFormat(m_handle);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return ListView_SetUnicodeFormat(m_handle, bUnicode);
		}

		int get_selected_column() const {
			return ListView_GetSelectedColumn(m_handle);
		}

		void set_selected_column(int nColumn) {
			ListView_SetSelectedColumn(m_handle, nColumn);
		}

		DWORD get_view() const {
			return ListView_GetView(m_handle);
		}

		int set_view(DWORD dwView) {
			return ListView_SetView(m_handle, dwView);
		}

		BOOL is_group_view_enabled() const {
			return ListView_IsGroupViewEnabled(m_handle);
		}

		void get_group_info(int nGroupID, PLVGROUP pGroup) const {
			ListView_GetGroupInfo(m_handle, nGroupID, pGroup);
		}

		void set_group_info(int nGroupID, PLVGROUP pGroup) {
			ListView_SetGroupInfo(m_handle, nGroupID, pGroup);
		}

		void get_group_metrics(PLVGROUPMETRICS pGroupMetrics) const {
			ListView_GetGroupMetrics(m_handle, pGroupMetrics);
		}

		void set_group_metrics(PLVGROUPMETRICS pGroupMetrics) {
			ListView_SetGroupMetrics(m_handle, pGroupMetrics);
		}

		void get_tile_view_info(PLVTILEVIEWINFO pTileViewInfo) const {
			ListView_GetTileViewInfo(m_handle, pTileViewInfo);
		}

		void set_tile_view_info(PLVTILEVIEWINFO pTileViewInfo) {
			ListView_SetTileViewInfo(m_handle, pTileViewInfo);
		}

		void get_tile_info(PLVTILEINFO pTileInfo) const {
			ListView_GetTileInfo(m_handle, pTileInfo);
		}

		void set_tile_info(PLVTILEINFO pTileInfo) {
			ListView_SetTileInfo(m_handle, pTileInfo);
		}

		BOOL get_insert_mark(LPLVINSERTMARK pInsertMark) const {
			return ListView_GetInsertMark(m_handle, pInsertMark);
		}

		BOOL set_insert_mark(LPLVINSERTMARK pInsertMark) {
			return ListView_SetInsertMark(m_handle, pInsertMark);
		}

		int get_insert_mark_rect(LPRECT lpRect) const {
			return ListView_GetInsertMarkRect(m_handle, lpRect);
		}

		COLORREF get_insert_mark_color() const {
			return ListView_GetInsertMarkColor(m_handle);
		}

		COLORREF set_insert_mark_color(COLORREF clr) {
			return ListView_SetInsertMarkColor(m_handle, clr);
		}

		COLORREF get_outline_color() const {
			return ListView_GetOutlineColor(m_handle);
		}

		COLORREF set_outline_color(COLORREF clr) {
			return ListView_SetOutlineColor(m_handle, clr);
		}

		int get_group_count() const {
			return (int)ListView_GetGroupCount(m_handle);
		}

		void get_group_info_by_index(int nIndex, PLVGROUP pGroup) const {
			ListView_GetGroupInfoByIndex(m_handle, nIndex, pGroup);
		}

		void get_group_rect(int nGroupID, int nType, LPRECT lpRect) const {
			ListView_GetGroupRect(m_handle, nGroupID, nType, lpRect);
		}

		UINT get_group_state(int nGroupID, UINT uMask) const {
			return ListView_GetGroupState(m_handle, nGroupID, uMask);
		}

		int get_focused_group() const {
			return (int)ListView_GetFocusedGroup(m_handle);
		}

		BOOL get_empty_text(LPWSTR lpstrText, int cchText) const {
			return ListView_GetEmptyText(m_handle, lpstrText, cchText);
		}

		BOOL get_footer_rect(LPRECT lpRect) const {
			return ListView_GetFooterRect(m_handle, lpRect);
		}

		BOOL get_footer_info(LPLVFOOTERINFO lpFooterInfo) const {
			return ListView_GetFooterInfo(m_handle, lpFooterInfo);
		}

		BOOL get_footer_item_rect(int nItem, LPRECT lpRect) const {
			return ListView_GetFooterItemRect(m_handle, nItem, lpRect);
		}

		BOOL get_footer_item(int nItem, LPLVFOOTERITEM lpFooterItem) const {
			return ListView_GetFooterItem(m_handle, nItem, lpFooterItem);
		}

		BOOL get_item_index_rect(PLVITEMINDEX pItemIndex, int nSubItem, int nType, LPRECT lpRect) const {
			return ListView_GetItemIndexRect(m_handle, pItemIndex, nSubItem, nType, lpRect);
		}

		void set_item_index_state(PLVITEMINDEX pItemIndex, UINT uState, UINT dwMask) {
			ListView_SetItemIndexState(m_handle, pItemIndex, uState, dwMask);
		}

		BOOL get_next_item_index(PLVITEMINDEX pItemIndex, WORD wFlags) const {
			return ListView_GetNextItemIndex(m_handle, pItemIndex, wFlags);
		}

		int insert_column(int nCol, const LVCOLUMN* pColumn) {
			return ListView_InsertColumn(m_handle, nCol, pColumn);
		}

		int insert_column(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT,
						  int nWidth = -1, int nSubItem = -1, int iImage = -1, int iOrder = -1) {
			LVCOLUMN column = { 0 };
			column.mask = LVCF_TEXT | LVCF_FMT;
			column.pszText = (LPTSTR)lpszColumnHeading;
			column.fmt = nFormat;
			if (nWidth != -1) {
				column.mask |= LVCF_WIDTH;
				column.cx = nWidth;
			}
			if (nSubItem != -1) {
				column.mask |= LVCF_SUBITEM;
				column.iSubItem = nSubItem;
			}
			if (iImage != -1) {
				column.mask |= LVCF_IMAGE;
				column.iImage = iImage;
			}
			if (iOrder != -1) {
				column.mask |= LVCF_ORDER;
				column.iOrder = iOrder;
			}
			return insert_column(nCol, &column);
		}

		BOOL delete_column(int nCol) {
			return ListView_DeleteColumn(m_handle, nCol);
		}

		int insert_item(const LVITEM* pItem) {
			return ListView_InsertItem(m_handle, pItem);
		}

		int insert_item(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam) {
			LVITEM item = { 0 };
			item.mask = nMask;
			item.iItem = nItem;
			item.iSubItem = 0;
			item.pszText = (LPTSTR)lpszItem;
			item.state = nState;
			item.stateMask = nStateMask;
			item.iImage = nImage;
			item.lParam = lParam;
			return insert_item(&item);
		}

		int insert_item(int nItem, LPCTSTR lpszItem) {
			return insert_item(LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0);
		}

		int insert_item(int nItem, LPCTSTR lpszItem, int nImage) {
			return insert_item(LVIF_TEXT | LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0);
		}

		int get_next_item(int nItem, int nFlags) const {
			return ListView_GetNextItem(m_handle, nItem, nFlags);
		}

		BOOL delete_item(int nItem) {
			return ListView_DeleteItem(m_handle, nItem);
		}

		BOOL delete_all_items() {
			return ListView_DeleteAllItems(m_handle);
		}

		int find_item(LVFINDINFO* pFindInfo, int nStart = -1) const {
			return ListView_FindItem(m_handle, nStart, pFindInfo);
		}

		int find_item(LPCTSTR lpstrFind, bool bPartial = true, bool bWrap = false, int nStart = -1) const {
			LVFINDINFO lvfi = { 0 };
			lvfi.flags = LVFI_STRING | (bWrap ? LVFI_WRAP : 0) | (bPartial ? LVFI_PARTIAL : 0);
			lvfi.psz = lpstrFind;
			return find_item(&lvfi, nStart);
		}

		int hit_test(LVHITTESTINFO* pHitTestInfo) const {
			return ListView_HitTest(m_handle, pHitTestInfo);
		}

		int hit_test(POINT pt, UINT* pFlags) const {
			LVHITTESTINFO hti = { 0 };
			hti.pt = pt;
			int nRes = hit_test(&hti);
			if (pFlags != NULL)
				*pFlags = hti.flags;
			return nRes;
		}

		BOOL ensure_visible(int nItem, BOOL bPartialOK) {
			return ListView_EnsureVisible(m_handle, nItem, bPartialOK);
		}

		BOOL scroll(SIZE size) {
			return ListView_Scroll(m_handle, size.cx, size.cy);
		}

		BOOL redraw_items(int nFirst, int nLast) {
			return ListView_RedrawItems(m_handle, nFirst, nLast);
		}

		BOOL arrange(UINT nCode) {
			return ListView_Arrange(m_handle, nCode);
		}

		edit_text edit_label(int nItem) {
			return edit_text{ ListView_EditLabel(m_handle, nItem) };
		}

		BOOL update(int nItem) {
			return ListView_Update(m_handle, nItem);
		}

		BOOL sort_items(PFNLVCOMPARE pfnCompare, LPARAM lParamSort) {
			return ListView_SortItems(m_handle, pfnCompare, lParamSort);
		}

		image_list remove_image_list(int nImageList) {
			return image_list{ ListView_SetImageList(m_handle, NULL, nImageList) };
		}

		image_list create_drag_image(int nItem, LPPOINT lpPoint) {
			return image_list{ ListView_CreateDragImage(m_handle, nItem, lpPoint) };
		}

		DWORD approximate_view_rect(int cx = -1, int cy = -1, int nCount = -1) {
			return ListView_ApproximateViewRect(m_handle, cx, cy, nCount);
		}

		int sub_item_hit_test(LPLVHITTESTINFO lpInfo) const {
			return ListView_SubItemHitTest(m_handle, lpInfo);
		}

		int add_column(int sub_item, LVCOLUMN column) {
			return insert_column(sub_item, &column);
		}

		int add_column(LPCTSTR strItem, int nItem, int nSubItem = -1,
					   int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
					   int nFmt = LVCFMT_LEFT) {
			const int cxOffset = 15;
			LVCOLUMN lvc = { 0 };
			lvc.mask = nMask;
			lvc.fmt = nFmt;
			lvc.pszText = (LPTSTR)strItem;
			lvc.cx = get_string_width(lvc.pszText) + cxOffset;
			if (nMask & LVCF_SUBITEM)
				lvc.iSubItem = (nSubItem != -1) ? nSubItem : nItem;
			return insert_column(nItem, &lvc);
		}

		int add_item(int nItem, int nSubItem, LPCTSTR strItem, int nImageIndex = -3) {
			LVITEM lvItem = { 0 };
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItem;
			lvItem.iSubItem = nSubItem;
			lvItem.pszText = (LPTSTR)strItem;
			if (nImageIndex != -3) {
				lvItem.mask |= LVIF_IMAGE;
				lvItem.iImage = nImageIndex;
			}
			if (nSubItem == 0)
				return insert_item(&lvItem);
			return set_item(&lvItem) ? nItem : -1;
		}

		BOOL sort_items_ex(PFNLVCOMPARE pfnCompare, LPARAM lParamSort) {
			return ListView_SortItemsEx(m_handle, pfnCompare, lParamSort);
		}

		BOOL insert_group(int nItem, PLVGROUP pGroup) {
			return (BOOL)ListView_InsertGroup(m_handle, nItem, pGroup);
		}

		BOOL add_group(PLVGROUP pGroup) {
			return insert_group(-1, pGroup);
		}

		BOOL remove_group(int nGroupID) {
			return (BOOL)ListView_RemoveGroup(m_handle, nGroupID);
		}

		BOOL move_group(int nGroupID, int nItem) {
			return (BOOL)ListView_MoveGroup(m_handle, nGroupID, nItem);
		}

		BOOL move_item_to_group(int nItem, int nGroupID) {
			return (BOOL)ListView_MoveItemToGroup(m_handle, nItem, nGroupID);
		}

		BOOL enable_group_view(BOOL bEnable) {
			return (BOOL)ListView_EnableGroupView(m_handle, bEnable);
		}

		BOOL sort_groups(PFNLVGROUPCOMPARE pCompareFunc, LPVOID lpVoid = NULL) {
			return (BOOL)ListView_SortGroups(m_handle, pCompareFunc, lpVoid);
		}

		BOOL insert_group_sorted(PLVINSERTGROUPSORTED pInsertGroupSorted) {
			return (BOOL)ListView_InsertGroupSorted(m_handle, pInsertGroupSorted);
		}

		BOOL remove_all_groups() {
			return (BOOL)ListView_RemoveAllGroups(m_handle);
		}

		BOOL has_group(int nGroupID) {
			return (BOOL)ListView_HasGroup(m_handle, nGroupID);
		}

		int insert_mark_hit_test(LPPOINT lpPoint, LPLVINSERTMARK pInsertMark) const {
			return ListView_InsertMarkHitTest(m_handle, lpPoint, pInsertMark);
		}

		BOOL set_info_tip(PLVSETINFOTIP pSetInfoTip) {
			return ListView_SetInfoTip(m_handle, pSetInfoTip);
		}

		void cancel_edit_label() {
			ListView_CancelEditLabel(m_handle);
		}

		UINT map_index_to_id(int nIndex) const {
			return ListView_MapIndexToID(m_handle, nIndex);
		}

		UINT map_id_to_index(UINT uID) const {
			return ListView_MapIDToIndex(m_handle, uID);
		}

		int hit_test_ex(LPLVHITTESTINFO lpHitTestInfo) const {
			return ListView_HitTestEx(m_handle, lpHitTestInfo);
		}

		int hit_test_ex(POINT pt, UINT* pFlags) const {
			LVHITTESTINFO hti = { 0 };
			hti.pt = pt;
			int nRes = hit_test(&hti);
			if (pFlags != NULL)
				*pFlags = hti.flags;
			return nRes;
		}

		int sub_item_hit_test(LPLVHITTESTINFO lpHitTestInfo) {
			return ListView_SubItemHitTest(m_handle, lpHitTestInfo);
		}

		BOOL select_item(int nIndex) {
			if ((get_style() & LVS_SINGLESEL) == 0)
				set_item_state(-1, 0, LVIS_SELECTED);

			BOOL bRet = set_item_state(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			if (bRet)
				bRet = ensure_visible(nIndex, FALSE);

			return bRet;
		}
		int get_count() const { return get_item_count(); }
		BOOL is_empty() const { return get_item_count() == 0; }
		BOOL is_valid_index(int nItem) const { return nItem >= 0 && nItem < get_item_count(); }

		std::tstring get_item_text_str(int nItem, int nSubItem = 0) const {
			TCHAR buffer[512] = { 0 };
			get_item_text(nItem, nSubItem, buffer, 512);
			return std::tstring(buffer);
		}

		template<typename T>
		T* get_item_data_typed(int nItem) const {
			return reinterpret_cast<T*>(get_item_data(nItem));
		}

		template<typename T>
		BOOL set_item_data_typed(int nItem, T* data) {
			return set_item_data(nItem, reinterpret_cast<DWORD_PTR>(data));
		}

		std::vector<int> get_selected_items() const {
			std::vector<int> items;
			int nItem = -1;
			while ((nItem = get_next_item(nItem, LVNI_SELECTED)) != -1) {
				items.push_back(nItem);
			}
			return items;
		}

		BOOL has_selection() const { return get_selected_count() > 0; }
		BOOL is_item_selected(int nItem) const { return (get_item_state(nItem, LVIS_SELECTED) & LVIS_SELECTED) != 0; }
		void deselect_all() { set_item_state(-1, 0, LVIS_SELECTED); }

		void select_all() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				set_item_state(i, LVIS_SELECTED, LVIS_SELECTED);
			}
		}

		BOOL delete_selected_items() {
			std::vector<int> indices = get_selected_items();
			for (int i = (int)indices.size() - 1; i >= 0; i--) {
				if (!delete_item(indices[i])) return FALSE;
			}
			return TRUE;
		}

		int get_column_count() const {
			header hdr = get_header();
			return Header_GetItemCount(hdr.get_handle());
		}

		void auto_size_columns(BOOL bUseHeader = TRUE) {
			int count = get_column_count();
			for (int i = 0; i < count; i++) {
				set_column_width(i, bUseHeader ? LVSCW_AUTOSIZE_USEHEADER : LVSCW_AUTOSIZE);
			}
		}

		int find_item_by_data(DWORD_PTR data) const {
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (get_item_data(i) == data) return i;
			}
			return -1;
		}

		template<typename T>
		int find_item_by_data_typed(T* data) const {
			return find_item_by_data(reinterpret_cast<DWORD_PTR>(data));
		}

		int get_checked_count() const {
			int count = get_item_count();
			int checked = 0;
			for (int i = 0; i < count; i++) {
				if (get_check_state(i)) checked++;
			}
			return checked;
		}

		std::vector<int> get_checked_items() const {
			std::vector<int> items;
			int count = get_item_count();
			for (int i = 0; i < count; i++) {
				if (get_check_state(i)) items.push_back(i);
			}
			return items;
		}

		void check_all_items() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) set_check_state(i, TRUE);
		}

		void uncheck_all_items() {
			int count = get_item_count();
			for (int i = 0; i < count; i++) set_check_state(i, FALSE);
		}

		BOOL is_view_details() const { return get_view() == LV_VIEW_DETAILS; }
		BOOL is_view_icon() const { return get_view() == LV_VIEW_ICON; }
		BOOL is_view_list() const { return get_view() == LV_VIEW_LIST; }
		void set_view_details() { set_view(LV_VIEW_DETAILS); }
		void set_view_icon() { set_view(LV_VIEW_ICON); }
		void set_view_list() { set_view(LV_VIEW_LIST); }

		void enable_checkboxes() { set_extended_listview_style(LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES); }
		void enable_gridlines() { set_extended_listview_style(LVS_EX_GRIDLINES, LVS_EX_GRIDLINES); }
		void enable_fullrowselect() { set_extended_listview_style(LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT); }
		void enable_doublebuffer() { set_extended_listview_style(LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER); }

		void refresh_all() {
			int count = get_count();
			if (count > 0) redraw_items(0, count - 1);
		}

		void set_default_colors() {
			set_back_color(RGB(255, 255, 255));
			set_text_color(RGB(0, 0, 0));
			set_text_back_color(RGB(255, 255, 255));
		}

		void clear() { delete_all_items(); }
		BOOL has_items() const { return !is_empty(); }
	};
}

#endif //__LIST_VIEW_HPP__