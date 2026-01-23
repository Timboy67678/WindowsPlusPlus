#ifndef __TRACK_BAR_H__
#define __TRACK_BAR_H__

namespace wpp
{
	class track_bar : public control {
	public:
		using control::control;

		track_bar& on_thumb_pos_changing(notify_callback callback) { register_notify_callback(TRBN_THUMBPOSCHANGING, std::move(callback)); return *this; }

		int get_line_size() const {
			return (int)SendMessage(m_handle, TBM_GETLINESIZE, 0, 0L);
		}

		int set_line_size(int nSize) {
			return (int)SendMessage(m_handle, TBM_SETLINESIZE, 0, nSize);
		}

		int get_page_size() const {
			return (int)SendMessage(m_handle, TBM_GETPAGESIZE, 0, 0L);
		}

		int set_page_size(int nSize) {
			return (int)SendMessage(m_handle, TBM_SETPAGESIZE, 0, nSize);
		}

		int get_range_min() const {
			return (int)SendMessage(m_handle, TBM_GETRANGEMIN, 0, 0L);
		}

		void set_range_min(int nMin, BOOL bRedraw = FALSE) {
			SendMessage(m_handle, TBM_SETRANGEMIN, bRedraw, nMin);
		}

		int get_range_max() const {
			return (int)SendMessage(m_handle, TBM_GETRANGEMAX, 0, 0L);
		}

		void set_range_max(int nMax, BOOL bRedraw = FALSE) {
			SendMessage(m_handle, TBM_SETRANGEMAX, bRedraw, nMax);
		}

		void get_range(int& nMin, int& nMax) const {
			nMin = get_range_min();
			nMax = get_range_max();
		}

		void set_range(int nMin, int nMax, BOOL bRedraw = TRUE) {
			SendMessage(m_handle, TBM_SETRANGE, bRedraw, MAKELPARAM(nMin, nMax));
		}

		int get_sel_start() const {
			return (int)SendMessage(m_handle, TBM_GETSELSTART, 0, 0L);
		}

		void set_sel_start(int nMin, BOOL bRedraw = FALSE) {
			SendMessage(m_handle, TBM_SETSELSTART, bRedraw, (LPARAM)nMin);
		}

		int get_sel_end() const {
			return (int)SendMessage(m_handle, TBM_GETSELEND, 0, 0L);
		}

		void set_sel_end(int nMax, BOOL bRedraw = FALSE) {
			SendMessage(m_handle, TBM_SETSELEND, bRedraw, (LPARAM)nMax);
		}

		void get_selection(int& nMin, int& nMax) const {
			nMin = get_sel_start();
			nMax = get_sel_end();
		}

		void set_selection(int nMin, int nMax, BOOL bRedraw = TRUE) {
			set_sel_start(nMin, FALSE);
			set_sel_end(nMax, bRedraw);
		}

		void get_channel_rect(LPRECT lprc) const {
			SendMessage(m_handle, TBM_GETCHANNELRECT, 0, (LPARAM)lprc);
		}

		void get_thumb_rect(LPRECT lprc) const {
			SendMessage(m_handle, TBM_GETTHUMBRECT, 0, (LPARAM)lprc);
		}

		int get_pos() const {
			return (int)SendMessage(m_handle, TBM_GETPOS, 0, 0L);
		}

		void set_pos(int nPos) {
			SendMessage(m_handle, TBM_SETPOS, TRUE, nPos);
		}

		UINT get_num_tics() const {
			return (UINT)SendMessage(m_handle, TBM_GETNUMTICS, 0, 0L);
		}

		DWORD* get_tic_array() const {
			return (DWORD*)SendMessage(m_handle, TBM_GETPTICS, 0, 0L);
		}

		int get_tic(int nTic) const {
			return (int)SendMessage(m_handle, TBM_GETTIC, nTic, 0L);
		}

		BOOL set_tic(int nTic) {
			return (BOOL)SendMessage(m_handle, TBM_SETTIC, 0, nTic);
		}

		int get_tic_pos(int nTic) const {
			return (int)SendMessage(m_handle, TBM_GETTICPOS, nTic, 0L);
		}

		void set_tic_freq(int nFreq) {
			SendMessage(m_handle, TBM_SETTICFREQ, nFreq, 0L);
		}

		int get_thumb_length() const {
			return (int)SendMessage(m_handle, TBM_GETTHUMBLENGTH, 0, 0L);
		}

		void set_thumb_length(int nLength) {
			SendMessage(m_handle, TBM_SETTHUMBLENGTH, nLength, 0L);
		}

		void set_sel(int nStart, int nEnd, BOOL bRedraw = TRUE) {
			SendMessage(m_handle, TBM_SETSEL, bRedraw, MAKELPARAM(nStart, nEnd));
		}

