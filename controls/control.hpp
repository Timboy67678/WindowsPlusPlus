#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "..\common.hpp"

namespace wpp
{
	class hwnd {
	public:
		typedef LRESULT(CALLBACK hwnd::* COMMAND_ID_MESSAGE_CALLBACK)(HWND, WPARAM, LPARAM);

		hwnd(int resource_id, HWND parent = NULL)
			: m_item_id(resource_id), m_parent_handle(parent), m_handle(NULL) {
		}

		hwnd(HWND handle)
			: m_handle(handle), m_item_id(-1), m_parent_handle(NULL) {
		}

		virtual HWND get_handle() const { return m_handle; }
		virtual void set_handle(HWND handle) { m_handle = handle; }
		virtual HWND get_parent() const { return m_parent_handle; }
		virtual HWND set_parent(HWND parent) {
			HWND oldParent = ::SetParent(m_handle, parent);
			m_parent_handle = parent;

			if (parent == NULL) {
				modify_style(WS_CHILD, WS_POPUP);
			} else {
				modify_style(WS_POPUP, WS_CHILD);
			}

			return oldParent;
		}

		virtual int get_id() const { return m_item_id; }

		virtual BOOL destroy() const {
			return ::DestroyWindow(m_handle);
		}

		virtual void enable_drag_drop(BOOL state) {
			::ChangeWindowMessageFilterEx(m_handle, WM_DROPFILES, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
			::ChangeWindowMessageFilterEx(m_handle, WM_COPYDATA, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
			::ChangeWindowMessageFilterEx(m_handle, WM_COPYGLOBALDATA, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
			::DragAcceptFiles(m_handle, state);
		}

		virtual std::tstring get_text() const {
			if (!m_handle) return std::tstring();
			int length = ::GetWindowTextLength(m_handle);
			if (length == 0) return std::tstring();

			std::vector<TCHAR> buffer(length + 1);
			::GetWindowText(m_handle, buffer.data(), length + 1);
			return std::tstring(buffer.data());
		}

		virtual int get_text_length() const {
			return ::GetWindowTextLength(m_handle);
		}

		virtual RECT get_rect() const {
			RECT rc = { 0 };
			::GetWindowRect(m_handle, &rc);
			return rc;
		}

		// Get client area rect (in client coordinates)
		virtual RECT get_client_rect() const {
			RECT rc = { 0 };
			::GetClientRect(m_handle, &rc);
			return rc;
		}

		virtual BOOL set_text(const std::tstring& text) {
			if (!m_handle) return FALSE;  // Safety check
			return ::SetWindowText(m_handle, text.c_str());
		}

		virtual BOOL set_showing(int state = SW_NORMAL) {
			return ::ShowWindow(m_handle, state);
		}

		virtual BOOL is_enabled() {
			return ::IsWindowEnabled(m_handle);
		}

		virtual BOOL set_enabled(BOOL enabled = TRUE) {
			return ::EnableWindow(m_handle, enabled);
		}

		virtual HWND focus() {
			return ::SetFocus(m_handle);
		}

		virtual BOOL is_focused() const {
			return ::GetFocus() == m_handle;
		}

		virtual HFONT get_font() const {
			return (HFONT)::SendMessage(m_handle, WM_GETFONT, 0, 0);
		}

		virtual void set_font(HFONT hFont, BOOL redraw = TRUE) {
			::SendMessage(m_handle, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(redraw, 0));
		}

		virtual DWORD get_style() const {
			return (DWORD)::GetWindowLong(m_handle, GWL_STYLE);
		}

		virtual DWORD add_style(DWORD dwStyle) {
			DWORD new_style = ::GetWindowLong(m_handle, GWL_STYLE) | dwStyle;
			return ::SetWindowLong(m_handle, GWL_STYLE, new_style);
		}

		virtual DWORD set_style(DWORD dwStyle) {
			return ::SetWindowLong(m_handle, GWL_STYLE, dwStyle);
		}

		virtual DWORD remove_style(DWORD dwStyle) {
			DWORD new_style = ::GetWindowLong(m_handle, GWL_STYLE) & ~dwStyle;
			return ::SetWindowLong(m_handle, GWL_STYLE, new_style);
		}

		//Stolen From ATL
		virtual BOOL modify_style(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) {
			DWORD dwStyle = ::GetWindowLong(m_handle, GWL_STYLE);
			DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
			if (dwStyle == dwNewStyle) return FALSE;
			::SetWindowLong(m_handle, GWL_STYLE, dwNewStyle);
			if (nFlags != 0)
				::SetWindowPos(m_handle, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
			return TRUE;
		}

		virtual void set_tab_stop(BOOL tab_stop = TRUE) {
			if (tab_stop)
				modify_style(0, WS_TABSTOP);
			else
				modify_style(WS_TABSTOP, 0);
		}

		virtual BOOL center_window(HWND hWndCenter = HWND_DESKTOP) {
			// Determine owner window to center against
			DWORD dwStyle = get_style();
			if (hWndCenter == HWND_DESKTOP) {
				hWndCenter = (dwStyle & WS_CHILD) ? ::GetParent(m_handle) : ::GetWindow(m_handle, GW_OWNER);
			}

			// Get window dimensions
			RECT rcWindow;
			if (!::GetWindowRect(m_handle, &rcWindow)) {
				return FALSE;
			}

			const int nWidth = rcWindow.right - rcWindow.left;
			const int nHeight = rcWindow.bottom - rcWindow.top;

			// Get centering area and target rectangle
			RECT rcArea, rcCenter;
			if (dwStyle & WS_CHILD) {
				// Center within parent client area
				HWND hWndParent = ::GetParent(m_handle);
				if (!hWndParent) {
					return FALSE;
				}

				::GetClientRect(hWndParent, &rcArea);
				::GetClientRect(hWndCenter, &rcCenter);
				::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
			} else {
				// Don't center against invisible or minimized windows
				if (hWndCenter != NULL) {
					DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
					if (!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE)) {
						hWndCenter = NULL;
					}
				}

				// Center within screen/monitor work area
				HMONITOR hMonitor = ::MonitorFromWindow(
					hWndCenter ? hWndCenter : m_handle,
					MONITOR_DEFAULTTONEAREST
				);

				MONITORINFO minfo = { sizeof(MONITORINFO) };
				if (!::GetMonitorInfo(hMonitor, &minfo)) {
					return FALSE;
				}

				rcArea = minfo.rcWork;
				rcCenter = hWndCenter ? ([&]() {
					RECT rc;
					::GetWindowRect(hWndCenter, &rc);
					return rc;
				})() : rcArea;
			}

			// Calculate centered position
			int xLeft = (rcCenter.left + rcCenter.right - nWidth) / 2;
			int yTop = (rcCenter.top + rcCenter.bottom - nHeight) / 2;

			// Ensure window stays within bounds
			xLeft = max(rcArea.left, min(xLeft, rcArea.right - nWidth));
			yTop = max(rcArea.top, min(yTop, rcArea.bottom - nHeight));

			// Position the window
			return ::SetWindowPos(m_handle, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}

		virtual BOOL is_visible() const {
			return ::IsWindowVisible(m_handle);
		}

		virtual BOOL move(int x, int y, int width, int height, BOOL repaint = TRUE) {
			return ::MoveWindow(m_handle, x, y, width, height, repaint);
		}

		virtual BOOL set_position(int x, int y, UINT flags = SWP_NOSIZE | SWP_NOZORDER) {
			return ::SetWindowPos(m_handle, NULL, x, y, 0, 0, flags);
		}

		virtual BOOL set_size(int width, int height, UINT flags = SWP_NOMOVE | SWP_NOZORDER) {
			return ::SetWindowPos(m_handle, NULL, 0, 0, width, height, flags);
		}

		virtual DWORD get_ex_style() const {
			return (DWORD)::GetWindowLong(m_handle, GWL_EXSTYLE);
		}

		virtual DWORD add_ex_style(DWORD dwExStyle) {
			DWORD style = ::GetWindowLong(m_handle, GWL_EXSTYLE) | dwExStyle;
			return ::SetWindowLong(m_handle, GWL_EXSTYLE, style);
		}

		virtual DWORD remove_ex_style(DWORD dwExStyle) {
			DWORD style = ::GetWindowLong(m_handle, GWL_EXSTYLE) & ~dwExStyle;
			return ::SetWindowLong(m_handle, GWL_EXSTYLE, style);
		}

	protected:
		int m_item_id;
		HWND m_handle, m_parent_handle;
	};

	class control : public hwnd {
	public:
		using command_callback = std::function<void(WPARAM, LPARAM)>;
		using notify_callback = std::function<void(LPNMHDR)>;

		control(int resource_id, HWND parent = nullptr)
			: hwnd(resource_id, parent) {
			if (parent != nullptr) {
				m_handle = ::GetDlgItem(m_parent_handle, resource_id);
			}
		}

		control(HWND handle) : hwnd(handle) {
		}

		control& on_click(notify_callback callback) { register_notify_callback(NM_CLICK, std::move(callback)); return *this; }
		control& on_double_click(notify_callback callback) { register_notify_callback(NM_DBLCLK, std::move(callback)); return *this; }
		control& on_right_click(notify_callback callback) { register_notify_callback(NM_RCLICK, std::move(callback)); return *this; }
		control& on_right_double_click(notify_callback callback) { register_notify_callback(NM_RDBLCLK, std::move(callback)); return *this; }
		control& on_return(notify_callback callback) { register_notify_callback(NM_RETURN, std::move(callback)); return *this; }
		control& on_set_focus(notify_callback callback) { register_notify_callback(NM_SETFOCUS, std::move(callback)); return *this; }
		control& on_kill_focus(notify_callback callback) { register_notify_callback(NM_KILLFOCUS, std::move(callback)); return *this; }
		control& on_hover(notify_callback callback) { register_notify_callback(NM_HOVER, std::move(callback)); return *this; }
		control& on_custom_draw(notify_callback callback) { register_notify_callback(NM_CUSTOMDRAW, std::move(callback)); return *this; }
		control& on_released_capture(notify_callback callback) { register_notify_callback(NM_RELEASEDCAPTURE, std::move(callback)); return *this; }

		void set_item(HWND parent, int item_id) {
			m_item_id = item_id;
			m_parent_handle = parent;
			m_handle = ::GetDlgItem(parent, item_id);
		}

		void register_command_callback(UINT command, command_callback callback) {
			if (callback == nullptr) return;
			m_command_callbacks[command].push_back(std::move(callback));
		}

		void remove_all_command_callbacks(UINT command) {
			m_command_callbacks.erase(command);
		}

		void clear_command_callbacks() {
			m_command_callbacks.clear();
		}

		void register_notify_callback(UINT notifyCode, notify_callback callback) {
			if (callback == nullptr) return;
			m_notify_callbacks[notifyCode].push_back(std::move(callback));
		}

		void remove_all_notify_callbacks(UINT notifyCode) {
			m_notify_callbacks.erase(notifyCode);
		}

		void clear_notify_callbacks() {
			m_notify_callbacks.clear();
		}

		void on_notify_callback(LPNMHDR nm) {
			if (nm == nullptr)
				return;

			auto it = m_notify_callbacks.find(nm->code);
			if (it != m_notify_callbacks.end()) {
				for (auto& callback : it->second) {
					if (callback)
						callback(nm);
				}
			}
		}

		void on_command_callback(WPARAM wParam, LPARAM lParam) {
			UINT command = HIWORD(wParam);
			auto it = m_command_callbacks.find(command);
			if (it != m_command_callbacks.end()) {
				for (auto& callback : it->second) {
					if (callback)
						callback(wParam, lParam);
				}
			}
		}

		void enable_children(BOOL enable = TRUE) const {
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				::EnableWindow(hChild, enable);
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
		}

		void disable_children() const {
			enable_children(FALSE);
		}

		void show_children(BOOL show = TRUE) const {
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				::ShowWindow(hChild, show ? SW_SHOW : SW_HIDE);
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
		}

		void hide_children() const {
			show_children(FALSE);
		}

		int get_child_count() const {
			int count = 0;
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				count++;
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
			return count;
		}

		BOOL has_children() const {
			return ::GetWindow(m_handle, GW_CHILD) != NULL;
		}

		HWND add_child(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
					   int x, int y, int nWidth, int nHeight, int nID,
					   DWORD dwExStyle = 0) const {
			HWND hChild = ::CreateWindowEx(
				dwExStyle,
				lpClassName,
				lpWindowName,
				dwStyle | WS_CHILD | WS_VISIBLE,
				x, y, nWidth, nHeight,
				m_handle,
				(HMENU)(INT_PTR)nID,
				::GetModuleHandle(NULL),
				NULL
			);
			if (hChild && get_font()) {  // Inherit parent font
				::SendMessage(hChild, WM_SETFONT, (WPARAM)get_font(), TRUE);
			}
			return hChild;
		}

		BOOL remove_child(const control& child) const {
			return ::DestroyWindow(child.get_handle());
		}

		BOOL remove_child(int child_id) const {
			HWND hChild = ::GetDlgItem(m_handle, child_id);
			if (hChild) {
				return ::DestroyWindow(hChild);
			}
			return FALSE;
		}

		HWND get_child(int child_id) const {
			return ::GetDlgItem(m_handle, child_id);
		}

		HWND attach_child(HWND hChild) const {
			if (hChild) {
				HWND oldParent = ::SetParent(hChild, m_handle);

				// Ensure child has WS_CHILD style
				LONG_PTR style = ::GetWindowLongPtr(hChild, GWL_STYLE);
				if (!(style & WS_CHILD)) {
					style = (style & ~WS_POPUP) | WS_CHILD;
					::SetWindowLongPtr(hChild, GWL_STYLE, style);
				}

				return oldParent;
			}
			return NULL;
		}

		HWND attach_child(const control& child) const {
			return attach_child(child.get_handle());
		}

		HWND detach_child(HWND hChild, HWND hNewParent = NULL) const {
			if (hChild) {
				HWND oldParent = ::SetParent(hChild, hNewParent);

				// If becoming top-level, adjust styles
				if (hNewParent == NULL) {
					LONG_PTR style = ::GetWindowLongPtr(hChild, GWL_STYLE);
					style = (style & ~WS_CHILD) | WS_POPUP;
					::SetWindowLongPtr(hChild, GWL_STYLE, style);
				}

				return oldParent;
			}
			return NULL;
		}

		HWND detach_child(const control& child, HWND hNewParent = NULL) const {
			return detach_child(child.get_handle(), hNewParent);
		}

		std::vector<control> get_children() const {
			std::vector<control> children;
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				children.push_back(control{ hChild });
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
			return children;
		}

		// Enumerate children with callback (avoids copying)
		void for_each_child(std::function<bool(control)> callback) const {
			if (callback == nullptr) return;
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				if (!callback(control{ hChild })) break;
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
		}

		// Find child by predicate
		std::optional<control> find_child(std::function<bool(control)> pred) const {
			if (pred == nullptr) return std::nullopt;
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				control child{ hChild };
				if (pred(child)) return child;
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
			return std::nullopt;
		}

	protected:
		std::unordered_map<UINT, std::vector<command_callback>> m_command_callbacks;
		std::unordered_map<UINT, std::vector<notify_callback>> m_notify_callbacks;
	};
}

#endif //__INTERFACES_H__
