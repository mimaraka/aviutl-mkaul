#pragma once

#include "gdiplus_common.hpp"
#include "path_base.hpp"



namespace mkaul {
	namespace graphics {
		// パス
		struct GdiplusPath : public Path, protected GdiplusBase {
		public:
			using Path::Path;
			~GdiplusPath() noexcept;

			void release() noexcept override;
			bool begin(const Point<float>& pt) noexcept override;
			void end(FigureEnd fe = FigureEnd::Closed) noexcept override;

			// 弧を追加
			void add_arc(
				const Size<float>& size,
				float angle_start,
				float angle_sweep
			) noexcept override;

			// 線を追加
			void add_line(
				const Point<float>& pt
			) noexcept override;

			// ベジェを追加
			void add_bezier(
				const Point<float>& pt_0,
				const Point<float>& pt_1,
				const Point<float>& pt_2
			) noexcept override;
		};
	}
}