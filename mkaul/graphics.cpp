//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics.hpp"



namespace mkaul {
	namespace graphics {
		Graphics::Drawing_Method Graphics::drawing_method = Graphics::Drawing_Method::Null;

		// �`����̏���
		inline bool Graphics::startup(Drawing_Method drawing_method_)
		{
			bool result = false;
			// ���ɏ���������Ă���ꍇ
			if (drawing_method != Drawing_Method::Null)
				return false;

			// �`������ŏꍇ����
			switch (drawing_method_) {
			// GDI+
			case Drawing_Method::Gdiplus:
				result = Graphics_Gdiplus::startup();
				break;

			// DirectX
			case Drawing_Method::Directx:
				result = Graphics_Directx::startup();
				break;

			default:
				return false;
			}

			if (result)
				drawing_method = drawing_method_;
			return result;
		}


		// �`����̔j��
		inline bool Graphics::shutdown()
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				Graphics_Gdiplus::shutdown();
				break;

			case Drawing_Method::Directx:
				Graphics_Directx::shutdown();
				break;

			default:
				return false;
			}

			drawing_method = Drawing_Method::Null;
			return true;
		}


		// ������(�C���X�^���X��)
		inline bool Graphics::init_instance(HWND hwnd_)
		{
			bool result = false;
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				result = graphics_gdiplus.init_instance(hwnd_);
				break;

			case Drawing_Method::Directx:
				result = graphics_directx.init_instance(hwnd_);
				break;

			default:
				return false;
			}

			hwnd = hwnd_;
			return result;
		}

		// �I��(�C���X�^���X��)
		inline bool Graphics::exit_instance()
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.exit_instance();
				break;

			case Drawing_Method::Directx:
				graphics_directx.exit_instance();
				break;

			default:
				return false;
			}

			return true;
		}

		// �`��J�n
		bool Graphics::begin_draw()
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.begin_draw(hwnd);
				break;

			case Drawing_Method::Directx:
				graphics_directx.begin_draw(hwnd);
				break;
			
			default:
				return false;
			}

			return true;
		}

		// �`��I��
		bool Graphics::end_draw()
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.end_draw(hwnd);
				break;

			case Drawing_Method::Directx:
				graphics_directx.end_draw(hwnd);
				break;

			default:
				return false;
			}
			return true;
		}


		// ���T�C�Y
		void Graphics::resize()
		{
			if (drawing_method == Drawing_Method::Directx)
				graphics_directx.resize(hwnd);
		}


		// ��`��`��(��)
		bool Graphics::draw_rectangle(
			const Rectangle<float>& rectangle,
			const Stroke& stroke,
			const Color_F& col_f,
			float round_radius_x,
			float round_radius_y
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.draw_rectangle(
					rectangle,
					round_radius_x,
					round_radius_y,
					col_f,
					stroke
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.draw_rectangle(
					rectangle,
					round_radius_x,
					round_radius_y,
					col_f,
					stroke
				);
				break;

			default:
				return false;
			}

			return true;
		}

		// ��`��`��(�h��)
		bool Graphics::fill_rectangle(
			const Rectangle<float>& rectangle,
			const Color_F& color_f,
			float round_radius_x,
			float round_radius_y
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.fill_rectangle(
					rectangle,
					round_radius_x,
					round_radius_y,
					color_f
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.fill_rectangle(
					rectangle,
					round_radius_x,
					round_radius_y,
					color_f
				);
				break;

			default:
				return false;
			}

			return true;
		}
	}
}