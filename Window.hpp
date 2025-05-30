#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "winplusplus.h"

#define WINDOW_MESSAGE_HANDLER(X) virtual LRESULT CALLBACK X(HWND hWnd, WPARAM wParam, LPARAM lParam)

#define WINDOW_TIMER_OFFSET_START 0x2374

namespace WPP
{
	class Window : public Hwnd
	{
	public:
		using MenuCallback = std::function<void(WPARAM, LPARAM)>;
		using TimerCallback = std::function<void()>;
		using WindowMessageCallback = std::function<LRESULT(HWND, WPARAM, LPARAM)>;

		/**
		 * @struct Class
		 * @brief Encapsulates window class information.
		 */
		struct Class {
			friend class Window;

			/**
			 * @brief Default constructor.
			 */
			Class() = default;

			/**
			 * @brief Parameterized constructor.
			 * @param name Class name.
			 * @param instance Instance handle.
			 * @param icon Icon handle.
			 * @param cursor Cursor handle.
			 * @param background Background brush.
			 * @param menu Menu name.
			 * @param style Class style.
			 * @param extra Extra bytes.
			 */
			Class(LPCTSTR name, HINSTANCE instance = NULL, HICON icon = NULL, HCURSOR cursor = NULL,
				  HBRUSH background = (HBRUSH)(COLOR_WINDOW), LPCTSTR menu = NULL, UINT style = 0, int extra = 0)
			{
				ZeroMemory(&m_window_class, sizeof(WNDCLASSEX));
				m_window_class.cbSize = sizeof(WNDCLASSEX);
				m_window_class.style = style;
				m_window_class.cbClsExtra = 0;
				m_window_class.cbWndExtra = extra;
				m_window_class.hInstance = instance;
				m_window_class.hIcon = icon;
				m_window_class.hCursor = cursor;
				m_window_class.hbrBackground = background;
				m_window_class.lpszMenuName = menu;
				m_window_class.lpszClassName = name;
				m_window_class.hIconSm = icon;
			}

		protected:
			/**
			 * @brief Gets the instance handle.
			 * @return Instance handle.
			 */
			HINSTANCE instance() const { return m_window_class.hInstance; }

			/**
			 * @brief Gets the class name.
			 * @return Class name.
			 */
			LPCTSTR class_name() const { return m_window_class.lpszClassName; }

			/**
			 * @brief Gets the class style.
			 * @return Class style.
			 */
			UINT style() const { return m_window_class.style; }

			/**
			 * @brief Registers the window class.
			 */
			void Register() {
				m_class_atom = RegisterClassEx(&m_window_class);
			}

			/**
			 * @brief Unregisters the window class.
			 */
			void Unregister() {
				UnregisterClass(m_window_class.lpszClassName, m_window_class.hInstance);
				m_class_atom = NULL;
			}

			/**
			 * @brief Gets the class atom.
			 * @return Class atom.
			 */
			ATOM& atom() { return m_class_atom; }

			/**
			 * @brief Gets the window class structure.
			 * @return Window class structure.
			 */
			WNDCLASSEX& get() { return m_window_class; }

		private:
			WNDCLASSEX m_window_class; ///< Window class structure.
			ATOM m_class_atom = NULL; ///< Class atom.
		};

		/**
		* @class RadioButtonGroup
		* @brief Manages a group of radio buttons within a window.
		*/
		class RadioButtonGroup {
			friend class Window;
		public:
			/**
			* @brief Constructor.
			* @param parent Pointer to the parent window.
			*/
			RadioButtonGroup(Window* parent)
				: m_Parent(parent)
			{}

			virtual ~RadioButtonGroup() = default;

			/**
			* @brief Creates a radio button.
			* @param control_id Control ID.
			* @param text Button text.
			* @param x X position.
			* @param y Y position.
			* @param width Button width.
			* @param height Button height.
			* @param initial_state Initial state of the button.
			* @return Pointer to the created radio button.
			*/
			std::shared_ptr<RadioButton> CreateButton(LPCTSTR text, int x, int y, int width, int height, BOOL initial_state = FALSE);

			/**
			* @brief Gets the index of the selected radio button.
			* @return Index of the selected radio button.
			*/
			int SelectedIndex();

