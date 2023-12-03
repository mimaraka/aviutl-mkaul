#pragma once

#include "gdiplus_common.hpp"
#include "bitmap_base.hpp"



namespace mkaul {
	namespace graphics {
		// ビットマップ
		struct GdiplusBitmap : public Bitmap, protected GdiplusBase {
		public:
			using Bitmap::Bitmap;
			~GdiplusBitmap();

			void release() noexcept override;

			size_t get_width() const override;
			size_t get_height() const override;
		};
	}
}