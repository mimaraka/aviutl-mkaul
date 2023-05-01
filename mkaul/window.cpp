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
			HWND					hwnd_parent,
			const std::string&		window_name,
			const std::string&		class_name,
			WNDPROC					wndproc_,
			LONG					window_style,
			LONG					class_style,
			const Rectangle<LONG>&	rect,
			HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW),
			LPVOID					lp_param = nullptr
		)
		{
			HINSTANCE hinstance = (HINSTANCE)::GetModuleHandle(NULL);

			WNDCLASSEX ws;
			ws.cbSize = sizeof(ws);
			ws.style = CS_HREDRAW | CS_VREDRAW | class_style;
			ws.lpfnWndProc = wndproc_;
			ws.cbClsExtra = 0;
			ws.cbWndExtra = 0;
			ws.hInstance = hinstance;
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
					hinstance,
					lp_param
				);
				if (hwnd) return true;
			}
			return false;
		}

		// ウィンドウを移動
		inline bool Window::move(const Rectangle<LONG>& rect) const
		{
			return ::MoveWindow(
				hwnd,
				(std::min)(rect.left, rect.right),
				(std::min)(rect.top, rect.bottom),
				std::abs(rect.right - rect.left),
				std::abs(rect.bottom - rect.top),
				TRUE
			);
		}

		// 再描画
		inline bool Window::redraw() const
		{
			if (hwnd) {
				::PostMessage(hwnd, WM_REDRAW, 0, 0);
				return true;
			}
			else return false;
		}


		// ウィンドウを表示
		inline bool Window::show() const
		{
			return ::ShowWindow(hwnd, SW_SHOW);
		}

		// ウィンドウを非表示
		inline bool Window::hide() const
		{
			return ::ShowWindow(hwnd, SW_HIDE);
		}

		// ウィンドウを閉じる
		inline bool Window::close() const
		{
			return ::DestroyWindow(hwnd);
		}
	}
}