#pragma once

#include <string>



namespace mkaul {
	namespace graphics {
		// ƒtƒHƒ“ƒg
		struct Font {
			enum class FontStyle : uint32_t {
				Regular = 0u,
				Italic = 1u << 0,
				UnderLine = 1u << 1
			} style;

			std::string family_name;
			float height;
			int weight;

			Font(
				const std::string& family_name_ = "Meiryo",
				float height_ = 24.f,
				FontStyle style_ = FontStyle::Regular,
				int weight_ = 500
			) :
				style(style_),
				family_name(family_name_),
				height(height_),
				weight(weight_)
			{}
		};
	}
}