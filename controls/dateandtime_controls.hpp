#ifndef __DATEANDTIME_CONTROLS_H__
#define __DATEANDTIME_CONTROLS_H__

#include "common.hpp"
#include <utility>

namespace wpp
{
	class month_calendar : public control {
	public:
		using control::control;

		COLORREF get_color(int nColorType) const {
			return (COLORREF)SendMessage(m_handle, MCM_GETCOLOR, nColorType, 0L);
		}

		COLORREF set_color(int nColorType, COLORREF clr) {
			return (COLORREF)SendMessage(m_handle, MCM_SETCOLOR, nColorType, clr);
		}

		BOOL get_cur_sel(LPSYSTEMTIME lpSysTime) const {
			return (BOOL)SendMessage(m_handle, MCM_GETCURSEL, 0, (LPARAM)lpSysTime);
		}

		BOOL set_cur_sel(LPSYSTEMTIME lpSysTime) {
			return (BOOL)SendMessage(m_handle, MCM_SETCURSEL, 0, (LPARAM)lpSysTime);
		}

		int get_first_day_of_week(BOOL* pbLocaleVal = NULL) const {
			DWORD dwRet = (DWORD)SendMessage(m_handle, MCM_GETFIRSTDAYOFWEEK, 0, 0L);
			if (pbLocaleVal != NULL) *pbLocaleVal = (BOOL)HIWORD(dwRet);
			return (int)(short)LOWORD(dwRet);
		}

		int set_first_day_of_week(int nDay, BOOL* pbLocaleVal = NULL) {
			DWORD dwRet = (DWORD)SendMessage(m_handle, MCM_SETFIRSTDAYOFWEEK, 0, nDay);
			if (pbLocaleVal != NULL) *pbLocaleVal = (BOOL)HIWORD(dwRet);
			return (int)(short)LOWORD(dwRet);
		}

		int get_max_sel_count() const {
			return (int)SendMessage(m_handle, MCM_GETMAXSELCOUNT, 0, 0L);
		}

		BOOL set_max_sel_count(int nMax) {
			return (BOOL)SendMessage(m_handle, MCM_SETMAXSELCOUNT, nMax, 0L);
		}

		int get_month_delta() const {
			return (int)SendMessage(m_handle, MCM_GETMONTHDELTA, 0, 0L);
		}

		int set_month_delta(int nDelta) {
			return (int)SendMessage(m_handle, MCM_SETMONTHDELTA, nDelta, 0L);
		}

		DWORD get_range(LPSYSTEMTIME lprgSysTimeArray) const {
			return (DWORD)SendMessage(m_handle, MCM_GETRANGE, 0, (LPARAM)lprgSysTimeArray);
		}

		BOOL set_range(DWORD dwFlags, LPSYSTEMTIME lprgSysTimeArray) {
			return (BOOL)SendMessage(m_handle, MCM_SETRANGE, dwFlags, (LPARAM)lprgSysTimeArray);
		}

		BOOL get_sel_range(LPSYSTEMTIME lprgSysTimeArray) const {
			return (BOOL)SendMessage(m_handle, MCM_GETSELRANGE, 0, (LPARAM)lprgSysTimeArray);
		}

		BOOL set_sel_range(LPSYSTEMTIME lprgSysTimeArray) {
			return (BOOL)SendMessage(m_handle, MCM_SETSELRANGE, 0, (LPARAM)lprgSysTimeArray);
		}

		BOOL get_today(LPSYSTEMTIME lpSysTime) const {
			return (BOOL)SendMessage(m_handle, MCM_GETTODAY, 0, (LPARAM)lpSysTime);
		}

		void set_today(LPSYSTEMTIME lpSysTime) {
			SendMessage(m_handle, MCM_SETTODAY, 0, (LPARAM)lpSysTime);
		}

		BOOL get_min_req_rect(LPRECT lpRectInfo) const {
			return (BOOL)SendMessage(m_handle, MCM_GETMINREQRECT, 0, (LPARAM)lpRectInfo);
		}

		int get_max_today_width() const {
			return (int)SendMessage(m_handle, MCM_GETMAXTODAYWIDTH, 0, 0L);
		}

