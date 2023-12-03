#include <vector>
#include "rectangle.hpp"



namespace mkaul {
	// 初期化
	void WindowRectangle::set(int l, int t, int r, int b) noexcept
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}


	// マージンを設定
	void WindowRectangle::set_margin(int l, int t, int r, int b) noexcept
	{
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
	void WindowRectangle::client_to_screen(HWND hwnd) noexcept
	{
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
	void WindowRectangle::screen_to_client(HWND hwnd) noexcept
	{
		POINT pt_lefttop = { left, top };
		POINT pt_rightbottom = { right, bottom };

		::ScreenToClient(hwnd, &pt_lefttop);
		::ScreenToClient(hwnd, &pt_rightbottom);

		left = pt_lefttop.x;
		top = pt_lefttop.y;
		right = pt_rightbottom.x;
		bottom = pt_rightbottom.y;
	}
}