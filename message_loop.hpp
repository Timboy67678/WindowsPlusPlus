#ifndef __MESSAGE_LOOP_H__
#define __MESSAGE_LOOP_H__

#include "winplusplus.hpp"

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
		bool register_window(const window_base& window) {
			std::scoped_lock lock(m_windows_mutex);
			if (std::ranges::find(m_message_windows, window) != m_message_windows.end())
				return false;
			m_message_windows.push_back(window);
			return true;
		}

		/// @brief Unregisters a window from this message loop.
		/// @param window The hwnd handle of the window to unregister.
		/// @return true if the window was successfully unregistered, false if not found.
		bool unregister_window(const window_base& window) {
			std::scoped_lock lock(m_windows_mutex);
			auto it = std::ranges::find(m_message_windows, window);
			if (it != m_message_windows.end()) {
				m_message_windows.erase(it);
				return true;
			}
			return false;
		}

		/// @brief Runs the message loop in blocking mode.
		/// Processes messages for all registered windows until WM_QUIT is received.
		/// This is the standard Windows message loop implementation.
		/// @return The wParam of the WM_QUIT message, or std::nullopt if already running.
		std::optional<WPARAM> run() {
			auto guard = try_start_loop();
			if (!guard)
				return std::nullopt;

			MSG msg{};
			while (m_message_loop_running.load(std::memory_order_acquire)) {
				const BOOL result = ::GetMessage(&msg, nullptr, 0, 0);

				if (result == -1) {
					return 0;
				}

				if (result == 0 || msg.message == WM_QUIT) {
					return msg.wParam;
				}

				if (!process_message(msg) && should_exit_on_no_windows()) {
					::PostQuitMessage(0);
					return 0;
				}
			}

			return 0;
		}

		/// @brief Runs the message loop in non-blocking peek mode.
		/// Processes available messages and calls an idle callback when no messages
		/// are pending. This allows for background processing without blocking.
		/// @param idle_callback Optional callback function that is called when no messages are available. 
		/// Should return true to continue running, or false to exit.
		/// @param user_data Optional user data passed to the idle callback.
		/// @return The wParam of WM_QUIT or 0 on normal exit, or std::nullopt if already running.
		std::optional<WPARAM> run_peek(std::function<bool(void*)> idle_callback = nullptr, void* user_data = nullptr) {
			auto guard = try_start_loop();
			if (!guard)
				return std::nullopt;

			MSG msg{};
			while (m_message_loop_running.load(std::memory_order_acquire)) {
				if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						return msg.wParam;
					}

					if (!process_message(msg) && should_exit_on_no_windows()) {
						return 0;
					}
				} else {
					if (should_exit_on_no_windows()) {
						return 0;
					}

					if (idle_callback && !idle_callback(user_data)) {
						return 0;
					}

					std::this_thread::yield();
				}
			}

			return 0;
		}

		/// @brief Stops the message loop.
		/// Sets the running flag to false, causing the message loop to exit
		/// on the next iteration.
		void stop() noexcept {
			m_message_loop_running.store(false, std::memory_order_release);
		}

		/// @brief Checks if the message loop is currently running.
		/// @return true if the message loop is running, false otherwise.
		[[nodiscard]] bool is_running() const noexcept {
			return m_message_loop_running.load(std::memory_order_acquire);
		}

		/// @brief Clears all registered windows.
		void clear_windows() {
			std::scoped_lock lock(m_windows_mutex);
			m_message_windows.clear();
		}

		/// @brief Gets the number of registered windows.
		/// @return The count of currently registered windows.
		[[nodiscard]] size_t window_count() const {
			std::scoped_lock lock(m_windows_mutex);
			return m_message_windows.size();
		}

	private:
		class loop_guard {
		public:
			explicit loop_guard(std::atomic_bool& flag) : m_flag(&flag) {}

			~loop_guard() {
				if (m_flag) {
					m_flag->store(false, std::memory_order_release);
				}
			}

			loop_guard(const loop_guard&) = delete;
			loop_guard& operator=(const loop_guard&) = delete;

			loop_guard(loop_guard&& other) noexcept : m_flag(other.m_flag) {
				other.m_flag = nullptr;
			}

			loop_guard& operator=(loop_guard&&) = delete;

		private:
			std::atomic_bool* m_flag;
		};

		/// @brief Attempts to start the message loop
		/// @return Optional guard that manages the running state, nullopt if already running
		[[nodiscard]] std::optional<loop_guard> try_start_loop() {
			bool expected = false;
			if (!m_message_loop_running.compare_exchange_strong(expected, true,
																std::memory_order_acq_rel)) {
				return std::nullopt;
			}
			return loop_guard(m_message_loop_running);
		}

		/// @brief Processes a single message
		/// @param msg The message to process
		/// @return true if windows remain, false if all windows are gone
		bool process_message(MSG& msg) {
			if (!is_dialog_message(msg)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			return !cleanup_invalid_windows();
		}

		/// @brief Checks if all windows are gone and we should exit
		/// @return true if should exit, false otherwise
		[[nodiscard]] bool should_exit_on_no_windows() {
			return cleanup_invalid_windows();
		}

		/// @brief Removes invalid window handles and returns true if no windows remain.
		/// @return true if all windows have been removed, false otherwise.
		bool cleanup_invalid_windows() {
			std::scoped_lock lock(m_windows_mutex);

			auto new_end = std::ranges::remove_if(m_message_windows,
												  [](const window_base& window) {
				return !::IsWindow(window.get_handle());
			}).begin();

			m_message_windows.erase(new_end, m_message_windows.end());
			return m_message_windows.empty();
		}

		/// @brief Checks if the message is a dialog message for any registered window
		/// @param msg The message to check
		/// @return true if message was handled as dialog message, false otherwise
		[[nodiscard]] bool is_dialog_message(MSG& msg) {
			std::scoped_lock lock(m_windows_mutex);

			return std::ranges::any_of(m_message_windows,
									   [&msg](const window_base& window) {
				return ::IsWindow(window.get_handle()) &&
					::IsDialogMessage(window.get_handle(), &msg);
			});
		}

		mutable std::mutex m_windows_mutex;				 ///< Mutex to protect access to the message windows collection
		std::atomic_bool m_message_loop_running = false; ///< Thread-safe flag indicating if loop is running
		std::vector<window_base> m_message_windows;      ///< Collection of registered dialog windows
	};
}

#endif // __MESSAGE_LOOP_H__
