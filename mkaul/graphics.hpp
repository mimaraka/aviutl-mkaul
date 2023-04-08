//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_gdiplus.hpp"
#include "graphics_directx.hpp"



namespace mkaul {
	namespace graphics {
		class Graphics {
		private:
			enum class Drawing_Method {
				Null,
				Gdiplus,
				Directx
			} static drawing_method;
			Graphics_Gdiplus graphics_gdiplus;
			Graphics_Directx graphics_directx;
			HWND hwnd;

		public:
			

			// �R���X�g���N�^
			Graphics() :
				graphics_gdiplus(),
				graphics_directx(),
				hwnd(NULL)
			{}

			static bool startup(Drawing_Method drawing_method_);
			static bool shutdown();
			bool init_instance(HWND hwnd_);
			bool exit_instance();

			// �`��J�n
			bool begin_draw();
			// �`��I��
			bool end_draw();

			// ���T�C�Y
			void resize();

			// ����`��
			bool draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ����`��(����)
			bool draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ��`��`��(��)
			bool draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ��`��`��(�h��)
			bool fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			);

			// �ȉ~��`��(��)(���S�_�w��)
			bool draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// �ȉ~��`��(��)(��`�w��)
			bool draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// �ȉ~��`��(�h��)(���S�_�w��)
			bool fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f
			);

			// �ȉ~��`��(�h��)(��`�w��)
			bool fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			);
		};
	}
}