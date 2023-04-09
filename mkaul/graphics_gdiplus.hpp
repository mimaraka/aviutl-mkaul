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

			// Stroke‚Ìî•ñ‚ğPen‚É”½‰f
			static void apply_pen_style(
				Gdiplus::Pen* p_pen,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ƒCƒ“ƒXƒ^ƒ“ƒX‰Šú‰»
			bool init_instance(HWND hwnd);
			// ƒCƒ“ƒXƒ^ƒ“ƒXI—¹
			bool exit_instance();

			// •`‰æŠJn
			void begin_draw(HWND hwnd);
			// •`‰æI—¹
			bool end_draw(HWND hwnd);

			// ü‚ğ•`‰æ
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ü‚ğ•`‰æ(•¡”)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ƒxƒWƒF‹Èü‚ğ•`‰æ
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ƒxƒWƒF‹Èü‚ğ•`‰æ(•¡”)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ‹éŒ`‚ğ•`‰æ(ü)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ‹éŒ`‚ğ•`‰æ(“h‚è)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8
			);

			// ‘È‰~‚ğ•`‰æ(ü)(’†S“_w’è)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ‘È‰~‚ğ•`‰æ(ü)(‹éŒ`w’è)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// ‘È‰~‚ğ•`‰æ(“h‚è)(’†S“_w’è)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_I8& color_i8
			);

			// ‘È‰~‚ğ•`‰æ(“h‚è)(‹éŒ`w’è)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_I8& color_i8
			);
		};
	}
}