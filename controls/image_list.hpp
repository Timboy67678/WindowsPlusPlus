#ifndef __IMAGE_LIST_HPP__
#define __IMAGE_LIST_HPP__

namespace wpp
{
	class image_list {
	public:
		HIMAGELIST m_image_list_handle;

		image_list(HIMAGELIST hImageList = NULL) : m_image_list_handle(hImageList) {
		}

		operator HIMAGELIST() const { return m_image_list_handle; }
		bool is_null() const { return (m_image_list_handle == NULL); }
		explicit operator bool() const { return !is_null(); }

		// Attributes
		int get_image_count() const {
			return ImageList_GetImageCount(m_image_list_handle);
		}

		COLORREF get_back_color() const {
			return ImageList_GetBkColor(m_image_list_handle);
		}

		COLORREF set_back_color(COLORREF cr) {
			return ImageList_SetBkColor(m_image_list_handle, cr);
		}

		BOOL get_image_info(int nImage, IMAGEINFO* pImageInfo) const {
			return ImageList_GetImageInfo(m_image_list_handle, nImage, pImageInfo);
		}

		HICON get_icon(int nIndex, UINT uFlags = ILD_NORMAL) const {
			return ImageList_GetIcon(m_image_list_handle, nIndex, uFlags);
		}

		BOOL get_icon_size(int& cx, int& cy) const {
			return ImageList_GetIconSize(m_image_list_handle, &cx, &cy);
		}

		BOOL get_icon_size(SIZE& size) const {
			return ImageList_GetIconSize(m_image_list_handle, (int*)&size.cx, (int*)&size.cy);
		}

		BOOL set_icon_size(int cx, int cy) {
			return ImageList_SetIconSize(m_image_list_handle, cx, cy);
		}

		BOOL set_icon_size(SIZE size) {
			return ImageList_SetIconSize(m_image_list_handle, size.cx, size.cy);
		}

		BOOL set_image_count(UINT uNewCount) {
			return ImageList_SetImageCount(m_image_list_handle, uNewCount);
		}

		BOOL set_overlay_image(int nImage, int nOverlay) {
			return ImageList_SetOverlayImage(m_image_list_handle, nImage, nOverlay);
		}

		// Operations
		BOOL create(int cx, int cy, UINT nFlags, int nInitial, int nGrow) {
			m_image_list_handle = ImageList_Create(cx, cy, nFlags, nInitial, nGrow);
			return (m_image_list_handle != NULL) ? TRUE : FALSE;
		}

		BOOL create(int nBimapID, int cx, int nGrow, COLORREF crMask) {
			m_image_list_handle = ImageList_LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(nBimapID), cx, nGrow, crMask);
			return (m_image_list_handle != NULL) ? TRUE : FALSE;
		}

		BOOL create_from_image(int nImageID, int cx, int nGrow, COLORREF crMask, UINT uType, UINT uFlags = LR_DEFAULTCOLOR | LR_DEFAULTSIZE) {
			m_image_list_handle = ImageList_LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(nImageID), cx, nGrow, crMask, uType, uFlags);
			return (m_image_list_handle != NULL) ? TRUE : FALSE;
		}

		BOOL merge(HIMAGELIST hImageList1, int nImage1, HIMAGELIST hImageList2, int nImage2, int dx, int dy) {
			m_image_list_handle = ImageList_Merge(hImageList1, nImage1, hImageList2, nImage2, dx, dy);
			return (m_image_list_handle != NULL) ? TRUE : FALSE;
		}

#ifndef _WIN32_WCE
#ifdef __IStream_INTERFACE_DEFINED__
		BOOL create_from_stream(LPSTREAM lpStream) {
			m_image_list_handle = ImageList_Read(lpStream);
			return (m_image_list_handle != NULL) ? TRUE : FALSE;
		}
