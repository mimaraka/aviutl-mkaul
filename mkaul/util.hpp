#pragma once

#include <cmath>
#include <numbers>



namespace mkaul {
	template <typename T>
	concept pt_lower = requires (T pt) {
		pt.x; pt.y;
	};


	template <typename T>
	concept pt_upper = requires (T pt) {
		pt.X; pt.Y;
	};


	// �͈͓�����
	inline bool in_range(auto val, auto min, auto max, bool equal) noexcept
	{
		if (val >= min && val <= max) {
			if (val == min || val == max)
				return equal;
			else return true;
		}
		else return false;
	}


	// �������Z�o
	inline double distance(const pt_lower auto& pt1, const pt_lower auto& pt2) noexcept
	{
		return std::sqrt(std::pow(pt2.x - pt1.x, 2) + std::pow(pt2.y - pt1.y, 2));
	}


	// �������Z�o
	inline double distance(const pt_upper auto& pt1, const pt_upper auto& pt2) noexcept
	{
		return std::sqrt(std::pow(pt2.X - pt1.X, 2) + std::pow(pt2.Y - pt1.Y, 2));
	}


	// ���W�A�� -> �x
	inline constexpr auto rad2deg(auto rad) noexcept
	{
		return (decltype(rad))180 * rad / std::numbers::pi_v<decltype(rad)>;
	}


	// �x -> ���W�A��
	inline constexpr auto deg2rad(auto deg) noexcept
	{
		return std::numbers::pi_v<decltype(deg)> * deg / (decltype(deg))180;
	}


	// �����֐�
	inline constexpr auto sign(auto value) noexcept
	{
		if (value < (decltype(value))0)
			return (decltype(value))(-1);
		else if (value == (decltype(value))0)
			return (decltype(value))0;
		else return (decltype(value))1;
	}
}