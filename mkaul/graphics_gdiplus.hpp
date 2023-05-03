//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_base.hpp"
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// �r�b�g�}�b�v
		struct Bitmap_Gdiplus : public Bitmap {
		public:
			using Bitmap::Bitmap;

			void release() override;

			size_t get_width() override;
			size_t get_height() override;
		};

		struct Geometry_Gdiplus : public Geometry {
		public:
			using Geometry::Geometry;

			bool open() override;
			void close() override;
			void release() override;

			void begin_figure() override;
			void end_figure() override;

			// �ʂ�ǉ�
			void add_arc(
				const Rectangle<float>& rect,
				float angle_from,
				float angle_to
			) override;
			// ����ǉ�
			void add_line(
				const Point<float>& point_0,
				const Point<float>& point_1
			) override;
			// �x�W�F��ǉ�
			void add_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3
			) override;
		};

		// �O���t�B�b�N�X
		class Graphics_Gdiplus : public Graphics {
		private:
			inline static ULONG_PTR gdiplus_token = NULL;
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

			void resize() override {};

			// ����`��
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ����`��(����)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �x�W�F�Ȑ���`��
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �x�W�F�Ȑ���`��(����)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ��`��`��(��)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ��`��`��(�h��)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& color_f = 0
			) override;

			// �ȉ~��`��(��)(���S�_�w��)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �ȉ~��`��(��)(��`�w��)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �ȉ~��`��(�h��)(���S�_�w��)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f = 0
			) override;

			// �ȉ~��`��(�h��)(��`�w��)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f = 0
			) override;

			// ��̃r�b�g�}�b�v���쐬
			void initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				Color_F color_f = 0
			) override;

			// �t�@�C������r�b�g�}�b�v���쐬
			bool load_bitmap_from_filename(
				Bitmap* p_bitmap,
				const std::filesystem::path& path
			) override;

			// ���\�[�X����r�b�g�}�b�v���쐬
			bool load_bitmap_from_resource(
				Bitmap* p_bitmap,
				const char* resource
			) override;

			// �r�b�g�}�b�v��`��(�A���J�[�|�C���g�w��)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Point<float>& point,
				Anchor_Position anchor_pos,
				float opacity = 1.f
			) override;

			// �r�b�g�}�b�v��`��(��`�w��)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) override;
		};
	}
}