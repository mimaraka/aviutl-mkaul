//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_common.hpp"



namespace mkaul {
	namespace graphics {
		class Graphics_Gdiplus {
		private:
			static ULONG_PTR gdiplus_token;
			Gdiplus::Graphics* p_graphics_buffer;
			Gdiplus::Bitmap* p_bitmap_buffer;

		public:
			Graphics_Gdiplus() :
				p_graphics_buffer(nullptr),
				p_bitmap_buffer(nullptr)
			{}

			static bool startup();
			static void shutdown();

			// StrokeΜξρπPenΙ½f
			static void apply_pen_style(
				Gdiplus::Pen* p_pen,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// CX^Xϊ»
			bool init_instance(HWND hwnd);
			// CX^XIΉ
			bool exit_instance();

			// `ζJn
			void begin_draw(HWND hwnd);
			// `ζIΉ
			bool end_draw(HWND hwnd);

			// όπ`ζ
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// όπ`ζ(‘)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// xWFΘόπ`ζ
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// xWFΘόπ`ζ(‘)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ι`π`ζ(ό)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ι`π`ζ(hθ)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8
			);

			// Θ~π`ζ(ό)(S_wθ)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// Θ~π`ζ(ό)(ι`wθ)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// Θ~π`ζ(hθ)(S_wθ)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_I8& color_i8
			);

			// Θ~π`ζ(hθ)(ι`wθ)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_I8& color_i8
			);
		};
	}
}