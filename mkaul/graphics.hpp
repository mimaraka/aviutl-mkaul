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
		class Manager {
		public:
			// 描画方式
			enum class Drawing_Method {
				Null,
				Gdiplus,
				Directx
			};

			// 描画環境の用意
			static bool startup(Drawing_Method drawing_method = Drawing_Method::Gdiplus);
			// 描画環境の破棄
			static bool shutdown();

			// グラフィックス作成
			static bool create_graphics(Graphics** pp_graphics);

			// ビットマップ作成
			static bool create_bitmap(Bitmap** pp_bitmap);

		private:
			inline static Drawing_Method g_drawing_method = Drawing_Method::Null;
		};
	}
}