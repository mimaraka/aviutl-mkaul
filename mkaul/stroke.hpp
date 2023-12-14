#pragma once



namespace mkaul {
	namespace graphics {
		// ストローク
		struct Stroke {
		public:
			float width;
			enum class DashStyle {
				Solid,
				Dash,
				Dot,
				DashDot,
				DashDotDot,
				Custom
			} dash_style;
			enum class CapStyle {
				Flat,
				Square,
				Round,
				Triangle
			} start_cap, end_cap, dash_cap;
			enum class LineJoin {
				Miter,
				Bevel,
				Round
			} line_join;
			float* custom_dashes;
			int dashes_count;
			float dash_offset;

			constexpr Stroke(
				float width_ = 1.f,
				DashStyle dash_style_ = DashStyle::Solid,
				CapStyle start_cap_ = CapStyle::Round,
				CapStyle end_cap_ = CapStyle::Round,
				CapStyle dash_cap_ = CapStyle::Round,
				LineJoin line_join_ = LineJoin::Round,
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