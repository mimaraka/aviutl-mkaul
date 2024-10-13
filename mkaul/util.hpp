#pragma once

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#define NOMINMAX

#include <algorithm>
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
	inline constexpr bool in_range(auto val, auto min, auto max, bool equal = true) noexcept {
		if (min <= val and val <= max) {
			if (val == min or val == max) {
				return equal;
			}
			else return true;
		}
		else return false;
	}


	// 距離を算出 (lower)
	inline constexpr double distance(const pt_lower auto& pt1, const pt_lower auto& pt2) noexcept {
		return std::sqrt(std::pow(pt2.x - pt1.x, 2) + std::pow(pt2.y - pt1.y, 2));
	}


	inline constexpr double distance(const pt_lower auto& pt) noexcept {
		return std::sqrt(std::pow(pt.x, 2) + std::pow(pt.y, 2));
	}


	// 距離を算出 (upper)
	inline constexpr double distance(const pt_upper auto& pt1, const pt_upper auto& pt2) noexcept {
		return std::sqrt(std::pow(pt2.X - pt1.X, 2) + std::pow(pt2.Y - pt1.Y, 2));
	}


	inline constexpr double distance(const pt_upper auto& pt) noexcept {
		return std::sqrt(std::pow(pt.X, 2) + std::pow(pt.Y, 2));
	}


	// ラジアン -> 度
	inline constexpr auto rad2deg(auto rad) noexcept {
		return (decltype(rad))180 * rad / std::numbers::pi_v<decltype(rad)>;
	}


	// 度 -> ラジアン
	inline constexpr auto deg2rad(auto deg) noexcept {
		return std::numbers::pi_v<decltype(deg)> * deg / (decltype(deg))180;
	}


	// 浮動小数点の誤差を考慮した等価比較
	template <typename T>
	inline constexpr bool real_equal(T val1, T val2) noexcept {
		const T diff = val1 - val2;
		return mkaul::in_range(diff, -std::numeric_limits<T>::epsilon(), std::numeric_limits<T>::epsilon());
	}

	template <typename T>
	inline constexpr bool real_in_range(T val, T min, T max, bool equal = true) noexcept {
		if (min - std::numeric_limits<T>::epsilon() <= val and val <= max + std::numeric_limits<T>::epsilon()) {
			if (real_equal(val, min) or real_equal(val, max)) {
				return equal;
			}
			else return true;
		}
		else return false;
	}

	// 符号関数
	inline constexpr auto sign(auto val) noexcept {
		if (val < (decltype(val))0) {
			return (decltype(val))(-1);
		}
		else if (val == (decltype(val))0) {
			return (decltype(val))0;
		}
		else {
			return (decltype(val))1;
		}
	}


	template<typename T>
	inline constexpr T clamp(T val, T min_or_max1, T min_or_max2) {
		return std::clamp(val, std::min(min_or_max1, min_or_max2), std::max(min_or_max1, min_or_max2));
	}
}