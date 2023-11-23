#pragma once

#include "gdiplus_common.hpp"
#include "path_base.hpp"



namespace mkaul {
	namespace graphics {
		// �p�X
		struct GdiplusPath : public Path, protected GdiplusBase {
		public:
			using Path::Path;

			void release() override;

			bool begin(const Point<float>& pt) override;
			void end(FigureEnd fe = FigureEnd::Closed) override;

			// �ʂ�ǉ�
			void add_arc(
				const Size<float>& size,
				float angle_start,
				float angle_sweep
			) override;

			// ����ǉ�
			void add_line(
				const Point<float>& pt
			) override;

			// �x�W�F��ǉ�
			void add_bezier(
				const Point<float>& pt_0,
				const Point<float>& pt_1,
				const Point<float>& pt_2
			) override;
		};
	}
}