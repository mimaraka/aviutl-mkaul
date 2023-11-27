#pragma once

#define NOMINMAX
#include <vector>
#include "rectangle.hpp"
#include "util.hpp"
#include "ui_component.hpp"
#include <Windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")



namespace mkaul {
	namespace ui {
		// 子ウィンドウを再帰的に検索
		void get_all_children(HWND hwnd, std::vector<HWND>* hwnd_list);


		// ウィンドウクラス
		class Window : public Component {
		protected:
			HWND hwnd_;

		public:
			Window() :
				hwnd_(NULL)
			{}

			virtual HWND create(
				HINSTANCE				hinst,
				HWND					hwnd_parent,
				LPCTSTR					window_name,
				LPCTSTR					class_name,
				WNDPROC					wndproc,
				LONG					window_style,
				LONG					class_style,
				const WindowRectangle&	rect,
				HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW),
				LPVOID					lp_param = nullptr
			);

			HWND get_hwnd() { return hwnd_; }
			bool move(const WindowRectangle& rect) const noexcept override;
			bool redraw() const;
			bool close() const { return ::DestroyWindow(hwnd_); }
			bool show() const { return ::ShowWindow(hwnd_, SW_SHOW); }
			bool hide() const { return ::ShowWindow(hwnd_, SW_HIDE); }
		};
	}
}