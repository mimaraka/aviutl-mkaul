//----------------------------------------------------------------------------------
//		MKAul (Rectangle)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "rectangle.hpp"



namespace mkaul {
	// 初期化
	void WindowRectangle::set(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}


	// 矩形をn個に分割
	void WindowRectangle::divide(
		Direction dir,
		WindowRectangle* rects_child[],
		float weights[],
		int n
	) const
	{
		int l_parent, l_child;
		float weights_sum = 0.f;
		std::vector<float> list_weights_sum;

		if (weights) {
			for (int i = 0; i < n; i++) {
				weights_sum += weights[i];
				list_weights_sum.emplace_back(weights_sum);
			}
		}
		if (dir == Direction::Vertical)
			l_parent = bottom - top;
		else
			l_parent = right - left;

		l_child = l_parent / n;

		for (int i = 0; i < n; i++) {
			int st, ed;

			if (weights) {
				st = (int)(l_parent * ((i == 0) ? 0 : list_weights_sum[i - 1]) / weights_sum);
				ed = (int)(l_parent * list_weights_sum[i] / weights_sum);
			}
			else {
				st = i * l_child;
				ed = (i + 1) * l_child;
			}

			if (dir == Direction::Vertical) {
				rects_child[i]->left = left;
				rects_child[i]->right = right;
				rects_child[i]->top = top + st;
				rects_child[i]->bottom = top + ed;
			}
			else {
				rects_child[i]->left = left + st;
				rects_child[i]->right = left + ed;
				rects_child[i]->top = top;
				rects_child[i]->bottom = bottom;
			}
		}
	}

	// マージンを設定
	void WindowRectangle::set_margin(int l, int t, int r, int b)
	{
		left += l;
		top += t;
		right -= r;
		bottom -= b;

		if (left > right)
			left = right = (left + right) / 2;
		if (top > bottom)
			top = bottom = (top + bottom) / 2;
	}


	// クライアント -> スクリーン
	void WindowRectangle::client_to_screen(HWND hwnd)
	{
		POINT pt[] = {
			{left, top},
			{right, bottom}
		};

		::ClientToScreen(hwnd, &pt[0]);
		::ClientToScreen(hwnd, &pt[1]);

		left = pt[0].x;
		top = pt[0].y;
		right = pt[1].x;
		bottom = pt[1].y;
	}

	// スクリーン -> クライアント
	void WindowRectangle::screen_to_client(HWND hwnd)
	{
		POINT pt[] = {
			{left, top},
			{right, bottom}
		};

		::ScreenToClient(hwnd, &pt[0]);
		::ScreenToClient(hwnd, &pt[1]);

		left = pt[0].x;
		top = pt[0].y;
		right = pt[1].x;
		bottom = pt[1].y;
	}
}