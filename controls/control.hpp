#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include "..\winplusplus.hpp"

namespace wpp
{
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
		control& on_dbl_click(notify_callback callback) { register_notify_callback(NM_DBLCLK, std::move(callback)); return *this; }
		control& on_right_click(notify_callback callback) { register_notify_callback(NM_RCLICK, std::move(callback)); return *this; }
		control& on_right_dbl_click(notify_callback callback) { register_notify_callback(NM_RDBLCLK, std::move(callback)); return *this; }
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

	template<typename T = control>
	using control_ptr = std::shared_ptr<T>;
}

#endif //__INTERFACES_H__
