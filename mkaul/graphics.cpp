//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics.hpp"



namespace mkaul {
	namespace graphics {
		Drawing_Method Graphics::drawing_method = Drawing_Method::Null;

		// •`‰æŠÂ‹«‚Ì€”õ
		inline bool Graphics::startup(Drawing_Method drawing_method_)
		{
			bool result = false;
			// Šù‚É‰Šú‰»‚³‚ê‚Ä‚¢‚éê‡
			if (drawing_method != Drawing_Method::Null)
				return false;

			// •`‰æ•û®‚Åê‡•ª‚¯
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


		// •`‰æŠÂ‹«‚Ì”jŠü
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


		// ‰Šú‰»(ƒCƒ“ƒXƒ^ƒ“ƒX–ˆ)
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


		// I—¹(ƒCƒ“ƒXƒ^ƒ“ƒX–ˆ)
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


		// •`‰æŠJn
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


		// •`‰æI—¹
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


		// ƒŠƒTƒCƒY
		void Graphics::resize()
		{
			if (drawing_method == Drawing_Method::Directx)
				graphics_directx.resize(hwnd);
		}


		// ü‚ğ•`‰æ
		bool Graphics::draw_line(
			const Point<float>& point_from,
			const Point<float>& point_to,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.draw_line(
					point_from,
					point_to,
					color_f,
					stroke
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.draw_line(
					point_from,
					point_to,
					color_f,
					stroke
				);
				break;

			default:
				return false;
			}
			return true;
		}


		// ‹éŒ`‚ğ•`‰æ(ü)
		bool Graphics::draw_rectangle(
			const Rectangle<float>& rectangle,
			float round_radius_x = 0.f,
			float round_radius_y = 0.f,
			const Color_F& color_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.draw_rectangle(
					rectangle,
					round_radius_x,
					round_radius_y,
					color_f,
					stroke
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.draw_rectangle(
					rectangle,
					round_radius_x,
					round_radius_y,
					color_f,
					stroke
				);
				break;

			default:
				return false;
			}
			return true;
		}


		// ‹éŒ`‚ğ•`‰æ(“h‚è)
		bool Graphics::fill_rectangle(
			const Rectangle<float>& rectangle,
			float round_radius_x = 0.f,
			float round_radius_y = 0.f,
			const Color_F& color_f = 0
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


		// ‘È‰~‚ğ•`‰æ(ü)(’†S“_w’è)
		bool Graphics::draw_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& col_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.draw_ellipse(
					point,
					radius_x,
					radius_y,
					col_f,
					stroke
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.draw_ellipse(
					point,
					radius_x,
					radius_y,
					col_f,
					stroke
				);
				break;

			default:
				return false;
			}
			return true;
		}


		// ‘È‰~‚ğ•`‰æ(ü)(‹éŒ`w’è)
		bool Graphics::draw_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& col_f = 0,
			const Stroke& stroke = Stroke()
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.draw_ellipse(
					rectangle,
					col_f,
					stroke
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.draw_ellipse(
					rectangle,
					col_f,
					stroke
				);
				break;

			default:
				return false;
			}
			return true;
		}


		// ‘È‰~‚ğ•`‰æ(“h‚è)(’†S“_w’è)
		bool Graphics::fill_ellipse(
			const Point<float>& point,
			float radius_x,
			float radius_y,
			const Color_F& color_f
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.fill_ellipse(
					point,
					radius_x,
					radius_y,
					color_f
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.fill_ellipse(
					point,
					radius_x,
					radius_y,
					color_f
				);
				break;

			default:
				return false;
			}
			return true;
		}


		// ‘È‰~‚ğ•`‰æ(“h‚è)(‹éŒ`w’è)
		bool Graphics::fill_ellipse(
			const Rectangle<float>& rectangle,
			const Color_F& color_f
		)
		{
			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				graphics_gdiplus.fill_ellipse(
					rectangle,
					color_f
				);
				break;

			case Drawing_Method::Directx:
				graphics_directx.fill_ellipse(
					rectangle,
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