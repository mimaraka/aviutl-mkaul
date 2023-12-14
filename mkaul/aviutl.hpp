#pragma once

#include <aviutl.hpp>
#include <string>



namespace mkaul {
	namespace aviutl {
		using namespace AviUtl;

		FilterPlugin* get_filterp_by_name(FilterPlugin* fp, const std::string& name, bool by_filter_name=false);
	}
}