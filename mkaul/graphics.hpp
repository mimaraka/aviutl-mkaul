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
			

			// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
			Graphics() :
				graphics_gdiplus(),
				graphics_directx(),
				hwnd(NULL)
			{}

			static bool startup(Drawing_Method drawing_method_);
			static bool shutdown();
			bool init_instance(HWND hwnd_);
			bool exit_instance();

			// •`‰æŠJn
			bool begin_draw();
			// •`‰æI—¹
			bool end_draw();

			// ƒŠƒTƒCƒY
			void resize();

			// ‹éŒ`‚ğ•`‰æ(ü)
			bool draw_rectangle(
				const Rectangle<float>& rectangle,
				const Stroke& stroke,
				const Color_F& color_f,
				float round_radius_x,
				float round_radius_y
			);

			// ‹éŒ`‚ğ•`‰æ(“h‚è)
			bool fill_rectangle(
				const Rectangle<float>& rectangle,
				const Color_F& color_f,
				float round_radius_x,
				float round_radius_y
			);

			// ‘È‰~‚ğ•`‰æ(ü)(’†S“_w’è)
			bool draw_ellipse(
				const Point<float>& point,
				const Stroke& stroke,
				const Color_F& color_f,
				float radius_x,
				float radius_y
			);

			// ‘È‰~‚ğ•`‰æ(ü)(‹éŒ`w’è)
			bool draw_ellipse(
				const Rectangle<float>& rectangle,
				const Stroke& stroke,
				const Color_F& color_f
			);

			// ‘È‰~‚ğ•`‰æ(“h‚è)(’†S“_w’è)
			bool fill_ellipse(
				const Point<float>& point,
				const Color_F& color_f,
				float radius_x,
				float radius_y
			);

			// ‘È‰~‚ğ•`‰æ(“h‚è)(‹éŒ`w’è)
			bool fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			);
		};
	}
}