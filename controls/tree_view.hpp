#ifndef __TREE_VIEW_HPP__
#define __TREE_VIEW_HPP__

namespace wpp
{
	class tree_view : public control {
	public:
		using control::control;

		UINT get_count() const {
			return TreeView_GetCount(m_handle);
		}

		UINT get_indent() const {
			return TreeView_GetIndent(m_handle);
		}

		void set_indent(UINT nIndent) {
			TreeView_SetIndent(m_handle, nIndent);
		}

		image_list get_image_list(int nImageListType = TVSIL_NORMAL) const {
			return image_list{ TreeView_GetImageList(m_handle, nImageListType) };
		}

		image_list set_image_list(HIMAGELIST hImageList, int nImageListType = TVSIL_NORMAL) {
			return image_list{ TreeView_SetImageList(m_handle, hImageList, nImageListType) };
		}

		BOOL get_item(LPTVITEM pItem) const {
			return TreeView_GetItem(m_handle, pItem);
		}

		BOOL set_item(LPTVITEM pItem) {
			return TreeView_SetItem(m_handle, pItem);
		}

		BOOL set_item(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
					 int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam) {
			TVITEM item = { 0 };
			item.hItem = hItem;
			item.mask = nMask;
			item.pszText = (LPTSTR)lpszItem;
			item.iImage = nImage;
			item.iSelectedImage = nSelectedImage;
			item.state = nState;
			item.stateMask = nStateMask;
			item.lParam = lParam;
			return TreeView_SetItem(m_handle, &item);
		}

		BOOL get_item_text(HTREEITEM hItem, LPTSTR lpstrText, int nLen) const {
			TVITEM item = { 0 };
			item.hItem = hItem;
			item.mask = TVIF_TEXT;
			item.pszText = lpstrText;
			item.cchTextMax = nLen;

			return TreeView_GetItem(m_handle, &item);
		}

		BOOL set_item_text(HTREEITEM hItem, LPCTSTR lpszItem) {
			return set_item(hItem, TVIF_TEXT, lpszItem, 0, 0, 0, 0, NULL);
		}

		BOOL get_item_image(HTREEITEM hItem, int& nImage, int& nSelectedImage) const {
			TVITEM item = { 0 };
			item.hItem = hItem;
			item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			BOOL bRes = get_item(&item);
			if (bRes) {
				nImage = item.iImage;
				nSelectedImage = item.iSelectedImage;
			}
			return bRes;
		}

		BOOL set_item_image(HTREEITEM hItem, int nImage, int nSelectedImage) {
			return set_item(hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE, NULL, nImage, nSelectedImage, 0, 0, NULL);
		}

		UINT get_item_state(HTREEITEM hItem, UINT nStateMask) const {
			return TreeView_GetItemState(m_handle, hItem, nStateMask);
		}

		BOOL set_item_state(HTREEITEM hItem, UINT nState, UINT nStateMask) {
			return set_item(hItem, TVIF_STATE, NULL, 0, 0, nState, nStateMask, NULL);
		}

		DWORD_PTR get_item_data(HTREEITEM hItem) const {
			TVITEM item = { 0 };
			item.hItem = hItem;
			item.mask = TVIF_PARAM;
			BOOL bRet = TreeView_GetItem(m_handle, &item);
			return (DWORD_PTR)(bRet ? item.lParam : NULL);
		}

		BOOL set_item_data(HTREEITEM hItem, DWORD_PTR dwData) {
			return set_item(hItem, TVIF_PARAM, NULL, 0, 0, 0, 0, (LPARAM)dwData);
		}

		edit_text get_edit_control() const {
			return edit_text{ TreeView_GetEditControl(m_handle) };
		}

		UINT get_visible_count() const {
			return TreeView_GetVisibleCount(m_handle);
		}

		BOOL get_item_rect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const {
			return TreeView_GetItemRect(m_handle, hItem, lpRect, bTextOnly);
		}

		BOOL item_has_children(HTREEITEM hItem) const {
			TVITEM item = { 0 };
			item.hItem = hItem;
			item.mask = TVIF_CHILDREN;
			TreeView_GetItem(m_handle, &item);
			return item.cChildren;
		}

		tooltip get_tool_tips() const {
			return tooltip{ TreeView_GetToolTips(m_handle) };
		}

		tooltip set_tool_tips(tooltip hWndTT) {
			return tooltip{ TreeView_SetToolTips(m_handle, hWndTT.get_handle()) };
		}

