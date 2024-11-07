#pragma once

#include <aviutl.hpp>
#include <string>



namespace mkaul::aviutl {
	AviUtl::FilterPlugin* get_fp_by_name(AviUtl::FilterPlugin* fp, const std::string& name, bool by_filter_name = false);
} // namespace mkaul::aviutl