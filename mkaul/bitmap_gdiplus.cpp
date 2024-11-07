#include "bitmap_gdiplus.hpp"
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")



namespace mkaul::graphics {
	GdiplusBitmap::~GdiplusBitmap() {
		release();
	}

	void GdiplusBitmap::release() noexcept {
		gdip_release(reinterpret_cast<Gdiplus::Bitmap**>(&data));
	}

	size_t GdiplusBitmap::width() const {
		return get_data<Gdiplus::Bitmap*>()->GetWidth();
	}


	size_t GdiplusBitmap::height() const {
		return get_data<Gdiplus::Bitmap*>()->GetHeight();
	}
} // namespace mkaul::graphics