		private:
			std::vector<std::shared_ptr<RadioButton>> m_RadioButtons; ///< Vector of radio buttons.
			Window* m_Parent; ///< Pointer to the parent window.
		};

		/**
		 * @brief Constructor.
		 * @param wnd_class Window class.
		 * @param window_name Window name.
		 * @param x_pos X position.
		 * @param y_pos Y position.
		 * @param width Width.
		 * @param height Height.
		 * @param style Window style.
		 * @param menu_id Menu ID.
		 * @param menu Menu handle.
		 * @param param Additional parameters.
		 * @param style_ex Extended window style.
		 */
		Window(Class wnd_class, LPCTSTR window_name, int x_pos, int y_pos, int width, int height, DWORD style = WS_OVERLAPPEDWINDOW,
			   int menu_id = -1, HMENU menu = NULL, HFONT font = NULL, DWORD style_ex = 0);

		/**
		 * @brief Destructor.
		 */
		virtual ~Window();

		WINDOW_MESSAGE_HANDLER(OnCreate);
		WINDOW_MESSAGE_HANDLER(OnClose);
		WINDOW_MESSAGE_HANDLER(OnQuit);
		WINDOW_MESSAGE_HANDLER(OnTimer);
		WINDOW_MESSAGE_HANDLER(OnNotify);
		WINDOW_MESSAGE_HANDLER(OnCommand);
		WINDOW_MESSAGE_HANDLER(OnDestroy);
		WINDOW_MESSAGE_HANDLER(OnDisplayChange);
		WINDOW_MESSAGE_HANDLER(OnMove);
		WINDOW_MESSAGE_HANDLER(OnMenuCommand);
		WINDOW_MESSAGE_HANDLER(OnPaint);
		WINDOW_MESSAGE_HANDLER(OnSize);
		WINDOW_MESSAGE_HANDLER(OnKeyDown);
		WINDOW_MESSAGE_HANDLER(OnKeyUp);
		WINDOW_MESSAGE_HANDLER(OnHScroll);
		WINDOW_MESSAGE_HANDLER(OnVScroll);
		WINDOW_MESSAGE_HANDLER(OnDropFiles);

		/**
		 * @brief Runs the window message loop.
		 * @return True if window was created and ran, false otherwise.
		 */
		virtual bool WINAPI RunWindow(HWND parent_window = HWND_DESKTOP, LPVOID param = NULL);

		/**
		 * @brief Window procedure.
		 * @param hWnd Window handle.
		 * @param Msg Message.
		 * @param wParam WPARAM.
		 * @param lParam LPARAM.
		 * @return Result of message processing.
		 */
		virtual LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		/**
		 * @brief Displays a message box.
		 * @param message Message text.
		 * @param title Title text.
		 * @param type Message box type.
		 * @return Result of the message box.
		 */
		virtual int MsgBox(LPCTSTR message, LPCTSTR title, UINT type);

		/**
		 * @brief Displays an information message box.
		 * @param title Title text.
		 * @param fmt Format string.
		 * @param ... Additional arguments.
		 * @return Result of the message box.
		 */
		virtual int MsgBoxInfo(LPCTSTR title, LPCTSTR fmt, ...);

		/**
		 * @brief Displays an error message box.
		 * @param title Title text.
		 * @param fmt Format string.
		 * @param ... Additional arguments.
		 * @return Result of the message box.
		 */
		virtual int MsgBoxError(LPCTSTR title, LPCTSTR fmt, ...);

		/**
		 * @brief Displays a warning message box.
		 * @param title Title text.
		 * @param fmt Format string.
		 * @param ... Additional arguments.
		 * @return Result of the message box.
		 */
		virtual int MsgBoxWarn(LPCTSTR title, LPCTSTR fmt, ...);

		/**
		 * @brief Shows the window.
		 */
		void Show();

		/**
		 * @brief Hides the window.
		 */
		void Hide();

		/**
		 * @brief Closes the window.
		 */
		void Close();

		/**
		 * @brief Quits the window.
		 * @param exit_code Exit code.
		 */
		void Quit(INT exit_code = 0);

