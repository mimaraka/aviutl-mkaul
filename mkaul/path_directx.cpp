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

			auto hr = DirectxGraphics::get_factory()->CreatePathGeometry(pp_path);

			if (SUCCEEDED(hr))
				hr = (*pp_path)->Open(pp_sink);

			if (SUCCEEDED(hr)) {
				(*pp_sink)->BeginFigure(
					pt.to<D2D1_POINT_2F>(),
					D2D1_FIGURE_BEGIN_FILLED
				);
				pt_last = pt;

				return true;
			}

			// ���s�����ꍇ
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


		// �ʂ�ǉ�
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

				// �ʂ̊J�n�E�I���n�_(�ȉ~�̒��S����̑��΍��W)
				// �ʂ̏I���n�_
				// �ȉ~�̒��S
				Point<float> pt_ofs_start, pt_ofs_end, pt_o;

				// �p�x��-360d ~ 360d�͈̔͂Ɏ��߂�
				angle_sweep = std::fmodf(angle_sweep, 360.f);

				ellipse_pos(size, angle_start, &pt_ofs_start);
				ellipse_pos(size, angle_start + angle_sweep, &pt_ofs_end);

				pt_o = pt_last - pt_ofs_start;
				// �Ō�̓_���X�V
				pt_last = pt_o + pt_ofs_end;

				// ���v���
				if (0 < angle_sweep) {
					sd = D2D1_SWEEP_DIRECTION_CLOCKWISE;

					// �ʂ̊p�x�̑傫����180d���傫���ꍇ
					if (180.f < angle_sweep)
						as = D2D1_ARC_SIZE_LARGE;
					// �ʂ̊p�x�̑傫�����΂�菬�����ꍇ
					else
						as = D2D1_ARC_SIZE_SMALL;
				}
				// �����v���
				else {
					sd = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

					// �ʂ̊p�x�̑傫����180d���傫���ꍇ
					if (angle_sweep < -180.f)
						as = D2D1_ARC_SIZE_LARGE;
					// �ʂ̊p�x�̑傫�����΂�菬�����ꍇ
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


		// ����ǉ�
		void DirectxPath::add_line(
			const Point<float>& pt
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			if (p_sink) {
				// �Ō�̓_���X�V
				pt_last = pt;

				p_sink->AddLine(
					pt.to<D2D1_POINT_2F>()
				);
			}
		}


		// �x�W�F�Ȑ���ǉ�
		void DirectxPath::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_sink = reinterpret_cast<ID2D1GeometrySink*>(data[1]);

			if (p_sink) {
				// �Ō�̓_���X�V
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