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
			inline static ID2D1Factory* p_factory_;
			inline static IDWriteFactory* p_write_factory_;
			inline static IWICImagingFactory* p_imaging_factory_;
			ID2D1HwndRenderTarget* p_render_target_;
			ID2D1SolidColorBrush* p_brush_;
			PAINTSTRUCT paint_struct_;

			static void apply_stroke_style(
				const Stroke& stroke,
				_Out_ ID2D1StrokeStyle** pp_stroke_style
			) noexcept;
			static bool apply_text_layout(
				const std::string& text,
				const Font& font,
				const D2D1_SIZE_F& target_size,
				const AnchorPosition& anchor_pos,
				_Out_ IDWriteTextLayout** pp_text_layout
			) noexcept;

		public:
			DirectxGraphics() noexcept :
				p_render_target_(nullptr),
				p_brush_(nullptr),
				paint_struct_({ 0 })
			{}

			~DirectxGraphics() noexcept {
				release();
			}

			static bool startup() noexcept;
			static void shutdown() noexcept;

			// ファクトリー・ターゲット取得
			static auto get_factory() noexcept { return p_factory_; }
			static auto get_write_factory() noexcept { return p_write_factory_; }
			static auto get_imaging_factory() noexcept { return p_imaging_factory_; }
			auto get_render_target() const noexcept { return p_render_target_; }

			bool init(HWND hwnd) noexcept override;
			void release() noexcept override;
			bool begin_draw() noexcept override;
			bool end_draw() noexcept override;
			bool resize() noexcept override;

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
				const Point<float>* p_points,
				size_t n_points,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const ColorF& color = 0,
				const Stroke& stroke = Stroke()
			) noexcept override;

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