#include "../dialog.hpp"
#include "../thunk.hpp"

namespace wpp
{
	dialog::dialog(HINSTANCE instance, int resource_id, int menu_id)
		: m_main_instance(instance), hwnd(resource_id, NULL), m_internal_timerid(0), m_menu_id(menu_id), m_menu(NULL) {
		init_message_events();
	}

	dialog::dialog(HWND hWnd)
		: hwnd(hWnd), m_internal_timerid(0), m_menu_id(-1), m_menu(NULL) {
		m_main_instance = (HINSTANCE)::GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		init_message_events();
	}

	dialog::~dialog() {
		cleanup();
	}

	void dialog::init_message_events() {
		using namespace std::placeholders;

		m_message_events = {
			{WM_INITDIALOG, std::bind(&dialog::on_init_dialog, this, _1, _2, _3)},
			{WM_CLOSE, std::bind(&dialog::on_close, this, _1, _2, _3)},
			{WM_QUIT, std::bind(&dialog::on_quit, this, _1, _2, _3)},
			{WM_DESTROY, std::bind(&dialog::on_destroy, this, _1, _2, _3)},
			{WM_DISPLAYCHANGE, std::bind(&dialog::on_display_change, this, _1, _2, _3)},
			{WM_MOVE, std::bind(&dialog::on_move, this, _1, _2, _3)},
			{WM_COMMAND, std::bind(&dialog::on_command, this, _1, _2, _3)},
			{WM_MENUCOMMAND, std::bind(&dialog::on_menu_command, this, _1, _2, _3)},
			{WM_PAINT, std::bind(&dialog::on_paint, this, _1, _2, _3)},
			{WM_TIMER, std::bind(&dialog::on_timer, this, _1, _2, _3)},
			{WM_SIZE, std::bind(&dialog::on_size, this, _1, _2, _3)},
			{WM_KEYDOWN, std::bind(&dialog::on_key_down, this, _1, _2, _3)},
			{WM_KEYUP, std::bind(&dialog::on_key_up, this, _1, _2, _3)},
			{WM_NOTIFY, std::bind(&dialog::on_notify, this, _1, _2, _3)},
			{WM_HSCROLL, std::bind(&dialog::on_h_scroll, this, _1, _2, _3)},
			{WM_VSCROLL, std::bind(&dialog::on_v_scroll, this, _1, _2, _3)},
			{WM_DROPFILES, std::bind(&dialog::on_drop_files, this, _1, _2, _3)},
		};
	}

	void dialog::cleanup() {
		for (auto& timer : m_timer_events)
			::KillTimer(m_handle, timer.first);

		for (auto& control_pair : m_controls)
			control_pair.reset();

		m_timer_events.clear();
		m_menu_command_events.clear();
		m_controls.clear();

		::DestroyMenu(m_menu);
	}

	bool dialog::handle_scroll_message(scroll_orientation orientation, WPARAM wParam, LPARAM lParam) {
		if(lParam == NULL)
			return false;

		HWND hScrollBar = reinterpret_cast<HWND>(lParam);

		auto scrollbar = get_control_by_handle<scroll_bar>(hScrollBar);
		if (scrollbar) {
			int action = LOWORD(wParam);
			switch (action) {
			case SB_LINEUP:
				scrollbar->scroll_up(1, FALSE);
				break;
			case SB_LINEDOWN:
				scrollbar->scroll_down(1, FALSE);
				break;
			case SB_PAGEUP:
				scrollbar->page_up(FALSE);
				break;
			case SB_PAGEDOWN:
				scrollbar->page_down(FALSE);
				break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				scrollbar->set_scroll_pos(HIWORD(wParam), TRUE);
				break;
			case SB_TOP:
				scrollbar->scroll_to_top(FALSE);
				break;
			case SB_BOTTOM:
				scrollbar->scroll_to_bottom(FALSE);
				break;
			}

			scrollbar->on_scroll_event(orientation, wParam, lParam);
			return true;
		}
		return false;
	}

	void dialog::show_dialog() {
		::ShowWindow(m_handle, SW_SHOWNORMAL);
	}

