#pragma once

#include <cmath>
#include <Windows.h>
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

		// ���Z (lower)
		inline auto operator + (const pt_lower auto& pt) const noexcept
		{
			return Point<T>(
				this->x + (T)pt.x,
				this->y + (T)pt.y
			);
		}

		// ���Z (upper)
		inline auto operator + (const pt_upper auto& pt) const noexcept
		{
			return Point<T>(
				this->x + (T)pt.X,
				this->y + (T)pt.Y
			);
		}

		// ���Z (lower)
		inline auto operator - (const pt_lower auto& pt) const noexcept
		{
			return Point<T>(
				this->x - (T)pt.x,
				this->y - (T)pt.y
			);
		}

		// ���Z (upper)
		inline auto operator - (const pt_upper auto& pt) const noexcept
		{
			return Point<T>(
				this->x - (T)pt.X,
				this->y - (T)pt.Y
			);
		}

		// ���� (lower)
		inline auto operator * (const pt_lower auto& pt) const noexcept
		{
			return this->x * (T)pt.x + this->y * (T)pt.y;
		}

		// ����
		inline auto operator * (const pt_upper auto& pt) const noexcept
		{
			return this->x * (T)pt.X + this->y * (T)pt.Y;
		}

		// ��� (lower)
		inline auto operator = (const pt_lower auto& pt) noexcept
		{
			this->x = (T)pt.x;
			this->y = (T)pt.y;
			return *this;
		}

		// ��� (upper)
		inline auto operator = (const pt_upper auto& pt) noexcept
		{
			this->x = (T)pt.X;
			this->y = (T)pt.Y;
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

		// ���̌^�ɕϊ� (lower)
		template <pt_lower U>
		auto to() const noexcept
		{
			U pt;
			pt.x = static_cast<decltype(pt.x)>(x);
			pt.y = static_cast<decltype(pt.y)>(y);
			return pt;
		}

		// ���̌^�ɕϊ� (upper)
		template <pt_upper U>
		auto to() const noexcept
		{
			U pt;
			pt.X = static_cast<decltype(pt.X)>(x);
			pt.Y = static_cast<decltype(pt.Y)>(y);
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