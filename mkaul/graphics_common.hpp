//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include "point.hpp"
#include "rectangle.hpp"
#include "color.hpp"



namespace mkaul {
	namespace graphics {
		// ストローク
		struct Stroke {
		public:
			float width;
			enum class Dash_Style {
				Solid,
				Dash,
				Dot,
				Dash_Dot,
				Dash_Dot_Dot,
				Custom
			} dash_style;
			enum class Cap_Style {
				Flat,
				Square,
				Round,
				Triangle
			} start_cap, end_cap, dash_cap;
			enum class Line_Join {
				Miter,
				Bevel,
				Round
			} line_join;
			float* custom_dashes;
			int dashes_count;
			float dash_offset;

			Stroke(
				float width_ = 1.f,
				Dash_Style dash_style_ = Dash_Style::Solid,
				Cap_Style start_cap_ = Cap_Style::Round,
				Cap_Style end_cap_ = Cap_Style::Round,
				Cap_Style dash_cap_ = Cap_Style::Round,
				Line_Join line_join_ = Line_Join::Round,
				float* custom_dashes_ = nullptr,
				int dashes_count_ = 0,
				float dash_offset_ = 0.f
			) :
				width(width_),
				dash_style(dash_style_),
				start_cap(start_cap_),
				end_cap(end_cap_),
				dash_cap(dash_cap_),
				line_join(line_join_),
				custom_dashes(custom_dashes_),
				dashes_count(dashes_count_),
				dash_offset(dash_offset_)
			{}
		};


		// ビットマップ(抽象クラス)
		class Bitmap {
		public:
			void* data;

			// ビットマップを破棄
			virtual void release() = 0;

			// リサイズ
			virtual void resize(size_t width, size_t height) = 0;

			// リソースから読み込み
			virtual bool load_from_resource() = 0;
			// ファイル名から読み込み
			virtual bool load_from_filename() = 0;

			// 幅・高さを取得
			virtual size_t get_width() = 0;
			virtual size_t get_height() = 0;
		};


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


		// グラフィックス抽象クラス
		class Graphics {
		protected:
			HWND hwnd;

		public:
			Graphics():
				hwnd(NULL)
			{}

			virtual bool init(HWND hwnd_) = 0;
			virtual void exit() = 0;

			virtual void begin_draw() = 0;
			virtual bool end_draw() = 0;

			// リサイズ
			virtual void resize() = 0;

			// 線を描画
			virtual void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// 線を描画(複数)
			virtual void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// ベジェ曲線を描画
			virtual void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// ベジェ曲線を描画(複数)
			virtual void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// 矩形を描画(線)
			virtual void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// 矩形を描画(塗り)
			virtual void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x,
				float round_radius_y,
				const Color_F& color_f
			) = 0;

			// 楕円を描画(線)(中心点指定)
			virtual void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// 楕円を描画(線)(矩形指定)
			virtual void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f,
				const Stroke& stroke
			) = 0;

			// 楕円を描画(塗り)(中心点指定)
			virtual void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f
			) = 0;

			// 楕円を描画(塗り)(矩形指定)
			virtual void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f
			) = 0;

			// 空のビットマップを作成
			virtual void create_bitmap(
				Bitmap* pp_bitmap,
				const Size<unsigned>& size,
				Color_F color_f
			) = 0;

			// ファイルからビットマップを作成
			virtual bool load_bitmap_from_filename(
				Bitmap* pp_bitmap,
				const std::filesystem::path& path
			) = 0;

			// リソースからビットマップを作成
			virtual bool load_bitmap_from_resource(
				Bitmap* pp_bitmap,
				const std::wstring& resource_name,
				const std::wstring& resource_type
			) = 0;

			// ビットマップを描画(アンカーポイント指定)
			virtual void draw_bitmap(
				const Bitmap& bitmap,
				const Point<float>& point,
				Anchor_Position anchor_pos,
				float opacity
			) = 0;

			// ビットマップを描画(矩形指定)
			virtual void draw_bitmap(
				const Bitmap& bitmap,
				const Rectangle<float>& rect,
				float opacity
			) = 0;
		};
	}
}