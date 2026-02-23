#ifndef WPP_COMMON_HPP
#define WPP_COMMON_HPP

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
#include <unordered_map>
#include <map>
#include <format>
#include <algorithm>
#include <mutex>

namespace wpp
{
	// Convert between char and wchar_t strings

	inline std::wstring convert_to_wstring(const std::string& str) {
		if (str.empty()) return std::wstring();
		if (str.size() > static_cast<size_t>((std::numeric_limits<int>::max)()))
			return std::wstring();

		int size_needed = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
		if (size_needed <= 0)
			return std::wstring();

		std::wstring result(static_cast<size_t>(size_needed), L'\0');
		int converted = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>(str.size()), result.data(), size_needed);
		if (converted <= 0)
			return std::wstring();

		return result;
	}

	inline std::string convert_to_string(const std::wstring& wstr) {
		if (wstr.empty()) return std::string();
		if (wstr.size() > static_cast<size_t>((std::numeric_limits<int>::max)()))
			return std::string();

		int size_needed = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
		if (size_needed <= 0)
			return std::string();

		std::string result(static_cast<size_t>(size_needed), '\0');
		int converted = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), result.data(), size_needed, nullptr, nullptr);
		if (converted <= 0)
			return std::string();

		return result;
	}

#ifdef _UNICODE
	using tstring = std::wstring;
	using tstring_view = std::wstring_view;

	template <typename Type> tstring to_tstring(Type t) {
		return std::to_wstring(t);
	}

	inline bool tstrcmpi(const tstring& str1, const tstring& str2) {
		if (str1.length() != str2.length()) {
			return false;
		}
		return std::equal(str1.begin(), str1.end(), str2.begin(),
						  [](wchar_t a, wchar_t b) {
			return std::tolower(a) == std::tolower(b);
		});
	}

	inline tstring to_tstring(const std::string& str) { return convert_to_wstring(str); }
	inline tstring to_tstring(const std::wstring& str) { return str; }
#else
	using tstring = std::string;
	using tstring_view = std::string_view;

	template <typename Type> tstring to_tstring(Type t) {
		return std::to_string(t);
	}

	inline bool tstrcmpi(const tstring& str1, const tstring& str2) {
		if (str1.length() != str2.length()) {
			return false;
		}
		return std::equal(str1.begin(), str1.end(), str2.begin(),
						  [](unsigned char a, unsigned char b) {
			return std::tolower(a) == std::tolower(b);
		});
	}

	inline tstring to_tstring(const std::string& str) { return str; }
	inline tstring to_tstring(const std::wstring& wstr) { return convert_to_string(wstr); }