		int get_isearch_string(LPTSTR lpstr) const {
			return TreeView_GetISearchString(m_handle, lpstr);
		}

		BOOL get_check_state(HTREEITEM hItem) const {
			UINT uRet = get_item_state(hItem, TVIS_STATEIMAGEMASK);
			return (uRet >> 12) - 1;
		}

		BOOL set_check_state(HTREEITEM hItem, BOOL bCheck) {
			int nCheck = bCheck ? 2 : 1;
			return set_item_state(hItem, INDEXTOSTATEIMAGEMASK(nCheck), TVIS_STATEIMAGEMASK);
		}

		COLORREF get_bk_color() const {
			return TreeView_GetBkColor(m_handle);
		}

		COLORREF set_bk_color(COLORREF clr) {
			return TreeView_SetBkColor(m_handle, clr);
		}

		COLORREF get_insert_mark_color() const {
			return TreeView_GetInsertMarkColor(m_handle);
		}

		COLORREF set_insert_mark_color(COLORREF clr) {
			return TreeView_SetInsertMarkColor(m_handle, clr);
		}

		int get_item_height() const {
			return TreeView_GetItemHeight(m_handle);
		}

		int set_item_height(int cyHeight) {
			return TreeView_SetItemHeight(m_handle, cyHeight);
		}

		int get_scroll_time() const {
			return TreeView_GetScrollTime(m_handle);
		}

		int set_scroll_time(int nScrollTime) {
			return TreeView_SetScrollTime(m_handle, nScrollTime);
		}

		COLORREF get_text_color() const {
			return TreeView_GetTextColor(m_handle);
		}

		COLORREF set_text_color(COLORREF clr) {
			return TreeView_SetTextColor(m_handle, clr);
		}

		BOOL get_unicode_format() const {
			return TreeView_GetUnicodeFormat(m_handle);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return TreeView_SetUnicodeFormat(m_handle, bUnicode);
		}

		COLORREF get_line_color() const {
			return TreeView_GetLineColor(m_handle);
		}

		COLORREF set_line_color(COLORREF clrNew = CLR_DEFAULT) {
			return TreeView_SetLineColor(m_handle, clrNew);
		}

		BOOL get_item(LPTVITEMEX pItem) const {
			return TreeView_GetItem(m_handle, (LPTVITEM)pItem);
		}

		BOOL set_item(LPTVITEMEX pItem) {
			return TreeView_SetItem(m_handle, (LPTVITEM)pItem);
		}

		DWORD get_extended_style() const {
			return TreeView_GetExtendedStyle(m_handle);
		}

		DWORD set_extended_style(DWORD dwStyle, DWORD dwMask) {
			return TreeView_SetExtendedStyle(m_handle, dwMask, dwStyle);
		}

		BOOL set_auto_scroll_info(UINT uPixPerSec, UINT uUpdateTime) {
			return (BOOL)TreeView_SetAutoScrollInfo(m_handle, uPixPerSec, uUpdateTime);
		}

		DWORD get_selected_count() const {
			return TreeView_GetSelectedCount(m_handle);
		}

		BOOL get_item_part_rect(HTREEITEM hItem, TVITEMPART partID, LPRECT lpRect) const {
			TVGETITEMPARTRECTINFO gipri = { hItem, lpRect, partID };
			return (BOOL)::SendMessage(m_handle, TVM_GETITEMPARTRECT, 0, (LPARAM)&gipri);
		}

		HTREEITEM insert_item(LPTVINSERTSTRUCT lpInsertStruct) {
			return TreeView_InsertItem(m_handle, lpInsertStruct);
		}

		HTREEITEM insert_item(LPCTSTR lpszItem, int nImage,
							 int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter) {
			return insert_item(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter);
		}

