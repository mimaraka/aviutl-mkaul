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
		// GDI+共通の基底クラス
		class Gdiplus_Base {
		protected:
			// オブジェクトを開放
			template <class Interface>
			static void gdip_release(Interface** pp_obj)
			{
				if (*pp_obj) {
					delete (*pp_obj);
					(*pp_obj) = nullptr;
				}
			}
		};


		// ビットマップ
		struct Bitmap_Gdiplus : public Bitmap, protected Gdiplus_Base {
		public:
			using Bitmap::Bitmap;

			void release() override;

			size_t get_width() override;
			size_t get_height() override;
		};


		// パス
		struct Path_Gdiplus : public Path, protected Gdiplus_Base {
		public:
			using Path::Path;

			void release() override;

			bool begin(const Point<float>& pt) override;
			void end(Figure_End fe = Figure_End::Closed) override;

			// 弧を追加
			void add_arc(
				const Size<float>& size,
				float angle_start,
				float angle_sweep
			) override;

			// 線を追加
			void add_line(
				const Point<float>& pt
			) override;

			// ベジェを追加
			void add_bezier(
				const Point<float>& pt_0,
				const Point<float>& pt_1,
				const Point<float>& pt_2
			) override;
		};


		// グラフィックス
		class Graphics_Gdiplus : public Graphics, protected Gdiplus_Base {
		private:
			inline static ULONG_PTR gdiplus_token = NULL;
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
				const Color_F& col_f,
				const Stroke& stroke
			);

			// インスタンス初期化
			bool init(HWND hwnd) override;
			// インスタンス終了
			void exit() override;

			// 描画開始
			bool begin_draw() override;
			// 描画終了
			bool end_draw() override;

			bool resize() override { return true; };

			// 背景を塗りつぶし
			void fill_background(
				const Color_F& col_f = 0
			) override;

			// 線を描画
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 線を描画(複数)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ベジェ曲線を描画
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// ベジェ曲線を描画(複数)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 矩形を描画(線)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 矩形を描画(塗り)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& col_f = 0
			) override;

			// 楕円を描画(線)(中心点指定)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 楕円を描画(線)(矩形指定)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 楕円を描画(塗り)(中心点指定)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& col_f = 0
			) override;

			// 楕円を描画(塗り)(矩形指定)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& col_f = 0
			) override;

			// 空のビットマップを作成
			bool initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				Color_F col_f = 0
			) override;

			// ファイルからビットマップを作成
			bool load_bitmap_from_filename(
				Bitmap* p_bitmap,
				const std::filesystem::path& path
			) override;

			// リソースからビットマップを作成
			bool load_bitmap_from_resource(
				Bitmap* p_bitmap,
				HINSTANCE hinst,
				const char* res_name,
				const char* res_type = RT_BITMAP
			) override;

			// ビットマップを描画(アンカーポイント指定)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Point<float>& point,
				Anchor_Position anchor_pos,
				float opacity = 1.f
			) override;

			// ビットマップを描画(矩形指定)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) override;
		};
	}
}