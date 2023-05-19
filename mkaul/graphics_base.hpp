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
#include "utils.hpp"



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
		struct Bitmap {
		public:
			void* data;

			Bitmap() :
				data(nullptr)
			{}

			// ビットマップを破棄
			virtual void release() = 0;

			// 幅・高さを取得
			virtual size_t get_width() = 0;
			virtual size_t get_height() = 0;
		};


		// アンカーポイントの位置
		enum class Anchor_Position {
			Center,
			Left,
			Top,
			Right,
			Bottom,
			Left_Top,
			Right_Top,
			Left_Bottom,
			Right_Bottom
		};


		// パス
		struct Path {
		protected:
			void* data[2];
			Point<float> pt_last;

			void ellipse_pos(
				const Size<float>& size,
				float angle,
				Point<float>* p_pt
			);

		public:
			Path() :
				data(),
				pt_last()
			{}

			enum class Figure_End {
				Open,
				Closed
			};

			enum class Sweep_Direction {
				Counter_Clockwise,
				Clockwise				
			};

			enum class Arc_Size {
				Small,
				Large
			};

			virtual void release() = 0;

			virtual bool begin(const Point<float>& pt) = 0;
			virtual void end(Figure_End fe = Figure_End::Closed) = 0;

			// 弧を追加
			virtual void add_arc(
				const Size<float>& size,
				float angle_start,
				float angle_sweep
			) = 0;

			// 線を追加
			virtual void add_line(
				const Point<float>& pt
			) = 0;

			// ベジェを追加
			virtual void add_bezier(
				const Point<float>& pt_0,
				const Point<float>& pt_1,
				const Point<float>& pt_2
			) = 0;
		};


		// 角度angleにある楕円上の点
		// 角度は時計の12時の方向を0dとした時計回りの度数
		inline void Path::ellipse_pos(
			const Size<float>& size,
			float angle,
			Point<float>* p_pt
		)
		{
			float rad = deg2rad(-angle + 90.f);
			float x = size.width * size.height / std::sqrt(size.height * size.height + size.width * size.width * (float)std::pow(std::tan(rad), 2)) * (float)sign(std::cos(rad));
			float y = -x * std::tan(rad);

			p_pt->x = x;
			p_pt->y = y;
		}


		// グラフィックス抽象クラス
		class Graphics {
		protected:
			HWND hwnd;
			bool drawing;

		public:
			Graphics():
				hwnd(NULL),
				drawing(false)
			{}

			virtual bool init(HWND hwnd_) = 0;
			virtual void exit() = 0;

			virtual bool begin_draw() = 0;
			virtual bool end_draw() = 0;

			bool is_drawing() { return drawing; }

			// リサイズ
			virtual bool resize() = 0;

			// 背景を塗りつぶし
			virtual void fill_background(
				const Color_F& col_f = 0
			) = 0;

			// 線を描画
			virtual void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 線を描画(複数)
			virtual void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ベジェ曲線を描画
			virtual void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// ベジェ曲線を描画(複数)
			virtual void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 矩形を描画(線)
			virtual void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 矩形を描画(塗り)
			virtual void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& col_f = 0
			) = 0;

			// 楕円を描画(線)(中心点指定)
			virtual void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 楕円を描画(線)(矩形指定)
			virtual void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& col_f = 0,
				const Stroke& stroke = Stroke()
			) = 0;

			// 楕円を描画(塗り)(中心点指定)
			virtual void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& col_f = 0
			) = 0;

			// 楕円を描画(塗り)(矩形指定)
			virtual void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& col_f = 0
			) = 0;

			// 空のビットマップを作成
			virtual bool initialize_bitmap(
				Bitmap* pp_bitmap,
				const Size<unsigned>& size,
				Color_F col_f = 0
			) = 0;

			// ファイルからビットマップを作成
			virtual bool load_bitmap_from_filename(
				Bitmap* pp_bitmap,
				const std::filesystem::path& path
			) = 0;

			// リソースからビットマップを作成
			virtual bool load_bitmap_from_resource(
				Bitmap* pp_bitmap,
				HINSTANCE hinst,
				const char* res_name,
				const char* res_type = RT_BITMAP
			) = 0;

			// ビットマップを描画(アンカーポイント指定)
			virtual void draw_bitmap(
				const Bitmap* bitmap,
				const Point<float>& point,
				Anchor_Position anchor_pos,
				float opacity = 1.f
			) = 0;

			// ビットマップを描画(矩形指定)
			virtual void draw_bitmap(
				const Bitmap* bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) = 0;
		};
	}
}