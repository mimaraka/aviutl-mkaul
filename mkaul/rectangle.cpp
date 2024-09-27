#include <vector>
#include "rectangle.hpp"



namespace mkaul {
	// マージンを設定
	void WindowRectangle::set_margin(int l, int t, int r, int b) noexcept {
		left += l;
		top += t;
		right -= r;
		bottom -= b;

		if (right < left)
			left = right = (left + right) / 2;
		if (bottom < top)
			top = bottom = (top + bottom) / 2;
	}


	// クライアント -> スクリーン
	void WindowRectangle::client_to_screen(HWND hwnd) noexcept {
		POINT pt_lefttop = { left, top };
		POINT pt_rightbottom = { right, bottom };

		::ClientToScreen(hwnd, &pt_lefttop);
		::ClientToScreen(hwnd, &pt_rightbottom);

		left = pt_lefttop.x;
		top = pt_lefttop.y;
		right = pt_rightbottom.x;
		bottom = pt_rightbottom.y;
	}


	// スクリーン -> クライアント
	void WindowRectangle::screen_to_client(HWND hwnd) noexcept {
		POINT pt_lefttop = { left, top };
		POINT pt_rightbottom = { right, bottom };

		::ScreenToClient(hwnd, &pt_lefttop);
		::ScreenToClient(hwnd, &pt_rightbottom);

		left = pt_lefttop.x;
		top = pt_lefttop.y;
		right = pt_rightbottom.x;
		bottom = pt_rightbottom.y;
	}


	// クライアント領域の矩形を取得
	bool WindowRectangle::from_client_rect(HWND hwnd) noexcept {
		RECT rc;
		if (!::GetClientRect(hwnd, &rc))
			return false;
		set(rc);
		return true;
	}


	// ウィンドウ領域の矩形を取得
	bool WindowRectangle::from_window_rect(HWND hwnd) noexcept {
		RECT rc;
		if (!::GetWindowRect(hwnd, &rc))
			return false;
		set(rc);
		return true;
	}
} // namespace mkaul