		BOOL get_unicode_format() const {
			return (BOOL)SendMessage(m_handle, MCM_GETUNICODEFORMAT, 0, 0L);
		}

		BOOL set_unicode_format(BOOL bUnicode = TRUE) {
			return (BOOL)SendMessage(m_handle, MCM_SETUNICODEFORMAT, bUnicode, 0L);
		}

		DWORD get_current_view() const {
			return (DWORD)SendMessage(m_handle, MCM_GETCURRENTVIEW, 0, 0L);
		}

		BOOL set_current_view(DWORD dwView) {
			return (BOOL)SendMessage(m_handle, MCM_SETCURRENTVIEW, 0, dwView);
		}

		DWORD get_calendar_count() const {
			return (DWORD)SendMessage(m_handle, MCM_GETCALENDARCOUNT, 0, 0L);
		}

		BOOL get_calendar_grid_info(PMCGRIDINFO pGridInfo) const {
			return (BOOL)SendMessage(m_handle, MCM_GETCALENDARGRIDINFO, 0, (LPARAM)pGridInfo);
		}

		CALID get_cal_id() const {
			return (CALID)SendMessage(m_handle, MCM_GETCALID, 0, 0L);
		}

		void set_cal_id(CALID calid) {
			SendMessage(m_handle, MCM_SETCALID, (LPARAM)calid, 0L);
		}

		int get_calendar_border() const {
			return (int)SendMessage(m_handle, MCM_GETCALENDARBORDER, 0, 0L);
		}

		void set_calendar_border(int cxyBorder, BOOL bSet = TRUE) {
			SendMessage(m_handle, MCM_SETCALENDARBORDER, (WPARAM)bSet, (LPARAM)cxyBorder);
		}

		int get_month_range(DWORD dwFlags, LPSYSTEMTIME lprgSysTimeArray) const {
			return (int)SendMessage(m_handle, MCM_GETMONTHRANGE, dwFlags, (LPARAM)lprgSysTimeArray);
		}

		BOOL set_day_state(int nMonths, LPMONTHDAYSTATE lpDayStateArray) {
			return (BOOL)SendMessage(m_handle, MCM_SETDAYSTATE, nMonths, (LPARAM)lpDayStateArray);
		}

		DWORD hit_test(PMCHITTESTINFO pMCHitTest) const {
			return (DWORD)SendMessage(m_handle, MCM_HITTEST, 0, (LPARAM)pMCHitTest);
		}

		void size_rect_to_min(LPRECT lpRect) {
			SendMessage(m_handle, MCM_SIZERECTTOMIN, 0, (LPARAM)lpRect);
		}

		SYSTEMTIME get_current_selection() const {
			SYSTEMTIME st = { 0 };
			get_cur_sel(&st);
			return st;
		}

		bool set_current_selection(WORD year, WORD month, WORD day) {
			SYSTEMTIME st = { 0 };
			st.wYear = year;
			st.wMonth = month;
			st.wDay = day;
			return set_cur_sel(&st) != FALSE;
		}

		std::pair<SYSTEMTIME, SYSTEMTIME> get_selection_range() const {
			SYSTEMTIME range[2] = { 0 };
			get_sel_range(range);
			return { range[0], range[1] };
		}

		bool set_selection_range(const SYSTEMTIME& start, const SYSTEMTIME& end) {
			SYSTEMTIME range[2] = { start, end };
			return set_sel_range(range) != FALSE;
		}

		bool set_selection_range(WORD startYear, WORD startMonth, WORD startDay,
								 WORD endYear, WORD endMonth, WORD endDay) {
			SYSTEMTIME range[2] = { 0 };
			range[0].wYear = startYear;
			range[0].wMonth = startMonth;
			range[0].wDay = startDay;
			range[1].wYear = endYear;
			range[1].wMonth = endMonth;
			range[1].wDay = endDay;
			return set_sel_range(range) != FALSE;
		}

		SYSTEMTIME get_today_date() const {
			SYSTEMTIME st = { 0 };
			get_today(&st);
			return st;
		}

