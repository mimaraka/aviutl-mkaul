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
			enum class Api {
				Null,
				Gdiplus,
				Directx
			};

			// �`����̗p��
			static bool startup(Api api_ = Api::Gdiplus);
			// �`����̔j��
			static bool shutdown();

			static Api get_api() { return api; }

			// �O���t�B�b�N�X�쐬
			static bool create_graphics(Graphics** pp_graphics);

			// �r�b�g�}�b�v�쐬
			static bool create_bitmap(Bitmap** pp_bitmap);

			// �W�I���g���쐬
			static bool create_path(Path** pp_path);

		private:
			inline static Api api = Api::Null;
		};
	}
}