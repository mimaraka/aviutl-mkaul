#pragma once

#include "stroke.hpp"
#include "font.hpp"
#include "bitmap_base.hpp"
#include "path_base.hpp"
#include "point.hpp"
#include "size.hpp"
#include "rectangle.hpp"
#include "color.hpp"
#include "util.hpp"
#include <string>
#include <filesystem>
#include <Windows.h>



namespace mkaul {
	namespace graphics {
		// アンカーポイントの位置
		enum class AnchorPosition {
			Center,
			Left,
			Top,
			Right,
			Bottom,
			LeftTop,
			RightTop,
			LeftBottom,
			RightBottom
		};


		// グラフィックス抽象クラス
		class Graphics {
		protected:
			HWND hwnd_;
			bool drawing_;

		public:
			Graphics():
				hwnd_(NULL),
				drawing_(false)
			{}

			virtual bool init(HWND hwnd) = 0;
			virtual void exit() = 0;

			virtual bool begin_draw() = 0;
			virtual bool end_draw() = 0;

			bool is_drawing() { return drawing_; }
			HWND get_hwnd() { return hwnd_; }

			// リサイズ
			virtual bool resize() = 0;

			// 背景を塗りつぶし
			virtual void fill_background(
				const ColorF& color = 0
			) = 0;

			// 線を描画
			virtual void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 線を描画(複数)
			virtual void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ベジェ曲線を描画
			virtual void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ベジェ曲線を描画(複数)
			virtual void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 矩形を描画(線)
			virtual void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 矩形を描画(塗り)
			virtual void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& color = 0
			) = 0;

			// 楕円を描画(線)(中心点指定)
			virtual void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 楕円を描画(線)(矩形指定)
			virtual void draw_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 楕円を描画(塗り)(中心点指定)
			virtual void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& color = 0
			) = 0;

			// 楕円を描画(塗り)(矩形指定)
			virtual void fill_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& color = 0
			) = 0;

			// パスを描画(線)
			virtual void draw_path(
				const Path* p_path,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// パスを描画(塗り)
			virtual void fill_path(
				const Path* p_path,
				const ColorF& color = 0
			) = 0;

			// 空のビットマップを作成
			virtual bool initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				const ColorF& color = 0
			) = 0;

			// ファイルからビットマップを作成
			virtual bool load_bitmap_from_filename(
				Bitmap* p_bitmap,
				const std::filesystem::path& path
			) = 0;

			// リソースからビットマップを作成
			virtual bool load_bitmap_from_resource(
				Bitmap* p_bitmap,
				HINSTANCE hinst,
				const char* res_name,
				const char* res_type = RT_BITMAP
			) = 0;

			// ビットマップを描画(アンカーポイント指定)
			virtual void draw_bitmap(
				const Bitmap* p_bitmap,
				const Point<float>& point,
				AnchorPosition anchor_pos,
				float opacity = 1.f
			) = 0;

			// ビットマップを描画(矩形指定)
			virtual void draw_bitmap(
				const Bitmap* p_bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) = 0;

			// テキストを描画(アンカーポイント指定)
			virtual void draw_text(
				const std::string& text,
				const Point<float>& point,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				const ColorF& color = 0
			) = 0;

			// テキストを描画(矩形指定)
			virtual void draw_text(
				const std::string& text,
				const Rectangle<float>& rect,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				bool fit_size = true,
				const ColorF& color = 0
			) = 0;
		};
	}
}