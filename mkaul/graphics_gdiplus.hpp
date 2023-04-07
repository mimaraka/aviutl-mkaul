//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include "graphics_common.hpp"

#pragma comment(lib, "gdiplus.lib")



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

			// Strokeの情報をPenに反映
			static void apply_pen_style(
				Gdiplus::Pen* p_pen,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// インスタンス初期化
			bool init_instance(HWND hwnd);
			// インスタンス終了
			bool exit_instance();

			// 描画開始
			void begin_draw(HWND hwnd);
			// 描画終了
			bool end_draw(HWND hwnd);

			// 線を描画
			void draw_line(
				const Point<float>& pt_from,
				const Point<float>& pt_to,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// 線を描画(複数)
			void draw_lines(
				const Point<float>* p_pt,
				size_t n_points,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// 矩形を描画(線)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8,
				const Stroke& stroke
			);

			// 矩形を描画(塗り)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_I8& color_i8
			);
		};
	}
}