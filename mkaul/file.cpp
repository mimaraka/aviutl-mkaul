#include "file.hpp"



namespace mkaul {
	bool get_file_names(const std::filesystem::path& directory, std::vector<std::filesystem::path>& file_names, bool recursive) {
		try {
			if (recursive) {
				for (auto& path : std::filesystem::recursive_directory_iterator{ directory }) {
					file_names.push_back(path.path());
				}
			}
			else {
				for (auto& path : std::filesystem::directory_iterator{ directory }) {
					file_names.push_back(path.path());
				}
			}
			return true;
		}
		catch (std::filesystem::filesystem_error&) {
			return false;
		}
	}
}