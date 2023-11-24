#pragma once

#include <type_traits>
#include <concepts>



namespace mkaul {
	namespace flag {
		template<typename T>
		concept c_enum = std::is_enum_v<T>;

		template <c_enum T>
		T operator ! (T x) {
			return static_cast<T>(!static_cast<std::underlying_type_t<T>>(x));
		}

		template <c_enum T>
		T operator | (T x, T y) {
			return static_cast<T>(static_cast<std::underlying_type_t<T>>(x) | static_cast<std::underlying_type_t<T>>(y));
		}

		template <c_enum T>
		T operator & (T x, T y) {
			return static_cast<T>(static_cast<std::underlying_type_t<T>>(x) & static_cast<std::underlying_type_t<T>>(y));
		}

		template <c_enum T>
		T operator ^ (T x, T y) {
			return static_cast<T>(static_cast<std::underlying_type_t<T>>(x) ^ static_cast<std::underlying_type_t<T>>(y));
		}

		template <c_enum T>
		T operator != (T& x, T y) {
			return x = !y;
		}

		template <c_enum T>
		T operator |= (T& x, T y) {
			return x = x | y;
		}

		template <c_enum T>
		T operator &= (T& x, T y) {
			return x = x & y;
		}

		template <c_enum T>
		T operator ^= (T& x, T y) {
			return x = x ^ y;
		}
	}
}