#include "..\layout\panel.hpp"

namespace wpp::layout
{
    panel::panel(type panel_type, HWND parent)
        : control(create_panel_window(parent)),
        m_panel_type(panel_type) {
        if (m_handle) {
			m_original_wndproc = reinterpret_cast<WNDPROC>(set_window_long(GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(panel_wndproc)));
			set_window_long(GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        }
    }

	LRESULT CALLBACK panel::panel_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		panel* panel_ptr = reinterpret_cast<panel*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

		LRESULT result = 0;
		if (panel_ptr && panel_ptr->m_original_wndproc) {
			result = ::CallWindowProc(panel_ptr->m_original_wndproc, hwnd, msg, wParam, lParam);
		} else {
			result = ::DefWindowProc(hwnd, msg, wParam, lParam);
		}

		if (msg == WM_PAINT && panel_ptr) {
			HDC hdc = ::GetDC(hwnd);
			if (hdc) {
				panel_ptr->paint(hdc);
				::ReleaseDC(hwnd, hdc);
			}
		}

		return result;
	}
}
