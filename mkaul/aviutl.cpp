#include "aviutl.hpp"


namespace mkaul {
	namespace aviutl {
		FilterPlugin* get_filterp_by_name(FilterPlugin* fp, const std::string& name, bool by_filter_name) {
			SysInfo si;
			if (!fp->exfunc->get_sys_info(nullptr, &si)) {
				return nullptr;
			}

			for (int i = 0; i < si.filter_n; i++) {
				auto tfp = (AviUtl::FilterPlugin*)fp->exfunc->get_filterp(i);
				if (by_filter_name) {
					if (name == tfp->name) {
						return tfp;
					}
				}
				else {
					if (tfp->dll_hinst == ::GetModuleHandleA(name.c_str())) {
						return tfp;
					}
				}
			}
			return nullptr;
		}
	}
}