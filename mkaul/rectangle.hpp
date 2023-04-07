//----------------------------------------------------------------------------------
//		MKAul (Rectangle)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	template <typename T>
	// 矩形
	struct Rectangle {
		T left, top, right, bottom;

		inline Rectangle(T l = 0, T t = 0, T r = 0, T b = 0) :
			left(l), top(t), right(r), bottom(b)
		{}

		inline Rectangle(RECT rc) :
			left((T)rc.left),
			top((T)rc.top),
			right((T)rc.right),
			bottom((T)rc.bottom)
		{}

		template <typename U>
		void convert_to(Rectangle<U>* r)
		{
			r->left = (U)left;
			r->top = (U)top;
			r->right = (U)right;
			r->bottom = (U)bottom;
		}
	};


	// 矩形(ウィンドウ用)
	struct Window_Rectangle {
		RECT rect;

		void set(const RECT& rc);
		void set(int left, int top, int right, int bottom);
		void set_margin(int left, int top, int right, int bottom);
		void divide(LPRECT rects_child[], float weights[], int n) const;
		void client_to_screen(HWND hwnd);
		void screen_to_client(HWND hwnd);
	};
}