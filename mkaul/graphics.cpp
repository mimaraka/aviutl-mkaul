//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics.hpp"



namespace mkaul {
	namespace graphics {
		// •`‰æŠÂ‹«‚Ì—pˆÓ
		bool Manager::startup(Api api_)
		{
			bool result = false;

			switch (api_) {
			case Api::Gdiplus:
				result = Graphics_Gdiplus::startup();
				break;

			case Api::Directx:
				result = Graphics_Directx::startup();
				break;

			default:
				return false;
			}

			api = api_;

			return result;
		}


		// •`‰æŠÂ‹«‚Ì”jŠü
		bool Manager::shutdown()
		{
			switch (api) {
			case Api::Gdiplus:
				Graphics_Gdiplus::shutdown();
				break;

			case Api::Directx:
				Graphics_Directx::shutdown();
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_graphics(Graphics** pp_graphics)
		{
			switch (api) {
			case Api::Gdiplus:
				*pp_graphics = new Graphics_Gdiplus;
				break;

			case Api::Directx:
				*pp_graphics = new Graphics_Directx;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_bitmap(Bitmap** pp_bitmap)
		{
			switch (api) {
			case Api::Gdiplus:
				*pp_bitmap = new Bitmap_Gdiplus;
				break;

			case Api::Directx:
				*pp_bitmap = new Bitmap_Directx;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_geometry(Geometry** pp_geometry)
		{
			switch (api) {
			case Api::Gdiplus:
				*pp_geometry = new Geometry_Gdiplus;
				break;

			case Api::Directx:
				*pp_geometry = new Geometry_Directx;
				break;

			default:
				return false;
			}

			return true;
		}
	}
}