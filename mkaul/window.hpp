//---------------------------------------------------------------------------------------
//		MKAul (Window)
//		VC++ 2022
//		developed by mimaraka
//----------------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include <commctrl.h>
#include "common.hpp"
#include "rectangle.hpp"

#pragma comment(lib, "comctl32.lib")



namespace mkaul {
	namespace window {
		class Window {
		public:
			static constexpr int WM_REDRAW = WM_APP;
			HWND hwnd;

			Window() :
				hwnd(NULL)
			{}

			virtual bool create(
				HWND					hwnd_parent,
				const std::string&		window_name,
				const std::string&		class_name,
				WNDPROC					wndproc_,
				LONG					window_style,
				LONG					class_style,
				const Rectangle<LONG>&	rect,
				HCURSOR					cursor = ::LoadCursor(NULL, IDC_ARROW),
				LPVOID					lp_param = nullptr
			);

			bool move(const Rectangle<LONG>& rect) const;
			bool redraw() const;
			bool close() const;
			bool show() const;
			bool hide() const;
		};
	}
}