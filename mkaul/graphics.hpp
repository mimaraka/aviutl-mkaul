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
		// �`�����
		enum class Drawing_Method {
			Null,
			Gdiplus,
			Directx
		};

		inline Drawing_Method g_drawing_method;

		// �`����̗p��
		bool startup(Drawing_Method drawing_method);
		// �`����̔j��
		void shutdown();

		// �O���t�B�b�N�X�쐬
		bool create_graphics();
	}
}