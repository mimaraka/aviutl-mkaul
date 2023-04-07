//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "point.hpp"
#include "rectangle.hpp"
#include "color.hpp"



namespace mkaul {
	namespace graphics {
		// ストローク
		struct Stroke {
		public:
			float width;
			enum class Dash_Style {
				Solid,
				Dash,
				Dot,
				Dash_Dot,
				Dash_Dot_Dot,
				Custom
			} dash_style;
			enum class Cap_Style {
				Flat,
				Square,
				Round,
				Triangle
			} start_cap, end_cap, dash_cap;
			enum class Line_Join {
				Miter,
				Bevel,
				Round
			} line_join;
			float* custom_dashes;
			int dashes_count;
			float dash_offset;

			Stroke(
				float width_ = 1.f,
				Dash_Style dash_style_ = Dash_Style::Solid,
				Cap_Style start_cap_ = Cap_Style::Round,
				Cap_Style end_cap_ = Cap_Style::Round,
				Cap_Style dash_cap_ = Cap_Style::Round,
				Line_Join line_join_ = Line_Join::Round,
				float* custom_dashes_ = nullptr,
				int dashes_count_ = 0,
				float dash_offset_ = 0.f
			) :
				width(width_),
				dash_style(dash_style_),
				start_cap(start_cap_),
				end_cap(end_cap_),
				dash_cap(dash_cap_),
				line_join(line_join_),
				custom_dashes(custom_dashes_),
				dashes_count(dashes_count_),
				dash_offset(dash_offset_)
			{}
		};
	}
}