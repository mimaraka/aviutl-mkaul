//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include "graphics_common.hpp"

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		class Graphics_Gdiplus {
		private:
			static ULONG_PTR gdiplus_token;
			Gdiplus::Graphics* p_graphics_buffer;
			Gdiplus::Bitmap* p_bitmap_buffer;

		public:
			Graphics_Gdiplus() :
				p_graphics_buffer(nullptr),
				p_bitmap_buffer(nullptr)
			{}

			static bool startup();
			static void shutdown();

			// Stroke�̏���Pen�ɔ��f
			static void apply_pen_style(
				Gdiplus::Pen* p_pen,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// �C���X�^���X������
			bool init_instance(HWND hwnd);
			// �C���X�^���X�I��
			bool exit_instance();

			// �`��J�n
			void begin_draw(HWND hwnd);
			// �`��I��
			bool end_draw(HWND hwnd);

			// ����`��
			void draw_line(
				const Point<float>& pt_from,
				const Point<float>& pt_to,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ����`��(����)
			void draw_lines(
				const Point<float>* p_pt,
				size_t n_points,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ��`��`��(��)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ��`��`��(�h��)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8
			);
		};
	}
}