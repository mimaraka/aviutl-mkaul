//----------------------------------------------------------------------------------
//		MKAul (Point)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include "util.hpp"



namespace mkaul {
	// �|�C���g
	template <typename T>
	struct Point {
		T x, y;

		// ����
		inline auto operator == (const Point<T>& pt) const noexcept
		{
			return (this->x == pt.x && this->y == pt.y);
		}

		// ���Z
		inline auto operator + (const PtType auto& pt) const noexcept
		{
			return Point<T>(
				this->x + (T)pt.x,
				this->y + (T)pt.y
			);
		}

		// ���Z
		inline auto operator - (const PtType auto& pt) const noexcept
		{
			return Point<T>(
				this->x - (T)pt.x,
				this->y - (T)pt.y
			);
		}

		// ����
		inline auto operator * (const PtType auto& pt) const noexcept
		{
			return this->x * (T)pt.x + this->y * (T)pt.y;
		}

		// ���
		inline auto operator = (const PtType auto& pt) noexcept
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
		template <PtType U>
		auto to() const noexcept
		{
			U pt = { 0 };
			pt.x = static_cast<decltype(pt.x)>(x);
			pt.y = static_cast<decltype(pt.y)>(y);

			return pt;
		}

		// �萔�{
		void scale(auto scl) noexcept
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
}