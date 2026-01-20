#ifndef __UPDOWN_CONTROL_HPP__
#define __UPDOWN_CONTROL_HPP__

namespace wpp
{
	class up_down_control : public control {
	public:
		using control::control;

		UINT get_accel(int nAccel, UDACCEL* pAccel) const {
			return (UINT)LOWORD(SendMessage(m_handle, UDM_GETACCEL, nAccel, (LPARAM)pAccel));
		}

		BOOL set_accel(int nAccel, UDACCEL* pAccel) {
			return (BOOL)LOWORD(SendMessage(m_handle, UDM_SETACCEL, nAccel, (LPARAM)pAccel));
		}

		UINT get_base() const {
			return (UINT)LOWORD(SendMessage(m_handle, UDM_GETBASE, 0, 0L));
		}

		int set_base(int nBase) {
			return (int)SendMessage(m_handle, UDM_SETBASE, nBase, 0L);
		}

		HWND get_buddy() const {
			return (HWND)SendMessage(m_handle, UDM_GETBUDDY, 0, 0L);
		}

		HWND set_buddy(HWND hWndBuddy) {
			return (HWND)SendMessage(m_handle, UDM_SETBUDDY, (WPARAM)hWndBuddy, 0L);
		}

		int get_pos(LPBOOL lpbError = NULL) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, UDM_GETPOS, 0, 0L);
			if (lpbError != NULL) *lpbError = (HIWORD(dwRet) != 0) ? TRUE : FALSE;
			return (int)(short)LOWORD(dwRet);
		}

		int set_pos(int nPos) {
			return (int)(short)LOWORD(SendMessage(m_handle, UDM_SETPOS, 0, MAKELPARAM(nPos, 0)));
		}

		DWORD get_range() const {
			return (DWORD)SendMessage(m_handle, UDM_GETRANGE, 0, 0L);
		}

		void get_range(int& nLower, int& nUpper) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, UDM_GETRANGE, 0, 0L);
			nLower = (int)(short)HIWORD(dwRet);
			nUpper = (int)(short)LOWORD(dwRet);
		}

		void set_range(int nLower, int nUpper) {
			SendMessage(m_handle, UDM_SETRANGE, 0, MAKELPARAM(nUpper, nLower));
		}

		void set_range32(int nLower, int nUpper) {
			SendMessage(m_handle, UDM_SETRANGE32, nLower, nUpper);
		}

		void get_range32(int& nLower, int& nUpper) const {
			SendMessage(m_handle, UDM_GETRANGE32, (WPARAM)&nLower, (LPARAM)&nUpper);
		}

		BOOL get_unicode_format() const {
			return (BOOL)SendMessage(m_handle, UDM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return (BOOL)SendMessage(m_handle, UDM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		int get_pos32(LPBOOL lpbError = NULL) const {
			return (int)SendMessage(m_handle, UDM_GETPOS32, 0, (LPARAM)lpbError);
		}

		int set_pos32(int nPos) {
			return (int)SendMessage(m_handle, UDM_SETPOS32, 0, (LPARAM)nPos);
		}

		int get_min() const {
			int nLower = 0, nUpper = 0;
			get_range32(nLower, nUpper);
			return nLower;
		}

		int get_max() const {
			int nLower = 0, nUpper = 0;
			get_range32(nLower, nUpper);
			return nUpper;
		}

		void set_min(int nMin) {
			int nLower = 0, nUpper = 0;
			get_range32(nLower, nUpper);
			set_range32(nMin, nUpper);
		}

		void set_max(int nMax) {
			int nLower = 0, nUpper = 0;
			get_range32(nLower, nUpper);
			set_range32(nLower, nMax);
		}

		int get_valid_pos(BOOL* pbError = NULL) const {
			BOOL bError = FALSE;
			int nPos = get_pos32(&bError);
			if (pbError) *pbError = bError;
			return bError ? get_min() : nPos;
		}

		BOOL increment(int nAmount = 1) {
			int nPos = get_pos32(NULL);
			int nMax = get_max();
			int nNewPos = nPos + nAmount;
			if (nNewPos > nMax) nNewPos = nMax;
			return set_pos32(nNewPos) != nPos;
		}

		BOOL decrement(int nAmount = 1) {
			int nPos = get_pos32(NULL);
			int nMin = get_min();
			int nNewPos = nPos - nAmount;
			if (nNewPos < nMin) nNewPos = nMin;
			return set_pos32(nNewPos) != nPos;
		}

		BOOL is_at_min() const {
			return get_pos32(NULL) == get_min();
		}

		BOOL is_at_max() const {
			return get_pos32(NULL) == get_max();
		}

		BOOL is_in_range(int nValue) const {
			int nMin = get_min();
			int nMax = get_max();
			return nValue >= nMin && nValue <= nMax;
		}

		int clamp(int nValue) const {
			int nMin = get_min();
			int nMax = get_max();
			if (nValue < nMin) return nMin;
			if (nValue > nMax) return nMax;
			return nValue;
		}

		void set_to_min() {
			set_pos32(get_min());
		}

		void set_to_max() {
			set_pos32(get_max());
		}

		void reset() {
			set_to_min();
		}

		void set_to_middle() {
			int nMin = get_min();
			int nMax = get_max();
			set_pos32((nMin + nMax) / 2);
		}

		int get_range_span() const {
			int nMin = get_min();
			int nMax = get_max();
			return nMax - nMin;
		}

		double get_percent() const {
			int nPos = get_pos32(NULL);
			int nMin = get_min();
			int nMax = get_max();
			if (nMax == nMin) return 0.0;
			return (double)(nPos - nMin) / (double)(nMax - nMin) * 100.0;
		}

		void set_percent(double percent) {
			int nMin = get_min();
			int nMax = get_max();
			if (percent < 0.0) percent = 0.0;
			if (percent > 100.0) percent = 100.0;
			int nPos = nMin + (int)((nMax - nMin) * percent / 100.0);
			set_pos32(nPos);
		}

		BOOL set_decimal_base() {
			return set_base(10) != 0;
		}

		BOOL set_hex_base() {
			return set_base(16) != 0;
		}

		BOOL is_decimal_base() const {
			return get_base() == 10;
		}

		BOOL is_hex_base() const {
			return get_base() == 16;
		}

		template<typename T = control>
		BOOL set_buddy_control(const T& buddy) {
			return set_buddy(buddy.get_handle()) != NULL;
		}

		int add_accelerator(UINT nSec, UINT nInc) {
			UDACCEL accel = { nSec, nInc };
			UDACCEL accels[32] = { 0 };
			int nCount = get_accel(32, accels);
			if (nCount >= 0 && nCount < 32) {
				accels[nCount] = accel;
				return set_accel(nCount + 1, accels) ? nCount + 1 : nCount;
			}
			return nCount;
		}

		void clear_accelerators() {
			UDACCEL accel = { 0, 1 };
			set_accel(1, &accel);
		}

		void set_default_accelerators() {
			UDACCEL accels[3] = {
				{ 0, 1 },
				{ 2, 5 },
				{ 5, 10 }
			};
			set_accel(3, accels);
		}
	};
}

#endif //__UPDOWN_CONTROL_HPP__
