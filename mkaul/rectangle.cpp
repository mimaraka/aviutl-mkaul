//----------------------------------------------------------------------------------
//		MKAul (Rectangle)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "rectangle.hpp"



namespace mkaul {
	// Initialize (1)
	void Window_Rectangle::set(const RECT& rc)
	{
		rect = rc;
	}

	// Initialize (2)
	void Window_Rectangle::set(int left, int top, int right, int bottom)
	{
		rect = { left, top, right, bottom };
	}

	// Divide a rectangle into n child rectangles
	void Window_Rectangle::divide(LPRECT rects_child[], float weights[], int n) const
	{
		int width_parent = rect.right - rect.left;
		int width_child = width_parent / n;
		float weights_sum = 0;
		std::vector<float> list_weights_sum;
		int left, right;

		if (weights) {
			for (int i = 0; i < n; i++) {
				weights_sum += weights[i];
				list_weights_sum.emplace_back(weights_sum);
			}
		}

		for (int i = 0; i < n; i++) {
			if (weights) {
				left = (int)(width_parent * ((i == 0) ? 0 : list_weights_sum[i - 1]) / weights_sum);
				right = (int)(width_parent * list_weights_sum[i] / weights_sum);
			}
			else {
				left = i * width_child;
				right = (i + 1) * width_child;
			}
			rects_child[i]->left = rect.left + left;
			rects_child[i]->right = rect.left + right;
			rects_child[i]->top = rect.top;
			rects_child[i]->bottom = rect.bottom;
		}
	}

	// Set Margin
	void Window_Rectangle::set_margin(int left, int top, int right, int bottom)
	{
		rect.left += left;
		rect.top += top;
		rect.right -= right;
		rect.bottom -= bottom;

		if (rect.left > rect.right)
			rect.left = rect.right = (rect.left + rect.right) / 2;
		if (rect.top > rect.bottom)
			rect.top = rect.bottom = (rect.top + rect.bottom) / 2;
	}

	// Client to Screen
	void Window_Rectangle::client_to_screen(HWND hwnd)
	{
		POINT pt[2];
		pt[0] = {
			rect.left,
			rect.top
		};
		pt[1] = {
			rect.right,
			rect.bottom
		};
		::ClientToScreen(hwnd, &pt[0]);
		::ClientToScreen(hwnd, &pt[1]);
		rect = {
			pt[0].x, pt[0].y,
			pt[1].x, pt[1].y,
		};
	}

	// Screen to Client
	void Window_Rectangle::screen_to_client(HWND hwnd)
	{
		POINT pt[2];
		pt[0] = {
			rect.left,
			rect.top
		};
		pt[1] = {
			rect.right,
			rect.bottom
		};
		::ScreenToClient(hwnd, &pt[0]);
		::ScreenToClient(hwnd, &pt[1]);
		rect = {
			pt[0].x, pt[0].y,
			pt[1].x, pt[1].y,
		};
	}
}