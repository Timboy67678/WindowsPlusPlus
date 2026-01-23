#ifndef __SCROLL_BAR_HPP__
#define __SCROLL_BAR_HPP__

namespace wpp
{
	class scroll_bar : public control {
	public:
		using control::control;

		int get_scroll_pos() const {
			return ::GetScrollPos(m_handle, SB_CTL);
		}

		int set_scroll_pos(int nPos, BOOL bRedraw = TRUE) {
			return ::SetScrollPos(m_handle, SB_CTL, nPos, bRedraw);
		}

		void get_scroll_range(LPINT lpMinPos, LPINT lpMaxPos) const {
			::GetScrollRange(m_handle, SB_CTL, lpMinPos, lpMaxPos);
		}

		void set_scroll_range(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE) {
			::SetScrollRange(m_handle, SB_CTL, nMinPos, nMaxPos, bRedraw);
		}

		BOOL get_scroll_info(LPSCROLLINFO lpScrollInfo) const {
			return ::GetScrollInfo(m_handle, SB_CTL, lpScrollInfo);
		}

		int set_scroll_info(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE) {
			return ::SetScrollInfo(m_handle, SB_CTL, lpScrollInfo, bRedraw);
		}

		int get_scroll_limit() const {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE };
			::GetScrollInfo(m_handle, SB_CTL, &info);
			if (info.nPage > 1)
				info.nMax -= info.nPage - 1;

			return info.nMax;
		}

		BOOL get_info(PSCROLLBARINFO pScrollBarInfo) const {
			return (BOOL)SendMessage(m_handle, SBM_GETSCROLLBARINFO, 0, (LPARAM)pScrollBarInfo);
		}

		void show_scrollbar(BOOL bShow = TRUE) {
			::ShowScrollBar(m_handle, SB_CTL, bShow);
		}

		BOOL enable_scrollbar(BOOL bEnable) {
			return ::EnableScrollBar(m_handle, SB_CTL, bEnable ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
		}

		int get_min_pos() const {
			int minPos = 0, maxPos = 0;
			get_scroll_range(&minPos, &maxPos);
			return minPos;
		}

		int get_max_pos() const {
			int minPos = 0, maxPos = 0;
			get_scroll_range(&minPos, &maxPos);
			return maxPos;
		}

		void get_range(int& nMin, int& nMax) const {
			get_scroll_range(&nMin, &nMax);
		}

		void set_range(int nMin, int nMax, BOOL bRedraw = TRUE) {
			set_scroll_range(nMin, nMax, bRedraw);
		}

		int get_range_span() const {
			int minPos = 0, maxPos = 0;
			get_scroll_range(&minPos, &maxPos);
			return maxPos - minPos;
		}

		UINT get_page_size() const {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_PAGE };
			::GetScrollInfo(m_handle, SB_CTL, &info);
			return info.nPage;
		}