#endif

	inline namespace literals
	{
		inline tstring operator""_ts(const char* str, size_t len) {
#ifdef _UNICODE
			return convert_to_wstring(std::string(str, len));
#else
			return std::string(str, len);
#endif
		}

		inline tstring operator""_ts(const wchar_t* str, size_t len) {
#ifdef _UNICODE
			return std::wstring(str, len);
#else
			return convert_to_string(std::wstring(str, len));
#endif
		}
	}

	template<typename... Args>
	tstring format_tstring(tstring_view fmt, Args&&... args) {
#ifdef _UNICODE
		return std::vformat(fmt, std::make_wformat_args(std::forward<Args>(args)...));
#else
		return std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
#endif
	}

	/// <summary>
	/// A RAII wrapper class for Windows timers that automatically manages timer creation and destruction.
	/// </summary>
	class timer {
	public:
		using callback = std::function<void()>;

		timer(HWND hwnd, UINT_PTR id, UINT interval, callback fn)
			: m_hwnd(hwnd), m_id(id), m_callback(std::move(fn)) {
			::SetTimer(m_hwnd, m_id, interval, nullptr);
		}

		~timer() {
			if (m_hwnd) ::KillTimer(m_hwnd, m_id);
		}

		timer(const timer&) = delete;
		timer& operator=(const timer&) = delete;

		timer(timer&& other) noexcept
			: m_hwnd(std::exchange(other.m_hwnd, nullptr))
			, m_id(std::exchange(other.m_id, 0))
			, m_callback(std::move(other.m_callback)) {
		}

		void invoke() { if (m_callback) m_callback(); }
		UINT_PTR get_id() const { return m_id; }

	private:
		HWND m_hwnd;
		UINT_PTR m_id;
		callback m_callback;
	};

	/// <summary>
	/// RAII wrapper for DeferWindowPos that automatically handles BeginDeferWindowPos and EndDeferWindowPos.
	/// Provides exception-safe and convenient batching of multiple window position changes for better performance.
	/// </summary>
	class deferred_window_pos {
	public:
		/// <summary>
		/// Begins a deferred window position update for the specified number of windows.
		/// </summary>
		/// <param name="count">The number of windows to be repositioned.</param>
		explicit deferred_window_pos(int count)
			: m_hdwp(count > 0 ? ::BeginDeferWindowPos(count) : nullptr) {
		}

		/// <summary>
		/// Automatically ends the deferred window position update.
		/// </summary>
		~deferred_window_pos() {
			end();
		}

		// Non-copyable
		deferred_window_pos(const deferred_window_pos&) = delete;
		deferred_window_pos& operator=(const deferred_window_pos&) = delete;

		// Movable
		deferred_window_pos(deferred_window_pos&& other) noexcept
			: m_hdwp(std::exchange(other.m_hdwp, nullptr)) {
		}

		deferred_window_pos& operator=(deferred_window_pos&& other) noexcept {
			if (this != &other) {
				end();
				m_hdwp = std::exchange(other.m_hdwp, nullptr);
			}
			return *this;
		}

		/// <summary>
		/// Defers the positioning of a window.
		/// </summary>
		/// <param name="hwnd">Handle to the window to be repositioned.</param>
		/// <param name="hwnd_insert_after">Handle to the window to precede hwnd in the Z order.</param>
		/// <param name="x">X-coordinate of the window.</param>
		/// <param name="y">Y-coordinate of the window.</param>
		/// <param name="cx">Width of the window.</param>
		/// <param name="cy">Height of the window.</param>
		/// <param name="flags">Window positioning flags (e.g., SWP_NOACTIVATE | SWP_NOZORDER).</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool defer(HWND hwnd, HWND hwnd_insert_after, int x, int y, int cx, int cy, UINT flags) {
			if (!m_hdwp || !hwnd) return false;

			HDWP new_hdwp = ::DeferWindowPos(m_hdwp, hwnd, hwnd_insert_after, x, y, cx, cy, flags);
			if (new_hdwp) {
				m_hdwp = new_hdwp;
				return true;
			}
			return false;
		}

		/// <summary>
		/// Defers the positioning of a window (convenience overload without Z-order control).
		/// </summary>
		/// <param name="hwnd">Handle to the window to be repositioned.</param>
		/// <param name="x">X-coordinate of the window.</param>
		/// <param name="y">Y-coordinate of the window.</param>
		/// <param name="cx">Width of the window.</param>
		/// <param name="cy">Height of the window.</param>
		/// <param name="flags">Window positioning flags (default: SWP_NOACTIVATE | SWP_NOZORDER).</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool defer(HWND hwnd, int x, int y, int cx, int cy, UINT flags = SWP_NOACTIVATE | SWP_NOZORDER) {
			return defer(hwnd, nullptr, x, y, cx, cy, flags);
		}

		/// <summary>
		/// Checks if the deferred window position handle is valid.
		/// </summary>
		/// <returns>True if valid, false otherwise.</returns>
		bool is_valid() const {
			return m_hdwp != nullptr;
		}

		/// <summary>
		/// Explicit boolean conversion operator.
		/// </summary>
		explicit operator bool() const {
			return is_valid();
		}

		/// <summary>
		/// Manually ends the deferred window position update (called automatically by destructor).
		/// </summary>
		/// <returns>True if successful, false otherwise.</returns>
		bool end() {
			if (m_hdwp) {
				BOOL result = ::EndDeferWindowPos(m_hdwp);
				m_hdwp = nullptr;
				return result != FALSE;
			}
			return false;
		}

		/// <summary>
		/// Gets the raw HDWP handle (use with caution).
		/// </summary>
		HDWP get_handle() const {
			return m_hdwp;
		}

	private:
		HDWP m_hdwp;
	};

	/// <summary>
	/// A wrapper class for Windows HWND (window handle) that provides an object-oriented interface for window management and manipulation.
	/// </summary>
	class hwnd {
	public:
		typedef LRESULT(CALLBACK hwnd::* COMMAND_ID_MESSAGE_CALLBACK)(HWND, WPARAM, LPARAM);

		hwnd(int resource_id, HWND parent = NULL)
			: m_item_id(resource_id), m_parent_handle(parent), m_handle(NULL) {
		}

		hwnd(HWND handle)
			: m_handle(handle), m_item_id(-1), m_parent_handle(NULL) {
		}

		virtual ~hwnd() noexcept = default;

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

		bool is_valid() const { return m_handle != NULL && ::IsWindow(m_handle); }

		explicit operator bool() const { return is_valid(); }

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

		template<typename T = LRESULT>
		T send_message(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0) const {
			LRESULT result = ::SendMessage(m_handle, Msg, wParam, lParam);
			if constexpr (std::is_pointer_v<T>) {
				return reinterpret_cast<T>(result);
			} else {
				return static_cast<T>(result);
			}
		}

		virtual void enable_drag_drop(BOOL state) {
			::ChangeWindowMessageFilterEx(m_handle, WM_DROPFILES, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
			::ChangeWindowMessageFilterEx(m_handle, WM_COPYDATA, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
			::ChangeWindowMessageFilterEx(m_handle, WM_COPYGLOBALDATA, state ? MSGFLT_ADD : MSGFLT_REMOVE, NULL);
			::DragAcceptFiles(m_handle, state);
		}

		virtual tstring get_text() const {
			if (!m_handle) return tstring();
			int length = ::GetWindowTextLength(m_handle);
			if (length == 0) return tstring();

			std::vector<TCHAR> buffer(length + 1);
			::GetWindowText(m_handle, buffer.data(), length + 1);
			return tstring(buffer.data());
		}

		virtual tstring get_class_name() const {
			if (!m_handle) return tstring();
			TCHAR buffer[1024];
			::GetClassName(m_handle, buffer, 1024);
			return tstring(buffer);
		}

		virtual int get_text_length() const {
			return ::GetWindowTextLength(m_handle);
		}

		virtual RECT get_rect() const {
			RECT rc = { 0 };
			::GetWindowRect(m_handle, &rc);
			return rc;
		}

		virtual RECT get_client_rect() const {
			RECT rc = { 0 };
			::GetClientRect(m_handle, &rc);
			return rc;
		}

		virtual BOOL map_dialog_rect(LPRECT rc) const {
			return ::MapDialogRect(m_handle, rc);
		}

		virtual BOOL set_text(const tstring& text) {
			if (!m_handle) return FALSE;
			return ::SetWindowText(m_handle, text.c_str());
		}

		virtual BOOL set_showing(int state = SW_NORMAL) {
			return ::ShowWindow(m_handle, state);
		}

		virtual BOOL is_enabled() const {
			return ::IsWindowEnabled(m_handle);
		}

		virtual BOOL set_enabled(BOOL enabled = TRUE) {
			return ::EnableWindow(m_handle, enabled);
		}

		virtual void set_topmost(BOOL topmost = TRUE) const {
			::SetWindowPos(m_handle, topmost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}

		virtual BOOL update_window() const {
			::InvalidateRect(m_handle, NULL, TRUE);
			return ::UpdateWindow(m_handle);
		}

		virtual bool is_topmost() const {
			return (get_ex_style() & WS_EX_TOPMOST) != 0;
		}

		virtual HWND focus() {
			return ::SetFocus(m_handle);
		}

		virtual BOOL is_focused() const {
			return ::GetFocus() == m_handle;
		}

		virtual HFONT get_font() const {
			return send_message<HFONT>(WM_GETFONT, 0, 0);
		}

		virtual void set_font(HFONT hFont, BOOL redraw = TRUE) {
			send_message(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(redraw, 0));
		}

		virtual LONG_PTR get_window_long(int index) const {
			return ::GetWindowLongPtr(m_handle, index);
		}

		virtual LONG_PTR set_window_long(int index, LONG_PTR dwNewLong) {
			return ::SetWindowLongPtr(m_handle, index, dwNewLong);
		}

		virtual DWORD get_style() const {
			return static_cast<DWORD>(get_window_long(GWL_STYLE));
		}

		virtual DWORD add_style(DWORD dwStyle) {
			DWORD new_style = get_style() | dwStyle;
			set_window_long(GWL_STYLE, new_style);
			return new_style;
		}

		virtual DWORD set_style(DWORD dwStyle) {
			set_window_long(GWL_STYLE, dwStyle);
			return dwStyle;
		}

		virtual DWORD remove_style(DWORD dwStyle) {
			DWORD new_style = get_style() & ~dwStyle;
			set_window_long(GWL_STYLE, new_style);
			return new_style;
		}

		virtual DWORD get_ex_style() const {
			return static_cast<DWORD>(get_window_long(GWL_EXSTYLE));
		}

		virtual DWORD add_ex_style(DWORD dwExStyle) {
			DWORD style = get_ex_style() | dwExStyle;
			set_window_long(GWL_EXSTYLE, style);
			return style;
		}

		virtual DWORD remove_ex_style(DWORD dwExStyle) {
			DWORD style = get_ex_style() & ~dwExStyle;
			set_window_long(GWL_EXSTYLE, style);
			return style;
		}

		//Stolen From ATL
		virtual BOOL modify_style(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) {
			DWORD dwStyle = get_style();
			DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
			if (dwStyle == dwNewStyle) return FALSE;
			set_window_long(GWL_STYLE, dwNewStyle);
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
					auto dwStyleCenter = ::GetWindowLongPtr(hWndCenter, GWL_STYLE);
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
				rcCenter = rcArea;

				if (hWndCenter != HWND_DESKTOP) {
					::GetWindowRect(hWndCenter, &rcCenter);
				}
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

		virtual BOOL set_position(int x, int y, int width, int height, UINT flags = SWP_NOZORDER | SWP_NOACTIVATE) {
			if (x == 0 && y == 0) flags |= SWP_NOMOVE;
			if (width == 0 && height == 0) flags |= SWP_NOSIZE;
			return ::SetWindowPos(m_handle, NULL, x, y, width, height, flags);
		}

		virtual BOOL set_pos(int x, int y) {
			return set_position(x, y, 0, 0);
		}

		virtual BOOL set_size(int width, int height) {
			return set_position(0, 0, width, height);
		}

		virtual BOOL invalidate(LPRECT rect = NULL, BOOL erase = FALSE) {
			return ::InvalidateRect(m_handle, rect, erase);
		}

		/// <summary>
		/// Iterate over all child windows, calling the provided callback for each child.
		/// </summary>
		/// <param name="callback">Function to call for each child window. Return false to stop iteration.</param>
		template<typename Func>
		void for_each_child(Func callback) const {
			HWND hChild = ::GetWindow(m_handle, GW_CHILD);
			while (hChild) {
				if constexpr (std::is_invocable_r_v<bool, Func, HWND>) {
					if (!callback(hChild)) break;
				} else {
					callback(hChild);
				}
				hChild = ::GetWindow(hChild, GW_HWNDNEXT);
			}
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
				if (ppv == nullptr)
					return E_POINTER;

				if (riid == IID_IUnknown) {
					*ppv = dynamic_cast<IUnknown*>(this);
					static_cast<IUnknown*>(*ppv)->AddRef();
					return S_OK;
				} else if (riid == __uuidof(base_com)) {
					*ppv = dynamic_cast<base_com*>(this);
					static_cast<IUnknown*>(*ppv)->AddRef();
					return S_OK;
				} else {
					*ppv = NULL;
					return E_NOINTERFACE;
				}
			}

			STDMETHODIMP_(ULONG) AddRef() {
				return InterlockedIncrement(&m_ref_count);
			}

			STDMETHODIMP_(ULONG) Release() {
				LONG count = InterlockedDecrement(&m_ref_count);
				if (count == 0)
					delete this;
				return count;
			}

		protected:
			LONG m_ref_count;
		};

		class file_dlg_event_handler :
			public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
			public ATL::CComCoClass<file_dlg_event_handler>,
			public IFileDialogEvents {
		public:
			using file_ok_callback = std::function<HRESULT(IFileDialog*)>;
			using folder_changing_callback = std::function<HRESULT(IFileDialog*, IShellItem*)>;
			using folder_changed_callback = std::function<HRESULT(IFileDialog*)>;
			using selection_changed_callback = std::function<HRESULT(IFileDialog*)>;
			using share_violation_callback = std::function<HRESULT(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*)>;
			using type_change_callback = std::function<HRESULT(IFileDialog*)>;
			using overwrite_callback = std::function<HRESULT(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*)>;

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

	using file_dialog_handler = ATL::CComObjectNoLock<com::file_dlg_event_handler>;
	using file_dialog_handler_stack = ATL::CComObjectStackEx<com::file_dlg_event_handler>;
}

#endif //WPP_COMMON_HPP