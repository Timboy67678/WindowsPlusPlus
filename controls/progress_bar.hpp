#ifndef __PROGRESS_BAR_HPP__
#define __PROGRESS_BAR_HPP__

namespace wpp
{
	class progress_bar : public control {
	public:
		using control::control;

		DWORD set_range(int nLower, int nUpper) {
			return (DWORD)SendMessage(m_handle, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
		}

		int set_pos(int nPos) {
			return (int)(short)LOWORD(SendMessage(m_handle, PBM_SETPOS, nPos, 0L));
		}

		int offset_pos(int nPos) {
			return (int)(short)LOWORD(SendMessage(m_handle, PBM_DELTAPOS, nPos, 0L));
		}

		int set_step(int nStep) {
			return (int)(short)LOWORD(SendMessage(m_handle, PBM_SETSTEP, nStep, 0L));
		}

		UINT get_pos() const {
			return (UINT)SendMessage(m_handle, PBM_GETPOS, 0, 0L);
		}

		void get_range(PPBRANGE pPBRange) const {
			SendMessage(m_handle, PBM_GETRANGE, TRUE, (LPARAM)pPBRange);
		}

		void get_range(int& nLower, int& nUpper) const {
			PBRANGE range = { 0 };
			SendMessage(m_handle, PBM_GETRANGE, TRUE, (LPARAM)&range);
			nLower = range.iLow;
			nUpper = range.iHigh;
		}

		int get_range_limit(BOOL bLowLimit) const {
			return (int)SendMessage(m_handle, PBM_GETRANGE, bLowLimit, 0L);
		}

		DWORD set_range32(int nMin, int nMax) {
			return (DWORD)SendMessage(m_handle, PBM_SETRANGE32, nMin, nMax);
		}

		COLORREF set_bar_color(COLORREF clr) {
			return (COLORREF)SendMessage(m_handle, PBM_SETBARCOLOR, 0, (LPARAM)clr);
		}

		COLORREF set_bk_color(COLORREF clr) {
			return (COLORREF)SendMessage(m_handle, PBM_SETBKCOLOR, 0, (LPARAM)clr);
		}

		BOOL set_marquee(BOOL bMarquee, UINT uUpdateTime = 0U) {
			if (bMarquee && (get_style() & PBS_MARQUEE) == 0) add_style(PBS_MARQUEE);
			return (BOOL)SendMessage(m_handle, PBM_SETMARQUEE, (WPARAM)bMarquee, (LPARAM)uUpdateTime);
		}

		int get_step() const {
			return (int)SendMessage(m_handle, PBM_GETSTEP, 0, 0L);
		}

		COLORREF get_bk_color() const {
			return (COLORREF)SendMessage(m_handle, PBM_GETBKCOLOR, 0, 0L);
		}

		COLORREF get_bar_color() const {
			return (COLORREF)SendMessage(m_handle, PBM_GETBARCOLOR, 0, 0L);
		}

		int get_state() const {
			return (int)SendMessage(m_handle, PBM_GETSTATE, 0, 0L);
		}

		int set_state(int nState) {
			return (int)SendMessage(m_handle, PBM_SETSTATE, nState, 0L);
		}

		int step_it() {
			return (int)(short)LOWORD(SendMessage(m_handle, PBM_STEPIT, 0, 0L));
		}

		int get_min() const {
			int nLower = 0, nUpper = 0;
			get_range(nLower, nUpper);
			return nLower;
		}

		int get_max() const {
			int nLower = 0, nUpper = 0;
			get_range(nLower, nUpper);
			return nUpper;
		}

		void set_min(int nMin) {
			int nLower = 0, nUpper = 0;
			get_range(nLower, nUpper);
			set_range32(nMin, nUpper);
		}

		void set_max(int nMax) {
			int nLower = 0, nUpper = 0;
			get_range(nLower, nUpper);
			set_range32(nLower, nMax);
		}

		int get_range_span() const {
			int nMin = get_min();
			int nMax = get_max();
			return nMax - nMin;
		}

		double get_percent() const {
			int nPos = (int)get_pos();
			int nMin = get_min();
			int nMax = get_max();
			if (nMax == nMin) return 0.0;
			return (double)(nPos - nMin) / (double)(nMax - nMin) * 100.0;
		}

		void set_percent(double percent) {
			if (percent < 0.0) percent = 0.0;
			if (percent > 100.0) percent = 100.0;
			int nMin = get_min();
			int nMax = get_max();
			int nPos = nMin + (int)((nMax - nMin) * percent / 100.0);
			set_pos(nPos);
		}

		void advance(int nAmount) {
			int nPos = (int)get_pos();
			set_pos(nPos + nAmount);
		}

		void reset() {
			set_pos(get_min());
		}

		void complete() {
			set_pos(get_max());
		}

		BOOL is_complete() const {
			return (int)get_pos() >= get_max();
		}

		BOOL is_empty() const {
			return (int)get_pos() <= get_min();
		}

		BOOL is_in_progress() const {
			int nPos = (int)get_pos();
			return nPos > get_min() && nPos < get_max();
		}

		void set_normal_state() {
			set_state(PBST_NORMAL);
		}

		void set_error_state() {
			set_state(PBST_ERROR);
		}

		void set_paused_state() {
			set_state(PBST_PAUSED);
		}

		BOOL is_normal_state() const {
			return get_state() == PBST_NORMAL;
		}

		BOOL is_error_state() const {
			return get_state() == PBST_ERROR;
		}

		BOOL is_paused_state() const {
			return get_state() == PBST_PAUSED;
		}

		void set_range_0_100() {
			set_range32(0, 100);
		}

		void set_range_from_count(int nTotalItems) {
			set_range32(0, nTotalItems);
		}

		void increment_by_step() {
			step_it();
		}

		void increment() {
			advance(1);
		}

		void decrement() {
			advance(-1);
		}

		BOOL set_smooth_style() {
			return add_style(PBS_SMOOTH);
		}

		BOOL set_vertical_style() {
			return add_style(PBS_VERTICAL);
		}

		BOOL is_marquee_active() const {
			return (get_style() & PBS_MARQUEE) == PBS_MARQUEE;
		}

		void start_marquee(UINT uUpdateTime = 30U) {
			set_marquee(TRUE, uUpdateTime);
		}

		void stop_marquee() {
			set_marquee(FALSE, 0);
		}

		void set_colors(COLORREF barColor, COLORREF bkColor) {
			set_bar_color(barColor);
			set_bk_color(bkColor);
		}

		void set_green_bar() {
			set_bar_color(RGB(0, 255, 0));
		}

		void set_red_bar() {
			set_bar_color(RGB(255, 0, 0));
		}

		void set_yellow_bar() {
			set_bar_color(RGB(255, 255, 0));
		}

		void set_blue_bar() {
			set_bar_color(RGB(0, 120, 215));
		}

		int remaining() const {
			return get_max() - (int)get_pos();
		}

		int completed() const {
			return (int)get_pos() - get_min();
		}
	};
}

#endif //__PROGRESS_BAR_HPP__
