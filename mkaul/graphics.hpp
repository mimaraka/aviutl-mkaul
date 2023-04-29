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

		inline Drawing_Method g_drawing_method = Drawing_Method::Null;

		// �`����̗p��
		bool startup(Drawing_Method drawing_method);
		// �`����̔j��
		bool shutdown();

		// �O���t�B�b�N�X�쐬
		bool create_graphics(Graphics** pp_graphics);

		// �r�b�g�}�b�v�쐬
		bool create_bitmap(Bitmap** pp_bitmap);
	}
}