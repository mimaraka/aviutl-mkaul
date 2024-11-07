#pragma once

#include "directx_common.hpp"
#include "bitmap_base.hpp"



namespace mkaul::graphics {
	// ビットマップ
	struct DirectxBitmap : public Bitmap, protected DirectxBase {
	public:
		using Bitmap::Bitmap;
		~DirectxBitmap();

		void release() noexcept override;

		size_t width() const override;
		size_t height() const override;
	};
} // namespace mkaul::graphics