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
			enum class GraphicMethod {
				Null,
				Gdiplus,
				Directx
			};

			Manager() = delete;
			~Manager() = delete;

			// �`����̗p��
			static bool startup(GraphicMethod method_ = GraphicMethod::Gdiplus);
			// �`����̔j��
			static bool shutdown();

			static auto get_method() { return method; }

			// �O���t�B�b�N�X�쐬
			static bool create_graphics(Graphics** pp_graphics);

			// �r�b�g�}�b�v�쐬
			static bool create_bitmap(Bitmap** pp_bitmap);

			// �W�I���g���쐬
			static bool create_path(Path** pp_path);

		private:
			inline static GraphicMethod method = GraphicMethod::Null;
		};
	}
}