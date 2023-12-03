#include "graphics_factory.hpp"
#include "bitmap_gdiplus.hpp"
#include "bitmap_directx.hpp"
#include "path_gdiplus.hpp"
#include "path_directx.hpp"



namespace mkaul {
	namespace graphics {
		// 描画環境の用意
		bool Factory::startup(GraphicMethod method) noexcept {
			bool result = false;

			switch (method) {
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
				method_ = method;

			return result;
		}


		// 描画環境の破棄
		bool Factory::shutdown() noexcept {
			switch (method_) {
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


		std::unique_ptr<Graphics> Factory::create_graphics() noexcept {
			switch (method_) {
			case GraphicMethod::Gdiplus:
				return std::make_unique<GdiplusGraphics>();

			case GraphicMethod::Directx:
				return std::make_unique<DirectxGraphics>();

			default:
				return nullptr;
			}
		}


		std::unique_ptr<Path> Factory::create_path() noexcept {
			switch (method_) {
			case GraphicMethod::Gdiplus:
				return std::make_unique<GdiplusPath>();

			case GraphicMethod::Directx:
				return std::make_unique<DirectxPath>();

			default:
				return nullptr;
			}
		}
	}
}