		void set_today_date(WORD year, WORD month, WORD day) {
			SYSTEMTIME st = { 0 };
			st.wYear = year;
			st.wMonth = month;
			st.wDay = day;
			set_today(&st);
		}

		void reset_today() {
			set_today(NULL);
		}

		RECT get_minimum_rect() const {
			RECT rc = { 0 };
			get_min_req_rect(&rc);
			return rc;
		}

		SIZE get_minimum_size() const {
			RECT rc = get_minimum_rect();
			SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
			return sz;
		}

		std::pair<SYSTEMTIME, SYSTEMTIME> get_date_range() const {
			SYSTEMTIME range[2] = { 0 };
			get_range(range);
			return { range[0], range[1] };
		}

		bool set_date_range(const SYSTEMTIME& min, const SYSTEMTIME& max) {
			SYSTEMTIME range[2] = { min, max };
			return set_range(GDTR_MIN | GDTR_MAX, range) != FALSE;
		}

		bool set_min_date(const SYSTEMTIME& min) {
			SYSTEMTIME range[2] = { min, {0} };
			return set_range(GDTR_MIN, range) != FALSE;
		}

		bool set_max_date(const SYSTEMTIME& max) {
			SYSTEMTIME range[2] = { {0}, max };
			return set_range(GDTR_MAX, range) != FALSE;
		}

		bool clear_date_range() {
			return set_range(0, NULL) != FALSE;
		}

		COLORREF get_background_color() const {
			return get_color(MCSC_BACKGROUND);
		}

		COLORREF set_background_color(COLORREF clr) {
			return set_color(MCSC_BACKGROUND, clr);
		}

		COLORREF get_text_color() const {
			return get_color(MCSC_TEXT);
		}

		COLORREF set_text_color(COLORREF clr) {
			return set_color(MCSC_TEXT, clr);
		}

		COLORREF get_title_background_color() const {
			return get_color(MCSC_TITLEBK);
		}

		COLORREF set_title_background_color(COLORREF clr) {
			return set_color(MCSC_TITLEBK, clr);
		}

		COLORREF get_title_text_color() const {
			return get_color(MCSC_TITLETEXT);
		}

		COLORREF set_title_text_color(COLORREF clr) {
			return set_color(MCSC_TITLETEXT, clr);
		}

		COLORREF get_month_background_color() const {
			return get_color(MCSC_MONTHBK);
		}

		COLORREF set_month_background_color(COLORREF clr) {
			return set_color(MCSC_MONTHBK, clr);
		}

		COLORREF get_trailing_text_color() const {
			return get_color(MCSC_TRAILINGTEXT);
		}

		COLORREF set_trailing_text_color(COLORREF clr) {
			return set_color(MCSC_TRAILINGTEXT, clr);
		}

		bool is_month_view() const {
			return get_current_view() == MCMV_MONTH;
		}

		bool is_year_view() const {
			return get_current_view() == MCMV_YEAR;
		}

		bool is_decade_view() const {
			return get_current_view() == MCMV_DECADE;
		}

		bool is_century_view() const {
			return get_current_view() == MCMV_CENTURY;
		}

		void switch_to_month_view() {
			set_current_view(MCMV_MONTH);
		}

		void switch_to_year_view() {
			set_current_view(MCMV_YEAR);
		}

		void switch_to_decade_view() {
			set_current_view(MCMV_DECADE);
		}

		void switch_to_century_view() {
			set_current_view(MCMV_CENTURY);
		}

		bool is_multiselect() const {
			return (get_style() & MCS_MULTISELECT) == MCS_MULTISELECT;
		}

		bool has_week_numbers() const {
			return (get_style() & MCS_WEEKNUMBERS) == MCS_WEEKNUMBERS;
		}

		bool has_today_circle() const {
			return (get_style() & MCS_NOTODAYCIRCLE) == 0;
		}

		bool shows_today() const {
			return (get_style() & MCS_NOTODAY) == 0;
		}

		int get_visible_months() const {
			return static_cast<int>(get_calendar_count());
		}
	};

	class datetime_picker : public control {
	public:
		using control::control;

