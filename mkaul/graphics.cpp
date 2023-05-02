//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics.hpp"



namespace mkaul {
	namespace graphics {
		// •`‰æŠÂ‹«‚Ì—pˆÓ
		bool Manager::startup(Drawing_Method drawing_method)
		{
			bool result = false;

			switch (drawing_method) {
			case Drawing_Method::Gdiplus:
				result = Graphics_Gdiplus::startup();
				break;

			case Drawing_Method::Directx:
				result = Graphics_Directx::startup();
				break;

			default:
				return false;
			}

			g_drawing_method = drawing_method;

			return result;
		}


		// •`‰æŠÂ‹«‚Ì”jŠü
		bool Manager::shutdown()
		{
			switch (g_drawing_method) {
			case Drawing_Method::Gdiplus:
				Graphics_Gdiplus::shutdown();
				break;

			case Drawing_Method::Directx:
				Graphics_Directx::shutdown();
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_graphics(Graphics** pp_graphics)
		{
			switch (g_drawing_method) {
			case Drawing_Method::Gdiplus:
				*pp_graphics = new Graphics_Gdiplus;
				break;

			case Drawing_Method::Directx:
				*pp_graphics = new Graphics_Directx;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_bitmap(Bitmap** pp_bitmap)
		{
			switch (g_drawing_method) {
			case Drawing_Method::Gdiplus:
				*pp_bitmap = new Bitmap_Gdiplus;
				break;

			case Drawing_Method::Directx:
				*pp_bitmap = new Bitmap_Directx;
				break;

			default:
				return false;
			}

			return true;
		}
	}
}