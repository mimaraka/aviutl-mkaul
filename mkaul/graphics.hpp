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
		// 描画方式
		enum class Drawing_Method {
			Null,
			Gdiplus,
			Directx
		};


		// グラフィック統括クラス
		class Graphics {
		private:
			static Drawing_Method drawing_method;
			Graphics_Gdiplus graphics_gdiplus;
			Graphics_Directx graphics_directx;
			HWND hwnd;

		public:
			enum class Anchor_Position {
				Left,
				Top,
				Right,
				Bottom,
				Left_Top,
				Right_Top,
				Left_Bottom,
				Right_Bottom,
				Center
			};
			// コンストラクタ
			Graphics() :
				graphics_gdiplus(),
				graphics_directx(),
				hwnd(NULL)
			{}

			static bool startup(Drawing_Method drawing_method_);
			static bool shutdown();
			bool init_instance(HWND hwnd_);
			bool exit_instance();

			// 描画開始
			bool begin_draw();
			// 描画終了
			bool end_draw();

			// リサイズ
			void resize();

			// 線を描画
			bool draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// 線を描画(複数)
			bool draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ベジェ曲線を描画
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// ベジェ曲線を描画(複数)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// 矩形を描画(線)
			bool draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// 矩形を描画(塗り)
			bool fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			);

			// 楕円を描画(線)(中心点指定)
			bool draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// 楕円を描画(線)(矩形指定)
			bool draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f,
				const Stroke& stroke
			);

			// 楕円を描画(塗り)(中心点指定)
			bool fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f
			);

			// 楕円を描画(塗り)(矩形指定)
			bool fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			);

			// ビットマップを描画
			bool draw_bitmap(
				const Bitmap& bitmap,
				const Point<float>& point,
				Anchor_Position anchor_pos
			);
		};
	}
}