#pragma once

#include <cmath>
#include "util.hpp"



namespace mkaul {
	template <typename T>
	concept size_lower = requires (T size) {
		size.width; size.height;
	};

	template <typename T>
	concept size_upper = requires (T size) {
		size.Width; size.Height;
	};

	// サイズ
	template <typename T>
	struct Size {
		T width, height;

		// 等号
		inline auto operator == (const Size<T>& size) const noexcept
		{
			return (this->width == size.width and this->height == size.height);
		}

		// 加算 (lower)
		inline auto operator + (const size_lower auto& size) const noexcept
		{
			return Size<T>(
				this->width + (T)size.width,
				this->height + (T)size.height
			);
		}

		// 加算 (upper)
		inline auto operator + (const size_upper auto& size) const noexcept
		{
			return Size<T>(
				this->width + (T)size.Width,
				this->height + (T)size.Height
			);
		}

		// 減算 (lower)
		inline auto operator - (const size_lower auto& size) const noexcept
		{
			return Size<T>(
				this->width - (T)size.width,
				this->height - (T)size.height
			);
		}

		// 減算 (upper)
		inline auto operator - (const size_upper auto& size) const noexcept
		{
			return Size<T>(
				this->width - (T)size.Width,
				this->height - (T)size.Height
			);
		}

		// 代入 (lower)
		inline void operator = (const size_lower auto& size) noexcept
		{
			this->width = (T)size.width;
			this->height = (T)size.height;
		}

		// 代入 (upper)
		inline void operator = (const size_upper auto& size) noexcept
		{
			this->width = (T)size.Width;
			this->height = (T)size.Height;
		}

		inline Size(T width_ = 0, T height_ = 0) :
			width(width_),
			height(height_)
		{}
		
		// 他の型に変換 (lower)
		template <size_lower U>
		auto to() const noexcept
		{
			U size = { 0 };
			size.width = static_cast<decltype(size.width)>(width);
			size.height = static_cast<decltype(size.height)>(height);

			return size;
		}

		// 他の型に変換 (upper)
		template <size_upper U>
		auto to() const noexcept
		{
			U size = { 0 };
			size.Width = static_cast<decltype(size.Width)>(width);
			size.Height = static_cast<decltype(size.Height)>(height);

			return size;
		}

		inline void scale(double scale) noexcept
		{
			width *= std::abs(scale);
			height *= std::abs(scale);
		}
	};
}