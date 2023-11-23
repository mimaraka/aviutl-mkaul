//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics_manager.hpp"



namespace mkaul {
	namespace graphics {
		// •`‰æŠÂ‹«‚Ì—pˆÓ
		bool Manager::startup(GraphicMethod method_)
		{
			bool result = false;

			switch (method_) {
			case GraphicMethod::Gdiplus:
				result = GdiplusGraphics::startup();
				break;

			case GraphicMethod::Directx:
				result = DirectxGraphics::startup();
				break;

			default:
				return false;
			}

			if (result)
				method = method_;

			return result;
		}


		// •`‰æŠÂ‹«‚Ì”jŠü
		bool Manager::shutdown()
		{
			switch (method) {
			case GraphicMethod::Gdiplus:
				GdiplusGraphics::shutdown();
				break;

			case GraphicMethod::Directx:
				DirectxGraphics::shutdown();
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_graphics(Graphics** pp_graphics)
		{
			switch (method) {
			case GraphicMethod::Gdiplus:
				*pp_graphics = new GdiplusGraphics;
				break;

			case GraphicMethod::Directx:
				*pp_graphics = new DirectxGraphics;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_bitmap(Bitmap** pp_bitmap)
		{
			switch (method) {
			case GraphicMethod::Gdiplus:
				*pp_bitmap = new GdiplusBitmap;
				break;

			case GraphicMethod::Directx:
				*pp_bitmap = new DirectxBitmap;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_path(Path** pp_path)
		{
			switch (method) {
			case GraphicMethod::Gdiplus:
				*pp_path = new GdiplusPath;
				break;

			case GraphicMethod::Directx:
				*pp_path = new DirectxPath;
				break;

			default:
				return false;
			}

			return true;
		}
	}
}