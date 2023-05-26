//----------------------------------------------------------------------------------
//		MKAul (Point)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include "utils.hpp"



namespace mkaul {
	// �|�C���g
	template <typename T>
	struct Point {
		T x, y;

		// ����
		inline bool operator == (const Point<T>& pt) const noexcept
		{
			return (this->x == pt.x && this->y == pt.y);
		}

		// ���Z
		template <typename PtType>
		inline Point<T> operator + (const PtType& pt) const noexcept
		{
			return {
				this->x + (T)pt.x,
				this->y + (T)pt.y
			};
		}

		// ���Z
		template <typename PtType>
		inline Point<T> operator - (const PtType& pt) const noexcept
		{
			return {
				this->x - (T)pt.x,
				this->y - (T)pt.y
			};
		}

		// ����
		template <typename PtType>
		inline T operator * (const PtType& pt) const noexcept
		{
			return this->x * (T)pt.x + this->y * (T)pt.y;
		}

		// ���
		template <typename PtType>
		inline Point<T> operator = (const PtType& pt) noexcept
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

		// ���̌^�ɕϊ�
		template <typename PtType>
		auto to() const noexcept
		{
			PtType pt = { 0 };
			pt.x = static_cast<decltype(pt.x)>(x);
			pt.y = static_cast<decltype(pt.y)>(y);

			return pt;
		}

		// �萔�{
		template <typename U>
		void scale(U scl) noexcept
		{
			x = (T)(x * scl);
			y = (T)(y * scl);
		}

		// ��]
		void rotate(double deg) noexcept
		{
			auto rad = deg2rad(deg);
			auto t = x;
			x = (T)(x * std::cos(rad) - y * std::sin(rad));
			y = (T)(t * std::sin(rad) + y * std::cos(rad));
		}
	};


	// �T�C�Y
	template <typename T>
	struct Size {
		T width, height;

		inline bool operator == (const Size<T>& size) const noexcept
		{
			return (this->width == size.width && this->height == size.height);
		}

		template <typename SizeType>
		inline Size<T> operator + (const SizeType& size) const noexcept
		{
			return {
				this->width + (T)size.width,
				this->height + (T)size.height
			};
		}

		template <typename SizeType>
		inline Size<T> operator - (const SizeType& size) const noexcept
		{
			return {
				this->width - (T)size.width,
				this->height - (T)size.height
			};
		}

		template <typename SizeType>
		inline void operator = (const SizeType& size) noexcept
		{
			this->width = (T)size.width;
			this->height = (T)size.height;
		}

		inline Size(T width_ = 0, T height_ = 0) :
			width(width_),
			height(height_)
		{}

		template <typename SizeType>
		auto to() const noexcept
		{
			SizeType size = { 0 };
			size.width = static_cast<decltype(size.width)>(width);
			size.height = static_cast<decltype(size.height)>(height);

			return size;
		}

		inline void scale(double scale) noexcept
		{
			width *= std::abs(scale);
			height *= std::abs(scale);
		}
	};
}