		void set_page_size(UINT nPage, BOOL bRedraw = TRUE) {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_PAGE };
			info.nPage = nPage;
			set_scroll_info(&info, bRedraw);
		}

		int get_track_pos() const {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_TRACKPOS };
			::GetScrollInfo(m_handle, SB_CTL, &info);
			return info.nTrackPos;
		}

		void get_all_info(int& nPos, int& nMin, int& nMax, UINT& nPage) const {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_ALL };
			::GetScrollInfo(m_handle, SB_CTL, &info);
			nPos = info.nPos;
			nMin = info.nMin;
			nMax = info.nMax;
			nPage = info.nPage;
		}

		void set_all_info(int nPos, int nMin, int nMax, UINT nPage, BOOL bRedraw = TRUE) {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_ALL };
			info.nPos = nPos;
			info.nMin = nMin;
			info.nMax = nMax;
			info.nPage = nPage;
			set_scroll_info(&info, bRedraw);
		}

		BOOL is_at_min() const {
			return get_scroll_pos() == get_min_pos();
		}

		BOOL is_at_max() const {
			int pos = get_scroll_pos();
			int max = get_max_pos();
			UINT page = get_page_size();
			return pos >= (max - (int)page + 1);
		}

		BOOL is_in_range(int nPos) const {
			int minPos = get_min_pos();
			int maxPos = get_max_pos();
			return nPos >= minPos && nPos <= maxPos;
		}

		int clamp_pos(int nPos) const {
			int minPos = get_min_pos();
			int maxPos = get_max_pos();
			UINT page = get_page_size();
			int actualMax = maxPos - (int)page + 1;
			
			if (nPos < minPos) return minPos;
			if (nPos > actualMax) return actualMax;
			return nPos;
		}

		int scroll_by(int nDelta, BOOL bRedraw = TRUE) {
			int currentPos = get_scroll_pos();
			int newPos = clamp_pos(currentPos + nDelta);
			return set_scroll_pos(newPos, bRedraw);
		}

		int scroll_up(int nAmount = 1, BOOL bRedraw = TRUE) {
			return scroll_by(-nAmount, bRedraw);
		}

		int scroll_down(int nAmount = 1, BOOL bRedraw = TRUE) {
			return scroll_by(nAmount, bRedraw);
		}

		int page_up(BOOL bRedraw = TRUE) {
			int page = (int)get_page_size();
			return scroll_up(page > 0 ? page : 10, bRedraw);
		}

		int page_down(BOOL bRedraw = TRUE) {
			int page = (int)get_page_size();
			return scroll_down(page > 0 ? page : 10, bRedraw);
		}

		int scroll_to_top(BOOL bRedraw = TRUE) {
			return set_scroll_pos(get_min_pos(), bRedraw);
		}

		int scroll_to_bottom(BOOL bRedraw = TRUE) {
			int maxPos = get_max_pos();
			UINT page = get_page_size();
			return set_scroll_pos(maxPos - (int)page + 1, bRedraw);
		}

		double get_percent() const {
			int pos = get_scroll_pos();
			int minPos = get_min_pos();
			int maxPos = get_max_pos();
			UINT page = get_page_size();
			int actualMax = maxPos - (int)page + 1;
			
			if (actualMax == minPos) return 0.0;
			return (double)(pos - minPos) / (double)(actualMax - minPos) * 100.0;
		}

		void set_percent(double percent, BOOL bRedraw = TRUE) {
			if (percent < 0.0) percent = 0.0;
			if (percent > 100.0) percent = 100.0;
			
			int minPos = get_min_pos();
			int maxPos = get_max_pos();
			UINT page = get_page_size();
			int actualMax = maxPos - (int)page + 1;
			
			int pos = minPos + (int)((actualMax - minPos) * percent / 100.0);
			set_scroll_pos(pos, bRedraw);
		}

		BOOL is_horizontal() const {
			return (get_style() & SBS_VERT) == 0;
		}

		BOOL is_vertical() const {
			return (get_style() & SBS_VERT) == SBS_VERT;
		}

		BOOL is_enabled() const {
			SCROLLBARINFO info = { sizeof(SCROLLBARINFO) };
			if (get_info(&info)) {
				return (info.rgstate[0] & STATE_SYSTEM_UNAVAILABLE) == 0;
			}
			return TRUE;
		}

		BOOL is_top_arrow_enabled() const {
			SCROLLBARINFO info = { sizeof(SCROLLBARINFO) };
			if (get_info(&info)) {
				return (info.rgstate[0] & STATE_SYSTEM_UNAVAILABLE) == 0 &&
				       (info.rgstate[0] & STATE_SYSTEM_PRESSED) == 0;
			}
			return TRUE;
		}

		BOOL is_bottom_arrow_enabled() const {
			SCROLLBARINFO info = { sizeof(SCROLLBARINFO) };
			if (get_info(&info)) {
				return (info.rgstate[1] & STATE_SYSTEM_UNAVAILABLE) == 0 &&
				       (info.rgstate[1] & STATE_SYSTEM_PRESSED) == 0;
			}
			return TRUE;
		}

		void disable_arrows_at_limits() {
			if (is_at_min()) {
				enable_scrollbar(ESB_DISABLE_UP);
			} else if (is_at_max()) {
				enable_scrollbar(ESB_DISABLE_DOWN);
			} else {
				enable_scrollbar(ESB_ENABLE_BOTH);
			}
		}

		void disable_top_arrow() {
			enable_scrollbar(is_horizontal() ? ESB_DISABLE_LEFT : ESB_DISABLE_UP);
		}

		void disable_bottom_arrow() {
			enable_scrollbar(is_horizontal() ? ESB_DISABLE_RIGHT : ESB_DISABLE_DOWN);
		}

		void disable_both_arrows() {
			enable_scrollbar(ESB_DISABLE_BOTH);
		}

		void enable_both_arrows() {
			enable_scrollbar(ESB_ENABLE_BOTH);
		}

		void hide() {
			show_scrollbar(FALSE);
		}

		void show() {
			show_scrollbar(TRUE);
		}

		BOOL is_visible() const {
			return IsWindowVisible(m_handle);
		}

		int get_thumb_size() const {
			SCROLLBARINFO info = { sizeof(SCROLLBARINFO) };
			if (get_info(&info)) {
				if (is_horizontal()) {
					return info.xyThumbBottom - info.xyThumbTop;
				} else {
					return info.xyThumbBottom - info.xyThumbTop;
				}
			}
			return 0;
		}

		RECT get_thumb_rect() const {
			SCROLLBARINFO info = { sizeof(SCROLLBARINFO) };
			RECT rc = { 0 };
			if (get_info(&info)) {
				rc.left = info.rcScrollBar.left;
				rc.top = info.rcScrollBar.top + info.xyThumbTop;
				rc.right = info.rcScrollBar.right;
				rc.bottom = info.rcScrollBar.top + info.xyThumbBottom;
			}
			return rc;
		}

		void set_range_page_pos(int nMin, int nMax, UINT nPage, int nPos, BOOL bRedraw = TRUE) {
			SCROLLINFO info = { sizeof(SCROLLINFO), SIF_ALL };
			info.nMin = nMin;
			info.nMax = nMax;
			info.nPage = nPage;
			info.nPos = clamp_pos(nPos);
			set_scroll_info(&info, bRedraw);
		}

		void set_proportional(int nTotal, int nVisible, int nPos, BOOL bRedraw = TRUE) {
			set_range_page_pos(0, nTotal - 1, nVisible, nPos, bRedraw);
		}

		int get_effective_max() const {
			int maxPos = get_max_pos();
			UINT page = get_page_size();
			return maxPos - (int)page + 1;
		}

		int get_remaining() const {
			return get_effective_max() - get_scroll_pos();
		}

		int get_completed() const {
			return get_scroll_pos() - get_min_pos();
		}

		BOOL can_scroll_up() const {
			return !is_at_min();
		}

		BOOL can_scroll_down() const {
			return !is_at_max();
		}

		void reset() {
			set_scroll_pos(get_min_pos(), TRUE);
		}
	};
}

#endif //__SCROLL_BAR_HPP__