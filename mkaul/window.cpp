//----------------------------------------------------------------------------------
//		MKAul (Window)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#include "window.hpp"



namespace mkaul {
	namespace window {
		// 子ウィンドウを再帰的に検索
		void get_all_children(HWND hwnd, std::vector<HWND>* p_vec_hwnd)
		{
			HWND hwnd_child = ::GetWindow(hwnd, GW_CHILD);
			// 初回ループは子ウィンドウが存在するかを判定(子ウィンドウが存在しなかったらそのまま終了)
			// 2回目以降のループは兄弟ウィンドウが存在する限り続く
			while (hwnd_child) {
				// 再帰呼び出し
				get_all_children(hwnd_child, p_vec_hwnd);
				// 再帰呼び出しが終わったら(=それ以上子ウィンドウが見つからなくなったら)
				// ウィンドウハンドルをリストに追加
				p_vec_hwnd->emplace_back(hwnd_child);
				// 兄弟ウィンドウを検索
				hwnd_child = ::GetWindow(hwnd_child, GW_HWNDNEXT);
			}
		}

		// ウィンドウを作成
		bool Window::create(
			HINSTANCE				hinst,
			HWND					hwnd_parent,
			LPCTSTR					window_name,
			LPCTSTR					class_name,
			WNDPROC					wndproc_,
			LONG					window_style,
			LONG					class_style,
			const Window_Rectangle&	rect,
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
			ws.lpszClassName = class_name;
			ws.hIconSm = NULL;

			if (::RegisterClassEx(&ws)) {
				hwnd = ::CreateWindowEx(
					NULL,
					class_name,
					window_name,
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