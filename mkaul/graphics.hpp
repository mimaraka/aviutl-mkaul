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
		// 描画方式
		enum class Drawing_Method {
			Null,
			Gdiplus,
			Directx
		};

		inline Drawing_Method g_drawing_method;

		// 描画環境の用意
		bool startup(Drawing_Method drawing_method);
		// 描画環境の破棄
		void shutdown();

		// グラフィックス作成
		bool create_graphics();
	}
}