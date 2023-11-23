#pragma once



namespace mkaul {
	namespace graphics {
		// GDI+���ʂ̊��N���X
		class GdiplusBase {
		protected:
			// �I�u�W�F�N�g���J��
			template <class Interface>
			static void gdip_release(Interface** pp_obj)
			{
				if (*pp_obj) {
					delete (*pp_obj);
					(*pp_obj) = nullptr;
				}
			}
		};
	}
}