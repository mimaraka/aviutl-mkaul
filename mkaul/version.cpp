#include "version.hpp"
#include <regex>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>



namespace mkaul {
	std::string VersionNumber::str() const noexcept {
		std::string result = std::to_string(major_) + "." + std::to_string(minor_);
		if (revision_) {
			result += "." + std::to_string(revision_);
		}
		return result;
	}

	bool VersionNumber::from_str(const std::string& str) noexcept {
		const std::regex regex_number{ R"(^\d+\.\d+(\.\d+)?$)" };

		if (std::regex_match(str, regex_number)) {
			std::vector<std::string> tmp;
			boost::algorithm::split(tmp, str, boost::is_any_of("."));
			major_ = std::stoi(tmp[0]);
			minor_ = std::stoi(tmp[1]);
			if (2 < tmp.size()) {
				revision_ = std::stoi(tmp[2]);
			}
			return true;
		}
		return false;
	}

	std::string PreviewType::capitalize_(const char* str) noexcept {
		std::string tmp{ str };
		if (tmp.size() == 0) return "";
		tmp[0] = std::toupper(tmp[0]);
		return tmp;
	}

	std::string PreviewType::str(bool capitalize) const noexcept {
		if ((uint32_t)Type::Release <= (uint32_t)type_) {
			return "";
		}
		else {
			if (capitalize) {
				return capitalize_(TYPE_STR_TABLE_[(uint32_t)type_]);
			}
			else {
				return TYPE_STR_TABLE_[(uint32_t)type_];
			}
		}
	}

	bool PreviewType::from_str(const std::string& str) noexcept {
		for (size_t i = 0u; i < TYPE_STR_TABLE_.size(); i++) {
			if (str == TYPE_STR_TABLE_[i]) {
				type_ = (Type)i;
				return true;
			}
		}
		return false;
	}

	std::string Version::str() const noexcept {
		return "v" + number_.str() + "-" + preview_type_.str() + number_preview_.str();
	}

	bool Version::from_str(const std::string& ver_str) noexcept {
		const std::regex regex_version{ R"(^v(\d+\.\d+(?:\.\d+)?)(?:-(alpha|beta)(\d+\.\d+(?:\.\d+)?))?$)" };
		std::smatch match;

		// バージョンの正規表現にマッチした場合
		if (std::regex_match(ver_str, match, regex_version)) {
			if (3 < match.size()) {
				number_.from_str(match[1]);
				preview_type_.from_str(match[2]);
				number_preview_.from_str(match[3]);
			}
			else {
				number_.from_str(match[1]);
				preview_type_.set_type(PreviewType::Type::Release);
			}
			return true;
		}
		else return false;
	}
}