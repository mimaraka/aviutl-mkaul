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
			enum class Api {
				Null,
				Gdiplus,
				Directx
			};

			// 描画環境の用意
			static bool startup(Api api_ = Api::Gdiplus);
			// 描画環境の破棄
			static bool shutdown();

			static Api get_api() { return api; }

			// グラフィックス作成
			static bool create_graphics(Graphics** pp_graphics);

			// ビットマップ作成
			static bool create_bitmap(Bitmap** pp_bitmap);

			// ジオメトリ作成
			static bool create_path(Path** pp_path);

		private:
			inline static Api api = Api::Null;
		};
	}
}