#endif // __IStream_INTERFACE_DEFINED__
#endif // !_WIN32_WCE

		BOOL destroy() {
			if (m_image_list_handle == NULL) return FALSE;
			BOOL bRet = ImageList_Destroy(m_image_list_handle);
			if (bRet) m_image_list_handle = NULL;
			return bRet;
		}

		int add(HBITMAP hBitmap, HBITMAP hBitmapMask = NULL) {
			return ImageList_Add(m_image_list_handle, hBitmap, hBitmapMask);
		}

		int add(HBITMAP hBitmap, COLORREF crMask) {
			return ImageList_AddMasked(m_image_list_handle, hBitmap, crMask);
		}

		BOOL remove(int nImage) {
			return ImageList_Remove(m_image_list_handle, nImage);
		}

		BOOL remove_all() {
			return ImageList_RemoveAll(m_image_list_handle);
		}

		BOOL replace(int nImage, HBITMAP hBitmap, HBITMAP hBitmapMask) {
			return ImageList_Replace(m_image_list_handle, nImage, hBitmap, hBitmapMask);
		}

		int add_icon(HICON hIcon) {
			return ImageList_AddIcon(m_image_list_handle, hIcon);
		}

		int replace_icon(int nImage, HICON hIcon) {
			return ImageList_ReplaceIcon(m_image_list_handle, nImage, hIcon);
		}

		HICON extract_icon(int nImage) {
			return ImageList_ExtractIcon(NULL, m_image_list_handle, nImage);
		}

		BOOL draw(HDC hDC, int nImage, int x, int y, UINT nStyle) {
			return ImageList_Draw(m_image_list_handle, nImage, hDC, x, y, nStyle);
		}

		BOOL draw(HDC hDC, int nImage, POINT pt, UINT nStyle) {
			return ImageList_Draw(m_image_list_handle, nImage, hDC, pt.x, pt.y, nStyle);
		}

		BOOL draw_ex(int nImage, HDC hDC, int x, int y, int dx, int dy, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle) {
			return ImageList_DrawEx(m_image_list_handle, nImage, hDC, x, y, dx, dy, rgbBk, rgbFg, fStyle);
		}

		BOOL draw_ex(int nImage, HDC hDC, RECT& rect, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle) {
			return ImageList_DrawEx(m_image_list_handle, nImage, hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, rgbBk, rgbFg, fStyle);
		}

		static BOOL draw_indirect(IMAGELISTDRAWPARAMS* pimldp) {
			return ImageList_DrawIndirect(pimldp);
		}

		BOOL copy(int nSrc, int nDst, UINT uFlags = ILCF_MOVE) {
			return ImageList_Copy(m_image_list_handle, nDst, m_image_list_handle, nSrc, uFlags);
		}

#ifdef __IStream_INTERFACE_DEFINED__
#ifndef _WIN32_WCE
		static HIMAGELIST read(LPSTREAM lpStream) {
			return ImageList_Read(lpStream);
		}

		BOOL write(LPSTREAM lpStream) {
			return ImageList_Write(m_image_list_handle, lpStream);
		}
#endif // !_WIN32_WCE

#if (_WIN32_WINNT >= 0x0501)
		static HRESULT read_ex(DWORD dwFlags, LPSTREAM lpStream, REFIID riid, PVOID* ppv) {
			return ImageList_ReadEx(dwFlags, lpStream, riid, ppv);
		}

		HRESULT write_ex(DWORD dwFlags, LPSTREAM lpStream) {
			return ImageList_WriteEx(m_image_list_handle, dwFlags, lpStream);
		}
#endif // (_WIN32_WINNT >= 0x0501)
#endif // __IStream_INTERFACE_DEFINED__

		// Drag operations
		BOOL begin_drag(int nImage, POINT ptHotSpot) {
			return ImageList_BeginDrag(m_image_list_handle, nImage, ptHotSpot.x, ptHotSpot.y);
		}

		BOOL begin_drag(int nImage, int xHotSpot, int yHotSpot) {
			return ImageList_BeginDrag(m_image_list_handle, nImage, xHotSpot, yHotSpot);
		}

		void end_drag() {
			ImageList_EndDrag();
		}

		BOOL drag_move(POINT pt) {
			return ImageList_DragMove(pt.x, pt.y);
		}

		BOOL drag_move(int x, int y) {
			return ImageList_DragMove(x, y);
		}

		BOOL set_drag_cursor(int nDrag, POINT ptHotSpot) {
			return ImageList_SetDragCursorImage(m_image_list_handle, nDrag, ptHotSpot.x, ptHotSpot.y);
		}

		BOOL set_drag_cursor(int nDrag, int xHotSpot, int yHotSpot) {
			return ImageList_SetDragCursorImage(m_image_list_handle, nDrag, xHotSpot, yHotSpot);
		}

		BOOL drag_show_nolock(BOOL bShow = TRUE) {
			return ImageList_DragShowNolock(bShow);
		}

		image_list get_drag_image(LPPOINT lpPoint, LPPOINT lpPointHotSpot) {
			return image_list(ImageList_GetDragImage(lpPoint, lpPointHotSpot));
		}

		BOOL drag_enter(HWND hWnd, POINT point) {
			return ImageList_DragEnter(hWnd, point.x, point.y);
		}

		BOOL drag_enter(HWND hWnd, int x, int y) {
			return ImageList_DragEnter(hWnd, x, y);
		}

		BOOL drag_leave(HWND hWnd) {
			return ImageList_DragLeave(hWnd);
		}

#if (_WIN32_IE >= 0x0400)
		image_list duplicate() {
			return image_list(ImageList_Duplicate(m_image_list_handle));
		}

		image_list duplicate(HIMAGELIST hImageList) {
			return image_list(ImageList_Duplicate(hImageList));
		}
#endif // (_WIN32_IE >= 0x0400)
	};
}

#endif //__IMAGE_LIST_HPP__