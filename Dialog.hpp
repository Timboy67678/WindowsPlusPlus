#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "winplusplus.hpp"
#include "window_base.hpp"
#include "message_loop.hpp"

namespace wpp
{
	/// <summary>
	/// Represents a dialog window in a Windows application.
	/// </summary>
	class dialog : public window_base {
	public:
		using menu_callback = std::function<void(WPARAM, LPARAM)>;
		using dialog_message_callback = std::function<INT_PTR(HWND, WPARAM, LPARAM)>;
		using message_handler = INT_PTR(HWND hWnd, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Constructs a dialog using the specified application instance and resource identifiers.
		/// </summary>
		/// <param name="instance">A handle to the application instance that contains the dialog resource.</param>
		/// <param name="resource_id">The identifier of the dialog resource template.</param>
		/// <param name="menu_id">The identifier of the menu resource to attach to the dialog. Defaults to -1 if no menu is specified.</param>
		dialog(HINSTANCE instance, int resource_id, int menu_id = -1);

		/// <summary>
		/// Creates or displays a dialog window.
		/// </summary>
		/// <param name="hWnd">A handle to the parent or owner window for the dialog.</param>
		dialog(HWND hWnd);

		/// <summary>
		/// Virtual destructor for the dialog class.
		/// </summary>
		virtual ~dialog() noexcept;

		virtual message_handler on_init_dialog;
		virtual message_handler on_close;
		virtual message_handler on_timer;
		virtual message_handler on_notify;
		virtual message_handler on_command;
		virtual message_handler on_destroy;
		virtual message_handler on_display_change;
		virtual message_handler on_move;
		virtual message_handler on_menu_command;
		virtual message_handler on_paint;
		virtual message_handler on_size;
		virtual message_handler on_key_down;
		virtual message_handler on_key_up;
		virtual message_handler on_h_scroll;
		virtual message_handler on_v_scroll;
		virtual message_handler on_drop_files;

		/// <summary>
		/// Runs the dialog box with optional parent window and parameter.
		/// </summary>
		/// <param name="parent">Handle to the parent window. If NULL, the dialog has no parent.</param>
		/// <param name="param">Optional pointer to user-defined data passed to the dialog.</param>
		/// <returns>The dialog result value, typically indicating how the dialog was closed.</returns>
		virtual INT_PTR run_dlg(HWND parent = NULL, LPVOID param = NULL);

		/// <summary>
		/// Creates a modeless dialog or window.
		/// </summary>
		/// <param name="parent">Handle to the parent window. If NULL, the dialog has no parent.</param>
		/// <param name="param">Optional user-defined data to pass to the dialog during creation.</param>
		virtual void create_modeless(HWND parent = NULL, LPVOID param = NULL);

		/// <summary>
		/// Processes messages for a dialog window.
		/// </summary>
		/// <param name="hWnd">A handle to the dialog box.</param>
		/// <param name="Msg">The message identifier.</param>
		/// <param name="wParam">Additional message-specific information.</param>
		/// <param name="lParam">Additional message-specific information.</param>
		/// <returns>The return value is typically TRUE if the message was processed, or FALSE otherwise. The specific meaning depends on the message being processed.</returns>
		virtual INT_PTR dialog_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Determines whether this object is modeless.
		/// </summary>
		/// <returns>True if the object is modeless; otherwise, false.</returns>
		bool is_modeless() const { return m_is_modeless; }

		void end_dialog(INT_PTR result) {
			if (m_is_modeless) {
				destroy();
			} else {
				::EndDialog(m_handle, result);
			}
		}

		/// <summary>
		/// Registers or unregisters a callback for a menu command.
		/// </summary>
		/// <param name="menu_id">The unique identifier of the menu command.</param>
		/// <param name="callback">The callback function to register. If null or empty, the menu command callback is unregistered.</param>
		void register_menu_command(UINT_PTR menu_id, menu_callback callback) {
			if (callback)
				m_menu_command_events[menu_id] = std::move(callback);
			else
				m_menu_command_events.erase(menu_id);
		}

		/// <summary>
		/// Registers a control by creating and storing it in the control collection.
		/// </summary>
		/// <typeparam name="CtrlType">The type of control to register. Defaults to the base control type.</typeparam>
		/// <param name="control_id">The unique identifier for the control to register.</param>
		/// <param name="ctrl">A reference to a control pointer that will be populated with the newly created control.</param>
		/// <returns>True if the control was successfully created and registered; otherwise, false.</returns>
		template<typename CtrlType = control>
		bool register_control(UINT control_id, control_ptr<CtrlType>& ctrl) {
			ctrl = std::make_shared<CtrlType>(control_id, m_handle);
			if (ctrl != nullptr)
				m_controls.emplace_back(ctrl);
			return ctrl != nullptr;
		}

		/// <summary>
		/// Retrieves a control by its ID and casts it to the specified control type.
		/// </summary>
		/// <typeparam name="CtrlType">The type of control to cast the result to. Defaults to the base control type.</typeparam>
		/// <param name="control_id">The unique identifier of the control to retrieve.</param>
		/// <returns>A shared pointer to the control cast to the specified type, or nullptr if no control with the given ID is found or the cast fails.</returns>
		template<typename CtrlType = control>
		control_ptr<CtrlType> get_control(UINT control_id) {
			for (auto& control : m_controls)
				if (control && control->get_id() == control_id)
					return std::dynamic_pointer_cast<CtrlType>(control);
			return nullptr;
		}

		/// <summary>
		/// Retrieves a control from the collection by its window handle.
		/// </summary>
		/// <typeparam name="CtrlType">The type of control to retrieve. Defaults to the base control type.</typeparam>
		/// <param name="handle">The window handle (HWND) of the control to find.</param>
		/// <returns>A shared pointer to the control of the specified type if found and the cast succeeds; otherwise, nullptr.</returns>
		template<typename CtrlType = control>
		inline control_ptr<CtrlType> get_control_by_handle(HWND handle) const {
			if (!handle) return nullptr;

			auto it = std::find_if(m_controls.begin(), m_controls.end(), [handle](const auto& control) {
				return control && control->get_handle() == handle;
			});

			return (it != m_controls.end()) ? std::dynamic_pointer_cast<CtrlType>(*it) : nullptr;
		}

		/// <summary>
		/// Gets a read-only reference to the controls collection.
		/// </summary>
		/// <returns>A const reference to the vector of controls.</returns>
		inline const std::vector<control_ptr<>>& get_controls() const override { return m_controls; }

	private:
		void init_message_events();
		void cleanup();
		bool handle_scroll_message(scroll_orientation orientation, WPARAM wParam, LPARAM lParam);

	protected:
		std::unique_ptr<void, void(*)(void*)> m_thunk_storage{ nullptr, +[](void* p) {} }; ///< Thunk storage for dialog procedure.
		
		std::map<UINT_PTR, menu_callback> m_menu_command_events; ///< Menu command events.
		std::map<INT, dialog_message_callback> m_message_events; ///< Message events.
		std::vector<control_ptr<>> m_controls; ///< Controls container.
		
		bool m_is_modeless = false; ///< Modeless flag.

		UINT_PTR m_internal_timerid; ///< Internal timer ID.

		HINSTANCE m_main_instance; ///< Main instance handle.

		int m_menu_id; ///< Menu ID.

		HMENU m_menu; ///< Menu handle.
	};
}

#endif //__DIALOG_H__
