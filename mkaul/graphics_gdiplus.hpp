#pragma once

#include "graphics_base.hpp"
#include "gdiplus_common.hpp"
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		// グラフィックス
		class GdiplusGraphics : public Graphics, protected GdiplusBase {
		private:
			inline static ULONG_PTR gdiplus_token_ = NULL;
			std::unique_ptr<Gdiplus::Graphics> p_graphics_buffer_;
			std::unique_ptr<Gdiplus::Bitmap> p_bitmap_buffer_;

		public:
			GdiplusGraphics() :
				p_graphics_buffer_(),
				p_bitmap_buffer_()
			{}

			static bool startup() noexcept;
			static void shutdown() noexcept;

			// Stroke, ColorからGdiplus::Penを生成
			static void get_gdip_pen(
				const ColorF& color,
				const Stroke& stroke,
				_Out_ Gdiplus::Pen* p_pen
			) noexcept;

			// ColorからGdiplus::Brushを生成
			static void get_gdip_brush(
				const ColorF& color,
				_Out_ Gdiplus::SolidBrush* p_brush
			) noexcept;

			// FontからGdiplus::Fontを生成
			static void get_gdip_font(
				const Font& font,
				_Out_ Gdiplus::Font** pp_font
			) noexcept;

			bool init(HWND hwnd) noexcept override;
			void release() noexcept override;
			bool begin_draw() noexcept override;
			bool end_draw() noexcept override;
			bool resize() noexcept override { return true; };

			// 背景を塗りつぶし
			void fill_background(
				const ColorF& color = 0
			) noexcept override;

			// 線を描画
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// 線を描画(複数)
			void draw_lines(
				const Point<float>* points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// ベジェ曲線を描画
			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// ベジェ曲線を描画(複数)
			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// 矩形を描画(線)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// 矩形を描画(塗り)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& color = 0
			) noexcept override;

			// 楕円を描画(線)(中心点指定)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// 楕円を描画(線)(矩形指定)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// 楕円を描画(塗り)(中心点指定)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& color = 0
			) noexcept override;

			// 楕円を描画(塗り)(矩形指定)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& color = 0
			) noexcept override;

			// パスを描画(線)
			void draw_path(
				const Path* p_path,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			// パスを描画(塗り)
			void fill_path(
				const Path* p_path,
				const ColorF& color = 0
			) noexcept override;

			// 空のビットマップを作成
			std::unique_ptr<Bitmap> create_bitmap(
				const Size<unsigned>& size,
				const ColorF& color = 0
			) noexcept override;

			// ファイルからビットマップを作成
			std::unique_ptr<Bitmap> load_bitmap_from_filename(
				const std::filesystem::path& path
			) noexcept override;

			// リソースからビットマップを作成
			std::unique_ptr<Bitmap> load_bitmap_from_resource(
				HINSTANCE hinst,
				const char* res_name,
				const char* res_type = RT_BITMAP
			) noexcept override;

			// ビットマップを描画(アンカーポイント指定)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Point<float>& point,
				const AnchorPosition& anchor_pos = AnchorPosition{},
				float opacity = 1.f
			) noexcept override;

			// ビットマップを描画(矩形指定)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) noexcept override;

			// テキストを描画(アンカーポイント指定)
			void draw_text(
				const std::string& text,
				const Point<float>& point,
				const Font& font = Font{},
				const AnchorPosition& anchor_pos = AnchorPosition{},
				const ColorF& color = 0
			) noexcept override;

			// テキストを描画(矩形指定)
			void draw_text(
				const std::string& text,
				const Rectangle<float>& rect,
				const Font& font = Font{},
				const AnchorPosition& anchor_pos = AnchorPosition{},
				bool fit_size = true,
				const ColorF& color = 0
			) noexcept override;
		};
	}
}