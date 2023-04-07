//----------------------------------------------------------------------------------
//		MKAul (Utils)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"



namespace mkaul {
	// ”ÍˆÍ“à”»’è
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


	// ‹——£‚ðŽZo
	template <class Pt>
	inline double distance(const Pt& pt1, const Pt& pt2)
	{
		return std::sqrt(std::pow(pt2.x - pt1.x, 2) + std::pow(pt2.y - pt1.y, 2));
	}


	// •¶Žš—ñ‚ð•ªŠ„
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
}