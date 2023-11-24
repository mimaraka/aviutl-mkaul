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

	// �T�C�Y
	template <typename T>
	struct Size {
		T width, height;

		// ����
		inline auto operator == (const Size<T>& size) const noexcept
		{
			return (this->width == size.width && this->height == size.height);
		}

		// ���Z (lower)
		inline auto operator + (const size_lower auto& size) const noexcept
		{
			return Size<T>(
				this->width + (T)size.width,
				this->height + (T)size.height
			);
		}

		// ���Z (upper)
		inline auto operator + (const size_upper auto& size) const noexcept
		{
			return Size<T>(
				this->width + (T)size.Width,
				this->height + (T)size.Height
			);
		}

		// ���Z (lower)
		inline auto operator - (const size_lower auto& size) const noexcept
		{
			return Size<T>(
				this->width - (T)size.width,
				this->height - (T)size.height
			);
		}

		// ���Z (upper)
		inline auto operator - (const size_upper auto& size) const noexcept
		{
			return Size<T>(
				this->width - (T)size.Width,
				this->height - (T)size.Height
			);
		}

		// ��� (lower)
		inline void operator = (const size_lower auto& size) noexcept
		{
			this->width = (T)size.width;
			this->height = (T)size.height;
		}

		// ��� (upper)
		inline void operator = (const size_upper auto& size) noexcept
		{
			this->width = (T)size.Width;
			this->height = (T)size.Height;
		}

		inline Size(T width_ = 0, T height_ = 0) :
			width(width_),
			height(height_)
		{}
		
		// ���̌^�ɕϊ� (lower)
		template <size_lower U>
		auto to() const noexcept
		{
			U size = { 0 };
			size.width = static_cast<decltype(size.width)>(width);
			size.height = static_cast<decltype(size.height)>(height);

			return size;
		}

		// ���̌^�ɕϊ� (upper)
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