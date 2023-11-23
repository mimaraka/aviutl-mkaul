#pragma once

#include "graphics_base.hpp"
#include "directx_common.hpp"
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "WindowsCodecs.lib" )



namespace mkaul {
	namespace graphics {
		// グラフィック
		class DirectxGraphics : public Graphics, protected DirectxBase {
		private:
			inline static ID2D1Factory* p_factory;
			inline static IDWriteFactory* p_write_factory;
			inline static IWICImagingFactory* p_imaging_factory;
			ID2D1HwndRenderTarget* p_render_target;
			ID2D1SolidColorBrush* p_brush;
			PAINTSTRUCT ps;

		public:
			DirectxGraphics() :
				p_render_target(nullptr),
				p_brush(nullptr),
				ps({ 0 })
			{}

			// 描画環境の用意
			static bool startup();
			// 描画環境の破棄
			static void shutdown();

			static void stroke_to_d2d_strokestyle(
				const Stroke& stroke,
				ID2D1StrokeStyle** pp_stroke_style
			);

			// ファクトリー・ターゲット取得
			static auto get_factory() { return p_factory; }
			static auto get_write_factory() { return p_write_factory; }
			static auto get_imaging_factory() { return p_imaging_factory; }
			auto get_render_target() { return p_render_target; }

			bool init(HWND hwnd_) override;
			void exit() override;
			bool begin_draw() override;
			bool end_draw() override;
			bool resize() override;

			// 背景を塗りつぶし
			void fill_background(
				const ColorF& col_f = 0
			) override;

			// 線を描画
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 線を描画(複数)
			void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 矩形を描画(線)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 矩形を描画(塗り)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const ColorF& col_f = 0
			) override;

			// 楕円を描画(線)(中心点指定)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 楕円を描画(線)(矩形指定)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 楕円を描画(塗り)(中心点指定)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const ColorF& col_f = 0
			) override;

			// 楕円を描画(塗り)(矩形指定)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const ColorF& col_f = 0
			) override;

			// パスを描画(線)
			void draw_path(
				const Path* p_path,
				const ColorF& col_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// パスを描画(塗り)
			void fill_path(
				const Path* p_path,
				const ColorF& col_f = 0
			) override;

			// 空のビットマップを作成
			bool initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				const ColorF& col_f = 0
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
				AnchorPosition anchor_pos,
				float opacity = 1.f
			) override;

			// ビットマップを描画(矩形指定)
			void draw_bitmap(
				const Bitmap* bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) override;

			// テキストを描画(アンカーポイント指定)
			void draw_text(
				const std::string& text,
				const Point<float>& point,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				const ColorF& col_f = 0
			) override;

			// テキストを描画(矩形指定)
			void draw_text(
				const std::string& text,
				const Rectangle<float>& rect,
				const Font& font = Font{},
				AnchorPosition anchor_pos = AnchorPosition::Center,
				bool fit_size = true,
				const ColorF& col_f = 0
			) override;
		};
	}
}