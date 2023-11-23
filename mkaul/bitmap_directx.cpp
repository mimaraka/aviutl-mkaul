#include "bitmap_directx.hpp"
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")



namespace mkaul {
	namespace graphics {
		void DirectxBitmap::release()
		{
			auto p_bitmap = reinterpret_cast<ID2D1Bitmap*>(data);
			dx_release(&p_bitmap);
		}

		size_t DirectxBitmap::get_width() const
		{
			return (reinterpret_cast<ID2D1Bitmap*>(data)->GetPixelSize()).width;
		}

		size_t DirectxBitmap::get_height() const
		{
			return (reinterpret_cast<ID2D1Bitmap*>(data)->GetPixelSize()).height;
		}
	}
}