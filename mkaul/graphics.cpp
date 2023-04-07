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

		// 描画環境の準備
		inline bool Graphics::startup(Drawing_Method drawing_method_)
		{
			bool result = false;
			// 既に初期化されている場合
			if (drawing_method != Drawing_Method::Null)
				return false;

			// 描画方式で場合分け
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


		// 描画環境の破棄
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


		// 初期化(インスタンス毎)
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

		// 終了(インスタンス毎)
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

		// 描画開始
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

		// 描画終了
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


		// リサイズ
		void Graphics::resize()
		{
			if (drawing_method == Drawing_Method::Directx)
				graphics_directx.resize(hwnd);
		}


		// 矩形を描画(線)
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

		// 矩形を描画(塗り)
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