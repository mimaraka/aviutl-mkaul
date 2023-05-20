//----------------------------------------------------------------------------------
//		MKAul (Utils)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	// 範囲内判定
	template <typename T>
	inline bool in_range(T val, T min, T max, bool equal)
	{
		if (val >= min && val <= max) {
			if (val == min || val == max)
				return equal;
			else
				return true;
		}
		else
			return false;
	}


	// 距離を算出
	template <typename Pt_Type>
	inline double distance(const Pt_Type& pt1, const Pt_Type& pt2)
	{
		return std::sqrt(std::pow(pt2.x - pt1.x, 2) + std::pow(pt2.y - pt1.y, 2));
	}


	// 文字列を分割
	inline std::vector<std::string> split(const std::string& s, TCHAR c)
	{
		std::vector<std::string> elems;
		std::string item;
		for (TCHAR ch : s) {
			if (ch == c) {
				if (!item.empty()) {
					elems.emplace_back(item);
					item.clear();
				}
			}
			else item += ch;
		}
		if (!item.empty())
			elems.emplace_back(item);

		return elems;
	}


	// ラジアン -> 度
	template <typename T>
	inline T rad2deg(T rad)
	{
		return (T)180 * rad / std::numbers::pi_v<T>;
	}


	// 度 -> ラジアン
	template <typename T>
	inline T deg2rad(T deg)
	{
		return std::numbers::pi_v<T> *deg / (T)180;
	}


	// 符号関数
	template <typename T>
	inline T sign(T value)
	{
		if ((T)0 > value)
			return (T)(-1);
		else if (value == (T)0)
			return (T)0;
		else return (T)1;
	}
}