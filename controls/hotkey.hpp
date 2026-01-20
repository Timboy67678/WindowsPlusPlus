#ifndef __HOTKEY_HPP__
#define __HOTKEY_HPP__

namespace wpp
{
	class hotkey : public control {
	public:
		// Modifier flags
		static constexpr WORD HK_MOD_SHIFT = HOTKEYF_SHIFT;
		static constexpr WORD HK_MOD_CONTROL = HOTKEYF_CONTROL;
		static constexpr WORD HK_MOD_ALT = HOTKEYF_ALT;
		static constexpr WORD HK_MOD_EXT = HOTKEYF_EXT;

		// Invalid combination flags
		static constexpr WORD INVALID_NONE = HKCOMB_NONE;
		static constexpr WORD INVALID_A = HKCOMB_A;
		static constexpr WORD INVALID_C = HKCOMB_C;
		static constexpr WORD INVALID_CA = HKCOMB_CA;
		static constexpr WORD INVALID_S = HKCOMB_S;
		static constexpr WORD INVALID_SA = HKCOMB_SA;
		static constexpr WORD INVALID_SC = HKCOMB_SC;
		static constexpr WORD INVALID_SCA = HKCOMB_SCA;

		using control::control;

		DWORD get_hot_key() const {
			return (DWORD)SendMessage(m_handle, HKM_GETHOTKEY, 0, 0L);
		}

		void get_hot_key(WORD& wVirtualKeyCode, WORD& wModifiers) const {
			DWORD dw = (DWORD)SendMessage(m_handle, HKM_GETHOTKEY, 0, 0L);
			wVirtualKeyCode = LOBYTE(LOWORD(dw));
			wModifiers = HIBYTE(LOWORD(dw));
		}

		WORD get_virtual_key() const {
			return LOBYTE(LOWORD(get_hot_key()));
		}

		WORD get_modifiers() const {
			return HIBYTE(LOWORD(get_hot_key()));
		}

		void set_hot_key(WORD wVirtualKeyCode, WORD wModifiers = 0) {
			SendMessage(m_handle, HKM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0L);
		}

		void set_rules(WORD wInvalidComb, WORD wModifiers = 0) {
			SendMessage(m_handle, HKM_SETRULES, wInvalidComb, MAKELPARAM(wModifiers, 0));
		}

		void clear() {
			set_hot_key(0, 0);
		}

		bool is_set() const {
			return get_hot_key() != 0;
		}

		bool has_shift() const {
			return (get_modifiers() & HK_MOD_SHIFT) != 0;
		}

		bool has_control() const {
			return (get_modifiers() & HK_MOD_CONTROL) != 0;
		}

		bool has_alt() const {
			return (get_modifiers() & HK_MOD_ALT) != 0;
		}

		bool has_extended() const {
			return (get_modifiers() & HK_MOD_EXT) != 0;
		}

		std::tstring to_string() const {
			WORD vk = get_virtual_key();
			WORD mods = get_modifiers();

			if (vk == 0) {
				return _T("None");
			}

			std::tstring result;

			if (mods & HK_MOD_CONTROL) {
				result += _T("Ctrl+");
			}
			if (mods & HK_MOD_ALT) {
				result += _T("Alt+");
			}
			if (mods & HK_MOD_SHIFT) {
				result += _T("Shift+");
			}
			if (mods & HK_MOD_EXT) {
				result += _T("Ext+");
			}

			// Get key name
			TCHAR keyName[50] = {};
			LONG scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
			
			if (scanCode != 0) {
				// Extended keys need the extended flag set
				if ((vk >= VK_PRIOR && vk <= VK_DOWN) ||
					vk == VK_INSERT || vk == VK_DELETE ||
					vk == VK_NUMLOCK || vk == VK_DIVIDE) {
					scanCode |= 0x100;
				}

				if (GetKeyNameText(scanCode << 16, keyName, 50) > 0) {
					result += keyName;
				} else {
					result += _T("VK_") + std::to_tstring(vk);
				}
			} else {
				result += _T("VK_") + std::to_tstring(vk);
			}

			return result;
		}

		operator std::tstring() const {
			return to_string();
		}
	};
}

#endif //__HOTKEY_HPP__