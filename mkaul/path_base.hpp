#pragma once

#include "point.hpp"
#include "size.hpp"



namespace mkaul {
	namespace graphics {
		// パス
		struct Path {
		protected:
			void* data_[2];
			Point<float> pt_last_;

			static void ellipse_pos(const Size<float>& size, float angle, Point<float>* p_pt);

		public:
			enum class FigureEnd {
				Open,
				Closed
			};

			Path() : data_(), pt_last_() {}
			virtual ~Path() {}

			virtual void release() noexcept = 0;

			template <typename Ptr>
			Ptr get_data(int idx) const noexcept { return reinterpret_cast<Ptr>(data_[idx]); }

			virtual bool begin(const Point<float>& pt) noexcept = 0;
			virtual void end(FigureEnd fe = FigureEnd::Closed) noexcept = 0;

			// 弧を追加
			virtual void add_arc(const Size<float>& size, float angle_start, float angle_sweep) noexcept = 0;
			// 線を追加
			virtual void add_line(const Point<float>& pt) noexcept = 0;
			// ベジェを追加
			virtual void add_bezier(const Point<float>& pt_0, const Point<float>& pt_1, const Point<float>& pt_2) noexcept = 0;
		};
	}
}