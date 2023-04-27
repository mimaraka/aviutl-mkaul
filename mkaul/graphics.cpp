//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics.hpp"



namespace mkaul {
	namespace graphics {
		bool startup(Drawing_Method drawing_method)
		{
			g_drawing_method = drawing_method;
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

			return result;
		}


		bool create_graphics()
		{

		}
	}
}