	void dialog::hide_dialog() {
		::ShowWindow(m_handle, SW_HIDE);
	}

	void dialog::end_dialog() {
		cleanup();
		if (m_is_modeless) {
			::DestroyWindow(m_handle);
		} else {
			::EndDialog(m_handle, 0);
		}
	}

	INT_PTR dialog::run_dlg(HWND parent, LPVOID param) {
		m_parent_handle = parent;
		m_is_modeless = false;
		auto thunk = new Win32Thunk<DLGPROC, dialog>{ &dialog::dialog_proc, this };
		m_thunk_storage = std::unique_ptr<void, void(*)(void*)>(
			thunk,
			+[](void* p) { delete static_cast<Win32Thunk<DLGPROC, dialog>*>(p); }
		);
		return ::DialogBoxParam(m_main_instance, MAKEINTRESOURCE(m_item_id), parent, thunk->GetThunk(), (LPARAM)param);
	}

	void dialog::create_modeless(HWND parent, LPVOID param) {
		m_parent_handle = parent;
		m_is_modeless = true;
		auto thunk = new Win32Thunk<DLGPROC, dialog>{ &dialog::dialog_proc, this };
		m_thunk_storage = std::unique_ptr<void, void(*)(void*)>(
			thunk,
			+[](void* p) { delete static_cast<Win32Thunk<DLGPROC, dialog>*>(p); }
		);
		m_handle = ::CreateDialogParam(m_main_instance, MAKEINTRESOURCE(m_item_id), parent, thunk->GetThunk(), (LPARAM)param);
		if (m_handle) {
			::ShowWindow(m_handle, SW_SHOWNORMAL);
		}
	}

#pragma region Overrides
	INT_PTR dialog::on_init_dialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		if (m_menu_id != -1)
			m_menu = ::LoadMenu(m_main_instance, MAKEINTRESOURCE(m_menu_id));
		return TRUE;
	}

	INT_PTR dialog::on_close(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		end_dialog();
		return FALSE;
	}

	INT_PTR dialog::on_quit(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_timer(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		auto it = m_timer_events.find((UINT_PTR)wParam);
		if (it != m_timer_events.end())
			it->second();
		return FALSE;
	}

	INT_PTR dialog::on_notify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		auto nm = reinterpret_cast<LPNMHDR>(lParam);
		for (const auto& control : m_controls)
			if (control && nm->idFrom == control->get_id())
				control->on_notify_callback(nm);
		return TRUE;
	}

	INT_PTR dialog::on_command(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		UINT commandID = LOWORD(wParam);
		UINT notificationCode = HIWORD(wParam);

		// Check if the command is a menu command
		if (notificationCode == 0) {
			auto menuCommandIt = m_menu_command_events.find(commandID);
			if (menuCommandIt != m_menu_command_events.end()) {
				menuCommandIt->second(wParam, lParam);
				return TRUE;
			}
		}

		// Check if the command is from a control
		auto control = get_control(commandID);
		if (control) {
			control->on_command_callback(wParam, lParam);
			return TRUE;
		}

		return FALSE;
	}

	INT_PTR dialog::on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		end_dialog();
		return TRUE;
	}

	INT_PTR dialog::on_display_change(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}


	INT_PTR dialog::on_move(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_menu_command(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_size(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_key_up(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::on_h_scroll(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		if (lParam != 0) {
			return handle_scroll_message(scroll_orientation::horizontal, wParam, lParam);
		}
		return FALSE;
	}

	INT_PTR dialog::on_v_scroll(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		if (lParam != 0) {
			return handle_scroll_message(scroll_orientation::vertical, wParam, lParam);
		}
		return FALSE;
	}

	INT_PTR dialog::on_drop_files(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		return FALSE;
	}

	INT_PTR dialog::dialog_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
		m_handle = hWnd;
		auto it = m_message_events.find(Msg);
		if (it != m_message_events.end())
			return it->second(hWnd, wParam, lParam);
		return FALSE;
	}
#pragma endregion
}
