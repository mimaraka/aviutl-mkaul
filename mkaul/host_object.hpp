#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <Windows.h>
#include <WebView2.h>
#include <wil/com.h>
#include <wrl/implements.h>
#include "disp_params.hpp"



namespace mkaul {
	namespace wv2 {
		class HostObject : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, IDispatch> {
		public:
			enum class DispatchType : WORD {
				Method = DISPATCH_METHOD,
				PropertyGet = DISPATCH_PROPERTYGET,
				PropertyPut = DISPATCH_PROPERTYPUT,
				PropertyPutRef = DISPATCH_PROPERTYPUTREF
			};

		private:
			struct MemberInfo {
				DISPID id;
				WORD flags;
				std::function<HRESULT(DISPPARAMS*, VARIANT*)> func;
			};

			DISPID id_count_ = 0;
			std::vector<MemberInfo> member_info_;
			std::vector<std::wstring> member_names_;
			static constexpr DISPID DISPID_START = 0x400;

			template <int N = 0, typename Head, typename... Tail>
			HRESULT change_type(std::vector<Variant>& args) {
				auto hr = args[N].change_type<Head>();
				if (FAILED(hr)) return hr;
				return change_type<N + 1, Tail...>(args);
			}

			template <int N = 0>
			HRESULT change_type(std::vector<Variant>&) {
				return S_OK;
			}

			template <int N = 0, typename... Args>
			void make_tuple_from_args(std::tuple<Args...>& t, const std::vector<Variant>& args) {
				constexpr size_t n_args = sizeof...(Args);
				if constexpr (0 < n_args) {
					static_assert(0 <= N, "N must be greater than or equal to 0.");
					static_assert(N < n_args, "N must be less than the number of arguments.");
					std::get<N>(t) = args[N].get<std::tuple_element_t<N, std::tuple<Args...>>>();
					if constexpr (N < n_args - 1) {
						make_tuple_from_args<N + 1, Args...>(t, args);
					}
				}
			}

		protected:
			template <typename Ret, typename... Args>
			void register_member(const std::wstring& name, DispatchType dispatch_type, Ret(*func)(Args...)) {
				auto it = std::find(member_names_.begin(), member_names_.end(), name);
				DISPID id;
				if (it != member_names_.end()) {
					id = std::distance(member_names_.begin(), it) + DISPID_START;
				}
				else {
					id = id_count_++ + DISPID_START;
					member_names_.emplace_back(name);
				}
				member_info_.emplace_back(MemberInfo{
					.id = id,
					.flags = static_cast<std::underlying_type_t<DispatchType>>(dispatch_type),
					.func = [this, func](DISPPARAMS* disp_params, VARIANT* p_result) -> HRESULT {
						constexpr size_t n_args = sizeof...(Args);
						DispParams dp{ disp_params };
						Variant result{ p_result };
						if (dp.args.size() != n_args) return DISP_E_BADPARAMCOUNT;
						if (!p_result) return E_POINTER;

						auto hr = change_type<0, Args...>(dp.args);
						if (FAILED(hr)) return hr;
						result.init();
						std::tuple<Args...> tuple_args;
						make_tuple_from_args(tuple_args, dp.args);
						if constexpr (std::is_void_v<Ret>) {
							std::apply(func, tuple_args);
						}
						else {
							Ret ret = std::apply(func, tuple_args);
							result.set(ret);
							*p_result = result.variant();
						}
						return S_OK;
					}
				});
			}

		public:
			HostObject() = default;
			virtual ~HostObject() = default;

			STDMETHODIMP GetTypeInfoCount(UINT* p_count) override;

			STDMETHODIMP GetTypeInfo(UINT, LCID, ITypeInfo** pp_type_info) override;

			STDMETHODIMP GetIDsOfNames(
				REFIID riid,
				LPOLESTR* array_names,
				UINT n_names,
				LCID lcid,
				DISPID* p_dispid
			) override;

			STDMETHODIMP Invoke(
				DISPID dispid_member,
				REFIID riid,
				LCID lcid,
				WORD flags,
				DISPPARAMS* disp_params,
				VARIANT* p_result,
				EXCEPINFO* p_exep_info,
				UINT* p_arg_err
			) override;
		};
	} // namespace wv2
} // namespace mkaul