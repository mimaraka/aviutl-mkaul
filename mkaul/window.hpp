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
			) noexcept;

			HWND get_hwnd() const noexcept { return hwnd_; }
			bool move(const WindowRectangle& rect) const noexcept override;
			bool redraw() const noexcept override;
			bool close() const noexcept { return ::DestroyWindow(hwnd_); }
			bool show() const noexcept { return ::ShowWindow(hwnd_, SW_SHOW); }
			bool hide() const noexcept { return ::ShowWindow(hwnd_, SW_HIDE); }
		};
	}
}