		HTREEITEM insert_item(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter) {
			return insert_item(TVIF_TEXT, lpszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
		}

		HTREEITEM insert_item(UINT nMask, LPCTSTR lpszItem, int nImage,
							 int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
							 HTREEITEM hParent, HTREEITEM hInsertAfter) {
			TVINSERTSTRUCT tvis = { 0 };
			tvis.hParent = hParent;
			tvis.hInsertAfter = hInsertAfter;
			tvis.item.mask = nMask;
			tvis.item.pszText = (LPTSTR)lpszItem;
			tvis.item.iImage = nImage;
			tvis.item.iSelectedImage = nSelectedImage;
			tvis.item.state = nState;
			tvis.item.stateMask = nStateMask;
			tvis.item.lParam = lParam;
			return TreeView_InsertItem(m_handle, &tvis);
		}

		BOOL delete_item(HTREEITEM hItem) {
			return TreeView_DeleteItem(m_handle, hItem);
		}

		BOOL delete_all_items() {
			return TreeView_DeleteAllItems(m_handle);
		}

		BOOL expand(HTREEITEM hItem, UINT nCode = TVE_EXPAND) {
			return TreeView_Expand(m_handle, hItem, nCode);
		}

		HTREEITEM get_next_item(HTREEITEM hItem, UINT nCode) const {
			return TreeView_GetNextItem(m_handle, hItem, nCode);
		}

		HTREEITEM get_child_item(HTREEITEM hItem) const {
			return TreeView_GetChild(m_handle, hItem);
		}

		HTREEITEM get_next_sibling_item(HTREEITEM hItem) const {
			return TreeView_GetNextSibling(m_handle, hItem);
		}

		HTREEITEM get_prev_sibling_item(HTREEITEM hItem) const {
			return TreeView_GetPrevSibling(m_handle, hItem);
		}

		HTREEITEM get_parent_item(HTREEITEM hItem) const {
			return TreeView_GetParent(m_handle, hItem);
		}

		HTREEITEM get_first_visible_item() const {
			return TreeView_GetFirstVisible(m_handle);
		}

		HTREEITEM get_next_visible_item(HTREEITEM hItem) const {
			return TreeView_GetNextVisible(m_handle, hItem);
		}

		HTREEITEM get_prev_visible_item(HTREEITEM hItem) const {
			return TreeView_GetPrevVisible(m_handle, hItem);
		}

		HTREEITEM get_selected_item() const {
			return TreeView_GetSelection(m_handle);
		}

		HTREEITEM get_selected_item_parent() const {
			return get_parent_item(get_selected_item());
		}

		HTREEITEM get_drop_hilight_item() const {
			return TreeView_GetDropHilight(m_handle);
		}

		HTREEITEM get_root_item() const {
			return TreeView_GetRoot(m_handle);
		}

		HTREEITEM get_last_visible_item() const {
			return TreeView_GetLastVisible(m_handle);
		}

		HTREEITEM get_next_selected_item() const {
			return TreeView_GetNextSelected(m_handle, NULL);
		}

		BOOL select(HTREEITEM hItem, UINT nCode) {
			return TreeView_Select(m_handle, hItem, nCode);
		}

		BOOL select_item(HTREEITEM hItem) {
			return TreeView_SelectItem(m_handle, hItem);
		}

		BOOL select_drop_target(HTREEITEM hItem) {
			return TreeView_SelectDropTarget(m_handle, hItem);
		}

		BOOL select_set_first_visible(HTREEITEM hItem) {
			return TreeView_SelectSetFirstVisible(m_handle, hItem);
		}

		edit_text edit_label(HTREEITEM hItem) {
			return edit_text{ TreeView_EditLabel(m_handle, hItem) };
		}

		BOOL end_edit_label_now(BOOL bCancel) {
			return TreeView_EndEditLabelNow(m_handle, bCancel);
		}

		HTREEITEM hit_test(TVHITTESTINFO* pHitTestInfo) const {
			return TreeView_HitTest(m_handle, pHitTestInfo);
		}

		HTREEITEM hit_test(POINT pt, UINT* pFlags) const {
			TVHITTESTINFO hti = { 0 };
			hti.pt = pt;
			HTREEITEM hTreeItem = hit_test(&hti);
			if (pFlags != NULL)
				*pFlags = hti.flags;
			return hTreeItem;
		}

		BOOL sort_children(HTREEITEM hItem, BOOL bRecurse = FALSE) {
			return TreeView_SortChildren(m_handle, hItem, bRecurse);
		}

		BOOL ensure_visible(HTREEITEM hItem) {
			return TreeView_EnsureVisible(m_handle, hItem);
		}

		BOOL sort_children_cb(LPTVSORTCB pSort, BOOL bRecurse = FALSE) {
			return TreeView_SortChildrenCB(m_handle, pSort, bRecurse);
		}

		image_list remove_image_list(int nImageList) {
			return image_list{ TreeView_SetImageList(m_handle, NULL, nImageList) };
		}

		image_list create_drag_image(HTREEITEM hItem) {
			return image_list{ TreeView_CreateDragImage(m_handle, hItem) };
		}

		BOOL set_insert_mark(HTREEITEM hTreeItem, BOOL bAfter) {
			return TreeView_SetInsertMark(m_handle, hTreeItem, bAfter);
		}

		BOOL remove_insert_mark() {
			return TreeView_SetInsertMark(m_handle, NULL, 0);
		}

		HTREEITEM map_acc_id_to_htreeitem(UINT uID) const {
			return TreeView_MapAccIDToHTREEITEM(m_handle, uID);
		}

		UINT map_htreeitem_to_acc_id(HTREEITEM hTreeItem) const {
			return TreeView_MapHTREEITEMToAccID(m_handle, hTreeItem);
		}

		void show_info_tip(HTREEITEM hItem) {
			TreeView_ShowInfoTip(m_handle, hItem);
		}

		BOOL is_empty() const {
			return get_count() == 0;
		}

		BOOL is_valid_item(HTREEITEM hItem) const {
			return hItem != NULL && hItem != TVI_ROOT;
		}

		std::tstring get_item_text_str(HTREEITEM hItem) const {
			TCHAR buffer[256] = { 0 };
			get_item_text(hItem, buffer, 256);
			return std::tstring(buffer);
		}

		template<typename T>
		T* get_item_data_typed(HTREEITEM hItem) const {
			return reinterpret_cast<T*>(get_item_data(hItem));
		}

		template<typename T>
		BOOL set_item_data_typed(HTREEITEM hItem, T* data) {
			return set_item_data(hItem, reinterpret_cast<DWORD_PTR>(data));
		}

		HTREEITEM add_child(HTREEITEM hParent, LPCTSTR lpszText, int nImage = -1, int nSelectedImage = -1) {
			if (nImage >= 0) {
				return insert_item(lpszText, nImage, nSelectedImage, hParent, TVI_LAST);
			}
			return insert_item(lpszText, hParent, TVI_LAST);
		}

		HTREEITEM add_root(LPCTSTR lpszText, int nImage = -1, int nSelectedImage = -1) {
			return add_child(TVI_ROOT, lpszText, nImage, nSelectedImage);
		}

		HTREEITEM add_child_with_data(HTREEITEM hParent, LPCTSTR lpszText, DWORD_PTR data, int nImage = -1) {
			HTREEITEM hItem = add_child(hParent, lpszText, nImage, nImage);
			if (hItem) {
				set_item_data(hItem, data);
			}
			return hItem;
		}

		template<typename T>
		HTREEITEM add_child_with_data_typed(HTREEITEM hParent, LPCTSTR lpszText, T* data, int nImage = -1) {
			return add_child_with_data(hParent, lpszText, reinterpret_cast<DWORD_PTR>(data), nImage);
		}

		BOOL is_item_expanded(HTREEITEM hItem) const {
			return (get_item_state(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) != 0;
		}

		BOOL is_item_selected(HTREEITEM hItem) const {
			return (get_item_state(hItem, TVIS_SELECTED) & TVIS_SELECTED) != 0;
		}

		BOOL is_item_bold(HTREEITEM hItem) const {
			return (get_item_state(hItem, TVIS_BOLD) & TVIS_BOLD) != 0;
		}

		BOOL is_item_cut(HTREEITEM hItem) const {
			return (get_item_state(hItem, TVIS_CUT) & TVIS_CUT) != 0;
		}

		void set_item_bold(HTREEITEM hItem, BOOL bBold = TRUE) {
			set_item_state(hItem, bBold ? TVIS_BOLD : 0, TVIS_BOLD);
		}

		BOOL collapse(HTREEITEM hItem) {
			return expand(hItem, TVE_COLLAPSE);
		}

		BOOL toggle(HTREEITEM hItem) {
			return expand(hItem, TVE_TOGGLE);
		}

		BOOL expand_all(HTREEITEM hItem = TVI_ROOT) {
			if (hItem == TVI_ROOT) {
				hItem = get_root_item();
			}
			
			if (!hItem) return FALSE;
			
			expand(hItem, TVE_EXPAND);
			
			HTREEITEM hChild = get_child_item(hItem);
			while (hChild) {
				expand_all(hChild);
				hChild = get_next_sibling_item(hChild);
			}
			return TRUE;
		}

		BOOL collapse_all(HTREEITEM hItem = TVI_ROOT) {
			if (hItem == TVI_ROOT) {
				hItem = get_root_item();
			}
			
			if (!hItem) return FALSE;
			
			HTREEITEM hChild = get_child_item(hItem);
			while (hChild) {
				collapse_all(hChild);
				hChild = get_next_sibling_item(hChild);
			}
			
			collapse(hItem);
			return TRUE;
		}

		int get_child_count(HTREEITEM hItem, BOOL bRecursive = FALSE) const {
			int count = 0;
			HTREEITEM hChild = get_child_item(hItem);
			
			while (hChild) {
				count++;
				if (bRecursive) {
					count += get_child_count(hChild, TRUE);
				}
				hChild = get_next_sibling_item(hChild);
			}
			return count;
		}

		int get_level(HTREEITEM hItem) const {
			int level = 0;
			HTREEITEM hParent = get_parent_item(hItem);
			while (hParent) {
				level++;
				hParent = get_parent_item(hParent);
			}
			return level;
		}

		HTREEITEM find_item(LPCTSTR lpszText, HTREEITEM hStart = TVI_ROOT, BOOL bExact = TRUE, BOOL bCaseSensitive = FALSE) const {
			if (hStart == TVI_ROOT) {
				hStart = get_root_item();
			}
			
			if (!hStart) return NULL;

			std::tstring searchText = lpszText;
			if (!bCaseSensitive) {
				std::transform(searchText.begin(), searchText.end(), searchText.begin(), ::_totupper);
			}
			
			HTREEITEM hCurrent = hStart;
			while (hCurrent) {
				std::tstring itemText = get_item_text_str(hCurrent);
				if (!bCaseSensitive) {
					std::transform(itemText.begin(), itemText.end(), itemText.begin(), ::_totupper);
				}
				
				if (bExact) {
					if (itemText == searchText) return hCurrent;
				} else {
					if (itemText.find(searchText) != std::tstring::npos) return hCurrent;
				}
				
				HTREEITEM hChild = get_child_item(hCurrent);
				if (hChild) {
					HTREEITEM hFound = find_item(lpszText, hChild, bExact, bCaseSensitive);
					if (hFound) return hFound;
				}
				
				hCurrent = get_next_sibling_item(hCurrent);
			}
			return NULL;
		}

		HTREEITEM find_item_by_data(DWORD_PTR data, HTREEITEM hStart = TVI_ROOT) const {
			if (hStart == TVI_ROOT) {
				hStart = get_root_item();
			}
			
			if (!hStart) return NULL;
			
			HTREEITEM hCurrent = hStart;
			while (hCurrent) {
				if (get_item_data(hCurrent) == data) return hCurrent;
				
				HTREEITEM hChild = get_child_item(hCurrent);
				if (hChild) {
					HTREEITEM hFound = find_item_by_data(data, hChild);
					if (hFound) return hFound;
				}
				
				hCurrent = get_next_sibling_item(hCurrent);
			}
			return NULL;
		}

		template<typename T>
		HTREEITEM find_item_by_data_typed(T* data, HTREEITEM hStart = TVI_ROOT) const {
			return find_item_by_data(reinterpret_cast<DWORD_PTR>(data), hStart);
		}

		std::vector<HTREEITEM> get_all_items(HTREEITEM hStart = TVI_ROOT) const {
			std::vector<HTREEITEM> items;
			
			if (hStart == TVI_ROOT) {
				hStart = get_root_item();
			}
			
			if (!hStart) return items;
			
			HTREEITEM hCurrent = hStart;
			while (hCurrent) {
				items.push_back(hCurrent);
				
				HTREEITEM hChild = get_child_item(hCurrent);
				if (hChild) {
					std::vector<HTREEITEM> childItems = get_all_items(hChild);
					items.insert(items.end(), childItems.begin(), childItems.end());
				}
				
				hCurrent = get_next_sibling_item(hCurrent);
			}
			return items;
		}

		std::vector<HTREEITEM> get_children(HTREEITEM hParent) const {
			std::vector<HTREEITEM> children;
			HTREEITEM hChild = get_child_item(hParent);
			
			while (hChild) {
				children.push_back(hChild);
				hChild = get_next_sibling_item(hChild);
			}
			return children;
		}

		std::vector<HTREEITEM> get_path_to_item(HTREEITEM hItem) const {
			std::vector<HTREEITEM> path;
			HTREEITEM hCurrent = hItem;
			
			while (hCurrent) {
				path.insert(path.begin(), hCurrent);
				hCurrent = get_parent_item(hCurrent);
			}
			return path;
		}

		void for_each_item(std::function<void(HTREEITEM)> func, HTREEITEM hStart = TVI_ROOT) {
			if (hStart == TVI_ROOT) {
				hStart = get_root_item();
			}
			
			if (!hStart) return;
			
			HTREEITEM hCurrent = hStart;
			while (hCurrent) {
				func(hCurrent);
				
				HTREEITEM hChild = get_child_item(hCurrent);
				if (hChild) {
					for_each_item(func, hChild);
				}
				
				hCurrent = get_next_sibling_item(hCurrent);
			}
		}

		BOOL delete_children(HTREEITEM hItem) {
			HTREEITEM hChild = get_child_item(hItem);
			while (hChild) {
				HTREEITEM hNext = get_next_sibling_item(hChild);
				if (!delete_item(hChild)) return FALSE;
				hChild = hNext;
			}
			return TRUE;
		}

		BOOL has_children(HTREEITEM hItem) const {
			return item_has_children(hItem);
		}

		BOOL is_root_item(HTREEITEM hItem) const {
			return get_parent_item(hItem) == NULL;
		}

		int get_sibling_index(HTREEITEM hItem) const {
			HTREEITEM hParent = get_parent_item(hItem);
			HTREEITEM hSibling = get_child_item(hParent ? hParent : TVI_ROOT);
			
			int index = 0;
			while (hSibling) {
				if (hSibling == hItem) return index;
				index++;
				hSibling = get_next_sibling_item(hSibling);
			}
			return -1;
		}

		BOOL select_and_expand(HTREEITEM hItem) {
			if (!is_valid_item(hItem)) return FALSE;
			expand(hItem, TVE_EXPAND);
			return select_item(hItem);
		}

		BOOL select_and_ensure_visible(HTREEITEM hItem) {
			if (!is_valid_item(hItem)) return FALSE;
			ensure_visible(hItem);
			return select_item(hItem);
		}

		void check_all_items(BOOL bCheck = TRUE, HTREEITEM hStart = TVI_ROOT) {
			for_each_item([this, bCheck](HTREEITEM hItem) {
				set_check_state(hItem, bCheck);
			}, hStart);
		}

		void uncheck_all_items(HTREEITEM hStart = TVI_ROOT) {
			check_all_items(FALSE, hStart);
		}

		int get_checked_count(HTREEITEM hStart = TVI_ROOT) const {
			int count = 0;
			std::vector<HTREEITEM> items = get_all_items(hStart);
			for (HTREEITEM hItem : items) {
				if (get_check_state(hItem)) count++;
			}
			return count;
		}

		std::vector<HTREEITEM> get_checked_items(HTREEITEM hStart = TVI_ROOT) const {
			std::vector<HTREEITEM> checked;
			std::vector<HTREEITEM> items = get_all_items(hStart);
			for (HTREEITEM hItem : items) {
				if (get_check_state(hItem)) checked.push_back(hItem);
			}
			return checked;
		}

		BOOL has_extended_style(DWORD dwStyle) const {
			return (get_extended_style() & dwStyle) == dwStyle;
		}

		void add_extended_style(DWORD dwStyle) {
			set_extended_style(dwStyle, dwStyle);
		}

		void remove_extended_style(DWORD dwStyle) {
			set_extended_style(0, dwStyle);
		}

		BOOL is_checkboxes_enabled() const {
			return has_extended_style(TVS_EX_DOUBLEBUFFER);
		}

		void enable_checkboxes() {
			add_extended_style(TVS_CHECKBOXES);
		}

		void enable_doublebuffer() {
			add_extended_style(TVS_EX_DOUBLEBUFFER);
		}

		void set_colors(COLORREF bkColor, COLORREF textColor, COLORREF lineColor = CLR_DEFAULT) {
			set_bk_color(bkColor);
			set_text_color(textColor);
			set_line_color(lineColor);
		}

		void set_default_colors() {
			set_bk_color(RGB(255, 255, 255));
			set_text_color(RGB(0, 0, 0));
			set_line_color(CLR_DEFAULT);
		}

		void set_dark_colors() {
			set_bk_color(RGB(30, 30, 30));
			set_text_color(RGB(241, 241, 241));
			set_line_color(RGB(80, 80, 80));
		}

		HTREEITEM get_last_child(HTREEITEM hParent) const {
			HTREEITEM hChild = get_child_item(hParent);
			if (!hChild) return NULL;
			
			HTREEITEM hLast = hChild;
			while (hChild) {
				hLast = hChild;
				hChild = get_next_sibling_item(hChild);
			}
			return hLast;
		}

		BOOL move_item(HTREEITEM hItem, HTREEITEM hNewParent, HTREEITEM hInsertAfter = TVI_LAST) {
			if (!is_valid_item(hItem)) return FALSE;
			
			TVITEM item = { 0 };
			item.hItem = hItem;
			item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_STATE;
			item.stateMask = (UINT)-1;
			item.pszText = new TCHAR[256];
			item.cchTextMax = 256;
			
			if (!get_item(&item)) {
				delete[] item.pszText;
				return FALSE;
			}
			
			BOOL wasExpanded = is_item_expanded(hItem);
			
			HTREEITEM hNewItem = insert_item(item.mask, item.pszText, item.iImage, 
				item.iSelectedImage, item.state, item.stateMask, item.lParam, 
				hNewParent, hInsertAfter);
			
			delete[] item.pszText;
			
			if (!hNewItem) return FALSE;
			
			std::vector<HTREEITEM> children = get_children(hItem);
			for (HTREEITEM hChild : children) {
				move_item(hChild, hNewItem, TVI_LAST);
			}
			
			if (wasExpanded) {
				expand(hNewItem, TVE_EXPAND);
			}
			
			delete_item(hItem);
			return TRUE;
		}

		BOOL is_ancestor(HTREEITEM hAncestor, HTREEITEM hItem) const {
			HTREEITEM hParent = get_parent_item(hItem);
			while (hParent) {
				if (hParent == hAncestor) return TRUE;
				hParent = get_parent_item(hParent);
			}
			return FALSE;
		}

		HTREEITEM hit_test(POINT pt) const {
			TVHITTESTINFO hti = { 0 };
			hti.pt = pt;
			return hit_test(&hti);
		}

		void clear() {
			delete_all_items();
		}

		BOOL has_items() const {
			return !is_empty();
		}

		std::tstring get_selected_text() const {
			HTREEITEM hItem = get_selected_item();
			return hItem ? get_item_text_str(hItem) : std::tstring();
		}

		DWORD_PTR get_selected_data() const {
			HTREEITEM hItem = get_selected_item();
			return hItem ? get_item_data(hItem) : 0;
		}

		template<typename T>
		T* get_selected_data_typed() const {
			HTREEITEM hItem = get_selected_item();
			return hItem ? get_item_data_typed<T>(hItem) : nullptr;
		}

		BOOL item_exists(LPCTSTR lpszText, HTREEITEM hStart = TVI_ROOT) const {
			return find_item(lpszText, hStart, TRUE) != NULL;
		}

		void sort_all_children(HTREEITEM hStart = TVI_ROOT, BOOL bRecurse = TRUE) {
			if (hStart == TVI_ROOT) {
				hStart = get_root_item();
			}
			
			if (!hStart) return;
			
			HTREEITEM hCurrent = hStart;
			while (hCurrent) {
				sort_children(hCurrent, bRecurse);
				hCurrent = get_next_sibling_item(hCurrent);
			}
		}

		HTREEITEM get_prev_item(HTREEITEM hItem) const {
			HTREEITEM hParent = get_parent_item(hItem);
			HTREEITEM hSibling = get_child_item(hParent ? hParent : TVI_ROOT);
			HTREEITEM hPrev = NULL;
			
			while (hSibling && hSibling != hItem) {
				hPrev = hSibling;
				hSibling = get_next_sibling_item(hSibling);
			}
			return hPrev;
		}

		void toggle_item_expansion(HTREEITEM hItem) {
			if (is_item_expanded(hItem)) {
				collapse(hItem);
			} else {
				expand(hItem, TVE_EXPAND);
			}
		}

		int get_expanded_count(HTREEITEM hStart = TVI_ROOT) const {
			int count = 0;
			std::vector<HTREEITEM> items = get_all_items(hStart);
			for (HTREEITEM hItem : items) {
				if (is_item_expanded(hItem)) count++;
			}
			return count;
		}

		RECT get_item_rect_ex(HTREEITEM hItem, BOOL bTextOnly = TRUE) const {
			RECT rc = { 0 };
			get_item_rect(hItem, &rc, bTextOnly);
			return rc;
		}
	};
}

#endif //__TREE_VIEW_HPP__