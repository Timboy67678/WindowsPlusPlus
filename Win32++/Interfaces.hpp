#ifndef __INTERFACES_H__
#define __INTERFACES_H__

/********************************************************************************/
/* All of these are taken from WTL, so big thanks to the author, info is below */
/********************************************************************************/

// The use and distribution terms for this software are covered by the
// Microsoft Public License (http://opensource.org/licenses/MS-PL)
// which can be found in the file MS-PL.txt at the root folder.

#include "winplusplus.h"

#define COMMAND_HANDLER(X) virtual INT_PTR CALLBACK X (INT wControlID, HWND hWnd, WPARAM wParam, LPARAM lParam)
#define COMMAND_REF(X) static_cast<WPP::Wnd::COMMAND_MESSAGE_CALLBACK>(X)

namespace WPP
{
	class Wnd
	{
	public:
		typedef INT_PTR(CALLBACK Wnd::*COMMAND_MESSAGE_CALLBACK)(INT, HWND, WPARAM, LPARAM);

		Wnd(int resource_id, HWND parent = NULL)
			: m_ItemID(resource_id), m_Parent(parent), m_hWnd(NULL)
		{}

		Wnd(HWND handle)
			: m_hWnd(handle), m_ItemID(-1)
		{}

		virtual HWND GetHandle() const { return m_hWnd; }
		virtual HWND GetParent() const { return m_Parent; }
		virtual int GetID() const { return m_ItemID; }

		virtual std::tstring GetText()
		{
			TCHAR title_buffer[1024] = {0};
			int text_length = ::GetWindowText(m_hWnd, title_buffer, ARRAYSIZE(title_buffer));
			return std::tstring(title_buffer, text_length);
		}

		virtual int GetTextLength()
		{
			return ::GetWindowTextLength(m_hWnd);
		}

		virtual BOOL SetText(const std::tstring &text)
		{
			return ::SetWindowText(m_hWnd, text.c_str());
		}

		virtual BOOL SetShowing(int state = SW_NORMAL)
		{
			return ::ShowWindow(m_hWnd, state);
		}

		virtual BOOL SetEnabled(BOOL enabled = TRUE)
		{
			return ::EnableWindow(m_hWnd, enabled);
		}

		virtual HWND Focus()
		{
			return ::SetFocus(m_hWnd);
		}

		virtual BOOL IsFocused()
		{
			return ::GetForegroundWindow() == m_hWnd;
		}

		UINT GetStyle() const
		{
			return (UINT) ::GetWindowLong(m_hWnd, GWL_STYLE) & 0xFFFF;
		}

		LONG AddStyle(DWORD dwStyle)
		{
			DWORD new_style = ::GetWindowLong(m_hWnd, GWL_STYLE) | dwStyle;
			return ::SetWindowLong(m_hWnd, GWL_STYLE, new_style);
		}

		//Stolen From ATL
		BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
		{
			DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
			DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
			if (dwStyle == dwNewStyle) return FALSE;
			::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
			if (nFlags != 0)
				::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
			return TRUE;
		}

	protected:
		int m_ItemID;
		HWND m_hWnd, m_Parent;
	};

	class Control : public Wnd
	{
	public:
		Control(int resource_id, HWND parent = NULL)
			: Wnd(resource_id, parent)
		{
			if (parent != NULL)
				m_hWnd = ::GetDlgItem(m_Parent, resource_id);
		}

		Control(HWND handle) : Wnd(handle)
		{}

		void SetItem(HWND parent, int item_id)
		{
			m_ItemID = item_id;
			m_Parent = parent;
			m_hWnd = ::GetDlgItem(parent, item_id);
		}
	};

	class ImageList
	{
	public:
		HIMAGELIST m_hImageList;

		ImageList(HIMAGELIST hImageList = NULL) : m_hImageList(hImageList)
		{}

		operator HIMAGELIST() const { return m_hImageList; }

		bool IsNull() const { return (m_hImageList == NULL); }

		// Attributes
		int GetImageCount() const
		{
			return ImageList_GetImageCount(m_hImageList);
		}

		COLORREF GetBkColor() const
		{
			return ImageList_GetBkColor(m_hImageList);
		}

		COLORREF SetBkColor(COLORREF cr)
		{
			return ImageList_SetBkColor(m_hImageList, cr);
		}

		BOOL GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const
		{
			return ImageList_GetImageInfo(m_hImageList, nImage, pImageInfo);
		}

		HICON GetIcon(int nIndex, UINT uFlags = ILD_NORMAL) const
		{
			return ImageList_GetIcon(m_hImageList, nIndex, uFlags);
		}

		BOOL GetIconSize(int& cx, int& cy) const
		{
			return ImageList_GetIconSize(m_hImageList, &cx, &cy);
		}

		BOOL GetIconSize(SIZE& size) const
		{
			return ImageList_GetIconSize(m_hImageList, (int*) &size.cx, (int*) &size.cy);
		}

		BOOL SetIconSize(int cx, int cy)
		{
			return ImageList_SetIconSize(m_hImageList, cx, cy);
		}

		BOOL SetIconSize(SIZE size)
		{
			return ImageList_SetIconSize(m_hImageList, size.cx, size.cy);
		}

		BOOL SetImageCount(UINT uNewCount)
		{
			return ImageList_SetImageCount(m_hImageList, uNewCount);
		}

		BOOL SetOverlayImage(int nImage, int nOverlay)
		{
			return ImageList_SetOverlayImage(m_hImageList, nImage, nOverlay);
		}

		// Operations
		BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow)
		{
			m_hImageList = ImageList_Create(cx, cy, nFlags, nInitial, nGrow);
			return (m_hImageList != NULL) ? TRUE : FALSE;
		}

		BOOL Create(int nBimapID, int cx, int nGrow, COLORREF crMask)
		{
			m_hImageList = ImageList_LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(nBimapID), cx, nGrow, crMask);
			return (m_hImageList != NULL) ? TRUE : FALSE;
		}

		BOOL CreateFromImage(int nImageID, int cx, int nGrow, COLORREF crMask, UINT uType, UINT uFlags = LR_DEFAULTCOLOR | LR_DEFAULTSIZE)
		{
			m_hImageList = ImageList_LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(nImageID), cx, nGrow, crMask, uType, uFlags);
			return (m_hImageList != NULL) ? TRUE : FALSE;
		}

		BOOL Merge(HIMAGELIST hImageList1, int nImage1, HIMAGELIST hImageList2, int nImage2, int dx, int dy)
		{
			m_hImageList = ImageList_Merge(hImageList1, nImage1, hImageList2, nImage2, dx, dy);
			return (m_hImageList != NULL) ? TRUE : FALSE;
		}

#ifndef _WIN32_WCE
#ifdef __IStream_INTERFACE_DEFINED__
		BOOL CreateFromStream(LPSTREAM lpStream)
		{
			m_hImageList = ImageList_Read(lpStream);
			return (m_hImageList != NULL) ? TRUE : FALSE;
		}
#endif // __IStream_INTERFACE_DEFINED__
#endif // !_WIN32_WCE

		BOOL Destroy()
		{
			if (m_hImageList == NULL) return FALSE;
			BOOL bRet = ImageList_Destroy(m_hImageList);
			if (bRet) m_hImageList = NULL;
			return bRet;
		}

		int Add(HBITMAP hBitmap, HBITMAP hBitmapMask = NULL)
		{
			return ImageList_Add(m_hImageList, hBitmap, hBitmapMask);
		}

		int Add(HBITMAP hBitmap, COLORREF crMask)
		{
			return ImageList_AddMasked(m_hImageList, hBitmap, crMask);
		}

		BOOL Remove(int nImage)
		{
			return ImageList_Remove(m_hImageList, nImage);
		}

		BOOL RemoveAll()
		{
			return ImageList_RemoveAll(m_hImageList);
		}

		BOOL Replace(int nImage, HBITMAP hBitmap, HBITMAP hBitmapMask)
		{
			return ImageList_Replace(m_hImageList, nImage, hBitmap, hBitmapMask);
		}

		int AddIcon(HICON hIcon)
		{
			return ImageList_AddIcon(m_hImageList, hIcon);
		}

		int ReplaceIcon(int nImage, HICON hIcon)
		{
			return ImageList_ReplaceIcon(m_hImageList, nImage, hIcon);
		}

		HICON ExtractIcon(int nImage)
		{
			return ImageList_ExtractIcon(NULL, m_hImageList, nImage);
		}

		BOOL Draw(HDC hDC, int nImage, int x, int y, UINT nStyle)
		{
			return ImageList_Draw(m_hImageList, nImage, hDC, x, y, nStyle);
		}

		BOOL Draw(HDC hDC, int nImage, POINT pt, UINT nStyle)
		{
			return ImageList_Draw(m_hImageList, nImage, hDC, pt.x, pt.y, nStyle);
		}

		BOOL DrawEx(int nImage, HDC hDC, int x, int y, int dx, int dy, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle)
		{
			return ImageList_DrawEx(m_hImageList, nImage, hDC, x, y, dx, dy, rgbBk, rgbFg, fStyle);
		}

		BOOL DrawEx(int nImage, HDC hDC, RECT& rect, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle)
		{
			return ImageList_DrawEx(m_hImageList, nImage, hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, rgbBk, rgbFg, fStyle);
		}

		static BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pimldp)
		{
			return ImageList_DrawIndirect(pimldp);
		}

		BOOL Copy(int nSrc, int nDst, UINT uFlags = ILCF_MOVE)
		{
			return ImageList_Copy(m_hImageList, nDst, m_hImageList, nSrc, uFlags);
		}

#ifdef __IStream_INTERFACE_DEFINED__
#ifndef _WIN32_WCE
		static HIMAGELIST Read(LPSTREAM lpStream)
		{
			return ImageList_Read(lpStream);
		}

		BOOL Write(LPSTREAM lpStream)
		{
			return ImageList_Write(m_hImageList, lpStream);
		}
#endif // !_WIN32_WCE

#if (_WIN32_WINNT >= 0x0501)
		static HRESULT ReadEx(DWORD dwFlags, LPSTREAM lpStream, REFIID riid, PVOID* ppv)
		{
			return ImageList_ReadEx(dwFlags, lpStream, riid, ppv);
		}

		HRESULT WriteEx(DWORD dwFlags, LPSTREAM lpStream)
		{
			return ImageList_WriteEx(m_hImageList, dwFlags, lpStream);
		}
#endif // (_WIN32_WINNT >= 0x0501)
#endif // __IStream_INTERFACE_DEFINED__

		// Drag operations
		BOOL BeginDrag(int nImage, POINT ptHotSpot)
		{
			return ImageList_BeginDrag(m_hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
		}

		BOOL BeginDrag(int nImage, int xHotSpot, int yHotSpot)
		{
			return ImageList_BeginDrag(m_hImageList, nImage, xHotSpot, yHotSpot);
		}

		static void EndDrag()
		{
			ImageList_EndDrag();
		}

		static BOOL DragMove(POINT pt)
		{
			return ImageList_DragMove(pt.x, pt.y);
		}

		static BOOL DragMove(int x, int y)
		{
			return ImageList_DragMove(x, y);
		}

		BOOL SetDragCursorImage(int nDrag, POINT ptHotSpot)
		{
			return ImageList_SetDragCursorImage(m_hImageList, nDrag, ptHotSpot.x, ptHotSpot.y);
		}

		BOOL SetDragCursorImage(int nDrag, int xHotSpot, int yHotSpot)
		{
			return ImageList_SetDragCursorImage(m_hImageList, nDrag, xHotSpot, yHotSpot);
		}

		static BOOL DragShowNolock(BOOL bShow = TRUE)
		{
			return ImageList_DragShowNolock(bShow);
		}

		static ImageList GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot)
		{
			return ImageList(ImageList_GetDragImage(lpPoint, lpPointHotSpot));
		}

		static BOOL DragEnter(HWND hWnd, POINT point)
		{
			return ImageList_DragEnter(hWnd, point.x, point.y);
		}

		static BOOL DragEnter(HWND hWnd, int x, int y)
		{
			return ImageList_DragEnter(hWnd, x, y);
		}

		static BOOL DragLeave(HWND hWnd)
		{
			return ImageList_DragLeave(hWnd);
		}

#if (_WIN32_IE >= 0x0400)
		ImageList Duplicate() const
		{
			return ImageList(ImageList_Duplicate(m_hImageList));
		}

		static ImageList Duplicate(HIMAGELIST hImageList)
		{
			return ImageList(ImageList_Duplicate(hImageList));
		}
