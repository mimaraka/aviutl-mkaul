#pragma once

#include "point.hpp"
#include "size.hpp"



namespace mkaul {
	namespace graphics {
		// パス
		struct Path {
		protected:
			void* data[2];
			Point<float> pt_last;

			void ellipse_pos(const Size<float>& size, float angle, Point<float>* p_pt);

		public:
			Path() : data(), pt_last() {}

			enum class FigureEnd {
				Open,
				Closed
			};

			virtual void release() = 0;

			template <typename Ptr>
			Ptr get_data(int idx) const { return reinterpret_cast<Ptr>(data[idx]); }

			virtual bool begin(const Point<float>& pt) = 0;
			virtual void end(FigureEnd fe = FigureEnd::Closed) = 0;

			// 弧を追加
			virtual void add_arc(const Size<float>& size, float angle_start, float angle_sweep) = 0;
			// 線を追加
			virtual void add_line(const Point<float>& pt) = 0;
			// ベジェを追加
			virtual void add_bezier(const Point<float>& pt_0, const Point<float>& pt_1, const Point<float>& pt_2) = 0;
		};
	}
}