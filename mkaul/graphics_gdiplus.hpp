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
		// GDI+���ʂ̊��N���X
		class Gdiplus_Base {
		protected:
			// �I�u�W�F�N�g���J��
			template <class Interface>
			static void gdip_release(Interface** pp_obj)
			{
				if (*pp_obj) {
					delete (*pp_obj);
					(*pp_obj) = nullptr;
				}
			}
		};


		// �r�b�g�}�b�v
		struct Bitmap_Gdiplus : public Bitmap, protected Gdiplus_Base {
		public:
			using Bitmap::Bitmap;

			void release() override;

			size_t get_width() override;
			size_t get_height() override;
		};


		// �p�X
		struct Path_Gdiplus : public Path, protected Gdiplus_Base {
		public:
			using Path::Path;

			void release() override;

			bool begin(const Point<float>& pt) override;
			void end(Figure_End fe = Figure_End::Closed) override;

			// �ʂ�ǉ�
			void add_arc(
				const Size<float>& size,
				float angle_start,
				float angle_sweep
			) override;

			// ����ǉ�
			void add_line(
				const Point<float>& pt
			) override;

			// �x�W�F��ǉ�
			void add_bezier(
				const Point<float>& pt_0,
				const Point<float>& pt_1,
				const Point<float>& pt_2
			) override;
		};


		// �O���t�B�b�N�X
		class Graphics_Gdiplus : public Graphics, protected Gdiplus_Base {
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
				const Color_F& col_f,
				const Stroke& stroke
			);

			// �C���X�^���X������
			bool init(HWND hwnd) override;
			// �C���X�^���X�I��
			void exit() override;

			// �`��J�n
			bool begin_draw() override;
			// �`��I��
			bool end_draw() override;

			bool resize() override { return true; };

			// �w�i��h��Ԃ�
			void fill_background(
				const Color_F& col_f = 0
			) override;

			// ����`��
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ����`��(����)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �x�W�F�Ȑ���`��
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �x�W�F�Ȑ���`��(����)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ��`��`��(��)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ��`��`��(�h��)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& col_f = 0
			) override;

			// �ȉ~��`��(��)(���S�_�w��)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �ȉ~��`��(��)(��`�w��)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �ȉ~��`��(�h��)(���S�_�w��)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& col_f = 0
			) override;

			// �ȉ~��`��(�h��)(��`�w��)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& col_f = 0
			) override;

			// ��̃r�b�g�}�b�v���쐬
			bool initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				Color_F col_f = 0
			) override;

			// �t�@�C������r�b�g�}�b�v���쐬
			bool load_bitmap_from_filename(
				Bitmap* p_bitmap,
				const std::filesystem::path& path
			) override;

			// ���\�[�X����r�b�g�}�b�v���쐬
			bool load_bitmap_from_resource(
				Bitmap* p_bitmap,
				HINSTANCE hinst,
				const char* res_name,
				const char* res_type = RT_BITMAP
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