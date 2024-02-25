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
			WindowRectangle padding_;
			// 
			void calc_net_rect(WindowRectangle* p_rect_wnd) const noexcept;

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
				const WindowRectangle&	rect = WindowRectangle{},
				const WindowRectangle&	padding = WindowRectangle{},
				HCURSOR					cursor = ::LoadCursorA(NULL, IDC_ARROW),
				LPVOID					lp_param = nullptr
			) noexcept;

			HWND get_hwnd() const noexcept { return hwnd_; }
			void get_rect(WindowRectangle* p_wnd_rect, bool include_padding = false) const noexcept;
			void set_padding(const WindowRectangle& rect) noexcept;
			bool move(const WindowRectangle& rect) const noexcept override;
			bool redraw() const noexcept override;
			bool close() const noexcept { return ::DestroyWindow(hwnd_); }
			bool show() const noexcept override { return ::ShowWindow(hwnd_, SW_SHOW); }
			bool hide() const noexcept override { return ::ShowWindow(hwnd_, SW_HIDE); }
			bool send_command(WPARAM wparam = 0, LPARAM lparam = 0) const noexcept { return ::SendMessageA(hwnd_, WM_COMMAND, wparam, lparam); }
		};
	}
}