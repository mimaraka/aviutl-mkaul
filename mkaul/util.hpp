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


	// 範囲内判定
	inline constexpr bool in_range(auto val, auto min, auto max, bool equal) noexcept {
		if (val >= min and val <= max) {
			if (val == min or val == max)
				return equal;
			else return true;
		}
		else return false;
	}


	// 距離を算出 (lower)
	inline constexpr double distance(const pt_lower auto& pt1, const pt_lower auto& pt2) noexcept {
		return std::sqrt(std::pow(pt2.x - pt1.x, 2) + std::pow(pt2.y - pt1.y, 2));
	}


	// 距離を算出 (upper)
	inline constexpr double distance(const pt_upper auto& pt1, const pt_upper auto& pt2) noexcept {
		return std::sqrt(std::pow(pt2.X - pt1.X, 2) + std::pow(pt2.Y - pt1.Y, 2));
	}


	// ラジアン -> 度
	inline constexpr auto rad2deg(auto rad) noexcept {
		return (decltype(rad))180 * rad / std::numbers::pi_v<decltype(rad)>;
	}


	// 度 -> ラジアン
	inline constexpr auto deg2rad(auto deg) noexcept {
		return std::numbers::pi_v<decltype(deg)> * deg / (decltype(deg))180;
	}


	// 符号関数
	inline constexpr auto sign(auto value) noexcept {
		if (value < (decltype(value))0) {
			return (decltype(value))(-1);
		}
		else if (value == (decltype(value))0) {
			return (decltype(value))0;
		}
		else {
			return (decltype(value))1;
		}
	}
}