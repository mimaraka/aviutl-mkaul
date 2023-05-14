//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_base.hpp"
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "WindowsCodecs.lib" )



namespace mkaul {
	namespace graphics {
		// DirectX���ʂ̊��N���X
		class Directx_Base {
		protected:
			// DirectX�I�u�W�F�N�g�̊J��
			template <class Interface>
			static void dx_release(Interface** pp_obj)
			{
				if (*pp_obj != nullptr) {
					(*pp_obj)->Release();
					(*pp_obj) = nullptr;
				}
			}
		};


		// �r�b�g�}�b�v
		struct Bitmap_Directx : public Bitmap, protected Directx_Base {
		public:
			using Bitmap::Bitmap;

			void release() override;

			size_t get_width() override;
			size_t get_height() override;
		};


		// �W�I���g��
		struct Path_Directx : public Path, protected Directx_Base {
		public:
			using Path::Path;

			void release() override;

			bool begin(const Point<float>& pt) override;
			void end(Figure_End fe = Figure_End::Closed) override;

			// �ʂ�ǉ�
			void add_arc(
				float radius_x,
				float radius_y,
				float angle_from,
				float angle_to
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


		// �O���t�B�b�N
		class Graphics_Directx : public Graphics, protected Directx_Base {
		private:
			inline static ID2D1Factory* p_factory;
			inline static IDWriteFactory* p_write_factory;
			inline static IWICImagingFactory* p_imaging_factory;
			ID2D1HwndRenderTarget* p_render_target;
			ID2D1SolidColorBrush* p_brush;
			PAINTSTRUCT ps;

		public:
			Graphics_Directx() :
				p_render_target(nullptr),
				p_brush(nullptr),
				ps({ 0 })
			{}

			// �`����̗p��
			static bool startup();
			// �`����̔j��
			static void shutdown();

			static void stroke_to_d2d_strokestyle(
				const Stroke& stroke,
				ID2D1StrokeStyle** pp_stroke_style
			);

			// �t�@�N�g���[�E�^�[�Q�b�g�擾
			static auto get_factory() { return p_factory; }
			static auto get_write_factory() { return p_write_factory; }
			static auto get_imaging_factory() { return p_imaging_factory; }
			auto get_render_target() { return p_render_target; }

			bool init(HWND hwnd_) override;
			void exit() override;
			void begin_draw() override;
			bool end_draw() override;
			void resize() override;

			// ����`��
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ����`��(����)
			void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

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