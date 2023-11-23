#include "bitmap_gdiplus.hpp"
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul {
	namespace graphics {
		void GdiplusBitmap::release()
		{
			gdip_release(reinterpret_cast<Gdiplus::Bitmap**>(&data));
		}


		size_t GdiplusBitmap::get_width() const
		{
			return reinterpret_cast<Gdiplus::Bitmap*>(data)->GetWidth();
		}


		size_t GdiplusBitmap::get_height() const
		{
			return reinterpret_cast<Gdiplus::Bitmap*>(data)->GetHeight();
		}
	}
}