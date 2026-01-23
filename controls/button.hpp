#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

namespace wpp
{
	class button : public control {
	public:
		using control::control;

		button& on_click(command_callback cb) { register_command_callback(BN_CLICKED, cb); return *this; }
		button& on_dbl_click(command_callback cb) { register_command_callback(BN_DBLCLK, cb); return *this; }
		button& on_mouseover(notify_callback cb) {register_notify_callback(BCN_HOTITEMCHANGE, cb); return *this; }

		int get_checked() const {
			return Button_GetCheck(m_handle);
		}

		void set_checked(int check) const {
			Button_SetCheck(m_handle, check);
		}

		int get_state() const {
			return Button_GetState(m_handle);
		}

		void set_state(int state) const {
			Button_SetState(m_handle, state);
		}

		void set_style(UINT style, BOOL redraw = TRUE) const {
			Button_SetStyle(m_handle, style, redraw);
		}

		HICON get_icon() const {
			return (HICON)SendMessage(m_handle, BM_GETIMAGE, IMAGE_ICON, 0L);
		}

		HICON set_icon(HICON hIcon) const {
			return (HICON)SendMessage(m_handle, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
		}

		HBITMAP get_bitmap() const {
			return (HBITMAP)SendMessage(m_handle, BM_GETIMAGE, IMAGE_BITMAP, 0L);
		}

		HBITMAP set_bitmap(HBITMAP hBitmap) const {
			return (HBITMAP)SendMessage(m_handle, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		}

		void set_shield(BOOL shield_state) const {
			Button_SetElevationRequiredState(m_handle, shield_state);
		}

		void emulate_click() const {
			HWND current_focus = ::GetForegroundWindow();
			::SetActiveWindow(get_parent());
			SendMessage(m_handle, BM_CLICK, 0, 0L);
			::SetActiveWindow(current_focus);
		}

		void click() const {
			emulate_click();
		}

		BOOL is_checked() const {
			return Button_GetCheck(m_handle) == BST_CHECKED;
		}

		BOOL is_unchecked() const {
			return Button_GetCheck(m_handle) == BST_UNCHECKED;
		}

		BOOL is_indeterminate() const {
			return Button_GetCheck(m_handle) == BST_INDETERMINATE;
		}

		void check() const {
			set_checked(BST_CHECKED);
		}

		void uncheck() const {
			set_checked(BST_UNCHECKED);
		}

		void set_indeterminate() const {
			set_checked(BST_INDETERMINATE);
		}

		void toggle() const {
			int current = get_checked();
			if (current == BST_CHECKED) {
				uncheck();
			} else {
				check();
			}
		}

		BOOL is_pushed() const {
			return (get_state() & BST_PUSHED) == BST_PUSHED;
		}

		BOOL is_focus() const {
			return (get_state() & BST_FOCUS) == BST_FOCUS;
		}

		BOOL is_hot() const {
			return (get_state() & BST_HOT) == BST_HOT;
		}

		UINT get_button_style() const {
			return (UINT)(get_style() & BS_TYPEMASK);
		}

		BOOL is_pushbutton() const {
			UINT style = get_button_style();
			return style == BS_PUSHBUTTON || style == BS_DEFPUSHBUTTON;
		}

		BOOL is_checkbox_type() const {
			UINT style = get_button_style();
			return style == BS_CHECKBOX || style == BS_AUTOCHECKBOX || 
			       style == BS_3STATE || style == BS_AUTO3STATE;
		}

		BOOL is_radiobutton_type() const {
			UINT style = get_button_style();
			return style == BS_RADIOBUTTON || style == BS_AUTORADIOBUTTON;
		}

		BOOL is_groupbox() const {
			return get_button_style() == BS_GROUPBOX;
		}

		BOOL is_default_button() const {
			return get_button_style() == BS_DEFPUSHBUTTON;
		}

		void set_as_default() {
			UINT currentStyle = get_button_style();
			if (currentStyle == BS_PUSHBUTTON) {
				set_style((get_style() & ~BS_TYPEMASK) | BS_DEFPUSHBUTTON, TRUE);
			}
		}

		void remove_default() {
			UINT currentStyle = get_button_style();
			if (currentStyle == BS_DEFPUSHBUTTON) {
				set_style((get_style() & ~BS_TYPEMASK) | BS_PUSHBUTTON, TRUE);
			}
		}

		BOOL has_image() const {
			return get_icon() != NULL || get_bitmap() != NULL;
		}

		void remove_icon() const {
			set_icon(NULL);
		}

		void remove_bitmap() const {
			set_bitmap(NULL);
		}

		void remove_image() const {
			remove_icon();
			remove_bitmap();
		}

		BOOL set_text_and_icon(LPCTSTR lpszText, HICON hIcon) {
			set_text(lpszText);
			set_icon(hIcon);
			return TRUE;
		}

		BOOL set_text_and_bitmap(LPCTSTR lpszText, HBITMAP hBitmap) {
			set_text(lpszText);
			set_bitmap(hBitmap);
			return TRUE;
		}

		SIZE get_ideal_size() const {
			SIZE size = { 0 };
			SendMessage(m_handle, BCM_GETIDEALSIZE, 0, (LPARAM)&size);
			return size;
		}

		BOOL set_note(LPCTSTR lpszNote) {
			return (BOOL)SendMessage(m_handle, BCM_SETNOTE, 0, (LPARAM)lpszNote);
		}

		DWORD get_note(LPTSTR lpszNote, DWORD cchNote) const {
			return (DWORD)SendMessage(m_handle, BCM_GETNOTE, cchNote, (LPARAM)lpszNote);
		}

		DWORD get_note_length() const {
			return (DWORD)SendMessage(m_handle, BCM_GETNOTELENGTH, 0, 0L);
		}

		BOOL set_split_info(PBUTTON_SPLITINFO pInfo) {
			return (BOOL)SendMessage(m_handle, BCM_SETSPLITINFO, 0, (LPARAM)pInfo);
		}

		BOOL get_split_info(PBUTTON_SPLITINFO pInfo) const {
			return (BOOL)SendMessage(m_handle, BCM_GETSPLITINFO, 0, (LPARAM)pInfo);
		}

		BOOL set_drop_down_state(BOOL bDropDown) {
			return (BOOL)SendMessage(m_handle, BCM_SETDROPDOWNSTATE, bDropDown, 0L);
		}

		BOOL set_text_margin(RECT* pMargin) {
			return (BOOL)SendMessage(m_handle, BCM_SETTEXTMARGIN, 0, (LPARAM)pMargin);
		}

		BOOL get_text_margin(RECT* pMargin) const {
			return (BOOL)SendMessage(m_handle, BCM_GETTEXTMARGIN, 0, (LPARAM)pMargin);
		}

		void set_flat_style() {
			add_style(BS_FLAT);
		}

		void remove_flat_style() {
			remove_style(BS_FLAT);
		}

		BOOL is_flat_style() const {
			return (get_style() & BS_FLAT) == BS_FLAT;
		}
	};

	class check_box : public button {
	public:
		using button::button;

		void check() {
			set_checked(BST_CHECKED);
		}

		void uncheck() {
			set_checked(BST_UNCHECKED);
		}

		void toggle() {
			button::toggle();
		}

		BOOL is_checked() const {
			return button::is_checked();
		}

		BOOL is_unchecked() const {
			return button::is_unchecked();
		}

		BOOL is_3state() const {
			UINT style = get_button_style();
			return style == BS_3STATE || style == BS_AUTO3STATE;
		}

		BOOL is_auto() const {
			UINT style = get_button_style();
			return style == BS_AUTOCHECKBOX || style == BS_AUTO3STATE;
		}
	};

	class radio_button : public button {
	public:
		using button::button;

		void select() {
			check();
		}

		BOOL is_selected() const {
			return is_checked();
		}

		BOOL is_auto() const {
			return get_button_style() == BS_AUTORADIOBUTTON;
		}

		void select_in_group() {
			HWND hParent = get_parent();
			if (!hParent) return;

			HWND hCurrent = ::GetWindow(hParent, GW_CHILD);
			while (hCurrent) {
				if (hCurrent != m_handle) {
					TCHAR className[256] = { 0 };
					::GetClassName(hCurrent, className, 256);
					if (lstrcmpi(className, TEXT("Button")) == 0) {
						LONG_PTR style = ::GetWindowLongPtr(hCurrent, GWL_STYLE);
						UINT btnStyle = (UINT)(style & BS_TYPEMASK);
						if (btnStyle == BS_RADIOBUTTON || btnStyle == BS_AUTORADIOBUTTON) {
							::SendMessage(hCurrent, BM_SETCHECK, BST_UNCHECKED, 0L);
						}
					}
				}
				hCurrent = ::GetWindow(hCurrent, GW_HWNDNEXT);
			}
			check();
		}
	};

	class group_box : public button {
	public:
		using button::button;
	};
}

#endif //__BUTTON_HPP__