		HWND get_buddy(BOOL bLeft = TRUE) const {
			return  (HWND)SendMessage(m_handle, TBM_GETBUDDY, bLeft, 0L);
		}

		HWND set_buddy(HWND hWndBuddy, BOOL bLeft = TRUE) {
			return (HWND)SendMessage(m_handle, TBM_SETBUDDY, bLeft, (LPARAM)hWndBuddy);
		}

		tooltip get_tool_tips() const {
			return tooltip{ (HWND)SendMessage(m_handle, TBM_GETTOOLTIPS, 0, 0L) };
		}

		void set_tool_tips(tooltip hWndTT) {
			SendMessage(m_handle, TBM_SETTOOLTIPS, (WPARAM)hWndTT.get_handle(), 0L);
		}

		int set_tip_side(int nSide) {
			return (int)SendMessage(m_handle, TBM_SETTIPSIDE, nSide, 0L);
		}

		BOOL get_unicode_format() const {
			return (BOOL)SendMessage(m_handle, TBM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return (BOOL)SendMessage(m_handle, TBM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		void clear_sel(BOOL bRedraw = FALSE) {
			SendMessage(m_handle, TBM_CLEARSEL, bRedraw, 0L);
		}

		void verify_pos() {
			SendMessage(m_handle, TBM_SETPOS, FALSE, 0L);
		}

		void clear_tics(BOOL bRedraw = FALSE) {
			SendMessage(m_handle, TBM_CLEARTICS, bRedraw, 0L);
		}

		int get_min() const { return get_range_min(); }
		int get_max() const { return get_range_max(); }
		int get_range_span() const { return get_max() - get_min(); }

		BOOL is_at_min() const { return get_pos() == get_min(); }
		BOOL is_at_max() const { return get_pos() == get_max(); }

		BOOL is_in_range(int nPos) const {
			return nPos >= get_min() && nPos <= get_max();
		}

		int clamp_pos(int nPos) const {
			int min_val = get_min();
			int max_val = get_max();
			if (nPos < min_val) return min_val;
			if (nPos > max_val) return max_val;
			return nPos;
		}

		void increment(int nAmount = 1) {
			int newPos = get_pos() + nAmount;
			set_pos(clamp_pos(newPos));
		}

		void decrement(int nAmount = 1) {
			int newPos = get_pos() - nAmount;
			set_pos(clamp_pos(newPos));
		}

		void increment_line() { increment(get_line_size()); }
		void decrement_line() { decrement(get_line_size()); }
		void increment_page() { increment(get_page_size()); }
		void decrement_page() { decrement(get_page_size()); }

		void set_to_min() { set_pos(get_min()); }
		void set_to_max() { set_pos(get_max()); }
		void set_to_middle() { set_pos((get_min() + get_max()) / 2); }
		void reset() { set_to_min(); }

		double get_percent() const {
			int pos = get_pos();
			int min_val = get_min();
			int max_val = get_max();
			if (max_val == min_val) return 0.0;
			return (double)(pos - min_val) / (double)(max_val - min_val) * 100.0;
		}

		void set_percent(double percent) {
			if (percent < 0.0) percent = 0.0;
			if (percent > 100.0) percent = 100.0;
			int min_val = get_min();
			int max_val = get_max();
			int pos = min_val + (int)((max_val - min_val) * percent / 100.0);
			set_pos(pos);
		}

		BOOL is_horizontal() const {
			return (get_style() & TBS_VERT) == 0;
		}

		BOOL is_vertical() const {
			return (get_style() & TBS_VERT) == TBS_VERT;
		}

		BOOL has_selection() const {
			return (get_style() & TBS_ENABLESELRANGE) == TBS_ENABLESELRANGE;
		}

		BOOL has_tics() const {
			return (get_style() & TBS_NOTICKS) == 0;
		}

		BOOL is_autoticks() const {
			return (get_style() & TBS_AUTOTICKS) == TBS_AUTOTICKS;
		}

		int get_selection_length() const {
			return get_sel_end() - get_sel_start();
		}

		BOOL is_pos_in_selection() const {
			int pos = get_pos();
			return pos >= get_sel_start() && pos <= get_sel_end();
		}

		void center_selection() {
			int length = get_selection_length();
			int pos = get_pos();
			set_selection(pos - length / 2, pos + length / 2);
		}

		RECT get_channel_rect_ex() const {
			RECT rc = { 0 };
			get_channel_rect(&rc);
			return rc;
		}

		RECT get_thumb_rect_ex() const {
			RECT rc = { 0 };
			get_thumb_rect(&rc);
			return rc;
		}

		int get_thumb_width() const {
			RECT rc = get_thumb_rect_ex();
			return rc.right - rc.left;
		}

		int get_thumb_height() const {
			RECT rc = get_thumb_rect_ex();
			return rc.bottom - rc.top;
		}

		std::vector<int> get_tic_positions() const {
			std::vector<int> positions;
			UINT count = get_num_tics();
			for (UINT i = 0; i < count; i++) {
				positions.push_back(get_tic_pos((int)i));
			}
			return positions;
		}

		void set_tics_at_positions(const std::vector<int>& positions) {
			clear_tics(FALSE);
			for (int pos : positions) {
				set_tic(pos);
			}
		}

		void set_even_tics(int nCount) {
			clear_tics(FALSE);
			int min_val = get_min();
			int max_val = get_max();
			int span = max_val - min_val;
			
			if (nCount > 1) {
				for (int i = 0; i <= nCount; i++) {
					int pos = min_val + (span * i) / nCount;
					set_tic(pos);
				}
			}
		}

		void set_default_tics() {
			int freq = get_range_span() / 10;
			if (freq < 1) freq = 1;
			set_tic_freq(freq);
		}

		BOOL set_buddy_control(HWND hWnd, BOOL bLeft = TRUE) {
			return set_buddy(hWnd, bLeft) != NULL;
		}

		template<typename T>
		BOOL set_buddy_control(const T& control, BOOL bLeft = TRUE) {
			return set_buddy(control.get_handle(), bLeft) != NULL;
		}

		HWND get_left_buddy() const { return get_buddy(TRUE); }
		HWND get_right_buddy() const { return get_buddy(FALSE); }
		BOOL has_buddy() const { return get_buddy(TRUE) != NULL || get_buddy(FALSE) != NULL; }

		void enable_tooltips() {
			if (get_tool_tips().get_handle() == NULL) {
				HWND hTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, 
					WS_POPUP | TTS_ALWAYSTIP, 
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
					m_handle, NULL, GetModuleHandle(NULL), NULL);
				if (hTip) {
					set_tool_tips(tooltip(hTip));
				}
			}
		}

		void set_range_and_pos(int nMin, int nMax, int nPos, BOOL bRedraw = TRUE) {
			set_range(nMin, nMax, bRedraw);
			set_pos(clamp_pos(nPos));
		}

		void configure(int nMin, int nMax, int nPos, int nPageSize = 10, int nLineSize = 1) {
			set_range(nMin, nMax, FALSE);
			set_page_size(nPageSize);
			set_line_size(nLineSize);
			set_pos(clamp_pos(nPos));
		}

		int get_remaining() const {
			return get_max() - get_pos();
		}

		int get_completed() const {
			return get_pos() - get_min();
		}

		BOOL can_increment() const { return !is_at_max(); }
		BOOL can_decrement() const { return !is_at_min(); }

		void set_range_symmetric(int nRadius) {
			set_range(-nRadius, nRadius);
		}

		void snap_to_tic() {
			if (!has_tics()) return;
			
			int pos = get_pos();
			UINT count = get_num_tics();
			int closest = get_tic(0);
			int minDist = abs(pos - closest);
			
			for (UINT i = 1; i < count; i++) {
				int tic = get_tic((int)i);
				int dist = abs(pos - tic);
				if (dist < minDist) {
					minDist = dist;
					closest = tic;
				}
			}
			set_pos(closest);
		}

		int find_nearest_tic() const {
			if (!has_tics()) return -1;
			
			int pos = get_pos();
			UINT count = get_num_tics();
			int closest = get_tic(0);
			int minDist = abs(pos - closest);
			
			for (UINT i = 1; i < count; i++) {
				int tic = get_tic((int)i);
				int dist = abs(pos - tic);
				if (dist < minDist) {
					minDist = dist;
					closest = tic;
				}
			}
			return closest;
		}

		void set_selection_around_pos(int nRange) {
			int pos = get_pos();
			int min_val = get_min();
			int max_val = get_max();
			int start = __max(min_val, pos - nRange);
			int end = __min(max_val, pos + nRange);
			set_selection(start, end);
		}

		BOOL has_tooltips() const {
			return get_tool_tips().get_handle() != NULL;
		}

		void set_standard_range() { set_range(0, 100); }

		void animate_to(int nTargetPos, int nSteps = 10, int nDelayMs = 50) {
			int current = get_pos();
			int diff = nTargetPos - current;
			int step = diff / nSteps;
			
			for (int i = 0; i < nSteps; i++) {
				current += step;
				set_pos(current);
				Sleep(nDelayMs);
			}
			set_pos(nTargetPos);
		}
	};
}

#endif //__TRACK_BAR_H__