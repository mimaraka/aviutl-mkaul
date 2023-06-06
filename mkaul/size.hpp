//----------------------------------------------------------------------------------
//		MKAul (Size)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include "common.hpp"
#include "util.hpp"



namespace mkaul {
	template <typename T>
	concept SizeType = requires (T size) {
		size.width; size.height;
	};

	// ÉTÉCÉY
	template <typename T>
	struct Size {
		T width, height;

		inline auto operator == (const Size<T>& size) const noexcept
		{
			return (this->width == size.width && this->height == size.height);
		}

		inline auto operator + (const SizeType auto& size) const noexcept
		{
			return Size<T>(
				this->width + (T)size.width,
				this->height + (T)size.height
			);
		}

		inline auto operator - (const SizeType auto& size) const noexcept
		{
			return Size<T>(
				this->width - (T)size.width,
				this->height - (T)size.height
			);
		}

		inline void operator = (const SizeType auto& size) noexcept
		{
			this->width = (T)size.width;
			this->height = (T)size.height;
		}

		inline Size(T width_ = 0, T height_ = 0) :
			width(width_),
			height(height_)
		{}

		template <SizeType U>
		auto to() const noexcept
		{
			U size = { 0 };
			size.width = static_cast<decltype(size.width)>(width);
			size.height = static_cast<decltype(size.height)>(height);

			return size;
		}

		inline void scale(double scale) noexcept
		{
			width *= std::abs(scale);
			height *= std::abs(scale);
		}
	};
}