		BOOL set_format(LPCTSTR lpszFormat) {
			return (BOOL)SendMessage(m_handle, DTM_SETFORMAT, 0, (LPARAM)lpszFormat);
		}

		COLORREF get_month_cal_color(int nColorType) const {
			return (COLORREF)SendMessage(m_handle, DTM_GETMCCOLOR, nColorType, 0L);
		}

		COLORREF set_month_cal_color(int nColorType, COLORREF clr) {
			return (COLORREF)SendMessage(m_handle, DTM_SETMCCOLOR, nColorType, clr);
		}

		DWORD get_range(LPSYSTEMTIME lpSysTimeArray) const {
			return (DWORD)SendMessage(m_handle, DTM_GETRANGE, 0, (LPARAM)lpSysTimeArray);
		}

		BOOL set_range(DWORD dwFlags, LPSYSTEMTIME lpSysTimeArray) {
			return (BOOL)SendMessage(m_handle, DTM_SETRANGE, dwFlags, (LPARAM)lpSysTimeArray);
		}

		DWORD get_system_time(LPSYSTEMTIME lpSysTime) const {
			return (DWORD)SendMessage(m_handle, DTM_GETSYSTEMTIME, 0, (LPARAM)lpSysTime);
		}

		BOOL set_system_time(DWORD dwFlags, LPSYSTEMTIME lpSysTime) {
			return (BOOL)SendMessage(m_handle, DTM_SETSYSTEMTIME, dwFlags, (LPARAM)lpSysTime);
		}

		month_calendar get_month_cal() const {
			return month_calendar{ (HWND)SendMessage(m_handle, DTM_GETMONTHCAL, 0, 0L) };
		}

		HFONT get_month_cal_font() const {
			return (HFONT)SendMessage(m_handle, DTM_GETMCFONT, 0, 0L);
		}

