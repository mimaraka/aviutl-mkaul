//----------------------------------------------------------------------------------
//		MKAul (Point)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include "utils.hpp"



namespace mkaul {
	// É|ÉCÉìÉg
	template <typename T>
	struct Point {
		T x, y;

		// ìôâø
		inline bool operator == (const Point<T>& pt) const
		{
			return (this->x == pt.x && this->y == pt.y);
		}

		// â¡éZ
		template <typename PtType>
		inline Point<T> operator + (const PtType& pt) const
		{
			return {
				this->x + (T)pt.x,
				this->y + (T)pt.y
			};
		}

		// å∏éZ
		template <typename PtType>
		inline Point<T> operator - (const PtType& pt) const
		{
			return {
				this->x - (T)pt.x,
				this->y - (T)pt.y
			};
		}

		// ì‡êœ
		template <typename PtType>
		inline T operator * (const PtType& pt) const
		{
			return this->x * (T)pt.x + this->y * (T)pt.y;
		}

		// ë„ì¸
		template <typename PtType>
		inline Point<T> operator = (const PtType& pt)
		{
			this->x = (T)pt.x;
			this->y = (T)pt.y;
			return *this;
		}

		Point(T x_ = 0, T y_ = 0) :
			x(x_),
			y(y_)
		{}

		Point(const POINT& pt) :
			x((T)pt.x),
			y((T)pt.y)
		{}

		// ëºÇÃå^Ç…ïœä∑
		template <typename PtType>
		auto to() const
		{
			PtType pt = { 0 };
			pt.x = static_cast<decltype(pt.x)>(x);
			pt.y = static_cast<decltype(pt.y)>(y);

			return pt;
		}

		// íËêîî{
		template <typename U>
		void scale(U scl)
		{
			x = (T)(x * scl);
			y = (T)(y * scl);
		}

		// âÒì]
		void rotate(double deg)
		{
			auto rad = deg2rad(-deg);
			auto t = x;
			x = (T)(x * std::cos(rad) - y * std::sin(rad));
			y = (T)(-t * std::sin(rad) - y * std::cos(rad));
		}
	};


	// ÉTÉCÉY
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

		template <typename SizeType>
		auto to() const
		{
			SizeType size = { 0 };
			size.width = static_cast<decltype(size.width)>(width);
			size.height = static_cast<decltype(size.height)>(height);

			return size;
		}

		inline void scale(double scale)
		{
			width *= std::abs(scale);
			height *= std::abs(scale);
		}
	};
}