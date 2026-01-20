#ifndef __MISC_CONTROLS_HPP__
#define __MISC_CONTROLS_HPP__

namespace wpp
{
	class ipv4_address : public control {
	public:
		using control::control;

		BOOL is_blank() const {
			return (BOOL)SendMessage(m_handle, IPM_ISBLANK, 0, 0L);
		}

		int get_address(LPDWORD lpdwAddress) const {
			return (int)SendMessage(m_handle, IPM_GETADDRESS, 0, (LPARAM)lpdwAddress);
		}

		DWORD get_address() const {
			DWORD dwAddress = 0;
			SendMessage(m_handle, IPM_GETADDRESS, 0, (LPARAM)&dwAddress);
			return dwAddress;
		}

		void get_address(unsigned& b1, unsigned& b2, unsigned& b3, unsigned& b4) const {
			DWORD dwAddress = 0;
			SendMessage(m_handle, IPM_GETADDRESS, 0, (LPARAM)&dwAddress);
			b1 = FIRST_IPADDRESS(dwAddress);
			b2 = SECOND_IPADDRESS(dwAddress);
			b3 = THIRD_IPADDRESS(dwAddress);
			b4 = FOURTH_IPADDRESS(dwAddress);
		}

		void set_address(DWORD dwAddress) {
			SendMessage(m_handle, IPM_SETADDRESS, 0, dwAddress);
		}

		void set_address(unsigned b1, unsigned b2, unsigned b3, unsigned b4) {
			SendMessage(m_handle, IPM_SETADDRESS, 0, MAKEIPADDRESS(b1, b2, b3, b4));
		}

		void clear_address() {
			SendMessage(m_handle, IPM_CLEARADDRESS, 0, 0L);
		}

		void set_range(int nField, WORD wRange) {
			SendMessage(m_handle, IPM_SETRANGE, nField, wRange);
		}

		void set_range(int nField, BYTE nMin, BYTE nMax) {
			SendMessage(m_handle, IPM_SETRANGE, nField, MAKEIPRANGE(nMin, nMax));
		}

		void set_focus(int nField) {
			SendMessage(m_handle, IPM_SETFOCUS, nField, 0L);
		}

		void set_field(int nField, BYTE value) {
			DWORD dwAddress = get_address();
			BYTE* fields = (BYTE*)&dwAddress;
			if (nField >= 0 && nField < 4) {
				fields[3 - nField] = value;
				set_address(dwAddress);
			}
		}

		BYTE get_field(int nField) const {
			DWORD dwAddress = get_address();
			BYTE* fields = (BYTE*)&dwAddress;
			if (nField >= 0 && nField < 4) {
				return fields[3 - nField];
			}
			return 0;
		}
	};

	class sys_link : public control {
	public:
		using control::control;

		int get_ideal_height(int cxMaxWidth = 0) const {
			return (int)SendMessage(m_handle, LM_GETIDEALHEIGHT, cxMaxWidth, 0L);
		}

		BOOL get_item(PLITEM pLItem) const {
			return (BOOL)SendMessage(m_handle, LM_GETITEM, 0, (LPARAM)pLItem);
		}

		BOOL get_item(int iLink, LITEM& lItem) const {
			lItem.mask = LIF_ITEMINDEX | LIF_STATE | LIF_ITEMID | LIF_URL;
			lItem.iLink = iLink;
			return (BOOL)SendMessage(m_handle, LM_GETITEM, 0, (LPARAM)&lItem);
		}

		BOOL set_item_ex(UINT uMask, int iLink, UINT state, UINT stateMask, LPCWSTR szID, LPCWSTR szUrl) {
			LITEM lItem = { 0 };
			lItem.mask = uMask;
			lItem.iLink = iLink;
			lItem.state = state;
			lItem.stateMask = stateMask;
			wcscpy_s(lItem.szID, szID);
			wcscpy_s(lItem.szUrl, szUrl);
			return (BOOL)SendMessage(m_handle, LM_SETITEM, 0, (LPARAM)&lItem);
		}

		BOOL set_item(PLITEM pLItem) {
			return (BOOL)SendMessage(m_handle, LM_SETITEM, 0, (LPARAM)pLItem);
		}

		BOOL set_item_state(int iLink, UINT state, UINT stateMask = LIS_ENABLED | LIS_FOCUSED | LIS_VISITED) {
			LITEM lItem = { 0 };
			lItem.mask = LIF_ITEMINDEX | LIF_STATE;
			lItem.iLink = iLink;
			lItem.state = state;
			lItem.stateMask = stateMask;
			return (BOOL)SendMessage(m_handle, LM_SETITEM, 0, (LPARAM)&lItem);
		}

		BOOL set_item_url(int iLink, LPCWSTR szUrl) {
			LITEM lItem = { 0 };
			lItem.mask = LIF_ITEMINDEX | LIF_URL;
			lItem.iLink = iLink;
			wcscpy_s(lItem.szUrl, szUrl);
			return (BOOL)SendMessage(m_handle, LM_SETITEM, 0, (LPARAM)&lItem);
		}

		int get_ideal_size(SIZE& size, int cxMaxWidth = 0) const {
			return (int)SendMessage(m_handle, LM_GETIDEALSIZE, cxMaxWidth, (LPARAM)&size);
		}

		SIZE get_ideal_size(int cxMaxWidth = 0) const {
			SIZE size = { 0 };
			SendMessage(m_handle, LM_GETIDEALSIZE, cxMaxWidth, (LPARAM)&size);
			return size;
		}

		BOOL hit_test(PLHITTESTINFO pLHitTestInfo) const {
			return (BOOL)SendMessage(m_handle, LM_HITTEST, 0, (LPARAM)pLHitTestInfo);
		}

		int hit_test(POINT pt) const {
			LHITTESTINFO hti = { 0 };
			hti.pt = pt;
			if (SendMessage(m_handle, LM_HITTEST, 0, (LPARAM)&hti)) {
				return hti.item.iLink;
			}
			return -1;
		}
	};
}

#endif //__MISC_CONTROLS_HPP__