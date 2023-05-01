//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_base.hpp"
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// ビットマップ
		class Bitmap_Gdiplus : public Bitmap {
		public:
			void release() override;

			size_t get_width() override;
			size_t get_height() override;
		};

		// グラフィックス
		class Graphics_Gdiplus : public Graphics {
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
				const Color_F& color_f,
				const Stroke& stroke
			);

			// インスタンス初期化
			bool init(HWND hwnd) override;
			// インスタンス終了
			void exit() override;

			// 描画開始
			void begin_draw() override;
			// 描画終了
			bool end_draw() override;

			void resize() override {};

			// 線を描画
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// 線を描画(複数)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// ベジェ曲線を描画
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// ベジェ曲線を描画(複数)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// 矩形を描画(線)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// 矩形を描画(塗り)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			) override;

			// 楕円を描画(線)(中心点指定)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			) override;

			// 楕円を描画(線)(矩形指定)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_i8,
				const Stroke& stroke
			) override;

			// 楕円を描画(塗り)(中心点指定)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f
			) override;

			// 楕円を描画(塗り)(矩形指定)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			) override;

			// 空のビットマップを作成
			void initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				Color_F color_f
			) override;

			// ファイルからビットマップを作成
			bool load_bitmap_from_filename(
				Bitmap* p_bitmap,
				const std::filesystem::path& path
			) override;

			// リソースからビットマップを作成
			bool load_bitmap_from_resource(
				Bitmap* p_bitmap,
				const char* resource
			) override;

			// ビットマップを描画(アンカーポイント指定)
			void draw_bitmap(
				const Bitmap& bitmap,
				const Point<float>& point,
				Anchor_Position anchor_pos,
				float opacity
			) override;

			// ビットマップを描画(矩形指定)
			void draw_bitmap(
				const Bitmap& bitmap,
				const Rectangle<float>& rect,
				float opacity
			) override;
		};
	}
}