#endif // (_WIN32_IE >= 0x0400)
	};

	class Static : public Control
	{
	public:
		using Control::Control;

		HICON GetIcon()
		{
			return (HICON) SendMessage(m_hWnd, STM_GETICON, 0, 0L);
		}

		HICON SetIcon(HICON hIcon)
		{
			return (HICON) SendMessage(m_hWnd, STM_SETICON, (WPARAM) hIcon, 0L);
		}

		HENHMETAFILE GetEnhMetaFile()
		{
			return (HENHMETAFILE) SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);
		}

		HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile)
		{
			return (HENHMETAFILE) SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM) hMetaFile);
		}

		HBITMAP GetBitmap()
		{
			return (HBITMAP) SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_BITMAP, 0L);
		}

		HBITMAP SetBitmap(HBITMAP hBitmap)
		{
			return (HBITMAP) SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hBitmap);
		}

		HCURSOR GetCursor()
		{
			return (HCURSOR) SendMessage(m_hWnd, STM_GETIMAGE, IMAGE_CURSOR, 0L);
		}

		HCURSOR SetCursor(HCURSOR hCursor)
		{
			return (HCURSOR) SendMessage(m_hWnd, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM) hCursor);
		}
	};

	class Button : public Control
	{
	public:
		using Control::Control;

		void SetChecked(BOOL checked = FALSE)
		{
			Button_SetCheck(m_hWnd, checked ? BST_CHECKED : BST_UNCHECKED);
		}

		int GetState()
		{
			return Button_GetState(m_hWnd);
		}

		void SetState(int state)
		{
			Button_SetState(m_hWnd, state);
		}

		BOOL IsChecked()
		{
			return GetState() == BST_CHECKED;
		}

		void SetStyle(UINT style, BOOL redraw = TRUE)
		{
			Button_SetStyle(m_hWnd, style, redraw);
		}

		HICON GetIcon()
		{
			return (HICON) SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L);
		}

		HICON SetIcon(HICON hIcon)
		{
			return (HICON) SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM) hIcon);
		}

		HBITMAP GetBitmap()
		{
			return (HBITMAP) SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L);
		}

		HBITMAP SetBitmap(HBITMAP hBitmap)
		{
			return (HBITMAP) SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hBitmap);
		}

		void SetShield(BOOL shield_state)
		{
			Button_SetElevationRequiredState(m_hWnd, shield_state);
		}

		void EmulatePress()
		{
			SendMessage(m_hWnd, BM_CLICK, 0, 0L);
		}
	};

	class CheckBox : public Button
	{
	public:
		using Button::Button;
	};

	class ListBox : public Control
	{
	public:
		using Control::Control;

		int GetListCount()
		{
			return ListBox_GetCount(m_hWnd);
		}

		int GetHorizonExtent()
		{
			return ListBox_GetHorizontalExtent(m_hWnd);
		}

		void SetHorizonExtent(int extent)
		{
			ListBox_SetHorizontalExtent(m_hWnd, extent);
		}

		int GetTopIndex()
		{
			return ListBox_GetTopIndex(m_hWnd);
		}

		int SetTopIndex(int top_index)
		{
			return ListBox_SetTopIndex(m_hWnd, top_index);
		}

		LCID GetLocale()
		{
			return (LCID) SendMessage(m_hWnd, LB_GETLOCALE, 0, 0L);
		}

		LCID SetLocale(LCID locale)
		{
			return (LCID) SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM) locale, 0L);
		}

		DWORD GetListboxInfo()
		{
			return (DWORD) SendMessage(m_hWnd, LB_GETLISTBOXINFO, 0, 0L);
		}

		int GetSelected()
		{
			return ListBox_GetCurSel(m_hWnd);
		}

		int SetSelected(int index)
		{
			return ListBox_SetCurSel(m_hWnd, index);
		}

		int GetMultiSel(int index)
		{
			return ListBox_GetSel(m_hWnd, index);
		}

		int SetMultiSel(int index, BOOL selected = TRUE)
		{
			return ListBox_SetSel(m_hWnd, selected, index);
		}

		int GetMultiSelCount()
		{
			return ListBox_GetSelCount(m_hWnd);
		}

		int GetSelectedIndexs(int max_count, int *return_indexs)
		{
			return ListBox_GetSelItems(m_hWnd, max_count, return_indexs);
		}

		int GetAnchorIndex()
		{
			return (int) SendMessage(m_hWnd, LB_GETANCHORINDEX, 0, 0L);
		}

		void SetAnchorIndex(int nIndex)
		{
			SendMessage(m_hWnd, LB_SETANCHORINDEX, nIndex, 0L);
		}

		int GetCarretIndex()
		{
			return ListBox_GetCaretIndex(m_hWnd);
		}

		int SetCarretIndex(int index, BOOL scroll_to = TRUE)
		{
			return (int) SendMessage(m_hWnd, LB_SETCARETINDEX, index, MAKELONG(scroll_to, 0));
		}

		DWORD_PTR GetItemData(int index)
		{
			return ListBox_GetItemData(m_hWnd, index);
		}

		int SetItemData(int index, DWORD_PTR item_data)
		{
			return ListBox_SetItemData(m_hWnd, index, item_data);
		}

		template < typename R >
		R *GetItemDataPtr(int index)
		{
			return reinterpret_cast<R*>(GetItemData(index));
		}

		template < typename T >
		int SetItemDataPtr(int index, T *data)
		{
			return SetItemData(index, reinterpret_cast<T*>(data));
		}

		int GetItemRect(int index, LPRECT rect_out)
		{
			return ListBox_GetItemRect(m_hWnd, index, rect_out);
		}

		int GetItemTextLen(int index)
		{
			return ListBox_GetTextLen(m_hWnd, index);
		}

		int GetItemHeight(int index)
		{
			return ListBox_GetItemHeight(m_hWnd, index);
		}

		int SetItemHeight(int index, int item_height)
		{
			return ListBox_SetItemHeight(m_hWnd, index, item_height);
		}

		void SetColumnWidth(int width)
		{
			ListBox_SetColumnWidth(m_hWnd, width);
		}

		BOOL SetTabStops(int tab_stop_num, int * tab_stops)
		{
			return ListBox_SetTabStops(m_hWnd, tab_stop_num, tab_stops);
		}

		BOOL SetTabStops()
		{
			return ListBox_SetTabStops(m_hWnd, 0, 0L);
		}

		BOOL SetTabStops(const int &each_step)
		{
			return ListBox_SetTabStops(m_hWnd, 1, &each_step);
		}

		int InitStorage(int item_count, UINT storage_initial_bytesize)
		{
			return (int) SendMessage(m_hWnd, LB_INITSTORAGE, (WPARAM) item_count, (LPARAM) storage_initial_bytesize);
		}

		void ResetContent()
		{
			ListBox_ResetContent(m_hWnd);
		}

		int ItemFromPoint(POINT position, BOOL& is_outside)
		{
			DWORD dw = (DWORD) SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(position.x, position.y));
			is_outside = (BOOL) HIWORD(dw);
			return (int) LOWORD(dw);
		}

		int Remove(int index)
		{
			return ListBox_DeleteString(m_hWnd, index);
		}

		int GetItemText(int index, LPCTSTR buffer)
		{
			return ListBox_GetText(m_hWnd, index, buffer);
		}

		int Add(LPCTSTR string)
		{
			return ListBox_AddString(m_hWnd, string);
		}

		int Insert(int index, LPCTSTR string)
		{
			return ListBox_InsertString(m_hWnd, index, string);
		}

		int AddDir(int attribute, LPCTSTR file_spec)
		{
			return ListBox_Dir(m_hWnd, attribute, file_spec);
		}

		int AddFile(LPCTSTR file_path)
		{
			return (int) SendMessage(m_hWnd, LB_ADDFILE, 0, (LPARAM) file_path);
		}

		int FindString(int start_index, LPCTSTR item_string) const
		{
			return (int) SendMessage(m_hWnd, LB_FINDSTRING, start_index, (LPARAM) item_string);
		}

		int FindStringExact(int start_index, LPCTSTR item_string) const
		{
			return (int) SendMessage(m_hWnd, LB_FINDSTRINGEXACT, start_index, (LPARAM) item_string);
		}

		int SelectString(int start_index, LPCTSTR item_string)
		{
			return (int) SendMessage(m_hWnd, LB_SELECTSTRING, start_index, (LPARAM) item_string);
		}

		int SelItemRange(BOOL bSelect, int first_item, int last_item)
		{
			return bSelect ?
				(int) SendMessage(m_hWnd, LB_SELITEMRANGEEX, first_item, last_item) :
				(int) SendMessage(m_hWnd, LB_SELITEMRANGEEX, last_item, first_item);
		}
	};

	class ComboBox : public Control
	{
	public:
		using Control::Control;

		int GetCount()
		{
			return ComboBox_GetCount(m_hWnd);
		}

		int GetCurSel()
		{
			return ComboBox_GetCurSel(m_hWnd);
		}

		int SetCurSel(int index)
		{
			return ComboBox_SetCurSel(m_hWnd, index);
		}

		LCID GetLocale()
		{
			return (LCID) SendMessage(m_hWnd, CB_GETLOCALE, 0, 0L);
		}

		LCID SetLocale(LCID locale)
		{
			return (LCID) SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM) locale, 0L);
		}

		int GetTopIndex()
		{
			return (int) SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0L);
		}

		int SetTopIndex(int index)
		{
			return (int) SendMessage(m_hWnd, CB_SETTOPINDEX, index, 0L);
		}

		UINT GetHorizontalExtent()
		{
			return (UINT) SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0L);
		}

		void SetHorizontalExtent(UINT extent)
		{
			SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, extent, 0L);
		}

		int GetDroppedWidth()
		{
			return (int) SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0L);
		}

		int SetDroppedWidth(UINT width)
		{
			return (int) SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, width, 0L);
		}

		BOOL GetComboBoxInfo(PCOMBOBOXINFO combobox_info)
		{
			return (BOOL) SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM) combobox_info);
		}

		DWORD GetEditSel()
		{
			return (DWORD) SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0L);
		}

		BOOL SetEditSel(int start_index, int end_index)
		{
			return (BOOL) SendMessage(m_hWnd, CB_SETEDITSEL, 0, MAKELONG(start_index, end_index));
		}

		DWORD_PTR GetItemData(int index)
		{
			return (DWORD_PTR) SendMessage(m_hWnd, CB_GETITEMDATA, index, 0L);
		}

		int SetItemData(int index, DWORD_PTR item_data)
		{
			return (int) SendMessage(m_hWnd, CB_SETITEMDATA, index, (LPARAM) item_data);
		}

		int GetLBText(int index, LPTSTR string)
		{
			return (int) SendMessage(m_hWnd, CB_GETLBTEXT, index, (LPARAM) string);
		}

		int GetLBTextLen(int index)
		{
			return (int) SendMessage(m_hWnd, CB_GETLBTEXTLEN, index, 0L);
		}

		std::tstring GetSelectedText()
		{
			TCHAR text[1024] = {0};
			if (GetLBText(GetCurSel(), text) != LB_ERR)
				return text;
			return std::tstring();
		}

		int GetItemHeight(int index)
		{
			return (int) SendMessage(m_hWnd, CB_GETITEMHEIGHT, index, 0L);
		}

		int SetItemHeight(int index, UINT item_height)
		{
			return (int) SendMessage(m_hWnd, CB_SETITEMHEIGHT, index, MAKELONG(item_height, 0));
		}

		BOOL GetExtendedUI()
		{
			return (BOOL) SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L);
		}

		int SetExtendedUI(BOOL extended = TRUE)
		{
			return (int) SendMessage(m_hWnd, CB_SETEXTENDEDUI, extended, 0L);
		}

		void GetDroppedControlRect(LPRECT lprect)
		{
			SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (LPARAM) lprect);
		}

		BOOL GetDroppedState()
		{
			return (BOOL) SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L);
		}

		int GetMinVisible()
		{
			return (int) SendMessage(m_hWnd, CB_GETMINVISIBLE, 0, 0L);
		}

		BOOL SetMinVisible(int min_vis)
		{
			return (BOOL) SendMessage(m_hWnd, CB_SETMINVISIBLE, min_vis, 0L);
		}

		BOOL GetCueBannerText(LPWSTR lpwText, int cchText) const
		{
			return (BOOL) SendMessage(m_hWnd, CB_GETCUEBANNER, (WPARAM) lpwText, cchText);
		}

		BOOL SetCueBannerText(LPCWSTR lpcwText)
		{
			return (BOOL) SendMessage(m_hWnd, CB_SETCUEBANNER, 0, (LPARAM) lpcwText);
		}

		int InitStorage(int item_count, UINT byte_count)
		{
			return (int) SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM) item_count, byte_count);
		}

		void ResetContent()
		{
			SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0L);
		}

		BOOL LimitText(int max_char)
		{
			return (BOOL) SendMessage(m_hWnd, CB_LIMITTEXT, max_char, 0L);
		}

		void ShowDropDown(BOOL show_dropdown = TRUE)
		{
			SendMessage(m_hWnd, CB_SHOWDROPDOWN, show_dropdown, 0L);
		}

		int Add(LPCTSTR lpszString)
		{
			return (int) SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM) lpszString);
		}

		int Remove(UINT index)
		{
			return (int) SendMessage(m_hWnd, CB_DELETESTRING, index, 0L);
		}

		int InsertString(int index, LPCTSTR lpszString)
		{
			return (int) SendMessage(m_hWnd, CB_INSERTSTRING, index, (LPARAM) lpszString);
		}

		int AddDir(UINT attr, LPCTSTR lpszWildCard)
		{
			return (int) SendMessage(m_hWnd, CB_DIR, attr, (LPARAM) lpszWildCard);
		}

		int FindString(int start_index, LPCTSTR lpszString) const
		{
			return (int) SendMessage(m_hWnd, CB_FINDSTRING, start_index, (LPARAM) lpszString);
		}

		int FindStringExact(int start_index, LPCTSTR lpszFind) const
		{
			return (int) SendMessage(m_hWnd, CB_FINDSTRINGEXACT, start_index, (LPARAM) lpszFind);
		}

		int SelectString(int start_index, LPCTSTR lpszString)
		{
			return (int) SendMessage(m_hWnd, CB_SELECTSTRING, start_index, (LPARAM) lpszString);
		}

		void Clear()
		{
			SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
		}

		void Copy()
		{
			SendMessage(m_hWnd, WM_COPY, 0, 0L);
		}

		void Cut()
		{
			SendMessage(m_hWnd, WM_CUT, 0, 0L);
		}

		void Paste()
		{
			SendMessage(m_hWnd, WM_PASTE, 0, 0L);
		}
	};

	class EditText : public Control
	{
	public:
		using Control::Control;

		BOOL CanUndo()
		{
			return Edit_CanUndo(m_hWnd);
		}

		int GetLineCount() const
		{
			return (int) SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
		}

		BOOL GetModify() const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETMODIFY, 0, 0L);
		}

		void SetModify(BOOL bModified = TRUE)
		{
			SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0L);
		}

		void GetRect(LPRECT lpRect) const
		{
			SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM) lpRect);
		}

		DWORD GetSel() const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETSEL, 0, 0L);
		}

		void GetSel(int& nStartChar, int& nEndChar) const
		{
			SendMessage(m_hWnd, EM_GETSEL, (WPARAM) &nStartChar, (LPARAM) &nEndChar);
		}

		HLOCAL GetResHandle() const
		{
			return (HLOCAL) SendMessage(m_hWnd, EM_GETHANDLE, 0, 0L);
		}

		void SetHandle(HLOCAL hBuffer)
		{
			SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM) hBuffer, 0L);
		}

		DWORD GetMargins() const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETMARGINS, 0, 0L);
		}

		void GetMargins(UINT& nLeft, UINT& nRight) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, EM_GETMARGINS, 0, 0L);
			nLeft = LOWORD(dwRet);
			nRight = HIWORD(dwRet);
		}

		void SetMargins(UINT nLeft, UINT nRight, WORD wFlags = EC_LEFTMARGIN | EC_RIGHTMARGIN)
		{
			SendMessage(m_hWnd, EM_SETMARGINS, wFlags, MAKELONG(nLeft, nRight));
		}

		UINT GetLimitText() const
		{
			return (UINT) SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
		}

		void SetTextLimit(UINT nMax)
		{
			SendMessage(m_hWnd, EM_SETLIMITTEXT, nMax, 0L);
		}

		POINT PosFromChar(UINT nChar) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, EM_POSFROMCHAR, nChar, 0);
			POINT point = {GET_X_LPARAM(dwRet), GET_Y_LPARAM(dwRet)};
			return point;
		}

		int CharFromPos(POINT pt, int* pLine = NULL) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
			if (pLine != NULL)
				*pLine = (int) (short) HIWORD(dwRet);
			return (int) (short) LOWORD(dwRet);
		}

		int GetLine(int nIndex, LPTSTR lpszBuffer) const
		{
			return (int) SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM) lpszBuffer);
		}

		int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
		{
			*(LPWORD) lpszBuffer = (WORD) nMaxLength;
			return (int) SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM) lpszBuffer);
		}

		TCHAR GetPasswordChar() const
		{
			return (TCHAR) SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L);
		}

		void SetPasswordChar(TCHAR ch)
		{
			SendMessage(m_hWnd, EM_SETPASSWORDCHAR, ch, 0L);
		}

		void SetUsingPassword(BOOL bPassword, TCHAR pass_char = '*')
		{
			SetPasswordChar(bPassword ? pass_char : 0);
		}

		EDITWORDBREAKPROC GetWordBreakProc() const
		{
			return (EDITWORDBREAKPROC) SendMessage(m_hWnd, EM_GETWORDBREAKPROC, 0, 0L);
		}

		void SetWordBreakProc(EDITWORDBREAKPROC ewbprc)
		{
			SendMessage(m_hWnd, EM_SETWORDBREAKPROC, 0, (LPARAM) ewbprc);
		}

		int GetFirstVisibleLine() const
		{
			return (int) SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
		}

		int GetThumb() const
		{
			return (int) SendMessage(m_hWnd, EM_GETTHUMB, 0, 0L);
		}

		BOOL SetReadOnly(BOOL bReadOnly = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
		}

		UINT GetImeStatus(UINT uStatus) const
		{
			return (UINT) SendMessage(m_hWnd, EM_GETIMESTATUS, uStatus, 0L);
		}

		UINT SetImeStatus(UINT uStatus, UINT uData)
		{
			return (UINT) SendMessage(m_hWnd, EM_SETIMESTATUS, uStatus, uData);
		}

		BOOL GetCueBannerText(LPCWSTR lpstrText, int cchText) const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETCUEBANNER, (WPARAM) lpstrText, cchText);
		}

		BOOL SetCueBannerText(LPCWSTR lpstrText, BOOL bKeepWithFocus = FALSE)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETCUEBANNER, (WPARAM) bKeepWithFocus, (LPARAM) lpstrText);
		}

		void EmptyUndoBuffer()
		{
			SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
		}

		BOOL FmtLines(BOOL bAddEOL)
		{
			return (BOOL) SendMessage(m_hWnd, EM_FMTLINES, bAddEOL, 0L);
		}

		void LimitText(int nChars = 0)
		{
			SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0L);
		}

		int LineFromChar(int nIndex = -1) const
		{
			return (int) SendMessage(m_hWnd, EM_LINEFROMCHAR, nIndex, 0L);
		}

		int LineIndex(int nLine = -1) const
		{
			return (int) SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0L);
		}

		int LineLength(int nLine = -1) const
		{
			return (int) SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0L);
		}

		void LineScroll(int nLines, int nChars = 0)
		{
			SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
		}

		void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE)
		{
			SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM) lpszNewText);
		}

		void SetRect(LPCRECT lpRect)
		{
			SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM) lpRect);
		}

		void SetRectNP(LPCRECT lpRect)
		{
			SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM) lpRect);
		}

		void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE)
		{
			SendMessage(m_hWnd, EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
			if (!bNoScroll)
				SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
		}

		void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE)
		{
			SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
			if (!bNoScroll)
				SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
		}

		void SetSelAll(BOOL bNoScroll = FALSE)
		{
			SetSel(0, -1, bNoScroll);
		}

		void SetSelNone(BOOL bNoScroll = FALSE)
		{
			SetSel(-1, 0, bNoScroll);
		}

		BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM) rgTabStops);
		}

		BOOL SetTabStops()
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);
		}

		BOOL SetTabStops(const int& cxEachStop)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM) (LPINT) &cxEachStop);
		}

		void ScrollCaret()
		{
			SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
		}

		int Scroll(int nScrollAction)
		{
			LRESULT lRet = SendMessage(m_hWnd, EM_SCROLL, nScrollAction, 0L);
			if (!(BOOL) HIWORD(lRet)) return -1;
			return (int) (short) LOWORD(lRet);
		}

		void InsertText(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE)
		{
			SetSel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
			ReplaceSel(lpstrText, bCanUndo);
		}

		void AppendText(LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE)
		{
			InsertText(GetTextLength(), lpstrText, bNoScroll, bCanUndo);
		}

		BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBaloonTip)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SHOWBALLOONTIP, 0, (LPARAM) pEditBaloonTip);
		}

		BOOL HideBalloonTip()
		{
			return (BOOL) SendMessage(m_hWnd, EM_HIDEBALLOONTIP, 0, 0L);
		}

		DWORD GetHilite() const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETHILITE, 0, 0L);
		}

		void GetHilite(int& nStartChar, int& nEndChar) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, EM_GETHILITE, 0, 0L);
			nStartChar = (int) (short) LOWORD(dwRet);
			nEndChar = (int) (short) HIWORD(dwRet);
		}

		void SetHilite(int nStartChar, int nEndChar)
		{
			SendMessage(m_hWnd, EM_SETHILITE, nStartChar, nEndChar);
		}

		BOOL Undo()
		{
			return (BOOL) SendMessage(m_hWnd, EM_UNDO, 0, 0L);
		}

		void Clear()
		{
			SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
		}

		void Copy()
		{
			SendMessage(m_hWnd, WM_COPY, 0, 0L);
		}

		void Cut()
		{
			SendMessage(m_hWnd, WM_CUT, 0, 0L);
		}

		void Paste()
		{
			SendMessage(m_hWnd, WM_PASTE, 0, 0L);
		}
	};

	class ScrollBar : public Control
	{
	public:
		using Control::Control;

		int GetScrollPos() const
		{
			return ::GetScrollPos(m_hWnd, SB_CTL);
		}

		int SetScrollPos(int nPos, BOOL bRedraw = TRUE)
		{
			return ::SetScrollPos(m_hWnd, SB_CTL, nPos, bRedraw);
		}

		void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
		{
			::GetScrollRange(m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
		}

		void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE)
		{
			::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw);
		}

		BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo) const
		{
			return ::GetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo);
		}

		int SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE)
		{
			return ::SetScrollInfo(m_hWnd, SB_CTL, lpScrollInfo, bRedraw);
		}

		int GetScrollLimit() const
		{
			SCROLLINFO info = {sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE};
			::GetScrollInfo(m_hWnd, SB_CTL, &info);
			if (info.nPage > 1)
				info.nMax -= info.nPage - 1;

			return info.nMax;
		}

		BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollBarInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, SBM_GETSCROLLBARINFO, 0, (LPARAM) pScrollBarInfo);
		}

		void ShowScrollBar(BOOL bShow = TRUE)
		{
			::ShowScrollBar(m_hWnd, SB_CTL, bShow);
		}

		BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH)
		{
			return ::EnableScrollBar(m_hWnd, SB_CTL, nArrowFlags);
		}
	};

	class CToolInfo : public TOOLINFO
	{
	public:
		CToolInfo(UINT nFlags, HWND hWnd, UINT_PTR nIDTool = 0, LPRECT lpRect = NULL, LPTSTR lpstrText = LPSTR_TEXTCALLBACK, LPARAM lUserParam = NULL)
		{
			Init(nFlags, hWnd, nIDTool, lpRect, lpstrText, lUserParam);
		}

		operator LPTOOLINFO() { return this; }

		operator LPARAM() { return (LPARAM)this; }

		void Init(UINT nFlags, HWND hWnd, UINT_PTR nIDTool = 0, LPRECT lpRect = NULL, LPTSTR lpstrText = LPSTR_TEXTCALLBACK, LPARAM lUserParam = NULL)
		{
			memset(this, 0, sizeof(TOOLINFO));
			cbSize = sizeof(TOOLINFO);
			uFlags = nFlags;
			if (nIDTool == 0)
			{
				hwnd = ::GetParent(hWnd);
				uFlags |= TTF_IDISHWND;
				uId = (UINT_PTR) hWnd;
			} else
			{
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

	class ToolTip : public Control
	{
	public:
		using Control::Control;

		void GetText(LPTOOLINFO lpToolInfo) const
		{
			SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM) &lpToolInfo);
		}

		void GetText(LPTSTR lpstrText, HWND hWnd, UINT_PTR nIDTool = 0) const
		{
			CToolInfo ti(0, hWnd, nIDTool, NULL, lpstrText);
			SendMessage(m_hWnd, TTM_GETTEXT, 0, ti);
		}

		BOOL GetToolInfo(LPTOOLINFO lpToolInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM) lpToolInfo);
		}

		BOOL GetToolInfo(HWND hWnd, UINT_PTR nIDTool, UINT* puFlags, LPRECT lpRect, LPTSTR lpstrText) const
		{
			CToolInfo ti(0, hWnd, nIDTool, NULL, lpstrText);
			BOOL bRet = (BOOL) SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, ti);
			if (bRet != FALSE)
			{
				*puFlags = ti.uFlags;
				*lpRect = ti.rect;
			}
			return bRet;
		}

		void SetToolInfo(LPTOOLINFO lpToolInfo)
		{
			SendMessage(m_hWnd, TTM_SETTOOLINFO, 0, (LPARAM) lpToolInfo);
		}

		void SetToolRect(LPTOOLINFO lpToolInfo)
		{
			SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM) lpToolInfo);
		}

		void SetToolRect(HWND hWnd, UINT_PTR nIDTool, LPCRECT lpRect)
		{
			CToolInfo ti(0, hWnd, nIDTool, (LPRECT) lpRect, NULL);
			SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, ti);
		}

		int GetToolCount() const
		{
			return (int) SendMessage(m_hWnd, TTM_GETTOOLCOUNT, 0, 0L);
		}

		int GetDelayTime(DWORD dwType) const
		{
			return (int) SendMessage(m_hWnd, TTM_GETDELAYTIME, dwType, 0L);
		}

		void SetDelayTime(DWORD dwType, int nTime)
		{
			SendMessage(m_hWnd, TTM_SETDELAYTIME, dwType, MAKELPARAM(nTime, 0));
		}

		void GetMargin(LPRECT lpRect) const
		{
			SendMessage(m_hWnd, TTM_GETMARGIN, 0, (LPARAM) lpRect);
		}

		void SetMargin(LPRECT lpRect)
		{
			SendMessage(m_hWnd, TTM_SETMARGIN, 0, (LPARAM) lpRect);
		}

		int GetMaxTipWidth() const
		{
			return (int) SendMessage(m_hWnd, TTM_GETMAXTIPWIDTH, 0, 0L);
		}

		int SetMaxTipWidth(int nWidth)
		{
			return (int) SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, nWidth);
		}

		COLORREF GetTipBkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TTM_GETTIPBKCOLOR, 0, 0L);
		}

		void SetTipBkColor(COLORREF clr)
		{
			SendMessage(m_hWnd, TTM_SETTIPBKCOLOR, (WPARAM) clr, 0L);
		}

		COLORREF GetTipTextColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TTM_GETTIPTEXTCOLOR, 0, 0L);
		}

		void SetTipTextColor(COLORREF clr)
		{
			SendMessage(m_hWnd, TTM_SETTIPTEXTCOLOR, (WPARAM) clr, 0L);
		}

		BOOL GetCurrentTool(LPTOOLINFO lpToolInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, TTM_GETCURRENTTOOL, 0, (LPARAM) lpToolInfo);
		}

		SIZE GetBubbleSize(LPTOOLINFO lpToolInfo) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, TTM_GETBUBBLESIZE, 0, (LPARAM) lpToolInfo);
			SIZE size = {GET_X_LPARAM(dwRet), GET_Y_LPARAM(dwRet)};
			return size;
		}

		BOOL SetTitle(UINT_PTR uIcon, LPCTSTR lpstrTitle)
		{
			return (BOOL) SendMessage(m_hWnd, TTM_SETTITLE, uIcon, (LPARAM) lpstrTitle);
		}

		BOOL SetTitle(HICON hIcon, LPCTSTR lpstrTitle)
		{
			return (BOOL) SendMessage(m_hWnd, TTM_SETTITLE, (WPARAM) hIcon, (LPARAM) lpstrTitle);
		}
		void GetTitle(PTTGETTITLE pTTGetTitle) const
		{
			SendMessage(m_hWnd, TTM_GETTITLE, 0, (LPARAM) pTTGetTitle);
		}

		void SetWindowTheme(LPCWSTR lpstrTheme)
		{
			SendMessage(m_hWnd, TTM_SETWINDOWTHEME, 0, (LPARAM) lpstrTheme);
		}

		void Activate(BOOL bActivate)
		{
			SendMessage(m_hWnd, TTM_ACTIVATE, bActivate, 0L);
		}

		BOOL AddTool(LPTOOLINFO lpToolInfo)
		{
			return (BOOL) SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM) lpToolInfo);
		}

		BOOL AddTool(HWND hWnd, LPTSTR text = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT_PTR nIDTool = 0)
		{
			CToolInfo ti(0, hWnd, nIDTool, (LPRECT) lpRectTool, (LPTSTR) text);
			return (BOOL) SendMessage(m_hWnd, TTM_ADDTOOL, 0, ti);
		}

		void DelTool(LPTOOLINFO lpToolInfo)
		{
			SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM) lpToolInfo);
		}

		void DelTool(HWND hWnd, UINT_PTR nIDTool = 0)
		{
			CToolInfo ti(0, hWnd, nIDTool, NULL, NULL);
			SendMessage(m_hWnd, TTM_DELTOOL, 0, ti);
		}

		BOOL HitTest(LPTTHITTESTINFO lpHitTestInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM) lpHitTestInfo);
		}

		BOOL HitTest(HWND hWnd, POINT pt, LPTOOLINFO lpToolInfo) const
		{
			TTHITTESTINFO hti = {0};
			hti.ti.cbSize = sizeof(TOOLINFO);
			hti.hwnd = hWnd;
			hti.pt = pt;

			if ((BOOL) SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM) &hti) != FALSE)
			{
				*lpToolInfo = hti.ti;
				return TRUE;
			}
			return FALSE;
		}

		void RelayEvent(LPMSG lpMsg)
		{
			SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM) lpMsg);
		}

		void UpdateTipText(LPTOOLINFO lpToolInfo)
		{
			SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM) lpToolInfo);
		}

		void UpdateTipText(LPCTSTR text, HWND hWnd, UINT_PTR nIDTool = 0)
		{
			CToolInfo ti(0, hWnd, nIDTool, NULL, (LPTSTR) text);
			SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, ti);
		}

		BOOL EnumTools(UINT_PTR nTool, LPTOOLINFO lpToolInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, TTM_ENUMTOOLS, nTool, (LPARAM) lpToolInfo);
		}

		void Pop()
		{
			SendMessage(m_hWnd, TTM_POP, 0, 0L);
		}

		void TrackActivate(LPTOOLINFO lpToolInfo, BOOL bActivate)
		{
			SendMessage(m_hWnd, TTM_TRACKACTIVATE, bActivate, (LPARAM) lpToolInfo);
		}

		void TrackActivate(HWND hWnd, UINT_PTR nIDTool, BOOL bActivate)
		{
			CToolInfo ti(0, hWnd, nIDTool);
			SendMessage(m_hWnd, TTM_TRACKACTIVATE, bActivate, ti);
		}

		void TrackPosition(int xPos, int yPos)
		{
			SendMessage(m_hWnd, TTM_TRACKPOSITION, 0, MAKELPARAM(xPos, yPos));
		}

		void Update()
		{
			SendMessage(m_hWnd, TTM_UPDATE, 0, 0L);
		}

		BOOL AdjustRect(LPRECT lpRect, BOOL bLarger = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TTM_ADJUSTRECT, bLarger, (LPARAM) lpRect);
		}

		void Popup()
		{
			SendMessage(m_hWnd, TTM_POPUP, 0, 0L);
		}
	};

	class Header : public Control
	{
	public:
		using Control::Control;

		int GetItemCount() const
		{
			return (int) SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0L);
		}

		BOOL GetItem(int nIndex, LPHDITEM pHeaderItem) const
		{
			return (BOOL) SendMessage(m_hWnd, HDM_GETITEM, nIndex, (LPARAM) pHeaderItem);
		}

		BOOL SetItem(int nIndex, LPHDITEM pHeaderItem)
		{
			return (BOOL) SendMessage(m_hWnd, HDM_SETITEM, nIndex, (LPARAM) pHeaderItem);
		}

		ImageList GetImageList() const
		{
			return  ImageList((HIMAGELIST) SendMessage(m_hWnd, HDM_GETIMAGELIST, 0, 0L));
		}

		ImageList SetImageList(HIMAGELIST hImageList)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, HDM_SETIMAGELIST, 0, (LPARAM) hImageList));
		}

		BOOL GetOrderArray(int nSize, int* lpnArray) const
		{
			return (BOOL) SendMessage(m_hWnd, HDM_GETORDERARRAY, nSize, (LPARAM) lpnArray);
		}

		BOOL SetOrderArray(int nSize, int* lpnArray)
		{
			return (BOOL) SendMessage(m_hWnd, HDM_SETORDERARRAY, nSize, (LPARAM) lpnArray);
		}

		BOOL GetItemRect(int nIndex, LPRECT lpItemRect) const
		{
			return (BOOL) SendMessage(m_hWnd, HDM_GETITEMRECT, nIndex, (LPARAM) lpItemRect);
		}

		int SetHotDivider(BOOL bPos, DWORD dwInputValue)
		{
			return (int) SendMessage(m_hWnd, HDM_SETHOTDIVIDER, bPos, dwInputValue);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, HDM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, HDM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int GetBitmapMargin() const
		{
			return (int) SendMessage(m_hWnd, HDM_GETBITMAPMARGIN, 0, 0L);
		}

		int SetBitmapMargin(int nWidth)
		{
			return (int) SendMessage(m_hWnd, HDM_SETBITMAPMARGIN, nWidth, 0L);
		}

		int SetFilterChangeTimeout(DWORD dwTimeOut)
		{
			return (int) SendMessage(m_hWnd, HDM_SETFILTERCHANGETIMEOUT, 0, dwTimeOut);
		}

		BOOL GetItemDropDownRect(int nIndex, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, HDM_GETITEMDROPDOWNRECT, nIndex, (LPARAM) lpRect);
		}

		BOOL GetOverflowRect(LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, HDM_GETOVERFLOWRECT, 0, (LPARAM) lpRect);
		}

		int GetFocusedItem() const
		{
			return (int) SendMessage(m_hWnd, HDM_GETFOCUSEDITEM, 0, 0L);
		}

		BOOL SetFocusedItem(int nIndex)
		{
			return (BOOL) SendMessage(m_hWnd, HDM_SETFOCUSEDITEM, 0, nIndex);
		}

		int InsertItem(int nIndex, LPHDITEM phdi)
		{
			return (int) SendMessage(m_hWnd, HDM_INSERTITEM, nIndex, (LPARAM) phdi);
		}

		int AddItem(LPHDITEM phdi)
		{
			return InsertItem(GetItemCount(), phdi);
		}

		BOOL DeleteItem(int nIndex)
		{
			return (BOOL) SendMessage(m_hWnd, HDM_DELETEITEM, nIndex, 0L);
		}

		BOOL Layout(HD_LAYOUT* pHeaderLayout)
		{
			return (BOOL) SendMessage(m_hWnd, HDM_LAYOUT, 0, (LPARAM) pHeaderLayout);
		}

		int HitTest(LPHDHITTESTINFO lpHitTestInfo) const
		{
			return (int) SendMessage(m_hWnd, HDM_HITTEST, 0, (LPARAM) lpHitTestInfo);
		}

		int OrderToIndex(int nOrder)
		{
			return (int) SendMessage(m_hWnd, HDM_ORDERTOINDEX, nOrder, 0L);
		}

		ImageList CreateDragImage(int nIndex)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, HDM_CREATEDRAGIMAGE, nIndex, 0L));
		}

		int EditFilter(int nColumn, BOOL bDiscardChanges)
		{
			return (int) SendMessage(m_hWnd, HDM_EDITFILTER, nColumn, MAKELPARAM(bDiscardChanges, 0));
		}

		int ClearFilter(int nColumn)
		{
			return (int) SendMessage(m_hWnd, HDM_CLEARFILTER, nColumn, 0L);
		}

		int ClearAllFilters()
		{
			return (int) SendMessage(m_hWnd, HDM_CLEARFILTER, (WPARAM) -1, 0L);
		}
	};

	class ListView : public Control
	{
	public:
		using Control::Control;

		COLORREF GetBkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_GETBKCOLOR, 0, 0L);
		}

		BOOL SetBkColor(COLORREF cr)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETBKCOLOR, 0, cr);
		}

		ImageList GetImageList(int nImageListType) const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, LVM_GETIMAGELIST, nImageListType, 0L));
		}

		ImageList SetImageList(HIMAGELIST hImageList, int nImageList)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, LVM_SETIMAGELIST, nImageList, (LPARAM) hImageList));
		}

		int GetItemCount() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETITEMCOUNT, 0, 0L);
		}

		BOOL SetItemCount(int nItems)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, 0L);
		}

		BOOL GetItem(LPLVITEM pItem) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM) pItem);
		}

		BOOL SetItem(const LVITEM* pItem)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM) pItem);
		}

		BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
					 int nImage, UINT nState, UINT nStateMask, LPARAM lParam)
		{
			LVITEM lvi = {0};
			lvi.mask = nMask;
			lvi.iItem = nItem;
			lvi.iSubItem = nSubItem;
			lvi.stateMask = nStateMask;
			lvi.state = nState;
			lvi.pszText = (LPTSTR) lpszItem;
			lvi.iImage = nImage;
			lvi.lParam = lParam;
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM) &lvi);
		}

		UINT GetItemState(int nItem, UINT nMask) const
		{
			return (UINT) SendMessage(m_hWnd, LVM_GETITEMSTATE, nItem, nMask);
		}

		BOOL SetItemState(int nItem, UINT nState, UINT nStateMask)
		{
			LVITEM lvi = {0};
			lvi.state = nState;
			lvi.stateMask = nStateMask;
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM) &lvi);
		}

		BOOL SetItemState(int nItem, LPLVITEM pItem)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM) pItem);
		}

		int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
		{
			LVITEM lvi = {0};
			lvi.iSubItem = nSubItem;
			lvi.cchTextMax = nLen;
			lvi.pszText = lpszText;
			return (int) SendMessage(m_hWnd, LVM_GETITEMTEXT, (WPARAM) nItem, (LPARAM) &lvi);
		}

		BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
		{
			return SetItem(nItem, nSubItem, LVIF_TEXT, lpszText, 0, 0, 0, 0);
		}

		DWORD_PTR GetItemData(int nItem) const
		{
			LVITEM lvi = {0};
			lvi.iItem = nItem;
			lvi.mask = LVIF_PARAM;
			BOOL bRet = (BOOL) SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM) &lvi);
			return (DWORD_PTR) (bRet ? lvi.lParam : NULL);
		}

		BOOL SetItemData(int nItem, DWORD_PTR dwData)
		{
			return SetItem(nItem, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM) dwData);
		}

		UINT GetCallbackMask() const
		{
			return (UINT) SendMessage(m_hWnd, LVM_GETCALLBACKMASK, 0, 0L);
		}

		BOOL SetCallbackMask(UINT nMask)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETCALLBACKMASK, nMask, 0L);
		}

		BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETITEMPOSITION, nItem, (LPARAM) lpPoint);
		}

		BOOL SetItemPosition(int nItem, POINT pt)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM) &pt);
		}

		BOOL SetItemPosition(int nItem, int x, int y)
		{
			POINT pt = {x, y};
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMPOSITION32, nItem, (LPARAM) &pt);
		}

		int GetStringWidth(LPCTSTR lpsz) const
		{
			return (int) SendMessage(m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM) lpsz);
		}

		EditText GetEditControl() const
		{
			return EditText((HWND) SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L));
		}

		BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETCOLUMN, nCol, (LPARAM) pColumn);
		}

		BOOL SetColumn(int nCol, const LVCOLUMN* pColumn)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETCOLUMN, nCol, (LPARAM) pColumn);
		}

		int GetColumnWidth(int nCol) const
		{
			return (int) SendMessage(m_hWnd, LVM_GETCOLUMNWIDTH, nCol, 0L);
		}

		BOOL SetColumnWidth(int nCol, int cx)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETCOLUMNWIDTH, nCol, MAKELPARAM(cx, 0));
		}

		BOOL GetViewRect(LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETVIEWRECT, 0, (LPARAM) lpRect);
		}

		COLORREF GetTextColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_GETTEXTCOLOR, 0, 0L);
		}

		BOOL SetTextColor(COLORREF cr)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETTEXTCOLOR, 0, cr);
		}

		COLORREF GetTextBkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_GETTEXTBKCOLOR, 0, 0L);
		}

		BOOL SetTextBkColor(COLORREF cr)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETTEXTBKCOLOR, 0, cr);
		}

		int GetTopIndex() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETTOPINDEX, 0, 0L);
		}

		int GetCountPerPage() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETCOUNTPERPAGE, 0, 0L);
		}

		BOOL GetOrigin(LPPOINT lpPoint) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETORIGIN, 0, (LPARAM) lpPoint);
		}

		UINT GetSelectedCount() const
		{
			return (UINT) SendMessage(m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0L);
		}

		BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const
		{
			lpRect->left = nCode;
			return (BOOL) SendMessage(m_hWnd, LVM_GETITEMRECT, (WPARAM) nItem, (LPARAM) lpRect);
		}

		HCURSOR GetHotCursor() const
		{
			return (HCURSOR) SendMessage(m_hWnd, LVM_GETHOTCURSOR, 0, 0L);
		}

		HCURSOR SetHotCursor(HCURSOR hHotCursor)
		{
			return (HCURSOR) SendMessage(m_hWnd, LVM_SETHOTCURSOR, 0, (LPARAM) hHotCursor);
		}

		int GetHotItem() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETHOTITEM, 0, 0L);
		}

		int SetHotItem(int nIndex)
		{
			return (int) SendMessage(m_hWnd, LVM_SETHOTITEM, nIndex, 0L);
		}

		BOOL GetColumnOrderArray(int nCount, int* lpnArray) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETCOLUMNORDERARRAY, nCount, (LPARAM) lpnArray);
		}

		BOOL SetColumnOrderArray(int nCount, int* lpnArray)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETCOLUMNORDERARRAY, nCount, (LPARAM) lpnArray);
		}

		Header GetHeader() const
		{
			return Header((HWND) SendMessage(m_hWnd, LVM_GETHEADER, 0, 0L));
		}

		BOOL GetSubItemRect(int nItem, int nSubItem, int nFlag, LPRECT lpRect) const
		{
			lpRect->top = nSubItem;
			lpRect->left = nFlag;
			return (BOOL) SendMessage(m_hWnd, LVM_GETSUBITEMRECT, nItem, (LPARAM) lpRect);
		}

		DWORD SetIconSpacing(int cx, int cy)
		{
			return (DWORD) SendMessage(m_hWnd, LVM_SETICONSPACING, 0, MAKELPARAM(cx, cy));
		}

		int GetISearchString(LPTSTR lpstr) const
		{
			return (int) SendMessage(m_hWnd, LVM_GETISEARCHSTRING, 0, (LPARAM) lpstr);
		}

		void GetItemSpacing(SIZE& sizeSpacing, BOOL bSmallIconView = FALSE) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, LVM_GETITEMSPACING, bSmallIconView, 0L);
			sizeSpacing.cx = GET_X_LPARAM(dwRet);
			sizeSpacing.cy = GET_Y_LPARAM(dwRet);
		}

		int GetSelectedIndex() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM) -1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));
		}

		BOOL GetSelectedItem(LPLVITEM pItem) const
		{
			pItem->iItem = (int) SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM) -1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));
			if (pItem->iItem == -1) return FALSE;
			return (BOOL) SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM) pItem);
		}

		DWORD GetExtendedListViewStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0L);
		}

		DWORD SetExtendedListViewStyle(DWORD dwExStyle, DWORD dwExMask = 0)
		{
			return (DWORD) SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, dwExMask, dwExStyle);
		}

		BOOL GetCheckState(int nIndex) const
		{
			UINT uRet = GetItemState(nIndex, LVIS_STATEIMAGEMASK);
			return (uRet >> 12) - 1;
		}

		BOOL SetCheckState(int nItem, BOOL bCheck)
		{
			int nCheck = bCheck ? 2 : 1;
			return SetItemState(nItem, INDEXTOSTATEIMAGEMASK(nCheck), LVIS_STATEIMAGEMASK);
		}

		DWORD GetViewType() const
		{
			return (GetStyle() & LVS_TYPEMASK);
		}

		DWORD SetViewType(DWORD dwType)
		{
			DWORD dwOldType = GetViewType();
			if (dwType != dwOldType) ModifyStyle(LVS_TYPEMASK, (dwType & LVS_TYPEMASK));
			return dwOldType;
		}

		BOOL GetBkImage(LPLVBKIMAGE plvbki) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETBKIMAGE, 0, (LPARAM) plvbki);
		}

		BOOL SetBkImage(LPLVBKIMAGE plvbki)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETBKIMAGE, 0, (LPARAM) plvbki);
		}

		int GetSelectionMark() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETSELECTIONMARK, 0, 0L);
		}

		int SetSelectionMark(int nIndex)
		{
			return (int) SendMessage(m_hWnd, LVM_SETSELECTIONMARK, 0, nIndex);
		}

		BOOL GetWorkAreas(int nWorkAreas, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETWORKAREAS, nWorkAreas, (LPARAM) lpRect);
		}

		BOOL SetWorkAreas(int nWorkAreas, LPRECT lpRect)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETWORKAREAS, nWorkAreas, (LPARAM) lpRect);
		}

		DWORD GetHoverTime() const
		{
			return (DWORD) SendMessage(m_hWnd, LVM_GETHOVERTIME, 0, 0L);
		}

		DWORD SetHoverTime(DWORD dwHoverTime)
		{
			return (DWORD) SendMessage(m_hWnd, LVM_SETHOVERTIME, 0, dwHoverTime);
		}

		BOOL GetNumberOfWorkAreas(int* pnWorkAreas) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETNUMBEROFWORKAREAS, 0, (LPARAM) pnWorkAreas);
		}

		BOOL SetItemCountEx(int nItems, DWORD dwFlags)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMCOUNT, nItems, dwFlags);
		}

		ToolTip GetToolTips() const
		{
			return ToolTip((HWND) SendMessage(m_hWnd, LVM_GETTOOLTIPS, 0, 0L));
		}

		ToolTip SetToolTips(HWND hWndTT)
		{
			return ToolTip((HWND) SendMessage(m_hWnd, LVM_SETTOOLTIPS, (WPARAM) hWndTT, 0L));
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int GetSelectedColumn() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETSELECTEDCOLUMN, 0, 0L);
		}

		void SetSelectedColumn(int nColumn)
		{
			SendMessage(m_hWnd, LVM_SETSELECTEDCOLUMN, nColumn, 0L);
		}

		DWORD GetView() const
		{
			return (DWORD) SendMessage(m_hWnd, LVM_GETVIEW, 0, 0L);
		}

		int SetView(DWORD dwView)
		{
			return (int) SendMessage(m_hWnd, LVM_SETVIEW, dwView, 0L);
		}

		BOOL IsGroupViewEnabled() const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_ISGROUPVIEWENABLED, 0, 0L);
		}

		int GetGroupInfo(int nGroupID, PLVGROUP pGroup) const
		{
			return (int) SendMessage(m_hWnd, LVM_GETGROUPINFO, nGroupID, (LPARAM) pGroup);
		}

		int SetGroupInfo(int nGroupID, PLVGROUP pGroup)
		{
			return (int) SendMessage(m_hWnd, LVM_SETGROUPINFO, nGroupID, (LPARAM) pGroup);
		}

		void GetGroupMetrics(PLVGROUPMETRICS pGroupMetrics) const
		{
			SendMessage(m_hWnd, LVM_GETGROUPMETRICS, 0, (LPARAM) pGroupMetrics);
		}

		void SetGroupMetrics(PLVGROUPMETRICS pGroupMetrics)
		{
			SendMessage(m_hWnd, LVM_SETGROUPMETRICS, 0, (LPARAM) pGroupMetrics);
		}

		void GetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo) const
		{
			SendMessage(m_hWnd, LVM_GETTILEVIEWINFO, 0, (LPARAM) pTileViewInfo);
		}

		BOOL SetTileViewInfo(PLVTILEVIEWINFO pTileViewInfo)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETTILEVIEWINFO, 0, (LPARAM) pTileViewInfo);
		}

		void GetTileInfo(PLVTILEINFO pTileInfo) const
		{
			SendMessage(m_hWnd, LVM_GETTILEINFO, 0, (LPARAM) pTileInfo);
		}

		BOOL SetTileInfo(PLVTILEINFO pTileInfo)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETTILEINFO, 0, (LPARAM) pTileInfo);
		}

		BOOL GetInsertMark(LPLVINSERTMARK pInsertMark) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETINSERTMARK, 0, (LPARAM) pInsertMark);
		}

		BOOL SetInsertMark(LPLVINSERTMARK pInsertMark)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETINSERTMARK, 0, (LPARAM) pInsertMark);
		}

		int GetInsertMarkRect(LPRECT lpRect) const
		{
			return (int) SendMessage(m_hWnd, LVM_GETINSERTMARKRECT, 0, (LPARAM) lpRect);
		}

		COLORREF GetInsertMarkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_GETINSERTMARKCOLOR, 0, 0L);
		}

		COLORREF SetInsertMarkColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_SETINSERTMARKCOLOR, 0, clr);
		}

		COLORREF GetOutlineColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_GETOUTLINECOLOR, 0, 0L);
		}

		COLORREF SetOutlineColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, LVM_SETOUTLINECOLOR, 0, clr);
		}

		int GetGroupCount() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETGROUPCOUNT, 0, 0L);
		}

		BOOL GetGroupInfoByIndex(int nIndex, PLVGROUP pGroup) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETGROUPINFOBYINDEX, nIndex, (LPARAM) pGroup);
		}

		BOOL GetGroupRect(int nGroupID, int nType, LPRECT lpRect) const
		{
			if (lpRect != NULL) lpRect->top = nType;
			return (BOOL) SendMessage(m_hWnd, LVM_GETGROUPRECT, nGroupID, (LPARAM) lpRect);
		}

		UINT GetGroupState(int nGroupID, UINT uMask) const
		{
			return (UINT) SendMessage(m_hWnd, LVM_GETGROUPSTATE, nGroupID, (LPARAM) uMask);
		}

		int GetFocusedGroup() const
		{
			return (int) SendMessage(m_hWnd, LVM_GETFOCUSEDGROUP, 0, 0L);
		}

		BOOL GetEmptyText(LPWSTR lpstrText, int cchText) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETEMPTYTEXT, cchText, (LPARAM) lpstrText);
		}

		BOOL GetFooterRect(LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETFOOTERRECT, 0, (LPARAM) lpRect);
		}

		BOOL GetFooterInfo(LPLVFOOTERINFO lpFooterInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETFOOTERINFO, 0, (LPARAM) lpFooterInfo);
		}

		BOOL GetFooterItemRect(int nItem, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETFOOTERITEMRECT, nItem, (LPARAM) lpRect);
		}

		BOOL GetFooterItem(int nItem, LPLVFOOTERITEM lpFooterItem) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETFOOTERITEM, nItem, (LPARAM) lpFooterItem);
		}

		BOOL GetItemIndexRect(PLVITEMINDEX pItemIndex, int nSubItem, int nType, LPRECT lpRect) const
		{
			if (lpRect != NULL)
			{
				lpRect->top = nSubItem;
				lpRect->left = nType;
			}
			return (BOOL) SendMessage(m_hWnd, LVM_GETITEMINDEXRECT, (WPARAM) pItemIndex, (LPARAM) lpRect);
		}

		BOOL SetItemIndexState(PLVITEMINDEX pItemIndex, UINT uState, UINT dwMask)
		{
			LVITEM lvi = {0};
			lvi.state = uState;
			lvi.stateMask = dwMask;
			return (BOOL) SendMessage(m_hWnd, LVM_SETITEMINDEXSTATE, (WPARAM) pItemIndex, (LPARAM) &lvi);
		}

		BOOL GetNextItemIndex(PLVITEMINDEX pItemIndex, WORD wFlags) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_GETNEXTITEMINDEX, (WPARAM) pItemIndex, MAKELPARAM(wFlags, 0));
		}

		int InsertColumn(int nCol, const LVCOLUMN* pColumn)
		{
			return (int) SendMessage(m_hWnd, LVM_INSERTCOLUMN, nCol, (LPARAM) pColumn);
		}

		int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT,
						 int nWidth = -1, int nSubItem = -1, int iImage = -1, int iOrder = -1)
		{
			LVCOLUMN column = {0};
			column.mask = LVCF_TEXT | LVCF_FMT;
			column.pszText = (LPTSTR) lpszColumnHeading;
			column.fmt = nFormat;
			if (nWidth != -1)
			{
				column.mask |= LVCF_WIDTH;
				column.cx = nWidth;
			}
			if (nSubItem != -1)
			{
				column.mask |= LVCF_SUBITEM;
				column.iSubItem = nSubItem;
			}
			if (iImage != -1)
			{
				column.mask |= LVCF_IMAGE;
				column.iImage = iImage;
			}
			if (iOrder != -1)
			{
				column.mask |= LVCF_ORDER;
				column.iOrder = iOrder;
			}
			return InsertColumn(nCol, &column);
		}

		BOOL DeleteColumn(int nCol)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_DELETECOLUMN, nCol, 0L);
		}

		int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
		{
			LVITEM item = {0};
			item.mask = nMask;
			item.iItem = nItem;
			item.iSubItem = 0;
			item.pszText = (LPTSTR) lpszItem;
			item.state = nState;
			item.stateMask = nStateMask;
			item.iImage = nImage;
			item.lParam = lParam;
			return InsertItem(&item);
		}

		int InsertItem(const LVITEM* pItem)
		{
			return (int) SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM) pItem);
		}

		int InsertItem(int nItem, LPCTSTR lpszItem)
		{
			return InsertItem(LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0);
		}

		int InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
		{
			return InsertItem(LVIF_TEXT | LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0);
		}

		int GetNextItem(int nItem, int nFlags) const
		{
			return (int) SendMessage(m_hWnd, LVM_GETNEXTITEM, nItem, MAKELPARAM(nFlags, 0));
		}

		BOOL DeleteItem(int nItem)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_DELETEITEM, nItem, 0L);
		}

		BOOL DeleteAllItems()
		{
			return (BOOL) SendMessage(m_hWnd, LVM_DELETEALLITEMS, 0, 0L);
		}

		int FindItem(LVFINDINFO* pFindInfo, int nStart = -1) const
		{
			return (int) SendMessage(m_hWnd, LVM_FINDITEM, nStart, (LPARAM) pFindInfo);
		}

		int FindItem(LPCTSTR lpstrFind, bool bPartial = true, bool bWrap = false, int nStart = -1) const
		{
			LVFINDINFO lvfi = {0};
			lvfi.flags = LVFI_STRING | (bWrap ? LVFI_WRAP : 0) | (bPartial ? LVFI_PARTIAL : 0);
			lvfi.psz = lpstrFind;
			return (int) SendMessage(m_hWnd, LVM_FINDITEM, nStart, (LPARAM) &lvfi);
		}

		int HitTest(LVHITTESTINFO* pHitTestInfo) const
		{
			return (int) SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM) pHitTestInfo);
		}

		int HitTest(POINT pt, UINT* pFlags) const
		{
			LVHITTESTINFO hti = {0};
			hti.pt = pt;
			int nRes = (int) SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM) &hti);
			if (pFlags != NULL)
				*pFlags = hti.flags;
			return nRes;
		}

		BOOL EnsureVisible(int nItem, BOOL bPartialOK)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_ENSUREVISIBLE, nItem, MAKELPARAM(bPartialOK, 0));
		}

		BOOL Scroll(SIZE size)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SCROLL, size.cx, size.cy);
		}

		BOOL RedrawItems(int nFirst, int nLast)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_REDRAWITEMS, nFirst, nLast);
		}

		BOOL Arrange(UINT nCode)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_ARRANGE, nCode, 0L);
		}

		EditText EditLabel(int nItem)
		{
			return EditText((HWND) SendMessage(m_hWnd, LVM_EDITLABEL, nItem, 0L));
		}

		BOOL Update(int nItem)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_UPDATE, nItem, 0L);
		}

		BOOL SortItems(PFNLVCOMPARE pfnCompare, LPARAM lParamSort)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SORTITEMS, (WPARAM) lParamSort, (LPARAM) pfnCompare);
		}

		ImageList RemoveImageList(int nImageList)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, LVM_SETIMAGELIST, (WPARAM) nImageList, NULL));
		}

		ImageList CreateDragImage(int nItem, LPPOINT lpPoint)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, LVM_CREATEDRAGIMAGE, nItem, (LPARAM) lpPoint));
		}

		DWORD ApproximateViewRect(int cx = -1, int cy = -1, int nCount = -1)
		{
			return (DWORD) SendMessage(m_hWnd, LVM_APPROXIMATEVIEWRECT, nCount, MAKELPARAM(cx, cy));
		}

		int SubItemHitTest(LPLVHITTESTINFO lpInfo) const
		{
			return (int) SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) lpInfo);
		}

		int AddColumn(int sub_item, LVCOLUMN column)
		{
			return InsertColumn(sub_item, &column);
		}

		int AddColumn(LPCTSTR strItem, int nItem, int nSubItem = -1,
					  int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
					  int nFmt = LVCFMT_LEFT)
		{
			const int cxOffset = 15;
			LVCOLUMN lvc = {0};
			lvc.mask = nMask;
			lvc.fmt = nFmt;
			lvc.pszText = (LPTSTR) strItem;
			lvc.cx = GetStringWidth(lvc.pszText) + cxOffset;
			if (nMask & LVCF_SUBITEM)
				lvc.iSubItem = (nSubItem != -1) ? nSubItem : nItem;
			return InsertColumn(nItem, &lvc);
		}

		int AddItem(int nItem, int nSubItem, LPCTSTR strItem, int nImageIndex = -3)
		{
			LVITEM lvItem = {0};
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItem;
			lvItem.iSubItem = nSubItem;
			lvItem.pszText = (LPTSTR) strItem;
			if (nImageIndex != -3)
			{
				lvItem.mask |= LVIF_IMAGE;
				lvItem.iImage = nImageIndex;
			}
			if (nSubItem == 0)
				return InsertItem(&lvItem);
			return SetItem(&lvItem) ? nItem : -1;
		}

		BOOL SortItemsEx(PFNLVCOMPARE pfnCompare, LPARAM lParamSort)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SORTITEMSEX, (WPARAM) lParamSort, (LPARAM) pfnCompare);
		}

		int InsertGroup(int nItem, PLVGROUP pGroup)
		{
			return (int) SendMessage(m_hWnd, LVM_INSERTGROUP, nItem, (LPARAM) pGroup);
		}

		int AddGroup(PLVGROUP pGroup)
		{
			return InsertGroup(-1, pGroup);
		}

		int RemoveGroup(int nGroupID)
		{
			return (int) SendMessage(m_hWnd, LVM_REMOVEGROUP, nGroupID, 0L);
		}

		void MoveGroup(int nGroupID, int nItem)
		{
			SendMessage(m_hWnd, LVM_MOVEGROUP, nGroupID, nItem);
		}

		void MoveItemToGroup(int nItem, int nGroupID)
		{
			SendMessage(m_hWnd, LVM_MOVEITEMTOGROUP, nItem, nGroupID);
		}

		int EnableGroupView(BOOL bEnable)
		{
			return (int) SendMessage(m_hWnd, LVM_ENABLEGROUPVIEW, bEnable, 0L);
		}

		int SortGroups(PFNLVGROUPCOMPARE pCompareFunc, LPVOID lpVoid = NULL)
		{
			return (int) SendMessage(m_hWnd, LVM_SORTGROUPS, (WPARAM) pCompareFunc, (LPARAM) lpVoid);
		}

		void InsertGroupSorted(PLVINSERTGROUPSORTED pInsertGroupSorted)
		{
			SendMessage(m_hWnd, LVM_INSERTGROUPSORTED, (WPARAM) pInsertGroupSorted, 0L);
		}

		void RemoveAllGroups()
		{
			SendMessage(m_hWnd, LVM_REMOVEALLGROUPS, 0, 0L);
		}

		BOOL HasGroup(int nGroupID)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_HASGROUP, nGroupID, 0L);
		}

		BOOL InsertMarkHitTest(LPPOINT lpPoint, LPLVINSERTMARK pInsertMark) const
		{
			return (BOOL) SendMessage(m_hWnd, LVM_INSERTMARKHITTEST, (WPARAM) lpPoint, (LPARAM) pInsertMark);
		}

		BOOL SetInfoTip(PLVSETINFOTIP pSetInfoTip)
		{
			return (BOOL) SendMessage(m_hWnd, LVM_SETINFOTIP, 0, (LPARAM) pSetInfoTip);
		}

		void CancelEditLabel()
		{
			SendMessage(m_hWnd, LVM_CANCELEDITLABEL, 0, 0L);
		}

		UINT MapIndexToID(int nIndex) const
		{
			return (UINT) SendMessage(m_hWnd, LVM_MAPINDEXTOID, nIndex, 0L);
		}

		int MapIDToIndex(UINT uID) const
		{
			return (int) SendMessage(m_hWnd, LVM_MAPIDTOINDEX, uID, 0L);
		}

		int HitTestEx(LPLVHITTESTINFO lpHitTestInfo) const
		{
			return (int) SendMessage(m_hWnd, LVM_HITTEST, (WPARAM) -1, (LPARAM) lpHitTestInfo);
		}

		int HitTestEx(POINT pt, UINT* pFlags) const
		{
			LVHITTESTINFO hti = {0};
			hti.pt = pt;
			int nRes = (int) SendMessage(m_hWnd, LVM_HITTEST, (WPARAM) -1, (LPARAM) &hti);
			if (pFlags != NULL)
				*pFlags = hti.flags;
			return nRes;
		}

		int SubItemHitTestEx(LPLVHITTESTINFO lpHitTestInfo) const
		{
			return (int) SendMessage(m_hWnd, LVM_SUBITEMHITTEST, (WPARAM) -1, (LPARAM) lpHitTestInfo);
		}

		BOOL SelectItem(int nIndex)
		{
			if ((GetStyle() & LVS_SINGLESEL) == 0)
				SetItemState(-1, 0, LVIS_SELECTED);

			BOOL bRet = SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			if (bRet)
				bRet = EnsureVisible(nIndex, FALSE);

			return bRet;
		}
	};

	class TreeView : public Control
	{
	public:
		using Control::Control;

		UINT GetCount() const
		{
			return (UINT) SendMessage(m_hWnd, TVM_GETCOUNT, 0, 0L);
		}

		UINT GetIndent() const
		{
			return (UINT) SendMessage(m_hWnd, TVM_GETINDENT, 0, 0L);
		}

		void SetIndent(UINT nIndent)
		{
			SendMessage(m_hWnd, TVM_SETINDENT, nIndent, 0L);
		}

		ImageList GetImageList(int nImageListType = TVSIL_NORMAL) const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TVM_GETIMAGELIST, (WPARAM) nImageListType, 0L));
		}

		ImageList SetImageList(HIMAGELIST hImageList, int nImageListType = TVSIL_NORMAL)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) nImageListType, (LPARAM) hImageList));
		}

		BOOL GetItem(LPTVITEM pItem) const
		{
			return (BOOL) SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) pItem);
		}

		BOOL SetItem(LPTVITEM pItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM) pItem);
		}

		BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
					 int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
		{
			TVITEM item = {0};
			item.hItem = hItem;
			item.mask = nMask;
			item.pszText = (LPTSTR) lpszItem;
			item.iImage = nImage;
			item.iSelectedImage = nSelectedImage;
			item.state = nState;
			item.stateMask = nStateMask;
			item.lParam = lParam;
			return (BOOL) SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM) &item);
		}

		BOOL GetItemText(HTREEITEM hItem, LPTSTR lpstrText, int nLen) const
		{
			TVITEM item = {0};
			item.hItem = hItem;
			item.mask = TVIF_TEXT;
			item.pszText = lpstrText;
			item.cchTextMax = nLen;

			return (BOOL) SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) &item);
		}

		BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem)
		{
			return SetItem(hItem, TVIF_TEXT, lpszItem, 0, 0, 0, 0, NULL);
		}

		BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage) const
		{
			TVITEM item = {0};
			item.hItem = hItem;
			item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			BOOL bRes = (BOOL) SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) &item);
			if (bRes)
			{
				nImage = item.iImage;
				nSelectedImage = item.iSelectedImage;
			}
			return bRes;
		}

		BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
		{
			return SetItem(hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE, NULL, nImage, nSelectedImage, 0, 0, NULL);
		}

		UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const
		{
			return (((UINT) SendMessage(m_hWnd, TVM_GETITEMSTATE, (WPARAM) hItem, (LPARAM) nStateMask)) & nStateMask);
		}

		BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
		{
			return SetItem(hItem, TVIF_STATE, NULL, 0, 0, nState, nStateMask, NULL);
		}

		DWORD_PTR GetItemData(HTREEITEM hItem) const
		{
			TVITEM item = {0};
			item.hItem = hItem;
			item.mask = TVIF_PARAM;
			BOOL bRet = (BOOL) SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) &item);
			return (DWORD_PTR) (bRet ? item.lParam : NULL);
		}

		BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData)
		{
			return SetItem(hItem, TVIF_PARAM, NULL, 0, 0, 0, 0, (LPARAM) dwData);
		}

		EditText GetEditControl() const
		{
			return EditText((HWND) SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0L));
		}

		UINT GetVisibleCount() const
		{
			return (UINT) SendMessage(m_hWnd, TVM_GETVISIBLECOUNT, 0, 0L);
		}

		BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const
		{
			*(HTREEITEM*) lpRect = hItem;
			return (BOOL) SendMessage(m_hWnd, TVM_GETITEMRECT, (WPARAM) bTextOnly, (LPARAM) lpRect);
		}

		BOOL ItemHasChildren(HTREEITEM hItem) const
		{
			TVITEM item = {0};
			item.hItem = hItem;
			item.mask = TVIF_CHILDREN;
			SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) &item);
			return item.cChildren;
		}

		ToolTip GetToolTips() const
		{
			return ToolTip((HWND) SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L));
		}

		ToolTip SetToolTips(HWND hWndTT)
		{
			return ToolTip((HWND) SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM) hWndTT, 0L));
		}

		int GetISearchString(LPTSTR lpstr) const
		{
			return (int) SendMessage(m_hWnd, TVM_GETISEARCHSTRING, 0, (LPARAM) lpstr);
		}

		BOOL GetCheckState(HTREEITEM hItem) const
		{
			UINT uRet = GetItemState(hItem, TVIS_STATEIMAGEMASK);
			return (uRet >> 12) - 1;
		}

		BOOL SetCheckState(HTREEITEM hItem, BOOL bCheck)
		{
			int nCheck = bCheck ? 2 : 1;   // one based index
			return SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nCheck), TVIS_STATEIMAGEMASK);
		}

		COLORREF GetBkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_GETBKCOLOR, 0, 0L);
		}

		COLORREF SetBkColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM) clr);
		}

		COLORREF GetInsertMarkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_GETINSERTMARKCOLOR, 0, 0L);
		}

		COLORREF SetInsertMarkColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_SETINSERTMARKCOLOR, 0, (LPARAM) clr);
		}

		int GetItemHeight() const
		{
			return (int) SendMessage(m_hWnd, TVM_GETITEMHEIGHT, 0, 0L);
		}

		int SetItemHeight(int cyHeight)
		{
			return (int) SendMessage(m_hWnd, TVM_SETITEMHEIGHT, cyHeight, 0L);
		}

		int GetScrollTime() const
		{
			return (int) SendMessage(m_hWnd, TVM_GETSCROLLTIME, 0, 0L);
		}

		int SetScrollTime(int nScrollTime)
		{
			return (int) SendMessage(m_hWnd, TVM_SETSCROLLTIME, nScrollTime, 0L);
		}

		COLORREF GetTextColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_GETTEXTCOLOR, 0, 0L);
		}

		COLORREF SetTextColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM) clr);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, TVM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		COLORREF GetLineColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_GETLINECOLOR, 0, 0L);
		}

		COLORREF SetLineColor(COLORREF clrNew /*= CLR_DEFAULT*/)
		{
			return (COLORREF) SendMessage(m_hWnd, TVM_SETLINECOLOR, 0, (LPARAM) clrNew);
		}

		BOOL GetItem(LPTVITEMEX pItem) const
		{
			return (BOOL) SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) pItem);
		}

		BOOL SetItem(LPTVITEMEX pItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM) pItem);
		}

		DWORD GetExtendedStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, TVM_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD SetExtendedStyle(DWORD dwStyle, DWORD dwMask)
		{
			return (DWORD) SendMessage(m_hWnd, TVM_SETEXTENDEDSTYLE, dwMask, dwStyle);
		}

		BOOL SetAutoScrollInfo(UINT uPixPerSec, UINT uUpdateTime)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SETAUTOSCROLLINFO, (WPARAM) uPixPerSec, (LPARAM) uUpdateTime);
		}

		DWORD GetSelectedCount() const
		{
			return (DWORD) SendMessage(m_hWnd, TVM_GETSELECTEDCOUNT, 0, 0L);
		}

		BOOL GetItemPartRect(HTREEITEM hItem, TVITEMPART partID, LPRECT lpRect) const
		{
			TVGETITEMPARTRECTINFO gipri = {hItem, lpRect, partID};
			return (BOOL) SendMessage(m_hWnd, TVM_GETITEMPARTRECT, 0, (LPARAM) &gipri);
		}

		HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM) lpInsertStruct);
		}

		HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage,
							 int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
		{
			return InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter);
		}

		HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
		{
			return InsertItem(TVIF_TEXT, lpszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
		}

		HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
							 int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
							 HTREEITEM hParent, HTREEITEM hInsertAfter)
		{
			TVINSERTSTRUCT tvis = {0};
			tvis.hParent = hParent;
			tvis.hInsertAfter = hInsertAfter;
			tvis.item.mask = nMask;
			tvis.item.pszText = (LPTSTR) lpszItem;
			tvis.item.iImage = nImage;
			tvis.item.iSelectedImage = nSelectedImage;
			tvis.item.state = nState;
			tvis.item.stateMask = nStateMask;
			tvis.item.lParam = lParam;
			return (HTREEITEM) SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM) &tvis);
		}

		BOOL DeleteItem(HTREEITEM hItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM) hItem);
		}

		BOOL DeleteAllItems()
		{
			return (BOOL) SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM) TVI_ROOT);
		}

		BOOL Expand(HTREEITEM hItem, UINT nCode = TVE_EXPAND)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_EXPAND, nCode, (LPARAM) hItem);
		}

		HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, nCode, (LPARAM) hItem);
		}

		HTREEITEM GetChildItem(HTREEITEM hItem) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM) hItem);
		}

		HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM) hItem);
		}

		HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM) hItem);
		}

		HTREEITEM GetParentItem(HTREEITEM hItem) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM) hItem);
		}

		HTREEITEM GetFirstVisibleItem() const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0L);
		}

		HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM) hItem);
		}

		HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM) hItem);
		}

		HTREEITEM GetSelectedItem() const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0L);
		}

		HTREEITEM GetSelectedItemParent() const
		{
			return GetParentItem(GetSelectedItem());
		}

		HTREEITEM GetDropHilightItem() const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0L);
		}

		HTREEITEM GetRootItem() const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0L);
		}

		HTREEITEM GetLastVisibleItem() const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0L);
		}

		HTREEITEM GetNextSelectedItem() const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTSELECTED, 0L);
		}

		BOOL Select(HTREEITEM hItem, UINT nCode)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SELECTITEM, nCode, (LPARAM) hItem);
		}

		BOOL SelectItem(HTREEITEM hItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM) hItem);
		}

		BOOL SelectDropTarget(HTREEITEM hItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM) hItem);
		}

		BOOL SelectSetFirstVisible(HTREEITEM hItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM) hItem);
		}

		EditText EditLabel(HTREEITEM hItem)
		{
			return EditText((HWND) SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM) hItem));
		}

		BOOL EndEditLabelNow(BOOL bCancel)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_ENDEDITLABELNOW, bCancel, 0L);
		}

		HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM) pHitTestInfo);
		}

		HTREEITEM HitTest(POINT pt, UINT* pFlags) const
		{
			TVHITTESTINFO hti = {0};
			hti.pt = pt;
			HTREEITEM hTreeItem = (HTREEITEM) SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM) &hti);
			if (pFlags != NULL)
				*pFlags = hti.flags;
			return hTreeItem;
		}

		BOOL SortChildren(HTREEITEM hItem, BOOL bRecurse = FALSE)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SORTCHILDREN, (WPARAM) bRecurse, (LPARAM) hItem);
		}

		BOOL EnsureVisible(HTREEITEM hItem)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM) hItem);
		}

		BOOL SortChildrenCB(LPTVSORTCB pSort, BOOL bRecurse = FALSE)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SORTCHILDRENCB, (WPARAM) bRecurse, (LPARAM) pSort);
		}

		ImageList RemoveImageList(int nImageList)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) nImageList, NULL));
		}

		ImageList CreateDragImage(HTREEITEM hItem)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TVM_CREATEDRAGIMAGE, 0, (LPARAM) hItem));
		}

		BOOL SetInsertMark(HTREEITEM hTreeItem, BOOL bAfter)
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SETINSERTMARK, bAfter, (LPARAM) hTreeItem);
		}

		BOOL RemoveInsertMark()
		{
			return (BOOL) SendMessage(m_hWnd, TVM_SETINSERTMARK, 0, 0L);
		}

		HTREEITEM MapAccIDToHTREEITEM(UINT uID) const
		{
			return (HTREEITEM) SendMessage(m_hWnd, TVM_MAPACCIDTOHTREEITEM, uID, 0L);
		}

		UINT MapHTREEITEMToAccID(HTREEITEM hTreeItem) const
		{
			return (UINT) SendMessage(m_hWnd, TVM_MAPHTREEITEMTOACCID, (WPARAM) hTreeItem, 0L);
		}

		void ShowInfoTip(HTREEITEM hItem)
		{
			SendMessage(m_hWnd, TVM_SHOWINFOTIP, 0, (LPARAM) hItem);
		}
	};

	class ToolBar : public Control
	{
	public:
		using Control::Control;

		BOOL IsButtonEnabled(int nID) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_ISBUTTONENABLED, nID, 0L);
		}

		BOOL IsButtonChecked(int nID) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_ISBUTTONCHECKED, nID, 0L);
		}

		BOOL IsButtonPressed(int nID) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_ISBUTTONPRESSED, nID, 0L);
		}

		BOOL IsButtonHidden(int nID) const
		{
			return(BOOL) SendMessage(m_hWnd, TB_ISBUTTONHIDDEN, nID, 0L);
		}

		BOOL IsButtonIndeterminate(int nID) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_ISBUTTONINDETERMINATE, nID, 0L);
		}

		int GetState(int nID) const
		{
			return (int) SendMessage(m_hWnd, TB_GETSTATE, nID, 0L);
		}

		BOOL SetState(int nID, UINT nState)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETSTATE, nID, MAKELPARAM(nState, 0));
		}

		BOOL GetButton(int nIndex, LPTBBUTTON lpButton) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM) lpButton);
		}

		int GetButtonCount() const
		{
			return (int) SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0L);
		}

		BOOL GetItemRect(int nIndex, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM) lpRect);
		}

		void SetButtonStructSize(int nSize = sizeof(TBBUTTON))
		{
			SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, nSize, 0L);
		}

		BOOL SetButtonSize(SIZE size)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(size.cx, size.cy));
		}

		BOOL SetButtonSize(int cx, int cy)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(cx, cy));
		}

		BOOL SetBitmapSize(SIZE size)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(size.cx, size.cy));
		}

		BOOL SetBitmapSize(int cx, int cy)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(cx, cy));
		}

		ToolTip GetToolTips() const
		{
			return ToolTip((HWND) SendMessage(m_hWnd, TB_GETTOOLTIPS, 0, 0L));
		}

		void SetToolTips(HWND hWndToolTip)
		{
			SendMessage(m_hWnd, TB_SETTOOLTIPS, (WPARAM) hWndToolTip, 0L);
		}

		void SetNotifyWnd(HWND hWnd)
		{
			SendMessage(m_hWnd, TB_SETPARENT, (WPARAM) hWnd, 0L);
		}

		int GetRows() const
		{
			return (int) SendMessage(m_hWnd, TB_GETROWS, 0, 0L);
		}

		void SetRows(int nRows, BOOL bLarger, LPRECT lpRect)
		{
			SendMessage(m_hWnd, TB_SETROWS, MAKELPARAM(nRows, bLarger), (LPARAM) lpRect);
		}

		BOOL SetCmdID(int nIndex, UINT nID)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETCMDID, nIndex, nID);
		}

		DWORD GetBitmapFlags() const
		{
			return (DWORD) SendMessage(m_hWnd, TB_GETBITMAPFLAGS, 0, 0L);
		}

		int GetBitmap(int nID) const
		{
			return (int) SendMessage(m_hWnd, TB_GETBITMAP, nID, 0L);
		}

		int GetButtonText(int nID, LPTSTR lpstrText) const
		{
			return (int) SendMessage(m_hWnd, TB_GETBUTTONTEXT, nID, (LPARAM) lpstrText);
		}

		ImageList GetImageList(int nIndex = 0) const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_GETIMAGELIST, nIndex, 0L));
		}

		ImageList SetImageList(HIMAGELIST hImageList, int nIndex = 0)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_SETIMAGELIST, nIndex, (LPARAM) hImageList));
		}

		ImageList GetDisabledImageList(int nIndex = 0) const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, nIndex, 0L));
		}

		ImageList SetDisabledImageList(HIMAGELIST hImageList, int nIndex = 0)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, nIndex, (LPARAM) hImageList));
		}

		ImageList GetHotImageList(int nIndex = 0) const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_GETHOTIMAGELIST, nIndex, 0L));
		}

		ImageList SetHotImageList(HIMAGELIST hImageList, int nIndex = 0)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_SETHOTIMAGELIST, nIndex, (LPARAM) hImageList));
		}

		DWORD GetStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L);
		}

		void SetStyle(DWORD dwStyle)
		{
			SendMessage(m_hWnd, TB_SETSTYLE, 0, dwStyle);
		}

		DWORD GetButtonSize() const
		{
			return (DWORD) SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
		}

		void GetButtonSize(SIZE& size) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
			size.cx = LOWORD(dwRet);
			size.cy = HIWORD(dwRet);
		}

		BOOL GetRect(int nID, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETRECT, nID, (LPARAM) lpRect);
		}

		int GetTextRows() const
		{
			return (int) SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0L);
		}

		BOOL SetButtonWidth(int cxMin, int cxMax)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax));
		}

		BOOL SetIndent(int nIndent)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETINDENT, nIndent, 0L);
		}

		BOOL SetMaxTextRows(int nMaxTextRows)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETMAXTEXTROWS, nMaxTextRows, 0L);
		}

		BOOL GetAnchorHighlight() const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETANCHORHIGHLIGHT, 0, 0L);
		}

		BOOL SetAnchorHighlight(BOOL bEnable = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETANCHORHIGHLIGHT, bEnable, 0L);
		}

		int GetButtonInfo(int nID, LPTBBUTTONINFO lptbbi) const
		{
			return (int) SendMessage(m_hWnd, TB_GETBUTTONINFO, nID, (LPARAM) lptbbi);
		}

		BOOL SetButtonInfo(int nID, LPTBBUTTONINFO lptbbi)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETBUTTONINFO, nID, (LPARAM) lptbbi);
		}

		BOOL SetButtonInfo(int nID, DWORD dwMask, BYTE Style, BYTE State, LPCTSTR lpszItem,
						   int iImage, WORD cx, int iCommand, DWORD_PTR lParam)
		{
			TBBUTTONINFO tbbi = {0};
			tbbi.cbSize = sizeof(TBBUTTONINFO);
			tbbi.dwMask = dwMask;
			tbbi.idCommand = iCommand;
			tbbi.iImage = iImage;
			tbbi.fsState = State;
			tbbi.fsStyle = Style;
			tbbi.cx = cx;
			tbbi.pszText = (LPTSTR) lpszItem;
			tbbi.lParam = lParam;
			return (BOOL) SendMessage(m_hWnd, TB_SETBUTTONINFO, nID, (LPARAM) &tbbi);
		}

		int GetHotItem() const
		{
			return (int) SendMessage(m_hWnd, TB_GETHOTITEM, 0, 0L);
		}

		int SetHotItem(int nItem)
		{
			return (int) SendMessage(m_hWnd, TB_SETHOTITEM, nItem, 0L);
		}

		BOOL IsButtonHighlighted(int nButtonID) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_ISBUTTONHIGHLIGHTED, nButtonID, 0L);
		}

		DWORD SetDrawTextFlags(DWORD dwMask, DWORD dwFlags)
		{
			return (DWORD) SendMessage(m_hWnd, TB_SETDRAWTEXTFLAGS, dwMask, dwFlags);
		}

		BOOL GetColorScheme(LPCOLORSCHEME lpcs) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETCOLORSCHEME, 0, (LPARAM) lpcs);
		}

		void SetColorScheme(LPCOLORSCHEME lpcs)
		{
			SendMessage(m_hWnd, TB_SETCOLORSCHEME, 0, (LPARAM) lpcs);
		}

		DWORD GetExtendedStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD SetExtendedStyle(DWORD dwStyle)
		{
			return (DWORD) SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwStyle);
		}

		void GetInsertMark(LPTBINSERTMARK lptbim) const
		{
			SendMessage(m_hWnd, TB_GETINSERTMARK, 0, (LPARAM) lptbim);
		}

		void SetInsertMark(LPTBINSERTMARK lptbim)
		{
			SendMessage(m_hWnd, TB_SETINSERTMARK, 0, (LPARAM) lptbim);
		}

		COLORREF GetInsertMarkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, TB_GETINSERTMARKCOLOR, 0, 0L);
		}

		COLORREF SetInsertMarkColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, TB_SETINSERTMARKCOLOR, 0, (LPARAM) clr);
		}

		BOOL GetMaxSize(LPSIZE lpSize) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM) lpSize);
		}

		void GetPadding(LPSIZE lpSizePadding) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, TB_GETPADDING, 0, 0L);
			lpSizePadding->cx = GET_X_LPARAM(dwRet);
			lpSizePadding->cy = GET_Y_LPARAM(dwRet);
		}

		void SetPadding(int cx, int cy, LPSIZE lpSizePadding = NULL)
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, TB_SETPADDING, 0, MAKELPARAM(cx, cy));
			if (lpSizePadding != NULL)
			{
				lpSizePadding->cx = GET_X_LPARAM(dwRet);
				lpSizePadding->cy = GET_Y_LPARAM(dwRet);
			}
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, TB_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int GetString(int nString, LPTSTR lpstrString, int cchMaxLen) const
		{
			return (int) SendMessage(m_hWnd, TB_GETSTRING, MAKEWPARAM(cchMaxLen, nString), (LPARAM) lpstrString);
		}

		void GetMetrics(LPTBMETRICS lptbm) const
		{
			SendMessage(m_hWnd, TB_GETMETRICS, 0, (LPARAM) lptbm);
		}

		void SetMetrics(LPTBMETRICS lptbm)
		{
			SendMessage(m_hWnd, TB_SETMETRICS, 0, (LPARAM) lptbm);
		}

		void SetWindowTheme(LPCWSTR lpstrTheme)
		{
			SendMessage(m_hWnd, TB_SETWINDOWTHEME, 0, (LPARAM) lpstrTheme);
		}

		ImageList GetPressedImageList(int nIndex = 0) const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_GETPRESSEDIMAGELIST, nIndex, 0L));
		}

		ImageList SetPressedImageList(HIMAGELIST hImageList, int nIndex = 0)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TB_SETPRESSEDIMAGELIST, nIndex, (LPARAM) hImageList));
		}

		void GetItemDropDownRect(int nIndex, LPRECT lpRect) const
		{
			BOOL bRet = (BOOL) SendMessage(m_hWnd, TB_GETITEMDROPDOWNRECT, nIndex, (LPARAM) lpRect);
		}

		BOOL EnableButton(int nID, BOOL bEnable = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_ENABLEBUTTON, nID, MAKELPARAM(bEnable, 0));
		}

		BOOL CheckButton(int nID, BOOL bCheck = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_CHECKBUTTON, nID, MAKELPARAM(bCheck, 0));
		}

		BOOL PressButton(int nID, BOOL bPress = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_PRESSBUTTON, nID, MAKELPARAM(bPress, 0));
		}

		BOOL HideButton(int nID, BOOL bHide = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_HIDEBUTTON, nID, MAKELPARAM(bHide, 0));
		}

		BOOL Indeterminate(int nID, BOOL bIndeterminate = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_INDETERMINATE, nID, MAKELPARAM(bIndeterminate, 0));
		}

		int AddBitmap(int nNumButtons, UINT nBitmapID)
		{
			TBADDBITMAP tbab = {0};
			tbab.hInst = GetModuleHandle(0);
			tbab.nID = nBitmapID;
			return (int) SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM) nNumButtons, (LPARAM) &tbab);
		}

		int AddBitmap(int nNumButtons, HBITMAP hBitmap)
		{
			TBADDBITMAP tbab = {0};
			tbab.hInst = NULL;
			tbab.nID = (UINT_PTR) hBitmap;
			return (int) SendMessage(m_hWnd, TB_ADDBITMAP, (WPARAM) nNumButtons, (LPARAM) &tbab);
		}

		BOOL AddButtons(int nNumButtons, LPTBBUTTON lpButtons)
		{
			return (BOOL) SendMessage(m_hWnd, TB_ADDBUTTONS, nNumButtons, (LPARAM) lpButtons);
		}

		BOOL InsertButton(int nIndex, LPTBBUTTON lpButton)
		{
			return (BOOL) SendMessage(m_hWnd, TB_INSERTBUTTON, nIndex, (LPARAM) lpButton);
		}

		BOOL InsertButton(int nIndex, int iCommand, BYTE Style, BYTE State, int iBitmap,
						  INT_PTR iString, DWORD_PTR lParam)
		{
			TBBUTTON tbb = {0};
			tbb.fsStyle = Style;
			tbb.fsState = State;
			tbb.idCommand = iCommand;
			tbb.iBitmap = iBitmap;
			tbb.iString = iString;
			tbb.dwData = lParam;
			return (BOOL) SendMessage(m_hWnd, TB_INSERTBUTTON, nIndex, (LPARAM) &tbb);
		}

		BOOL InsertButton(int nIndex, int iCommand, BYTE Style, BYTE State, int iBitmap,
						  LPCTSTR lpszItem, DWORD_PTR lParam)
		{
			return InsertButton(nIndex, iCommand, Style, State, iBitmap, (INT_PTR) lpszItem, lParam);
		}

		BOOL AddButton(LPTBBUTTON lpButton)
		{
			return InsertButton(-1, lpButton);
		}

		BOOL AddButton(int iCommand, BYTE Style, BYTE State, int iBitmap, INT_PTR iString, DWORD_PTR lParam)
		{
			return InsertButton(-1, iCommand, Style, State, iBitmap, iString, lParam);
		}

		BOOL AddButton(int iCommand, BYTE Style, BYTE State, int iBitmap, LPCTSTR lpszItem, DWORD_PTR lParam)
		{
			return InsertButton(-1, iCommand, Style, State, iBitmap, lpszItem, lParam);
		}

		BOOL DeleteButton(int nIndex)
		{
			return (BOOL) SendMessage(m_hWnd, TB_DELETEBUTTON, nIndex, 0L);
		}

		BOOL InsertSeparator(int nIndex, int cxWidth = 8)
		{
			return InsertButton(nIndex, 0, BTNS_SEP, 0, cxWidth, (INT_PTR) 0, 0);
		}

		BOOL AddSeparator(int cxWidth = 8)
		{
			return AddButton(0, BTNS_SEP, 0, cxWidth, (INT_PTR) 0, 0);
		}

		int CommandToIndex(UINT nID) const
		{
			return (int) SendMessage(m_hWnd, TB_COMMANDTOINDEX, nID, 0L);
		}

		void SaveState(HKEY hKeyRoot, LPCTSTR lpszSubKey, LPCTSTR lpszValueName)
		{
			TBSAVEPARAMS tbs = {0};
			tbs.hkr = hKeyRoot;
			tbs.pszSubKey = lpszSubKey;
			tbs.pszValueName = lpszValueName;
			SendMessage(m_hWnd, TB_SAVERESTORE, (WPARAM) TRUE, (LPARAM) &tbs);
		}

		void RestoreState(HKEY hKeyRoot, LPCTSTR lpszSubKey, LPCTSTR lpszValueName)
		{
			TBSAVEPARAMS tbs = {0};
			tbs.hkr = hKeyRoot;
			tbs.pszSubKey = lpszSubKey;
			tbs.pszValueName = lpszValueName;
			SendMessage(m_hWnd, TB_SAVERESTORE, (WPARAM) FALSE, (LPARAM) &tbs);
		}

		void Customize()
		{
			SendMessage(m_hWnd, TB_CUSTOMIZE, 0, 0L);
		}

		int AddString(UINT nStringID)
		{
			return (int) SendMessage(m_hWnd, TB_ADDSTRING, (WPARAM) GetModuleHandle(NULL), (LPARAM) nStringID);
		}

		int AddStrings(LPCTSTR lpszStrings)
		{
			return (int) SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM) lpszStrings);
		}

		void AutoSize()
		{
			SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0L);
		}

		BOOL ChangeBitmap(int nID, int nBitmap)
		{
			return (BOOL) SendMessage(m_hWnd, TB_CHANGEBITMAP, nID, MAKELPARAM(nBitmap, 0));
		}

		int LoadImages(int nBitmapID)
		{
			return (int) SendMessage(m_hWnd, TB_LOADIMAGES, nBitmapID, (LPARAM) GetModuleHandle(NULL));
		}

		int LoadStdImages(int nBitmapID)
		{
			return (int) SendMessage(m_hWnd, TB_LOADIMAGES, nBitmapID, (LPARAM) HINST_COMMCTRL);
		}

		BOOL ReplaceBitmap(LPTBREPLACEBITMAP ptbrb)
		{
			return (BOOL) SendMessage(m_hWnd, TB_REPLACEBITMAP, 0, (LPARAM) ptbrb);
		}

		int HitTest(LPPOINT lpPoint) const
		{
			return (int) SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM) lpPoint);
		}

		BOOL InsertMarkHitTest(LPPOINT lpPoint, LPTBINSERTMARK lptbim) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM) lpPoint, (LPARAM) lptbim);
		}

		BOOL InsertMarkHitTest(int x, int y, LPTBINSERTMARK lptbim) const
		{
			POINT pt = {x, y};
			return (BOOL) SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM) &pt, (LPARAM) lptbim);
		}

		BOOL MapAccelerator(TCHAR chAccel, int& nID) const
		{
			return (BOOL) SendMessage(m_hWnd, TB_MAPACCELERATOR, (WPARAM) chAccel, (LPARAM) &nID);
		}

		BOOL MarkButton(int nID, BOOL bHighlight = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TB_MARKBUTTON, nID, MAKELPARAM(bHighlight, 0));
		}

		BOOL MoveButton(int nOldPos, int nNewPos)
		{
			return (BOOL) SendMessage(m_hWnd, TB_MOVEBUTTON, nOldPos, nNewPos);
		}

		HRESULT GetObject(REFIID iid, LPVOID* ppvObject)
		{
			return (HRESULT) SendMessage(m_hWnd, TB_GETOBJECT, (WPARAM) &iid, (LPARAM) ppvObject);
		}
	};

	class StatusBar : public Control
	{
	public:
		using Control::Control;

		int GetParts(int nParts, int* pParts) const
		{
			return (int) SendMessage(m_hWnd, SB_GETPARTS, nParts, (LPARAM) pParts);
		}

		BOOL SetParts(int nParts, int* pWidths)
		{
			return (BOOL) SendMessage(m_hWnd, SB_SETPARTS, nParts, (LPARAM) pWidths);
		}

		int GetTextLength(int nPane, int* pType = NULL) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, SB_GETTEXTLENGTH, (WPARAM) nPane, 0L);
			if (pType != NULL)
				*pType = (int) (short) HIWORD(dwRet);
			return (int) (short) LOWORD(dwRet);
		}

		int GetText(int nPane, LPTSTR lpszText, int* pType = NULL) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, SB_GETTEXT, (WPARAM) nPane, (LPARAM) lpszText);
			if (pType != NULL)
				*pType = (int) (short) HIWORD(dwRet);
			return (int) (short) LOWORD(dwRet);
		}

		BOOL SetText(int nPane, LPCTSTR lpszText, int nType = 0)
		{
			return (BOOL) SendMessage(m_hWnd, SB_SETTEXT, (nPane | nType), (LPARAM) lpszText);
		}

		BOOL GetRect(int nPane, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, SB_GETRECT, nPane, (LPARAM) lpRect);
		}

		BOOL GetBorders(int* pBorders) const
		{
			return (BOOL) SendMessage(m_hWnd, SB_GETBORDERS, 0, (LPARAM) pBorders);
		}

		BOOL GetBorders(int& nHorz, int& nVert, int& nSpacing) const
		{
			int borders[3] = {0, 0, 0};
			BOOL bResult = (BOOL) SendMessage(m_hWnd, SB_GETBORDERS, 0, (LPARAM) &borders);
			if (bResult)
			{
				nHorz = borders[0];
				nVert = borders[1];
				nSpacing = borders[2];
			}
			return bResult;
		}

		void SetMinHeight(int nMin)
		{
			SendMessage(m_hWnd, SB_SETMINHEIGHT, nMin, 0L);
		}

		BOOL SetSimple(BOOL bSimple = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, SB_SIMPLE, bSimple, 0L);
		}

		BOOL IsSimple() const
		{
			return (BOOL) SendMessage(m_hWnd, SB_ISSIMPLE, 0, 0L);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, SB_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, SB_SETUNICODEFORMAT, bUnicode, 0L);
		}

		void GetTipText(int nPane, LPTSTR lpstrText, int nSize) const
		{
			SendMessage(m_hWnd, SB_GETTIPTEXT, MAKEWPARAM(nPane, nSize), (LPARAM) lpstrText);
		}

		void SetTipText(int nPane, LPCTSTR lpstrText)
		{
			SendMessage(m_hWnd, SB_SETTIPTEXT, nPane, (LPARAM) lpstrText);
		}

		COLORREF SetBkColor(COLORREF clrBk)
		{
			return (COLORREF) SendMessage(m_hWnd, SB_SETBKCOLOR, 0, (LPARAM) clrBk);
		}

		HICON GetIcon(int nPane) const
		{
			return (HICON) SendMessage(m_hWnd, SB_GETICON, nPane, 0L);
		}

		BOOL SetIcon(int nPane, HICON hIcon)
		{
			return (BOOL) SendMessage(m_hWnd, SB_SETICON, nPane, (LPARAM) hIcon);
		}
	};

	class TabControl : public Control
	{
	public:
		using Control::Control;

		ImageList GetImageList() const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TCM_GETIMAGELIST, 0, 0L));
		}

		ImageList SetImageList(HIMAGELIST hImageList)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, TCM_SETIMAGELIST, 0, (LPARAM) hImageList));
		}

		int GetItemCount() const
		{
			return (int) SendMessage(m_hWnd, TCM_GETITEMCOUNT, 0, 0L);
		}

		BOOL GetItem(int nItem, LPTCITEM pTabCtrlItem) const
		{
			return (BOOL) SendMessage(m_hWnd, TCM_GETITEM, nItem, (LPARAM) pTabCtrlItem);
		}

		BOOL SetItem(int nItem, LPTCITEM pTabCtrlItem)
		{
			return (BOOL) SendMessage(m_hWnd, TCM_SETITEM, nItem, (LPARAM) pTabCtrlItem);
		}

		int SetItem(int nItem, UINT mask, LPCTSTR lpszItem, DWORD dwState, DWORD dwStateMask, int iImage, LPARAM lParam)
		{
			TCITEM tci = {0};
			tci.mask = mask;
			tci.pszText = (LPTSTR) lpszItem;
			tci.dwState = dwState;
			tci.dwStateMask = dwStateMask;
			tci.iImage = iImage;
			tci.lParam = lParam;
			return (int) SendMessage(m_hWnd, TCM_SETITEM, nItem, (LPARAM) &tci);
		}

		BOOL GetItemRect(int nItem, LPRECT lpRect) const
		{
			return (BOOL) SendMessage(m_hWnd, TCM_GETITEMRECT, nItem, (LPARAM) lpRect);
		}

		int GetCurSel() const
		{
			return (int) SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0L);
		}

		int SetCurSel(int nItem)
		{
			return (int) SendMessage(m_hWnd, TCM_SETCURSEL, nItem, 0L);
		}

		SIZE SetItemSize(SIZE size)
		{
			DWORD dwSize = (DWORD) SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, MAKELPARAM(size.cx, size.cy));
			SIZE sizeRet = {GET_X_LPARAM(dwSize), GET_Y_LPARAM(dwSize)};
			return sizeRet;
		}

		void SetItemSize(int cx, int cy)
		{
			SendMessage(m_hWnd, TCM_SETITEMSIZE, 0, MAKELPARAM(cx, cy));
		}

		void SetPadding(SIZE size)
		{
			SendMessage(m_hWnd, TCM_SETPADDING, 0, MAKELPARAM(size.cx, size.cy));
		}

		int GetRowCount() const
		{
			return (int) SendMessage(m_hWnd, TCM_GETROWCOUNT, 0, 0L);
		}

		ToolTip GetToolTips() const
		{
			return ToolTip((HWND) SendMessage(m_hWnd, TCM_GETTOOLTIPS, 0, 0L));
		}

		void SetToolTips(HWND hWndToolTip)
		{
			SendMessage(m_hWnd, TCM_SETTOOLTIPS, (WPARAM) hWndToolTip, 0L);
		}

		int GetCurFocus() const
		{
			return (int) SendMessage(m_hWnd, TCM_GETCURFOCUS, 0, 0L);
		}

		void SetCurFocus(int nItem)
		{
			SendMessage(m_hWnd, TCM_SETCURFOCUS, nItem, 0L);
		}

		BOOL SetItemExtra(int cbExtra)
		{
			return (BOOL) SendMessage(m_hWnd, TCM_SETITEMEXTRA, cbExtra, 0L);
		}

		int SetMinTabWidth(int nWidth = -1)
		{
			return (int) SendMessage(m_hWnd, TCM_SETMINTABWIDTH, 0, nWidth);
		}

		DWORD GetExtendedStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, TCM_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyle)
		{
			return (DWORD) SendMessage(m_hWnd, TCM_SETEXTENDEDSTYLE, dwExMask, dwExStyle);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, TCM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TCM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int InsertItem(int nItem, LPTCITEM pTabCtrlItem)
		{
			return (int) SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM) pTabCtrlItem);
		}

		int InsertItem(int nItem, UINT mask, LPCTSTR lpszItem, int iImage, LPARAM lParam)
		{
			TCITEM tci = {0};
			tci.mask = mask;
			tci.pszText = (LPTSTR) lpszItem;
			tci.iImage = iImage;
			tci.lParam = lParam;
			return (int) SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM) &tci);
		}

		int InsertItem(int nItem, LPCTSTR lpszItem)
		{
			TCITEM tci = {0};
			tci.mask = TCIF_TEXT;
			tci.pszText = (LPTSTR) lpszItem;
			return (int) SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM) &tci);
		}

		int AddItem(LPTCITEM pTabCtrlItem)
		{
			return InsertItem(GetItemCount(), pTabCtrlItem);
		}

		int AddItem(UINT mask, LPCTSTR lpszItem, int iImage, LPARAM lParam)
		{
			return InsertItem(GetItemCount(), mask, lpszItem, iImage, lParam);
		}

		int AddItem(LPCTSTR lpszItem)
		{
			return InsertItem(GetItemCount(), lpszItem);
		}

		BOOL DeleteItem(int nItem)
		{
			return (BOOL) SendMessage(m_hWnd, TCM_DELETEITEM, nItem, 0L);
		}

		BOOL DeleteAllItems()
		{
			return (BOOL) SendMessage(m_hWnd, TCM_DELETEALLITEMS, 0, 0L);
		}

		void AdjustRect(BOOL bLarger, LPRECT lpRect)
		{
			SendMessage(m_hWnd, TCM_ADJUSTRECT, bLarger, (LPARAM) lpRect);
		}

		void RemoveImage(int nImage)
		{
			SendMessage(m_hWnd, TCM_REMOVEIMAGE, nImage, 0L);
		}

		int HitTest(TC_HITTESTINFO* pHitTestInfo) const
		{
			return (int) SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM) pHitTestInfo);
		}

		void DeselectAll(BOOL bExcludeFocus = TRUE)
		{
			SendMessage(m_hWnd, TCM_DESELECTALL, bExcludeFocus, 0L);
		}

		BOOL HighlightItem(int nIndex, BOOL bHighlight = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TCM_HIGHLIGHTITEM, nIndex, MAKELPARAM(bHighlight, 0));
		}
	};

	class TrackBar : public Control
	{
	public:
		using Control::Control;

		int GetLineSize() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETLINESIZE, 0, 0L);
		}

		int SetLineSize(int nSize)
		{
			return (int) SendMessage(m_hWnd, TBM_SETLINESIZE, 0, nSize);
		}

		int GetPageSize() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0L);
		}

		int SetPageSize(int nSize)
		{
			return (int) SendMessage(m_hWnd, TBM_SETPAGESIZE, 0, nSize);
		}

		int GetRangeMin() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0L);
		}

		void SetRangeMin(int nMin, BOOL bRedraw = FALSE)
		{
			SendMessage(m_hWnd, TBM_SETRANGEMIN, bRedraw, nMin);
		}

		int GetRangeMax() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0L);
		}

		void SetRangeMax(int nMax, BOOL bRedraw = FALSE)
		{
			SendMessage(m_hWnd, TBM_SETRANGEMAX, bRedraw, nMax);
		}

		void GetRange(int& nMin, int& nMax) const
		{
			nMin = GetRangeMin();
			nMax = GetRangeMax();
		}

		void SetRange(int nMin, int nMax, BOOL bRedraw = TRUE)
		{
			SendMessage(m_hWnd, TBM_SETRANGE, bRedraw, MAKELPARAM(nMin, nMax));
		}

		int GetSelStart() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETSELSTART, 0, 0L);
		}

		void SetSelStart(int nMin, BOOL bRedraw = FALSE)
		{
			SendMessage(m_hWnd, TBM_SETSELSTART, bRedraw, (LPARAM) nMin);
		}

		int GetSelEnd() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETSELEND, 0, 0L);
		}

		void SetSelEnd(int nMax, BOOL bRedraw = FALSE)
		{
			SendMessage(m_hWnd, TBM_SETSELEND, bRedraw, (LPARAM) nMax);
		}

		void GetSelection(int& nMin, int& nMax) const
		{
			nMin = GetSelStart();
			nMax = GetSelEnd();
		}

		void SetSelection(int nMin, int nMax, BOOL bRedraw = TRUE)
		{
			SetSelStart(nMin, FALSE);
			SetSelEnd(nMax, bRedraw);
		}

		void GetChannelRect(LPRECT lprc) const
		{
			SendMessage(m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM) lprc);
		}

		void GetThumbRect(LPRECT lprc) const
		{
			SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM) lprc);
		}

		int GetPos() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETPOS, 0, 0L);
		}

		void SetPos(int nPos)
		{
			SendMessage(m_hWnd, TBM_SETPOS, TRUE, nPos);
		}

		UINT GetNumTics() const
		{
			return (UINT) SendMessage(m_hWnd, TBM_GETNUMTICS, 0, 0L);
		}

		DWORD* GetTicArray() const
		{
			return (DWORD*) SendMessage(m_hWnd, TBM_GETPTICS, 0, 0L);
		}

		int GetTic(int nTic) const
		{
			return (int) SendMessage(m_hWnd, TBM_GETTIC, nTic, 0L);
		}

		BOOL SetTic(int nTic)
		{
			return (BOOL) SendMessage(m_hWnd, TBM_SETTIC, 0, nTic);
		}

		int GetTicPos(int nTic) const
		{
			return (int) SendMessage(m_hWnd, TBM_GETTICPOS, nTic, 0L);
		}

		void SetTicFreq(int nFreq)
		{
			SendMessage(m_hWnd, TBM_SETTICFREQ, nFreq, 0L);
		}

		int GetThumbLength() const
		{
			return (int) SendMessage(m_hWnd, TBM_GETTHUMBLENGTH, 0, 0L);
		}

		void SetThumbLength(int nLength)
		{
			SendMessage(m_hWnd, TBM_SETTHUMBLENGTH, nLength, 0L);
		}

		void SetSel(int nStart, int nEnd, BOOL bRedraw = TRUE)
		{
			SendMessage(m_hWnd, TBM_SETSEL, bRedraw, MAKELPARAM(nStart, nEnd));
		}

		HWND GetBuddy(BOOL bLeft = TRUE) const
		{
			return  (HWND) SendMessage(m_hWnd, TBM_GETBUDDY, bLeft, 0L);
		}

		HWND SetBuddy(HWND hWndBuddy, BOOL bLeft = TRUE)
		{
			return (HWND) SendMessage(m_hWnd, TBM_SETBUDDY, bLeft, (LPARAM) hWndBuddy);
		}

		ToolTip GetToolTips() const
		{
			return ToolTip((HWND) SendMessage(m_hWnd, TBM_GETTOOLTIPS, 0, 0L));
		}

		void SetToolTips(HWND hWndTT)
		{
			SendMessage(m_hWnd, TBM_SETTOOLTIPS, (WPARAM) hWndTT, 0L);
		}

		int SetTipSide(int nSide)
		{
			return (int) SendMessage(m_hWnd, TBM_SETTIPSIDE, nSide, 0L);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, TBM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, TBM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		void ClearSel(BOOL bRedraw = FALSE)
		{
			SendMessage(m_hWnd, TBM_CLEARSEL, bRedraw, 0L);
		}

		void VerifyPos()
		{
			SendMessage(m_hWnd, TBM_SETPOS, FALSE, 0L);
		}

		void ClearTics(BOOL bRedraw = FALSE)
		{
			SendMessage(m_hWnd, TBM_CLEARTICS, bRedraw, 0L);
		}
	};

	class SpinControl : public Control
	{
	public:
		using Control::Control;

		UINT GetAccel(int nAccel, UDACCEL* pAccel) const
		{
			return (UINT) LOWORD(SendMessage(m_hWnd, UDM_GETACCEL, nAccel, (LPARAM) pAccel));
		}

		BOOL SetAccel(int nAccel, UDACCEL* pAccel)
		{
			return (BOOL) LOWORD(SendMessage(m_hWnd, UDM_SETACCEL, nAccel, (LPARAM) pAccel));
		}

		UINT GetBase() const
		{
			return (UINT) LOWORD(SendMessage(m_hWnd, UDM_GETBASE, 0, 0L));
		}

		int SetBase(int nBase)
		{
			return (int) SendMessage(m_hWnd, UDM_SETBASE, nBase, 0L);
		}

		HWND GetBuddy() const
		{
			return (HWND) SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0L);
		}

		HWND SetBuddy(HWND hWndBuddy)
		{
			return (HWND) SendMessage(m_hWnd, UDM_SETBUDDY, (WPARAM) hWndBuddy, 0L);
		}

		int GetPos(LPBOOL lpbError = NULL) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, UDM_GETPOS, 0, 0L);
			if (lpbError != NULL) *lpbError = (HIWORD(dwRet) != 0) ? TRUE : FALSE;
			return (int) (short) LOWORD(dwRet);
		}

		int SetPos(int nPos)
		{
			return (int) (short) LOWORD(SendMessage(m_hWnd, UDM_SETPOS, 0, MAKELPARAM(nPos, 0)));
		}

		DWORD GetRange() const
		{
			return (DWORD) SendMessage(m_hWnd, UDM_GETRANGE, 0, 0L);
		}

		void GetRange(int& nLower, int& nUpper) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, UDM_GETRANGE, 0, 0L);
			nLower = (int) (short) HIWORD(dwRet);
			nUpper = (int) (short) LOWORD(dwRet);
		}

		void SetRange(int nLower, int nUpper)
		{
			SendMessage(m_hWnd, UDM_SETRANGE, 0, MAKELPARAM(nUpper, nLower));
		}

		void SetRange32(int nLower, int nUpper)
		{
			SendMessage(m_hWnd, UDM_SETRANGE32, nLower, nUpper);
		}

		void GetRange32(int& nLower, int& nUpper) const
		{
			SendMessage(m_hWnd, UDM_GETRANGE32, (WPARAM) &nLower, (LPARAM) &nUpper);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, UDM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, UDM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int GetPos32(LPBOOL lpbError = NULL) const
		{
			return (int) SendMessage(m_hWnd, UDM_GETPOS32, 0, (LPARAM) lpbError);
		}

		int SetPos32(int nPos)
		{
			return (int) SendMessage(m_hWnd, UDM_SETPOS32, 0, (LPARAM) nPos);
		}
	};

	class ProgressBar : public Control
	{
	public:
		using Control::Control;

		DWORD SetRange(int nLower, int nUpper)
		{
			return (DWORD) SendMessage(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
		}

		int SetPos(int nPos)
		{
			return (int) (short) LOWORD(SendMessage(m_hWnd, PBM_SETPOS, nPos, 0L));
		}

		int OffsetPos(int nPos)
		{
			return (int) (short) LOWORD(SendMessage(m_hWnd, PBM_DELTAPOS, nPos, 0L));
		}

		int SetStep(int nStep)
		{
			return (int) (short) LOWORD(SendMessage(m_hWnd, PBM_SETSTEP, nStep, 0L));
		}

		UINT GetPos() const
		{
			return (UINT) SendMessage(m_hWnd, PBM_GETPOS, 0, 0L);
		}

		void GetRange(PPBRANGE pPBRange) const
		{
			SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM) pPBRange);
		}

		void GetRange(int& nLower, int& nUpper) const
		{
			PBRANGE range = {0};
			SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM) &range);
			nLower = range.iLow;
			nUpper = range.iHigh;
		}

		int GetRangeLimit(BOOL bLowLimit) const
		{
			return (int) SendMessage(m_hWnd, PBM_GETRANGE, bLowLimit, 0L);
		}

		DWORD SetRange32(int nMin, int nMax)
		{
			return (DWORD) SendMessage(m_hWnd, PBM_SETRANGE32, nMin, nMax);
		}

		COLORREF SetBarColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM) clr);
		}

		COLORREF SetBkColor(COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM) clr);
		}

		BOOL SetMarquee(BOOL bMarquee, UINT uUpdateTime = 0U)
		{
			if (bMarquee && (GetStyle() & PBS_MARQUEE) == 0) AddStyle(PBS_MARQUEE);
			return (BOOL) SendMessage(m_hWnd, PBM_SETMARQUEE, (WPARAM) bMarquee, (LPARAM) uUpdateTime);
		}

		int GetStep() const
		{
			return (int) SendMessage(m_hWnd, PBM_GETSTEP, 0, 0L);
		}

		COLORREF GetBkColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, PBM_GETBKCOLOR, 0, 0L);
		}

		COLORREF GetBarColor() const
		{
			return (COLORREF) SendMessage(m_hWnd, PBM_GETBARCOLOR, 0, 0L);
		}

		int GetState() const
		{
			return (int) SendMessage(m_hWnd, PBM_GETSTATE, 0, 0L);
		}

		int SetState(int nState)
		{
			return (int) SendMessage(m_hWnd, PBM_SETSTATE, nState, 0L);
		}

		int StepIt()
		{
			return (int) (short) LOWORD(SendMessage(m_hWnd, PBM_STEPIT, 0, 0L));
		}
	};

	class HotKey : public Control
	{
	public:
		using Control::Control;

		DWORD GetHotKey() const
		{
			return (DWORD) SendMessage(m_hWnd, HKM_GETHOTKEY, 0, 0L);
		}

		void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const
		{
			DWORD dw = (DWORD) SendMessage(m_hWnd, HKM_GETHOTKEY, 0, 0L);
			wVirtualKeyCode = LOBYTE(LOWORD(dw));
			wModifiers = HIBYTE(LOWORD(dw));
		}

		void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
		{
			SendMessage(m_hWnd, HKM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0L);
		}

		void SetRules(WORD wInvalidComb, WORD wModifiers)
		{
			SendMessage(m_hWnd, HKM_SETRULES, wInvalidComb, MAKELPARAM(wModifiers, 0));
		}
	};

	class Animation : public Control
	{
	public:
		using Control::Control;

		BOOL Open(LPCTSTR FileName)
		{
			return (BOOL) SendMessage(m_hWnd, ACM_OPEN, 0, (LPARAM) FileName);
		}

		BOOL Play(UINT nFrom, UINT nTo, UINT nRep)
		{
			return (BOOL) SendMessage(m_hWnd, ACM_PLAY, nRep, MAKELPARAM(nFrom, nTo));
		}

		BOOL Stop()
		{
			return (BOOL) SendMessage(m_hWnd, ACM_STOP, 0, 0L);
		}

		BOOL Close()
		{
			return (BOOL) SendMessage(m_hWnd, ACM_OPEN, 0, 0L);
		}

		BOOL Seek(UINT nTo)
		{
			return (BOOL) SendMessage(m_hWnd, ACM_PLAY, 0, MAKELPARAM(nTo, nTo));
		}

		BOOL IsPlaying() const
		{
			return (BOOL) SendMessage(m_hWnd, ACM_ISPLAYING, 0, 0L);
		}
	};

	class RichEdit : public Control
	{
	public:
		using Control::Control;

		int GetLineCount() const
		{
			return (int) SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
		}

		BOOL GetModify() const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETMODIFY, 0, 0L);
		}

		void SetModify(BOOL bModified = TRUE)
		{
			SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0L);
		}

		void GetRect(LPRECT lpRect) const
		{
			SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM) lpRect);
		}

		DWORD GetOptions() const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETOPTIONS, 0, 0L);
		}

		DWORD SetOptions(WORD wOperation, DWORD dwOptions)
		{
			return (DWORD) SendMessage(m_hWnd, EM_SETOPTIONS, wOperation, dwOptions);
		}

		int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
		{
			*(LPWORD) lpszBuffer = (WORD) nMaxLength;
			return (int) SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM) lpszBuffer);
		}

		BOOL CanUndo() const
		{
			return (BOOL) SendMessage(m_hWnd, EM_CANUNDO, 0, 0L);
		}

		BOOL CanPaste(UINT nFormat = 0) const
		{
			return (BOOL) SendMessage(m_hWnd, EM_CANPASTE, nFormat, 0L);
		}

		void GetSel(LONG& nStartChar, LONG& nEndChar) const
		{
			CHARRANGE cr = {0, 0};
			SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM) &cr);
			nStartChar = cr.cpMin;
			nEndChar = cr.cpMax;
		}

		void GetSel(CHARRANGE &cr) const
		{
			SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM) &cr);
		}

		int SetSel(LONG nStartChar, LONG nEndChar)
		{
			CHARRANGE cr = {nStartChar, nEndChar};
			return (int) SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM) &cr);
		}

		int SetSel(CHARRANGE &cr)
		{
			return (int) SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM) &cr);
		}

		int SetSelAll()
		{
			return SetSel(0, -1);
		}

		int SetSelNone()
		{
			return SetSel(-1, 0);
		}

		DWORD GetDefaultCharFormat(CHARFORMAT& cf) const
		{
			cf.cbSize = sizeof(CHARFORMAT);
			return (DWORD) SendMessage(m_hWnd, EM_GETCHARFORMAT, 0, (LPARAM) &cf);
		}

		DWORD GetSelectionCharFormat(CHARFORMAT& cf) const
		{
			cf.cbSize = sizeof(CHARFORMAT);
			return (DWORD) SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		}

		DWORD GetEventMask() const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETEVENTMASK, 0, 0L);
		}

		LONG GetLimitText() const
		{
			return (LONG) SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0L);
		}

		DWORD GetParaFormat(PARAFORMAT& pf) const
		{
			pf.cbSize = sizeof(PARAFORMAT);
			return (DWORD) SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		}

		LONG GetSelText(LPTSTR lpstrBuff) const
		{
			return (LONG) SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM) lpstrBuff);
		}

		WORD GetSelectionType() const
		{
			return (WORD) SendMessage(m_hWnd, EM_SELECTIONTYPE, 0, 0L);
		}

		COLORREF SetBackgroundColor(COLORREF cr)
		{
			return (COLORREF) SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 0, cr);
		}

		COLORREF SetBackgroundColor()
		{
			return (COLORREF) SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 1, 0);
		}

		BOOL SetCharFormat(CHARFORMAT& cf, WORD wFlags)
		{
			cf.cbSize = sizeof(CHARFORMAT);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, (WPARAM) wFlags, (LPARAM) &cf);
		}

		BOOL SetDefaultCharFormat(CHARFORMAT& cf)
		{
			cf.cbSize = sizeof(CHARFORMAT);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM) &cf);
		}

		BOOL SetSelectionCharFormat(CHARFORMAT& cf)
		{
			cf.cbSize = sizeof(CHARFORMAT);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
		}

		BOOL SetWordCharFormat(CHARFORMAT& cf)
		{
			cf.cbSize = sizeof(CHARFORMAT);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM) &cf);
		}

		DWORD SetEventMask(DWORD dwEventMask)
		{
			return (DWORD) SendMessage(m_hWnd, EM_SETEVENTMASK, 0, dwEventMask);
		}

		BOOL SetParaFormat(PARAFORMAT& pf)
		{
			pf.cbSize = sizeof(PARAFORMAT);
			return (BOOL) SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf);
		}

		BOOL SetTargetDevice(HDC hDC, int cxLineWidth)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTARGETDEVICE, (WPARAM) hDC, cxLineWidth);
		}

		int GetTextLength() const
		{
			return (int) SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0L);
		}

		BOOL SetReadOnly(BOOL bReadOnly = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
		}

		int GetFirstVisibleLine() const
		{
			return (int) SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
		}

		int GetTextRange(TEXTRANGE* pTextRange) const
		{
			return (int) SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) pTextRange);
		}

		int GetTextRange(LONG nStartChar, LONG nEndChar, LPTSTR lpstrText) const
		{
			TEXTRANGE tr = {0};
			tr.chrg.cpMin = nStartChar;
			tr.chrg.cpMax = nEndChar;
			tr.lpstrText = lpstrText;
			return (int) SendMessage(m_hWnd, EM_GETTEXTRANGE, 0, (LPARAM) &tr);
		}

		DWORD GetDefaultCharFormat(CHARFORMAT2& cf) const
		{
			cf.cbSize = sizeof(CHARFORMAT2);
			return (DWORD) SendMessage(m_hWnd, EM_GETCHARFORMAT, 0, (LPARAM) &cf);
		}

		BOOL SetCharFormat(CHARFORMAT2& cf, WORD wFlags)
		{
			cf.cbSize = sizeof(CHARFORMAT2);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, (WPARAM) wFlags, (LPARAM) &cf);
		}

		BOOL SetDefaultCharFormat(CHARFORMAT2& cf)
		{
			cf.cbSize = sizeof(CHARFORMAT2);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM) &cf);
		}

		DWORD GetSelectionCharFormat(CHARFORMAT2& cf) const
		{
			cf.cbSize = sizeof(CHARFORMAT2);
			return (DWORD) SendMessage(m_hWnd, EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		}

		BOOL SetSelectionCharFormat(CHARFORMAT2& cf)
		{
			cf.cbSize = sizeof(CHARFORMAT2);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
		}

		BOOL SetWordCharFormat(CHARFORMAT2& cf)
		{
			cf.cbSize = sizeof(CHARFORMAT2);
			return (BOOL) SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM) &cf);
		}

		DWORD GetParaFormat(PARAFORMAT2& pf) const
		{
			pf.cbSize = sizeof(PARAFORMAT2);
			return (DWORD) SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		}

		BOOL SetParaFormat(PARAFORMAT2& pf)
		{
			pf.cbSize = sizeof(PARAFORMAT2);
			return (BOOL) SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf);
		}

		TEXTMODE GetTextMode() const
		{
			return (TEXTMODE) SendMessage(m_hWnd, EM_GETTEXTMODE, 0, 0L);
		}

		BOOL SetTextMode(TEXTMODE enumTextMode)
		{
			return !(BOOL) SendMessage(m_hWnd, EM_SETTEXTMODE, enumTextMode, 0L);
		}

		UNDONAMEID GetUndoName() const
		{
			return (UNDONAMEID) SendMessage(m_hWnd, EM_GETUNDONAME, 0, 0L);
		}

		UNDONAMEID GetRedoName() const
		{
			return (UNDONAMEID) SendMessage(m_hWnd, EM_GETREDONAME, 0, 0L);
		}

		BOOL CanRedo() const
		{
			return (BOOL) SendMessage(m_hWnd, EM_CANREDO, 0, 0L);
		}

		BOOL GetAutoURLDetect() const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETAUTOURLDETECT, 0, 0L);
		}

		BOOL SetAutoURLDetect(BOOL bAutoDetect = TRUE)
		{
			return !(BOOL) SendMessage(m_hWnd, EM_AUTOURLDETECT, bAutoDetect, 0L);
		}

		UINT SetUndoLimit(UINT uUndoLimit)
		{
			return (UINT) SendMessage(m_hWnd, EM_SETUNDOLIMIT, uUndoLimit, 0L);
		}

		void SetPalette(HPALETTE hPalette)
		{
			SendMessage(m_hWnd, EM_SETPALETTE, (WPARAM) hPalette, 0L);
		}

		int GetTextEx(GETTEXTEX* pGetTextEx, LPTSTR lpstrText) const
		{
			return (int) SendMessage(m_hWnd, EM_GETTEXTEX, (WPARAM) pGetTextEx, (LPARAM) lpstrText);
		}

		int GetTextEx(LPTSTR lpstrText, int nTextLen, DWORD dwFlags = GT_DEFAULT, UINT uCodePage = CP_ACP, LPCSTR lpDefaultChar = NULL, LPBOOL lpUsedDefChar = NULL) const
		{
			GETTEXTEX gte = {0};
			gte.cb = nTextLen * sizeof(TCHAR);
			gte.codepage = uCodePage;
			gte.flags = dwFlags;
			gte.lpDefaultChar = lpDefaultChar;
			gte.lpUsedDefChar = lpUsedDefChar;
			return (int) SendMessage(m_hWnd, EM_GETTEXTEX, (WPARAM) &gte, (LPARAM) lpstrText);
		}

		int GetTextLengthEx(GETTEXTLENGTHEX* pGetTextLengthEx) const
		{
			return (int) SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM) pGetTextLengthEx, 0L);
		}

		int GetTextLengthEx(DWORD dwFlags = GTL_DEFAULT, UINT uCodePage = CP_ACP) const
		{
			GETTEXTLENGTHEX gtle = {0};
			gtle.codepage = uCodePage;
			gtle.flags = dwFlags;
			return (int) SendMessage(m_hWnd, EM_GETTEXTLENGTHEX, (WPARAM) &gtle, 0L);
		}

		EDITWORDBREAKPROC GetWordBreakProc() const
		{
			return (EDITWORDBREAKPROC) SendMessage(m_hWnd, EM_GETWORDBREAKPROC, 0, 0L);
		}

		void SetWordBreakProc(EDITWORDBREAKPROC ewbprc)
		{
			SendMessage(m_hWnd, EM_SETWORDBREAKPROC, 0, (LPARAM) ewbprc);
		}

		int SetTextEx(SETTEXTEX* pSetTextEx, LPCTSTR lpstrText)
		{
			return (int) SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM) pSetTextEx, (LPARAM) lpstrText);
		}

		int SetTextEx(LPCTSTR lpstrText, DWORD dwFlags = ST_DEFAULT, UINT uCodePage = CP_ACP)
		{
			SETTEXTEX ste = {0};
			ste.flags = dwFlags;
			ste.codepage = uCodePage;
			return (int) SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM) &ste, (LPARAM) lpstrText);
		}

		int GetEditStyle() const
		{
			return (int) SendMessage(m_hWnd, EM_GETEDITSTYLE, 0, 0L);
		}

		int SetEditStyle(int nStyle, int nMask = -1)
		{
			if (nMask == -1) nMask = nStyle;
			return (int) SendMessage(m_hWnd, EM_SETEDITSTYLE, nStyle, nMask);
		}

		BOOL SetFontSize(int nFontSizeDelta)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETFONTSIZE, nFontSizeDelta, 0L);
		}

		void GetScrollPos(LPPOINT lpPoint) const
		{
			SendMessage(m_hWnd, EM_GETSCROLLPOS, 0, (LPARAM) lpPoint);
		}

		void SetScrollPos(LPPOINT lpPoint)
		{
			SendMessage(m_hWnd, EM_SETSCROLLPOS, 0, (LPARAM) lpPoint);
		}

		BOOL GetZoom(int& nNum, int& nDen) const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETZOOM, (WPARAM) &nNum, (LPARAM) &nDen);
		}

		BOOL SetZoom(int nNum, int nDen)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETZOOM, nNum, nDen);
		}

		BOOL SetZoomOff()
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETZOOM, 0, 0L);
		}

		void SetMargins(UINT nLeft, UINT nRight, WORD wFlags = EC_LEFTMARGIN | EC_RIGHTMARGIN)
		{
			SendMessage(m_hWnd, EM_SETMARGINS, wFlags, MAKELONG(nLeft, nRight));
		}

		void LimitText(LONG nChars = 0)
		{
			SendMessage(m_hWnd, EM_EXLIMITTEXT, 0, nChars);
		}

		int LineFromChar(LONG nIndex) const
		{
			return (int) SendMessage(m_hWnd, EM_EXLINEFROMCHAR, 0, nIndex);
		}

		POINT PosFromChar(LONG nChar) const
		{
			POINT point = {0, 0};
			SendMessage(m_hWnd, EM_POSFROMCHAR, (WPARAM) &point, nChar);
			return point;
		}

		int CharFromPos(POINT pt) const
		{
			POINTL ptl = {pt.x, pt.y};
			return (int) SendMessage(m_hWnd, EM_CHARFROMPOS, 0, (LPARAM) &ptl);
		}

		void EmptyUndoBuffer()
		{
			SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0L);
		}

		int LineIndex(int nLine = -1) const
		{
			return (int) SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0L);
		}

		int LineLength(int nLine = -1) const
		{
			return (int) SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0L);
		}

		BOOL LineScroll(int nLines)
		{
			return (BOOL) SendMessage(m_hWnd, EM_LINESCROLL, 0, nLines);
		}

		void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE)
		{
			SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM) lpszNewText);
		}

		void SetRect(LPCRECT lpRect)
		{
			SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM) lpRect);
		}

		BOOL DisplayBand(LPRECT pDisplayRect)
		{
			return (BOOL) SendMessage(m_hWnd, EM_DISPLAYBAND, 0, (LPARAM) pDisplayRect);
		}

		LONG FindText(DWORD dwFlags, FINDTEXT& ft) const
		{
#if defined(_UNICODE)
			return (LONG) SendMessage(m_hWnd, EM_FINDTEXTW, dwFlags, (LPARAM) &ft);
#else
			return (LONG) SendMessage(m_hWnd, EM_FINDTEXT, dwFlags, (LPARAM) &ft);
#endif
		}

		LONG FindText(DWORD dwFlags, FINDTEXTEX& ft) const
		{
#if defined(_UNICODE)
			return (LONG) SendMessage(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM) &ft);
