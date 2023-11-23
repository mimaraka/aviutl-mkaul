#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include <winhttp.h>


namespace mkaul {
	namespace http {
		class Client {
		private:
			HINTERNET session_;
			HINTERNET connect_;
			URL_COMPONENTS url_comp_;
			wchar_t host_name_[256];
			wchar_t url_path_[2048];

		public:
			Client() :
				session_{ nullptr },
				connect_{ nullptr },
				url_comp_{ 0 },
				host_name_{},
				url_path_{}
			{}

			bool open(std::string url, std::string user_agent = "UserAgent/1.0");
			void close();
			bool get(std::vector<byte>* stream, LPDWORD p_status_code);
		};
	}
}