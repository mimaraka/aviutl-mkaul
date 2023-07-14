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
		class Manager final {
		public:
			// �`�����
			enum class GraphicApi {
				Null,
				Gdiplus,
				Directx
			};

			Manager() = delete;
			~Manager() = delete;

			// �`����̗p��
			static bool startup(GraphicApi api_ = GraphicApi::Gdiplus);
			// �`����̔j��
			static bool shutdown();

			static auto get_api() { return api; }

			// �O���t�B�b�N�X�쐬
			static bool create_graphics(Graphics** pp_graphics);

			// �r�b�g�}�b�v�쐬
			static bool create_bitmap(Bitmap** pp_bitmap);

			// �W�I���g���쐬
			static bool create_path(Path** pp_path);

		private:
			inline static GraphicApi api = GraphicApi::Null;
		};
	}
}