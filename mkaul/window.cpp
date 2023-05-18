//----------------------------------------------------------------------------------
//		MKAul (Window)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "window.hpp"



namespace mkaul {
	namespace window {
		// ウィンドウを作成
		bool Window::create(
			HINSTANCE				hinst,
			HWND					hwnd_parent,
			const std::string&		window_name,
			const std::string&		class_name,
			WNDPROC					wndproc_,
			LONG					window_style,
			LONG					class_style,
			const Rectangle<LONG>&	rect,
			HCURSOR					cursor,
			LPVOID					lp_param
		)
		{
			WNDCLASSEX ws;
			ws.cbSize = sizeof(ws);
			ws.style = CS_HREDRAW | CS_VREDRAW | class_style;
			ws.lpfnWndProc = wndproc_;
			ws.cbClsExtra = 0;
			ws.cbWndExtra = 0;
			ws.hInstance = hinst;
			ws.hIcon = NULL;
			ws.hCursor = cursor;
			ws.hbrBackground = NULL;
			ws.lpszMenuName = NULL;
			ws.lpszClassName = class_name.c_str();
			ws.hIconSm = NULL;

			if (::RegisterClassEx(&ws)) {
				hwnd = ::CreateWindowEx(
					NULL,
					class_name.c_str(),
					window_name.c_str(),
					WS_VISIBLE | WS_CLIPCHILDREN | window_style,
					rect.left,
					rect.top,
					rect.right - rect.left,
					rect.bottom - rect.top,
					hwnd_parent,
					NULL,
					hinst,
					lp_param
				);
				if (hwnd) return true;
			}
			return false;
		}
	}
}