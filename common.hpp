#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <richedit.h>
#include <richole.h>
#include <windowsx.h>
#include <atlbase.h>
#include <atlcom.h>
#include <Shellapi.h>
#include <Shobjidl.h>

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED 0x031E
#endif
#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDATA 0x0049
#endif
#ifndef WM_UNICHAR
#define WM_UNICHAR 0x0109
#endif
#ifndef UNICODE_NOCHAR
#define UNICODE_NOCHAR 0xFFFF
#endif

#include <string>
#include <thread>
#include <optional>
#include <functional>
#include <map>
#include <format>
#include <algorithm>

namespace std
{
#ifdef _UNICODE
	using tstring = wstring;
	using tstring_view = wstring_view;

	template <typename Type> std::tstring to_tstring(Type t) {
		return std::to_wstring(t);
	}
#else
	using tstring = string;
	using tstring_view = string_view;

	template <typename Type> std::tstring to_tstring(Type t) {
		return std::to_string(t);
	}
#endif
}

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

		bool operator==(const hwnd& other) const {
			return m_handle == other.m_handle;
		}

		bool operator!=(const hwnd& other) const {
			return m_handle != other.m_handle;
		}

		bool operator==(HWND handle) const {
			return m_handle == handle;
		}

		bool operator!=(HWND handle) const {
			return m_handle != handle;
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
			if (!m_handle) return FALSE;
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

		virtual void set_topmost(BOOL topmost = TRUE) const {
			::SetWindowPos(m_handle, topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}

		virtual bool is_topmost() const {
			return (::GetWindowLong(m_handle, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
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

	namespace com
	{
		template <class base_com>
		class skeleton_base : public base_com {
		public:
			skeleton_base() : m_ref_count(0) {}

			STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
				if (riid == IID_IUnknown) {
					*ppv = dynamic_cast<IUnknown*>(this);
					return S_OK;
				} else if (riid == __uuidof(base_com)) {
					*ppv = dynamic_cast<base_com*>(this);
					return S_OK;
				} else {
					*ppv = NULL;
					return E_NOINTERFACE;
				}
			}

			STDMETHODIMP_(ULONG) AddRef() {
				InterlockedIncrement((LONG*)&m_ref_count);
				return m_ref_count;
			}

			STDMETHODIMP_(ULONG) Release() {
				if (InterlockedDecrement((LONG*)&m_ref_count) == 0)
					delete this;
				return m_ref_count;
			}

		protected:
			LONG m_ref_count;
		};

		class file_dlg_event_handler :
			public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
			public ATL::CComCoClass<file_dlg_event_handler>,
			public IFileDialogEvents {
		public:
			using file_ok_callback = std::function<STDMETHODIMP(IFileDialog*)>;
			using folder_changing_callback = std::function<STDMETHODIMP(IFileDialog*, IShellItem*)>;
			using folder_changed_callback = std::function<STDMETHODIMP(IFileDialog*)>;
			using selection_changed_callback = std::function<STDMETHODIMP(IFileDialog*)>;
			using share_violation_callback = std::function<STDMETHODIMP(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*)>;
			using type_change_callback = std::function<STDMETHODIMP(IFileDialog*)>;
			using overwrite_callback = std::function<STDMETHODIMP(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*)>;

			file_dlg_event_handler() {}
			~file_dlg_event_handler() {};

			BEGIN_COM_MAP(file_dlg_event_handler)
				COM_INTERFACE_ENTRY(IFileDialogEvents)
			END_COM_MAP()

			STDOVERRIDEMETHODIMP OnFileOk(IFileDialog* pfd) { return m_file_ok_callback == nullptr ? E_NOTIMPL : m_file_ok_callback(pfd); }
			STDOVERRIDEMETHODIMP OnFolderChanging(IFileDialog* pfd, IShellItem* psiFolder) { return m_folder_changing_callback == nullptr ? E_NOTIMPL : m_folder_changing_callback(pfd, psiFolder); }
			STDOVERRIDEMETHODIMP OnFolderChange(IFileDialog* pfd) { return m_folder_changed_callback == nullptr ? E_NOTIMPL : m_folder_changed_callback(pfd); }
			STDOVERRIDEMETHODIMP OnSelectionChange(IFileDialog* pfd) { return m_selection_changed_callback == nullptr ? E_NOTIMPL : m_selection_changed_callback(pfd); }
			STDOVERRIDEMETHODIMP OnShareViolation(IFileDialog* pfd, IShellItem* psi, FDE_SHAREVIOLATION_RESPONSE* pResponse) { return m_share_violation_callback == nullptr ? E_NOTIMPL : m_share_violation_callback(pfd, psi, pResponse); }
			STDOVERRIDEMETHODIMP OnTypeChange(IFileDialog* pfd) { return m_type_changed_callback == nullptr ? E_NOTIMPL : m_type_changed_callback(pfd); }
			STDOVERRIDEMETHODIMP OnOverwrite(IFileDialog* pfd, IShellItem* psi, FDE_OVERWRITE_RESPONSE* pResponse) { return m_overwrite_callback == nullptr ? E_NOTIMPL : m_overwrite_callback(pfd, psi, pResponse); }

			file_ok_callback& file_ok() { return m_file_ok_callback; }
			folder_changing_callback& folder_changing() { return m_folder_changing_callback; }
			folder_changed_callback& folder_changed() { return m_folder_changed_callback; }
			selection_changed_callback& selection_changed() { return m_selection_changed_callback; }
			share_violation_callback& share_violation() { return m_share_violation_callback; }
			type_change_callback& type_change() { return m_type_changed_callback; }
			overwrite_callback& overwrite() { return m_overwrite_callback; }

		private:
			file_ok_callback m_file_ok_callback;
			folder_changing_callback m_folder_changing_callback;
			folder_changed_callback m_folder_changed_callback;
			selection_changed_callback m_selection_changed_callback;
			share_violation_callback m_share_violation_callback;
			type_change_callback m_type_changed_callback;
			overwrite_callback m_overwrite_callback;
		};
	}

	using file_dialog_handler = ATL::CComObjectNoLock<wpp::com::file_dlg_event_handler>;
	using file_dialog_handler_stack = ATL::CComObjectStackEx<wpp::com::file_dlg_event_handler>;
}

#endif //__COMMON_HPP__