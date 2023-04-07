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

			// DirectXIuWFNgΜJϊ
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

			// `ζΒ«ΜpΣ
			static bool startup();
			// `ζΒ«Μjό
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

			// όπ`ζ
			void draw_line(
				const Point<float>& pt_from,
				const Point<float>& pt_to,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// όπ`ζ(‘)
			void draw_lines(
				Point<float>* p_pt,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ι`π`ζ(ό)
			void draw_rectangle(
				const Rectangle<float>& rect,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ι`π`ζ(hθ)
			void fill_rectangle(
				const Rectangle<float>& rect,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			);

			// Θ~π`ζ(ό)(S_wθ)
			void draw_ellipse(
				const Point<float>& pt,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// Θ~π`ζ(ό)(ι`wθ)
			void draw_ellipse(
				const Rectangle<float>& rect,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// Θ~π`ζ(hθ)(S_wθ)
			void fill_ellipse(
				const Point<float>& pt,
				const Color_F& color_f,
				float radius_x,
				float radius_y
			);

			// Θ~π`ζ(hθ)(ι`wθ)
			void fill_ellipse(
				const Rectangle<float>& rect,
				const Color_F& color_f
			);
		};
	}
}