		void set_month_cal_font(HFONT hFont, BOOL bRedraw = TRUE) {
			SendMessage(m_handle, DTM_SETMCFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
		}

		DWORD get_month_cal_style() const {
			return (DWORD)SendMessage(m_handle, DTM_GETMCSTYLE, 0, 0L);
		}

		DWORD set_month_cal_style(DWORD dwStyle) {
			return (DWORD)SendMessage(m_handle, DTM_SETMCSTYLE, 0, (LPARAM)dwStyle);
		}

		void get_datetime_picker_info(LPDATETIMEPICKERINFO lpPickerInfo) const {
			SendMessage(m_handle, DTM_GETDATETIMEPICKERINFO, 0, (LPARAM)lpPickerInfo);
		}

		BOOL get_ideal_size(LPSIZE lpSize) const {
			return (BOOL)SendMessage(m_handle, DTM_GETIDEALSIZE, 0, (LPARAM)lpSize);
		}

		void close_month_cal() {
			SendMessage(m_handle, DTM_CLOSEMONTHCAL, 0, 0L);
		}

		SYSTEMTIME get_time() const {
			SYSTEMTIME st = { 0 };
			get_system_time(&st);
			return st;
		}

		bool set_time(const SYSTEMTIME& st) {
			return set_system_time(GDT_VALID, const_cast<LPSYSTEMTIME>(&st)) != FALSE;
		}

		bool set_time(WORD year, WORD month, WORD day, WORD hour = 0, WORD minute = 0, WORD second = 0) {
			SYSTEMTIME st = { 0 };
			st.wYear = year;
			st.wMonth = month;
			st.wDay = day;
			st.wHour = hour;
			st.wMinute = minute;
			st.wSecond = second;
			return set_system_time(GDT_VALID, &st) != FALSE;
		}

		bool set_date(WORD year, WORD month, WORD day) {
			return set_time(year, month, day, 0, 0, 0);
		}

		bool clear_time() {
			return set_system_time(GDT_NONE, NULL) != FALSE;
		}

		bool has_valid_time() const {
			SYSTEMTIME st = { 0 };
			return get_system_time(&st) == GDT_VALID;
		}

		bool is_time_none() const {
			return !has_valid_time();
		}

		std::pair<SYSTEMTIME, SYSTEMTIME> get_date_range() const {
			SYSTEMTIME range[2] = { 0 };
			get_range(range);
			return { range[0], range[1] };
		}

		bool set_date_range(const SYSTEMTIME& min, const SYSTEMTIME& max) {
			SYSTEMTIME range[2] = { min, max };
			return set_range(GDTR_MIN | GDTR_MAX, range) != FALSE;
		}

		bool set_min_date(const SYSTEMTIME& min) {
			SYSTEMTIME range[2] = { min, {0} };
			return set_range(GDTR_MIN, range) != FALSE;
		}

		bool set_max_date(const SYSTEMTIME& max) {
			SYSTEMTIME range[2] = { {0}, max };
			return set_range(GDTR_MAX, range) != FALSE;
		}

		bool clear_date_range() {
			return set_range(0, NULL) != FALSE;
		}

		SIZE get_ideal_size() const {
			SIZE sz = { 0 };
			get_ideal_size(&sz);
			return sz;
		}

		bool set_format_string(const std::tstring& format) {
			return set_format(format.c_str()) != FALSE;
		}

		void set_short_date_format() {
			set_format(TEXT("d"));
		}

		void set_long_date_format() {
			set_format(TEXT("D"));
		}

		void set_time_format() {
			set_format(TEXT("t"));
		}

		void set_custom_format(const std::tstring& fmt) {
			set_format(fmt.c_str());
		}

		void reset_format() {
			set_format(NULL);
		}

		bool is_checkbox_checked() const {
			return has_valid_time();
		}

		void check_checkbox(bool check) {
			if (check && is_time_none()) {
				SYSTEMTIME st = { 0 };
				::GetLocalTime(&st);
				set_time(st);
			} else if (!check && has_valid_time()) {
				clear_time();
			}
		}

		bool shows_checkbox() const {
			return (get_style() & DTS_SHOWNONE) == DTS_SHOWNONE;
		}

		bool is_up_down_style() const {
			return (get_style() & DTS_UPDOWN) == DTS_UPDOWN;
		}

		bool shows_calendar_button() const {
			return !is_up_down_style();
		}

		bool is_right_align() const {
			return (get_style() & DTS_RIGHTALIGN) == DTS_RIGHTALIGN;
		}

		bool is_long_date_format() const {
			return (get_style() & DTS_LONGDATEFORMAT) == DTS_LONGDATEFORMAT;
		}

		bool is_short_date_format() const {
			return (get_style() & DTS_SHORTDATEFORMAT) == DTS_SHORTDATEFORMAT;
		}

		bool is_time_format() const {
			return (get_style() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT;
		}

		COLORREF get_calendar_background_color() const {
			return get_month_cal_color(MCSC_BACKGROUND);
		}

		COLORREF set_calendar_background_color(COLORREF clr) {
			return set_month_cal_color(MCSC_BACKGROUND, clr);
		}

		COLORREF get_calendar_text_color() const {
			return get_month_cal_color(MCSC_TEXT);
		}

		COLORREF set_calendar_text_color(COLORREF clr) {
			return set_month_cal_color(MCSC_TEXT, clr);
		}

		COLORREF get_calendar_title_background_color() const {
			return get_month_cal_color(MCSC_TITLEBK);
		}

		COLORREF set_calendar_title_background_color(COLORREF clr) {
			return set_month_cal_color(MCSC_TITLEBK, clr);
		}

		COLORREF get_calendar_title_text_color() const {
			return get_month_cal_color(MCSC_TITLETEXT);
		}

		COLORREF set_calendar_title_text_color(COLORREF clr) {
			return set_month_cal_color(MCSC_TITLETEXT, clr);
		}

		void open_calendar() {
			SendMessage(m_handle, WM_SYSKEYDOWN, VK_DOWN, 0);
		}

		void set_to_today() {
			SYSTEMTIME st = { 0 };
			::GetLocalTime(&st);
			set_time(st);
		}

		void set_to_now() {
			set_to_today();
		}
	};
}

#endif // __DATEANDTIME_CONTROLS_H__