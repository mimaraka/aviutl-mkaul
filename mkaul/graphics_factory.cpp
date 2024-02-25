#include "graphics_factory.hpp"
#include "bitmap_gdiplus.hpp"
#include "bitmap_directx.hpp"
#include "path_gdiplus.hpp"
#include "path_directx.hpp"



namespace mkaul {
	namespace graphics {
		// 描画環境の用意
		bool Factory::startup(GraphicEngine engine) noexcept {
			bool result = false;

			switch (engine) {
			case GraphicEngine::Gdiplus:
				result = GdiplusGraphics::startup();
				break;

			case GraphicEngine::Directx:
				result = DirectxGraphics::startup();
				break;

			default:
				return false;
			}

			if (result)
				engine_ = engine;

			return result;
		}


		// 描画環境の破棄
		bool Factory::shutdown() noexcept {
			switch (engine_) {
			case GraphicEngine::Gdiplus:
				GdiplusGraphics::shutdown();
				break;

			case GraphicEngine::Directx:
				DirectxGraphics::shutdown();
				break;

			default:
				return false;
			}

			return true;
		}


		std::unique_ptr<Graphics> Factory::create_graphics() noexcept {
			switch (engine_) {
			case GraphicEngine::Gdiplus:
				return std::make_unique<GdiplusGraphics>();

			case GraphicEngine::Directx:
				return std::make_unique<DirectxGraphics>();

			default:
				return nullptr;
			}
		}


		std::unique_ptr<Path> Factory::create_path() noexcept {
			switch (engine_) {
			case GraphicEngine::Gdiplus:
				return std::make_unique<GdiplusPath>();

			case GraphicEngine::Directx:
				return std::make_unique<DirectxPath>();

			default:
				return nullptr;
			}
		}
	}
}