#ifndef __MESSAGE_LOOP_H__
#define __MESSAGE_LOOP_H__

#include "winplusplus.h"

namespace wpp
{
	/// @class message_loop
	/// @brief Manages a Windows message loop for one or more dialog windows.
	///
	/// The message_loop class provides a flexible Windows message processing system
	/// that supports multiple registered dialog windows. It offers both blocking
	/// (run) and non-blocking (run_peek) message loop implementations.
	///
	/// Features:
	/// - Register and unregister multiple dialog windows
	/// - Thread-safe message loop execution
	/// - Support for idle callbacks in peek mode
	/// - Automatic cleanup of invalid window handles
	/// - Control over message loop lifecycle
	///
	/// @note This class is not copyable but is move-constructible and move-assignable.
	/// @note Only one message loop can run at a time per instance.
	class message_loop {
	public:
		/// @brief Default constructor.
		message_loop() = default;

		/// @brief Destructor.
		~message_loop() = default;

		/// @brief Deleted copy constructor.
		message_loop(const message_loop&) = delete;

		/// @brief Deleted copy assignment operator.
		message_loop& operator=(const message_loop&) = delete;

		/// @brief Move constructor.
		message_loop(message_loop&&) noexcept = default;

		/// @brief Move assignment operator.
		message_loop& operator=(message_loop&&) noexcept = default;

		/// @brief Registers a window to receive messages from this message loop.
		/// @param window The hwnd handle of the window to register.
		/// @return true if the window was successfully registered, false if already registered.
		bool register_window(const hwnd& window) {
			if (std::find(m_message_windows.begin(), m_message_windows.end(), window) != m_message_windows.end())
				return false;
			m_message_windows.push_back(window);
			return true;
		}

		/// @brief Unregisters a window from this message loop.
		/// @param window The hwnd handle of the window to unregister.
		/// @return true if the window was successfully unregistered, false if not found.
		bool unregister_window(const hwnd& window) {
			auto it = std::find(m_message_windows.begin(), m_message_windows.end(), window);
			if (it != m_message_windows.end()) {
				m_message_windows.erase(it);
				return true;
			}
			return false;
		}

		/// @brief Runs the message loop in blocking mode.
		///
		/// Processes messages for all registered windows until WM_QUIT is received.
		/// This is the standard Windows message loop implementation.
		///
		/// @return The wParam of the WM_QUIT message, or -1 if already running.
		int run() {
			if (m_message_loop_running.exchange(true))
				return -1; // Already running

			MSG msg{};
			while (m_message_loop_running.load() && ::GetMessage(&msg, NULL, 0, 0)) {
				if (!is_dialog_message(msg)) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			m_message_loop_running.store(false);
			return static_cast<int>(msg.wParam);
		}

		/// @brief Runs the message loop in non-blocking peek mode.
		///
		/// Processes available messages and calls an idle callback when no messages
		/// are pending. This allows for background processing without blocking.
		///
		/// @param idle_callback Optional callback invoked when no messages are available.
		///                       Return false to exit the loop, true to continue.
		///                       If nullptr, the loop yields to other threads.
		/// @return The wParam of WM_QUIT or 0 on normal exit, or -1 if already running.
		int run_peek(std::function<bool()> idle_callback = nullptr) {
			if (m_message_loop_running.exchange(true))
				return -1; // Already running

			MSG msg{};
			while (m_message_loop_running.load()) {
				// PM_REMOVE: Remove message from queue after peeking
				if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						m_message_loop_running.store(false);
						return static_cast<int>(msg.wParam);
					}
					if (!is_dialog_message(msg)) {
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				} else {
					// No message available - call idle callback or yield
					if (idle_callback && !idle_callback()) {
						m_message_loop_running.store(false);
						return 0;
					} else {
						::Sleep(0); // Yield to other threads
					}
				}
			}
			m_message_loop_running.store(false);
			return 0;
		}

		/// @brief Stops the message loop.
		///
		/// Sets the running flag to false, causing the message loop to exit
		/// on the next iteration.
		void stop() {
			m_message_loop_running.store(false);
		}

		/// @brief Checks if the message loop is currently running.
		/// @return true if the message loop is running, false otherwise.
		bool is_running() const {
			return m_message_loop_running.load();
		}

		/// @brief Clears all registered windows.
		void clear_windows() {
			m_message_windows.clear();
		}

		/// @brief Gets the number of registered windows.
		/// @return The count of currently registered windows.
		size_t window_count() const {
			return m_message_windows.size();
		}

	private:
		/// @brief Checks if a message belongs to any registered dialog window.
		///
		/// Also performs cleanup of invalid window handles.
		///
		/// @param msg The message to check.
		/// @return true if the message was processed by a dialog, false otherwise.
		bool is_dialog_message(MSG& msg) {
			// Clean up invalid handles
			m_message_windows.erase(
				std::remove_if(m_message_windows.begin(), m_message_windows.end(),
							   [](const hwnd& handle) { return !::IsWindow(handle.get_handle()); }),
				m_message_windows.end()
			);

			if (m_message_windows.empty()) {
				::PostQuitMessage(0);
				return false;
			}

			// Check if message is for any registered dialog
			for (const hwnd& window : m_message_windows) {
				if (::IsDialogMessage(window.get_handle(), &msg))
					return true;
			}
			return false;
		}

		std::atomic_bool m_message_loop_running = false; ///< Thread-safe flag indicating if loop is running
		std::vector<hwnd> m_message_windows;             ///< Collection of registered dialog windows
	};
}

#endif // __MESSAGE_LOOP_H__