#include "host_object.hpp"



namespace mkaul {
	namespace wv2 {
		STDMETHODIMP HostObject::GetTypeInfoCount(UINT* p_count) {
			*p_count = 0;
			return S_OK;
		}

		STDMETHODIMP HostObject::GetTypeInfo(UINT, LCID, ITypeInfo** pp_type_info) {
			if (pp_type_info) {
				*pp_type_info = nullptr;
			}
			return S_OK;
		}

		STDMETHODIMP HostObject::GetIDsOfNames(
			REFIID,
			LPOLESTR* array_names,
			UINT n_names,
			LCID,
			DISPID* p_dispid
		) {
			if (n_names != 1) {
				return DISP_E_UNKNOWNNAME;
			}
			auto it = std::find(member_names_.begin(), member_names_.end(), array_names[0]);
			if (it == member_names_.end()) {
				return DISP_E_UNKNOWNNAME;
			}
			*p_dispid = std::distance(member_names_.begin(), it) + DISPID_START;
			return S_OK;
		}

		STDMETHODIMP HostObject::Invoke(
			DISPID dispid_member,
			REFIID,
			LCID,
			WORD flags,
			DISPPARAMS* disp_params,
			VARIANT* p_result,
			EXCEPINFO*,
			UINT*
		) {
			for (const auto& member : member_info_) {
				if (member.id == dispid_member and member.flags == flags) {
					return member.func(disp_params, p_result);
				}
			}
			return DISP_E_MEMBERNOTFOUND;
		}
	}
}