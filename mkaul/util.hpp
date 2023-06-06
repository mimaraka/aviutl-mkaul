//----------------------------------------------------------------------------------
//		MKAul (Utils)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	template <typename T>
	concept PtType = requires (T pt) {
		pt.x; pt.y;
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
	inline double distance(const PtType auto& pt1, const PtType auto& pt2) noexcept
	{
		return std::sqrt(std::pow(pt2.x - pt1.x, 2) + std::pow(pt2.y - pt1.y, 2));
	}


	// ������𕪊�
	inline auto split(const std::string& str, const char spr)
	{
		std::vector<std::string> elems;
		std::string item;
		for (const char c : str) {
			if (c == spr) {
				if (!item.empty()) {
					elems.emplace_back(item);
					item.clear();
				}
			}
			else item += c;
		}
		if (!item.empty())
			elems.emplace_back(item);

		return elems;
	}


	// �����������
	inline auto join(const std::vector<std::string>& vec_str, const std::string& spr)
	{
		std::string result;
		for (auto str : vec_str) {
			result += str + spr;
		}
		result.erase(result.size() - spr.size());
		return result;
	}


	// ���W�A�� -> �x
	inline constexpr auto rad2deg(auto rad) noexcept
	{
		return (decltype(rad))180 * rad / std::numbers::pi_v<decltype(rad)>;
	}


	// �x -> ���W�A��
	inline constexpr auto deg2rad(auto deg) noexcept
	{
		return std::numbers::pi_v<decltype(deg)> *deg / (decltype(deg))180;
	}


	// �����֐�
	inline constexpr auto sign(auto value) noexcept
	{
		if ((decltype(value))0 > value)
			return (decltype(value))(-1);
		else if (value == (decltype(value))0)
			return (decltype(value))0;
		else return (decltype(value))1;
	}
}