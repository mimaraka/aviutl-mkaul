#pragma once

#include "gdiplus_common.hpp"
#include "bitmap_base.hpp"



namespace mkaul::graphics {
	// ビットマップ
	struct GdiplusBitmap : public Bitmap, protected GdiplusBase {
	public:
		using Bitmap::Bitmap;
		~GdiplusBitmap();

		void release() noexcept override;

		size_t width() const override;
		size_t height() const override;
	};
} // namespace mkaul::graphics