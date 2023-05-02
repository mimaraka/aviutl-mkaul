//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "graphics_base.hpp"
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "WindowsCodecs.lib" )



namespace mkaul {
	namespace graphics {
		// ビットマップ
		class Bitmap_Directx : public Bitmap {
		public:
			void release() override;

			size_t get_width() override;
			size_t get_height() override;
		};


		// グラフィック
		class Graphics_Directx : public Graphics {
		private:
			inline static ID2D1Factory* p_factory;
			inline static IDWriteFactory* p_write_factory;
			inline static IWICImagingFactory* p_imaging_factory;
			ID2D1HwndRenderTarget* p_render_target;
			ID2D1SolidColorBrush* p_brush;
			PAINTSTRUCT ps;

			// DirectXオブジェクトの開放
			template <class Interface>
			inline static void release(Interface** pp_object)
			{
				if (*pp_object != nullptr) {
					(*pp_object)->Release();
					(*pp_object) = nullptr;
				}
			}

		public:
			Graphics_Directx() :
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
			static auto get_factory();
			static auto get_write_factory();
			static auto get_imaging_factory();
			auto get_render_target();

			bool init(HWND hwnd_) override;
			void exit() override;
			void begin_draw() override;
			bool end_draw() override;
			void resize() override;

			// 線を描画
			void draw_line(
				const Point<float>& point_from,
				const Point<float>& point_to,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 線を描画(複数)
			void draw_lines(
				const Point<float>* p_points,
				size_t n_points,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			void draw_bezier(
				const Point<float>& point_0,
				const Point<float>& point_1,
				const Point<float>& point_2,
				const Point<float>& point_3,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			void draw_beziers(
				const Point<float>* points,
				size_t n_points,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 矩形を描画(線)
			void draw_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 矩形を描画(塗り)
			void fill_rectangle(
				const Rectangle<float>& rectangle,
				float round_radius_x = 0.f,
				float round_radius_y = 0.f,
				const Color_F& color_f = 0
			) override;

			// 楕円を描画(線)(中心点指定)
			void draw_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 楕円を描画(線)(矩形指定)
			void draw_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f = 0,
				const Stroke& stroke = Stroke()
			) override;

			// 楕円を描画(塗り)(中心点指定)
			void fill_ellipse(
				const Point<float>& point,
				float radius_x,
				float radius_y,
				const Color_F& color_f = 0
			) override;

			// 楕円を描画(塗り)(矩形指定)
			void fill_ellipse(
				const Rectangle<float>& rectangle,
				const Color_F& color_f = 0
			) override;

			// 空のビットマップを作成
			void initialize_bitmap(
				Bitmap* p_bitmap,
				const Size<unsigned>& size,
				Color_F color_f = 0
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
				float opacity = 1.f
			) override;

			// ビットマップを描画(矩形指定)
			void draw_bitmap(
				const Bitmap& bitmap,
				const Rectangle<float>& rect,
				float opacity = 1.f
			) override;
		};
	}
}