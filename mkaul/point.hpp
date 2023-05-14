//----------------------------------------------------------------------------------
//		MKAul (Point)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	// ポイント
	template <typename T>
	struct Point {
		T x, y;

		inline bool operator == (const Point<T>& pt) const
		{
			return (this->x == pt.x && this->y == pt.y);
		}

		template <typename PtType>
		inline Point<T> operator + (const PtType& pt) const
		{
			return {
				this->x + (T)pt.x,
				this->y + (T)pt.y
			};
		}

		template <typename PtType>
		inline Point<T> operator - (const PtType& pt) const
		{
			return {
				this->x - (T)pt.x,
				this->y - (T)pt.y
			};
		}

		template <typename PtType>
		inline void operator = (const PtType& pt)
		{
			this->x = (T)pt.x;
			this->y = (T)pt.y;
		}

		Point(T x_ = 0, T y_ = 0) :
			x(x_),
			y(y_)
		{}

		Point(const POINT& pt) :
			x((T)pt.x),
			y((T)pt.y)
		{}

		template <typename PtType>
		auto to() const
		{
			PtType pt = { 0 };
			pt.x = static_cast<decltype(pt.x)>(x);
			pt.y = static_cast<decltype(pt.y)>(y);

			return pt;
		}
	};


	// サイズ
	template <typename T>
	struct Size {
		T width, height;

		inline bool operator == (const Size<T>& size) const
		{
			return (this->width == size.width && this->height == size.height);
		}

		template <typename SizeType>
		inline Size<T> operator + (const SizeType& size) const
		{
			return {
				this->width + (T)size.width,
				this->height + (T)size.height
			};
		}

		template <typename SizeType>
		inline Size<T> operator - (const SizeType& size) const
		{
			return {
				this->width - (T)size.width,
				this->height - (T)size.height
			};
		}

		template <typename SizeType>
		inline void operator = (const SizeType& size)
		{
			this->width = (T)size.width;
			this->height = (T)size.height;
		}

		inline Size(T width_ = 0, T height_ = 0) :
			width(width_),
			height(height_)
		{}

		template<typename U>
		inline Size<U> to()
		{
			return Size<U>(width, height);
		}

		inline void scale(double scale)
		{
			width *= std::abs(scale);
			height *= std::abs(scale);
		}
	};
}