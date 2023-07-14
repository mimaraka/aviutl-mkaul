//----------------------------------------------------------------------------------
//		MKAul (Graphics)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "graphics_manager.hpp"



namespace mkaul {
	namespace graphics {
		// •`‰æŠÂ‹«‚Ì—pˆÓ
		bool Manager::startup(GraphicApi api_)
		{
			bool result = false;

			switch (api_) {
			case GraphicApi::Gdiplus:
				result = GdiplusGraphics::startup();
				break;

			case GraphicApi::Directx:
				result = DirectxGraphics::startup();
				break;

			default:
				return false;
			}

			if (result)
				api = api_;

			return result;
		}


		// •`‰æŠÂ‹«‚Ì”jŠü
		bool Manager::shutdown()
		{
			switch (api) {
			case GraphicApi::Gdiplus:
				GdiplusGraphics::shutdown();
				break;

			case GraphicApi::Directx:
				DirectxGraphics::shutdown();
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_graphics(Graphics** pp_graphics)
		{
			switch (api) {
			case GraphicApi::Gdiplus:
				*pp_graphics = new GdiplusGraphics;
				break;

			case GraphicApi::Directx:
				*pp_graphics = new DirectxGraphics;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_bitmap(Bitmap** pp_bitmap)
		{
			switch (api) {
			case GraphicApi::Gdiplus:
				*pp_bitmap = new GdiplusBitmap;
				break;

			case GraphicApi::Directx:
				*pp_bitmap = new DirectxBitmap;
				break;

			default:
				return false;
			}

			return true;
		}


		bool Manager::create_path(Path** pp_path)
		{
			switch (api) {
			case GraphicApi::Gdiplus:
				*pp_path = new GdiplusPath;
				break;

			case GraphicApi::Directx:
				*pp_path = new DirectxPath;
				break;

			default:
				return false;
			}

			return true;
		}
	}
}