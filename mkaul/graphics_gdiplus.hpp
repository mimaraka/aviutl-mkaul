#pragma once

#include "graphics_base.hpp"
#include "gdiplus_common.hpp"
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// �O���t�B�b�N�X
		class GdiplusGraphics : public Graphics, protected GdiplusBase {
		private:
			inline static ULONG_PTR gdiplus_token_ = NULL;
			Gdiplus::Graphics* p_graphics_buffer_;
			Gdiplus::Bitmap* p_bitmap_buffer_;

		public:
			GdiplusGraphics() :
				p_graphics_buffer_(nullptr),
				p_bitmap_buffer_(nullptr)
			{}

			static bool startup();
			static void shutdown();

			// Stroke�̏���Pen�ɔ��f
			static void apply_pen_style(
				Gdiplus::Pen* p_pen,
				const ColorF& color,
				const Stroke& stroke
			) noexcept;

			// Brush�ɐF�𔽉f
			static void apply_brush_color(
				Gdiplus::SolidBrush* p_brush_,
				const ColorF& color
			) noexcept;

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
				const ColorF& color = 0
			) override;

			// ����`��
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ����`��(����)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �x�W�F�Ȑ���`��
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �x�W�F�Ȑ���`��(����)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ��`��`��(��)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ��`��`��(�h��)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& color = 0
			) override;

			// �ȉ~��`��(��)(���S�_�w��)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �ȉ~��`��(��)(��`�w��)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �ȉ~��`��(�h��)(���S�_�w��)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& color = 0
			) override;

			// �ȉ~��`��(�h��)(��`�w��)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& color = 0
			) override;

			// �p�X��`��(��)
			void draw_path(
				const Path* p_path,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) override;

			// �p�X��`��(�h��)
			void fill_path(
				const Path* p_path,
				const ColorF& color = 0
			) override;

			// ��̃r�b�g�}�b�v���쐬
			bool initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				const ColorF& color = 0
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
				AnchorPosition anchor_pos,
				float opacity = 1.f
			) override;

			// �r�b�g�}�b�v��`��(��`�w��)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) override;

			// �e�L�X�g��`��(�A���J�[�|�C���g�w��)
			void draw_text(
				const std::string& text,
				const Point<float>& point,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				const ColorF& color = 0
			) override;

			// �e�L�X�g��`��(��`�w��)
			void draw_text(
				const std::string& text,
				const Rectangle<float>& rect,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				bool fit_size = true,
				const ColorF& color = 0
			) override;
		};
	}
}