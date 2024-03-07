#pragma once



namespace mkaul {
	namespace graphics {
		// DirectX共通の基底クラス
		class DirectxBase {
		protected:
			// DirectXオブジェクトの開放
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