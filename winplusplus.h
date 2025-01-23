#ifndef __WINPLUSPLUS_H__
#define __WINPLUSPLUS_H__

/*
 *	Big thanks to DarthTon's Xenos Injector for his pseudo win32++ impl, helped a ton (especially mapping message values to appropriate callbacks with help from his bound member thunking)
 *	Also a big thanks to the creator of WTL:: For his functions that i started implementing but half way through just ended up copying his impl
 */

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
#include <functional>
#include <map>

namespace std
{
#ifdef _UNICODE
	using tstring = wstring;

	template <typename Type> std::tstring to_tstring(Type t)
	{
		return std::to_wstring(t);
	}
#else
	using tstring = string;

	template <typename Type> std::tstring to_tstring(Type t)
	{
		return std::to_string(t);
	}
#endif
}

namespace COM
{
	template <class BaseCom>
	class SkeletonCOM : public BaseCom
	{
	public:
		SkeletonCOM() : m_ref_count(0) {}

		STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
		{
			if (riid == IID_IUnknown) {
				*ppv = dynamic_cast<IUnknown*>(this);
				return S_OK;
			} else if (riid == __uuidof(BaseCom)) {
				*ppv = dynamic_cast<BaseCom*>(this);
				return S_OK;
			} else {
				*ppv = NULL;
				return E_NOINTERFACE;
			}
		}

		STDMETHODIMP_(ULONG) AddRef()
		{
			InterlockedIncrement((LONG*)&m_ref_count);
			return m_ref_count;
		}

		STDMETHODIMP_(ULONG) Release()
		{
			if (InterlockedDecrement((LONG*)&m_ref_count) == 0)
				delete this;
			return m_ref_count;
		}

	protected:
		ULONG m_ref_count;
	};

	class FileDialogEventsHandler :
		public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
		public ATL::CComCoClass<FileDialogEventsHandler>,
		public IFileDialogEvents
	{
	public:
		using FileOkCb = std::function<STDMETHODIMP(IFileDialog*)>;
		using FolderChangingCb = std::function<STDMETHODIMP(IFileDialog* pfd, IShellItem*)>;
		using FolderChangeCb = std::function<STDMETHODIMP(IFileDialog*)>;
		using SelectionChangeCb = std::function<STDMETHODIMP(IFileDialog*)>;
		using ShareViolationCb = std::function<STDMETHODIMP(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*)>;
		using TypeChangeCb = std::function<STDMETHODIMP(IFileDialog*)>;
		using OverwriteCb = std::function<STDMETHODIMP(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*)>;

		FileDialogEventsHandler() {}
		~FileDialogEventsHandler() {};

		BEGIN_COM_MAP(FileDialogEventsHandler)
			COM_INTERFACE_ENTRY(IFileDialogEvents)
		END_COM_MAP()

		STDOVERRIDEMETHODIMP OnFileOk(IFileDialog* pfd) { return m_FileOkCb == nullptr ? E_NOTIMPL : m_FileOkCb(pfd); }
		STDOVERRIDEMETHODIMP OnFolderChanging(IFileDialog* pfd, IShellItem* psiFolder) { return m_FolderChangingCb == nullptr ? E_NOTIMPL : m_FolderChangingCb(pfd, psiFolder); }
		STDOVERRIDEMETHODIMP OnFolderChange(IFileDialog* pfd) { return m_FolderChangeCb == nullptr ? E_NOTIMPL : m_FolderChangeCb(pfd); }
		STDOVERRIDEMETHODIMP OnSelectionChange(IFileDialog* pfd) { return m_SelectionChangeCb == nullptr ? E_NOTIMPL : m_SelectionChangeCb(pfd); }
		STDOVERRIDEMETHODIMP OnShareViolation(IFileDialog* pfd, IShellItem* psi, FDE_SHAREVIOLATION_RESPONSE* pResponse) { return m_ShareViolationCb == nullptr ? E_NOTIMPL : m_ShareViolationCb(pfd, psi, pResponse); }
		STDOVERRIDEMETHODIMP OnTypeChange(IFileDialog* pfd) { return m_TypeChangeCb == nullptr ? E_NOTIMPL : m_TypeChangeCb(pfd); }
		STDOVERRIDEMETHODIMP OnOverwrite(IFileDialog* pfd, IShellItem* psi, FDE_OVERWRITE_RESPONSE* pResponse) { return m_OverwriteCb == nullptr ? E_NOTIMPL : m_OverwriteCb(pfd, psi, pResponse); }

		FileOkCb& FileOk() { return m_FileOkCb; }
		FolderChangingCb& FolderChanging() { return m_FolderChangingCb; }
		FolderChangeCb& FolderChange() { return m_FolderChangeCb; }
		SelectionChangeCb& SelectionChange() { return m_SelectionChangeCb; }
		ShareViolationCb& ShareViolation() { return m_ShareViolationCb; }
		TypeChangeCb& TypeChange() { return m_TypeChangeCb; }
		OverwriteCb& Overwrite() { return m_OverwriteCb; }

	private:
		FileOkCb m_FileOkCb;
		FolderChangingCb m_FolderChangingCb;
		FolderChangeCb m_FolderChangeCb;
		SelectionChangeCb m_SelectionChangeCb;
		ShareViolationCb m_ShareViolationCb;
		TypeChangeCb m_TypeChangeCb;
		OverwriteCb m_OverwriteCb;
	};
}

using FileDialogHandler = ATL::CComObjectNoLock<COM::FileDialogEventsHandler>;
using StackFileDialogHandler = ATL::CComObjectStackEx<COM::FileDialogEventsHandler>;

#include "Interfaces.hpp"
#include "Dialog.hpp"
#include "Window.hpp"

#endif //__WINPLUSPLUS_H__

