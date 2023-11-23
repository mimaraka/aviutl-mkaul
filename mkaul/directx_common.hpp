#pragma once



namespace mkaul {
	namespace graphics {
		// DirectX���ʂ̊��N���X
		class DirectxBase {
		protected:
			// DirectX�I�u�W�F�N�g�̊J��
			template <class Interface>
			static void dx_release(Interface** pp_obj)
			{
				if (*pp_obj) {
					(*pp_obj)->Release();
					(*pp_obj) = nullptr;
				}
			}
		};
	}
}