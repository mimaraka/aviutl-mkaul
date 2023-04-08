//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include "graphics_common.hpp"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "WindowsCodecs.lib" )



namespace mkaul {
	namespace graphics {
		class Graphics_Directx {
		private:
			static ID2D1Factory* p_factory;
			static IDWriteFactory* p_write_factory;
			static IWICImagingFactory* p_imaging_factory;
			ID2D1HwndRenderTarget* p_render_target;
			ID2D1SolidColorBrush* p_brush;
			PAINTSTRUCT ps;

			// DirectXƒIƒuƒWƒFƒNƒg‚ÌŠJ•ú
			template <class Interface>
			inline static void release(Interface** pp_object)
			{
				if (*pp_object != nullptr) {
					(*pp_object)->Release();
					(*pp_object) = nullptr;
				}
			}

		public:
			Graphics_Directx() :
				p_render_target(nullptr),
				p_brush(nullptr),
				ps({ NULL })
			{}

			// •`‰æŠÂ‹«‚Ì—pˆÓ
			static bool startup();
			// •`‰æŠÂ‹«‚Ì”jŠü
			static void shutdown();

			static void stroke_to_d2d_strokestyle(
				const Stroke& stroke,
				ID2D1StrokeStyle** pp_stroke_style
			);

			bool init_instance(HWND hwnd);
			void exit_instance();
			void begin_draw(HWND hwnd);
			bool end_draw(HWND hwnd);
			void resize(HWND hwnd);

			// ü‚ğ•`‰æ
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ü‚ğ•`‰æ(•¡”)
			void draw_lines(
				Point<float>* p_point,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ‹éŒ`‚ğ•`‰æ(ü)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ‹éŒ`‚ğ•`‰æ(“h‚è)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			);

			// ‘È‰~‚ğ•`‰æ(ü)(’†S“_w’è)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ‘È‰~‚ğ•`‰æ(ü)(‹éŒ`w’è)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ‘È‰~‚ğ•`‰æ(“h‚è)(’†S“_w’è)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f
			);

			// ‘È‰~‚ğ•`‰æ(“h‚è)(‹éŒ`w’è)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			);
		};
	}
}