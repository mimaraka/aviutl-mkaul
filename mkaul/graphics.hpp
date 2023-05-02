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
			// �`�����
			enum class Drawing_Method {
				Null,
				Gdiplus,
				Directx
			};

			// �`����̗p��
			static bool startup(Drawing_Method drawing_method = Drawing_Method::Gdiplus);
			// �`����̔j��
			static bool shutdown();

			// �O���t�B�b�N�X�쐬
			static bool create_graphics(Graphics** pp_graphics);

			// �r�b�g�}�b�v�쐬
			static bool create_bitmap(Bitmap** pp_bitmap);

		private:
			inline static Drawing_Method g_drawing_method = Drawing_Method::Null;
		};
	}
}