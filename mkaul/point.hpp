//----------------------------------------------------------------------------------
//		MKAul (Point)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	template <typename T>
	// ƒ|ƒCƒ“ƒg
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

		inline Point(T x_ = 0, T y_ = 0) :
			x(x_),
			y(y_)
		{}

		inline Point(const POINT& pt) :
			x((T)pt.x),
			y((T)pt.y)
		{}

		template<typename U>
		inline Point<U> to()
		{
			return { (U)x, (U)y };
		}

		inline POINT to_win32_point()
		{
			return { (LONG)x, (LONG)y };
		}
	};
}