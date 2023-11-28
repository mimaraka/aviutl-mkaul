#include "graphics_manager.hpp"
#include "bitmap_gdiplus.hpp"
#include "bitmap_directx.hpp"
#include "path_gdiplus.hpp"
#include "path_directx.hpp"



namespace mkaul {
	namespace graphics {
		// 描画環境の用意
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


		// 描画環境の破棄
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