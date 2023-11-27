#pragma once

#include "flag.hpp"
#include <string>



namespace mkaul {
	namespace flag {
		enum class FontStyle : uint32_t {
			Regular = 0u,
			Italic = 1u << 0,
			UnderLine = 1u << 1
		};
	}
	namespace graphics {
		// フォント
		struct Font {
			std::string family_name;
			flag::FontStyle style;
			float height;
			int weight;

			Font(
				float height_ = 24.f,
				const std::string& family_name_ = "Meiryo",
				flag::FontStyle style_ = flag::FontStyle::Regular,
				int weight_ = 500
			) :
				height(height_),
				family_name(family_name_),
				style(style_),
				weight(weight_)
			{}
		};
	}
}