#else
			return (LONG) SendMessage(m_hWnd, EM_FINDTEXTEX, dwFlags, (LPARAM) &ft);
#endif
		}

		LONG FormatRange(FORMATRANGE& fr, BOOL bDisplay = TRUE)
		{
			return (LONG) SendMessage(m_hWnd, EM_FORMATRANGE, bDisplay, (LPARAM) &fr);
		}

		LONG FormatRange(FORMATRANGE* pFormatRange, BOOL bDisplay = TRUE)
		{
			return (LONG) SendMessage(m_hWnd, EM_FORMATRANGE, bDisplay, (LPARAM) pFormatRange);
		}

		void HideSelection(BOOL bHide = TRUE, BOOL bChangeStyle = FALSE)
		{
			SendMessage(m_hWnd, EM_HIDESELECTION, bHide, bChangeStyle);
		}

		void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0)
		{
			REPASTESPECIAL reps = {dwAspect, (DWORD_PTR) hMF};
			SendMessage(m_hWnd, EM_PASTESPECIAL, uClipFormat, (LPARAM) &reps);
		}

		void RequestResize()
		{
			SendMessage(m_hWnd, EM_REQUESTRESIZE, 0, 0L);
		}

		LONG StreamIn(UINT uFormat, EDITSTREAM& es)
		{
			return (LONG) SendMessage(m_hWnd, EM_STREAMIN, uFormat, (LPARAM) &es);
		}

		LONG StreamOut(UINT uFormat, EDITSTREAM& es)
		{
			return (LONG) SendMessage(m_hWnd, EM_STREAMOUT, uFormat, (LPARAM) &es);
		}

		DWORD FindWordBreak(int nCode, LONG nStartChar)
		{
			return (DWORD) SendMessage(m_hWnd, EM_FINDWORDBREAK, nCode, nStartChar);
		}

		void ScrollCaret()
		{
			SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
		}

		int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, BOOL bCanUndo = FALSE)
		{
			int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
			ReplaceSel(lpstrText, bCanUndo);
			return nRet;
		}

		int AppendText(LPCTSTR lpstrText, BOOL bCanUndo = FALSE)
		{
			return InsertText(GetTextLength(), lpstrText, bCanUndo);
		}

		BOOL Undo()
		{
			return (BOOL) SendMessage(m_hWnd, EM_UNDO, 0, 0L);
		}

		void Clear()
		{
			SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
		}

		void Copy()
		{
			SendMessage(m_hWnd, WM_COPY, 0, 0L);
		}

		void Cut()
		{
			SendMessage(m_hWnd, WM_CUT, 0, 0L);
		}

		void Paste()
		{
			SendMessage(m_hWnd, WM_PASTE, 0, 0L);
		}

		IRichEditOle* GetOleInterface() const
		{
			IRichEditOle *pRichEditOle = NULL;
			SendMessage(m_hWnd, EM_GETOLEINTERFACE, 0, (LPARAM) &pRichEditOle);
			return pRichEditOle;
		}

		BOOL SetOleCallback(IRichEditOleCallback* pCallback)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM) pCallback);
		}

		BOOL Redo()
		{
			return (BOOL) SendMessage(m_hWnd, EM_REDO, 0, 0L);
		}

		void StopGroupTyping()
		{
			SendMessage(m_hWnd, EM_STOPGROUPTYPING, 0, 0L);
		}

		void ShowScrollBar(int nBarType, BOOL bVisible = TRUE)
		{
			SendMessage(m_hWnd, EM_SHOWSCROLLBAR, nBarType, bVisible);
		}

		BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM) rgTabStops);
		}

		BOOL SetTabStops()
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);
		}

		BOOL SetTabStops(const int& cxEachStop)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETTABSTOPS, 1, (LPARAM) (LPINT) &cxEachStop);
		}

		::AutoCorrectProc GetAutoCorrectProc() const
		{
			return (::AutoCorrectProc) SendMessage(m_hWnd, EM_GETAUTOCORRECTPROC, 0, 0L);
		}

		BOOL SetAutoCorrectProc(::AutoCorrectProc pfn)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETAUTOCORRECTPROC, (WPARAM) pfn, 0L);
		}

		BOOL CallAutoCorrectProc(WCHAR ch)
		{
			return (BOOL) SendMessage(m_hWnd, EM_CALLAUTOCORRECTPROC, (WPARAM) ch, 0L);
		}

		DWORD GetEditStyleEx() const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETEDITSTYLEEX, 0, 0L);
		}

		DWORD SetEditStyleEx(DWORD dwStyleEx, DWORD dwMask)
		{
			return (DWORD) SendMessage(m_hWnd, EM_SETEDITSTYLEEX, dwStyleEx, dwMask);
		}

		DWORD GetStoryType(int nStoryIndex) const
		{
			return (DWORD) SendMessage(m_hWnd, EM_GETSTORYTYPE, nStoryIndex, 0L);
		}

		DWORD SetStoryType(int nStoryIndex, DWORD dwStoryType)
		{
			return (DWORD) SendMessage(m_hWnd, EM_SETSTORYTYPE, nStoryIndex, dwStoryType);
		}

		DWORD GetEllipsisMode() const
		{
			DWORD dwMode = 0;
			BOOL bRet = (BOOL) SendMessage(m_hWnd, EM_GETELLIPSISMODE, 0, (LPARAM) &dwMode);
			bRet;
			return dwMode;
		}

		BOOL SetEllipsisMode(DWORD dwEllipsisMode)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETELLIPSISMODE, 0, dwEllipsisMode);
		}

		BOOL GetEllipsisState() const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETELLIPSISSTATE, 0, 0L);
		}

		BOOL GetTouchOptions(int nTouchOptions) const
		{
			return (BOOL) SendMessage(m_hWnd, EM_GETTOUCHOPTIONS, nTouchOptions, 0L);
		}

		void SetTouchOptions(int nTouchOptions, BOOL bEnable)
		{
			SendMessage(m_hWnd, EM_SETTOUCHOPTIONS, nTouchOptions, bEnable);
		}

		HRESULT InsertTable(TABLEROWPARMS* pRowParams, TABLECELLPARMS* pCellParams)
		{
			return (HRESULT) SendMessage(m_hWnd, EM_INSERTTABLE, (WPARAM) pRowParams, (LPARAM) pCellParams);
		}

		HRESULT GetTableParams(TABLEROWPARMS* pRowParams, TABLECELLPARMS* pCellParams) const
		{
			return (HRESULT) SendMessage(m_hWnd, EM_GETTABLEPARMS, (WPARAM) pRowParams, (LPARAM) pCellParams);
		}

		HRESULT SetTableParams(TABLEROWPARMS* pRowParams, TABLECELLPARMS* pCellParams)
		{
			return (HRESULT) SendMessage(m_hWnd, EM_SETTABLEPARMS, (WPARAM) pRowParams, (LPARAM) pCellParams);
		}

		HRESULT InsertImage(RICHEDIT_IMAGE_PARAMETERS* pParams)
		{
			return (HRESULT) SendMessage(m_hWnd, EM_INSERTIMAGE, 0, (LPARAM) pParams);
		}

		BOOL SetUiaName(LPCTSTR lpstrName)
		{
			return (BOOL) SendMessage(m_hWnd, EM_SETUIANAME, 0, (LPARAM) lpstrName);
		}
	};

	class ComboBoxEx : public Control
	{
	public:
		using Control::Control;

		ImageList GetImageList() const
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0L));
		}

		ImageList SetImageList(HIMAGELIST hImageList)
		{
			return ImageList((HIMAGELIST) SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM) hImageList));
		}

		DWORD GetExtendedStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0L);
		}

		DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyle)
		{
			return (DWORD) SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, dwExMask, dwExStyle);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, CBEM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, CBEM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		void SetWindowTheme(LPCWSTR lpstrTheme)
		{
			SendMessage(m_hWnd, CBEM_SETWINDOWTHEME, 0, (LPARAM) lpstrTheme);
		}

		int InsertItem(const COMBOBOXEXITEM* lpcCBItem)
		{
			return (int) SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM) lpcCBItem);
		}

		int InsertItem(UINT nMask, int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage,
					   int iIndent, int iOverlay, LPARAM lParam)
		{
			COMBOBOXEXITEM cbex = {0};
			cbex.mask = nMask;
			cbex.iItem = nIndex;
			cbex.pszText = (LPTSTR) lpszItem;
			cbex.iImage = nImage;
			cbex.iSelectedImage = nSelImage;
			cbex.iIndent = iIndent;
			cbex.iOverlay = iOverlay;
			cbex.lParam = lParam;
			return (int) SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM) &cbex);
		}

		int InsertItem(int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, LPARAM lParam = 0)
		{
			COMBOBOXEXITEM cbex = {0};
			cbex.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT | CBEIF_LPARAM;
			cbex.iItem = nIndex;
			cbex.pszText = (LPTSTR) lpszItem;
			cbex.iImage = nImage;
			cbex.iSelectedImage = nSelImage;
			cbex.iIndent = iIndent;
			cbex.lParam = lParam;
			return (int) SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM) &cbex);
		}

		int AddItem(UINT nMask, LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, int iOverlay, LPARAM lParam)
		{
			return InsertItem(nMask, -1, lpszItem, nImage, nSelImage, iIndent, iOverlay, lParam);
		}

		int AddItem(LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, LPARAM lParam = 0)
		{
			return InsertItem(-1, lpszItem, nImage, nSelImage, iIndent, lParam);
		}

		int DeleteItem(int nIndex)
		{
			return (int) SendMessage(m_hWnd, CBEM_DELETEITEM, nIndex, 0L);
		}

		BOOL GetItem(PCOMBOBOXEXITEM pCBItem) const
		{
			return (BOOL) SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM) pCBItem);
		}

		BOOL SetItem(const COMBOBOXEXITEM* lpcCBItem)
		{
			return (BOOL) SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM) lpcCBItem);
		}

		int SetItem(int nIndex, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelImage,
					int iIndent, int iOverlay, LPARAM lParam)
		{
			COMBOBOXEXITEM cbex = {0};
			cbex.mask = nMask;
			cbex.iItem = nIndex;
			cbex.pszText = (LPTSTR) lpszItem;
			cbex.iImage = nImage;
			cbex.iSelectedImage = nSelImage;
			cbex.iIndent = iIndent;
			cbex.iOverlay = iOverlay;
			cbex.lParam = lParam;
			return (int) SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM) &cbex);
		}

		BOOL GetItemText(int nIndex, LPTSTR lpszItem, int nLen) const
		{
			COMBOBOXEXITEM cbex = {0};
			cbex.mask = CBEIF_TEXT;
			cbex.iItem = nIndex;
			cbex.pszText = lpszItem;
			cbex.cchTextMax = nLen;

			return (BOOL) SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM) &cbex);
		}

		BOOL SetItemText(int nIndex, LPCTSTR lpszItem)
		{
			return SetItem(nIndex, CBEIF_TEXT, lpszItem, 0, 0, 0, 0, 0);
		}

		ComboBox GetComboCtrl() const
		{
			return ComboBox((HWND) SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0L));
		}

		EditText GetEditCtrl() const
		{
			return EditText((HWND) SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0L));
		}

		BOOL HasEditChanged() const
		{
			return (BOOL) SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0L);
		}
	};

	class MonthCalendar : public Control
	{
	public:
		using Control::Control;

		COLORREF GetColor(int nColorType) const
		{
			return (COLORREF) SendMessage(m_hWnd, MCM_GETCOLOR, nColorType, 0L);
		}

		COLORREF SetColor(int nColorType, COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, MCM_SETCOLOR, nColorType, clr);
		}

		BOOL GetCurSel(LPSYSTEMTIME lpSysTime) const
		{
			return (BOOL) SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) lpSysTime);
		}

		BOOL SetCurSel(LPSYSTEMTIME lpSysTime)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM) lpSysTime);
		}

		int GetFirstDayOfWeek(BOOL* pbLocaleVal = NULL) const
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, MCM_GETFIRSTDAYOFWEEK, 0, 0L);
			if (pbLocaleVal != NULL) *pbLocaleVal = (BOOL) HIWORD(dwRet);
			return (int) (short) LOWORD(dwRet);
		}

		int SetFirstDayOfWeek(int nDay, BOOL* pbLocaleVal = NULL)
		{
			DWORD dwRet = (DWORD) SendMessage(m_hWnd, MCM_SETFIRSTDAYOFWEEK, 0, nDay);
			if (pbLocaleVal != NULL) *pbLocaleVal = (BOOL) HIWORD(dwRet);
			return (int) (short) LOWORD(dwRet);
		}

		int GetMaxSelCount() const
		{
			return (int) SendMessage(m_hWnd, MCM_GETMAXSELCOUNT, 0, 0L);
		}

		BOOL SetMaxSelCount(int nMax)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETMAXSELCOUNT, nMax, 0L);
		}

		int GetMonthDelta() const
		{
			return (int) SendMessage(m_hWnd, MCM_GETMONTHDELTA, 0, 0L);
		}

		int SetMonthDelta(int nDelta)
		{
			return (int) SendMessage(m_hWnd, MCM_SETMONTHDELTA, nDelta, 0L);
		}

		DWORD GetRange(LPSYSTEMTIME lprgSysTimeArray) const
		{
			return (DWORD) SendMessage(m_hWnd, MCM_GETRANGE, 0, (LPARAM) lprgSysTimeArray);
		}

		BOOL SetRange(DWORD dwFlags, LPSYSTEMTIME lprgSysTimeArray)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETRANGE, dwFlags, (LPARAM) lprgSysTimeArray);
		}

		BOOL GetSelRange(LPSYSTEMTIME lprgSysTimeArray) const
		{
			return (BOOL) SendMessage(m_hWnd, MCM_GETSELRANGE, 0, (LPARAM) lprgSysTimeArray);
		}

		BOOL SetSelRange(LPSYSTEMTIME lprgSysTimeArray)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETSELRANGE, 0, (LPARAM) lprgSysTimeArray);
		}

		BOOL GetToday(LPSYSTEMTIME lpSysTime) const
		{
			return (BOOL) SendMessage(m_hWnd, MCM_GETTODAY, 0, (LPARAM) lpSysTime);
		}

		void SetToday(LPSYSTEMTIME lpSysTime)
		{
			SendMessage(m_hWnd, MCM_SETTODAY, 0, (LPARAM) lpSysTime);
		}

		BOOL GetMinReqRect(LPRECT lpRectInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, MCM_GETMINREQRECT, 0, (LPARAM) lpRectInfo);
		}

		int GetMaxTodayWidth() const
		{
			return (int) SendMessage(m_hWnd, MCM_GETMAXTODAYWIDTH, 0, 0L);
		}

		BOOL GetUnicodeFormat() const
		{
			return (BOOL) SendMessage(m_hWnd, MCM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL SetUnicodeFormat(BOOL bUnicode = TRUE)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		DWORD GetCurrentView() const
		{
			return (DWORD) SendMessage(m_hWnd, MCM_GETCURRENTVIEW, 0, 0L);
		}

		BOOL SetCurrentView(DWORD dwView)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETCURRENTVIEW, 0, dwView);
		}

		DWORD GetCalendarCount() const
		{
			return (DWORD) SendMessage(m_hWnd, MCM_GETCALENDARCOUNT, 0, 0L);
		}

		BOOL GetCalendarGridInfo(PMCGRIDINFO pGridInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, MCM_GETCALENDARGRIDINFO, 0, (LPARAM) pGridInfo);
		}

		CALID GetCALID() const
		{
			return (CALID) SendMessage(m_hWnd, MCM_GETCALID, 0, 0L);
		}

		void SetCALID(CALID calid)
		{
			SendMessage(m_hWnd, MCM_SETCALID, (LPARAM) calid, 0L);
		}

		int GetCalendarBorder() const
		{
			return (int) SendMessage(m_hWnd, MCM_GETCALENDARBORDER, 0, 0L);
		}

		void SetCalendarBorder(int cxyBorder, BOOL bSet = TRUE)
		{
			SendMessage(m_hWnd, MCM_SETCALENDARBORDER, (WPARAM) bSet, (LPARAM) cxyBorder);
		}

		int GetMonthRange(DWORD dwFlags, LPSYSTEMTIME lprgSysTimeArray) const
		{
			return (int) SendMessage(m_hWnd, MCM_GETMONTHRANGE, dwFlags, (LPARAM) lprgSysTimeArray);
		}

		BOOL SetDayState(int nMonths, LPMONTHDAYSTATE lpDayStateArray)
		{
			return (BOOL) SendMessage(m_hWnd, MCM_SETDAYSTATE, nMonths, (LPARAM) lpDayStateArray);
		}

		DWORD HitTest(PMCHITTESTINFO pMCHitTest) const
		{
			return (DWORD) SendMessage(m_hWnd, MCM_HITTEST, 0, (LPARAM) pMCHitTest);
		}

		void SizeRectToMin(LPRECT lpRect)
		{
			SendMessage(m_hWnd, MCM_SIZERECTTOMIN, 0, (LPARAM) lpRect);
		}
	};

	class DateTimePicker : public Control
	{
	public:
		using Control::Control;

		BOOL SetFormat(LPCTSTR lpszFormat)
		{
			return (BOOL) SendMessage(m_hWnd, DTM_SETFORMAT, 0, (LPARAM) lpszFormat);
		}

		COLORREF GetMonthCalColor(int nColorType) const
		{
			return (COLORREF) SendMessage(m_hWnd, DTM_GETMCCOLOR, nColorType, 0L);
		}

		COLORREF SetMonthCalColor(int nColorType, COLORREF clr)
		{
			return (COLORREF) SendMessage(m_hWnd, DTM_SETMCCOLOR, nColorType, clr);
		}

		DWORD GetRange(LPSYSTEMTIME lpSysTimeArray) const
		{
			return (DWORD) SendMessage(m_hWnd, DTM_GETRANGE, 0, (LPARAM) lpSysTimeArray);
		}

		BOOL SetRange(DWORD dwFlags, LPSYSTEMTIME lpSysTimeArray)
		{
			return (BOOL) SendMessage(m_hWnd, DTM_SETRANGE, dwFlags, (LPARAM) lpSysTimeArray);
		}

		DWORD GetSystemTime(LPSYSTEMTIME lpSysTime) const
		{
			return (DWORD) SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM) lpSysTime);
		}

		BOOL SetSystemTime(DWORD dwFlags, LPSYSTEMTIME lpSysTime)
		{
			return (BOOL) SendMessage(m_hWnd, DTM_SETSYSTEMTIME, dwFlags, (LPARAM) lpSysTime);
		}

		MonthCalendar GetMonthCal() const
		{
			return MonthCalendar((HWND) SendMessage(m_hWnd, DTM_GETMONTHCAL, 0, 0L));
		}

		HFONT GetMonthCalFont() const
		{
			return (HFONT) SendMessage(m_hWnd, DTM_GETMCFONT, 0, 0L);
		}

		void SetMonthCalFont(HFONT hFont, BOOL bRedraw = TRUE)
		{
			SendMessage(m_hWnd, DTM_SETMCFONT, (WPARAM) hFont, MAKELPARAM(bRedraw, 0));
		}

		DWORD GetMonthCalStyle() const
		{
			return (DWORD) SendMessage(m_hWnd, DTM_GETMCSTYLE, 0, 0L);
		}

		DWORD SetMonthCalStyle(DWORD dwStyle)
		{
			return (DWORD) SendMessage(m_hWnd, DTM_SETMCSTYLE, 0, (LPARAM) dwStyle);
		}

		void GetDateTimePickerInfo(LPDATETIMEPICKERINFO lpPickerInfo) const
		{
			SendMessage(m_hWnd, DTM_GETDATETIMEPICKERINFO, 0, (LPARAM) lpPickerInfo);
		}

		BOOL GetIdealSize(LPSIZE lpSize) const
		{
			return (BOOL) SendMessage(m_hWnd, DTM_GETIDEALSIZE, 0, (LPARAM) lpSize);
		}

		void CloseMonthCal()
		{
			SendMessage(m_hWnd, DTM_CLOSEMONTHCAL, 0, 0L);
		}
	};

	class IP4Address : public Control
	{
	public:
		using Control::Control;

		BOOL IsBlank() const
		{
			return (BOOL) SendMessage(m_hWnd, IPM_ISBLANK, 0, 0L);
		}

		int GetAddress(LPDWORD lpdwAddress) const
		{
			return (int) SendMessage(m_hWnd, IPM_GETADDRESS, 0, (LPARAM) lpdwAddress);
		}

		void SetAddress(DWORD dwAddress)
		{
			SendMessage(m_hWnd, IPM_SETADDRESS, 0, dwAddress);
		}

		void SetAddress(unsigned b1, unsigned b2, unsigned b3, unsigned b4)
		{
			SendMessage(m_hWnd, IPM_SETADDRESS, 0, MAKEIPADDRESS(b1, b2, b3, b4));
		}

		void ClearAddress()
		{
			SendMessage(m_hWnd, IPM_CLEARADDRESS, 0, 0L);
		}

		void SetRange(int nField, WORD wRange)
		{
			SendMessage(m_hWnd, IPM_SETRANGE, nField, wRange);
		}

		void SetRange(int nField, BYTE nMin, BYTE nMax)
		{
			SendMessage(m_hWnd, IPM_SETRANGE, nField, MAKEIPRANGE(nMin, nMax));
		}

		void SetFocus(int nField)
		{
			SendMessage(m_hWnd, IPM_SETFOCUS, nField, 0L);
		}
	};

	class LinkControl : public Control
	{
	public:
		using Control::Control;

		int GetIdealHeight(int cxMaxWidth = 0) const
		{
			return (int) SendMessage(m_hWnd, LM_GETIDEALHEIGHT, cxMaxWidth, 0L);
		}

		BOOL GetItem(PLITEM pLItem) const
		{
			return (BOOL) SendMessage(m_hWnd, LM_GETITEM, 0, (LPARAM) pLItem);
		}

		BOOL SetItem(PLITEM pLItem)
		{
			return (BOOL) SendMessage(m_hWnd, LM_SETITEM, 0, (LPARAM) pLItem);
		}

		int GetIdealSize(SIZE& size, int cxMaxWidth = 0) const
		{
			return (int) SendMessage(m_hWnd, LM_GETIDEALSIZE, cxMaxWidth, (LPARAM) &size);
		}

		BOOL HitTest(PLHITTESTINFO pLHitTestInfo) const
		{
			return (BOOL) SendMessage(m_hWnd, LM_HITTEST, 0, (LPARAM) pLHitTestInfo);
		}
	};
}

#endif //__INTERFACES_H__
