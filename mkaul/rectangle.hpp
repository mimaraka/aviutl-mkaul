#pragma once

#include <Windows.h>
#include "point.hpp"



namespace mkaul {
	template <typename T>
	// 矩形
	struct Rectangle {
		T left, top, right, bottom;

		bool operator == (const Rectangle<T>& rc) const
		{
			return (
				this->left == rc.left and
				this->top == rc.top and
				this->right == rc.right and
				this->bottom == rc.bottom
				);
		}

		Rectangle<T> operator = (const RECT& rc)
		{
			this->left = static_cast<T>(rc.left);
			this->top = static_cast<T>(rc.top);
			this->right = static_cast<T>(rc.right);
			this->bottom = static_cast<T>(rc.bottom);

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
			left(static_cast<T>(rc.left)),
			top(static_cast<T>(rc.top)),
			right(static_cast<T>(rc.right)),
			bottom(static_cast<T>(rc.bottom))
		{}

		template <typename U>
		void convert_to(Rectangle<U>* r) const noexcept
		{
			r->left = static_cast<U>(left);
			r->top = static_cast<U>(top);
			r->right = static_cast<U>(right);
			r->bottom = static_cast<U>(bottom);
		}

		constexpr T get_width() const noexcept { return static_cast<T>(std::abs(right - left)); }
		constexpr T get_height() const noexcept { return static_cast<T>(std::abs(bottom - top)); }
		constexpr T get_area() const noexcept { return get_width() * get_height(); }
		
		template <typename U = T>
		constexpr auto get_center()
		{
			return Point<U>(
				(left + right) / static_cast<U>(2.),
				(top + bottom) / static_cast<U>(2.)
			);
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
			Rectangle<LONG>::operator = (rc);
			return *this;
		}

		using Rectangle<LONG>::Rectangle;

		void set(int left, int top, int right, int bottom) noexcept;
		void set_margin(int left, int top, int right, int bottom) noexcept;
		auto get_rect() const noexcept { return RECT{ left, top, right, bottom }; };
		void client_to_screen(HWND hwnd) noexcept;
		void screen_to_client(HWND hwnd) noexcept;
	};
}