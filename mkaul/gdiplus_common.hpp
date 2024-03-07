#pragma once



namespace mkaul {
	namespace graphics {
		// GDI+共通の基底クラス
		class GdiplusBase {
		protected:
			// オブジェクトを開放
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