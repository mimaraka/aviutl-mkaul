#include <Windows.h>
#include <string>
#include <winhttp.h>
#include <strconv2.h>
#include "http.hpp"

#pragma comment (lib, "winhttp.lib")



namespace mkaul {
	namespace http {
		bool Client::open(std::string url, std::string user_agent)
		{
			auto wsurl = ::utf8_to_wide(url);
			url_comp_.dwStructSize = sizeof(URL_COMPONENTS);
			url_comp_.lpszHostName = host_name_;
			url_comp_.dwHostNameLength = sizeof(host_name_) / sizeof(WCHAR);
			url_comp_.lpszUrlPath = url_path_;
			url_comp_.dwUrlPathLength = sizeof(url_path_) / sizeof(WCHAR);

			session_ = ::WinHttpOpen(
				::utf8_to_wide(user_agent).c_str(),
				WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
				WINHTTP_NO_PROXY_NAME,
				WINHTTP_NO_PROXY_BYPASS,
				0
			);

			if (!session_) return false;

			if (!::WinHttpCrackUrl(wsurl.c_str(), wsurl.length(), 0, &url_comp_)) {
				::WinHttpCloseHandle(session_);
				return false;
			}

			connect_ = ::WinHttpConnect(session_, host_name_, url_comp_.nPort, 0);
			if (!connect_) {
				::WinHttpCloseHandle(session_);
				return false;
			}

			return true;
		}

		void Client::close()
		{
			::WinHttpCloseHandle(connect_);
			::WinHttpCloseHandle(session_);
		}

		bool Client::get(std::vector<byte>* stream, LPDWORD p_status_code)
		{
			HINTERNET request;
			wchar_t* header = nullptr;
			bool result = false;
			DWORD size = 0;
			*p_status_code = 0;

			request = ::WinHttpOpenRequest(
				connect_,
				L"GET",
				url_path_,
				NULL,
				WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				(INTERNET_SCHEME_HTTPS == url_comp_.nScheme) ? WINHTTP_FLAG_SECURE : 0
			);
			if (!request) return false;

			result = ::WinHttpSendRequest(
				request,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0,
				WINHTTP_IGNORE_REQUEST_TOTAL_LENGTH, 0
			);
			if (result) {
				result = ::WinHttpReceiveResponse(request, NULL);
			}
			if (result) {
				result = ::WinHttpQueryHeaders(
					request,
					WINHTTP_QUERY_RAW_HEADERS_CRLF,
					WINHTTP_HEADER_NAME_BY_INDEX,
					WINHTTP_NO_OUTPUT_BUFFER, &size,
					WINHTTP_NO_HEADER_INDEX
				);
			}
			if (result or ::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
				header = (wchar_t*)::HeapAlloc(::GetProcessHeap(), 0, size);
			}
			if (header) {
				result = ::WinHttpQueryHeaders(
					request,
					WINHTTP_QUERY_RAW_HEADERS_CRLF,
					WINHTTP_HEADER_NAME_BY_INDEX,
					header, &size, WINHTTP_NO_HEADER_INDEX
				);
				::HeapFree(::GetProcessHeap(), 0, header);
			}
			if (result) {
				size = sizeof(DWORD);
				::WinHttpQueryHeaders(
					request,
					WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
					WINHTTP_HEADER_NAME_BY_INDEX,
					p_status_code,
					&size,
					WINHTTP_NO_HEADER_INDEX
				);
			}
			if (*p_status_code == HTTP_STATUS_OK) {
				// 本文の受信
				stream->clear();
				while (true) {
					size = 0;
					if (!::WinHttpQueryDataAvailable(request, &size)) {
						result = false;
						break;
					}
					if (size == 0) {
						result = true;
						break;
					}
					auto buffer = (byte*)::HeapAlloc(::GetProcessHeap(), 0, size * sizeof(byte));
					if (!buffer) {
						result = false;
						break;
					}
					if (!::WinHttpReadData(request, buffer, size, NULL)) {
						::HeapFree(::GetProcessHeap(), 0, buffer);
						result = false;
						break;
					}
					for (size_t i = 0u; i < size; i++) {
						stream->emplace_back(buffer[i]);
					}
					::HeapFree(::GetProcessHeap(), 0, buffer);
				}
			}
			::WinHttpCloseHandle(request);
			return result;
		}
	}
}