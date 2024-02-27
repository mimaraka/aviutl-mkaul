#pragma once

#include <filesystem>



namespace mkaul {
	bool get_file_names(const std::filesystem::path& directory, std::vector<std::filesystem::path>& file_names, bool recursive = false);
}