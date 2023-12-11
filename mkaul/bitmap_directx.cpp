#include "bitmap_directx.hpp"
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")



namespace mkaul {
	namespace graphics {
		DirectxBitmap::~DirectxBitmap() {
			release();
		}

		void DirectxBitmap::release() noexcept {
			dx_release(reinterpret_cast<ID2D1Bitmap**>(&data));
		}

		size_t DirectxBitmap::get_width() const {
			return (reinterpret_cast<ID2D1Bitmap*>(data)->GetPixelSize()).width;
		}

		size_t DirectxBitmap::get_height() const {
			return (reinterpret_cast<ID2D1Bitmap*>(data)->GetPixelSize()).height;
		}
	}
}