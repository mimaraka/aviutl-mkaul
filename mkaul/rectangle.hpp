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

		bool operator == (const Rectangle<T>& rc) const
		{
			return (
				this->left == rc.left &&
				this->top == rc.top &&
				this->right == rc.right &&
				this->bottom == rc.bottom
				);
		}

		Rectangle<T> operator = (const RECT& rc)
		{
			this->left = (T)rc.left;
			this->top = (T)rc.top;
			this->right = (T)rc.right;
			this->bottom = (T)rc.bottom;

			return *this;
		}

		Rectangle(
			T l = 0,
			T t = 0,
			T r = 0,
			T b = 0
		) :
			left(l),
			top(t),
			right(r),
			bottom(b)
		{}

		Rectangle(RECT rc) :
			left((T)rc.left),
			top((T)rc.top),
			right((T)rc.right),
			bottom((T)rc.bottom)
		{}

		template <typename U>
		inline void convert_to(Rectangle<U>* r)
		{
			r->left = (U)left;
			r->top = (U)top;
			r->right = (U)right;
			r->bottom = (U)bottom;
		}
	};


	// 矩形(ウィンドウ用)
	struct WindowRectangle : public Rectangle<LONG> {
		enum class Direction {
			Vertical,
			Horizontal
		};

		WindowRectangle operator = (const RECT& rc)
		{
			Rectangle<LONG>::operator=(rc);
			return *this;
		}

		using Rectangle<LONG>::Rectangle;

		void set(int left, int top, int right, int bottom);
		void set_margin(int left, int top, int right, int bottom);
		void divide(
			Direction dir,
			WindowRectangle* rects_child[],
			float weights[],
			int n
		) const;
		void client_to_screen(HWND hwnd);
		void screen_to_client(HWND hwnd);
	};
}