#include "path_gdiplus.hpp"
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// �J��
		void GdiplusPath::release()
		{
			gdip_release(reinterpret_cast<Gdiplus::GraphicsPath**>(&data[0]));
		}


		// �p�X�̊J�n
		bool GdiplusPath::begin(const Point<float>& pt)
		{
			if (!data[0]) {
				auto p_path = new Gdiplus::GraphicsPath;
				data[0] = p_path;

				pt_last = pt;

				return true;
			}
			else return false;
		}


		// �p�X�̏I��
		void GdiplusPath::end(FigureEnd fe)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			if (fe == FigureEnd::Closed)
				p_path->CloseAllFigures();
		}


		// �ʂ�ǉ�
		void GdiplusPath::add_arc(
			const Size<float>& size,
			float angle_start,
			float angle_sweep
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);
			Point<float> pt_ofs_start, pt_ofs_end, pt_o;

			// �p�x��-360d ~ 360d�͈̔͂Ɏ��߂�
			angle_sweep = std::fmodf(angle_sweep, 360.f);

			ellipse_pos(size, angle_start, &pt_ofs_start);
			ellipse_pos(size, angle_start + angle_sweep, &pt_ofs_end);
			pt_o = pt_last - pt_ofs_start;
			// �Ō�̓_���X�V
			pt_last = pt_o + pt_ofs_end;

			// ���W�̂���␳
			if (p_path) p_path->AddArc(
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


		// ����ǉ�
		void GdiplusPath::add_line(
			const Point<float>& pt
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			// ���W�̂���␳
			if (p_path) p_path->AddLine(
				Gdiplus::PointF{ pt_last.x - 0.5f, pt_last.y - 0.5f },
				Gdiplus::PointF{ pt.x - 0.5f, pt.y - 0.5f }
			);
			// �Ō�̓_���X�V
			pt_last = pt;
		}


		// �x�W�F�Ȑ���ǉ�
		void GdiplusPath::add_bezier(
			const Point<float>& pt_0,
			const Point<float>& pt_1,
			const Point<float>& pt_2
		)
		{
			auto p_path = reinterpret_cast<Gdiplus::GraphicsPath*>(data[0]);

			// ���W�̂���␳
			if (p_path) p_path->AddBezier(
				Gdiplus::PointF{ pt_last.x - 0.5f, pt_last.y - 0.5f },
				Gdiplus::PointF{ pt_0.x - 0.5f, pt_0.y - 0.5f },
				Gdiplus::PointF{ pt_1.x - 0.5f, pt_1.y - 0.5f },
				Gdiplus::PointF{ pt_2.x - 0.5f, pt_2.y - 0.5f }
			);
			// �Ō�̓_���X�V
			pt_last = pt_2;
		}
	}
}