#pragma once

#include <vector>
#include "variant.hpp"



namespace mkaul {
	namespace wv2 {
		struct DispParams {
			std::vector<Variant> args;
			std::vector<int32_t> dispid_named_args;

			DispParams(DISPPARAMS* disp_params) {
				for (size_t i = 0; i < disp_params->cArgs; i++) {
					args.emplace_back(Variant{ &disp_params->rgvarg[disp_params->cArgs - i - 1] });
				}
				for (size_t i = 0; i < disp_params->cNamedArgs; i++) {
					dispid_named_args.emplace_back(disp_params->rgdispidNamedArgs[disp_params->cNamedArgs - i - 1]);
				}
			}

			bool to_disp_params(DISPPARAMS* disp_params) {
				if (args.size() != disp_params->cArgs) return false;
				if (dispid_named_args.size() != disp_params->cNamedArgs) return false;
				for (size_t i = 0; i < disp_params->cArgs; i++) {
					disp_params->rgvarg[disp_params->cArgs - i - 1] = args[i].variant();
				}
				for (size_t i = 0; i < disp_params->cNamedArgs; i++) {
					disp_params->rgdispidNamedArgs[disp_params->cNamedArgs - i - 1] = static_cast<DISPID>(dispid_named_args[i]);
				}
				return true;
			}
		};
	} // namespace wv2
} // namespace mkaul