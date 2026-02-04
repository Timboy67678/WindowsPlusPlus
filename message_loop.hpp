#ifndef __MESSAGE_LOOP_H__
#define __MESSAGE_LOOP_H__

#include "winplusplus.h"

namespace wpp
{
	class message_loop {
	public:
		message_loop() = default;
		virtual ~message_loop() = default;

		bool register_window(hwnd& window) {
			if (std::find(m_message_windows.begin(), m_message_windows.end(), window) != m_message_windows.end())
				return false;
			m_message_windows.push_back(window);
			return true;
		}

		bool unregister_window(hwnd& window) {
			auto it = std::find(m_message_windows.begin(), m_message_windows.end(), window);
			if (it != m_message_windows.end()) {
				m_message_windows.erase(it);
				return true;
			}
			return false;
		}

		int run() {
			if (m_message_loop_running.load())
				return -1; // Already running
			m_message_loop_running.store(true);
			MSG msg;
			while (m_message_loop_running.load() && ::GetMessage(&msg, NULL, 0, 0)) {
				if (!is_dialog_message(msg)) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			m_message_loop_running.store(false);
			return static_cast<int>(msg.wParam);
		}
	
		void stop() {
			m_message_loop_running.store(false);
		}

		bool is_running() const {
			return m_message_loop_running.load();
		}

		void clear_windows() {
			m_message_windows.clear();
		}

		size_t window_count() const {
			return m_message_windows.size();
		}

	private:
		bool is_dialog_message(MSG& msg) {
			// Clean up invalid handles
			m_message_windows.erase(
				std::remove_if(m_message_windows.begin(), m_message_windows.end(),
							   [](hwnd handle) { return !::IsWindow(handle.get_handle()); }),
				m_message_windows.end()
			);

			if (m_message_windows.empty()) {
				::PostQuitMessage(0);
				return false;
			}

			// Check if message is for any registered dialog
			for (hwnd window : m_message_windows) {
				if (::IsDialogMessage(window.get_handle(), &msg))
					return true;
			}
			return false;
		}

		std::atomic_bool m_message_loop_running = false;
		std::vector<hwnd> m_message_windows;
	};
}

#endif // __MESSAGE_LOOP_H__