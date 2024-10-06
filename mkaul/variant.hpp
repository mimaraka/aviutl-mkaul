#pragma once

#include <string>
#include <vector>
#include <type_traits>
#include <oaidl.h>



namespace mkaul {
	namespace wv2 {
		namespace detail {
			template<typename> struct is_vector : std::false_type {};
			template<typename T, typename A> struct is_vector<std::vector<T, A>> : std::true_type {};
		}
		class Variant {
			VARIANT variant_;

			template<typename T>
			static VARTYPE get_vt() {
				if constexpr (std::is_enum_v<T>) {
					return get_vt<std::underlying_type_t<T>>();
				}
				else {
					VARENUM ret = VT_UNKNOWN;
					if constexpr (std::is_same_v<T, int8_t>) {
						ret = VT_I1;
					}
					else if constexpr (std::is_same_v<T, int16_t>) {
						ret = VT_I2;
					}
					else if constexpr (std::is_same_v<T, int32_t>) {
						ret = VT_I4;
					}
					else if constexpr (std::is_same_v<T, int64_t>) {
						ret = VT_I8;
					}
					else if constexpr (std::is_same_v<T, uint8_t>) {
						ret = VT_UI1;
					}
					else if constexpr (std::is_same_v<T, uint16_t>) {
						ret = VT_UI2;
					}
					else if constexpr (std::is_same_v<T, uint32_t>) {
						ret = VT_UI4;
					}
					else if constexpr (std::is_same_v<T, uint64_t>) {
						ret = VT_UI8;
					}
					else if constexpr (std::is_same_v<T, bool>) {
						ret = VT_BOOL;
					}
					else if constexpr (std::is_same_v<T, float>) {
						ret = VT_R4;
					}
					else if constexpr (std::is_same_v<T, double>) {
						ret = VT_R8;
					}
					else if constexpr (std::is_same_v<T, std::wstring>) {
						ret = VT_BSTR;
					}
					else if constexpr (std::is_same_v<T, IDispatch*>) {
						ret = VT_DISPATCH;
					}
					return static_cast<VARTYPE>(ret);
				}
			}

		public:
			Variant(VARIANT* variant) : variant_{*variant} {}

			void init() noexcept { ::VariantInit(&variant_); }

			auto& variant() noexcept { return variant_; }

			template<typename T>
			constexpr T get() const noexcept {
				if constexpr (std::is_enum_v<T>) {
					return static_cast<T>(get<std::underlying_type_t<T>>());
				}
				else if constexpr (std::is_same_v<T, int8_t>) {
					return variant_.cVal;
				}
				else if constexpr (std::is_same_v<T, int16_t>) {
					return variant_.iVal;
				}
				else if constexpr (std::is_same_v<T, int32_t>) {
					return variant_.lVal;
				}
				else if constexpr (std::is_same_v<T, int64_t>) {
					return variant_.llVal;
				}
				else if constexpr (std::is_same_v<T, uint8_t>) {
					return variant_.bVal;
				}
				else if constexpr (std::is_same_v<T, uint16_t>) {
					return variant_.uiVal;
				}
				else if constexpr (std::is_same_v<T, uint32_t>) {
					return variant_.ulVal;
				}
				else if constexpr (std::is_same_v<T, uint64_t>) {
					return variant_.ullVal;
				}
				else if constexpr (std::is_same_v<T, float>) {
					return variant_.fltVal;
				}
				else if constexpr (std::is_same_v<T, double>) {
					return variant_.dblVal;
				}
				else if constexpr (std::is_same_v<T, bool>) {
					return variant_.boolVal;
				}
				else if constexpr (std::is_same_v<T, std::wstring>) {
					return std::wstring(variant_.bstrVal);
				}
				else if constexpr (std::is_same_v<T, IDispatch*>) {
					return variant_.pdispVal;
				}
				else {
					static_assert(false, "Unsupported type");
				}
			}

			template<typename T>
			HRESULT set(T val) {
				if constexpr (detail::is_vector<T>::value) {
					using value_type = typename T::value_type;
					auto vt = get_vt<value_type>();
					variant_.vt = VT_ARRAY | vt;
					SAFEARRAY* p_safe_array = ::SafeArrayCreateVector(vt, 0, val.size());
					if (!p_safe_array) return E_OUTOFMEMORY;
					for (size_t i = 0; i < val.size(); i++) {
						auto hr = ::SafeArrayPutElement(p_safe_array, (LONG*)&i, &val[i]);
						if (FAILED(hr)) {
							::SafeArrayDestroy(p_safe_array);
							return hr;
						}
					}
					variant_.parray = p_safe_array;
					return S_OK;
				}
				else {
					variant_.vt = get_vt<T>();
					if constexpr (std::is_enum_v<T>) {
						using underlying_type = std::underlying_type_t<T>;
						return set<underlying_type>(static_cast<underlying_type>(val));
					}
					else {
						if constexpr (std::is_same_v<T, int8_t>) {
							variant_.cVal = val;
						}
						else if constexpr (std::is_same_v<T, int16_t>) {
							variant_.iVal = val;
						}
						else if constexpr (std::is_same_v<T, int32_t>) {
							variant_.lVal = val;
						}
						else if constexpr (std::is_same_v<T, int64_t>) {
							variant_.llVal = val;
						}
						else if constexpr (std::is_same_v<T, uint8_t>) {
							variant_.bVal = val;
						}
						else if constexpr (std::is_same_v<T, uint16_t>) {
							variant_.uiVal = val;
						}
						else if constexpr (std::is_same_v<T, uint32_t>) {
							variant_.ulVal = val;
						}
						else if constexpr (std::is_same_v<T, uint64_t>) {
							variant_.ullVal = val;
						}
						else if constexpr (std::is_same_v<T, float>) {
							variant_.fltVal = val;
						}
						else if constexpr (std::is_same_v<T, double>) {
							variant_.dblVal = val;
						}
						else if constexpr (std::is_same_v<T, bool>) {
							variant_.boolVal = val ? VARIANT_TRUE : VARIANT_FALSE;
						}
						else if constexpr (std::is_same_v<T, std::wstring>) {
							variant_.bstrVal = ::SysAllocString(val.data());
						}
						else if constexpr (std::is_same_v<T, wchar_t*> or std::is_same_v<T, const wchar_t*>) {
							variant_.bstrVal = ::SysAllocString(val);
						}
						else if constexpr (std::is_same_v<T, IDispatch*>) {
							variant_.pdispVal = val;
						}
						else if constexpr (std::is_same_v<T, VARIANT>) {
							variant_ = val;
						}
						else {
							static_assert(false, "Unsupported type");
						}
						return S_OK;
					}
				}
			}

			template<typename T>
			HRESULT change_type() {
				return ::VariantChangeType(&variant_, &variant_, VARIANT_ALPHABOOL, get_vt<T>());
			}
		};
	} // namespace wv2
} // namespace mkaul