#include "path_gdiplus.hpp"
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// デストラクタ
		GdiplusPath::~GdiplusPath() noexcept {
			release();
		}


		void GdiplusPath::release() noexcept {
			gdip_release(reinterpret_cast<Gdiplus::GraphicsPath**>(&data_[0]));
		}


		// パスの開始
		bool GdiplusPath::begin(const Point<float>& pt) noexcept {
			if (!data_[0]) {
				data_[0] = new Gdiplus::GraphicsPath;
				pt_last_ = pt;
				return true;
			}
			else return false;
		}


		// パスの終了
		void GdiplusPath::end(FigureEnd fe) noexcept {
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data_[0]);

			if (fe == FigureEnd::Closed) {
				p_path->CloseAllFigures();
			}
		}


		// 弧を追加
		void GdiplusPath::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		) noexcept {
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data_[0]);
			Point<float> pt_ofs_start, pt_ofs_end, pt_o;

			// 角度を-360d ~ 360dの範囲に収める
			angle_sweep = std::fmodf(angle_sweep, 360.f);

			ellipse_pos(size, angle_start, &pt_ofs_start);
			ellipse_pos(size, angle_start + angle_sweep, &pt_ofs_end);
			pt_o = pt_last_ - pt_ofs_start;
			// 最後の点を更新
			pt_last_ = pt_o + pt_ofs_end;

			// 座標のずれ補正
			if (p_path) {
				p_path->AddArc(
					Gdiplus::RectF{
						pt_o.x - size.width - 0.5f,
						pt_o.y - size.height - 0.5f,
						size.width * 2.f,
						size.height * 2.f
					},
					angle_start - 90.f,
					angle_sweep
				);
			}
		}


		// 線を追加
		void GdiplusPath::add_line(
			const Point<float>& pt
		) noexcept {
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data_[0]);

			// 座標のずれ補正
			if (p_path) {
				p_path->AddLine(
					Gdiplus::PointF{ pt_last_.x - 0.5f, pt_last_.y - 0.5f },
					Gdiplus::PointF{ pt.x - 0.5f, pt.y - 0.5f }
				);
			}
			// 最後の点を更新
			pt_last_ = pt;
		}


		// ベジェ曲線を追加
		void GdiplusPath::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		) noexcept {
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data_[0]);

			// 座標のずれ補正
			if (p_path) {
				p_path->AddBezier(
					Gdiplus::PointF{ pt_last_.x - 0.5f, pt_last_.y - 0.5f },
					Gdiplus::PointF{ pt_0.x - 0.5f, pt_0.y - 0.5f },
					Gdiplus::PointF{ pt_1.x - 0.5f, pt_1.y - 0.5f },
					Gdiplus::PointF{ pt_2.x - 0.5f, pt_2.y - 0.5f }
				);
			}
			// 最後の点を更新
			pt_last_ = pt_2;
		}
	}
}