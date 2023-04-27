//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_common.hpp"
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// �r�b�g�}�b�v
		class Bitmap_Gdiplus : public Bitmap {
		private:
			Gdiplus::Bitmap* p_bitmap;

		public:
			// ��̉摜���쐬
			bool create(size_t width_, size_t height_) override;
			void release() override;

			// ���\�[�X����ǂݍ���
			bool load_from_resource() override;
			// �t�@�C��������ǂݍ���
			bool load_from_filename() override;

			size_t get_width() override;
			size_t get_height() override;
		};

		// �O���t�B�b�N�X
		class Graphics_Gdiplus : public Graphics {
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
				const Color_F& color_f,
				const Stroke& stroke
			);

			// �C���X�^���X������
			bool init(HWND hwnd) override;
			// �C���X�^���X�I��
			void exit() override;

			// �`��J�n
			void begin_draw() override;
			// �`��I��
			bool end_draw() override;

			// ����`��
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// ����`��(����)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// �x�W�F�Ȑ���`��
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// �x�W�F�Ȑ���`��(����)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// ��`��`��(��)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// ��`��`��(�h��)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			) override;

			// �ȉ~��`��(��)(���S�_�w��)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// �ȉ~��`��(��)(��`�w��)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_i8,
				const Stroke& stroke
			) override;

			// �ȉ~��`��(�h��)(���S�_�w��)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f
			) override;

			// �ȉ~��`��(�h��)(��`�w��)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			) override;
		};
	}
}