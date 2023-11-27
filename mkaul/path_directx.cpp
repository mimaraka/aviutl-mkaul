#include "path_directx.hpp"
#include "graphics_directx.hpp"
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")



namespace mkaul {
	namespace graphics {
		void DirectxPath::release()
		{
			auto p_path = reinterpret_cast<ID2D1PathGeometry*>(data[0]);
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			dx_release(&p_sink);
			dx_release(&p_path);
		}


		bool DirectxPath::begin(const Point<float>& pt)
		{
			auto pp_path = reinterpret_cast<ID2D1PathGeometry**>(&data[0]);
			auto pp_sink = reinterpret_cast<ID2D1GeometrySink**>(&data[1]);
			data[0] = nullptr;
			data[1] = nullptr;

			auto hresult = DirectxGraphics::get_factory()->CreatePathGeometry(pp_path);

			if (SUCCEEDED(hresult))
				hresult = (*pp_path)->Open(pp_sink);

			if (SUCCEEDED(hresult)) {
				(*pp_sink)->BeginFigure(
					pt.to<D2D1_POINT_2F>(),
					D2D1_FIGURE_BEGIN_FILLED
				);
				pt_last = pt;

				return true;
			}

			// 失敗した場合
			dx_release(pp_sink);
			dx_release(pp_path);

			return false;
		}


		void DirectxPath::end(FigureEnd fe)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			p_sink->EndFigure((D2D1_FIGURE_END)fe);
			p_sink->Close();
		}


		// 弧を追加
		void DirectxPath::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			if (p_sink) {
				D2D1_SWEEP_DIRECTION sd;
				D2D1_ARC_SIZE as;

				// 弧の開始・終了地点(楕円の中心からの相対座標)
				// 弧の終了地点
				// 楕円の中心
				Point<float> pt_ofs_start, pt_ofs_end, pt_o;

				// 角度を-360d ~ 360dの範囲に収める
				angle_sweep = std::fmodf(angle_sweep, 360.f);

				ellipse_pos(size, angle_start, &pt_ofs_start);
				ellipse_pos(size, angle_start + angle_sweep, &pt_ofs_end);

				pt_o = pt_last - pt_ofs_start;
				// 最後の点を更新
				pt_last = pt_o + pt_ofs_end;

				// 時計回り
				if (0 < angle_sweep) {
					sd = D2D1_SWEEP_DIRECTION_CLOCKWISE;

					// 弧の角度の大きさが180dより大きい場合
					if (180.f < angle_sweep)
						as = D2D1_ARC_SIZE_LARGE;
					// 弧の角度の大きさがπより小さい場合
					else
						as = D2D1_ARC_SIZE_SMALL;
				}
				// 反時計回り
				else {
					sd = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

					// 弧の角度の大きさが180dより大きい場合
					if (angle_sweep < -180.f)
						as = D2D1_ARC_SIZE_LARGE;
					// 弧の角度の大きさがπより小さい場合
					else
						as = D2D1_ARC_SIZE_SMALL;
				}


				p_sink->AddArc(
					D2D1::ArcSegment(
						pt_last.to<D2D1_POINT_2F>(),
						size.to<D2D1_SIZE_F>(),
						0.f,
						sd,
						as
					)
				);
			}
		}


		// 線を追加
		void DirectxPath::add_line(
			const Point<float>& pt
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			if (p_sink) {
				// 最後の点を更新
				pt_last = pt;

				p_sink->AddLine(
					pt.to<D2D1_POINT_2F>()
				);
			}
		}


		// ベジェ曲線を追加
		void DirectxPath::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			if (p_sink) {
				// 最後の点を更新
				pt_last = pt_2;

				p_sink->AddBezier(
					D2D1::BezierSegment(
						pt_0.to<D2D1_POINT_2F>(),
						pt_1.to<D2D1_POINT_2F>(),
						pt_2.to<D2D1_POINT_2F>()
					)
				);
			}
		}
	}
}