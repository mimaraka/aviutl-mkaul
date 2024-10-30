#pragma once

#include <string>
#include <vector>
#include <type_traits>
#include <oaidl.h>
#include <strconv2.h>



namespace mkaul {
	namespace wv2 {
		namespace detail {
			template<typename> struct is_vector : std::false_type {};
			template<typename T, typename A> struct is_vector<std::vector<T, A>> : std::true_type {};
		} // namespace detail

		class Variant {
			VARIANT variant_;

			static void* to_element_ptr(VARIANT& var) noexcept {
				if (var.vt & VT_ARRAY) {
					return &var.parray;
				}
				switch (var.vt) {
				case VT_I1: return &var.cVal;
				case VT_I2: return &var.iVal;
				case VT_I4: return &var.lVal;
				case VT_I8: return &var.llVal;
				case VT_UI1: return &var.bVal;
				case VT_UI2: return &var.uiVal;
				case VT_UI4: return &var.ulVal;
				case VT_UI8: return &var.ullVal;
				case VT_R4: return &var.fltVal;
				case VT_R8: return &var.dblVal;
				case VT_BOOL: return &var.boolVal;
				case VT_BSTR: return &var.bstrVal;
				case VT_DISPATCH: return &var.pdispVal;
				default: return nullptr;
				}
			}

			template<typename T>
			static VARTYPE get_vt() {
				if constexpr (std::is_enum_v<T>) {
					return get_vt<std::underlying_type_t<T>>();
				}
				else {
					VARENUM ret = VT_UNKNOWN;
					if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int8_t>) {
						ret = VT_I1;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int16_t>) {
						ret = VT_I2;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int32_t>) {
						ret = VT_I4;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int64_t>) {
						ret = VT_I8;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint8_t>) {
						ret = VT_UI1;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint16_t>) {
						ret = VT_UI2;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint32_t>) {
						ret = VT_UI4;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint64_t>) {
						ret = VT_UI8;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, bool>) {
						ret = VT_BOOL;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, float>) {
						ret = VT_R4;
					}
					else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, double>) {
						ret = VT_R8;
					}
					else if constexpr (std::is_convertible_v<T, std::string> or std::is_convertible_v<T, std::wstring>) {
						ret = VT_BSTR;
					}
					else if constexpr (std::is_convertible_v<T, IDispatch*>) {
						ret = VT_DISPATCH;
					}
					return static_cast<VARTYPE>(ret);
				}
			}

		public:
			Variant(VARIANT* variant) : variant_{ *variant } {}

			void init() noexcept { ::VariantInit(&variant_); }

			auto& variant() noexcept { return variant_; }

			template<typename T>
			constexpr T get() const noexcept {
				if constexpr (std::is_enum_v<T>) {
					return static_cast<T>(get<std::underlying_type_t<T>>());
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int8_t>) {
					return variant_.cVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int16_t>) {
					return variant_.iVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int32_t>) {
					return variant_.lVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int64_t>) {
					return variant_.llVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint8_t>) {
					return variant_.bVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint16_t>) {
					return variant_.uiVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint32_t>) {
					return variant_.ulVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint64_t>) {
					return variant_.ullVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, float>) {
					return variant_.fltVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, double>) {
					return variant_.dblVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, bool>) {
					return variant_.boolVal;
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, std::wstring>) {
					return std::wstring(variant_.bstrVal);
				}
				else if constexpr (std::is_same_v<std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<T>>>, wchar_t*>) {
					return const_cast<T>(variant_.bstrVal);
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, std::string>) {
					return ::wide_to_sjis(variant_.bstrVal);
				}
				else if constexpr (std::is_same_v<std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<T>>>, char*>) {
					return const_cast<T>(::wide_to_sjis(variant_.bstrVal).c_str());
				}
				else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, IDispatch*>) {
					return variant_.pdispVal;
				}
				else {
					static_assert(false, "Unsupported type");
				}
			}

			template<typename T>
			HRESULT set(T&& val) {
				if constexpr (detail::is_vector<T>::value) {
					using value_type = typename T::value_type;
					auto vt = get_vt<value_type>();
					variant_.vt = VT_ARRAY | vt;
					SAFEARRAY* p_safe_array = ::SafeArrayCreateVector(vt, 0, val.size());
					if (!p_safe_array) return E_OUTOFMEMORY;
					for (size_t i = 0; i < val.size(); i++) {
						Variant tmp;
						auto hr = tmp.set(val[i]);
						if (SUCCEEDED(hr)) {
							hr = ::SafeArrayPutElement(p_safe_array, (LONG*)&i, to_element_ptr(tmp.variant_));
						}
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
						if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int8_t>) {
							variant_.cVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int16_t>) {
							variant_.iVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int32_t>) {
							variant_.lVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, int64_t>) {
							variant_.llVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint8_t>) {
							variant_.bVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint16_t>) {
							variant_.uiVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint32_t>) {
							variant_.ulVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, uint64_t>) {
							variant_.ullVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, float>) {
							variant_.fltVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, double>) {
							variant_.dblVal = val;
						}
						else if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, bool>) {
							variant_.boolVal = val ? VARIANT_TRUE : VARIANT_FALSE;
						}
						else if constexpr (std::is_convertible_v<T, std::wstring>) {
							variant_.bstrVal = ::SysAllocString(std::wstring(val).data());
						}
						else if constexpr (std::is_convertible_v<T, std::string>) {
							variant_.bstrVal = ::SysAllocString(::sjis_to_wide(val).c_str());
						}
						else if constexpr (std::is_convertible_v<T, IDispatch*>) {
							variant_.pdispVal = val;
						}
						else if constexpr (std::is_convertible_v<T, VARIANT>) {
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