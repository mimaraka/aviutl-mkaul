#include <vector>
#include "window.hpp"



namespace mkaul {
	namespace ui {
		void Window::calc_net_rect(WindowRectangle* p_rect_wnd) const noexcept {
			p_rect_wnd->left += padding_.left;
			p_rect_wnd->top += padding_.top;
			p_rect_wnd->right -= padding_.right;
			p_rect_wnd->bottom -= padding_.bottom;
			if (p_rect_wnd->right < p_rect_wnd->left) {
				p_rect_wnd->left = p_rect_wnd->right = (p_rect_wnd->left + p_rect_wnd->right) / 2;
			}
			if (p_rect_wnd->bottom < p_rect_wnd->top) {
				p_rect_wnd->top = p_rect_wnd->bottom = (p_rect_wnd->top + p_rect_wnd->bottom) / 2;
			}
		}

		void Window::get_rect(WindowRectangle* p_wnd_rect, bool include_padding) const noexcept {
			RECT tmp;
			::GetClientRect(hwnd_, &tmp);
			if (include_padding) {
				tmp.left += padding_.left;
				tmp.top += padding_.top;
				tmp.right += padding_.right;
				tmp.bottom += padding_.bottom;
			}
			*p_wnd_rect = tmp;
		}

		void Window::set_padding(const WindowRectangle& rect) noexcept {
			padding_ = rect;
		}

		// ウィンドウを移動
		bool Window::move(const WindowRectangle& rect) const noexcept {
			WindowRectangle tmp{rect};
			calc_net_rect(&tmp);
			return ::MoveWindow(
				hwnd_,
				std::min(tmp.left, tmp.right),
				std::min(tmp.top, tmp.bottom),
				tmp.get_width(),
				tmp.get_height(),
				TRUE
			);
		}

		// 再描画
		bool Window::redraw() const noexcept {
			if (hwnd_) {
				std::vector<HWND> hwnd_children;
				get_all_children(hwnd_, &hwnd_children);
				// 自身を再描画
				::InvalidateRect(hwnd_, NULL, FALSE);

				// 下層のウィンドウを全て再描画
				for (auto hwnd : hwnd_children) {
					::InvalidateRect(hwnd, NULL, FALSE);
				}

				return true;
			}
			else return false;
		}

		// 子ウィンドウを再帰的に検索
		void get_all_children(HWND hwnd, std::vector<HWND>* p_vec_hwnd) {
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
			const WindowRectangle&	padding,
			HCURSOR					cursor,
			LPVOID					lp_param
		)  noexcept {
			padding_ = padding;
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