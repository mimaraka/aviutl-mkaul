#include <vector>
#include "window.hpp"



namespace mkaul {
	namespace ui {
		// ウィンドウを移動
		bool Window::move(const WindowRectangle& rect) const noexcept
		{
			return ::MoveWindow(
				hwnd_,
				std::min(rect.left, rect.right),
				std::min(rect.top, rect.bottom),
				rect.get_width(),
				rect.get_height(),
				TRUE
			);
		}

		// 再描画
		bool Window::redraw() const
		{
			if (hwnd_) {
				std::vector<HWND> hwnd_list;
				get_all_children(hwnd_, &hwnd_list);
				// 自身を再描画
				::InvalidateRect(hwnd_, NULL, FALSE);

				// 下層のウィンドウを全て再描画
				for (auto hw : hwnd_list)
					::InvalidateRect(hw, NULL, FALSE);

				return true;
			}
			else return false;
		}

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
		HWND Window::create(
			HINSTANCE				hinst,
			HWND					hwnd_parent,
			LPCTSTR					window_name,
			LPCTSTR					class_name,
			WNDPROC					wndproc,
			LONG					window_style,
			LONG					class_style,
			const WindowRectangle&	rect,
			HCURSOR					cursor,
			LPVOID					lp_param
		)
		{
			WNDCLASSEX tmp;
			// 同名クラスが存在しない場合は作成
			if (!::GetClassInfoExA(hinst, class_name, &tmp)) {
				WNDCLASSEX ws;
				ws.cbSize = sizeof(ws);
				ws.style = CS_HREDRAW | CS_VREDRAW | class_style;
				ws.lpfnWndProc = wndproc;
				ws.cbClsExtra = 0;
				ws.cbWndExtra = 0;
				ws.hInstance = hinst;
				ws.hIcon = NULL;
				ws.hCursor = cursor;
				ws.hbrBackground = NULL;
				ws.lpszMenuName = NULL;
				ws.lpszClassName = class_name;
				ws.hIconSm = NULL;

				if (!::RegisterClassExA(&ws)) return NULL;
			}
			
			return hwnd_ = ::CreateWindowExA(
				NULL,
				class_name,
				window_name,
				WS_VISIBLE | WS_CLIPCHILDREN | window_style,
				rect.left,
				rect.top,
				rect.get_width(),
				rect.get_height(),
				hwnd_parent,
				NULL,
				hinst,
				lp_param
			);
		}
	}
}