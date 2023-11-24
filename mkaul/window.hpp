#pragma once

#define NOMINMAX
#include <vector>
#include <Windows.h>
#include <commctrl.h>
#include "rectangle.hpp"
#include "util.hpp"

#pragma comment(lib, "comctl32.lib")



namespace mkaul {
	namespace window {
		// �q�E�B���h�E���ċA�I�Ɍ���
		void get_all_children(HWND hwnd, std::vector<HWND>* hwnd_list);


		// �E�B���h�E�N���X
		class Window {
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
			bool move(const WindowRectangle& rect) const;
			bool redraw() const;
			bool close() const { return ::DestroyWindow(hwnd_); }
			bool show() const { return ::ShowWindow(hwnd_, SW_SHOW); }
			bool hide() const { return ::ShowWindow(hwnd_, SW_HIDE); }
		};
	}
}