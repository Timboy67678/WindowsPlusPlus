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
#include <algorithm>

#define TIMER_HANDLER(X) virtual void CALLBACK X()

namespace std
{
#ifdef _UNICODE
	using tstring = wstring;

	template <typename Type> std::tstring to_tstring(Type t) {
		return std::to_wstring(t);
	}
#else
	using tstring = string;

	template <typename Type> std::tstring to_tstring(Type t) {
		return std::to_string(t);
	}
#endif
}

namespace wpp
{
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