		/**
		* @brief Creates a radio button group.
		* @return Pointer to the created radio button group.
		*/
		std::shared_ptr<Window::RadioButtonGroup> CreateRadioButtonGroup();

		// Create window controls
		std::shared_ptr<Button> CreateButton(LPCTSTR text, int x, int y, int width, int height);
		std::shared_ptr<CheckBox> CreateCheckBox(LPCTSTR text, int x, int y, int width, int height, BOOL initial_state = false);
		std::shared_ptr<ComboBox> CreateComboBox(int x, int y, int width, int height);
		std::shared_ptr<EditText> CreateEditText(int x, int y, int width, int height, LPCTSTR initial_text = _T(""));
		std::shared_ptr<ListBox> CreateListBox(int x, int y, int width, int height);
		std::shared_ptr<ListView> CreateListView(int x, int y, int width, int height);
		std::shared_ptr<TreeView> CreateTreeView(int x, int y, int width, int height);
		std::shared_ptr<TabControl> CreateTabControl(int x, int y, int width, int height);
		std::shared_ptr<ProgressBar> CreateProgressBar(int x, int y, int width, int height);
		std::shared_ptr<UpDownControl> CreateSpinControl(int x, int y, int width, int height);
		std::shared_ptr<RichEdit> CreateRichEdit(int x, int y, int width, int height, LPCTSTR initial_text = _T(""));
		std::shared_ptr<SysLink> CreateLinkControl(LPCTSTR text, int x, int y, int width, int height);

		/**
		 * @brief Registers a menu control callback.
		 * @param control_id Control ID.
		 * @param callback Callback for the menu item.
		 */
		void RegisterMenuCommand(UINT_PTR menu_id, MenuCallback callback)
		{
			if (callback)
				m_MenuCommandEvents[menu_id] = std::move(callback);
			else
				m_MenuCommandEvents.erase(menu_id);
		}

		/**
		 * @brief Adds a timer event.
		 * @tparam DC Callback type.
		 * @param timer_elapse Timer interval in milliseconds.
		 * @param callback Callback function.
		 */
		template<typename DC>
		void AddTimer(INT timer_elapse, DC callback)
		{
			const UINT_PTR timer_id = ++m_InternalTimerID + WINDOW_TIMER_OFFSET_START;
			if (::SetTimer(m_hWnd, timer_id, timer_elapse, NULL) != 0)
				m_TimerEvents[timer_id] = WINDOW_TIMER_REF(callback);
		}

        /**
        * @brief Gets a control by ID.
        * @tparam CtrlType Control type.
        * @param control_id Control ID.
        * @return Pointer to the control.
        */
		template<typename CtrlType = Control>
		inline std::shared_ptr<CtrlType> GetControl(UINT control_id)
		{
			auto it = std::find_if(m_Controls.begin(), m_Controls.end(), [control_id](const std::shared_ptr<Control>& control) {
				return control && control->GetID() == control_id;
			});

			if (it != m_Controls.end() && *it) {
				return std::dynamic_pointer_cast<CtrlType>(*it);
			}
			return nullptr;
		}

	private:
		void InitializeMessageEvents();
		void CleanupResources();

	protected:
		Class m_WindowClass; ///< Window class.
		int m_XPos, m_YPos, m_Width, m_Height; ///< Window position and size.
		std::tstring m_WindowName; ///< Window name.
		HMENU m_Menu; ///< Menu handle.
		HFONT m_Font; ///< Font handle.
		int m_MenuID; ///< Menu ID.
		DWORD m_Style, m_StyleEx; ///< Window styles.
		UINT m_ControlID = WM_USER+1; ///< Control ID index.
		UINT_PTR m_InternalTimerID = 0; ///< Internal timer ID.
		std::atomic_bool m_WindowRunning = false; ///< Window running flag.
		std::map<INT, WindowMessageCallback> m_MessageEvents; ///< Message events.
		std::map<UINT_PTR, TimerCallback> m_TimerEvents; ///< Timer events.
		std::map<UINT_PTR, MenuCallback> m_MenuCommandEvents; ///< Menu command events.
		std::vector<std::shared_ptr<Control>> m_Controls; ///< Controls container.
	};
}

#endif //__WINDOW_H__
