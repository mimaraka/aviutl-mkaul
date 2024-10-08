#pragma once

#include <string>
#include <array>



namespace mkaul {
	// バージョン(数字部分)
	class VersionNumber {
		unsigned major_ = 0u;
		unsigned minor_ = 0u;
		unsigned revision_ = 0u;

	public:
		constexpr VersionNumber(unsigned major = 1u, unsigned minor = 0u, unsigned revision = 0u) :
			major_{ major }, minor_{ minor }, revision_{ revision }
		{}

		constexpr bool operator == (const VersionNumber& ver) const noexcept {
			return (this->major_ == ver.major_) and (this->minor_ == ver.minor_) and (this->revision_ == ver.revision_);
		}

		constexpr bool operator < (const VersionNumber& ver) const {
			if (this->major_ == ver.major_) {
				if (this->minor_ == ver.minor_) {
					return this->revision_ < ver.revision_;
				}
				else return this->minor_ < ver.minor_;
			}
			else return this->major_ < ver.major_;
		}

		constexpr bool operator > (const VersionNumber& ver) const {
			return not (*this < ver) and not (*this == ver);
		}

		constexpr bool operator >= (const VersionNumber& ver) const {
			return not (*this < ver);
		}

		constexpr bool operator <= (const VersionNumber& ver) const {
			return not (*this > ver);
		}

		constexpr bool operator != (const VersionNumber& ver) const {
			return not (*this == ver);
		}

		std::string str() const noexcept;
		bool from_str(const std::string& str) noexcept;
		constexpr auto major() const noexcept { return major_; }
		constexpr auto minor() const noexcept { return minor_; }
		constexpr auto revision() const noexcept { return revision_; }
	};

	class PreviewType {
	public:
		enum class Type : uint32_t {
			Alpha,
			Beta,
			Release
		};

	private:
		Type type_;
		static constexpr std::array<const char*, 2> TYPE_STR_TABLE_{
			"alpha",
			"beta"
		};

		static std::string capitalize_(const char* str) noexcept;

	public:
		constexpr PreviewType(Type type = Type::Release) : type_{type}
		{}

		constexpr bool operator == (const PreviewType& preview_type) const noexcept {
			return this->type_ == preview_type.type_;
		}

		constexpr bool operator < (const PreviewType& preview_type) const noexcept {
			return this->type_ < preview_type.type_;
		}

		constexpr bool operator > (const PreviewType& preview_type) const noexcept {
			return not (*this < preview_type) and not (*this == preview_type);
		}

		constexpr bool operator <= (const PreviewType& preview_type) const noexcept {
			return not (*this > preview_type);
		}

		constexpr bool operator >= (const PreviewType& preview_type) const noexcept {
			return not (*this < preview_type);
		}

		constexpr bool operator != (const PreviewType& preview_type) const {
			return not (*this == preview_type);
		}

		constexpr auto type() const noexcept { return type_; }
		void set_type(Type type) noexcept { type_ = type; }
		constexpr bool is_preview() const noexcept { return type_ != Type::Release; }
		std::string str(bool capitalize = false) const noexcept;
		bool from_str(const std::string& str) noexcept;
	};

	// バージョン
	class Version {
		VersionNumber number_;
		PreviewType preview_type_;
		VersionNumber number_preview_;

	public:
		constexpr Version(
			const VersionNumber& number = VersionNumber{},
			PreviewType preview_type = PreviewType{},
			const VersionNumber& number_preview = VersionNumber{}
		) :
			number_{number},
			preview_type_{preview_type},
			number_preview_{number_preview}
		{}

		constexpr bool operator == (const Version& ver) const noexcept {
			bool tmp = this->number_ == ver.number_ and this->preview_type_ == ver.preview_type_;
			if (this->preview_type_.is_preview()) {
				return tmp and this->number_preview_ == ver.number_preview_;
			}
			else {
				return tmp;
			}
		}

		constexpr bool operator < (const Version& ver) const noexcept {
			if (this->number_ == ver.number_) {
				if (this->preview_type_ == ver.preview_type_) {
					if (!this->preview_type_.is_preview()) {
						return false;
					}
					else return this->number_preview_ < ver.number_preview_;
					
				}
				else return this->preview_type_ < ver.preview_type_;
			}
			else return this->number_ < ver.number_;
		}

		constexpr bool operator > (const Version& ver) const noexcept {
			return not (*this < ver) and not (*this == ver);
		}

		constexpr bool operator <= (const Version& ver) const noexcept {
			return not (*this > ver);
		}

		constexpr bool operator >= (const Version& ver) const noexcept {
			return not (*this < ver);
		}

		constexpr bool operator != (const Version& ver) const noexcept {
			return not (*this == ver);
		}

		std::string str() const noexcept;
		bool from_str(const std::string& ver_str) noexcept;
		constexpr bool is_preview() const noexcept { return preview_type_.is_preview(); }
		constexpr auto major() const noexcept { return number_.major(); }
		constexpr auto minor() const noexcept { return number_.minor(); }
		constexpr auto revision() const noexcept { return number_.revision(); }
		const auto& number() const noexcept { return number_; }
		const auto& preview_type() const noexcept { return preview_type_; }
		const auto& number_preview() const noexcept { return number_preview_; }
	};
}