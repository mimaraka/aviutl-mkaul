#pragma once

#include "directx_common.hpp"
#include "bitmap_base.hpp"



namespace mkaul {
	namespace graphics {
		// ビットマップ
		struct DirectxBitmap : public Bitmap, protected DirectxBase {
		public:
			using Bitmap::Bitmap;
			~DirectxBitmap();

			void release() noexcept override;

			size_t get_width() const override;
			size_t get_height() const override;
		};
	}
}