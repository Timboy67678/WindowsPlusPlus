#ifndef __STATIC_CONTROL_HPP__
#define __STATIC_CONTROL_HPP__

namespace wpp
{
	class static_control : public control {
	public:
		using control::control;

		HICON get_icon() const {
			return (HICON)SendMessage(m_handle, STM_GETIMAGE, IMAGE_ICON, 0L);
		}

		HICON set_icon(HICON hIcon) {
			return (HICON)SendMessage(m_handle, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
		}

		HENHMETAFILE get_enh_meta_file() const {
			return (HENHMETAFILE)SendMessage(m_handle, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L);
		}

		HENHMETAFILE set_enh_meta_file(HENHMETAFILE hMetaFile) {
			return (HENHMETAFILE)SendMessage(m_handle, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
		}

		HBITMAP get_bitmap() const {
			return (HBITMAP)SendMessage(m_handle, STM_GETIMAGE, IMAGE_BITMAP, 0L);
		}

		HBITMAP set_bitmap(HBITMAP hBitmap) {
			return (HBITMAP)SendMessage(m_handle, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		}

		HCURSOR get_cursor() const {
			return (HCURSOR)SendMessage(m_handle, STM_GETIMAGE, IMAGE_CURSOR, 0L);
		}

		HCURSOR set_cursor(HCURSOR hCursor) {
			return (HCURSOR)SendMessage(m_handle, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
		}

		HANDLE get_image(UINT image_type) const {
			return (HANDLE)SendMessage(m_handle, STM_GETIMAGE, image_type, 0L);
		}

		HANDLE set_image(UINT image_type, HANDLE hImage) {
			return (HANDLE)SendMessage(m_handle, STM_SETIMAGE, image_type, (LPARAM)hImage);
		}

		bool load_icon_resource(HINSTANCE hInstance, LPCTSTR lpIconName) {
			HICON hIcon = ::LoadIcon(hInstance, lpIconName);
			if (hIcon) {
				set_icon(hIcon);
				return true;
			}
			return false;
		}

		bool load_icon_resource(HINSTANCE hInstance, int iconId) {
			return load_icon_resource(hInstance, MAKEINTRESOURCE(iconId));
		}

		bool load_bitmap_resource(HINSTANCE hInstance, LPCTSTR lpBitmapName) {
			HBITMAP hBitmap = ::LoadBitmap(hInstance, lpBitmapName);
			if (hBitmap) {
				set_bitmap(hBitmap);
				return true;
			}
			return false;
		}

		bool load_bitmap_resource(HINSTANCE hInstance, int bitmapId) {
			return load_bitmap_resource(hInstance, MAKEINTRESOURCE(bitmapId));
		}

		bool load_cursor_resource(HINSTANCE hInstance, LPCTSTR lpCursorName) {
			HCURSOR hCursor = ::LoadCursor(hInstance, lpCursorName);
			if (hCursor) {
				set_cursor(hCursor);
				return true;
			}
			return false;
		}

		bool load_cursor_resource(HINSTANCE hInstance, int cursorId) {
			return load_cursor_resource(hInstance, MAKEINTRESOURCE(cursorId));
		}

		bool load_image_file(LPCTSTR lpFileName, UINT image_type = IMAGE_BITMAP, int cx = 0, int cy = 0, UINT fuLoad = LR_LOADFROMFILE) {
			HANDLE hImage = ::LoadImage(NULL, lpFileName, image_type, cx, cy, fuLoad);
			if (hImage) {
				set_image(image_type, hImage);
				return true;
			}
			return false;
		}

		bool load_bitmap_file(LPCTSTR lpFileName, int cx = 0, int cy = 0) {
			return load_image_file(lpFileName, IMAGE_BITMAP, cx, cy, LR_LOADFROMFILE);
		}

		bool load_icon_file(LPCTSTR lpFileName, int cx = 0, int cy = 0) {
			return load_image_file(lpFileName, IMAGE_ICON, cx, cy, LR_LOADFROMFILE);
		}

		bool load_cursor_file(LPCTSTR lpFileName, int cx = 0, int cy = 0) {
			return load_image_file(lpFileName, IMAGE_CURSOR, cx, cy, LR_LOADFROMFILE);
		}

		void clear_image() {
			DWORD style = get_style();
			if ((style & SS_TYPEMASK) == SS_ICON) {
				set_icon(NULL);
			} else if ((style & SS_TYPEMASK) == SS_BITMAP) {
				set_bitmap(NULL);
			} else if ((style & SS_TYPEMASK) == SS_ENHMETAFILE) {
				set_enh_meta_file(NULL);
			}
		}

		UINT get_control_type() const {
			return static_cast<UINT>(get_style() & SS_TYPEMASK);
		}

		bool is_text() const {
			UINT type = get_control_type();
			return type == SS_LEFT || type == SS_CENTER || type == SS_RIGHT ||
				type == SS_SIMPLE || type == SS_LEFTNOWORDWRAP;
		}

		bool is_icon() const {
			return get_control_type() == SS_ICON;
		}

		bool is_bitmap() const {
			return get_control_type() == SS_BITMAP;
		}

		bool is_enh_meta_file() const {
			return get_control_type() == SS_ENHMETAFILE;
		}

		bool is_owner_draw() const {
			return get_control_type() == SS_OWNERDRAW;
		}

		bool is_left_align() const {
			return get_control_type() == SS_LEFT;
		}

		bool is_center_align() const {
			return get_control_type() == SS_CENTER;
		}

		bool is_right_align() const {
			return get_control_type() == SS_RIGHT;
		}

		bool has_border() const {
			DWORD style = get_style();
			return (style & WS_BORDER) == WS_BORDER;
		}

		bool has_sunken_border() const {
			return (get_style() & SS_SUNKEN) == SS_SUNKEN;
		}

		bool has_notify() const {
			return (get_style() & SS_NOTIFY) == SS_NOTIFY;
		}

		bool has_center_image() const {
			return (get_style() & SS_CENTERIMAGE) == SS_CENTERIMAGE;
		}

		bool has_real_size_control() const {
			return (get_style() & SS_REALSIZECONTROL) == SS_REALSIZECONTROL;
		}

		bool has_real_size_image() const {
			return (get_style() & SS_REALSIZEIMAGE) == SS_REALSIZEIMAGE;
		}

		bool is_right_just() const {
			return (get_style() & SS_RIGHTJUST) == SS_RIGHTJUST;
		}

		bool is_end_ellipsis() const {
			return (get_style() & SS_ENDELLIPSIS) == SS_ENDELLIPSIS;
		}

		bool is_path_ellipsis() const {
			return (get_style() & SS_PATHELLIPSIS) == SS_PATHELLIPSIS;
		}

		bool is_word_ellipsis() const {
			return (get_style() & SS_WORDELLIPSIS) == SS_WORDELLIPSIS;
		}

		bool has_no_prefix() const {
			return (get_style() & SS_NOPREFIX) == SS_NOPREFIX;
		}

		SIZE get_icon_size() const {
			HICON hIcon = get_icon();
			SIZE sz = { 0, 0 };
			if (hIcon) {
				ICONINFO ii = { 0 };
				if (::GetIconInfo(hIcon, &ii)) {
					BITMAP bm = { 0 };
					if (::GetObject(ii.hbmColor ? ii.hbmColor : ii.hbmMask, sizeof(BITMAP), &bm)) {
						sz.cx = bm.bmWidth;
						sz.cy = ii.hbmColor ? bm.bmHeight : bm.bmHeight / 2;
					}
					if (ii.hbmColor) ::DeleteObject(ii.hbmColor);
					if (ii.hbmMask) ::DeleteObject(ii.hbmMask);
				}
			}
			return sz;
		}

		SIZE get_bitmap_size() const {
			HBITMAP hBitmap = get_bitmap();
			SIZE sz = { 0, 0 };
			if (hBitmap) {
				BITMAP bm = { 0 };
				if (::GetObject(hBitmap, sizeof(BITMAP), &bm)) {
					sz.cx = bm.bmWidth;
					sz.cy = bm.bmHeight;
				}
			}
			return sz;
		}

		SIZE get_image_size() const {
			if (is_icon()) {
				return get_icon_size();
			} else if (is_bitmap()) {
				return get_bitmap_size();
			}
			return { 0, 0 };
		}

		void set_text_align_left() {
			modify_style(SS_TYPEMASK, SS_LEFT);
		}

		void set_text_align_center() {
			modify_style(SS_TYPEMASK, SS_CENTER);
		}

		void set_text_align_right() {
			modify_style(SS_TYPEMASK, SS_RIGHT);
		}

		void enable_notify(bool enable = true) {
			if (enable) {
				add_style(SS_NOTIFY);
			} else {
				remove_style(SS_NOTIFY);
			}
		}

		void enable_sunken_border(bool enable = true) {
			if (enable) {
				add_style(SS_SUNKEN);
			} else {
				remove_style(SS_SUNKEN);
			}
		}

		void enable_center_image(bool enable = true) {
			if (enable) {
				add_style(SS_CENTERIMAGE);
			} else {
				remove_style(SS_CENTERIMAGE);
			}
		}

		void enable_end_ellipsis(bool enable = true) {
			if (enable) {
				add_style(SS_ENDELLIPSIS);
			} else {
				remove_style(SS_ENDELLIPSIS);
			}
		}

		void enable_path_ellipsis(bool enable = true) {
			if (enable) {
				add_style(SS_PATHELLIPSIS);
			} else {
				remove_style(SS_PATHELLIPSIS);
			}
		}

		void enable_no_prefix(bool enable = true) {
			if (enable) {
				add_style(SS_NOPREFIX);
			} else {
				remove_style(SS_NOPREFIX);
			}
		}

		std::tstring get_label_text() const {
			return get_text();
		}

		void set_label_text(LPCTSTR text) {
			set_text(text);
		}

		void set_label_text(const std::tstring& text) {
			set_text(text.c_str());
		}

		bool is_hyperlink() const {
			return get_control_type() == 0x0E;
		}

		void resize_to_image() {
			SIZE sz = get_image_size();
			if (sz.cx > 0 && sz.cy > 0) {
				::SetWindowPos(m_handle, NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}

		void resize_to_fit_text() {
			HDC hdc = ::GetDC(m_handle);
			if (hdc) {
				std::tstring text = get_text();
				HFONT hFont = get_font();
				HFONT hOldFont = hFont ? (HFONT)::SelectObject(hdc, hFont) : NULL;

				RECT rc = { 0 };
				::DrawText(hdc, text.c_str(), -1, &rc, DT_CALCRECT);

				if (hOldFont) ::SelectObject(hdc, hOldFont);
				::ReleaseDC(m_handle, hdc);

				::SetWindowPos(m_handle, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}

		bool has_image() const {
			if (is_icon()) {
				return get_icon() != NULL;
			} else if (is_bitmap()) {
				return get_bitmap() != NULL;
			} else if (is_enh_meta_file()) {
				return get_enh_meta_file() != NULL;
			}
			return false;
		}

		void set_bold(bool bold = true) {
			HFONT hFont = get_font();
			if (hFont) {
				LOGFONT lf = { 0 };
				if (::GetObject(hFont, sizeof(LOGFONT), &lf)) {
					lf.lfWeight = bold ? FW_BOLD : FW_NORMAL;
					HFONT hNewFont = ::CreateFontIndirect(&lf);
					if (hNewFont) {
						set_font(hNewFont);
					}
				}
			}
		}

		void set_italic(bool italic = true) {
			HFONT hFont = get_font();
			if (hFont) {
				LOGFONT lf = { 0 };
				if (::GetObject(hFont, sizeof(LOGFONT), &lf)) {
					lf.lfItalic = italic ? TRUE : FALSE;
					HFONT hNewFont = ::CreateFontIndirect(&lf);
					if (hNewFont) {
						set_font(hNewFont);
					}
				}
			}
		}

		void set_underline(bool underline = true) {
			HFONT hFont = get_font();
			if (hFont) {
				LOGFONT lf = { 0 };
				if (::GetObject(hFont, sizeof(LOGFONT), &lf)) {
					lf.lfUnderline = underline ? TRUE : FALSE;
					HFONT hNewFont = ::CreateFontIndirect(&lf);
					if (hNewFont) {
						set_font(hNewFont);
					}
				}
			}
		}

		void set_font_size(int point_size) {
			HFONT hFont = get_font();
			if (hFont) {
				LOGFONT lf = { 0 };
				if (::GetObject(hFont, sizeof(LOGFONT), &lf)) {
					HDC hdc = ::GetDC(m_handle);
					lf.lfHeight = -::MulDiv(point_size, ::GetDeviceCaps(hdc, LOGPIXELSY), 72);
					::ReleaseDC(m_handle, hdc);

					HFONT hNewFont = ::CreateFontIndirect(&lf);
					if (hNewFont) {
						set_font(hNewFont);
					}
				}
			}
		}
	};

	using label = static_control;
}

#endif
